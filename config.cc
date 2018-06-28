#include "config.h"
#include "minisql.h"

/* ---------------------------------------------*/
/* Table */
/* TODO
 * 这两个函数都要改
 * 
 */

void Table::PrintTableHead() const
{
    for(auto AttriName : attri_name)
    {
        cout << AttriName << "\t";
    }

}

/* 打印出rawdata的内容
 *
 */
void Table::PrintRawdata(const char * rawdata) const
{
    for(auto AttriName : attri_name)
    {
        Attribute Attri = Name2Attri.at(AttriName);
        int Pos = Name2Pos.at(AttriName);
        int type = Attri.get_type();
        int length = Attri.get_length();
        string PrintData;
        switch(type)
        {
            case TYPE_INT:
            {
                int data = Method::rawdata2int(rawdata + Pos);
                stringstream ss;
                ss << data;
                ss >> PrintData;

                break;
            }
            case TYPE_FLOAT:
            {
                float data = Method::rawdata2float(rawdata+Pos);
                stringstream ss;
                ss << data;
                ss >> PrintData;
                break;
            }
            default:
            {
                Method::rawdata2string(rawdata + Pos, length, PrintData);
            }
        }
        cout << PrintData << "\t";
    }
}
/* 目的：判断rawdata，是否符合所有条件
 * 通过attri, 分割
 * 
 */
bool Table::isSatisfyAllCondition(const char *rawdata, const vector<string> &attribute_name,
                                  const vector<int> &condition, const vector<string> &operand) const 
{
    for (int i = 0; i < attribute_name.size(); i++)
    {
        string AttriName = attribute_name.at(i);
        Attribute Attri = Name2Attri.at(AttriName);
        int Pos = Name2Pos.at(AttriName);
        int length = Attri.get_length();
        int type = Attri.get_type();
        //提取出对应于attribute的一段内容
        char AttriData[length];
        memcpy(AttriData, rawdata + Pos, length);
        if(Method::isSatisfyConditon(AttriData, condition.at(i), operand.at(i), type) == false)
            return false;
        
    }
    return true;
}

void Table::setAttriPos()
{
    int bias = 0;
    for (auto AttriName : attri_name)
    {
        Name2Pos.insert({AttriName, bias});
        Attribute attri = Name2Attri.at(AttriName);
        bias += attri.get_length();
    }
}

bool Table::GetAttriByName(const string &attri_name, Attribute &attri) const throw(Error)
{
    attri = Name2Attri.at(attri_name);
}

bool Table::isAttribute(const string &attri_name) const throw(Error)
{
    auto search = Name2Attri.find(attri_name);

    // 不存在这个Attribute
    if (search == Name2Attri.end())
    {
        string err = "[Table::isAttribute] '" + attri_name + "' doesn't exist";
        throw Error(err);
    }
    return true;
}
/* 检查rawdata中的unique/ primary属性是否重复出现
 *
 * 
 */
