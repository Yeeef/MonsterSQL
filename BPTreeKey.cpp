//
// Created by emma on 2018/6/14.
//

#include "BPTreeKey.h"
#include <cmath>
BPTreeKey::BPTreeKey(const char *raw_data, ptr& Pointer, int dataType): dataType(dataType)
{
    this->Pointer.set_id(Pointer.get_id());
    int keyLength = Method::getLengthFromType(dataType);
    if(raw_data != nullptr)
    {
        rawData = new char[keyLength];
        memcpy(rawData,raw_data, keyLength);
    } else
    {
        rawData = nullptr;
    }

    //rawData2key();


}

BPTreeKey::BPTreeKey(const char *raw_data, int id, int data_type):dataType(data_type) {

    Pointer.set_id(id);
    int keyLength = Method::getLengthFromType(dataType);

    if(raw_data != nullptr)
    {
        rawData = new char[keyLength];
        memcpy(rawData,raw_data, keyLength);
    } else
    {
        rawData = nullptr;
    }


    //rawData2key();

}

BPTreeKey::BPTreeKey(const char *raw_data, int data_type) :dataType(data_type)
{

    Pointer = -1;
    int keyLength = Method::getLengthFromType(dataType);
    if(raw_data != nullptr)
    {
        rawData = new char[keyLength];
        memcpy(rawData,raw_data, keyLength);
    } else
    {
        rawData = nullptr;
    }
    //rawData2key();
}

//void BPTreeKey::rawData2key() {
//    if(dataType == TYPE_CHAR)
//        key = rawData;
//    else if(dataType == TYPE_INT)
//        key = Method::rawdata2int(rawData);
//    else if(dataType == TYPE_FLOAT)
//        key = Method::rawdata2int(rawData);
//}



/*
 * this->key 比较大，返回-1
 * 相等返回0
 * 客主比较大，返回1
 */
int BPTreeKey::compareKey(const BPTreeKey &thatkey) {

#if DEBUG

    //用来检验合法性问题
    assert(thatkey.getDataType() == dataType);

#endif

    switch(dataType)
    {
        case TYPE_INT:
        {
            int thisdata = Method::rawdata2int(rawData);
            int thatdata = Method::rawdata2int(thatkey.rawData);
            if(thisdata == thatdata)
                return 0;
            else if(thisdata < thatdata)
                return 1;
            else return -1;
        }

        case TYPE_FLOAT:
        {
            float thisdata = Method::rawdata2float(rawData);
            float thatdata = Method::rawdata2float(thatkey.rawData);
            if(fabs(thatdata - thisdata) < 0.000001)
                return 0;
            else if(thatdata > thisdata)
                return 1;
            else return -1;
        }


            //对char默认字典序，怎么写？
        case TYPE_CHAR:
        {
            return strncmp(thatkey.rawData, rawData, Method::getLengthFromType(dataType));
        }

        default:
            return 0;
    }
}

bool BPTreeKey::operator > (const BPTreeKey &entry) {
    return compareKey(entry) == -1;
}

bool BPTreeKey::operator < (const BPTreeKey &entry) {
    return compareKey(entry) == 1;
}

bool BPTreeKey::operator==(const BPTreeKey &entry) {
    return compareKey(entry) == 0;
}


bool BPTreeKey::operator!=(const BPTreeKey &entry) {
    return compareKey(entry) != 0;
}


void BPTreeKey::setKey(const char *raw_data, int nodeID) {
    memcpy(rawData,raw_data, Method::getLengthFromType(dataType));
    Pointer.set_id(nodeID);

}

void BPTreeKey::setKey(const char *raw_data) {
    memcpy(rawData,raw_data, Method::getLengthFromType(dataType));
}

BPTreeKey& BPTreeKey::operator=(const BPTreeKey &entry) {
    memcpy(rawData,entry.getKeyRawData(), Method::getLengthFromType(dataType));
}

BPTreeKey::BPTreeKey(const BPTreeKey &copy) :dataType(copy.dataType){
    Pointer.set_id(copy.Pointer.get_id());
    int keyLength = Method::getLengthFromType(dataType);

    if(copy.rawData == nullptr)
        this->rawData = nullptr;
    else
    {
        this->rawData = new char[keyLength];

        memcpy(rawData,copy.rawData, keyLength);
    }

}




