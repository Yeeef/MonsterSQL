#ifndef CONFIG_H
#define CONFIG_H
#include <iostream>

using namespace std;

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

class Error
{
    private:
    string error_info;

    public:
    Error(string error_info) : error_info(error_info) {}
    ~Error();

    void print_error() { cout << error_info << endl; }
    
};



#endif