//
// Created by emma on 2018/6/14.
//

#ifndef MONSTERSQL_BPTREEKEY_H
#define MONSTERSQL_BPTREEKEY_H

#include "config.h"
class BPTreeKey {
public:

    /*
     * 拷贝构造只有在不为nullptr时才new一块空间
     */
    BPTreeKey(const BPTreeKey& copy);
    BPTreeKey(const char* rawData, ptr& Pointer, int data_type);
    BPTreeKey(const char* rawData,int id, int data_type );
    BPTreeKey(const char* rawData, int data_type);
    BPTreeKey(){ rawData = nullptr; }

    ~BPTreeKey()
    {
#ifdef DEBUGINDEX
        if(rawData == nullptr)
        {
            cout << "fuck" << endl;
        }
#endif
        if(rawData != nullptr)
        {
            delete [] rawData;
        }
    }
    const char* getPointerRawData(){return Pointer.get_rawdata();}
    int getPointer(){return Pointer.get_id();}
    ptr getPtrClass(){return Pointer;}
    const char* getKeyRawData() const {return rawData;}
    void setPointer(int nodeID){Pointer.set_id(nodeID);}
    int getDataType()const {return dataType;}
    void setKey(const char* raw_data, int nodeID);
    void setKey(const char* raw_data);


    //todo:记得要更新rawData的具体值
//    void setKey(int keyValue, int dataType) { key = keyValue; Method::int2rawdata(keyValue, rawData);}
//    void setKey(float keyValue, int dataType)   {key = keyValue; Method::float2rawdata(keyValue, rawData);}
//    void setKey(char * keyValue, int dataType)   { key = keyValue; rawData = keyValue;}




    bool operator > (const BPTreeKey& entry );
    bool operator < (const BPTreeKey& entry );
    bool operator == (const BPTreeKey& entry );
    bool operator != (const BPTreeKey& entry );
    BPTreeKey& operator = (const BPTreeKey& entry );

private:
    char * rawData;
    int dataType;
    ptr Pointer;

private:
    int compareKey(const BPTreeKey& key);
    void rawData2key();



};


#endif //MONSTERSQL_BPTREEKEY_H
