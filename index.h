#ifndef INDEX_H
#define INDEX_H
#include "config.h"

class IndexManager
{
    private:
    void createFile(const string& file_name);
    public:
    IndexManager(){}
    ~IndexManager(){}

    void print() { cout << "[IndexManager]" << endl; }

    bool insert(const string & index_name, const char * raw_data, int data_type, ptr& Pointer)throw(Error);

    //todo:修改了返回值，直接返回nodeID
    int find(const string & index_name, const char * raw_data, int data_type);
    bool remove(const string & index_name, const char * raw_data, int data_type, ptr& retPointer);

    //done!
    bool createIndex(const string & table_name, const string & attribute_name, const string &index_name, int data_type = -1, int posinRecord=-1, bool isBeginning = true);
    bool dropIndex(const string & index_name);



};

#endif