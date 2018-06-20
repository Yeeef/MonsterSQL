#include "record.h"
#include "minisql.h"

bool RecordManager::create_table(const string &table_name) throw(Error)
{
}

/* TODO
 * 删除数据区文件
 */
bool RecordManager::drop_table(const string &table_name) throw(Error)
{
    Method::deleteFile("data/" + table_name);


}

/* 该函数返回插入后的绝对地址
 * 先看一下data文件是否存在，如果不存在，则创建  ✅
 * 检查重复属性 ✅
 * 插入 这里有一个内存泄漏的风险
 */
const char * RecordManager::select(const string &table_name, const string &attri_name,
                                  const int cond, const char *keydata, int * addr)
{
}

const char * RecordManager::GetRecordByAddr(const string & table_name, const int addr)
{

}
int RecordManager::insert(const string &table_name, const char *rawdata) throw(Error)
{
    CatalogManager &catalogmanager = MiniSQL::get_catalog_manager();

    const Table *table = catalogmanager.get_table(table_name);
    const int record_length = table->get_record_length();
    if (Method::isFileExist("data/" + table_name) == false)
    {
        Method::createFile("data/" + table_name, record_length);
    }
    FileManager filemanager("data/" + table_name);
    filemanager.ReNewAllPtr();

    // 检查unique, primary 是否 duplicate
    char ExistData[record_length];
    while (filemanager.getNextRecord(ExistData) != -1)
    {
        //拿到了下一条记录的rawcontent.
        string duplicate;
        // 不符合
        if (table->CheckConsistency(ExistData, rawdata, duplicate) == false)
        {
            // 删除rawdata
            delete[] rawdata;
            string err_info = "[RecordManager::insert] duplicate '" + duplicate + "' ";
            throw Error(err_info);
        }
    }

    // duplicate检查完毕，可以正常插入
    return filemanager.add_record(rawdata);
}