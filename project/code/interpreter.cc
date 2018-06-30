#include <ctime>
#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <string.h>
#include <stdlib.h>

#include "interpreter.h"
#include "minisql.h"
using namespace std;





int Interpreter::getTokens(const char* sql, vector<string>& tokens, vector<TOKEN>& type)
{
    int Count = 0;
    TOKEN state = IDLE;
    int cursor = 0;
    string token = "";

    while (true)
    {
        bool moveCursor = true;//标识是否移动到下一字符
        char c = sql[cursor];
        if (state != SCHAR && state != STRING && c >= 'A' && c <= 'Z')
            c += 'a' - 'A'; //除属性字符外统一小写

        if (state == IDLE)//空闲状态
        {
            if (c == ';')//分号完成一条语句，跳到END状态
            {
                state = END;
                moveCursor = false;
            }
            else if ((c >= 'a' && c <= 'z') || c == '_')//单词名称
            {
                state = IDENTIFIER;
                moveCursor = false;
            }
            else if ((c >= '0' && c <= '9') || c == '+' || c == '-' || c == '.')//数字
            {
                state = NUMBER;
                moveCursor = false;
            }
            else if (c == '\'')//单引号字母
                state = SCHAR;
            else if (c == '"')//双引号字符串
                state = STRING;
            else if (c == ',' || c == '(' || c == ')' || c == '*')//符号
            {
                state = SYMBOL;
                moveCursor = false;
            }
            else if (c == '=' || c == '<' || c == '>')//操作符
            {
                state = OPERATOR;
                moveCursor = false;
            }
            else if (c == ' ' || c == '\n' || c == '\t') {}
            else if (c == 0)
                break;
            else
            {
            cerr << "ERROR: [Tokenizer::getTokens] Unknown character '" << c << "'!" << endl;
                tokens.push_back(token);
                type.push_back(INVALID);
                token.clear();
            }
        }
        else if (state == END)//end
        {
            token += c;

            tokens.push_back(token);
            type.push_back(END);
            token.clear();
            Count++;

            state = IDLE;
        }
        else if (state == IDENTIFIER)
        {
            if ((c >= 'a' && c <= 'z') || (c >= '0' && c <= '9') || c == '_')
                token += c;
            else
            {
                tokens.push_back(token);
                type.push_back(IDENTIFIER);
                token.clear();

                state = IDLE;
                moveCursor = false;
            }
        }
        else if (state == NUMBER)
        {
            if ((c >= '0' && c <= '9') || c == '+' || c == '-' || c == '.')
                token += c;
            else
            {
                tokens.push_back(token);
                type.push_back(NUMBER);
                token.clear();

                state = IDLE;
                moveCursor = false;
            }
        }
        else if (state == SCHAR || state == STRING)
        {
            if ((c == '\'' && state == SCHAR) || (c == '"' && state == STRING))
            {
                tokens.push_back(token);
                type.push_back(state);
                token.clear();

                state = IDLE;
            }
            else if (c == '\n' || c == 0)
            {
                cerr << "ERROR: [Tokenizer::getTokens] New line symbol reached when reading string!" << endl;
                tokens.push_back(token);
                type.push_back(INVALID);
                token.clear();
                state = IDLE;
            }
            else
                token += c;
        }
        else if (state == SYMBOL)
        {
            token += c;

            tokens.push_back(token);
            type.push_back(SYMBOL);
            token.clear();

            state = IDLE;
        }
        else if (state == OPERATOR)
        {
            if (c == '=' || c == '<' || c == '>')
                token += c;
            else
            {
                tokens.push_back(token);
                type.push_back(OPERATOR);
                token.clear();

                state = IDLE;
                moveCursor = false;
            }
        }

        if (moveCursor)
            cursor++;
    }

    return Count;
}

// Constructor
Interpreter::Interpreter(bool _fromFile): fromFile(_fromFile)
{
    ptr = -1;

   exiting = false;
}

Interpreter::~Interpreter(){}

/*
bool Interpreter::tokenVecEmpty() const
{
    return ptr == (int)tokens.size() - 1;
}
*/

// Skip current statement
void Interpreter::skipStatement()
{
    if (ptr < 0)
        ptr = 0;
    for (; type[ptr] != END; ptr++);
} 

void Interpreter::reportUnexpected(const char* position, const char* expecting)
{
    cerr << "ERROR: [Interpreter::" << position << "] Expecting " << expecting << ", but found '" << tokens[ptr] << "'." << endl;
    skipStatement();
}

