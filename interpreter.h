#ifndef INTERPRETER_H
#define INTERPRETER_H

#include "config.h"
#include <ctime>
#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <string.h>
#include <stdlib.h>


#include <vector>
#include <string>
#include "api.h"

using namespace std;

//字符状态类型
enum TOKEN{
    INVALID, IDLE, END, IDENTIFIER, NUMBER, SCHAR, STRING, SYMBOL, OPERATOR
};


class Interpreter
{
    private:
     vector<string> tokens;//字符
     vector<TOKEN> type;//类型
     int ptr;
     bool exiting; //是否退出
     bool fromFile; //是否是文件运行
     
     //语句转换成字符vector
     int getTokens(const char* sql, vector<string>& tokens, vector<TOKEN>& type);

     //跳过当前语句
     void skipStatement();

     //具体位置报错
     void reportUnexpected(const char* position, const char* expecting);

     //操作符类型
     int GetOperatorType(const string& op);

     //解析操作条件
     bool where(vector<string>& attribute_name, vector<int>& condition , vector<string>& operand);

     //字符类型
     int GetType();

     //select
     void select();

     //delete
     void remove();

     //删除表
     void drop_table();

     //删除索引
     void drop_index();

     //创建索引
     void create_index();

     //创建表
     void create_table();

     //insert
     void insert();

     //运行文件
     void execfile();

     //退出
     void exit();
    	
    public:
    Interpreter(bool _fromFile = false);
    ~Interpreter();
    
    bool isTokenEmpty()
    {
        return !!tokens.size();
    }
    bool isExiting() const
    {
        return exiting;
    }
    void execute(const char* sql);

/*
    void print() 
    {
        cout << "[Interpreter]" << endl;
        API & api = MiniSQL::get_api();
        CatalogManager & catalog_manager = MiniSQL::get_catalog_manager();
        RecordManager & record_manager = MiniSQL::get_record_manager();
        BufferManager & buffer_manager = MiniSQL::get_buffer_manager();
        IndexManager & index_manager = MiniSQL::get_index_manager();
        catalog_manager.print();
        record_manager.print();
        buffer_manager.print();
        index_manager.print();
    }
*/
};

#endif 
