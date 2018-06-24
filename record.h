#ifndef RECORD_H
#define RECORD_H
#include "config.h"


class RecordManager
{
    private:
    public:
    RecordManager(){};
    ~RecordManager(){};

    void print(){ cout << "[RecordManager]" << endl; }
    // dosen't need other info
    bool create_table(const string & table_name) throw(Error);
    bool drop_table(const string & table_name) throw(Error);
    int insert(const string & table_name, const char * rawdata) throw(Error);
    // 找到对应条件的content并返回一个记录的rawdata，能不能在select里面直接打印？
    bool select(const string & table_name, const vector<string> & attribute_name, 
                        const vector<int> & condition, const vector<string> & operand);
    bool Delete(const string & table_name, const vector<string> & attribute_name, 
                        const vector<int> & condition, const vector<string> & operand);
    const char * GetRecordByAddr(const string & table_name, const int addr); 
    bool DeleteRecordByAddr(const string & table_name, const int addr);
};
#endif