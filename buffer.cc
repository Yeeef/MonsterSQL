#include "buffer.h"

/* ---------------------------------------------------------------------*/
/* BufferNode */
BufferNode::BufferNode() : block(nullptr), reference_bit(false), next(nullptr)
{


}
BufferNode::~BufferNode()
{

}

void BufferNode::set_next(BufferNode * next)
{
    this->next = next;
}

BufferNode * BufferNode::get_next()
{
    return next;
}

Block * BufferNode::get_block()
{
    return block;
}
/* ---------------------------------------------------------------------*/

/* ---------------------------------------------------------------------*/

BufferManager::BufferManager() : max_block_count(8)
{
    // 建立一个循环大小为max的循环链表
    BufferNode * current_node = new BufferNode;
    clock_ptr = current_node;

    for(int i = 0; i < max_block_count - 1; i++)
    {
        BufferNode * new_node = new BufferNode;
        current_node->set_next(new_node);
        current_node = new_node;
    }
    // 循环链表最后一下连接
    current_node->set_next(clock_ptr);

    
}

BufferManager::~BufferManager()
{
    // 把循环链表清空

    BufferNode * cleaner = clock_ptr;
    for(int i = 0; i < max_block_count; i++)
    {
        BufferNode * temp = cleaner->get_next();
        delete cleaner;
        cleaner = temp;
    }

}

Block *BufferManager::getBlock(const string &fileName, int block_id)
{
    stringstream ss;
    ss << block_id;
    string block_id_str;
    ss >> block_id_str;
    string BlockMapID = fileName + block_id_str;
    // 现在自己里面找
    auto search = Name2Node.find(BlockMapID);

    if(search == Name2Node.end())
    {
        //没找到，要从文件里取

    }
    else
    {
        //找到了，直接返回
        return Name2Node[BlockMapID]->get_block();
    }
    return nullptr;
}