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
    // 找到对应条件的content并返回一个记录的rawdata
    const char * select(const string & table_name, const string & attri_name, 
                        const int cond, const char * keydata, int * addr);
    const char * GetRecordByAddr(const string & table_name, const int addr); 
};
#endif