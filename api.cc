#include "api.h"

/*
bool API::create_table(const string &table_name, const Attribute & primary, const vector<Attribute> &attributes) const throw(Error)
{
    #ifdef DEBUG
    cout << "[API::create_table]" << endl;
    #endif

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
        index_name = table_name+primary.get_name();
        create_index(table_name, primary.get_name(), index_name);

        #ifdef DEBUG
        cout << "[API:create table: '" << table_name << "' successfully" << endl;
        #endif

        return true;
        
    }
    catch(Error err)
    {
       err.print_error();
       return false;
    }

    
}

bool API::drop_table(const string &table_name) const throw(Error)
{
    #ifdef DEBUG
    cout << "[API::drop_table]" << endl;
    #endif

     // 获取catalogmanager
    CatalogManager &catalogmanager = MiniSQL::get_catalog_manager();
    // 获取recordmanager
    RecordManager &recordmanager = MiniSQL::get_record_manager();

    try
    {
        vector <string> indices_name;
        // get indices from catalog
        catalogmanager.get_indices(table_name, indices_name);
        // drop all indices related to table
        for(auto index : indices_name)
        {
            drop_index(index);
        }
        catalogmanager.drop_table(table_name);
        recordmanager.drop_table(table_name);

        #ifdef DEBUG
        cout << "[API:drop table: '" << table_name << "' successfully" << endl;
        #endif
        
        return true;
    }
    catch(Error err)
    {
        err.print_error();
        return false;
    }

    
}

bool API::insert(const string &table_name, const vector<string> & insert_data) const throw(Error)
{
    #ifdef DEBUG
    cout << "[API::insert]" << endl;
    #endif

    // 获取catalogmanager
    CatalogManager &catalogmanager = MiniSQL::get_catalog_manager();
    // 获取recordmanager
    RecordManager &recordmanager = MiniSQL::get_record_manager();
    // 获取 indexmanager
    IndexManager &indexmanager = MiniSQL::get_index_manager();

    // parse the vector into raw_content
    // interpreter 已经做过catalog检查，所以一定是可以插的数据
    // insert data into record
   



    
    return false;
}

bool API::Delete(const string &table_name, const vector<string> primary) const throw(Error)
{
    return false;
}
bool API::select(const string &table_name, const vector<string> primary) const throw(Error)
{
    return false;
}
bool API::create_index(const string &table_name, const string &attribute_name,
                       const string &index_name) const throw(Error)
{
    return false;
}
bool API::drop_index(const string &index_name) const throw(Error)
{
    return false;
}
*/