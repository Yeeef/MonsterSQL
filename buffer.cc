#include "buffer.h"

/* ---------------------------------------------------------------------*/
/* BufferNode */
BufferNode::BufferNode() : block(nullptr), reference_bit(false), next(nullptr)
{
}
BufferNode::~BufferNode()
{
}

inline void BufferNode::set_next(BufferNode *next)
{
    this->next = next;
}

inline BufferNode *BufferNode::get_next()
{
    return next;
}

inline Block *BufferNode::get_block()
{
    return block;
}

inline bool BufferNode::get_reference_bit()
{
    return reference_bit;
}

inline void BufferNode::set_reference_bit(bool bit)
{
    reference_bit = bit;
}

//注意这个函数同时会把 ref_bit变为1
inline void BufferNode::set_block(Block *block)
{

    reference_bit = true;
    this->block = block;
}
/* ---------------------------------------------------------------------*/

/* ---------------------------------------------------------------------*/

BufferManager::BufferManager() : max_block_count(4)
{
    // 建立一个循环大小为max的循环链表
    BufferNode *current_node = new BufferNode();
    clock_ptr = current_node;

    for (int i = 0; i < max_block_count - 1; i++)
    {
        BufferNode *new_node = new BufferNode();
        current_node->set_next(new_node);
        current_node = new_node;
    }
    // 循环链表最后一下连接
    current_node->set_next(clock_ptr);
}

BufferManager::~BufferManager()
{
    // 把循环链表清空

    BufferNode *cleaner = clock_ptr;
    for (int i = 0; i < max_block_count; i++)
    {
        BufferNode *temp = cleaner->get_next();
        delete cleaner;
        cleaner = temp;
    }
}

/* TODO
 * 从文件里读bufferblock✅
 * 时钟算法替换以后应该把指针下移一个✅
 */

Block *BufferManager::getBlock(const string &fileName, int block_id) throw(Error)
{
    string BlockMapID;
    getMapName(BlockMapID, fileName, block_id);
    // 现在自己里面找
    auto search = Name2Node.find(BlockMapID);

    if (search == Name2Node.end())
    {
        //没找到，要从文件里取
        return getBlockFromFile(fileName, block_id);
    }
    else
    {
        //找到了，直接返回
        return Name2Node[BlockMapID]->get_block();
    }
}

// 从文件里读取block同时插入循环链表中✅
// 同时还要加入map中！✅
Block *BufferManager::getBlockFromFile(const string &fileName, int block_id) throw(Error)
{
    char * content= new char[BLOCK_SIZE];
    // 打开文件以二进制格式
    ReadBlockContent(fileName, block_id, content);

    Block *block = new Block(fileName, block_id, content);

    // 把这个block填进buffernode，需要时钟算法,此时clock_ptr指向更新的block_node *
    addBlock(block);

#ifdef DEBUGBUFFER
    cout << "[BufferManager::addBlock] '" << fileName << "' " << endl;
#endif

    return block;
}

/* 加入新结点，同时更新map */ ✅
void BufferManager::addBlock(Block *block) throw(Error)
{
    //把block加入时钟循环链表
    // 需要找到被替代的blockNode

    while (clock_ptr->get_reference_bit() == true)
    {
        clock_ptr->set_reference_bit(false);
        clock_ptr = clock_ptr->get_next();
    }

    // 找到了第一个 refer_bit 为0的块
    // 进行替换, 先要判断这个block是不是脏的或者它直接就是个null
    Block *replace_block = clock_ptr->get_block();

    if (replace_block != nullptr)
    {


        if (replace_block->isDirty() == true)
        {

            WriteBlockBack(replace_block);
        }
        string map_name;
        getMapName(map_name, replace_block->get_filename(), replace_block->get_blockID());
        //map要更行

        Name2Node.erase(map_name);
        delete replace_block;
    }

    //注意这个函数同时会把 ref_bit变为1
    clock_ptr->set_block(block);
    string BlockMapID;
    getMapName(BlockMapID, block->get_filename(), block->get_blockID());
    Name2Node.insert({BlockMapID, clock_ptr});
    clock_ptr = clock_ptr->get_next();
}

