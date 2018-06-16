#ifndef BUFFER_H
#define BUFFER_H
#include "config.h"
#include <unordered_map>


class BufferNode
{
    private:
    Block * block;
    bool reference_bit;
    BufferNode * next;

    public:
    BufferNode();
    ~BufferNode();
    BufferNode * get_next();
    void set_next(BufferNode * next);
    Block * get_block();


};

class BufferManager
{
    private:
    const int max_block_count; // 8?

    BufferNode * clock_ptr; // ptr of the 时钟算法
    // block名字到block的映射，一个block由它的文件名和block_id唯一决定
    unordered_map <string, BufferNode * > Name2Node; 

    

    public:
    BufferManager();
    ~BufferManager();

    void print() { cout << "[BufferManager]" << endl; }
    // get a block by the filename + block_id
    Block* getBlock(const string &fileName, int block_id);
};


#endif