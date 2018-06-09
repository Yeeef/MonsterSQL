#ifndef INTERPRETER_H
#define INTERPRETER_H
#include "config.h"
#include "minisql.h"

class Interpreter
{
    private:
    public:
    Interpreter(){};
    ~Interpreter(){};

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
};

#endif 