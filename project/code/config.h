#ifndef CONFIG_H
#define CONFIG_H
#include <iostream>
#include <vector>
#include <sstream>
#include <map>
#include <unordered_map>
#include <unordered_set>
#define DEBUG -1


#define PATH "/Users/yee/Desktop/monster-sql/MiniSQL/project/DB_data/"


#define BLOCK_SIZE 4096
#define MAX_TABLE_NAME 32
#define MAX_ATTRIBUTE_NAME 32
#define MAX_INDEX_NAME 64
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

class Method
{
    public:
    static void vec2rawdata(const vector <char *> & raw_vec );
    //这个函数内部有new
    static void string2rawdata(const string & str, const int type, char * rawdata);
    static int rawdata2int(const char * rawdata);
    static float rawdata2float(const char * rawdata);
    static void rawdata2string(const char * rawdata, int length, string & out_str);
    // 下面两个函数格外注意，返回的指针就是原来的指针
    static const int getLengthFromType(int type);
    static void createFile(const string & file_name, int record_length);
    static string AbsolutePath(const string & file_name);
    static void Cutrawdata(int type, int beginPos, char * rawdata);
    static void setIndexFromTableAttri(const string & table_name, const string & attri_name, string & index_name);
    static void deleteFile(const string & file_name);
    static bool isFileExist(const string & file_name);
    static bool isEqual(const char * a, const char * b, const int length);
    static int GetLogicalLength(const int PhysicLength);
    static bool isSatisfyConditon(const char * rawdata, const int cond, const string & operand, const int type);
    static bool isSatisfy(const int data, const int cond, const int operand);
    static bool isSatisfy(const float data, const int cond, const float operand);
    static bool isSatisfy(const string & data, const int cond, const string & operand);

};

class Attribute
{
    // all this can be read from the catalog
    private:
    // attribute name
    string name;
    // which type is the Attribute
    short type;

    int length;

    bool isPrimary;
    bool isUnique;


    public:
    Attribute(string  name, short type, bool isPrimary, bool isUnique)
    : name(name), type(type), isPrimary(isPrimary), isUnique(isUnique) 
    {
        length = Method::getLengthFromType(type); 
    }
    Attribute(string  name, short type, bool isUnique)
    : name(name), type(type), isUnique(isUnique) 
    { length = Method::getLengthFromType(type); 
         isPrimary = false; }
    ~Attribute(){};

    void set_primary(bool status) { isPrimary = status; }
    void set_Unique(bool status) {isUnique = status; }
    string get_name() const { return name; }
    int get_type() const {return type; }
    int get_length() const { return length; }
    bool is_primary() { return isPrimary; }
    bool is_unique() { return isUnique; }


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
    Index(const string & index_name, const string & table_name, const string & attri_name) 
    : index_name(index_name), table_name(table_name), attri_name(attri_name) {}
    string get_index_name() { return index_name; }



};

/* TODO
 * record_length是否需要？需要
 * attri_count?
 * Attri2Index的行为一定要格外注意
 * Name2Attri可以改成指针
 * 添加 Name2Pos 需要在两个地方修改：1、catalog构造Table 2、create table ✅
 * 
 */
class Table
{
    private:
    string table_name;
    int record_length; //通过计算得来,这个是实在的记录的一个长度
    int attribute_count; // 通过上一步metadata已经填好
    unordered_map <string, Attribute> Name2Attri; //名字索引到属性
    unordered_map <string, int > Name2Pos; // 名字索引一个相对起始地址
    //下面的index*不需要我来delete，上一步已经填好
   
    unordered_set <string> UniqueAttri;
    string primary_name; 
    vector <short> typeVec; //这个必须有，用于做类型检查
    
    public:
     unordered_map <string, Index*> Attri2Index; //属性名索引到Index名
    vector <string> attri_name; //这个也必须有，反映了attribute的排序

    // 专门为metadata准备的
    Table(const string & table_name, const int attribute_count) 
    : table_name(table_name), attribute_count(attribute_count) { }

    void setAttriPos();
    int GetPosByName(const string & AttriName) const
    {
        return Name2Pos.at(AttriName);
    }
    void DropIndex(const string & index_name)
    {
        for(auto attri_index : Attri2Index)
        {
            if(attri_index.second->get_index_name() == index_name)
            {
                Attri2Index.erase(attri_index.first);                
            }
        }
    }
    bool isUnique(const string & attri_name) const
    {
        auto search = UniqueAttri.find(attri_name);
        if(search == UniqueAttri.end())
            return false;
        else
            return true; 
            
    }

    void PrintRawdata(const char * rawdata) const;
    void PrintTableHead() const;
    bool isSatisfyAllCondition(const char * rawdata, const vector<string> & attribute_name,
    const vector<int> & condition, const vector<string> & operand) const ;
    bool isIndex(const string & attri_name, string & index_name) const
    {
        auto search = Attri2Index.find(attri_name);
        if(search == Attri2Index.end())
            return false;
        else
        {
            index_name = search->second->get_index_name(); 
            return true;
        }
            
    }
    bool GetAttriByName(const string & attri_name, Attribute & attri) const throw(Error);
    bool isAttribute(const string & attri_name) const throw(Error); 
    bool CheckConsistency(const char * ExistData, const char * rawdata, string & duplicate)  const throw(Error);
    void addType(short type){ typeVec.push_back(type); }
    void addAttriName(string & name) { attri_name.push_back(name); }
    string get_table_name() const  {return table_name;};
    int get_record_length() const { return record_length; }
    int get_attribute_count() const {return attribute_count; }
    void get_indices(vector <string> & indicesName) const ;
    void GetAttriIndexed(vector <Attribute> & AttriIndexed) const
    {
        for(auto attri : Attri2Index)
        {
            string attriName = attri.first;
            AttriIndexed.push_back(Name2Attri.at(attriName));

        }
    }
    void set_map_Attri2Index(const string & attribute_name, Index * index)
    {
        Attri2Index.insert({attribute_name, index});
    }
    void set_map_Name2Attri(const string & attribute_name, Attribute attri)
    {
        Name2Attri.insert({attribute_name, attri});
    }
    void set_uniqueSet(const string & attri_name)
    {
        UniqueAttri.insert(attri_name);
    }
    void set_primary_name(const string & primary_name)
    {
        this->primary_name = primary_name;
    }
    void set_record_length(const int record_length)
    {
        this->record_length = record_length;
    }
    void set_primary(const string & attri_name);
    void set_unique(const string & attri_name);

    bool isValidInput(const vector <string> & insert_data, 
    const vector <int> & type, vector <char*> & raw_data) const throw(Error);

    bool rawVec2rawData(const vector <char*> & raw_Vec, char * raw_data) const throw(Error);

    void print();


};





#endif