//
// Created by emma on 2018/6/14.
//

#include "BPTreeNode.h"

/*
 * 构造了一个空结点
 * 即node里没有存任何东西
 */
BPTreeNode::BPTreeNode(const char *_filename, int _id, int data_type, bool isLeaf, int nextNodeId):
        fileName(_filename), nodeId(_id), data_type(data_type), isLeaf(isLeaf)
{
    keyLength = Method::getLengthFromType(data_type);
    isDirty = true;
    isRemoved = false;
    nodeSize = 0;
    nodeCapability = ((4096 - 12) / (keyLength + 4));
    //存NextNodeID 是单独存的
    //对内结点来说第一个是空的
    //统一处理吧！
    keys.emplace_back(nullptr, nextNodeId, data_type);

}


BPTreeNode::BPTreeNode(const char *_filename, int _id, int data_type):
        fileName(_filename), nodeId(_id), data_type(data_type)
{

    keyLength = Method::getLengthFromType(data_type);
    //调用Buffer Manager去得到data
    BufferManager& manager = MiniSQL::get_buffer_manager();
    Block* block = manager.getBlock(_filename, nodeId);
    char* data = block->getContent();

    //初始化
    isDirty = false;
    isRemoved = false;


    //得到nodeSize的信息
    char* temp = new char[4];
    memcpy(temp, data, 4);
    nodeSize = Method::rawdata2int(temp);

    //得到是否为叶节点的信息
    memcpy(temp, data+4, 4);
    isLeaf = (bool)Method::rawdata2int(temp);


    //更新nodecapability
    nodeCapability = ((4096 - 12) / (keyLength + 4));


    //更新第一个指针，
    //vector的第0位对叶子结点来说放下一个nodeID
    //对于非叶子结点放下一层的nodeID
    memcpy(temp, data+8, 4);
    keys.emplace_back(nullptr, Method::rawdata2int(temp), data_type);

    /*
     * 其实都是一样的，无需区分对一个非叶子结点的key，它也是有pointer的，也是有key的
     */

    int bias = 12;
    char* keyPtr;
    for (int i = 0; i < nodeSize; i++) {
        //取出key和pointer
        keyPtr = new char[keyLength];
        memcpy(keyPtr, data + bias, keyLength);
        memcpy(temp, data + bias + keyLength, 4);
        //把key+id放进vector
        keys.emplace_back(keyPtr, Method::rawdata2int(temp), data_type);
        bias += keyLength + 4;
    }
    delete[] temp;
}


BPTreeNode::~BPTreeNode() {
    //remove的block在前面的地方就需要更新，此时不用管
    if(isDirty && !isRemoved)
    {
        BufferManager & manager = MiniSQL::get_buffer_manager();
        Block* block = manager.getBlock(fileName, nodeId);
        char* data = block->getContent();

        //更新nodesize
        int leaf = isLeaf;
        memcpy(data, &nodeSize, 4);
        memcpy(data+4, &leaf, 4);

        //把所有数据存进去
        int recordPointer = keys[0].getPointer();
        memcpy(data + 8, &(recordPointer), 4);
        int bias = 12;
        for(int i=1; i <= nodeSize; i++)
        {
            memcpy(data + bias, keys[i].getKeyRawData(), keyLength);
            recordPointer = keys[i].getPointer();
            memcpy(data + bias + keyLength, &recordPointer, 4);
            bias += keyLength + 4;
        }
        block->set_dirty(true);
    }


    //delete 那些keys 里面char* 的内容

//    for(int i=1; i<=nodeSize; i++ )
//    {
//        if(keys[i].getKeyRawData() != nullptr)
//        {
//            delete[] keys[i].getKeyRawData();
//        }
//    }
}

int BPTreeNode::insertEntry(BPTreeKey& entry, int pos)  {

    //BPTreeKey copy(entry.getKeyRawData(), entry.getPointer(), data_type);
    if(pos == nodeSize +1 )
    {
        keys.push_back(entry);
    }else if( keys[pos] == entry)
    {
        return BPRepeat;
    }else
    {
        keys.insert(keys.begin()+pos, entry);
    }

    isDirty = true;
    nodeSize++;
    return BPNormal;
}


/*
 * 分裂的时候会获取一个新的节点
 *
 */
