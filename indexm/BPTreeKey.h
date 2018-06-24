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
    BPTreeKey(const char* rawData, int pointerID, int data_type);
    BPTreeKey(const char* rawData, int data_type);
    /*
     * default pointerID = -1; dataType = TYPE_INT;
     */
    BPTreeKey(){ rawData = nullptr; pointerID = -1; dataType = TYPE_INT; }

    ~BPTreeKey()
    {
#ifdef DEBUGINDEX
 
#endif
        if(rawData != nullptr)
        {
            delete [] rawData;
        }
    }
    //const char* getPointerRawData(){return }
    int getPointer(){return pointerID;}
    const char* getKeyRawData() const {return rawData;}
    void setPointer(int nodeID){pointerID = nodeID;}
    int getDataType()const {return dataType;}
    void setKey(const char* raw_data, int nodeID);
    void setKey(const char* raw_data);



    bool operator > (const BPTreeKey& entry );
    bool operator < (const BPTreeKey& entry );
    bool operator == (const BPTreeKey& entry );
    bool operator != (const BPTreeKey& entry );
    BPTreeKey& operator = (const BPTreeKey& entry );

private:
    char * rawData;
    int dataType;
    int pointerID;

private:
    int compareKey(const BPTreeKey& key);
    //void rawData2key();



};


#endif //MONSTERSQL_BPTREEKEY_H
