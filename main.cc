#include "config.h"
#include "minisql.h"
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
 * drop_table出了问题，还是要添加有效位！原有的record_length必须+1
 * 1. 修改初始的 tables indices ✅
 * 2. 现在有一个物理长度和逻辑长度的问题，在文件里，我现在存了物理长度 ✅
 * 3. 先从file改起, filemanager里面有个record_length，增加 LogicalLength并改正构造函数 ✅
 * 4. filemanager内部所有有关于 record_length 均改完 ✅
 * 5. 开始修改 catalog, 目前暂时觉得 catalog没什么可修改的 ✅
 * 6. 还要修改 create_table / insert
 * 直接createtable, droptable会有问题 ✅ 是 buffer出了问题
 * insert还是会问题，assert那里
 * getNextRecord有问题，如果遇到中途删除过的地方，没有正常读好
 * 就看indexmanger的表现了
 * 检验这条记录是否满足一致性，千万不能用遍历的方法，太麻烦！
 * 暂时去掉一致性检验，先看看insert不检验的表现，现在Index出了问题
 * 现在开始看看select不考虑index
 * delete无效，先解决 select * from table;没有条件的问题
 * drop table 之后 立即 create_table会出现 tablemeta/book 的问题 ✅
 * 王丹尧不会默认 primary是一个unique ✅
 * addrecord在多次操作中会出问题，可能是 pointer 的问题 ✅
 * 65521调试突破口 ✅
 * unique建索引的问题
 * 现在insert加入index ✅  但是插入会有问题 ✅
 * string2rawdata有问题，有空格的时候流会忽略空格
 */
int main()
{

    
    cout << "MonsterSQL👹" << endl;
    CatalogManager & catalogmanager = MiniSQL::get_catalog_manager();
    BufferManager & buffermanager = MiniSQL::get_buffer_manager();


    Interpreter interpreter;
    string sql1 = "Create table book (id float primary key, name varchar(30));" ;
    string sql2 = "drop table book;";
    string sql3 = "insert into book values (819);";
    string sqlselect = "select * from book ;";
    string sqldelete = "delete from book where id < 99990;";

    //sql = "Create table book (id int primary key, name varchar(50), age INT, unique(id));drop table book;";
    string rawsql = "insert into book values (";
    string endsql = ", 'I am upset');";
    string selectStartsql = "select * from book where id = ";
    string selectEndsql = " ;";

    try
    {
         //interpreter.execute(sqlselect.c_str());
        //interpreter.execute(sqldelete.c_str());
        //interpreter.execute(sql3.c_str());

        for(int i = 0; i < 10; i++)
        {
            stringstream ss;
            ss << (i + 0.1);
            string num;
            ss >> num;
            string sql4 = rawsql + num + endsql;
            //cout << sql4 << endl;
            interpreter.execute(sql4.c_str());
            //cout << sql4 << endl;
            //buffermanager.print();

        }
//         cout << "good" << endl;
    // for(int i = 0; i < 1000; i++)
    // {
    //     stringstream ss;
    //     ss << i;
    //     string num;
    //     ss >> num;
    //     string batchSelectSql = selectStartsql + num + selectEndsql;
    //     interpreter.execute(batchSelectSql.c_str());

    // }

//
//         interpreter.execute(sql2.c_str());
//
//         interpreter.execute(sql1.c_str());
        catalogmanager.print();

        
        buffermanager.WriteAllBack();

    }
    catch (Error err)
    {
        err.print_error();


    }


    





    return 0;
}