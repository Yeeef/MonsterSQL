
#include "catalog.h"
#include <unordered_set>

CatalogManager::CatalogManager()
    : TableFile(FileManager("TableMeta/tables")), IndexFile(FileManager("IndexMeta/indices"))
{
    // 从文件读信息填充我的table-set
    // 这两个是常常“读”的，所以我把它们固定在内存里进行管理，更加方便
    // 先把Table的名字全部读出来
    // tables table_name attribute_count

    //TableFile, IndexFile已经构建完毕，开始利用它们读取信息
    // table: table_name(32)  attribute_count(1)
    // 开始读
    char Tablerawdata[MAX_TABLE_NAME + 1];
    char TableName[MAX_ATTRIBUTE_NAME];
    int attribute_count;
    int addr = 0;
    while ((addr = TableFile.getNextRecord(Tablerawdata)) != -1)
    {

        memcpy(TableName, Tablerawdata, MAX_TABLE_NAME);
        memcpy((&attribute_count), Tablerawdata + MAX_TABLE_NAME, 1);
        string table_name(TableName);
        Table *table = new Table(table_name, attribute_count);
        //读取了tablename attribute count
        Name2Table.insert({table_name, table});
        // 名字到地址的索引
        TableName2Addr.insert({table_name, addr});
    }

    // index_name table_name, attribute_name
    char Indexrawdata[MAX_TABLE_NAME + MAX_INDEX_NAME + MAX_ATTRIBUTE_NAME];
    char IndexName[MAX_INDEX_NAME];
    char AttributeName[MAX_ATTRIBUTE_NAME];
    while ((addr = IndexFile.getNextRecord(Indexrawdata)) != -1)
    {
        memcpy(IndexName, Indexrawdata, MAX_INDEX_NAME);
        memcpy(TableName, Indexrawdata + MAX_INDEX_NAME, MAX_TABLE_NAME);
        memcpy(AttributeName, Indexrawdata + MAX_INDEX_NAME + MAX_TABLE_NAME, MAX_ATTRIBUTE_NAME);
        string table_name;
        Method::rawdata2string(TableName, MAX_TABLE_NAME, table_name);

        string index_name;
        Method::rawdata2string(IndexName, MAX_INDEX_NAME, index_name);

        string attri_name;
        Method::rawdata2string(AttributeName, MAX_ATTRIBUTE_NAME, attri_name);

        Index *index = new Index(index_name, table_name, attri_name);
        //建立索引名到索引的映射
        Name2Index.insert({index_name, index});
        //建立表+属性到索引的映射
        Name2Table.at(table_name)->set_map_Attri2Index(attri_name, index);
        //名字到地址的索引
        IndexName2Addr.insert({index_name, addr});
    }

    //还要给table把attibute的信息填满
    //一个个去读文件 同时更新recordlength
    LoadAttriInfo();
}

CatalogManager::~CatalogManager()
{
    for (auto index : Name2Index)
        delete index.second;
    for (auto table : Name2Table)
        delete table.second;
}

