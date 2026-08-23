#pragma once

#include <types.h>

class CEventFile;

namespace cf { class CfObjEnumList; }

namespace nw4r { namespace lyt { class DrawInfo; } }
class CMCEffStart;
// Retail free draw helper for CMCEffStart effects (defined in CMCEffStart.cpp;
// global C++ linkage mangles to draw__FP11CMCEffStartPQ34nw4r3lyt8DrawInfo).
void draw(CMCEffStart*, nw4r::lyt::DrawInfo*);

// Full object layout for CModelDispMakeCrystal (used by C-linkage accessors)
struct CModelDispMakeCrystalFull {
    u8 _00[0x5F0];
    u8 field_5F0;                 // +0x5F0 char state
    u8 _5F1[0xBBC - 0x5F1];
    u8 field_BBC;                 // +0xBBC
    u8 _BBD[0xBDC - 0xBBD];
    u8 field_BDC;                 // +0xBDC
    u8 field_BDD;                 // +0xBDD
    u8 _BDE[0xBE8 - 0xBDE];
    u8 field_BE8;                 // +0xBE8
    u8 _BE9[0x2DC3 - 0xBE9];
    u8 field_2DC3;                // +0x2DC3
    u8 _2DC4[0x2DCC - 0x2DC4];
    f32 field_2DCC;               // +0x2DCC
    u8 _2DD0[0x2DD1 - 0x2DD0];
    s8 field_2DD1;                // +0x2DD1
    u8 field_2DD2;                // +0x2DD2
};

// Inline anim object at sub+0x08 of CMCrystalDispSub; vtable flush call at
// +0xE0 (index 0x38), used by func_80220E14 to flush the buffered anim.
// MWCC reserves 2 hidden vtable slots (RTTI): 55 declared methods put the
// flush at declared index 54 => slot +0xE0.
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
    virtual void m34(); virtual void m35(); virtual void m36();  // => +0xE0
};

// Virtual dispatch on the crystal-charged actor's embedded CfObjectMove at
// +0x3e9c: method +0x74 (index 29) and +0x148 (index 82, one int arg).
// MWCC reserves 2 hidden vtable slots (RTTI): 81 declared methods place the
// first call at declared index 27 and the second at declared index 80.
struct CMCryMoveVt {
    virtual void m00(); virtual void m01(); virtual void m02(); virtual void m03();
    virtual void m04(); virtual void m05(); virtual void m06(); virtual void m07();
    virtual void m08(); virtual void m09(); virtual void m0A(); virtual void m0B();
    virtual void m0C(); virtual void m0D(); virtual void m0E(); virtual void m0F();
    virtual void m10(); virtual void m11(); virtual void m12(); virtual void m13();
    virtual void m14(); virtual void m15(); virtual void m16(); virtual void m17();
    virtual void m18(); virtual void m19(); virtual void m1A(); virtual void m1B();
    virtual void m1C(); virtual void m1D(); virtual void m1E(); virtual void m1F();
    virtual void m20(); virtual void m21(); virtual void m22(); virtual void m23();
    virtual void m24(); virtual void m25(); virtual void m26();
    virtual int  m74();                              // declared index 27 => +0x74
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
    virtual int  m82(int arg);                       // declared index 80 => +0x148
};

// Virtual dispatch on the built model object (sub->field_00): method +0x48
// (index 18, one float) and +0x9c (index 39, two ints). MWCC reserves 2
// hidden vtable slots (RTTI): declared indices 16 and 37.
struct CMCModelVt {
    virtual void m00(); virtual void m01(); virtual void m02(); virtual void m03();
    virtual void m04(); virtual void m05(); virtual void m06(); virtual void m07();
    virtual void m08(); virtual void m09(); virtual void m0A(); virtual void m0B();
    virtual void m0C(); virtual void m0D(); virtual void m0E(); virtual void m0F();
    virtual void m10(); virtual void m11();
    virtual void m12(float f1);                  // declared index 16 => +0x48
    virtual void m17(); virtual void m18(); virtual void m19(); virtual void m1A();
    virtual void m1B(); virtual void m1C(); virtual void m1D(); virtual void m1E();
    virtual void m1F(); virtual void m20(); virtual void m21(); virtual void m22();
    virtual void m23(); virtual void m24(); virtual void m25(); virtual void m26();
    virtual void m27(int a, int b);              // declared index 37 => +0x9c
};