int Interpreter::GetOperatorType(const string& op)
{
    string s = op;
    if (s == "=")
        return COND_EQ;
    else if (s == "<")
        return COND_LT;
    else if (s == ">")
        return COND_GT;
    else if (s == "<=")
        return COND_LE;
    else if (s == ">=")
        return COND_GE;
    else if (s == "<>")
        return COND_NE;
    else
        return -1;
}

//判断选择和删除条件
bool Interpreter::where(vector<string>& attribute_name, vector<int>& condition , vector<string>& operand)
{
    ptr++;
    if (type[ptr] == END)
        // 没有条件
        return true;
    else if (tokens[ptr] != "where" || type[ptr] != IDENTIFIER)
    {
        reportUnexpected("select", "'where'");
        return false;
    }

    // 有条件
    while (true)
    {
        ptr++;
        if (type[ptr] != IDENTIFIER)
        {
            reportUnexpected("select", "column name");
            return false;
        }
        attribute_name.push_back(tokens[ptr]);

        ptr++;
        if (type[ptr] != OPERATOR)
        {
            reportUnexpected("select", "operator");
            return false;
        }
        int op = GetOperatorType(tokens[ptr]);
        if (op < 0)
        {
            cerr << "ERROR: [Interpreter::select] Unknown operator '" << tokens[ptr] << "'." << endl;
            skipStatement();
            return false;
        }
        condition.push_back(op);

        ptr++;
        if (type[ptr] != NUMBER && type[ptr] != SCHAR && type[ptr] != STRING)
        {
            reportUnexpected("select", "value");
            return false;
        }
        operand.push_back(tokens[ptr]);

        ptr++;
        if (type[ptr] == END)
            return true;
        else if (tokens[ptr] != "and" || type[ptr] != IDENTIFIER)
        {
            reportUnexpected("select", "'and'(MiniSQL only supports conjunctive selection)");
            return false;
        }
    }
}

int Interpreter::GetType()
{
    ptr++;
    if (tokens[ptr] == "varchar" && type[ptr] == IDENTIFIER)
    {
        return TYPE_CHAR;
    }
    else if (tokens[ptr] == "int" && type[ptr] == IDENTIFIER)
        return TYPE_INT;
    else if (tokens[ptr] == "float" && type[ptr] == IDENTIFIER)
        return TYPE_FLOAT;
    else
    {
        reportUnexpected("GetType", "'varchar', 'int' or 'float'(MiniSQL only supports these three data types)");
        return -1;
    }
}

void Interpreter::execute(const char* sql)
{
    int Count = getTokens(sql, tokens, type);

    while (Count--)
    {
        ptr++;
        if (type[ptr] != IDENTIFIER && type[ptr] != END)
            reportUnexpected("execute", "instruction");
        else if (type[ptr] == END) {}
        else if (tokens[ptr] == "select")
            select();
        else if (tokens[ptr] == "insert")
            insert();
        else if (tokens[ptr] == "delete")
            remove();
        else if (tokens[ptr] == "create")
            {
                ptr++;
                if (tokens[ptr] == "table" && type[ptr] == IDENTIFIER)
                create_table();
                else if (tokens[ptr] == "index" && type[ptr] == IDENTIFIER)
                create_index();
                else
                reportUnexpected("create", "'table' or 'index'");
            }
        else if (tokens[ptr] == "drop")
            {
                ptr++;
                if (tokens[ptr] == "table" && type[ptr] == IDENTIFIER)
                drop_table();
                else if (tokens[ptr] == "index" && type[ptr] == IDENTIFIER)
                drop_index();
                else
                reportUnexpected("drop", "'table' or 'index'");
            }
        else if (tokens[ptr] == "exec" || tokens[ptr] == "execfile")
            execfile();
        else if (tokens[ptr] == "exit" || tokens[ptr] == "quit")
            exit();
        else
        {
            cerr << "ERROR: [Interpreter::execute] Unknown instruction '" << tokens[ptr] << "'." << endl;
            skipStatement();
        }
    }
}

