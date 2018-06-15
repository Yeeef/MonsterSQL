
#include "catalog.h"

bool CatalogManager::create_table(const string &table_name, const vector<Attribute> &attributes) throw(Error)
{
    return false;
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
