
#include "catalog.h"
#include <unordered_set>

CatalogManager::CatalogManager()
{   
    // 从文件读信息填充我的table-set
    
    

}
bool CatalogManager::create_table(const string &table_name, const vector<Attribute> &attributes) throw(Error)
{
    //先判断是否是已经存在的table
    auto search_table = Name2Table.find(table_name);
    if(search_table == Name2Table.end())
    {
        Error err("[CatalogManager::create_table]: table already exists");
        throw err;
        return false;
    }

    // 保证没有重复属性
    unordered_set <string> attribute_exist;

    for(auto attribute : attributes)
    {
        auto search_attribute = attribute_exist.find(attribute.get_name());
        if(search_attribute != attribute_exist.end())
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

bool CatalogManager::get_table(const string & table_name, Table & table) throw(Error)
{
    bool isTable = false;
    for(auto Table : Table_set)
    {
        if(table_name == Table.get_table_name())
        {
            // 把Table值穿出去
            table = Table;
            isTable = true;
            break;
        }
    }
    if(isTable == false)
    {
        string err_info = "[CatalogManager::get_table]: cannot find table";
        Error err(err_info);
        throw err;
        return false;
    }

    return true;
}
