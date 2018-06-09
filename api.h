#ifndef API_H
#define API_H
#include "config.h"



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

#endif