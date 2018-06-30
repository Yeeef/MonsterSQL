//
// Created by emma on 2018/6/14.
//

#include "BPTreeKey.h"

BPTreeKey::BPTreeKey(const char *raw_data, int pointerID, int dataType): dataType(dataType), pointerID(pointerID)
{
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
    //rawData2key();


//BPTreeKey::BPTreeKey(const char *raw_data, int data_type) :dataType(data_type)
//{
//
//    pointerID = -1;
//    int keyLength = Method::getLengthFromType(dataType);
//    if(raw_data != nullptr)
//    {
//        rawData = new char[keyLength];
//        memcpy(rawData,raw_data, keyLength);
//    } else
//    {
//        rawData = nullptr;
//    }
//    //rawData2key();
//}

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
        default:
        {
            if(thatkey.getKeyRawData() == nullptr && this->rawData == nullptr)
                return 0;
            else
            {
                int keyLength = Method::getLengthFromType(dataType);
                string thisKeydata,thatkeydata;
                Method::rawdata2string(thatkey.getKeyRawData(),keyLength, thisKeydata);
                Method::rawdata2string(this->getKeyRawData(), keyLength, thatkeydata);
                if(thisKeydata == thatkeydata)
                    return 0;
                else if(thisKeydata < thatkeydata)
                    return 1;
                else return -1;
            }
        }
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
    if(raw_data == nullptr)
    {
        rawData = nullptr;
    }else
        memcpy(rawData,raw_data, Method::getLengthFromType(dataType));

    pointerID = nodeID;

}

void BPTreeKey::setKey(const char *raw_data) {
    if(raw_data == nullptr)
    {
        delete rawData;
        rawData = nullptr;
    }
    else
    {
        int keyLength = Method::getLengthFromType(dataType);
        if(rawData == nullptr)
        {
            rawData = new char[keyLength];
        }
        memcpy(rawData,raw_data, keyLength);
    }

}

BPTreeKey& BPTreeKey::operator=(const BPTreeKey &entry) {
    /*
     * 是只想要它的值还是recordID也要？
     *
     */
    assert(entry.dataType == this->dataType);
    int keyLength = Method::getLengthFromType(dataType);
    if(entry.rawData == nullptr)
        rawData = nullptr;
    else if(rawData == nullptr)
    {
        rawData = new char[keyLength];
    }
    memcpy(rawData,entry.getKeyRawData(), keyLength);
    this->pointerID = entry.pointerID;
    return *this;
}

BPTreeKey::BPTreeKey(const BPTreeKey &copy) :dataType(copy.dataType){
    pointerID = copy.pointerID;
    int keyLength = Method::getLengthFromType(dataType);

    if(copy.rawData == nullptr)
        this->rawData = nullptr;
    else
    {
        this->rawData = new char[keyLength];

        memcpy(this->rawData,copy.rawData, keyLength);
    }

}




