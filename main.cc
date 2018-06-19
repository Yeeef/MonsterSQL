
#include "interpreter.h"
#include <fstream>
#include <sstream>
#include <iostream>
using namespace std;

// todo: 了解 reinterpret


/* TODO
 * drop_index的catalog行为有问题✅
 * 删除table,再create会出问题，tables的free_list更新有误
 * 重复create_table会出问题
 */
int main()
{

    
    cout << "MonsterSQL👹" << endl;
    CatalogManager & catalogmanager = MiniSQL::get_catalog_manager();
    BufferManager & buffermanager = MiniSQL::get_buffer_manager();
    buffermanager.print();
    catalogmanager.print();
    API & api = MiniSQL::get_api();
    string pri_name = "ID";
    vector <Attribute> attri_set;
    Attribute primary(pri_name, TYPE_INT, true, true);
    attri_set.push_back(primary);

    Attribute attri1("a1", TYPE_FLOAT, false);
    attri_set.push_back(attri1);
    Attribute attri2("a2", 5, false, true);
    attri_set.push_back(attri2);

    try
    {
        //api.drop_table("student");

        //api.create_table("parent", primary, attri_set);
        //api.create_table("child", primary, attri_set);
        //api.create_table("teacher", primary, attri_set);
        //api.create_table("killer", primary, attri_set);
        //api.drop_table("killer");
        api.create_table("lover", primary, attri_set);
        catalogmanager.print();
        buffermanager.print();
        buffermanager.WriteAllBack();

    }
    catch (Error err)
    {
        err.print_error();


    }


    





    return 0;
}