// 选择
void Interpreter::select()
{
    API & api = MiniSQL::get_api();
    ptr++;
    if (tokens[ptr] != "*" || type[ptr] != SYMBOL)
    {
        reportUnexpected("select", "'*'(MiniSQL does not support selecting specific columns)");
        return;
    }

    ptr++;
    if (tokens[ptr] != "from" || type[ptr] != IDENTIFIER)
    {
        reportUnexpected("select", "'from'");
        return;
    }

    ptr++;
    if (type[ptr] != IDENTIFIER)
    {
        reportUnexpected("select", "table name");
        return;
    }

    
    const string table_name = tokens[ptr];
    vector<string> attribute_name;
    vector<int> condition;
    vector<string> operand;
    //这里还要细化选择条件
    if (where(attribute_name, condition, operand))
    {
       
        int tic, toc;
        tic = clock();
        int ifselected = api.select(table_name, attribute_name, condition, operand);
        toc = clock();

        // 运行时间
        
        if (!fromFile)
            cout << ifselected <<" record(s) selected. Query done in " << 1.0 * (toc-tic) / CLOCKS_PER_SEC << "s." << endl;
        
    /*    cout<<table_name<<endl;
        for(vector<string>::iterator iter=attribute_name.begin();iter!=attribute_name.end();++iter)
        {
    	cout<<*iter<<" ";
	    }
	    cout<<endl;
	   for(vector<int>::iterator iter=condition.begin();iter!=condition.end();++iter)
        {
    	cout<<*iter<<" ";
    	} 
    	cout<<endl;
    	for(vector<string>::iterator iter=operand.begin();iter!=operand.end();++iter)
        {
    	cout<<*iter<<" ";
    	} 
    	cout<<endl;
    */ 
	}
}

//删除记录
void Interpreter::remove()
{
    API & api = MiniSQL::get_api();
    ptr++;
    if (tokens[ptr] != "from" || type[ptr] != IDENTIFIER)
    {
        reportUnexpected("delete", "'from'");
        return;
    }

    ptr++;
    if (type[ptr] != IDENTIFIER)
    {
        reportUnexpected("delete", "table name");
        return;
    }

    
    const string table_name = tokens[ptr];
    vector<string> attribute_name;
    vector<int> condition;
    vector<string> operand;
    //这里还要细化选择条件
    if (where(attribute_name, condition, operand))
    {
        //Delete
        int tic, toc;
        tic = clock();
        int NumOfAffected = api.Delete(table_name, attribute_name, condition, operand);
        toc = clock();
        if(NumOfAffected < 0)
            NumOfAffected = 0;

        //运行时间
        if (!fromFile)
            cout << NumOfAffected << " record(s) deleted. Query done in " << 1.0 * (toc-tic) / CLOCKS_PER_SEC << "s." << endl;
/* 
        cout<<table_name<< endl; 
         for(vector<string>::iterator iter=attribute_name.begin();iter!=attribute_name.end();++iter)
        {
    	cout<<*iter<<" ";
	    }
	    cout<<endl;
	   for(vector<int>::iterator iter=condition.begin();iter!=condition.end();++iter)
        {
    	cout<<*iter<<" ";
    	} 
    	cout<<endl;
    	for(vector<string>::iterator iter=operand.begin();iter!=operand.end();++iter)
        {
    	cout<<*iter<<" ";
    	}
		 cout<<endl;  
*/ 
	}
}

//删除表
void Interpreter::drop_table()
{
    API & api = MiniSQL::get_api();
    ptr++;
    if (type[ptr] != IDENTIFIER)
    {
        reportUnexpected("drop_table", "table name");
        return;
    }
    const string table_name = tokens[ptr];

    ptr++;
    if (type[ptr] != END)
    {
        reportUnexpected("drop", "';'");
        return;
    }

    // Do drop table
    int tic, toc;
    bool ifdrop;
    tic = clock();
    ifdrop = api.drop_table(table_name);
    toc = clock();

    //运行时间
    if (ifdrop &&!fromFile)
    cout << "1 table dropped. Query done in " << 1.0 * (toc-tic) / CLOCKS_PER_SEC << "s." << endl;

//    cout<<table_name<<endl;
}


//删除索引
void Interpreter::drop_index()
{
    API & api = MiniSQL::get_api();
    ptr++;
    if (type[ptr] != IDENTIFIER)
    {
        reportUnexpected("drop", "index name");
        return;
    }
    const string index_name = tokens[ptr];

    ptr++;
    if (tokens[ptr] != "on" || type[ptr] != IDENTIFIER)
    {
        reportUnexpected("drop_index", "'on'");
        return;
    }

    ptr++;
    if (type[ptr] != IDENTIFIER)
    {
        reportUnexpected("drop_index", "table name");
        return;
    }
    const string table_name = tokens[ptr];

    ptr++;
    if (type[ptr] != END)
    {
        reportUnexpected("drop", "';'");
        return;
    }

    // Do drop index
    int tic, toc;
    bool res;
    tic = clock();
    res = api.drop_index(index_name, table_name);
    toc = clock();

    // 运行时间
    if (res && !fromFile)
        cout << "1 index dropped. Query done in " << 1.0 * (toc-tic) / CLOCKS_PER_SEC << "s." << endl;

//    cout<<index_name<<" "<<table_name<<endl;
}