// attri2Index是否要更新？不需要，在create_index集中更新
// 注意两个map都要更新
// 需要添加对 typeVec的更新✅
bool CatalogManager::create_table(const string &table_name, const vector<Attribute> &attributes) throw(Error)
{
    /* 一些错误检查 */
    //先判断是否是已经存在的table
    auto search_table = Name2Table.find(table_name);
    if (search_table != Name2Table.end())
    {
        string err_info = "[CatalogManager::create_table]: '" + table_name + "' already exists";
        Error err(err_info);
        throw err;
        return false;
    }

    // 保证没有重复属性
    unordered_set<string> attribute_exist;

    for (auto attribute : attributes)
    {
        auto search_attribute = attribute_exist.find(attribute.get_name());
        if (search_attribute != attribute_exist.end())
        {
            //重复
            string err_info = "[CatalogManager::create_table]: attribute '" + attribute.get_name() + "' duplicate";
            Error err(err_info);
            throw err;
            return false;
        }
        attribute_exist.insert(attribute.get_name());
    }

    //保证有一个primary，这个工作 interpreter做


    // 保证没有超过32个属性
    int attri_count = attributes.size();
    if (attri_count > 32)
    {
        string err_info = "[CatalogManager::create_table] '" + table_name + "' has too many attributes";
        Error err(err_info);
        throw err;
        return false;
    }
    /* -------------------------------------------- */
    
    //更新Name2Table
    // 初步创建一个table 
    Table *new_table = new Table(table_name, attri_count);
    // record_length是具体一个record的长度，而不是metadata的长度
    // metadata的长度其实都是定长
    // 这一步能否函数化
    int record_length = 0;
    for (auto attri : attributes)
    {
        new_table->addType(attri.get_type());
        string attri_name = attri.get_name();
        new_table->addAttriName(attri_name); 

        record_length += attri.get_length();

        new_table->set_map_Name2Attri(attri_name, attri);
        if (attri.is_primary() == true)
        {
            new_table->set_primary(attri_name);
            // 应该把catalog的index也更新了？不，在catalog::create_index做

        }

        if (attri.is_unique() == true)
            new_table->set_uniqueSet(attri_name);

    }
    // table信息填充完毕
    new_table->set_record_length(record_length);
    Name2Table.insert({table_name, new_table});

    // 更新tables文件
    char rawdata[MAX_TABLE_NAME + 1];
    memcpy(rawdata, table_name.c_str(), MAX_TABLE_NAME);
    memcpy(rawdata + MAX_TABLE_NAME, &attri_count, 1);
    int addr = TableFile.add_record(rawdata);

    TableName2Addr.insert({table_name, addr});

    // 创建tableMeta文件 并填充header, 注意这里的record_length是meta的长度，这是一个定长36
    Method::createFile("TableMeta/" + table_name, MAX_ATTRIBUTE_NAME + 2 + 1 + 1);
    // 更新tableMeta file, 主要是要填入attribute的信息
    FileManager tablemeta("TableMeta/" + table_name);
    char tabledata[MAX_ATTRIBUTE_NAME + 2 + 1 + 1];
    for (auto attri : attributes)
    {
        short type = attri.get_type();
        bool isPrimary = attri.is_primary();
        bool isUnique = attri.is_unique();
        memcpy(tabledata, attri.get_name().c_str(), MAX_ATTRIBUTE_NAME);
        memcpy(tabledata + MAX_ATTRIBUTE_NAME, (&type), 2);
        memcpy(tabledata + MAX_ATTRIBUTE_NAME + 2, &isPrimary, 1);
        memcpy(tabledata + MAX_ATTRIBUTE_NAME + 2 + 1, &isUnique, 1);
        tablemeta.add_record(tabledata);
    }

    return true;
}

/* catalog得把相关table的信息删了(两个map)，同时更新了文件
 * tables / 具体文件都要更新, 具体文件直接删除
 * Index的删除由drop_index来做
 */
bool CatalogManager::drop_table(const string &table_name) throw(Error)
{
    /* 错误检查 */
    //table 是否存在
    auto search = Name2Table.find(table_name);
    if(search == Name2Table.end())
    {
        string err_info = "[CatalogManager::drop_table] '" + table_name +"' doesn't exist";
        Error err(err_info);
        throw err;
        return false;
    }

    /* ------------------------*/
    delete search->second; //把table*的指针删除
    Name2Table.erase(search);


    // 在tables里删除table词条，同时更新map
    int addr = TableName2Addr.at(table_name);
    TableName2Addr.erase(table_name);
    TableFile.delete_record_ByAddr(addr);

    // 删除文件，同时在buffer中把所有对应filename的全部删了
    string file_name = "TableMeta/" + table_name;
    Method::deleteFile(file_name);


}

/*
 * 得到对应table的indices并把它们返回, indices_name就是要被赋值的vector
 */
bool CatalogManager::get_indices(const string &table_name, vector<string> &indices_name) throw(Error)
{   
    /* 错误检查 */
    //不需要检查是否存在这个table，如果我的createtable没问题的话，index肯定至少存在一个
    auto search = Name2Table.find(table_name);
    if(search == Name2Table.end())
    {
        string err_info = "[CatalogManager::get_indices] table '" + table_name + "' doesn't exist";
        Error err(err_info);
        throw err;
    }

    /* ------------------------------ */
    Table * table = Name2Table.at(table_name);
    table->get_indices(indices_name);
    return true;


}

const Table * CatalogManager::get_table(const string &table_name) throw(Error)
{

    auto search = Name2Table.find(table_name);
    if (search == Name2Table.end())
    {
        //没有找到
        string err_info = "[CatalogManager::get_table]: cannot find table: '" + table_name + "'";
        Error err(err_info);
        throw err;
        
    }

    return search->second;
}

