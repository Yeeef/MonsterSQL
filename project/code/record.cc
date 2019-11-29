#include "record.h"
#include "minisql.h"

bool RecordManager::create_table(const string &table_name) throw(Error)
{
    CatalogManager &catalogmanager = MiniSQL::get_catalog_manager();

    const Table *table = catalogmanager.get_table(table_name);
    const int record_length = table->get_record_length();
    //这里要创建data区文件
    Method::createFile("data/" + table_name, record_length);
}

/* TODO
 * 删除数据区文件
 */
bool RecordManager::drop_table(const string &table_name) throw(Error)
{
    Method::deleteFile("data/" + table_name);
}

/* 这个函数用来做index无法满足需求的 select, 结果实时打印，方法就是遍历文件，
 * 还需添加对重复记录的筛选？应该没有可能重复，应为我是从上之下遍历的
 * 
 */
int RecordManager::select(const string &table_name, const vector<string> &attribute_name,
                          const vector<int> &condition, const vector<string> &operand)
{
    int NumOfRecordAffected = 0;
    CatalogManager &catalogmanager = MiniSQL::get_catalog_manager();
    const Table *table = catalogmanager.get_table(table_name);

    FileManager filemanager("data/" + table_name);
    int addr;
    char rawdata[table->get_record_length()];
    if (condition.size() == 0)
    {
        while ((addr = filemanager.getNextRecord(rawdata)) != -1)
        {

            table->PrintRawdata(rawdata);
            cout << endl;
            NumOfRecordAffected++;
        }
        return NumOfRecordAffected;
    }
    while ((addr = filemanager.getNextRecord(rawdata)) != -1)
    {
        if (table->isSatisfyAllCondition(rawdata, attribute_name, condition, operand) == true)
        {
            table->PrintRawdata(rawdata);
            cout << endl;
            NumOfRecordAffected++;
        }
    }

    return NumOfRecordAffected;
}

int RecordManager::Delete(const string &table_name, const vector<string> &attribute_name,
                          const vector<int> &condition, const vector<string> &operand)
{

    IndexManager &indexmanager = MiniSQL::get_index_manager();
    int NumOfRecordAffected = 0;
    CatalogManager &catalogmanager = MiniSQL::get_catalog_manager();
    const Table *table = catalogmanager.get_table(table_name);
    vector<Attribute> AttriIndexed;
    table->GetAttriIndexed(AttriIndexed);

    FileManager filemanager("data/" + table_name);
    int addr;
    char rawdata[table->get_record_length()];
    if (condition.size() == 0)
    {
        while ((addr = filemanager.getNextRecord(rawdata)) != -1)
        {
            // 更新所有索引
            for (auto attri_index : table->Attri2Index)
            {
                Attribute attribute("", 0, 0, 0);
                table->GetAttriByName(attri_index.first, attribute);
                int Pos = table->GetPosByName(attri_index.first);
                int datatype = attribute.get_type();
                if (indexmanager.remove(attri_index.second->get_index_name(), rawdata + Pos, datatype) < 0)
                {
                    cout << "something wrong in record::delete" << endl;
                }
            }
            /*
            for(auto attri : AttriIndexed)
            {
                char keydata[attri.get_length()];
                string index_name;
                Method::setIndexFromTableAttri(table_name, attri.get_name(), index_name);
                int Pos = table->GetPosByName(attri.get_name());
                int datatype = attri.get_type();
                memcpy(keydata, rawdata + Pos, attri.get_length());

                if(indexmanager.remove(index_name, keydata, datatype) < 0)
                {
                    cout << Method::rawdata2int(keydata) << endl;

                }
            }
            */
            filemanager.delete_record_ByAddr(addr);
            NumOfRecordAffected++;
        }
        return NumOfRecordAffected;
    }

    while ((addr = filemanager.getNextRecord(rawdata)) != -1)
    {

        if (table->isSatisfyAllCondition(rawdata, attribute_name, condition, operand) == true)
        {
            // 更新所有索引
            for (auto attri_index : table->Attri2Index)
            {
                Attribute attribute("", 0, 0, 0);
                table->GetAttriByName(attri_index.first, attribute);
                int Pos = table->GetPosByName(attri_index.first);
                int datatype = attribute.get_type();
                if (indexmanager.remove(attri_index.second->get_index_name(), rawdata + Pos, datatype) < 0)
                {
                    cout << "something wrong in record::delete" << endl;
                }
            }
            /*
            for (auto attri : AttriIndexed)
            {
                char keydata[attri.get_length()];
                string index_name;
                Method::setIndexFromTableAttri(table_name, attri.get_name(), index_name);
                int Pos = table->GetPosByName(attri.get_name());
                int datatype = attri.get_type();
                memcpy(keydata, rawdata + Pos, attri.get_length());
                cout << Method::rawdata2float(keydata) << endl;
                indexmanager.remove(index_name, keydata, datatype);
            }
            */
            filemanager.delete_record_ByAddr(addr);
            NumOfRecordAffected++;
        }
    }
    return NumOfRecordAffected;
}

/* 通过地址获得具体的rawdata
 *
 */
const char *RecordManager::GetRecordByAddr(const string &table_name, const int addr)
{
    FileManager file("data/" + table_name);

    return file.get_record(addr);
}

bool RecordManager::DeleteRecordByAddr(const string &table_name, const int addr)
{
    FileManager file("data/" + table_name);

    return file.delete_record_ByAddr(addr);
}

/* 该函数返回插入后的绝对地址
 * 先看一下data文件是否存在，如果不存在，则创建  ✅
 * 检查重复属性 ,不需要record来检查 ✅
 * 插入 这里有一个内存泄漏的风险
 */
int RecordManager::insert(const string &table_name, const char *rawdata) throw(Error)
{
    FileManager filemanager("data/" + table_name);

    // duplicate检查完毕，可以正常插入
    return filemanager.add_record(rawdata);
}