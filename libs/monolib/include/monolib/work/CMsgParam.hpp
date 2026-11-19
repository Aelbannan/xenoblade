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

// Layout (from retail): vptr, N * 0x24 entries, then five tail words
// { mArrayPtr, mFront, mSize, mCapacity, field6 } -> sizeof = 0x18 + N*0x24.
// CView::mContextMsg (N=10) reads 0x280..0x400 immediately followed by the
// view's FixStr<64> name at 0x400, and __ct__CMsgParam_32 (CGXCache) inits
// exactly these five words before the cache's own message-target pointer.
template <int N>
class CMsgParam{
public:
    CMsgParam(){
        mCapacity = N;
        mArrayPtr = mEntries;
        mSize = 0;
        mFront = 0;
        field6 = 0;
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
        u32 value4 = entry.wid;
        u32 value8 = entry.unk8;
        u32 valueC = entry.unkC;
        u32 value10 = entry.unk10;
        u32 value14 = entry.unk14;
        u32 value18 = entry.unk18;

        // Ring buffer index: (mFront + mSize) % mCapacity, entry stride 0x24.
        CMsgParamEntry& target = mArrayPtr[(int)(mFront + mSize) % (int)mCapacity];

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

    int find(u32 msg) const;

    // Ring state (public like _reslist_base's): retail inlines the ring ops at
    // several call sites with site-specific register colouring that the
    // enqueue()/last() bodies cannot reproduce, so those sites address the
    // slots directly.
    //0x0: vtable
    CMsgParamEntry mEntries[N]; //0x4
    CMsgParamEntry* mArrayPtr; //N*0x24 + 0x4
    u32 mFront; //N*0x24 + 0x8
    u32 mSize; //N*0x24 + 0xC
    u32 mCapacity; //N*0x24 + 0x10
    u32 field6; //N*0x24 + 0x14 (index of the newest entry)
};
