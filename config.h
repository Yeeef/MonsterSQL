#ifndef CONFIG_H
#define CONFIG_H
#include <iostream>
#include <vector>
#define DEBUG -1

#define BLOCK_SIZE 4096
#define MAX_TABLE_NAME 32
#define MAX_ATTRIBUTE_NAME 32
#define CHAR_LENGTH 255
#define TYPE_NULL 0
#define TYPE_CHAR 1
#define TYPE_INT 2
#define TYPE_FLOAT 3

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

    // 这个就不一定有了
    char * rawcontent;

    bool isPrimary;
    bool isUnique;
    bool isIndex;


    public:
    Attribute(string name, bool isPrimary = false, bool isUnique = false)
    : name(name), isPrimary(isPrimary), isUnique(isUnique) {}
    ~Attribute(){};

    string get_name() const { return name; }
    bool is_primary() { return isPrimary; }
    bool is_unique() { return isUnique; }

};

class Error
{
    private:
    string error_info;

    public:
    Error(string error_info) : error_info(error_info) {}
    ~Error();

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

class Table
{
    private:
    string table_name;
    int record_length;
    int attribute_count;
    vector <Attribute> attribute_set;

    public:
    
};

class Method
{
    public:
    //static void vec2rawdata(const vector<string> );
};


#endif