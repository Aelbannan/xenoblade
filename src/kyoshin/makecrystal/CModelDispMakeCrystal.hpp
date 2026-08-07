#pragma once

#include <types.h>

// Full object layout for CModelDispMakeCrystal (used by C-linkage accessors)
struct CModelDispMakeCrystalFull {
    u8 _00[0xBE8];
    u8 field_BE8;
    u8 _BE9[0x2DC3 - 0xBE9];
    u8 field_2DC3;
    u8 _2DC4[0x2DD2 - 0x2DC4];
    u8 field_2DD2;
};

// Inline anim object at sub+0x08 of CMCrystalDispSub; vtable flush call at
// +0xE0 (index 0x38), used by func_80220E14 to flush the buffered anim.
struct CMCryAnimVt {
    virtual void m00(); virtual void m01(); virtual void m02(); virtual void m03();
    virtual void m04(); virtual void m05(); virtual void m06(); virtual void m07();
    virtual void m08(); virtual void m09(); virtual void m0A(); virtual void m0B();
    virtual void m0C(); virtual void m0D(); virtual void m0E(); virtual void m0F();
    virtual void m10(); virtual void m11(); virtual void m12(); virtual void m13();
    virtual void m14(); virtual void m15(); virtual void m16(); virtual void m17();
    virtual void m18(); virtual void m19(); virtual void m1A(); virtual void m1B();
    virtual void m1C(); virtual void m1D(); virtual void m1E(); virtual void m1F();
    virtual void m20(); virtual void m21(); virtual void m22(); virtual void m23();
    virtual void m24(); virtual void m25(); virtual void m26(); virtual void m27();
    virtual void m28(); virtual void m29(); virtual void m2A(); virtual void m2B();
    virtual void m2C(); virtual void m2D(); virtual void m2E(); virtual void m2F();
    virtual void m30(); virtual void m31(); virtual void m32(); virtual void m33();
    virtual void m34(); virtual void m35(); virtual void m36(); virtual void m37();
    virtual void m38();
};

// Virtual dispatch on the crystal-charged actor's embedded CfObjectMove at
// +0x3e9c: method +0x74 (index 29) and +0x148 (index 82, one int arg).
struct CMCryMoveVt {
    virtual void m00(); virtual void m01(); virtual void m02(); virtual void m03();
    virtual void m04(); virtual void m05(); virtual void m06(); virtual void m07();
    virtual void m08(); virtual void m09(); virtual void m0A(); virtual void m0B();
    virtual void m0C(); virtual void m0D(); virtual void m0E(); virtual void m0F();
    virtual void m10(); virtual void m11(); virtual void m12(); virtual void m13();
    virtual void m14(); virtual void m15(); virtual void m16(); virtual void m17();
    virtual void m18(); virtual void m19(); virtual void m1A(); virtual void m1B();
    virtual void m1C(); virtual void m1D(); virtual void m1E(); virtual void m1F();
    virtual int  m20(); virtual int  m21(); virtual int  m22(); virtual int  m23();
    virtual int  m24(); virtual int  m25(); virtual int  m26(); virtual int  m27();
    virtual int  m28(); virtual int  m29(); virtual int  m2A(); virtual int  m2B();
    virtual int  m2C(); virtual int  m2D(); virtual int  m2E(); virtual int  m2F();
    virtual int  m30(); virtual int  m31(); virtual int  m32(); virtual int  m33();
    virtual int  m34(); virtual int  m35(); virtual int  m36(); virtual int  m37();
    virtual int  m38(); virtual int  m39(); virtual int  m3A(); virtual int  m3B();
    virtual int  m3C(); virtual int  m3D(); virtual int  m3E(); virtual int  m3F();
    virtual int  m40(); virtual int  m41(); virtual int  m42(); virtual int  m43();
    virtual int  m44(); virtual int  m45(); virtual int  m46(); virtual int  m47();
    virtual int  m48(); virtual int  m49(); virtual int  m4A(); virtual int  m4B();
    virtual int  m4C(); virtual int  m4D(); virtual int  m4E(); virtual int  m4F();
    virtual int  m50(); virtual int  m51(); virtual int  m52(); virtual int  m53();
    virtual int  m54(); virtual int  m55(); virtual int  m56(); virtual int  m57();
    virtual int  m58(); virtual int  m59(); virtual int  m5A(); virtual int  m5B();
    virtual int  m5C(); virtual int  m5D(); virtual int  m5E(); virtual int  m5F();
    virtual int  m60(); virtual int  m61(); virtual int  m62(); virtual int  m63();
    virtual int  m64(); virtual int  m65(); virtual int  m66(); virtual int  m67();
    virtual int  m68(); virtual int  m69(); virtual int  m6A(); virtual int  m6B();
    virtual int  m6C(); virtual int  m6D(); virtual int  m6E(); virtual int  m6F();
    virtual int  m70(); virtual int  m71(); virtual int  m72(); virtual int  m73();
    virtual int  m74();                              // +0x74
    virtual int  m75(); virtual int  m76(); virtual int  m77(); virtual int  m78();
    virtual int  m79(); virtual int  m7A(); virtual int  m7B(); virtual int  m7C();
    virtual int  m7D(); virtual int  m7E(); virtual int  m7F(); virtual int  m80();
    virtual int  m81(); virtual int  m82(int arg);    // +0x148 (index 0x52)
};