//创建索引
void Interpreter::create_index()
{
    API & api = MiniSQL::get_api();
    ptr++;
    if (type[ptr] != IDENTIFIER)
    {
        reportUnexpected("create_index", "index name");
        return;
    }

    
    const string index_name = tokens[ptr];
     string table_name;
     string attribute_name;

    ptr++;
    if (tokens[ptr] != "on" || type[ptr] != IDENTIFIER)
    {
        reportUnexpected("create_index", "'on'");
        return;
    }

    ptr++;
    if (type[ptr] != IDENTIFIER)
    {
        reportUnexpected("create_index", "table_name");
        return;
    }
    table_name = tokens[ptr];

    ptr++;
    if (tokens[ptr] != "(" || type[ptr] != SYMBOL)
    {
        reportUnexpected("create_index", "'('");
        return;
    }

    ptr++;
    if (type[ptr] != IDENTIFIER)
    {
        reportUnexpected("create_index", "attribute_name");
        return;
    }
    attribute_name = tokens[ptr];

    ptr++;
    if (tokens[ptr] != ")" || type[ptr] != SYMBOL)
    {
        reportUnexpected("create_index", "')'");
        return;
    }

    ptr++;
    if (type[ptr] != END)
    {
        reportUnexpected("createIndex", "';'");
        return;
    }

    // Do creation
    int tic, toc;
    bool res;
    tic = clock();
    res = api.create_index(table_name, attribute_name, index_name);
    toc = clock();
    
    // 运行时间
    if (res &&!fromFile)
        cout << "1 index created. Query done in " << 1.0 * (toc-tic) / CLOCKS_PER_SEC << "s." << endl;

//   cout<<table_name<<" "<<attribute_name<<" "<<index_name<<endl;
}