/* 通过文件名删除对应的块，不需要写回文件 */
/* 注意要更新delete对应的block，然后再置为nullptr; 更新block的map */
void BufferManager::DeleteBlockByFile(const string &fileName)
{
    BufferNode *finder = clock_ptr;

    for (int i = 0; i < max_block_count; i++)
    {
        Block *block = finder->get_block();
        string blockFileName = block->get_filename();
        int block_id = block->get_blockID();
        string map_name;
        getMapName(map_name, fileName, block_id);
        // 不会析构我的 BufferNode *
        Name2Node.erase(map_name);

        if (blockFileName == fileName)
        {
            delete block;
            finder->set_block(nullptr);
        }
        finder = finder->get_next();
    }
}

void BufferManager::WriteBlockBack(Block *block) throw(Error)
{

    string file_name = block->get_filename();
    int block_id = block->get_blockID();
    char *content = block->getContent();
#ifdef DEBUGBUFFER
    int temp1, temp2, temp3;
    memcpy(&temp1, block->getContent(), INT_LENGTH);
    memcpy(&temp2, block->getContent() + 4, 4);
    memcpy(&temp3, block->getContent() + 8, 4);
    cout << "[FileManager::updateMeta] " << temp1 << " " << temp2 << " "
         << temp3 << endl;
#endif

    //需要先写回文件
    FILE *file = fopen(Method::AbsolutePath(file_name).c_str(), "rb+");

    // 文件打开失败
    if (file == NULL)
    {
        string err_info("[BufferManager::WriteBlockBack]: open(wb) file: '");
        err_info += file_name;
        err_info += string("' error");
        throw Error(err_info);
        return;
    }

    // 文件打开成功
    fseek(file, block_id * BLOCK_SIZE, SEEK_SET);
#ifdef DEBUGBUFFER
    cout << "[BufferManager::WriteBlockBack] " << block->get_filename() << " " << block_id;
    int record_length, first, count;
    memcpy(&record_length, content, 4);
    memcpy(&first, content + 4, 4);
    memcpy(&count, content + 8, 4);
    cout << " length: " << record_length << " first: " << first << " count: " << count << endl;

#endif
    fseek(file, block_id * BLOCK_SIZE, SEEK_SET);

    fwrite(content, BLOCK_SIZE, 1, file);
    fclose(file);
}
void BufferManager::ReadBlockContent(string file_name, int block_id, char *content) throw(Error)
{
#ifdef DEBUGBUFFER
    cout << "[BufferManager::ReadBlockContent]: '" << file_name << "' " << endl;
#endif
    FILE *file = fopen(Method::AbsolutePath(file_name).c_str(), "rb");

    if (file == nullptr)
    {

        //文件未能正常打开

        string err_info("[BufferManager::getBlockFromFile]: file: '");
        err_info += file_name;
        err_info += string("' can't open");
        Error err(err_info);
        throw err;
        return;
    }
    else
    {

        fseek(file, block_id * BLOCK_SIZE, SEEK_SET);
        fread(content, BLOCK_SIZE, 1, file);

        fclose(file);
    }
}

void BufferManager::getMapName(string &map_name, const string &fileName, const int block_id)
{

    stringstream ss;
    ss << block_id;
    string block_id_str;
    ss >> block_id_str;
    //
    map_name = fileName + block_id_str;
}
void BufferManager::print()
{
    BufferNode *printer = clock_ptr;
    for (int i = 0; i < max_block_count; i++)
    {
        Block *block = printer->get_block();
        if (block == nullptr)
        {
            cout << "null"
                 << " ";
        }
        else
        {
            cout << block->get_filename() << block->get_blockID() << " ";
        }
        printer = printer->get_next();
    }
    cout << endl;
}

void BufferManager::WriteAllBack()
{
    for(int i = 0; i < max_block_count; i++)
    {
        Block * block = clock_ptr->get_block();
        if(block != nullptr && block->isDirty() == true)
            WriteBlockBack(block);
        clock_ptr = clock_ptr->get_next();
    }

}