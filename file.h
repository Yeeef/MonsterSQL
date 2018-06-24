#ifndef FILE_H
#define FILE_H
#include "config.h"

// 每个file都有fileheader存储metadata, 所以我必须把这些信息读出来
/* TODO
 * Block_count到底是否需要？
 * record_count是否需要？能否用last_record_id来代替. 本质上是一个东西
 * 必须用record_Addr!否则会出现无法对齐的情况
 * FileManager保存一个block有啥意义？
 * 注意deleteptr，如果没有它必须加一个有效位
 * 如何判断一个文件是否结束？只有地址/record_count不足以判断，因为中间有free_list,通过record_count是可以判断的
 * 删除的时候不需要减小record_count!
 * Pysic_record_length是否需要？
 */

// BufferManager又封了一层

class FileManager
{
    private:
    string file_name;
    Block * block; // 具体这个file的block, 这里的block由buffer集中delete不用file来管
    int block_count; // 可以算出来
    int record_length; // 读出来的
    int LogicalRecordLength;
    int first_free_record_addr; //绝对地址，读出来的
    int record_count; // 读出来的
    int record_count_perblock; //可以算出来
    
    int pointer; //指向实际地址
    int delete_ptr; // 如何管理这个玩意？专门用于遍历的？如果我从一个特殊位置开始呢？

    int AddByCount(const char * rawdata);
    int AddByFreeList(const char * rawdata);
    void updataMeta(); // 更新file metaheader
    void PointTo(int id);
    void renewPointer();
    void renewDeletePointer();
    void PointerIncrement();
    int getBlockIDFromAddr(const int addr);
    int getRelativeAddrInBlock(const int block_id, const int addr);
    int getEOF() const;
    void LoadRecord(const int pointer, char * rawdata);






    public:
    FileManager(string  file_name);
    
    const char * get_record(const int record_addr) const throw(Error);
    const int add_record(const char * rawdata) throw(Error);
    // 这个函数有应用场景
    bool delete_record_ByAddr(const int record_addr) throw(Error);
    int getNextRecord(char * rawdata) throw (Error);
    void ReNewAllPtr();
    //int getNextAttriRecord(string & attribute_name, char * rawdata) throw (Error);
    



    
};
#endif