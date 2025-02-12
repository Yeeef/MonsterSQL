//
// Created by emma on 2018/6/14.
//

#include "BPTree.h"
#include "file.h"
#include "minisql.h"
#include "BPTreeNode.h"



BPTree::BPTree(const string &fileName, int data_type):dataType(data_type),fileName(fileName) {

    /*
     * 调用manager获取header block的信息
     * 初始化B+树的整体信息
     */
    BufferManager & manager = MiniSQL::get_buffer_manager();
    Block* header = manager.getBlock(fileName, HeaderID);

    keyLength = Method::getLengthFromType(data_type);

    //获取header的信息
    firstEmptyBlock = *(reinterpret_cast<int*>(header->getContent()));
    nodeCount = *(reinterpret_cast<int*>(header->getContent() + 4));
    firstLeftBlock = *(reinterpret_cast<int*>(header->getContent() + 8));
    mostRightBlock = *(reinterpret_cast<int*>(header->getContent() + 12));
    RootID = *(reinterpret_cast<int*>(header->getContent() + 16));

    isSplit = false;
    isRemoved = false;

}
void BPTree::updateHeader() {

    BufferManager & manager = MiniSQL::get_buffer_manager();
    Block* header = manager.getBlock(fileName, 0);
    if(nodeCount == 0)
        firstEmptyBlock = -1;

    memcpy(header->getContent(), &firstEmptyBlock, 4);
    memcpy(header->getContent() +4, &nodeCount, 4);
    memcpy(header->getContent()+8, &firstLeftBlock, 4);
    memcpy(header->getContent()+12, &mostRightBlock, 4);
    memcpy(header->getContent()+16, &RootID, 4);

    header->set_dirty(true);
}




/*
 * 如果有firstEmptyBlock并且小于nodeCount，直接使用这个firstEmptyBlock，
 * 并把这个block里面存的下一个指针赋值给这棵树记录的firstEmptyBlock
 *
 * 如果block中间没有空的可以插入的block，赋值firstEmptyBlock-1
 * 并且直接返回nodeCount++
 */
int BPTree::getFirstEmptyBlock() {

    if(firstEmptyBlock == -1)
        return ++nodeCount;
    else{
        int ret = firstEmptyBlock;
        nodeCount++;
        BufferManager & manager = MiniSQL::get_buffer_manager();
        Block* header = manager.getBlock(fileName, firstEmptyBlock);

        //todo:没有分配长度，但是是int应该不用管
        firstEmptyBlock = Method::rawdata2int(header->getContent());
        return ret;
    }
}


int BPTree::insert(int nodeID, BPTreeKey& entry) {
    BPTreeNode* node = new BPTreeNode(fileName.c_str(), nodeID, dataType);
    int pos = node->findPosition_LowerBound(entry);
    int ret = BPNormal;
    int res;

    //nodesize == 0
    //todo
    assert(pos != -1);
    res = node->isleaf() ? BPInsert : insert(node->getkeyPointer(pos), entry);

    // Check for duplicate
    if (res == BPRepeat)
    {
        ret = BPRepeat;
    }else if (res == BPInsert)
    {
        pos = pos + 1;
        //返回值有BPrepeat和BPNormal两种
        res = node->insertEntry(entry, pos);
        if((res != BPRepeat ) && node->isoverflow())
        {
            //因为分裂出来的node是在右侧的，所以结点一定不会是上面的第一个，不用在意修改第一个值的问题
            //只是注意mostRightBlock的问题
            //在node的split里面已经执行过新的node的赋值
            //reset entry to new one
            int newID =getFirstEmptyBlock();
            node->split( entry, newID);
            if(nodeID == mostRightBlock)
            {
                mostRightBlock = newID;
                //不用修改，因为最后insert必然还要再updateHeader一次
                //updateHeader();
            }

            if(nodeID == RootID)
            {
                newID =getFirstEmptyBlock();
                BPTreeNode* rootNode = new BPTreeNode(fileName.c_str(), newID, keyLength, false, nodeID);
                rootNode->insertEntry(entry,1);
                RootID = newID;
                delete rootNode;
            }
            //split 之后都是要在上一层插入一个结点的
            ret = BPInsert;
        } else if(res == BPRepeat)
            ret = BPRepeat;
        else ret = BPNormal;

    }

    delete node;
    return ret;
}

