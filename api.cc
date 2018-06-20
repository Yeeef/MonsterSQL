#include "minisql.h"
#include "api.h"

bool API::create_table(const string &table_name, const Attribute &primary, const vector<Attribute> &attributes) const throw(Error)
{

    // 获取catalogmanager
    CatalogManager &catalogmanager = MiniSQL::get_catalog_manager();
    // 获取recordmanager
    RecordManager &recordmanager = MiniSQL::get_record_manager();

    try
    {
        catalogmanager.create_table(table_name, attributes);

        recordmanager.create_table(table_name);
        string index_name;
        // 默认的index名 table+primary
        Method::setIndexFromTableAttri(table_name, primary.get_name(), index_name);
        create_index(table_name, primary.get_name(), index_name);

        return true;
    }
    catch (Error err)
    {
        err.print_error();
        return false;
    }
}

/* catalog得把相关table的信息删了，同时更新了文件，还有index的信息(调用drop_index)
 * record得把实际的record全部删除
 * index得把存在的index全部删了(调用drop_index)
 */
bool API::drop_table(const string &table_name) const throw(Error)
{
    // 获取catalogmanager
    CatalogManager &catalogmanager = MiniSQL::get_catalog_manager();
    // 获取recordmanager
    RecordManager &recordmanager = MiniSQL::get_record_manager();

    try
    {
        vector<string> indices_name;
        // get indices from catalog
        catalogmanager.get_indices(table_name, indices_name);
        // drop all indices related to table
        for (auto index : indices_name)
        {
            drop_index(index, table_name);
        }
        catalogmanager.drop_table(table_name);
        recordmanager.drop_table(table_name);

        return true;
    }
    catch (Error err)
    {
        err.print_error();
        return false;
    }
}


/* TODO
 * rawdata 应该是一个 vector <char*> 
 * catalogmanger里面内建一个map访问更快
 * 修改了Table,attributeset那里要修改
 * rawdata到底能否delete？
 * 一定要保证，try catch过程中没有内存泄漏，如果你犯错了，那么你一定要帮我delete
 */
bool API::insert(const string &table_name, const vector<string> &insert_data, const vector<int> &type) const throw(Error)
{

    bool ret;


    // 获取catalogmanager
    CatalogManager &catalogmanager = MiniSQL::get_catalog_manager();
    // 获取recordmanager
    RecordManager &recordmanager = MiniSQL::get_record_manager();
    // 获取 indexmanager
    IndexManager &indexmanager = MiniSQL::get_index_manager();
    vector<char *> raw_Vec;
    char *rawdata;

    try
    {
        // 从catalog获得table
        const Table * table = catalogmanager.get_table(table_name);

        // parse data
        rawdata = new char[table->get_record_length()];
        // judge whether insert_data is a valid input due to the insert_data
        // At the meantime, convert the insert_data into raw_data
        table->isValidInput(insert_data, type, raw_Vec);

        // convert raw_data_Vec to raw_data
        table->rawVec2rawData(raw_Vec, rawdata);

        //我还需要知道插入后的 record_id
        // Pointer 就是我要的信息
        
        int addr = recordmanager.insert(table_name, rawdata);
        ptr Pointer(addr); 

        for (int i = 0; i < table->attri_name.size(); i++)
        {
            string index_name;
            if (table->isIndex(table->attri_name.at(i), index_name))
                indexmanager.insert(index_name, raw_Vec.at(i), type.at(i), Pointer);
        }

        ret = true;
        for (auto raw : raw_Vec)
        {
            delete[] raw;
        }
        delete[] rawdata; //？
    }
    catch (Error err)
    {
        err.print_error();
        ret = false;
    }

    return ret;
}

bool API::Delete(const string &table_name, const vector<string> &attribute_name,
                 const vector<int> &condition, const vector<string> &operand) const throw(Error)
{
    return false;
}

/*
 * 检查attribute_name是否都能对上号 ✅
 * 拿出所有符合条件的record同时利用 unordered_set 进行筛选
 * 针对等于号条件进行筛选
 * keydata没有new
 * 
 */
bool API::select(const string &table_name, const vector<string> & attribute_name,
                 const vector<int> & condition, const vector<string> & operand) const throw(Error)
{
    CatalogManager & catalogmanager = MiniSQL::get_catalog_manager();
    RecordManager & recordmanager = MiniSQL::get_record_manager();
    IndexManager & indexmanager = MiniSQL::get_index_manager();

    try
    {
        const Table * table = catalogmanager.get_table(table_name);
        // 检查attri是否都存在
        for(auto attri_name : attribute_name)
        {
            // 这个函数会直接抛出错误
            table->isAttribute(attri_name);
        }

        int size = attribute_name.size();
        
        unordered_map <int, const char *> addr_record; //通过这个hash来判断是否是出现过的addr_set?
        for(int i = 0; i < size; i++)
        {
            Attribute attri("", -1, 0);
            table->GetAttriByName(attribute_name.at(i), attri);
            // 注意new/delete
            // keydata是对应operand的rawdata
            //翻译keydata
            char keydata[attri.get_length()];
            Method::string2rawdata(operand.at(i), attri.get_type(), keydata);

            if(condition.at(i) == COND_EQ)
            {
                string index_name;
                // 有索引
                if(table->isIndex(attribute_name.at(i), index_name))
                {
                    ptr Pointer;
                    //找到绝对地址
                    indexmanager.find(index_name, keydata, attri.get_type() , Pointer);
                    int addr = Pointer.get_id();  
                    //加入映射中去
                    addr_record.insert({addr, recordmanager.GetRecordByAddr(table_name, addr)}); 
                }
                else
                {
                    //利用record暴力搜索
                    int addr;
                    const char * content = recordmanager.select(table_name, attri.get_name(), COND_EQ, keydata, &addr);
                    addr_record.insert({addr, content});                   
                }
            }
        }

        // 开始把所有记录拿出来





    }
    catch(Error err)
    {
        err.print_error();
        return false;

    }


}

/* 这个函数需要做的是
 * 为catalog::create_index index_manager create_index
 */
bool API::create_index(const string &table_name, const string &attribute_name,
                       const string &index_name) const throw(Error)
{
    try
    {
        CatalogManager &catalogmanager = MiniSQL::get_catalog_manager();
        IndexManager & indexManager = MiniSQL::get_index_manager();
        catalogmanager.create_index(index_name, table_name, attribute_name);
        //indexManager.createIndex(table_name, attribute_name, index_name, )
    }
    catch (Error err)
    {
        err.print_error();
        return false;
    }

    return true;
}

/* 删除catalog中的map;在catalog的两个map均要处理；对应文件要删除一个记录
 * 删除index_manager中的文件
 */
bool API::drop_index(const string &index_name, const string &table_name) const throw(Error)
{
    try
    {
        CatalogManager &catalogmanager = MiniSQL::get_catalog_manager();
        IndexManager &indexmanager = MiniSQL::get_index_manager();

        catalogmanager.drop_index(index_name);
        indexmanager.dropIndex(index_name);
        return true;
    }
    catch (Error err)
    {
        err.print_error();
        return false;
    }

    return true;
}