bool Table::CheckConsistency(const char *ExistData, const char *rawdata, string &duplicate) const throw(Error)
{
    int startPos = 0;
    for (auto attri : attri_name)
    {
        auto search = UniqueAttri.find(attri);
        auto Attribute = Name2Attri.at(attri);
        auto attri_length = Attribute.get_length();

        //这个属性是一个unique
        if (search != UniqueAttri.end())
        {
            // 判断这一段数据区是否一样,如果一样那么有问题了
            if (Method::isEqual(ExistData + startPos, rawdata + startPos, attri_length) == true)
            {
                duplicate = Attribute.get_name();
                return false;
            }
        }
        startPos += attri_length;
    }
    return true;
}

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
        switch(type.at(i))
        {
            case TYPE_INT:
            {
                if(typeVec.at(i) != TYPE_INT)
                {
                    Error err("[Table::isValidInput]: insert_data type mismatch");
                    throw err;
                    return false;
                }

                break;
            }
            case TYPE_FLOAT:
            {
                if(typeVec.at(i) != TYPE_FLOAT)
                {
                    Error err("[Table::isValidInput]: insert_data type mismatch");
                    throw err;
                    return false;
                }
                break;
            }
            default:
            {
                if(type.at(i) > typeVec.at(i))
                {
                    Error err("[Table::isValidInput]: insert_data type mismatch");
                    throw err;
                    return false;

                }
                break;
            }
        }

    }
    /* ---------------------------------------- */

    /* parse data */
    for (int i = 0; i < attribute_count; i++)
    {
        char *rawdata = new char[Method::getLengthFromType(type.at(i)) + 1]();
        Method::string2rawdata(insert_data.at(i), type.at(i), rawdata);
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
        for (auto raw : raw_Vec)
            delete[] raw;
        delete[] raw_data;
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
void Table::print()
{
    for (auto attri : Name2Attri)
    {
        cout << attri.first << " ";
    }
    for (auto indexName : Attri2Index)
    {
        cout << indexName.second->get_index_name() << " ";
    }
}
/* ---------------------------------------------*/

/* ---------------------------------------------*/
/* Method */

// convert a string to the raw data with given type
bool Method::isEqual(const char *a, const char *b, const int length)
{
    int isequal = true;
    for (int i = 0; i < length; i++)
    {
        if (a[i] != b[i])
        {
            isequal = false;
            break;
        }
        if(a[i] == '\0' && b[i] == '\0')
            break;
    }
    return isequal;
}

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
        memcpy(rawdata, reinterpret_cast<char *>(&temp), INT_LENGTH);
        break;
    }
    case TYPE_FLOAT:
    {
        float temp;
        ss >> temp;
        memcpy(rawdata, (&temp), FLOAT_LENGTH);
        break;
    }
    default:
    {
        //actually it's type_char
        // type_char的大小就代表了长度！
        int length = Method::getLengthFromType(type);
        memcpy(rawdata, str.c_str(), length);
        memset(rawdata + length, 0, 1);
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

void Method::createFile(const string &file_name, int record_length)
{
    record_length++;
    FILE *file = fopen(Method::AbsolutePath(file_name).c_str(), "wb");
    if (file == nullptr)
    {
        cout << "[Method::createFile] cannot create file '" << file_name << "' " << endl;
        return;
    }

    int first_free_record_id = -1;
    int record_count = 0;
    fwrite(&record_length, 4, 1, file);
    fwrite(&first_free_record_id, 4, 1, file);
    fwrite(&record_count, 4, 1, file);
    fclose(file);
}

void Method::deleteFile(const string &file_name)
{

    remove(Method::AbsolutePath(file_name).c_str());
    // 从buffer中删除？也许不需要删除，只需要把dirty标记为false就好？
    BufferManager &buffermanager = MiniSQL::get_buffer_manager();
    buffermanager.DeleteBlockByFile(file_name);
}

bool Method::isFileExist(const string &file_name)
{
    FILE *file = fopen(AbsolutePath(file_name).c_str(), "rb");
    if (file == nullptr)
        return false;
    else
        return true;
}
void Method::Cutrawdata(int type, int beginPos, char *rawdata)
{
    memcpy(rawdata, rawdata + beginPos, Method::getLengthFromType(type));
}

string Method::AbsolutePath(const string &file_name)
{
    return PATH + file_name;
}

void Method::setIndexFromTableAttri(const string &table_name, const string &attri_name, string &index_name)
{
    index_name = table_name + attri_name;
}

int Method::GetLogicalLength(const int PhysicLength)
{
    return (PhysicLength - 1);
}

bool Method::isSatisfy(const int data, const int cond, const int operand)
{
    switch (cond)
    {
        case COND_EQ:
        {
            if (data != operand)
                return false;

            break;
        }
        case COND_GE:
        {
            if (data < operand)
                return false;
            break;
        }
        case COND_LE:
        {
            if (data > operand)
                return false;
            break;
        }
        case COND_GT:
        {
            if (data <= operand)
                return false;
            break;
        }
        case COND_LT:
        {
            if (data >= operand)
                return false;
            break;
        }
        case COND_NE:
        {
            if (data == operand)
                return false;
            break;
        }
        default:
        {
            cout << "Wrong Cond: '" << cond << "'" << endl;
            return false;
        }
    }

    return true;
}

bool Method::isSatisfy(const float data, const int cond, const float operand)
{
    switch (cond)
    {
        case COND_EQ:
        {
            if (data != operand)
                return false;

            break;
        }
        case COND_GE:
        {
            if (data < operand)
                return false;
            break;
        }
        case COND_LE:
        {
            if (data > operand)
                return false;
            break;
        }
        case COND_GT:
        {
            if (data <= operand)
                return false;
            break;
        }
        case COND_LT:
        {
            if (data >= operand)
                return false;
            break;
        }
        case COND_NE:
        {
            if (data == operand)
                return false;
            break;
        }
        default:
        {
            cout << "Wrong Cond: '" << cond << "'" << endl;
            return false;
        }
    }

    return true;
}
bool Method::isSatisfy(const string & data, const int cond, const string & operand)
{
        switch (cond)
    {
        case COND_EQ:
        {
            if (data != operand)
                return false;

            break;
        }
        case COND_GE:
        {
            if (data < operand)
                return false;
            break;
        }
        case COND_LE:
        {
            if (data > operand)
                return false;
            break;
        }
        case COND_GT:
        {
            if (data <= operand)
                return false;
            break;
        }
        case COND_LT:
        {
            if (data >= operand)
                return false;
            break;
        }
        case COND_NE:
        {
            if (data == operand)
                return false;
            break;
        }
        default:
        {
            cout << "Wrong Cond: '" << cond << "'" << endl;
            return false;
        }
    }
    return true;
    
}

bool Method::isSatisfyConditon(const char *rawdata, const int cond, const string &operand, const int type)
{

    switch (type)
    {
    case TYPE_INT:
    {
        int data = Method::rawdata2int(rawdata);
        char Operand[Method::getLengthFromType(type)];
        Method::string2rawdata(operand, type, Operand);
        int operandValue = Method::rawdata2int(Operand);
        return Method::isSatisfy(data, cond, operandValue);
        break;
    
    }

        
    case TYPE_FLOAT:
    {
        float data = Method::rawdata2float(rawdata);
        char Operand[Method::getLengthFromType(type)];
        Method::string2rawdata(operand, type, Operand);
        float operandValue = Method::rawdata2float(Operand);
        return Method::isSatisfy(data, cond , operandValue);
        break;
    }
    default:
    {
        string data;
        Method::rawdata2string(rawdata, Method::getLengthFromType(type), data);
        string operandValue(operand);
        return Method::isSatisfy(data, cond, operandValue);
    }
    }
    return true;
}

/* ---------------------------------------------*/