int BPTree::insertKey(BPTreeKey & entry) {

    int nodeID = RootID;
    int ret;

    //当没有任何一个结点的时候，自己创建一个root;
    /*
     * 当nodeCount为0的时候认为无节点，取第一个block作为root
     * isleaf = true
     * nextID = -1，表示没有空的节点，需要nodeCount++
     */
    if(nodeCount == 0)
    {
        int newRoot = getFirstEmptyBlock();
        BPTreeNode* node = new BPTreeNode(fileName.c_str(), newRoot, keyLength, true, -1);
        ret = node->insertEntry(entry, 1);

        //更新一波header的内容
        RootID = newRoot;
        firstLeftBlock = RootID;
        mostRightBlock = RootID;

        delete node;

    }else
    {
        ret = insert(nodeID, entry);
    }


    updateHeader();
    return ret;

}
/*
 * 利用递归执行更新B+树的操作
 * 如果结点是叶结点，插入，返回值是BPRepeat说明有重复
 * 如果结点overflow了，分裂成两个节点，修改entry，使其变成新生成结点欲插入的值
 * 回到上一个节点，它也变成了BPInsert的mode，然后会自动对结点进行一次插入
 */

int BPTree::removeKey(BPTreeKey& entry, int* retPointer) {
    int nodeID = RootID;
    int ret = BPNormal;

    if(nodeCount == 0 )
    {
        //todo
        cout << "[BPTree::removeKey] "<<" The index is empty! You can't delete anything!" << endl;
        ret =  BPDeleteFail;
    }else
    {
        BPTreeNode* node = new BPTreeNode(fileName.c_str(), nodeID, dataType);

        //处理根结点即为叶节点的情景
        if(node->isleaf())
        {
            //处理只有一个节点情况
            int pos = node->findPosition_LowerBound(entry);
            if(pos <= node->getNodeSize() && node->getEntry(pos) == entry)
            {
                *retPointer = node->getEntry(pos).getPointer();
                node->deleteEntry(pos);
                if(node->getNodeSize() == 0)
                {
                    removeBlock(nodeID);
                }

            } else
            {
                cout << "[BPTree::removeKey] "<<" This record was not exsited in the index!" << endl;
                ret = BPDeleteFail;
            }


        } else
        {
            int pos = node->findPosition_LowerBound(entry);

            assert(pos != -1);

            //从root开始处理
            BPTreeKey temp(nullptr, -1, dataType);
            ret = remove(nodeID, entry,-1,false, retPointer, temp);

        }
        delete node;
    }
    updateHeader();
//#if DEBUGINDEX
//    debugPrint();
//#endif

    return ret;
}



