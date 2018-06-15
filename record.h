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
    bool insert(const string & table_name, const string rawdata, ptr & Pointer) throw(Error);
};
#endif