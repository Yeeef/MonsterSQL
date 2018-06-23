
#include "interpreter.h"
#include <fstream>
#include <sstream>
#include <iostream>
using namespace std;

// todo: 了解 reinterpret


/* TODO
 * drop_index的catalog行为有问题✅
 * 删除table,再create会出问题，tables的free_list更新有误✅
 * 重复create_table会出问题✅
 * recordmanager::insert有问题✅
 * 重复insert正常报错✅
 * 不同类型正常报错✅
 * drop table的时候记得把文件也删了✅
 * unique属性正常报错✅
 * 检查char[1]是否正常扩展位✅
 * 用户char输入非法长度的检验✅但是不知道王丹尧会不会把type给我传好
 * getNextRecord有问题，如果遇到中途删除过的地方，没有正常读好
 * 就看indexmanger的表现了
 */
int main()
{

    
    cout << "MonsterSQL👹" << endl;
    CatalogManager & catalogmanager = MiniSQL::get_catalog_manager();
    BufferManager & buffermanager = MiniSQL::get_buffer_manager();
    //buffermanager.print();
    //catalogmanager.print();
    API & api = MiniSQL::get_api();
    string pri_name = "ID";
    vector <Attribute> attri_set;
    Attribute primary(pri_name, TYPE_INT, true, true);
    attri_set.push_back(primary);

    Attribute attri1("float", TYPE_FLOAT, false);
    attri_set.push_back(attri1);
    Attribute attri2("char8", 8, false, true);
    attri_set.push_back(attri2);

    vector <string> insert_data;
    insert_data.push_back("9");
    insert_data.push_back("3.2");
    insert_data.push_back("XM");
    vector <int > type;
    type.push_back(TYPE_INT);
    type.push_back(TYPE_FLOAT);
    type.push_back(8);


    try
    {
        buffermanager.print();

        api.insert("one", insert_data, type);
//        api.create_table("one", primary, attri_set);
//        api.create_table("two", primary, attri_set);
//        api.create_table("three", primary, attri_set);
        //api.drop_table("four");
        //api.drop_table("two");
        //api.create_table("four", primary, attri_set);
        //api.insert("four", insert_data, type);
        //api.insert("lover" , insert_data, type);
        //api.create_table("night", primary, attri_set);
        
        //
        catalogmanager.print();
        buffermanager.WriteAllBack();

    }
    catch (Error err)
    {
        err.print_error();


    }


    





    return 0;
}