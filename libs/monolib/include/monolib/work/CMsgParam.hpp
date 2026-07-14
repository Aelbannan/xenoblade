#pragma once

#include <types.h>

#include "monolib/work/CWorkThreadSystem.hpp"

//Message param entry format:
//0x0: message id
//0x4: work ID
//0x8: address
//0xC: code address
//0x10: value
//0x14: address
//0x18: address
//0x1C: address
//0x20: address (pointer to string?)
struct CMsgParamEntry{
    u32 command; //0x0
    WORK_ID wid; //0x4
    u32 unk8;
    u32 unkC;
    u32 unk10;
    u32 unk14;
    u32 unk18;
    u32 unk1C;
    u16 unk20;
    u8 unk22;
    u8 unk23;
};

template <int N>
class CMsgParam{
public:
    CMsgParam(u32 r4){
        mCapacity = N;
        mArrayPtr = mEntries;
        mSize = 0;
        mFront = 0;
        field6 = 0;
        field7 = r4;
    }

    virtual ~CMsgParam(){
        clear();
    }

    void clear(){
        mSize = 0;
        mFront = 0;
    }

    bool empty() const{
        return mSize == 0;
    }

    u32 size() const{
        return mSize;
    }

    const CMsgParamEntry& front() const{
        return mArrayPtr[mFront % mCapacity];
    }

    void enqueue(u32 msg){
        volatile CMsgParamEntry entry;
        int index = (int)(mFront + mSize) % (int)mCapacity;
        u8* dst = reinterpret_cast<u8*>(mArrayPtr);

        *reinterpret_cast<u32*>(dst += index * sizeof(CMsgParamEntry)) = msg;
        *reinterpret_cast<u32*>(dst + 0x4) = entry.wid;
        *reinterpret_cast<u32*>(dst + 0x8) = entry.unk8;
        *reinterpret_cast<u32*>(dst + 0xC) = entry.unkC;
        *reinterpret_cast<u32*>(dst + 0x10) = entry.unk10;
        *reinterpret_cast<u32*>(dst + 0x14) = entry.unk14;
        *reinterpret_cast<u32*>(dst + 0x18) = entry.unk18;
        *reinterpret_cast<u32*>(dst + 0x1C) = entry.unk1C;
        *reinterpret_cast<u16*>(dst + 0x20) = entry.unk20;
        *(dst + 0x22) = entry.unk22;
        *(dst + 0x23) = 0;

        mSize++;
        field6 = mSize - 1;
    }

    CMsgParamEntry& last(){
        return mArrayPtr[(mFront + field6) % mCapacity];
    }

    void pop(){
        mSize--;
        mFront = (mFront + 1) % mCapacity;
    }

    int find(u32 msg) const{
        for(int i = 0; i < mSize; i++){
            if(mArrayPtr[(mFront + i) % mCapacity].command == msg){
                return i;
            }
        }

        return -1;
    }

private:
    //0x0: vtable
    CMsgParamEntry mEntries[N]; //0x4
    CMsgParamEntry* mArrayPtr; //N*0x24 + 0x4
    u32 mFront; //N*0x24 + 0x8
    u32 mSize; //N*0x24 + 0xC
    u32 mCapacity; //N*0x24 + 0x10
    u32 field6; //N*0x24 + 0x14
    u32 field7; //N*0x24 + 0x18
};