void BPTreeNode::split(BPTreeKey &entry, int nodeID) {

    /*
     * 分裂法则：capability为奇数，则nodeSize为偶数，那么两个node各对半
     * 反之
     */
    if(isLeaf)
    {
        BPTreeNode* newNode = new BPTreeNode(fileName.c_str(),nodeID, data_type, isLeaf, this->keys[0].getPointer());
        for(int i = nodeSize/2 + 1; i <= nodeSize; i++)
        {
            newNode->keys.emplace_back(this->keys[i].getKeyRawData(), this->keys[i].getPointer(),data_type);
        }
        newNode->nodeSize = nodeSize / 2 + 1;

        //把第一个结点的值赋给entry
        entry.setKey(newNode->getEntry(1).getKeyRawData(), nodeID);
        newNode->isDirty = true;
        delete newNode;

    }else
    {
        BPTreeNode* newNode = new BPTreeNode(fileName.c_str(),nodeID, data_type, isLeaf, keys[nodeSize/2 +1].getPointer());
        for(int i = nodeSize/2 + 2; i <= nodeSize; i++)
        {
            newNode->keys.emplace_back(this->keys[i].getKeyRawData(), this->keys[i].getPointer(),data_type);
        }
        newNode->nodeSize = nodeSize / 2 ;
        entry.setKey(keys[nodeSize/2 +1].getKeyRawData(), nodeID);
        newNode->isDirty = true;
        delete newNode;
    }
    nodeSize /= 2;
    isDirty = true;
    keys.resize((unsigned long)nodeSize+1);
}

BPTreeKey& BPTreeNode::getEntry(int pos) {

    //todo 应该被消灭掉的报错
    if(pos > nodeSize)
    {
        cout << "BPTreeNode::getEntry" << "你读取的key超过了nodeSize"  << endl;
        throw;
    }else
    return keys[pos];
}

void BPTreeNode::setKey(int pos, BPTreeKey& entry) {
#if DEBUG
    if (pos > nodeSize || pos < 0)
    {
        cerr << "ERROR: [BPTreeNode::setKey] Position " << pos << " out of range!" << endl;
        return;
    }
#endif
    isDirty = true;
    keys[pos] = entry;

}

int BPTreeNode::deleteEntry(int pos) {
    keys.erase(keys.begin()+pos);
    nodeSize--;
    isDirty = true;
    return BPNormal;
}



int BPTreeNode::getkeyPointer(int keyID) {
    return keys[keyID].getPointer();
}

int BPTreeNode::borrow(BPTreeKey &entry, BPTreeNode *sibling, bool isLeftSib, BPTreeKey &parentKey) {
    //左兄弟就把左边最后一位让出来，
    int ret = BPNormal;
    if(isLeftSib)
    {
        if(isLeaf)
        {
            //cout <<"BPTreeNode::borrow: line 237 : " <<Method::rawdata2int(sibling->keys[sibling->getNodeSize()].getKeyRawData()) << endl;
            keys.emplace(keys.begin()+1, sibling->keys[sibling->getNodeSize()].getKeyRawData(), sibling->keys[sibling->getNodeSize()].getPointer(), data_type);
            sibling->deleteEntry(sibling->getNodeSize());
            //修改父节点的值
            parentKey.setKey(keys[1].getKeyRawData());


        }else
        {
            //插入第零个指针
            keys.emplace(keys.begin(), nullptr, sibling->keys[sibling->getNodeSize()].getPointer(),data_type );
            //设置第一个指针
            keys[1].setKey(parentKey.getKeyRawData());

            //修改父节点的值为借过来的节点的值
            parentKey.setKey(sibling->keys[sibling->getNodeSize()].getKeyRawData());

            //删除左兄弟的第一个结点
            sibling->deleteEntry(sibling->getNodeSize());
        }




    }else
    {
        if(isLeaf)
        {
            keys.emplace_back(sibling->keys[1].getKeyRawData(), sibling->keys[1].getPointer() ,data_type);
            sibling->deleteEntry(1);
            parentKey.setKey(sibling->keys[1].getKeyRawData());

        } else
        {
            keys.emplace_back(parentKey.getKeyRawData(), sibling->keys[0].getPointer() ,data_type);
            //删除兄弟的第一个节点
            sibling->deleteEntry(0);

            //修改父节点的key
            parentKey.setKey(sibling->keys[0].getKeyRawData());

            sibling->keys[0].setKey(nullptr);
        }
    }


    isDirty = true;
    nodeSize++;
    return BPChangeEnd;
}

