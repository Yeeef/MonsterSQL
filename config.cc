#include "config.h"
#include "minisql.h"

/* ---------------------------------------------*/
/* Table */
/* TODO
 * 这两个函数都要改
 * 
 */
bool Table::isValidInput(const vector<string> &insert_data, const vector<int> &type, vector<char *> &raw_Vec) throw(Error)
{
    // 输入的格式检查,暂时不允许null的存在
    if (insert_data.size() != attribute_count)
    {
        Error err("[Table::isValidInput]: insert_data size mismatch");
        throw err;
        return false;
    }

    // 类型检查同时parse data
    /*
    for (int i = 0; i < attribute_count; i++)
    {
        if (type.at(i) != attribute_set.at(i).get_type())
        {

            Error err("[Table::isValidInput]: insert_data type mismatch");
            throw err;
            return false;
        }
       
        char *rawdata;
        rawdata = Method::string2rawdata(insert_data.at(i), type.at(i));
        // 翻译好的数据加入 raw_vec
        raw_Vec.push_back(rawdata);
    }
    */
    return true;
}
bool Table::rawVec2rawData(const vector<char *> &raw_Vec, char *raw_data) throw(Error)
{
    int bias = 0;
    if (raw_Vec.size() != attribute_count)
    {
        Error err("[Table::rawVec2rawData] attribute count mismatch");
        throw err;
        return false;
    }
    /*
    for (int i = 0; i < attribute_count; i++)
    {
        int attribute_length = attribute_set.at(i).get_length();
        memcpy(raw_data + bias, raw_Vec.at(i), attribute_length);
        bias += attribute_length;
    }
    */

    return true;
}
/* ---------------------------------------------*/

/* ---------------------------------------------*/
/* Method */

// convert a string to the raw data with given type
char * Method::string2rawdata(const string &str, const int type)
{
    stringstream ss;
    ss << str;

    switch (type)
    {
    case TYPE_INT:
    {
        int temp;
        ss >> temp;
        auto rawdata = new char[INT_LENGTH];
        memcpy(rawdata, reinterpret_cast<char *>(&temp), INT_LENGTH);
        return rawdata;
        break;
    }
    case TYPE_FLOAT:
    {
        float temp;
        ss >> temp;
        char *rawdata = new char[4];
        memcpy(rawdata, (&temp), FLOAT_LENGTH);
        return rawdata;
        break;
    }
    default:
    {
        //actually it's type_char
        // type_char的大小就代表了长度！
        int length = Method::getLengthFromType(type);
        auto rawdata = new char[length]; 
        ss >> rawdata;
        return rawdata;
        break;
    }
    }
}

int Method::rawdata2int(const char *rawdata)
{
    int realdata;
    memcpy(&realdata, (rawdata), INT_LENGTH);
    return realdata;
}
float Method::rawdata2float(const char *rawdata)
{
    float realdata;
    memcpy(&realdata, (rawdata), INT_LENGTH);
    return realdata;
}

const char * Method::int2rawdata(int * data)
{  
    
    return reinterpret_cast<char *>(data); 
}
const char * Method::float2rawdata(float * data)
{
    return reinterpret_cast<char *>(data);
}
const int Method::recordID2Addr(const int record_id, const int record_length)
{
    return record_id * record_length;
}
const int Method::getLengthFromType(int type)
{
    
    switch (type)
    {
        case TYPE_INT:
            return sizeof(int);
            break;
        case TYPE_FLOAT:
            return sizeof(float);
            break;   
        default:
            return type;
            break;
    }
}

void Method::createFile(string file_name)
{
    FILE * file = fopen(file_name.c_str(), "wb");
    if(file == nullptr)
    {
        cout << "[Method::createFile] cannot open file '"<< file_name << "' " << endl;
        return;
    }
    int record_length = 0;
    int first_free_record_id = -1; 
    int record_count = 1;
    fwrite(&record_length, 4, 1, file);
    fwrite(&first_free_record_id, 4, 1, file);
    fwrite(&record_count, 4, 1, file);
    fclose(file); 

}

string Method::AbsolutePath(string & file_name)
{
    return PATH +file_name;
}

/* ---------------------------------------------*/

