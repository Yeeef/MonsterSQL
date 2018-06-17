
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

    while (TableFile.getNextRecord(Tablerawdata) != -1)
    {

        memcpy(TableName, Tablerawdata, MAX_TABLE_NAME);
        memcpy((&attribute_count), Tablerawdata + MAX_TABLE_NAME, 1);
        string table_name(TableName);
        Table *table = new Table(table_name, attribute_count);
        //读取了tablename attribute count
        Name2Table.insert({table_name, table});
    }

    // index_name table_name, attribute_name
    char Indexrawdata[MAX_TABLE_NAME + MAX_INDEX_NAME + MAX_ATTRIBUTE_NAME];
    char IndexName[MAX_INDEX_NAME];
    char AttributeName[MAX_ATTRIBUTE_NAME];
    while (IndexFile.getNextRecord(Indexrawdata) != -1)
    {
        memcpy(IndexName, Indexrawdata, MAX_INDEX_NAME);
        memcpy(TableName, Indexrawdata + MAX_INDEX_NAME, MAX_TABLE_NAME);
        memcpy(AttributeName, Indexrawdata + MAX_INDEX_NAME + MAX_TABLE_NAME, MAX_ATTRIBUTE_NAME);
        string table_name(TableName);
        string index_name(IndexName);
        string attri_name(AttributeName);
        Index *index = new Index(index_name, table_name, attri_name);
        //建立索引名到索引的映射
        Name2Index.insert({index_name, index});
        //建立表+属性到索引的映射
        Name2Table[table_name]->set_map_Attri2Index(attri_name, index);
    }

    //还要给table把attibute的信息填满
    //一个个去读文件
    for(auto table : Name2Table)
    {

    }
}

CatalogManager::~CatalogManager()
{
    for (auto index : Name2Index)
        delete index.second;
    for (auto table : Name2Table)
        delete table.second;
}

bool CatalogManager::create_table(const string &table_name, const vector<Attribute> &attributes) throw(Error)
{
    //先判断是否是已经存在的table
    auto search_table = Name2Table.find(table_name);
    if (search_table == Name2Table.end())
    {
        Error err("[CatalogManager::create_table]: table already exists");
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
            Error err("[CatalogManager::create_table]: attribute duplicate");
            throw err;
            return false;
        }
        attribute_exist.insert(attribute.get_name());
    }

    //保证有一个primary，这个工作 interpreter做

    //生成一个table类，
    return true;
}
bool CatalogManager::drop_table(const string &table_name) throw(Error)
{
}
bool CatalogManager::get_indices(const string &table_name, vector<string> &indices_name) throw(Error)
{
}

bool CatalogManager::get_table(const string &table_name, Table &table) throw(Error)
{
    bool isTable = false;
    /*
    for (auto Table : Table_set)
    {
        if (table_name == Table.get_table_name())
        {
            // 把Table值穿出去
            table = Table;
            isTable = true;
            break;
        }
    }
    if (isTable == false)
    {
        string err_info = "[CatalogManager::get_table]: cannot find table";
        Error err(err_info);
        throw err;
        return false;
    }
    */

    return true;
}
