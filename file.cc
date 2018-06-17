#include "file.h"
#include "minisql.h"

/* ---------------------------------------------*/
/* FileManager */
/* TODO
 * getRecordByID的越界检查能不能做？
 * 
 */
FileManager::FileManager(string file_name)
{

    this->file_name = file_name;
    //向buffer manager请求 文件名的 第一个block
    BufferManager &buffermanager = MiniSQL::get_buffer_manager();
    // 0th block 存储 file metadata: record_length, first, record_count
    Block *meta_block = buffermanager.getBlock(file_name, 0);

    char *content = meta_block->getContent();

    // record_length record_count first_free_id(absolute)
    record_length = Method::rawdata2int(content);
    first_free_record_addr = Method::rawdata2int(content + INT_LENGTH);

    // 这个不一定要
    record_count = Method::rawdata2int(content + 2 * INT_LENGTH);

    // 计算 blockcount？, record_perblock
    record_count_perblock = BLOCK_SIZE / record_length;

#ifdef DEBUG
    cout << "[FileManager::FileManager] construct with '" << file_name << "'" << endl;
#endif
}

// const修饰，保证我的指针不会被改变
const char *FileManager::get_record(const int record_addr) const throw(Error)
{
    //通过record计算一下这个是否越界？越界检查非常难做。。。除非能保留一个到最后一条记录的指针

    // 计算block_id
    BufferManager buffermanager = MiniSQL::get_buffer_manager();

    int block_id = record_addr / BLOCK_SIZE;
    //就是实际的偏移地址
    int posInBlock = record_addr - (block_id * BLOCK_SIZE);
    Block *block = buffermanager.getBlock(file_name, block_id);

    // 读出所需的record
    return block->getContent() + posInBlock;
}

// 返回插入后的recordID
const int FileManager::add_record(const char *rawdata) throw(Error)
{
    //先判断freelist是否有效
    // recordIDInBlock实际上是相对地址
    int record_addr;

    if (first_free_record_addr == -1)
    {
        record_addr = AddByCount(rawdata);
        
    }
    else
    {
        record_addr = AddByFreeList(rawdata);
    }

    updataMeta();
    return record_addr; 

    //更新首块信息
}

// 返回一个绝对地址
int FileManager::AddByCount(const char *rawdata)
{
    int block_id, recordIDInBlock, record_relative_addr, record_addr;

    BufferManager &buffermanager = MiniSQL::get_buffer_manager();
    block_id = record_count / record_count_perblock + 1;                     //需要插入的block
    recordIDInBlock = record_count - (block_id - 1) * record_count_perblock; //需要插入的相对位置，在那个block里应该是第几条
    record_relative_addr = recordIDInBlock * record_length;                  //相对地址
    record_addr = recordIDInBlock * record_length + block_id * BLOCK_SIZE;   //绝对地址

    // 更新record_count
    record_count++;

    // 这里是否需要改成成员变量？更新内容
    Block *block = buffermanager.getBlock(file_name, block_id);
    memcpy(block->getContent() + record_relative_addr, rawdata, record_length);
    block->set_dirty(true); //标记脏块

    return record_addr;
}

int FileManager::AddByFreeList(const char *rawdata)
{
    int block_id, record_addr, record_relative_addr;
    BufferManager &buffermanager = MiniSQL::get_buffer_manager();

    //通过freelist来判断第一个应该插入的地址
    record_addr = first_free_record_addr;
    block_id = record_addr / BLOCK_SIZE;                              //应该插入的block, 注意这里不需要+1
    record_relative_addr = record_addr - (block_id + 1) * BLOCK_SIZE; //相对地址

    Block * block = buffermanager.getBlock(file_name, block_id);

    //更新first_free_record_addr;
    first_free_record_addr = *(reinterpret_cast<int *>(block->getContent() + record_relative_addr));
    // 更新内容
    memcpy(block->getContent() + record_relative_addr, rawdata, record_length);
    block->set_dirty(true); //标记脏块

    return record_addr;
}

void FileManager::updataMeta()
{
    BufferManager & buffermanager = MiniSQL::get_buffer_manager();

    Block * metahead = buffermanager.getBlock(file_name, 0);
    char * content = metahead->getContent();
    memcpy(content, (&record_length), INT_LENGTH);
    memcpy(content + INT_LENGTH, (&first_free_record_addr), INT_LENGTH);
    memcpy(content + 2*INT_LENGTH, (&record_count), INT_LENGTH);
    metahead->set_dirty(true); //标记脏
}