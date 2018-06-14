#ifndef MINISQL_H
#define MINISQL_H
#include "api.h"
#include "catalog.h"
#include "record.h"
#include "buffer.h"
#include "index.h"


class MiniSQL
{
    private:
    static API & api;
    static CatalogManager & catalog_manager;
    static RecordManager & record_manager;
    static BufferManager & buffer_manager;
    static IndexManager & index_manager;
    

    public:
    MiniSQL(){}
    ~MiniSQL(){}

    static API & get_api() {return api;}
    static CatalogManager & get_catalog_manager();
    static RecordManager & get_record_manager();
    static BufferManager & get_buffer_manager();
    static IndexManager & get_index_manager();

    
};



#endif