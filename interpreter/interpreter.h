#ifndef INTERPRETER_H
#define INTERPRETER_H

#include <vector>
#include <string>

using namespace std;

enum TOKEN{
    INVALID, IDLE, END, IDENTIFIER, NUMBER, SCHAR, STRING, SYMBOL, OPERATOR
};

class Interpreter
{
    private:
     vector<string> tokens;
     vector<TOKEN> type;
     int ptr;
   //API & api;

     int getTokens(const char* sql, vector<string>& tokens, vector<TOKEN>& type);
     void skipStatement();
     void reportUnexpected(const char* position, const char* expecting);
     int GetOperatorType(const string& op);
     bool where(vector<string>& attribute_name, vector<int>& condition , vector<string>& operand);
     int GetType();

     void select();
     void remove();
     void drop_table();
     void drop_index();
     void create_index();
     void create_table();
     void insert();

    	
    public:
    Interpreter();
    ~Interpreter();
    

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
