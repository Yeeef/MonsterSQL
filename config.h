#ifndef CONFIG_H
#define CONFIG_H
#include <iostream>
#include <vector>
#include <sstream>
#include <map>
#include <unordered_map>
#define DEBUG -1

#define BLOCK_SIZE 4096
#define MAX_TABLE_NAME 32
#define MAX_ATTRIBUTE_NAME 32
#define INT_LENGTH sizeof(int)
#define FLOAT_LENGTH sizeof(float)
#define CHAR_LENGTH 255
#define TYPE_NULL 0
#define TYPE_CHAR 1
#define TYPE_INT 2
#define TYPE_FLOAT 3

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
    char type;
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
    // a pointer / an address
    int block_id;
    // a record length
    int record_length;
    // free_list_header, point to the first deleted record
    short free_list_header;
    // attention: free_list_header occupies first 2 bytes in a block

    // 最后一条记录的id，为了计算下一个record应该插在哪里
    short last_record_id;

    bool full;
    bool dirty;
    bool pin;

    char content[BLOCK_SIZE];

    public:
    // Constructor
    Block(const string & filename, int block_id, int record_length)
    : filename(filename), block_id(block_id), record_length(record_length)
    {
        // point to itself
        free_list_header = 0;
        // record_length is 0
        record_length = 0;
        dirty = false;
        pin = false;
    }
    void set_filename(const string & filename) { this->filename = filename; }
    void set_block_id(const int block_id) { this->block_id = block_id; }
    void set_record_length( const int record_length) { this->record_length = record_length; }
    void set_free_list_header(const int header) { this->free_list_header = header; }
    void set_dirty(bool status) { this->dirty = status; }
    void set_full(bool status) {this->full = status; }
    void set_pin(bool status) { this->pin = status; }
    void set_content(const char * content)
    {
        memcpy(this->content, content, BLOCK_SIZE);
    }


};

// read from metafile
class Table
{
    private:
    string table_name;
    int record_length;
    int attribute_count;
    vector <Attribute> attribute_set;
    vector <string> indices_name;

    public:
    Table(string table_name, int record_length, int attribute_count)
    : table_name(table_name), record_length(record_length), attribute_count(attribute_count){ }
    string get_table_name() {return table_name;};
    int get_record_length() { return record_length; }
    int get_attribute_count() {return attribute_count; }
    vector <Attribute> & get_attribute_set() { return attribute_set; }
    vector <string> & get_indices_name() { return indices_name; }

    bool isValidInput(const vector <string> & insert_data, const vector <int> & type, vector <char*> & raw_data) throw(Error);
    bool rawVec2rawData(const vector <char*> & raw_Vec, char * raw_data) throw(Error);

};

class Method
{
    public:
    static void vec2rawdata(const vector <char *> & raw_vec );
    static void string2rawdata(const string & str, const int type, char * rawdata);
    static int rawdata2int(const char * rawdata);
    static float rawdata2float(const char * rawdata);
    static void int2rawdata(const int data, char * rawdata);
    static void float2rawdata(const float data, char * rawdata);

};

// now especially for the B+ tree
// record_id采用绝对地址没有问题，因为record一般不会移动
class ptr
{
    
    int id; // it can be a block_id(internal node) or a record_id(leaf)
    char * rawdata;


    public:
    ptr(int id = 0) : id(id)
    {
        rawdata = new char[4];
        memcpy(rawdata, reinterpret_cast<char*>(id), INT_LENGTH);
    }
    ~ptr()
    {
        delete [] rawdata;
    }

    int get_id() const { return id; }
    char * get_rawdata() const { return rawdata;}

};

// 每个file都有fileheader存储metadata, 所以我必须把这些信息读出来
class FileManager
{
    private:
    Block * block; // 具体这个file的block
    int block_count;
    int record_length;
    int first_free_record_id; //绝对地址
    int record_count;
    int record_count_perblock;
    ptr free_pointer; // current_pointer


    public:
    FileManager(const string & file_name);
    
};

#endif