void BPTreeNode::mergeRightNode(bool isLeftSib, BPTreeNode *sibling, const BPTreeKey& parentKey, BPTreeKey& entry) {
    if(isLeftSib)
    {

        if(isLeaf)
        {
            //修改左兄弟指向下一个结点的指针
            BPTreeKey borrowEntry (nullptr,getEntry(0).getPointer() ,data_type);
            sibling->setKey(0, borrowEntry);

            //设置返回要被修改的值
            entry.setKey(getEntry(1).getKeyRawData(), nodeId);



        }else //针对非叶节点
        {
            //修改左兄弟指向下一个结点的指针
            sibling->keys.emplace_back(parentKey.getKeyRawData(), keys[0].getPointer(),data_type);

            //设置返回要被修改的值
            entry.setKey(parentKey.getKeyRawData(), nodeId);

        }

        //逐一移动，把第一个key到最后一个key都搬过去
        for(int i = 1; i <= this->getNodeSize(); i++)
        {
            sibling->keys.emplace_back(keys[i].getKeyRawData(), keys[i].getPointer(),data_type);
        }

        sibling->isDirty= true;
        sibling->nodeSize =(int)(sibling->keys.size() - 1);
        setRemoved();

    }else
    {
        //右兄弟

        if(isLeaf)
        {
            //修改左兄弟指向下一个结点的指针
            BPTreeKey borrowEntry (nullptr,sibling->getEntry(0).getPointer() ,data_type);
            setKey(0, borrowEntry);

            //设置返回要被修改的值
            entry.setKey(sibling->getEntry(1).getKeyRawData(), sibling->nodeId);


        }
        else
        {
            //修改左兄弟指向下一个结点的指针
            keys.emplace_back(parentKey.getKeyRawData(), sibling->keys[0].getPointer(),data_type);

            //设置返回要被修改的值
            entry.setKey(parentKey.getKeyRawData(), sibling->nodeId);
        }

        //逐一移动，把第一个key到最后一个key都搬过去
        for(int i = 1; i <= sibling->getNodeSize(); i++)
        {
            keys.emplace_back(sibling->keys[i].getKeyRawData(), sibling->keys[i].getPointer(), data_type);
        }

        isDirty= true;
        nodeSize = (int)(keys.size() - 1);
        sibling->setRemoved();

//        entry = sibling->getEntry(1);
//        entry.setPointer(sibling->getNodeID());
//
//        //插入第一个特殊值
//        //取右兄弟的第一个值
//        //把父节点的key赋给它
//        //插入到当前结点
//        //删去右兄弟的第一个
//        BPTreeKey borrowEntry = sibling->getEntry(0);
//        borrowEntry.setKey(parentKey.getKeyRawData());
//        insertEntry(borrowEntry,getNodeSize()+1);
//
//        int keyCount = sibling->getNodeSize();
//        for(int i = 1; i<= keyCount; i++)
//        {
//            borrowEntry = sibling->getEntry(1);
//            sibling->deleteEntry(1);
//            insertEntry(borrowEntry, sibling->getNodeSize()+1);
//        }
//
//        sibling->deleteEntry(0);
//        sibling->setRemoved();

    }
}

int BPTreeNode::findPosition_LowerBound(const BPTreeKey &key) {


    int ret = findPosition_UpperBound(key);
    if(ret == nodeSize+1)
        ret--;
    else if(this->keys[ret] !=  key)
    {
        ret--;
    }

    return  ret;

}
/*
 * 首先明确：insert（begin（）+ pos）插入的是第pos位，如果不算第一个的话
 * findPosition 目的是为了找出比key大的第一个pos，然后插入到它，就是把pos位的东西赶到后面一格去
 *
 * 1。 如果nodeSize = 0 ，返回-1
 * 2。 如果nodeSize = 1，如果key大于high，返回2； 如果key小于high，返回middle
 * 3。 如果nodeSize 为某一个值，并且key 也是可行的
 *
 * 前面的循环找到的上界，（需要测试）
 * 论及getKeyPointer 对叶节点我需要的是个上界，对内子结点我需要的是个下界
 * 论及插入，需要的都是上界
 * 把相等归属为下界
 */

//int BPTreeNode::findPosition_UpperBound(const BPTreeKey &key)
//{
//    for(int i=1; i<=nodeSize; i++)
//    {
//        if(keys[i] < key)
//            return i;
//    }
//
//    return nodeSize+1;
//}





int BPTreeNode::findPosition_UpperBound(const BPTreeKey &key) {

    int low = 1, high = nodeSize;
    int middle = (low + high) / 2;

    //todo
    if(low > high )
    {
        cout <<"BPTreeNode::findPosition "<< "The node is empty!" << endl;
        return -1;
    }

//    //需要插在最后一个
//    if(keys[high] < key ) return nodeSize+1;
//
//    while (high > low)
//    {
//        if( keys[middle] > key )
//            high = middle;
//        else
//        if(keys[middle] == key)
//            return middle;
//        else
//            low = middle + 1;
//
//        middle = (low + high) / 2;
//    }

    if(keys[high] < key ) return nodeSize+1;

    while (high > low)
    {
        if( keys[middle] > key )
            high = middle;
        else
            low = middle + 1;

        middle = (low + high) / 2;
    }
    return middle ;
}

bool BPTreeNode::isUnderflow(int rootID) {
    if(nodeId != rootID)
     return  nodeSize < ceil(nodeCapability / 2.0);
    else
        return false;

}
