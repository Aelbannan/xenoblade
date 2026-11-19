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

// The fixed ring is a non-polymorphic member of the message interface.
// Its destructor clears the counters; CMsgParam itself has an empty virtual
// destructor. This accounts for retail's guarded cleanup at this + 4.
// CMsgParamRing is an inferred name.
template <int N>
class CMsgParamRing {
public:
    CMsgParamRing(){
        mCapacity = N;
        mArrayPtr = mEntries;
        mSize = 0;
        mFront = 0;
        field6 = 0;
    }

    ~CMsgParamRing(){
        clear();
    }

    void clear(){
        mSize = 0;
        mFront = 0;
    }

    CMsgParamEntry mEntries[N];
    CMsgParamEntry* mArrayPtr;
    u32 mFront;
    u32 mSize;
    u32 mCapacity;
    u32 field6;
};

// vptr + ring: sizeof(CMsgParam<N>) = 0x18 + N * 0x24.
// CView's 10-entry queue spans 0x280..0x400; its name follows at 0x400.
template <int N>
class CMsgParam {
public:
    CMsgParam(){}
    virtual ~CMsgParam(){}

    void clear(){
        mRing.clear();
    }

    bool empty() const{
        return mRing.mSize == 0;
    }

    u32 size() const{
        return mRing.mSize;
    }

    const CMsgParamEntry& front() const{
        return mRing.mArrayPtr[mRing.mFront % mRing.mCapacity];
    }

    void enqueue(u32 msg){
        volatile CMsgParamEntry entry;
        u32 value4 = entry.wid;
        u32 value8 = entry.unk8;
        u32 valueC = entry.unkC;
        u32 value10 = entry.unk10;
        u32 value14 = entry.unk14;
        u32 value18 = entry.unk18;

        // Ring buffer index: (mRing.mFront + mRing.mSize) % mRing.mCapacity, entry stride 0x24.
        CMsgParamEntry& target = mRing.mArrayPtr[(int)(mRing.mFront + mRing.mSize) % (int)mRing.mCapacity];

        u32 value1C = entry.unk1C;
        u16 value20 = entry.unk20;
        u8 value22 = entry.unk22;

        target.command = msg;
        target.wid = value4;
        target.unk8 = value8;
        target.unkC = valueC;
        target.unk10 = value10;
        target.unk14 = value14;
        target.unk18 = value18;
        target.unk1C = value1C;
        target.unk20 = value20;
        target.unk22 = value22;
        target.unk23 = 0;

        mRing.mSize++;
        mRing.field6 = mRing.mSize - 1;
    }

    CMsgParamEntry& last(){
        return mRing.mArrayPtr[(mRing.mFront + mRing.field6) % mRing.mCapacity];
    }

    void pop(){
        mRing.mSize--;
        mRing.mFront = (mRing.mFront + 1) % mRing.mCapacity;
    }

    int find(u32 msg) const;

    CMsgParamRing<N> mRing; //0x4
};
