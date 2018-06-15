#include "minisql.h"
#include "api.h"


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

/* TODO
 * rawdata 应该是一个 vector <char*> 
 * catalogmanger里面内建一个map访问更快
 */
bool API::insert(const string &table_name, const vector<string> & insert_data, const vector <int> & type) const throw(Error)
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

    try
    {
        // 从catalog获得table
        Table table("", 0, 0);
        catalogmanager.get_table(table_name, table);

        #ifdef DEBUG
        cout << "[API::insert]: find table: '" << table_name << "'" << endl;
        #endif

        // parse data
        vector <char*> raw_Vec;
        char * rawdata = new char[table.get_record_length()];
        // judge whether insert_data is a valid input due to the insert_data
        // At the meantime, convert the insert_data into raw_data
        table.isValidInput(insert_data, type, raw_Vec);

        // convert raw_data_Vec to raw_data
        table.rawVec2rawData(raw_Vec, rawdata);

        #ifdef DEBUG
        cout << "[API::insert]: parse data successfully"<<endl;
        #endif

        //我还需要知道插入后的 block_id 以及 record_id
        ptr Pointer;
        recordmanager.insert(table_name, rawdata, Pointer);

        #ifdef DEBUG
        cout << "[API::insert]: insert successfully"<<endl;
        #endif

        // 需要获得这个表上的所有indices
        // 需要调用catalog来给我index对应的attribute, 在这里我应该用不到index 的metadata
        // 用record metadata 足够了，我需要知道对应的index对应的type
        // 这里不妨通过table 的 attribute_set来做
        vector <Attribute> attributes = table.get_attribute_set();
        for(int i = 0; i < attributes.size(); i++)
        {
            if(attributes.at(i).is_index() == true)
            {
            
                indexmanager.insert(attributes.at(i).get_index_name(), raw_Vec.at(i), attributes.at(i).get_type(), Pointer);
            }
        }

        
        //indexmanager.insert();




        for(auto raw : raw_data)
        {
            delete [] raw;
        }
        delete [] data;
        return true;
    }
    catch(Error err)
    {
        err.print_error();
        return false;
    }
    


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
