# PLAN 🤯

## 结构说明

- 利用 `CMakeLists.txt` 构建
- 各个文件夹内部为对应的 `.h` `.cpp / .cc`
- 特别注意 `!cmake` 文件夹，这是我为了使用 `cmake` 生成 `Makefile` 过程不要把相关文件都存进主目录下
- 不会用 `cmake` 可以上网学习一下，可以跨平台生成 `Makefile`,方便我们之间跨平台(windows <-> Mac OS)交互，不想学完全可以，用 `CLion`，根据我给的 `CMakeLists.txt` 它应该正常编译，如果不行，把错误发群里，我们再讨论

## API接口

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
    ~Attribute();

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