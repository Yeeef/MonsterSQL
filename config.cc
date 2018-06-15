#include "config.h"

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
    for (int i = 0; i < attribute_count; i++)
    {
        if (type.at(i) != attribute_set.at(i).get_type())
        {

            Error err("[Table::isValidInput]: insert_data type mismatch");
            throw err;
            return false;
        }
        char * rawdata;
        Method::string2rawdata(insert_data.at(i), type.at(i), rawdata);
        // 翻译好的数据加入 raw_vec
        raw_Vec.push_back(rawdata);

    }
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
    for (int i = 0; i < attribute_count; i++)
    {
        int attribute_length = attribute_set.at(i).get_length();
        memcpy(raw_data + bias, raw_Vec.at(i), attribute_length);
        bias += attribute_length;
    }

    return true;
}

/* Method */
// convert a string to the raw data with given type
void Method::string2rawdata(const string &str, const int type, char *rawdata)
{
    stringstream ss;
    ss << str;

    switch (type)
    {
    case TYPE_INT:
    {
        int temp;
        ss >> temp;
        rawdata = new char [INT_LENGTH];
        memcpy(rawdata, reinterpret_cast<char *>(&temp), INT_LENGTH);
        break;
    }
    case TYPE_FLOAT:
    {
        double temp;
        ss >> temp;
        rawdata = new char [FLOAT_LENGTH];
        memcpy(rawdata, reinterpret_cast<char *>(&temp), FLOAT_LENGTH);
        break;
    }
    case TYPE_CHAR:
    {
        rawdata = new char [CHAR_LENGTH];
        ss >> rawdata;

        break;
    }
    }
}

int Method::rawdata2int(const char * rawdata)
{
    int realdata;
    memcpy(&realdata, (rawdata), INT_LENGTH);
    return realdata;


}
float Method::rawdata2float(const char * rawdata)
{
    float realdata;
    memcpy(&realdata, (rawdata), INT_LENGTH);
    return realdata;
}