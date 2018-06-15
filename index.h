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
    bool insert(const string & index_name, const char * raw_data, int data_type, ptr Pointer) throw(Error);
};

#endif