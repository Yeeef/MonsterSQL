#ifndef CONFIG_H
#define CONFIG_H
#include <iostream>
#include <vector>
#include <sstream>
#include <map>
#include <unordered_map>
#define DEBUG -1
#define PATH "/Users/yee/Desktop/monster-sql/MiniSQL/cmake/"

#define BLOCK_SIZE 4096
#define MAX_TABLE_NAME 32
#define MAX_ATTRIBUTE_NAME 32
#define MAX_INDEX_NAME 32
#define INT_LENGTH sizeof(int)
#define FLOAT_LENGTH sizeof(float)
#define CHAR_LENGTH 255
#define TYPE_NULL 0
#define TYPE_CHAR 255 //允许的最短长度实际上是4
#define MIN_TYPE_LENGTH 4
#define TYPE_INT 256
#define TYPE_FLOAT 257

#define COND_EQ 0
#define COND_NE 1
#define COND_LT 2
#define COND_GT 3
#define COND_LE 4
#define COND_GE 5

using namespace std;

class Attribute
{
    // all this can be read from the catalog
    private:
    // attribute name
    string name;
    // index_name if there is any
    string index_name;
    // which type is the Attribute
    int type;
    // the length of the Attribute
    int length;

    bool isPrimary;
    bool isUnique;
    bool isIndex;


    public:
    Attribute(string name, bool isPrimary = false, bool isUnique = false)
    : name(name), isPrimary(isPrimary), isUnique(isUnique) {}
    ~Attribute(){};

    string get_name() const { return name; }
    string get_index_name() const { return index_name; }
    int get_type() const {return type; }
    int get_length() const {return length; }
    bool is_primary() { return isPrimary; }
    bool is_unique() { return isUnique; }
    bool is_index() const {return isIndex;}


};

class Error
{
    private:
    string error_info;

    public:
    Error(string error_info) : error_info(error_info) {}
    ~Error() {};

    void print_error() { cout << error_info << endl; }
    
};

class Block
{
    // which file
    string filename;
    // a pointer / an address in a file
    int block_id;

    // 最后一条记录的id，为了计算下一个record应该插在哪里
    //short last_record_id;

    //bool full;
    bool dirty;
    bool pin;

    char * content;

    public:
    // Constructor
    Block(const string & filename, int block_id, char * content)
    : filename(filename), block_id(block_id), content(content)
    {
        dirty = false;
        pin = false;
    }
    
    ~Block()
    {
        delete [] content;
    }
    void set_filename(const string & filename) { this->filename = filename; }
    void set_block_id(const int block_id) { this->block_id = block_id; }
   // void set_record_length( const int record_length) { this->record_length = record_length; }
    //void set_free_list_header(const int header) { this->free_list_header = header; }
    void set_dirty(bool status) { this->dirty = status; }
    //void set_full(bool status) {this->full = status; }
    void set_pin(bool status) { this->pin = status; }
    void set_content(const char * content)
    {
        memcpy(this->content, content, BLOCK_SIZE);
    }
    char* getContent() { return content; }
    string get_filename() {return filename; }
    bool isDirty() { return dirty; }
    int get_blockID() { return block_id; }
    void print_info()
    {
        cout << "fileName: '" << filename << "' ";
        cout << "block_id: '" << block_id << "' ";
        cout << "status: dirty: " << dirty << endl;
    }



};

// read from metafile
/* TODO
 * 有了Name2Attri的映射以后，是不是Attri2Index, UniqueAttri都可以暂时丢弃？
 * Attri2Index不能丢弃，因为它可以直接让我知道谁有index
 * UniqueAttri暂时也留着把
 * 
 */
class Index
{
    private:
    string index_name;
    string table_name;
    string attri_name;
    public:
    Index(string & index_name, string & table_name, string & attri_name) 
    : index_name(index_name), table_name(table_name), attri_name(attri_name){}



};
class Table
{
    private:
    string table_name;
    int record_length; //通过计算得来
    int attribute_count; // 通过上一步metadata已经填好
    unordered_map <string, Attribute> Name2Attri; //名字索引到属性
    //下面的index*不需要我来delete，上一步已经填好
    unordered_map <string, Index*> Attri2Index; //属性名索引到Index名
    unordered_map <string, bool> isUnique;
    string primary_name; 

    public:
    Table(string table_name, int record_length, int attribute_count)
    : table_name(table_name), record_length(record_length), attribute_count(attribute_count){ }

    // 专门为metadata准备的
    Table(string & table_name, int attribute_count) 
    : table_name(table_name), attribute_count(attribute_count) { }

    string get_table_name() {return table_name;};
    int get_record_length() { return record_length; }
    int get_attribute_count() {return attribute_count; }
    void set_map_Attri2Index(string & attribute_name, Index * index)
    {
        Attri2Index.insert({attribute_name, index});
    }
    //vector <Attribute> & get_attribute_set() { return attribute_set; }
    //vector <string> & get_indices_name() { return indices_name; }

    bool isValidInput(const vector <string> & insert_data, const vector <int> & type, vector <char*> & raw_data) throw(Error);
    bool rawVec2rawData(const vector <char*> & raw_Vec, char * raw_data) throw(Error);

};



class Method
{
    public:
    static void vec2rawdata(const vector <char *> & raw_vec );
    //这个函数内部有new
    static char * string2rawdata(const string & str, const int type);
    static int rawdata2int(const char * rawdata);
    static float rawdata2float(const char * rawdata);
    // 下面两个函数格外注意，返回的指针就是原来的指针
    void int2rawdata(int data, char * rawdata);
    void float2rawdata(float data, char * rawdata);
    static const int getLengthFromType(int type);
    static void createFile(string file_name);
    static string AbsolutePath(string & file_name);
    static void Cutrawdata(int type, int beginPos, char * rawdata);

};

// now especially for the B+ tree
// record_id采用绝对地址没有问题，因为record一般不会移动
// 注意 record_id * length才是绝对的地址，为何不直接作为绝对呢？
class ptr
{
    
    int id; // it can be a block_id(internal node) or a record_id(leaf)
    char * rawdata;


    public:
    ptr(int id = 0) : id(id)
    {
        rawdata = new char[4];
        memcpy(rawdata, reinterpret_cast<char*>(&id), INT_LENGTH);
    }
    ~ptr()
    {
        delete [] rawdata;
    }

    int get_id() const { return id; }
    char * get_rawdata() const { return rawdata;}

};



#endif