//创建表
void Interpreter::create_table()
{
    API & api = MiniSQL::get_api();
    ptr++;
    if (type[ptr] != IDENTIFIER)
    {
        reportUnexpected("create_table", "table name");
        return;
    }

    
    string table_name = tokens[ptr];
    vector <Attribute> a;
    Attribute* primary = new Attribute(table_name, 4, true, true);
    ptr++;
    if (tokens[ptr] != "(" || type[ptr] != SYMBOL)
    {
        reportUnexpected("createTable", "'('");
        return;
    }

    bool hasprimary = false;

    while (true)
    {
        ptr++;
        //一般输入的属性
        if (tokens[ptr] == "unique" && type[ptr] == IDENTIFIER)
        {

            ptr++;
            if (tokens[ptr] != "(" && type[ptr] != SYMBOL)
            {
                reportUnexpected("createTable", "'('");
                return;
            }

            ptr++;
            if (type[ptr] == IDENTIFIER)
            {   
                bool set_u = false;
                 for(vector<Attribute>::iterator iter=a.begin();iter!=a.end();++iter)
                {
                    Attribute& k = *iter;
                    if(k.get_name() == tokens[ptr])
                    {
                        k.set_Unique(true);
                        if(primary->get_name() == tokens[ptr])
                        {
                        	primary->set_Unique(true);
						}
                        set_u = true;
                    }
                    if(set_u == false)
                    {
                        cerr << "ERROR: [Interpreter::createTable] Not exists unique definition." << endl;
                        return;
                    }
                }
            }
            else
            {
                reportUnexpected("createTable", "unique name or '('");
                return;
            }


            ptr++;
            if (tokens[ptr] != ")" || type[ptr] != SYMBOL)
            {
                reportUnexpected("createTable", "')'");
                return;
            }
        }
       
        
        //最后标记的primary key
         else if (tokens[ptr] == "primary" && type[ptr] == IDENTIFIER)
        {

            ptr++;
            if (tokens[ptr] != "key" || type[ptr] != IDENTIFIER)
            {
                reportUnexpected("createTable", "'key'");
                return;
            }

            ptr++;
            if (tokens[ptr] != "(" && type[ptr] != SYMBOL)
            {
                reportUnexpected("createTable", "'('");
                return;
            }

            ptr++;
            if (type[ptr] == IDENTIFIER)
            {
                if (hasprimary == false)
                {  
                    for(vector<Attribute>::iterator iter=a.begin();iter!=a.end();++iter)
                    {
                        Attribute& k = *iter;
                     if(k.get_name() == tokens[ptr])
                     {
                        k.set_primary(true);
                        k.set_Unique(true);
                    //    cout<<k.get_name()<<endl;
                         primary = &k;
                        hasprimary = true;
                     }
                    }
                    if(hasprimary == false)
                    {
                        cerr << "ERROR: [Interpreter::createTable] Not exists primary key definition." << endl;
                        return;
                    }
                }
                else
                {
                 cerr << "ERROR: [Interpreter::createTable] Multiple primary key definition." << endl;
                    return;   
                }
            }
            else
            {
                reportUnexpected("createTable", "primary key name or '('");
                return;
            }


            ptr++;
            if (tokens[ptr] != ")" || type[ptr] != SYMBOL)
            {
                reportUnexpected("createTable", "')'");
                return;
            }
        }
        
        else if (type[ptr] == IDENTIFIER)
        {
            string temp_name = tokens[ptr];

            int temp_type = GetType();
            int len = 0;
            if (temp_type<0)
                return;
            else if(temp_type == TYPE_CHAR)
            {
                ptr++;
                if (tokens[ptr] != "(" || type[ptr] != SYMBOL)
                {
                 reportUnexpected("GetType", "'('");
                 return;
                }
                else
                {
                    ptr++;
                    if (type[ptr] != NUMBER)
                   {
                     reportUnexpected("GetType", "'length'");
                     return;
                   }
                    const char* l = tokens[ptr].c_str();
                    temp_type = atoi(l);  
                }
                ptr++;
                if (tokens[ptr] != ")" || type[ptr] != SYMBOL)
                 {
                  reportUnexpected("GetType", "')'");
                  return;
                 }
            }
            
            bool isunique = false;
            bool isprimary = false;
            if (tokens[ptr+1] == "unique" && type[ptr+1] == IDENTIFIER)
            {  
                ptr++;
                isunique = true ;
                Attribute* temp = new Attribute(temp_name, temp_type, isprimary, isunique);
                a.push_back(*temp);
                delete temp;
            }
            else if(tokens[ptr+1] == "primary" && type[ptr+1] == IDENTIFIER)
            {
                ptr++;
                if(tokens[ptr+1] == "key" && type[ptr+1] == IDENTIFIER)
                {
                    ptr++;
                    isprimary = true;
                    isunique = true;
                   Attribute* temp = new Attribute(temp_name, temp_type, isprimary, isunique);
                   a.push_back(*temp);
                   *primary = *temp;
                   delete temp;
                    hasprimary = true;
                }
                else 
                {
                    reportUnexpected("createTable", "'primary key'");
                    return;
                }
            }
            else
            {
                Attribute* temp = new Attribute(temp_name, temp_type, isprimary, isunique);
                a.push_back(*temp);
                delete temp;
            }
        }

        else
        {
            reportUnexpected("createTable", "column name or 'primary' or 'unique'");
            return;
        }

        ptr++;
        if (tokens[ptr] == ")" && type[ptr] == SYMBOL)
            break;
        else if (tokens[ptr] != "," || type[ptr] != SYMBOL)
        {
            reportUnexpected("createTable", "','");
            return;
        }
    }

    ptr++;
    if (type[ptr] != END)
    {
        reportUnexpected("createTable", "';'");
        return;
    }

    if (hasprimary == false)
    {
        cerr << "ERROR: [Interpreter::createTable] No primary key definition!" << endl;
        return;
    }

    // Do creation
    int tic, toc;
    bool res;
    tic = clock();
    res = api.create_table(table_name, *primary, a);
    toc = clock();
    
    //运行时间
    if (res &&!fromFile)
        cout << "1 table created. Query done in " << 1.0 * (toc-tic) / CLOCKS_PER_SEC << "s." << endl;

/*   cout<<table_name<<endl;
   for(vector<Attribute>::iterator iter=a.begin();iter!=a.end();++iter)
        {
          Attribute k = *iter;
    	cout<<k.get_name()<<" "<<k.get_type()<<" "<<k.get_length()<<" "<<k.is_primary()<<" "<<k.is_unique()<<endl;
	    }
	if(hasprimary==true){
	cout<<primary->get_name()<<" "<<primary->get_type()<<" "<<primary->get_length()<<" "<<primary->is_primary()<<" "<<primary->is_unique()<<endl; 
    }
*/ 
}