// Virtual dispatch at +0x8 on the per-crystal param object (returns a ptr).
// MWCC reserves 2 hidden vtable slots (RTTI): declared index 0 => +0x8.
struct CMCCryParamObjVt {
    virtual void* m02();   // declared index 0 => +0x8
};

// 8-byte holder around a CfObjEnumList* (func_80043D90 / __dt__80043E88).
struct CMCryListHolder {
    cf::CfObjEnumList* list;   // 0x0
    u32 handle;   // 0x4
};

// Crystal-charged actor container: CfObjectMove embedded at +0x3e9c.
struct CMCCryChgActor {
    u8 _00[0x3e9c];
    u8 move[0x3f28 - 0x3e9c]; // embedded CfObjectMove
    u16 field_3f28;            // +0x3f28
    u32 field_3f2c;            // +0x3f2c
};

// 12-byte crystal sub-buffer (stride 0xc); 6 per display slot at +0x564.
// initCrystalSubStruct zeroes field_00/field_04/field_08.
struct CMCrySubBuf {
    u32 field_00;   // +0x00
    u32 field_04;   // +0x04
    u8  field_08;   // +0x08
    u8  _09[3];     // +0x09
};

// 12-byte vector holding the ctor's crystal-position groups (copied into the
// slot fields at +0x5b0/+0x5bc as raw words).
struct CMCryVec3 {
    f32 x, y, z;
};


// Per-crystal param slot (stride 0x3c); +0x2c holds a vtable-slot-2 object.
struct CMCCryParamSlot {
    u8 _00[0x2c];
    CMCCryParamObjVt* field_2c;            // +0x2c
    u8 _30[0x3c - 0x30];
};

// Crystal display sub-object (per 0x5cc slot, embedded at +0x44).
struct CMCrystalDispSub {
    void* field_00;            // +0x00 built model pointer (assigned from void* func -> kept void*)
    void* field_04;            // +0x04 (assigned from void* func -> kept void*)
    CMCryAnimVt mAnim;         // +0x08 inline CActParamAnimGame (vptr only here)
    u8 _0c[0x14 - 0x0c];       // +0x0c anim-object payload
    u32 field_14;              // +0x14 anim flags (CActParamAnimGame::field_0C)
    u8 _18[0x544 - 0x18];      // +0x18 rest of the anim object
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

// 8-byte param record written at each crystal slot entry's +2 (entries have
// an 8-byte stride, so the record overlaps 2 bytes into the next slot). Same
// layout as CMCCrystalBoxParam; copied by func_80213E20.
struct CMCrySlotParam {
    u16 m0;   // +0x00
    u16 m2;   // +0x02
    u16 m4;   // +0x04
    u8  m6;   // +0x06
    u8  m7;   // +0x07
};

// 8-byte crystal slot entry (stride 8, array at this+0). A CMCrySlotParam
// record overlaps starting at +2, so its last two bytes spill into the next
// slot. func_8021E888 scans entry.m2 for zero and stores the index in byte 0.
struct CMCrySlotEntry {
    u8 _00[0x2];   // +0..1 (byte 0 receives the found slot index)
    u16 m2;        // +0x02 first u16 of the overlapped param record
    u8 _04[0x8 - 0x4];
};

// CModelDispMakeCrystal fields used by func_80220E14.
struct CMCCrySelfFields {
    u8 _00[0xc];
    u8* field_0c;            // +0x0c allocator/parent
    u8 _10[0xbe0 - 0x10];
    f32 field_be0;             // +0xbe0
};

class CModelDispMakeCrystal {
public:
    CModelDispMakeCrystal();
    ~CModelDispMakeCrystal();
    int OnFileEvent(CEventFile* ev);

