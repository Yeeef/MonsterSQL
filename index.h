#ifndef INDEX_H
#define INDEX_H
#include "config.h"

class IndexManager
{
    private:
    public:
    IndexManager(){};
    ~IndexManager(){};

    void print() { cout << "[IndexManager]" << endl; }
    bool insert(const string & index_name, const char * raw_data, int data_type, ptr& Pointer) throw(Error);
    bool remove(const string & index_name, const char * raw_data, int data_type, ptr& retPointer);
    bool find(const string & index_name, const char * raw_data, int data_type, ptr& retPointer) {};
    bool scaleFind(const string & index_name, const char * sta_raw_data,const char * end_raw_data, int data_type, vector<ptr>& retPointer);
    bool scaleFindLeftEnd(const string & index_name, const char * end_raw_data, int data_type, vector<ptr>& retPointer);
    bool scaleFindRightEnd(const string & index_name, const char * sta_raw_data, int data_type, vector<ptr>& retPointer);
    bool createIndex(const string & table_name, const string & attribute_name, const string &index_name, int data_type, int posinRecord, bool isBeginning = true);
    bool dropIndex(const string & index_name);

};

#endif