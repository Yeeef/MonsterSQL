#ifndef FILE_H
#define FILE_H
#include "config.h"

// 每个file都有fileheader存储metadata, 所以我必须把这些信息读出来
/* TODO
 * Block_count到底是否需要？
 * record_count是否需要？能否用last_record_id来代替
 */

// BufferManager又封了一层

class FileManager
{
    private:
    string file_name;
    Block * block; // 具体这个file的block, 这里的block由buffer集中delete不用file来管
    int block_count; // 可以算出来
    int record_length; // 读出来的
    int first_free_record_id; //绝对地址，读出来的
    int record_count; // 读出来的
    int record_count_perblock; //可以算出来
    
    ptr current_pointer; // current_pointer


    public:
    FileManager(string  file_name);
    
    const char * get_record(const int record_id) const;
    
};
#endif