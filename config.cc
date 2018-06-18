#include "config.h"
#include "minisql.h"

/* ---------------------------------------------*/
/* Table */
/* TODO
 * 这两个函数都要改
 * 
 */

/*
 * 检查insert_data是否符合格式；翻译为rawdata给raw_Vec;
 * 
 */
bool Table::isValidInput(const vector<string> &insert_data,
                         const vector<int> &type, vector<char *> &raw_Vec) const throw(Error)
{
    /* 错误检查 */
    // 输入的格式检查,暂时不允许null的存在
    if (insert_data.size() != attribute_count)
    {
        Error err("[Table::isValidInput]: insert_data size mismatch");
        throw err;
        return false;
    }

    // 类型检查同时parse data

    for (int i = 0; i < attribute_count; i++)
    {
        if (type.at(i) != typeVec.at(i))
        {

            Error err("[Table::isValidInput]: insert_data type mismatch");
            throw err;
            return false;
        }
    }
    /* ---------------------------------------- */

    /* parse data */
    for (int i = 0; i < attribute_count; i++)
    {
        char *rawdata;
        rawdata = Method::string2rawdata(insert_data.at(i), type.at(i));
        // 翻译好的数据加入 raw_vec
        raw_Vec.push_back(rawdata);
    }

    return true;
}
bool Table::rawVec2rawData(const vector<char *> &raw_Vec, char *raw_data) const throw(Error)
{
    /* 检查错误 */

    if (raw_Vec.size() != attribute_count)
    {
        Error err("[Table::rawVec2rawData] attribute count mismatch");
        for(auto raw : raw_Vec)
            delete [] raw;
        delete [] raw_data; 
        throw err;
        return false;
    }

    /* ------------------------------- */

    int bias = 0;
    for (int i = 0; i < attribute_count; i++)
    {
        int attribute_length = Method::getLengthFromType(typeVec.at(i));
        memcpy(raw_data + bias, raw_Vec.at(i), attribute_length);
        bias += attribute_length;
    }

    return true;
}

// attri2Index的更新工作由 catalog::create_index来做
void Table::set_primary(const string &attri_name)
{
    set_primary_name(attri_name);
    set_uniqueSet(attri_name);
}

void Table::set_unique(const string &attri_name)
{
    set_uniqueSet(attri_name);
}

void Table::get_indices(vector<string> &indicesName) const
{
    for (auto index : Attri2Index)
    {
        indicesName.push_back(index.second->get_index_name());
    }
}
/* ---------------------------------------------*/

/* ---------------------------------------------*/
/* Method */

// convert a string to the raw data with given type
char *Method::string2rawdata(const string &str, const int type)
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

// 需要在结尾添加有效位
void Method::rawdata2string(const char *rawdata, int length, string &out_str)
{
    char str[length + 1];
    memcpy(str, rawdata, length);
    memset(str + length, 0, 1);
    out_str = string(str);
}

void Method::int2rawdata(int data, char *rawdata)
{
    memcpy(rawdata, &data, INT_LENGTH);
}
void Method::float2rawdata(float data, char *rawdata)
{
    memcpy(rawdata, &data, FLOAT_LENGTH);
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
    {
        if (type < MIN_TYPE_LENGTH)
            return MIN_TYPE_LENGTH;
        else
            return type;
        break;
    }
    }
}

void Method::createFile(string file_name, int record_length)
{
    FILE *file = fopen(Method::AbsolutePath(file_name).c_str(), "wb");
    if (file == nullptr)
    {
        cout << "[Method::createFile] cannot open file '" << file_name << "' " << endl;
        return;
    }
    int first_free_record_id = -1;
    int record_count = 0;
    fwrite(&record_length, 4, 1, file);
    fwrite(&first_free_record_id, 4, 1, file);
    fwrite(&record_count, 4, 1, file);
    fclose(file);
}

void Method::deleteFile(string file_name)
{
    remove(Method::AbsolutePath(file_name).c_str());
    // 从buffer中删除？也许不需要删除，只需要把dirty标记为false就好？
    BufferManager &buffermanager = MiniSQL::get_buffer_manager();
}

void Method::Cutrawdata(int type, int beginPos, char *rawdata)
{
    memcpy(rawdata, rawdata + beginPos, Method::getLengthFromType(type));
}

string Method::AbsolutePath(string &file_name)
{
    return PATH + file_name;
}

void Method::setIndexFromTableAttri(const string &table_name, const string &attri_name, string &index_name)
{
    index_name = table_name + attri_name;
}

/* ---------------------------------------------*/