int BPTree::remove(int nodeID, BPTreeKey &entry, int siblingID, bool isLeftSib, int* retPointer, BPTreeKey & parentKey ) {

    BPTreeNode* node = new BPTreeNode(fileName.c_str(), nodeID, dataType);
    BPTreeNode* sibling = nullptr;
    int ret = BPNormal;

    //找到结点的位置
    int pos = node->findPosition_LowerBound(entry);

    //cout <<"nodeID: " << node->getNodeID() <<  " -- parentKey" << Method::rawdata2int(temp.getKeyRawData()) << endl;
    int  res = node->isleaf() ? BPDelete : remove(node->getkeyPointer(pos), entry, node->getkeyPointer(pos > 0? pos-1:pos + 1), pos > 0, retPointer, node->getEntry(pos > 0? pos:pos+1));


    //先判断是不是叶子，如果不是的话就进入下一层，如果是的话就BPDelete
    //这里逻辑略复杂：很明显这里不是叶节点，getkeyPointer(pos)返回的是下界的值，然后获取正确的下一层结点的number，正确！后面一个是和0比较，通过是否为1来判断是否为左兄弟，正确！
    //parentKey的取法：有左兄弟，应该怎么样？有右兄弟又该取什么？

    // Check for duplicate
    if (res == BPDelete) {
        //开始删除entry
        //因为无论是跟节点还是叶子结点，都能够保证第一个不会被删除
        //内结点：总是删除右侧的那个节点
        //先检查要删除的是否存在
        if (node->isleaf()) {
            if (pos == 0) {
                cout << "[BPTree::remove]line 252" << "The key you want to delete is not in the table!" << endl;
                ret = BPDeleteFail;
            } else if (entry != node->getEntry(pos)) {

                cout << "[BPTree::remove]line 252" << "The key you want to delete is not in the table!" << endl;
                ret = BPDeleteFail;
            }

            //把要删除的key的pointer传出去！
            *retPointer = node->getEntry(pos).getPointer();
            res = node->deleteEntry(pos);
        } else {
            pos = node->findPosition_LowerBound(entry);
            res = node->deleteEntry(pos);
        }





        //如果是根节点
        if(nodeID == RootID)
        {
            if(node->getNodeSize() == 0 ) {
                if (node->getEntry(0).getPointer() == -1) {
                    //彻底清空
                    removeBlock(nodeID);
                    RootID = -1;
                    firstLeftBlock = -1;
                    mostRightBlock = -1;
                    firstEmptyBlock = -1;
                    node->setRemoved();
                } else {

                    removeBlock(RootID);
                    node->setRemoved();
                    RootID = node->getEntry(0).getPointer();


                }
            }
        }

        //删除没有出错并且underflow
        if(res != BPDeleteFail && node->isUnderflow(RootID) && (node->getRemoved()!=true))
        {
            //把兄弟结点new出来
            sibling = new BPTreeNode(fileName.c_str(), siblingID, dataType);

            //borrow from siblings
            if(sibling->getNodeSize() > sibling->getUpperBound())
            {
                ret = node->borrow(entry,sibling, isLeftSib, parentKey);

            }else if(sibling->getNodeSize() == sibling->getUpperBound())
            {
                //需要合并两个结点
                //在mergeRight里面会区分叶节点
                node->mergeRightNode(isLeftSib, sibling, parentKey, entry);

                //返回的entry记录了应该删除哪个结点
                removeBlock(entry.getPointer());


                //维护下mostRightBlock
                if(entry.getPointer() == mostRightBlock)
                {
                    if(isLeftSib)
                        mostRightBlock = sibling->getNodeID();
                    else
                    {
                        if(node->getNodeID() == -1)
                            mostRightBlock = nodeID;
                        else mostRightBlock = node->getNodeID();
                    }
                }
                ret = BPDelete;

            }

            delete sibling;

            //返回true说明需要删除右边的结点
            //在mergeRight操作里面我们需要调整两个节点里的内容
            //同时我们需要调整entry的值，变成一个node的key

        } else if(res == BPDeleteFail)
            ret = BPDeleteFail;
            //node->getNodeID()!= RootID 用来保证parentKey不为null
        else if(node->isleaf() && node->getNodeID()!= RootID && pos == 1)
        {
            if(isLeftSib)
            {
                //只用修改父亲结点的值
                parentKey.setKey(node->getEntry(1).getKeyRawData());
                //cout << Method::rawdata2int(parentKey.getKeyRawData());
                ret = BPChangeEnd;
            } else if(nodeID != firstLeftBlock)
            {
                entry.setKey(node->getEntry(1).getKeyRawData());
                ret = BPChange;
            }else ret = BPNormal;
        }
        else
            ret = BPNormal;


    }else if(res == BPChange)
    {
        //如何一层层修改他的值，如果是动的是右结点，就要一层层修改值
        if(pos != 0)
        {
            node->getEntry(pos).setKey(entry.getKeyRawData());
            node->setDirty();
            ret = BPNormal;
        }else ret = BPChange;

    }else if(res == BPChangeEnd)
    {
        node->setDirty();
        ret = BPNormal;
    } else if (res == BPDeleteFail)
    {
        ret = BPDeleteFail;
    }

    delete node;

    return ret;
}