/* 这个函数需要做的是
 * 为catalog填充index信息(两个map)，并在IndexMeta/indices中添加记录
 * 为catalog 的 table 的 attri2index 更新
 */
bool CatalogManager::create_index(const string & index_name, 
const string & table_name, const string & attribute_name) throw(Error)
{
    /* 错误检查 */
    auto search = Name2Index.find(index_name);
    if(search != Name2Index.end())
    {
        string err_info = "[CatalogManager::create_index] index '" + index_name + "' already exists";
        Error err(err_info);
        throw err;
    }

    /* -------------------------------------------------- */

    //为catalog填充index信息，并在IndexMeta/indices中添加记录
    Index * index = new Index(index_name, table_name, attribute_name);
    Name2Index.insert({index_name, index});
    char IndexData[MAX_INDEX_NAME + MAX_TABLE_NAME + MAX_ATTRIBUTE_NAME];
    memcpy(IndexData, index_name.c_str(), MAX_INDEX_NAME);
    memcpy(IndexData + MAX_INDEX_NAME, table_name.c_str(), MAX_TABLE_NAME);
    memcpy(IndexData + MAX_INDEX_NAME + MAX_TABLE_NAME, attribute_name.c_str(), MAX_ATTRIBUTE_NAME);
    int addr = IndexFile.add_record(IndexData);
    IndexName2Addr.insert({index_name, addr});

    //为catalog 的 table 的 attri2index 更新
    Name2Table.at(table_name)->set_map_Attri2Index(attribute_name, index);
    return true;


}

/* 
 * 删除catalog中的map;在catalog的两个map均要处理；对应文件要删除一个记录
 * 删除index_manager中的文件
 */
bool CatalogManager::drop_index(const string & index_name) throw(Error)
{
    /* 错误检查 */
    auto search = Name2Index.find(index_name);
    if(search == Name2Index.end())
    {
        string err_info = "[CatalogManager::drop_index] '" + index_name +"' doesn't exist";
        Error err(err_info);
        throw err;
        return false;
    }
    /* ----------------- */
    delete search->second;
    Name2Index.erase(search);

    int addr = IndexName2Addr.at(index_name);
    IndexFile.delete_record_ByAddr(addr);
    IndexName2Addr.erase(index_name);


}

void CatalogManager::LoadAttriInfo()
{

    for (auto tablemap : Name2Table)
    {
        int record_length = 0;
        string table_name = tablemap.first;
        Table * table = tablemap.second;

        FileManager tableMetaFile("TableMeta/" + table_name);
        tableMetaFile.ReNewAllPtr();
        // attribute_name, type, isPrimary, isUnique
        char tableMeta[MAX_ATTRIBUTE_NAME + 2 + 1 + 1];
        char AttriName[MAX_ATTRIBUTE_NAME];
        short type;
        bool isPrimary, isUnique;
        while (tableMetaFile.getNextRecord(tableMeta) != -1)
        {
            memcpy(AttriName, tableMeta, MAX_ATTRIBUTE_NAME);
            memcpy(&type, tableMeta + MAX_ATTRIBUTE_NAME, 2);
            memcpy(&isPrimary, tableMeta + MAX_ATTRIBUTE_NAME + 2, 1);
            memcpy(&isUnique, tableMeta + MAX_ATTRIBUTE_NAME + 2 + 1, 1);

            record_length += Method::getLengthFromType(type);
            table->addType(type);
            string attri_name(AttriName);
            table->addAttriName(attri_name);
            Attribute new_attri(attri_name, type, isPrimary, isUnique);
            // 建立名字到属性的索引

            table->set_map_Name2Attri(attri_name, new_attri);
            if (isPrimary == true)
            {
                table->set_primary_name(attri_name);
                table->set_uniqueSet(attri_name);
            }
            if (isUnique == true)
            {
                table->set_uniqueSet(attri_name);
            }
        }
        
        table->set_record_length(record_length);
    }
}

void CatalogManager::print()
{
    for(auto tablemap : Name2Table)
    {
        Table * table = tablemap.second;
        cout << tablemap.first << " ";
        table->print();
    }
    cout << endl;
    for(auto indexmap : Name2Index)
    {
        cout << indexmap.first << " ";
    }
    cout << endl;
}