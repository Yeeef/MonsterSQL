//
// Created by emma on 2018/6/14.
//

#ifndef MONSTERSQL_BPTREE_H
#define MONSTERSQL_BPTREE_H
#include "config.h"
#include "BPTreeKey.h"

class BPTree {
private:
    int dataType;
    int keyLength;
    int nodeCount;
    int firstEmptyBlock;
    int firstLeftBlock;
    int mostRightBlock;
    int RootID;
    bool isSplit;
    bool isRemoved;

    string fileName;
    static const int HeaderID = 0;

private:
    //done
    int insert(int nodeID,  BPTreeKey& entry);

    //todo
    int remove(int nodeID, BPTreeKey &entry, int siblingID, bool isLeftSib, int * retPointer, BPTreeKey & parentKey );
    int getFirstEmptyBlock();
   // int findKeyNode(const BPTreeKey & entry);
    void updateHeader();



public:
    /*
     * 析构函数无需执行，因为关于树需要的信息都存在header block里面，而一旦更改header Block我们就会updateHeader
     */
    //done
    int insertKey(BPTreeKey & entry);

    //todo
    ~BPTree() = default;
    explicit BPTree(const string & fileName, int data_type);

    int findKey(BPTreeKey & entry);

    int removeKey(BPTreeKey& entry, int * retPointer);
//    void scaleFindLeftEnd(BPTreeKey & entry, vector<ptr>& res);
//    void scaleFindRightEnd(BPTreeKey & entry, vector<ptr>& res);
//    void scaleFind(BPTreeKey &sta_entry, BPTreeKey &end_entry,vector<ptr> &res);
    void removeBlock(int id);
    void initialIndex(const string & table_name,const string & attribute_name, int attriLength, int posinRecord);

#ifdef DEBUG
    // Recursive function for tree structure printing
    void debugPrint();

    // Recursive function for tree structure printing
    void debugPrint(int id);
#endif
};


#endif //MONSTERSQL_BPTREE_H