void BPTree::removeBlock(int id)
{
    BufferManager& manager = MiniSQL::get_buffer_manager();
    Block* block = manager.getBlock(fileName, id);
    memcpy(block->getContent(), &firstEmptyBlock, 4);
    firstEmptyBlock = id;
    nodeCount--;
    if(nodeCount == 0)
    {
        firstLeftBlock = -1;
        mostRightBlock = -1;
        RootID = -1;
        firstLeftBlock = -1;
    }

    //总是删掉右侧结点，所以不用担心firstLeftBlock的问题
    //此刻删除的是唯一的根结点

    block->set_dirty(true);
}



int BPTree::findKey(BPTreeKey &entry) {
    int nodeID = RootID;
    int ret;
    //当rootID为空时，自己创建一个root;
    if(nodeCount == 0 )
    {
        return BPEmpty;
    }else
    {
        BPTreeNode* node;
        int pos;
        while(true)
        {
            node = new BPTreeNode(fileName.c_str(), nodeID, dataType);
            pos = node->findPosition_LowerBound(entry);
#if DEBUG
            assert(pos != -1);
#endif

            if(node->isleaf() )
            {
                //todo:去掉assert
                assert(pos <= node->getNodeSize());
                if (pos == 0) {
                    //cout << "[BPTree::find]line 425" << "The key you want to find is not in the table!" << endl;
                    ret = BPEmpty;
                    break;
                } else
                if(node->getEntry(pos) == entry)
                {
                    entry.setPointer(node->getEntry(pos).getPointer());
                    ret = BPNormal;
                    break;
                } else
                {
                    ret =  BPEmpty;
                    break;
                }

            } else
            {
                nodeID = node->getkeyPointer(pos);
            }
            delete node;
        }

    }
    return ret;
}




////
//void BPTree::scaleFindLeftEnd(BPTreeKey &entry,vector<ptr>& res) {
//    int nodeID = firstLeftBlock;
//    int pos = findKeyNode(entry);
//    BPTreeNode* node;
//    while(true)
//    {
//        node = new BPTreeNode(fileName.c_str(), nodeID, keyLength);
//        if(node->getNodeID() == pos)
//        {
//            pos = node->findPosition_LowerBound(entry);
//            node->mergeVec(res, pos);
//            break;
//        }else
//            node->mergeVec(res, node->getNodeSize());
//        nodeID = node->getEntry(0).getPointer();
//        delete node;
//    }
//    delete node;
//}
//
//int BPTree::findKeyNode(const BPTreeKey &entry) {
//    int nodeID = RootID;
//    //当rootID为空时，自己创建一个root;
//    if(firstEmptyBlock == 1 )
//    {
//#if DEBUG
//        cout <<   "The index is empty!" << endl;
//#endif
//        Error empty("The index is empty!");
//        throw;
//    }else
//    {
//        BPTreeNode* node;
//        int pos;
//        while(true)
//        {
//            node = new BPTreeNode(fileName.c_str(), nodeID, keyLength);
//            pos = node->findPosition(entry);
//            if(node->isleaf() && node->getEntry(pos).key == entry.key)
//            {
//                nodeID = node->getNodeID();
//                delete node;
//                return nodeID;
//            } else if(node->isleaf() && node->getEntry(pos).key != entry.key )
//            {
//#if DEBUG
//                cout <<   "The index is empty!" << endl;
//#endif
//                Error empty("The index is empty!");
//                throw;
//
//            }
//            nodeID = node->getkeyPointer(pos);
//            delete node;
//        }
//
//    }
//}
//
//void BPTree::scaleFindRightEnd(BPTreeKey &entry, vector<ptr> &res) {
//    int nodeID = findKeyNode(entry);
//    BPTreeNode* node = new BPTreeNode(fileName.c_str(), nodeID, keyLength);
//    int pos = node->findPosition(entry);
//    node->mergeVecRight(res, pos);
//    nodeID = node->getEntry(0).getPointer();
//    delete node;
//    while(true)
//    {
//        node = new BPTreeNode(fileName.c_str(), nodeID, keyLength);
//        if(node->getNodeID() == mostRightBlock)
//        {
//            pos = node->findPosition(entry);
//            node->mergeVec(res, pos);
//            break;
//        }else
//            node->mergeVec(res, node->getNodeSize());
//        nodeID = node->getEntry(0).getPointer();
//        delete node;
//    }
//    delete node;
//
//}
//
//
//void BPTree::scaleFind(BPTreeKey &sta_entry, BPTreeKey &end_entry,vector<ptr> &res) {
//    int nodeID = findKeyNode(sta_entry);
//    BPTreeNode* node = new BPTreeNode(fileName.c_str(), nodeID, keyLength);
//    int pos = node->findPosition(sta_entry);
//    node->mergeVecRight(res, pos);
//    nodeID = node->getEntry(0).getPointer();
//    delete node;
//    int endNodeID = findKeyNode(sta_entry);;
//    while(true)
//    {
//        node = new BPTreeNode(fileName.c_str(), nodeID, keyLength);
//        if(node->getNodeID() == endNodeID)
//        {
//            pos = node->findPosition(end_entry);
//            node->mergeVec(res, pos);
//            break;
//        }else
//            node->mergeVec(res, node->getNodeSize());
//        nodeID = node->getEntry(0).getPointer();
//        delete node;
//    }
//    delete node;
//}
//
/*
 * Todo: fileManager 需要修改
 * getFirstRecord()
 * getRecord_Attribute(const string & attribute_name, recordID )
 * 如何知道长度？
 */