// Virtual dispatch on the built model object (sub->field_00): method +0x48
// (index 18, one float) and +0x9c (index 39, two ints).
struct CMCModelVt {
    virtual void m00(); virtual void m01(); virtual void m02(); virtual void m03();
    virtual void m04(); virtual void m05(); virtual void m06(); virtual void m07();
    virtual void m08(); virtual void m09(); virtual void m0A(); virtual void m0B();
    virtual void m0C(); virtual void m0D(); virtual void m0E(); virtual void m0F();
    virtual void m10(); virtual void m11();
    virtual void m12(float f1);                  // +0x48
    virtual void m13(); virtual void m14(); virtual void m15(); virtual void m16();
    virtual void m17(); virtual void m18(); virtual void m19(); virtual void m1A();
    virtual void m1B(); virtual void m1C(); virtual void m1D(); virtual void m1E();
    virtual void m1F(); virtual void m20(); virtual void m21(); virtual void m22();
    virtual void m23(); virtual void m24(); virtual void m25(); virtual void m26();
    virtual void m27(int a, int b);              // +0x9c
};

// Virtual dispatch at +0x8 on the per-crystal param object (returns a ptr).
struct CMCCryParamObjVt {
    virtual void m00();
    virtual void* m01();
    virtual void* m02();   // +0x8
};

// 8-byte holder around a CfObjEnumList* (func_80043D90 / __dt__80043E88).
struct CMCryListHolder {
    void* list;   // 0x0
    u32 handle;   // 0x4
};

// Crystal-charged actor container: CfObjectMove embedded at +0x3e9c.
struct CMCCryChgActor {
    u8 _00[0x3e9c];
    u8 move[0x3f28 - 0x3e9c]; // embedded CfObjectMove
    u16 field_3f28;            // +0x3f28
    u32 field_3f2c;            // +0x3f2c
};

// Per-crystal param slot (stride 0x3c); +0x2c holds a vtable-slot-2 object.
struct CMCCryParamSlot {
    u8 _00[0x2c];
    void* field_2c;            // +0x2c
    u8 _30[0x3c - 0x30];
};

// Crystal display sub-object (per 0x5cc slot, embedded at +0x44).
struct CMCrystalDispSub {
    void* field_00;            // +0x00 built model pointer
    void* field_04;            // +0x04
    CMCryAnimVt mAnim;         // +0x08 inline anim object (flush at +0xE0)
    u32 field_14;              // +0x14 flags
    u8 _18[0x544 - 0x18];
    u32 mCrystalVals[6];       // +0x544
    u8 _55c[0x5a0 - 0x55c];
    u32 field_5a0;             // +0x5a0 file handle
    u32 field_5a4;             // +0x5a4
    u8 _5a8[0x5ac - 0x5a8];
    s8 field_5ac;              // +0x5ac state
    u8 _5ad[0x5b0 - 0x5ad];
    u32 field_5b0;             // +0x5b0 pos group 1
    u32 field_5b4;             // +0x5b4
    u32 field_5b8;             // +0x5b8
    u32 field_5bc;             // +0x5bc pos group 2
    u32 field_5c0;             // +0x5c0
    u32 field_5c4;             // +0x5c4
    u8 field_5c8;              // +0x5c8 built flag
};

// CModelDispMakeCrystal fields used by func_80220E14.
struct CMCCrySelfFields {
    u8 _00[0xc];
    void* field_0c;            // +0x0c allocator/parent
    u8 _10[0xbe0 - 0x10];
    f32 field_be0;             // +0xbe0
};

class CModelDispMakeCrystal {
public:
    CModelDispMakeCrystal();
    virtual ~CModelDispMakeCrystal();
    void OnFileEvent();

    void initCrystalSubStruct();
    u8 getCrystalStateA();
    u8 getCrystalStateB();
    u8 getCrystalStateC();
    void setCrystalPosEntry(unsigned short index, short a, short b);
    void setCrystalCursor(u32 val);
    void destroyCrystalDispThunk4();
    void releaseCrystalDispThunk();
    void destroyCrystalDispThunk8();

    // TODO: add fields
};

