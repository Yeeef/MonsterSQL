//
// Created by emma on 2018/6/14.
//

#ifndef BPTREE_BPTREENODE_H
#define BPTREE_BPTREENODE_H

#include <iostream>
#include "minisql.h"
#include <vector>
#include <cmath>
#include "BPTreeKey.h"
using namespace std;
#define BPInsert 1
#define BPDeleteFail 3
#define BPNormal 2
#define BPDelete (-1)
#define BPChange (-2)
#define BPEmpty (-1)
#define BPRepeat (-2)
#define BPFind 1

class BPTreeNode {
public:
    //文件名 + nodeID + keyLength
    //用于根据BufferManager传来的Node构建已有的B+树结点
    BPTreeNode(const char *_filename, int _id, int data_type);

    BPTreeNode(const char *_filename, int _id, int data_type, bool isLeaf, int nextNodeId);
    ~BPTreeNode();


    bool isleaf() { return isLeaf; }
    int insertEntry(BPTreeKey& entry, int pos);
    int deleteEntry(int pos);
    int findPosition_UpperBound(const BPTreeKey& key);
    int findPosition_LowerBound(const BPTreeKey& key);
    void split(BPTreeKey& entry , int nodeID);

    int getkeyPointer(int keyID);
    void mergeVec(vector<ptr>& res, int pos);
    void mergeVecRight(vector<ptr>& res, int pos);
    int borrow(BPTreeKey &entry, BPTreeNode *sibling, bool isLeftSib, BPTreeKey &parentKey);
    void mergeRightNode(bool isLeftSib, BPTreeNode *sibling, BPTreeKey& parentKey, BPTreeKey& entry) ;

    //可否等到BPTree的时候再考虑？
    bool isoverflow() { return nodeSize > nodeCapability; }
    bool isUnderflow() { return  nodeSize < ceil(nodeCapability / 2.0); }
    int getUpperBound() {return (int)ceil(nodeCapability / 2.0);}
    BPTreeKey& getEntry(int pos);
    void setKey(int pos, BPTreeKey& entry);
    void setRemoved(){isRemoved = true;}


    //对叶节点得到下一个结点的ID
    int getNextID(){return keys[0].getPointer();}
    void setNextID(int nodeID){ keys[0].setPointer(nodeID);}
    int getNodeSize(){return nodeSize;}
    int getNodeID(){return nodeId;}

private:
    //存储这个node所属的文件
    string fileName;
    int data_type;

    //存储这个node的ID
    // 用于node之间索引
    // 同时也是实际的文件里的位置
    int nodeId;

    //对叶子结点来说存放了下一个节点的nodeID
    //如果是最后一个节点的话，就存放-1；
    //int nextNodeID;

    //指的是一个node可以存的key的大小
    int nodeSize;
    int nodeCapability;

    //记录key的长度
    int keyLength;

    //通过isLeaf来判断是什么结点：leafNode + internalNode
    bool isLeaf;

    //利用这两个来标记数据是否需要重新写回buffer
    bool isDirty;
    bool isRemoved;

    vector<BPTreeKey> keys;



};








#endif //BPTREE_BPTREENODE_H