void BPTree::initialIndex(const string & table_name,const string & attribute_name,int recordLength, int posinRecord) {
    FileManager file("data/" + table_name);
    char * rawData = new char[recordLength];
    int pos;
    while((pos = file.getNextRecord(rawData)) > 0)
    {
        //如何剪切rawData指针存到entry里
        Method::Cutrawdata(dataType, posinRecord, rawData);
        BPTreeKey entry(rawData, pos, dataType);
        insertKey(entry);
    }
}

#ifdef DEBUG
// Print tree structure
void BPTree::debugPrint()
{
    cerr << "DEBUG: [BPTree::debugPrint] Debug print start." << endl;
    cerr << "Node Count = " << nodeCount << ", first empty = " << firstEmptyBlock <<", firLestBlock = "<< firstLeftBlock<< endl;
    cerr << "MosrRightBlock = " << mostRightBlock << endl;
    if (RootID >= 0)
    {
        cerr << "Root = " << RootID << endl;
        debugPrint(RootID);
    }
    else
        cerr << "Empty tree." << endl;
    cerr << "DEBUG: [BPTree::debugPrint] Debug print end." << endl;
}
#endif


#ifdef DEBUG
// Recursive function for tree structure printing
void BPTree::debugPrint(int id)
{
    BPTreeNode* node = new BPTreeNode(fileName.c_str(), id, keyLength);
    cerr << "nodeSize =  " << node->getNodeSize() << endl;
    cerr << "Block id = " << id << ", isLeaf = " << node->isleaf() << endl;
    cerr << "Keys:";

//    for (int i = 1; i <= (node->getNodeSize()>10? 10:node->getNodeSize()); i++)
    for (int i = 1; i <= node->getNodeSize(); i++)
    {
        if(i % 60 == 0)
        {
            cerr << endl;
        }
        const char* k = node->getEntry(i).getKeyRawData();
        cerr << (k) << " - ";
    }
    cerr << endl;
    cerr << "Pointers: ";
    for (int i = 0; i <= node->getNodeSize(); i++)
    {
        if(i % 30 == 0)
        {
            cerr << endl;
        }
        cerr << " " << node->getEntry(i).getPointer();
    }

    cerr << endl;

    if (!node->isleaf())
        for (int i = 0; i <= node->getNodeSize(); i++)
            debugPrint(node->getEntry(i).getPointer());

    delete node;
}
#endif
