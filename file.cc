#include "file.h"
#include "minisql.h"

/* ---------------------------------------------*/
/* FileManager */
FileManager::FileManager(string file_name)
{

    
    this->file_name = file_name;
    //向buffer manager请求 文件名的 第一个block
    BufferManager & buffermanager = MiniSQL::get_buffer_manager();
    // 0th block 存储 file metadata: record_length, first, record_count
    Block * meta_block = buffermanager.getBlock(file_name, 0);
     
    char * content = meta_block->getContent();

    // record_length record_count first_free_id(absolute)
    record_length = Method::rawdata2int(content);
    first_free_record_id = Method::rawdata2int(content + INT_LENGTH);
    
    // 这个不一定要
    record_count = Method::rawdata2int(content + 2*INT_LENGTH);
    
    

    
    // 计算 blockcount？, record_perblock
    record_count_perblock = BLOCK_SIZE / record_length;

    #ifdef DEBUG
    cout << "[FileManager::FileManager] construct with '" << file_name << "'" << endl;
    #endif
    
   
}

// const修饰，保证我的指针不会被改变
const char * FileManager::get_record(const int record_id) const
{
    int record_addr = Method::recordID2Addr(record_id, record_length);
    // 计算block_id
    BufferManager buffermanager = MiniSQL::get_buffer_manager();
    
    int block_id = record_addr / BLOCK_SIZE;
    //就是实际的偏移地址
    int posInBlock = record_addr - (block_id * BLOCK_SIZE);
    Block * block = buffermanager.getBlock(file_name, block_id);

    // 读出所需的record
    return block->getContent() + posInBlock;
    

}