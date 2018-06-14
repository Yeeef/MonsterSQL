
#include "interpreter.h"
#include <fstream>

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

    cout << "MonsterSQL👹" << endl;
}