#include "minisql.h"


API api_init = API();
API & MiniSQL::api = api_init;


CatalogManager catalog_init = CatalogManager();
CatalogManager & MiniSQL::catalog_manager = catalog_init;

RecordManager record_init = RecordManager();
RecordManager & MiniSQL::record_manager = record_init;

BufferManager buffer_init = BufferManager();
BufferManager & MiniSQL::buffer_manager = buffer_init;

IndexManager index_init = IndexManager();
IndexManager & MiniSQL::index_manager = index_init;





CatalogManager & MiniSQL::get_catalog_manager() { return catalog_manager; }
RecordManager & MiniSQL::get_record_manager() { return record_manager; }
BufferManager & MiniSQL::get_buffer_manager() { return buffer_manager; }
IndexManager &  MiniSQL::get_index_manager() { return index_manager; }