    void initCrystalSubStruct();
    u8 getCrystalStateA();
    u8 getCrystalStateB();
    u8 getCrystalStateC();
    void setCrystalPosEntry(unsigned short index, short a, short b);
    void setCrystalCursor(u32 val);
    void destroyCrystalDispThunk4();
    void releaseCrystalDispThunk();
    void destroyCrystalDispThunk8();

    // retail object layout (vptr at +0 covered by _00; member objects at
    // fixed offsets - gauge @0xbec, list @0xc18, scrollbar @0xe38 - are
    // accessed via casts in the TU)
    u8 _00[0x5F0];
    u8 field_5F0;                 // +0x5F0 char state
    u8 _5F1[0xBBC - 0x5F1];
    u8 field_BBC;                 // +0xBBC
    u8 _BBD[0xBDC - 0xBBD];
    u8 field_BDC;                 // +0xBDC
    u8 field_BDD;                 // +0xBDD
    u8 _BDE[0xBE8 - 0xBDE];
    u8 field_BE8;                 // +0xBE8
};

// C-linkage imports (retail symbol names - keep linkage/signatures verbatim)
extern "C" u32 lbl_eu_805090D8[]; // 3 state filter values for func_800F4A98
extern "C" u8 lbl_eu_80664718[0x8]; // makecrystal crystal-probability state (sbss, 8 bytes)
// Color table: rows of 3 bytes, indexed by character state - 1.
struct CMCryColorRow { u8 c[3]; };
extern "C" const CMCryColorRow lbl_eu_80535D90[];
// Retail hard symbols with C linkage (defined in this TU's retail .o).
extern "C" void initCrystalSubStruct(void* self);
extern "C" void __ct__Q22cf17CActParamAnimGameFv(void* p);
extern "C" void __dt__Q22cf17CActParamAnimGameFv(void* obj, int flag);

// Plain-C-linkage functions defined in this TU (retail symbol = plain name).
extern "C" void func_8021FC28(CModelDispMakeCrystal*, u8);
extern "C" void func_8021FD44(CModelDispMakeCrystal*);
extern "C" void func_802200A8(void*);
extern "C" void func_80220954(void*, int, u8);
extern "C" int func_8004392C(u32, u32, void*, u32, u32, f32);

// C-linkage helpers for the cylinder-count finalize (func_8021E5C0):
// item-step-list setup and scrollbar refresh.
extern "C" void func_80158300(void* item, int mode);
extern "C" void func_801570A0(void* item, u8 flag);
extern "C" void func_8015704C(void* dst, const void* src);
extern "C" void* CItem_initItemImplInstances(void* item);
extern "C" void func_801F3850(void* scrollbar, u16 index);
extern "C" void func_8021E840(void* entries, u16 idx);
extern "C" void func_8021E888(void* entries);

// C-linkage imports for the constructor (retail bare-name symbols - the
// member ctors in the sub-object headers would mangle differently).
extern "C" void __ct__17UnkClass_8045F564Fv(void* self);
extern "C" void __construct_array(void*, void*, void*, u32, u32);
extern "C" void __ct__CMCCrystalList(void* self);
extern "C" void __ct__CScrollBar(void* self, int arg);
extern "C" void __ct__CSysWin(void* self, int arg);
extern "C" void __ct__CMCGetItemBox(void* self);
extern "C" void __ct__CCur18(void* self, void* arg);
extern "C" void __ct__UnkClass_8011C974(void* dest, void* src);
extern "C" void func_8016742C(void* dest, void* src);
extern "C" size_t strlen(const char* s);
extern "C" char* strcpy(char* dst, const char* src);
extern "C" int func_80167A18();   // item-menu active gate (CMainMenu.cpp)

// Constructor data imports.
extern u8 lbl_eu_80535E70[];   // CModelDispMakeCrystal vtable (+0x88/+0xb4 sub-vtables)
extern const f32 lbl_eu_806684A8;
extern const f32 lbl_eu_806684B0;
extern const f32 lbl_eu_806684B4;
extern const f32 lbl_eu_806684B8;
extern const f32 lbl_eu_806684BC;
extern const f32 lbl_eu_806684C0;
extern const f32 lbl_eu_8066A210;

