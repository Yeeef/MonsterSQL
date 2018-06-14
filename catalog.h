#ifndef CATALOG_H
#define CATALOG_H
#include "config.h"

class TableCatalog
{

};

class IndexCatalog
{

};

class CatalogManager
{
    private:
    public:
    CatalogManager(){};
    ~CatalogManager(){};
    void print(){ cout << "[CatalogManager]" << endl; }

    // return true if create succefully, return false if meet some problem
    bool create_table(const string & table_name, const vector <Attribute> & attributes) throw(Error);
    bool drop_table(const string & table_name) throw(Error);
    bool get_indices(const string & table_name, vector <string> & indices_name) throw(Error);

};


#endif