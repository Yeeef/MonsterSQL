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
        for (auto Attri : attributes)
        {
            if (Attri.is_unique() == true || Attri.is_primary() == true)
            {
                Method::setIndexFromTableAttri(table_name, Attri.get_name(), index_name);
                create_index(table_name, Attri.get_name(), index_name);
            }
        }

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
 * 先通过 index 来确保唯一性
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
    BufferManager &bufferManager = MiniSQL::get_buffer_manager();
    vector<char *> raw_Vec;
    char *rawdata;

    try
    {
        // 从catalog获得table
        const Table *table = catalogmanager.get_table(table_name);

        // parse data
        rawdata = new char[table->get_record_length()]();
        // judge whether insert_data is a valid input due to the insert_data
        // At the meantime, convert the insert_data into raw_data
        table->isValidInput(insert_data, type, raw_Vec);

        // convert raw_data_Vec to raw_data
        table->rawVec2rawData(raw_Vec, rawdata);

        //我还需要知道插入后的 record_id
        // Pointer 就是我要的信息

        for (int i = 0; i < table->attri_name.size(); i++)
        {
            string index_name;
            if (table->isIndex(table->attri_name.at(i), index_name))
            {
                // 先执行一个find操作
                Attribute attri("", 0, 0, 0);
                table->GetAttriByName(table->attri_name.at(i), attri);
                int Type = attri.get_type();
                int addrInIndex = indexmanager.find(index_name, raw_Vec.at(i), Type);
                if (addrInIndex != -1)
                {
                    cout << "[API::insert] duplicate record" << endl;
                    return false;
                }
            }
            else if (table->isUnique(table->attri_name.at(i)) == true)
            { // 没有index但是却是unique属性
                cout << "Unique but not indexed" << endl;
                char ExistData[table->get_record_length()];
                FileManager filemanager("data/" + table_name);
                while (filemanager.getNextRecord(ExistData) != -1)
                {
                    string duplicate;
                    if (table->CheckConsistency(ExistData, rawdata, duplicate) == false)
                    {
                        cout << "[API::insert] duplicate record on '" + duplicate + "' ";
                        for (auto raw : raw_Vec)
                        {
                            delete[] raw;
                        }
                        delete[] rawdata; //？
                        return false;
                    }
                }
            }
        }

        const int addr = recordmanager.insert(table_name, rawdata);

        for (int i = 0; i < table->attri_name.size(); i++)
        {
            string index_name;
            if (table->isIndex(table->attri_name.at(i), index_name))
            {
                Attribute attri("", 0, 0, 0);
                table->GetAttriByName(table->attri_name.at(i), attri);
                int Type = attri.get_type();
                indexmanager.insert(index_name, raw_Vec.at(i), Type, addr);
            }
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

int API::Delete(const string &table_name, const vector<string> &attribute_name,
                const vector<int> &condition, const vector<string> &operand) const throw(Error)
{
    CatalogManager &catalogmanager = MiniSQL::get_catalog_manager();
    RecordManager &recordmanager = MiniSQL::get_record_manager();
    IndexManager &indexmanager = MiniSQL::get_index_manager();

    try
    {
        /* 错误检查 */
        const Table *table = catalogmanager.get_table(table_name);
        // 检查attri是否都存在
        for (auto attri_name : attribute_name)
        {
            // 这个函数会直接抛出错误
            table->isAttribute(attri_name);
        }

        /* --------------------------------------------------- */

        int size = attribute_name.size();

        /* 根据vector attribute_name 来一个个向下看, 找有没有能用index的 */
        /* 只要找到一个就立即开始做？ */

        for (int i = 0; i < size; i++)
        {
            if (condition.at(i) != COND_EQ)
                continue;

            string index_name;
            if (!table->isIndex(attribute_name.at(i), index_name))
                continue;

            Attribute attri("", -1, 0);
            table->GetAttriByName(attribute_name.at(i), attri);
            // keydata是对应operand的rawdata
            //翻译keydata
            char keydata[attri.get_length()];
            // 比如 a > 1，我现在要把这个1翻译出来
            Method::string2rawdata(operand.at(i), attri.get_type(), keydata);

            //找到绝对地址
            //int addr = indexmanager.remove(index_name, keydata, attri.get_type());
            int addr = indexmanager.find(index_name, keydata, attri.get_type());
            if (addr == -1)
            {
                return 0;
            }
            //调用record manager
            const char *rawdata = recordmanager.GetRecordByAddr(table_name, addr);

            // 判断是否符合所有要求, 如果满足要求，直接删除
            if (table->isSatisfyAllCondition(rawdata, attribute_name, condition, operand) == true)
            {
                // 更新所有索引
                for (auto attribute_index : table->Attri2Index)
                {
                    Attribute attri("", -1, 0);
                    table->GetAttriByName(attribute_index.first, attri);
                    int startPos = table->GetPosByName(attribute_index.first);
                    int addr = indexmanager.remove(attribute_index.second->get_index_name(),
                                                   rawdata + startPos, attri.get_type());

                    if (addr == -1)
                    {
                        cout << "something wrong" << endl;;
                    }
                }

                recordmanager.DeleteRecordByAddr(table_name, addr);
                return 1;
            }

            return 0;
        }

        //利用record暴力搜索，这里就不一定是唯一的了，不需要传入vector, 结果实时打印打印结果最好实时打印还是？
        return recordmanager.Delete(table_name, attribute_name, condition, operand);
    }
    catch (Error err)
    {
        err.print_error();
        return false;
    }
}

/*
 * 检查attribute_name是否都能对上号 ✅
 * 之前的思路有问题：应该是看看有没有能用index，如果能用，那我就不需要recordmanager了，找出来一条，然后判断是否符合所有条件即可 ✅
 * 寻找所有条件里有没有能用index的 ✅
 * 暂时没有让index实现scale find
 * 
 */
int API::select(const string &table_name, const vector<string> &attribute_name,
                const vector<int> &condition, const vector<string> &operand) const throw(Error)
{

    CatalogManager &catalogmanager = MiniSQL::get_catalog_manager();
    RecordManager &recordmanager = MiniSQL::get_record_manager();
    IndexManager &indexmanager = MiniSQL::get_index_manager();

    try
    {
        /* 错误检查 */
        const Table *table = catalogmanager.get_table(table_name);
        // 检查attri是否都存在
        for (auto attri_name : attribute_name)
        {
            // 这个函数会直接抛出错误
            table->isAttribute(attri_name);
        }

        /* --------------------------------------------------- */

        int size = attribute_name.size();

        /* 根据vector attribute_name 来一个个向下看, 找有没有能用index的 */
        /* 只要找到一个就立即开始做？ */

        for (int i = 0; i < size; i++)
        {
            if (condition.at(i) != COND_EQ)
                continue;

            string index_name;
            if (!table->isIndex(attribute_name.at(i), index_name))
                continue;

            Attribute attri("", -1, 0);
            table->GetAttriByName(attribute_name.at(i), attri);
            // keydata是对应operand的rawdata
            //翻译keydata
            char keydata[attri.get_length()];
            // 比如 a > 1，我现在要把这个1翻译出来
            Method::string2rawdata(operand.at(i), attri.get_type(), keydata);

            //找到绝对地址
            int addr = indexmanager.find(index_name, keydata, attri.get_type());
            //调用record manager
            if (addr == -1)
            {
                return false;
            }
            const char *rawdata = recordmanager.GetRecordByAddr(table_name, addr);
            // 判断是否符合所有要求, 如果满足要求，直接打印出来
            if (table->isSatisfyAllCondition(rawdata, attribute_name, condition, operand) == true)
            {
                table->PrintRawdata(rawdata);
                cout << endl;
            }
            return true;
        }

        //利用record暴力搜索，这里就不一定是唯一的了，不需要传入vector, 结果实时打印打印结果最好实时打印还是？
        return recordmanager.select(table_name, attribute_name, condition, operand);
    }
    catch (Error err)
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
        const Table *table = catalogmanager.get_table(table_name);
        int startPos = table->GetPosByName(attribute_name);
        Attribute attri("", 0, 0);
        table->GetAttriByName(attribute_name, attri);
        int datatype = attri.get_type();
        IndexManager &indexManager = MiniSQL::get_index_manager();
        catalogmanager.create_index(index_name, table_name, attribute_name);
        indexManager.createIndex(table_name, attribute_name, index_name, datatype, startPos, false);
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
 * table 中的 map要处理
 */
bool API::drop_index(const string &index_name, const string &table_name) const throw(Error)
{
    try
    {
        CatalogManager &catalogmanager = MiniSQL::get_catalog_manager();
        IndexManager &indexmanager = MiniSQL::get_index_manager();

        catalogmanager.drop_index(index_name, table_name);
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