//插入记录
void Interpreter::insert()
{
    API & api = MiniSQL::get_api();
    ptr++;
    if (tokens[ptr] != "into" || type[ptr] != IDENTIFIER)
    {
        reportUnexpected("insert", "'into'");
        return;
    }

    ptr++;
    if (type[ptr] != IDENTIFIER)
    {
        reportUnexpected("insert", "table name");
        return;
    }

    const string table_name = tokens[ptr];
    vector<string> data;
    vector<int> datatype;

    ptr++;
    if (tokens[ptr] != "values" || type[ptr] != IDENTIFIER)
    {
        reportUnexpected("insert", "'values'");
        return;
    }

    ptr++;
    if (tokens[ptr] != "(" || type[ptr] != SYMBOL)
    {
        reportUnexpected("insert", "'('");
        return;
    }

    while (true)
    {
        ptr++;
        if (type[ptr] != NUMBER && type[ptr] != SCHAR && type[ptr] != STRING)
        {
            reportUnexpected("insert", "value");
            return;
        }
        data.push_back(tokens[ptr]);
        if(type[ptr] == NUMBER)
        {
            string s = tokens[ptr];

            char  str[s.length()];
            strcpy(str,s.c_str());
            bool f = false;
            for(int i = 0; i<s.length(); i++)
            {
                if(str[i] == '.')
                {
                    datatype.push_back(TYPE_FLOAT);
                    f = true;
                }
            }
            if(f == false)
            {
                datatype.push_back(TYPE_INT);
            }
        }
        else
        {
        	 string s = tokens[ptr];
            datatype.push_back(s.length());
        }

        ptr++;
        if (tokens[ptr] == ")" && type[ptr] == SYMBOL)
            break;
        else if (tokens[ptr] != "," || type[ptr] != SYMBOL)
        {
            reportUnexpected("insert", "','");
            return;
        }
    }

    ptr++;
    if (type[ptr] != END)
    {
        reportUnexpected("insert", "';'");
        return;
    }
 
    // Do insertion
    int tic, toc;
    bool res;

    tic = clock();
    res = api.insert(table_name, data, datatype);
    toc = clock();

    //运行时间
    
    if (res &&!fromFile)
        cout << "1 record inserted. Query done in " << 1.0 * (toc-tic) / CLOCKS_PER_SEC << "s." << endl;
    
     /*
    cout<<table_name<<endl;
      for(vector<string>::iterator iter=data.begin();iter!=data.end();++iter)
    {
    	cout<<*iter<<" ";
	}
	cout<<endl;
	  for(vector<int>::iterator iter=datatype.begin();iter!=datatype.end();++iter)
    {
    	cout<<*iter<<" ";
	}
	cout<<endl;
*/ 
}

//运行文件
void Interpreter::execfile()
{
    API & api = MiniSQL::get_api();
    ptr++;
    if (type[ptr] != SCHAR && type[ptr] != STRING)
    {
        reportUnexpected("execfile", "a string as filename");
        return;
    }
    const char* filename = tokens[ptr].c_str();

    ptr++;
    if (type[ptr] != END)
    {
        reportUnexpected("execfile", "';'");
        return;
    }

    if (fromFile)
    {
        cerr << "ERROR: [Interpreter::execfile] Cannot do 'execfile' instruction when executing from file." << endl;
        return;
    }

    // 读取文件
    
    ifstream file(Method::AbsolutePath(filename));
    if (!file.is_open())
    {
        cerr << "ERROR: [Interpreter::execfile] Cannot load file " << filename << "!" << endl;
        return;
    }
    string line, content = "";
    while (!file.eof())
    {
        getline(file, line);
        content += line + '\n';
    }

    int tic, toc;
    Interpreter* interpreter = new Interpreter(true);
    tic = clock();
    interpreter->execute(content.c_str());
    toc = clock();

    //输出运行时间
    cout << " queries done in " << 1.0 * (toc-tic) / CLOCKS_PER_SEC << "s." << endl;

    delete interpreter;
}

// 退出
void Interpreter::exit()
{
    BufferManager & buffermanager = MiniSQL::get_buffer_manager();
    ptr++;
    if (type[ptr] != END)

    {
        reportUnexpected("exit", "';'");
        return;
    }

    if (fromFile)
        cerr << "ERROR: [Interpreter::exit] Cannot do 'exit' instruction when executing from file." << endl;
    else
    {
        cout << "Bye~ :)" << endl;
        exiting = true;
    }
    buffermanager.WriteAllBack();
}





