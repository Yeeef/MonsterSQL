#ifndef API_H
#define API_H
#include "config.h"

class API
{
  public:
    API(){};
    ~API(){};

    bool create_table(const string &table_name, const Attribute &primary, const vector<Attribute> &attributes) const throw(Error);
    bool drop_table(const string &table_name) const throw(Error);
    bool insert(const string &table_name, const vector<string> &insert_data, const vector<int> &type) const throw(Error);
    bool Delete(const string &table_name, const vector<string> &attribute_name,
                const vector<int> &condition, const vector<string> &operand) const throw(Error);
    bool select(const string &table_name, const vector<string> &attribute_name,
                const vector<int> &condition, const vector<string> &operand) const throw(Error);
    bool create_index(const string &table_name, const string &attribute_name,
                      const string &index_name) const throw(Error);
    bool drop_index(const string &index_name) const throw(Error);
};

#endif