#ifndef BUFFER_H
#define BUFFER_H
#include "config.h"
#include <unordered_map>

/* TODO
 * 对一些特定情况，第一次打开文件，我的buffer可能会打不开文件
 * 也就是创建文件时，我的行为还没有定下来
 * readblockfromfile的时候还需要更新我的map✅
 * 
 */ 
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
    bool get_reference_bit();
    void set_reference_bit(bool bit);

    //注意这个函数同时会把 ref_bit变为1
    void set_block(Block * block);
    


};

class BufferManager
{
    private:
    const int max_block_count; // 8?

    BufferNode * clock_ptr; // ptr of the 时钟算法
    // block名字到block的映射，一个block由它的文件名和block_id唯一决定
    unordered_map <string, BufferNode * > Name2Node; 

    void WriteBlockBack(Block * block) throw(Error);
    void ReadBlockContent(string file_name, int block_id, char *content) throw(Error);
    Block * getBlockFromFile(const string & fileName, int block_id) throw(Error);
    void getMapName(string & map_name, const string & fileName, const int block_id);
    

    public:
    BufferManager();
    ~BufferManager();

    void print() { cout << "[BufferManager]" << endl; }
    // get a block by the filename + block_id
    Block* getBlock(const string &fileName, int block_id) throw(Error);  
    void addBlock(Block * block) throw(Error);
    void DeleteBlockByFile(const string & fileName);
    
};


#endif