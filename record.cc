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
bool RecordManager::select(const string & table_name, const vector<string> & attribute_name, 
                        const vector<int> & condition, const vector<string> & operand)
{
    CatalogManager & catalogmanager = MiniSQL::get_catalog_manager();
    const Table * table = catalogmanager.get_table(table_name);
    
    FileManager filemanager("data/" + table_name);
    int addr;
    char rawdata[table->get_record_length()];
    while((addr = filemanager.getNextRecord(rawdata)) != -1)
    {
        if(table->isSatisfyAllCondition(rawdata, attribute_name, condition, operand) == true)
        {
            table->PrintRawdata(rawdata);
        }
        
    }
    return true;

}

bool RecordManager::Delete(const string & table_name, const vector<string> & attribute_name, 
                        const vector<int> & condition, const vector<string> & operand)
{
    CatalogManager & catalogmanager = MiniSQL::get_catalog_manager();
    const Table * table = catalogmanager.get_table(table_name);
    
    FileManager filemanager("data/" + table_name);
    int addr;
    char rawdata[table->get_record_length()];
    while((addr = filemanager.getNextRecord(rawdata)) != -1)
    {
        if(table->isSatisfyAllCondition(rawdata, attribute_name, condition, operand) == true)
        {
            filemanager.delete_record_ByAddr(addr);
        }       
    }
    return true;

}

/* 通过地址获得具体的rawdata
 *
 */
const char * RecordManager::GetRecordByAddr(const string & table_name, const int addr)
{
    FileManager file("data/" + table_name);

    return file.get_record(addr);


}

bool RecordManager::DeleteRecordByAddr(const string & table_name, const int addr)
{
    FileManager file("data/" + table_name);

    return file.delete_record_ByAddr(addr);

}

/* 该函数返回插入后的绝对地址
 * 先看一下data文件是否存在，如果不存在，则创建  ✅
 * 检查重复属性 ✅
 * 插入 这里有一个内存泄漏的风险
 */
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