# PLAN 🤯

## 说明

- 程序框架构建
  - `interpreter` 作为前端
  - `minisql` 作为后端
  - `config.h` 中包含公用类，可以移植到 minisql.h 中

- 大致框架已经写好，API接口暂时这样规定，有问题立即提出

- 如何调用 `API` 等，我在 `interpreter.h` 中内置了一个简单程序，可以看一下

- 注意调用的时候要用 `&` 引用，我们能不用指针则不用指针


## minisql 类

```c++
class MiniSQL
{
    private:
    static API & api;
    static CatalogManager & catalog_manager;
    static RecordManager & record_manager;
    static BufferManager & buffer_manager;
    static IndexManager & index_manager;
    

    public:
    MiniSQL();
    ~MiniSQL();

    static API & get_api();
    static CatalogManager & get_catalog_manager();
    static RecordManager & get_record_manager();
    static BufferManager & get_buffer_manager();
    static IndexManager & get_index_manager();
};
```

## API接口

```c++
class API
{
    private:
    public:
    API(){};
    ~API(){};

    bool create_table(const string & table_name, const vector <Attribute> & attributes) const throw(Error);
    bool drop_table(const string & table_name) const throw(Error);
    bool insert(const string & table_name, const vector <Attribute> attributes) const throw(Error);
    bool Delete(const string & table_name, const vector <string> primary) const throw(Error);
    bool select(const string & table_name, const vector <string> primary) const throw(Error);
    bool create_index(const string & table_name, const string & attribute_name, 
                      const string & index_name) const throw(Error);
    bool drop_index(const string & table_name, const string & attribute_name, 
                    const string & index_name) const throw(Error);
    
    
};
```
## 一些通用类

- 通用类存在 `Config` 目录下
- Error用于错误处理，层层throw最终在 `interpreter` 进行 catch? 还是 `API` 进行 catch?
- MiniSQL类集成 ....
- `interpreter` 类单独处理，作为 **前端**

```c++
class Attribute
{
    private:
    string name;
    bool isPrimary;
    bool isUnique;

    public:
    Attribute(string name, bool isPrimary = false, bool isUnique = false)
    : name(name), isPrimary(isPrimary), isUnique(isUnique) {}
    ~Attribute(){};

    string get_name() { return name; }
    bool is_primary() { return isPrimary; }
    bool is_unique() { return isUnique; }

};

// 集中管理 throw Error
class Error
{
    private:
    string error_info;

    public:
    Error(string error_info) : error_info(error_info) {}
    ~Error();
    string print_err() { cout << error_info << endl;}
};
```