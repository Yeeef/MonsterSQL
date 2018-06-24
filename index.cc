#include "index.h"
#include "BPTree.h"
#include "BPTreeNode.h"

bool IndexManager::insert(const string & index_name, const char * raw_data, int data_type, int recordID) throw(Error)
{
    BPTreeKey entry(raw_data, recordID, data_type);
    BPTree* tree = new BPTree("index/" + index_name, data_type);
    int ret = tree->insertKey(entry);

    delete tree;

    if(ret  == BPRepeat)
    {
        Error repeat("This record has exsited in the index!");
        throw;
    }

    return true;

}

int IndexManager::find(const string &index_name, const char *raw_data, int data_type) {
    BPTreeKey entry(raw_data, data_type);
    BPTree* tree = new BPTree("index/" + index_name, data_type);
    int ret = tree->findKey(entry );

    delete tree;
    if(ret  == BPEmpty)
    {
        Error repeat("This record has exsited in the index!");
        throw;
    }

    return entry.getPointer();
}

void IndexManager::createFile(const string& file_name)
{
    FILE *file = fopen((PATH + file_name).c_str(), "wb");
    if (file == nullptr)
    {
        cout << "[Method::createFileforIndex] cannot open file '" << file_name << "' " << endl;
        return;
    }
    /*
     *  待储存的header：
     *  firstEmptyBlock 为 -1    nodeCount 0  firstLeftBlock -1  mostRightBlock -1   RootID
     *
     */
    int header[] = {-1, 0, -1, -1, -1};

    fwrite(header, 4, 5, file);
    fclose(file);
}


bool IndexManager::createIndex(const string & table_name, const string & attribute_name, const string &index_name, int data_type, int posinRecord, bool isBeginning) {

    //假定API传给我的时候已经有了table，并且已经在catalogmanager那里备案了。
    //如果保证indexname不重名的话，我其实不需要这些前面两个信息
    //需要初始化一些信息
    // 对中间建的index需要record Count，（需要firstEmptyBlock，需要？这两个可以通过一直插入来解决）
    /*
     * 如果不附加前面的信息，也就要求了整个MiniSQL的索引名字不能一样
     * createFileforIndex(string fileName,int [] header, int size, int count)
     */
    int attriLength = Method::getLengthFromType(data_type);

    createFile("index/" + index_name);
    if(! isBeginning )
    {
        //todo: initialIndex 是method函数，需要attribute 在record的位置才能够剪切
        BPTree* tree = new BPTree("index/" + index_name, data_type);
        tree->initialIndex(table_name, attribute_name,attriLength, posinRecord);
    }
    return true;
}

bool IndexManager::dropIndex(const string &index_name) {
    //直接清理掉index所属的文件
    Method::deleteFile("index/" + index_name);
    return true;

}

int IndexManager::remove(const string &index_name, const char *raw_data, int data_type) {

    BPTreeKey entry(raw_data, data_type);
    BPTree* tree = new BPTree("index/" + index_name, data_type);
    int recordPointer;
    int* retPointer = &recordPointer;
    int ret = tree->removeKey(entry, retPointer);

    delete tree;

    if(ret == BPDeleteFail)
    {
        Error repeat("This key was existed in the Index Table!");
        throw;
    }

    return recordPointer;
}