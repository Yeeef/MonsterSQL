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

    bool insert(const string & index_name, const char * raw_data, int data_type, int recordID)throw(Error);

    //todo:修改了返回值，直接返回nodeID
    int find(const string & index_name, const char * raw_data, int data_type);
    int remove(const string & index_name, const char * raw_data, int data_type)throw(Error);

    //done!
    bool createIndex(const string & table_name, const string & attribute_name, const string &index_name, int data_type = -1, int posinRecord=-1, bool isBeginning = true);
    bool dropIndex(const string & index_name);



};

#endif

/*
 * 1。 关于dataType的事情，我们需要每次都给BPKey的dataType赋值
 */