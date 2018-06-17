#ifndef CATALOG_H
#define CATALOG_H
#include "config.h"


/* TODO
 * get_table可以从 unordered_map中获得
 * 是否需要学大佬，用一个 BlockManager 来封装block
 * catalogmanager构造的时候就把table/index metadata 读入自己的成员变量里，等价于读入了系统内存，更快,同时，我永远需要更新两份东西，一份是文件，
 * 一份是我的成员变量
 * 但是没有读更多的attribute信息，我要不要直接读呢？如果这个也要读的话代价是否大？如果表很多好像不太现实
 * 需要应用attribute的场景多吗？ insert / select / delete / create table / 应用场景很多，但是我一次读这么多，实际用的也不多
 * 全部读进内存的好处就在于“读”操作更快，和buffer其实是一个思路，是更高一层的buffer，其实修改attribute的场景很少很少，所以不妨全部读进来
 * 全部读进我的tableset
 * 把tables/indices读进内存还有一个重要原因，这样的话我把它的metadata存进来了，不需要每次读都去提取block
 * 只要涉及大量读的，放在内存里总是好一点的
 */

/* TODO
 * 是否需要建立一个table+attribute -> index_name的映射，在读indices的时候完成这个映射，这样可以减少冗余
 * 这个映射可能在table中内置更好一点
 */

class CatalogManager
{
    private:
    vector <Table> Table_set;
    unordered_map <string, Table*> Name2Table;
    
    FileManager TableFile;
    FileManager IndexFile;

    public:
    CatalogManager();
    ~CatalogManager();
    void print(){ std::cout << "[CatalogManager]" << std::endl; }

    // return true if create succefully, return false if meet some problem
    bool create_table(const string & table_name, const vector <Attribute> & attributes) throw(Error);
    bool drop_table(const string & table_name) throw(Error);
    bool get_indices(const string & table_name, vector <string> & indices_name) throw(Error);
    bool get_table(const string & table_name, Table & table) throw(Error);

};


#endif