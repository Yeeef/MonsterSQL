
#include "interpreter.h"
#include <fstream>
#include <sstream>

// todo: 了解 reinterpret



int main()
{



    Interpreter test;
    test.print();

    /*
    FILE * file = fopen("test.bin", "wb");
    char data[32];
    int * p2;
    int a = 123434;
    p2 = &a;
    memcpy(data, p2, sizeof(int));
    cout << data << endl;
    fwrite(data, 32, 1, file);
    file = fopen("test", "rb");
    fread(data, 32, 1, file);
    memcpy(p2, data, sizeof(int));
    cout << *p2 << endl;
    */
    /*
    fstream file("test", ios::in | ios::out | ios::binary);
    if (!file.is_open())
    {
        cout << "no" << endl;
    }
    int *p2;
    int a = 520;
    p2 = &a;
    //file.write(reinterpret_cast<char *>(p2), sizeof(int));

    char aa[4];
    file.read(aa, sizeof(int));
    p2 = reinterpret_cast<int *>(aa);

    cout << *p2 << endl;
    ;
    memcpy(p2, aa, sizeof(int));
    cout << *p2 << endl;
    */

    cout << "MonsterSQL👹" << endl;

    int block_id = 4;
    short record_id = 2;
    char data[6];
    memcpy(data, reinterpret_cast<char *>(&block_id), sizeof(int));
    memcpy(data + 4, reinterpret_cast<char *>(&record_id), sizeof(short));
    int *p1 = new int;
    short *p2 = new short;
    memcpy(p1, data, 4);
    memcpy(p2, data + 4, 2);
    cout << *p1 << endl;
    cout << *p2 << endl;

    // copy to string is not ok!
    string data_str(data);
    const char *dataa = data_str.c_str();
    memcpy(p1, dataa, 4);
    memcpy(p2, dataa + 4, 2);
    cout << *p1 << endl;
    cout << *p2 << endl;

    ptr Pointer(4);
    memcpy(p1, Pointer.get_rawdata(), 4);

    cout << *p1 << endl;

    delete p2;
    delete p1;

    vector<string> insert_data = {"1", "2.2", "1"};
    vector<char *> raw_Vec(3);
    vector<int> type = {TYPE_INT, TYPE_FLOAT, TYPE_CHAR};
    vector<int> length = {sizeof(int), sizeof(int), 255};

    for (int i = 0; i < 3; i++)
    {
        stringstream stream;
        stream << insert_data.at(i);
        switch (type.at(i))
        {
        case TYPE_INT:
        {
            int temp;
            stream >> temp;
            char *temp_data = new char[INT_LENGTH];
            memcpy(temp_data, (&temp), INT_LENGTH);
            raw_Vec.at(i) = temp_data;
            break;
        }
        case TYPE_FLOAT:
        {
            float temp;
            stream >> temp;
            char *temp_data = new char[FLOAT_LENGTH];
            memcpy(temp_data, (&temp), INT_LENGTH);
            raw_Vec.at(i) = temp_data;

            break;
        }
        case TYPE_CHAR:
        {

            char * temp_data = new char[CHAR_LENGTH];
            stream >> temp_data;
            raw_Vec.at(i) = temp_data;
            break;
        }
            
        }
    }
    int * p3 = new int;
    p3 = reinterpret_cast<int *>(raw_Vec.at(0));
    float * p4 = new float;
    p4 = reinterpret_cast<float*>(raw_Vec.at(1));
    char * p5 = new char[255];
    p5 = reinterpret_cast<char*>(raw_Vec.at(2));
    cout << *p3 << endl;
    cout << *p4 << endl;
    cout << *p5 << endl;
    delete p3;
    delete p4;
    delete [] p5;
        


    return 0;
}