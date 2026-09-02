#pragma once

#include <types.h>

extern const float lbl_eu_80665F7C;  // default float (sdata2)
extern const float lbl_eu_80665F78;  // default float (sdata2)
extern const float lbl_eu_80665F80;  // default float (sdata2)
extern const float lbl_eu_80665F84;  // default float (sdata2)
extern const float lbl_eu_80665F88;  // default float (sdata2)
extern const double lbl_eu_80665F90; // sdata2: 2^52 (int->float magic)
extern const double lbl_eu_80665F98; // sdata2: 2^52 (int->float magic, func_80055F94)

// External data referenced by sub-object pointers at +0x74.
// Both getShortValue_A and getShortValue_B read from this.
struct CActParamDataRef {
    u8 _pad_00[0x08];  // 0x00-0x07
    s16 mShort08;       // 0x08: read in getShortValue_A
    s16 mShort0A;       // 0x0A: read in getShortValue_B
    s16 mShort0C;       // 0x0C: returned by func_80055F94
};

// Pointer-offset return for getSubObjPtr (val + 0x14).
struct CActParamDataSub {
    u8 _pad_00[0x14];  // 0x00-0x13
    void* mPtr14;       // 0x14: returned by getSubObjPtr
};

class __declspec(novtable) CActParamData {
public:
    static void buildVTable();
    static void func_80057CDC();
    CActParamData();
    virtual ~CActParamData();

    // +0x00: vtable
    void* mEntry04;                 // 0x04: float-source entry (threshold at +0x388)
    void* mNode08;                  // 0x08
    void* mNode0C;                  // 0x0C: linked node chain head
    void* mObj10;                   // 0x10: callback interface object (vt+0x1C/0x80/0xC4)
    float mFloat14;                 // 0x14
    void* mPtr18;                   // 0x18: optional entry pointer (float getters)
    u32 mField1C;                   // 0x1C
    u32 mField20;                   // 0x20
    u32 mField24;                   // 0x24
    u32 mField28;                   // 0x28
    u32 mField2C;                   // 0x2C
    u32 mWords30[(0x1D0 - 0x30) / 4];  // 0x30-0x1CF: inline record area
    void* mKeys1D0[8];              // 0x1D0: node keys
    void* mNodes1F0[8];             // 0x1F0: node per key
    u32 mCount210;                  // 0x210: number of pushed entries
    u8 mByte214;                    // 0x214
    u8 _pad215;                     // 0x215
    u8 mByte216;                    // 0x216
    u8 _pad217;
    u16 mShorts218[32];             // 0x218-0x257: reset to 0xFFFF by func_80053A90
    u32 mField258;                  // 0x258
    u32 mField25C;                  // 0x25C
    // Sub-object A at 0x260 (0x80 bytes to 0x2DF)
    u32 mA260;
    u32 mA264;
    u32 mA268;
    u32 mA26C;
    u32 mA270;
    u32 mField274;                  // 0x274: flag (getNonNullPtr)
    u32 mA278;
    u32 mA27C;
    u32 mA280;
    float mFloat284;                // 0x284
    float mA288;                    // 0x288
    float mFloat28C;                // 0x28C
    u8 _pad290;                     // 0x290
    s8 mByte291;                    // 0x291 (signed)
    u8 _pad292[0x294 - 0x292];
    u32 mA294;                      // 0x294
    u32 mA298;                      // 0x298
    char* mStr29C;                  // 0x29C: name string compared by func_80053DE8
    float mA2A0;                    // 0x2A0
    float mA2A4;                    // 0x2A4
    u32 mA2A8;                      // 0x2A8
    u32 mA2AC;                      // 0x2AC
    float mA2B0;                    // 0x2B0: previous-value shadow of +0x2B4
    float mFloat2B4;                // 0x2B4: float saved by func_80055EE4
    u32 mA2B8;                      // 0x2B8
    u32 mA2BC;                      // 0x2BC
    u32 mA2C0;                      // 0x2C0
    u8 mA2C4;                       // 0x2C4
    u8 _pad2C5[0x2C8 - 0x2C5];
    float mA2C8;                    // 0x2C8: accumulated float
    u32 mField2CC;                  // 0x2CC: flag cleared by func_80055F08
    float mFloat2D0;                // 0x2D0: float cleared by func_80055F08
    u32 mField2D4;                  // 0x2D4 (tag/ptr for SubObjA)
    void* mPtr2D8;                  // 0x2D8 (getNonNullPtr)
    s16 mShort2DC;                  // 0x2DC: reset to -1 by func_80053B24
    // Sub-object B at 0x2E0 (reset block walked/cleared by func_80055DF0)
    u32 mField2E0;                  // 0x2E0
    u32 mField2E4;                  // 0x2E4
    u32 mField2E8;                  // 0x2E8
    u32 mField2EC;                  // 0x2EC
    u32 mField2F0;                  // 0x2F0: saved tag word
    void* mPtr2F4;                  // 0x2F4 (tag/ptr, getSubObjPtr)
    u32 mField2F8;                  // 0x2F8: fallback flag checked by func_80055EBC
    u32 mField2FC;                  // 0x2FC
    u32 mField300;                  // 0x300
    float mFloat304;                // 0x304
    float mFloat308;                // 0x308
    float mFloat30C;                // 0x30C
    u8 mByte310;                    // 0x310
    s8 mByte311;                    // 0x311 (signed)
    u8 _pad312[0x314 - 0x312];
    u32 mField314;                  // 0x314
    u32 mField318;                  // 0x318
    u32 mField31C;                  // 0x31C
    float mFloat320;                // 0x320
    float mFloat324;                // 0x324
    u32 mField328;                  // 0x328
    u32 mField32C;                  // 0x32C
    float mFloat330;                // 0x330
    float mFloat334;                // 0x334: float saved by func_80055EE4
    u32 mField338;                  // 0x338
    u32 mField33C;                  // 0x33C
    u32 mField340;                  // 0x340
    u8 mByte344;                    // 0x344
    u8 _pad345[0x348 - 0x345];
    float mFloat348;                // 0x348
    u32 mField34C;                  // 0x34C: flag cleared by func_80055F08
    float mFloat350;                // 0x350: float cleared by func_80055F08
    u32 mField354;                  // 0x354 (tag/ptr for SubObjB)
    void* mPtr358;                  // 0x358 (getNonNullPtr)
    s16 mShort35C;                  // 0x35C: reset to -1 by func_80055DF0

    u8* getResetBlock2E0() { return (u8*)&mField2E0; }
};

// Destination blocks for the flag-gated setters.
struct CActParamSet25C { u8 _pad00[0x25C]; u32 field_0x25C; };
struct CActParamSet18  { u8 _pad00[0x18]; u32 field_0x18; };
struct CActParamSet74  { u8 _pad00[0x74]; u32 field_0x74; };
struct CActParamSet78  { u8 _pad00[0x78]; u32 field_0x78; };
// Float copy pair for func_80056808: read src+0x08, write dst+0x14.
struct CActParamFloatSrc08 { u8 _pad00[0x08]; float mFloat08; };
struct CActParamFloatDst14 { u8 _pad00[0x14]; float mFloat14; };

// Flag-gated copy helpers (func_80056888..func_80057244).
// Word/byte/float source blocks read by each setter.
struct CActParamWordSrc20 { u8 _pad00[0x20]; u32 mWord20; };
struct CActParamWordSrc08 { u8 _pad00[0x08]; u32 mWord08; };
struct CActParamByteSrc08 { u8 _pad00[0x08]; u8 mByte08; };
struct CActParamFloatDst24 { u8 _pad00[0x24]; float mFloat24; };
struct CActParamSet20  { u8 _pad00[0x20]; u32 field_0x20; };
struct CActParamSet1C  { u8 _pad00[0x1C]; u32 field_0x1C; };
struct CActParamSet4C  { u8 _pad00[0x4C]; u32 field_0x4C; };

int func_80056CE4(u32 flags, CActParamSet25C* dst, void* unused, u32 val);
int func_80056EAC(u32 flags, CActParamSet18* dst, void* unused, u32 val);
int func_80057264(u32 flags, void* unused, CActParamSet74* dst, u32 val);
int func_80057654(u32 flags, void* unused, CActParamSet78* dst, u32 val);
int func_80056808(u32 flags, CActParamFloatDst14* dst, void* unused, const CActParamFloatSrc08* src);

// Destination block for func_80057470: s16 stored at +0x7C.
struct CActParamSet7C { u8 _pad00[0x7C]; s16 field_0x7C; };

// Float/byte copy pair for func_80056C54: read src+0x08 float and src+0x0C byte,
// write dst+0x28 float and dst+0x30 byte.
struct CActParamCopyDst2830 {
    u8 _pad00[0x28];
    float mFloat28;       // 0x28
    u8 _pad2C[0x30 - 0x2C];
    u8 mByte30;           // 0x30
};
struct CActParamCopySrc080C {
    u8 _pad00[0x08];
    float mFloat08;       // 0x08
    u8 mByte0C;           // 0x0C
};

// Global table referenced by func_80056730 (entry-count guard at +0x5C).
struct ActParamGlobalTable {
    u8 _pad00[0x5C];
    u32 mCount5C;         // 0x5C
    u32 mCount60;         // 0x60: entry-count guard used by func_80056760
};
// Node in the linked chain at CActParamData+0x0C.
struct ActParamNode94 {
    u8 _pad00[0x08];
    ActParamNode94* mPrev08;       // 0x08
    ActParamNode94* mNext0C;       // 0x0C: next link
    u32 mType10;                   // 0x10: only type 3 is processed
    u32 mCount14;                  // 0x14: repeat counter
    u8 _pad18[0x20 - 0x18];
    u8 mBytes20;                   // 0x20: payload passed to func_80055960
};

// Context record walked by func_80054A94.
struct ActParamCtx94 {
    u8 _pad00[0x14];
    void* mRec14;                  // 0x14: record-chain head
    u8 _pad18[0x1C - 0x18];
    u32 mField1C;                  // 0x1C: first gate value
    u32 mField20;                  // 0x20: fallback gate value
    u8 _pad24[0x74 - 0x24];
    u32 mField74;                  // 0x74: final gate value
};

// Callback interface stored at CActParamData+0x10.
struct ActParamObjView {
    virtual void dummy00(); virtual void dummy04(); virtual void dummy08(); virtual void dummy0C();
    virtual void dummy10(); virtual void dummy14(); virtual void dummy18();
    virtual int invoke1C(int sel, u32 val);   // vt+0x1C (sel: ctx-is-own-block flag)
    virtual void dummy20(); virtual void dummy24(); virtual void dummy28(); virtual void dummy2C();
    virtual void dummy30(); virtual void dummy34(); virtual void dummy38(); virtual void dummy3C();
    virtual void dummy40(); virtual void dummy44(); virtual void dummy48(); virtual void dummy4C();
    virtual void dummy50(); virtual void dummy54(); virtual void dummy58(); virtual void dummy5C();
    virtual void dummy60(); virtual void dummy64(); virtual void dummy68(); virtual void dummy6C();
    virtual void dummy70(); virtual void dummy74();
    virtual int check80(int sel);    // vt+0x80
    virtual void dummy084(); virtual void dummy088(); virtual void dummy08C();
    virtual void dummy090(); virtual void dummy094(); virtual void dummy098(); virtual void dummy09C();
    virtual void dummy0A0(); virtual void dummy0A4(); virtual void dummy0A8(); virtual void dummy0AC();
    virtual void dummy0B0(); virtual void dummy0B4(); virtual void dummy0B8(); virtual void dummy0BC();
    virtual void dummy0C0();
    virtual int invokeC4();          // vt+0xC4
};

// Retail EU-side helper (unmangled symbol).
extern "C" void func_eu_80053FDC();
extern "C" int func_80054438(CActParamData* self, void* data, int sel, u32 gate);
extern "C" void func_80053F7C(CActParamData* self, float arg1, float arg2);
extern "C" int func_80054A94(CActParamData* self, void* ctx);
extern "C" int func_80055B88(CActParamData* self, void* key, u32 tag);
extern "C" void func_80053B24(CActParamData* self, void* entry, void* obj, void* nodeA, void* nodeB);

// Slot table addressed relative to lbl_eu_80663D50, indexed directly by a
// record's base offset (byte-granular indexing, hence the pointer-view decl).
extern ActParamGlobalTable* lbl_eu_80663D50;

void* getNonNullPtr(void* param);
int func_80055EBC(CActParamData* self);
void func_80055EE4(CActParamData* self);
int func_80056730(struct ActParamCallView* self, void* unused, u32 index);
int func_80056C54(u32 flags, void* unused, CActParamCopyDst2830* dst, const CActParamCopySrc080C* src);
int func_80057470(u32 flags, void* unused, CActParamSet7C* dst, const CActParamDataRef* src);

// Interface whose virtual at vt+0xBC is tail-called by func_80056730.
struct ActParamCallView {
    virtual void dummy008(); virtual void dummy00C(); virtual void dummy010(); virtual void dummy014();
    virtual void dummy018(); virtual void dummy01C(); virtual void dummy020(); virtual void dummy024();
    virtual void dummy028(); virtual void dummy02C(); virtual void dummy030(); virtual void dummy034();
    virtual void dummy038(); virtual void dummy03C(); virtual void dummy040(); virtual void dummy044();
    virtual void dummy048(); virtual void dummy04C(); virtual void dummy050(); virtual void dummy054();
    virtual void dummy058(); virtual void dummy05C(); virtual void dummy060(); virtual void dummy064();
    virtual void dummy068(); virtual void dummy06C(); virtual void dummy070(); virtual void dummy074();
    virtual void dummy078(); virtual void dummy07C(); virtual void dummy080(); virtual void dummy084();
    virtual void dummy088(); virtual void dummy08C(); virtual void dummy090(); virtual void dummy094();
    virtual void dummy098(); virtual void dummy09C(); virtual void dummy0A0(); virtual void dummy0A4();
    virtual void dummy0A8(); virtual void dummy0AC(); virtual void dummy0B0(); virtual void dummy0B4();
    virtual void dummy0B8();
    virtual int findEntryByIndex(void* src, u32 index); // vt+0xBC
    virtual int findEntryByIndexWide(void* src, u32 index); // vt+0xC0
};

// Flag-gated copies: when bit 2 of `flags` is set, copy src field into dst;
// always return 0.
int func_80056888(u32 flags, void* unused, CActParamSet20* dst, const CActParamWordSrc20* src);
int func_800568A8(u32 flags, void* unused, CActParamFloatDst24* dst, const CActParamFloatSrc08* src);
int func_80056C34(u32 flags, void* unused, CActParamSet4C* dst, const CActParamByteSrc08* src);
int func_80057244(u32 flags, CActParamSet20* dst, void* unused, const CActParamWordSrc08* src);

// Entry referenced by mPtr18: floats read by func_80055DB8 / func_80055DD4.
struct CActParamDataEntry {
    u8 _pad_00[0x08];   // 0x00-0x07
    float mFloat08;     // 0x08
    float mFloat0C;     // 0x0C
};

// Target block whose +0x48 word is written by func_80056CC8.
struct CActParamBlock {
    u8 _pad_00[0x48];   // 0x00-0x47
    u32 mField48;       // 0x48
};

int func_80056CC8(u32 flags, void* unused, CActParamBlock* dst, u32 val);

// Inline byte-entry table: entries start at +0x28; the count word at +0x2C
// overlaps the tail of the inline area (retail layout, hence reloads).
struct CActParamByteList {
    u8 _pad00[0x28];
    union {
        u8 mEntries[0x10];
        struct {
            u8 _pad28[4];
            s32 mCount2C;
        };
    };
};

// Linked node chain walked by func_80056828.
struct CActParamNodePayload { u8 _pad00[4]; };
struct CActParamNode {
    u16 mOffset0;                  // 0x00: stride to the next node
    u16 mType2;                    // 0x02: 0=payload, 1=terminator, other=skip
    u8 _pad04[0x14 - 0x04];
    CActParamNodePayload mPayload; // 0x14
};
struct CActParamNodeTable {
    u8 _pad00[0x30];
    CActParamNode* mNodes[0x10];   // indexed by the selector from src+0x08
};
struct CActParamSet1C38 {
    u8 _pad00[0x1C];
    u32 mField1C;                  // 0x1C: receives the selector
    u8 _pad20[0x38 - 0x20];
    u8* mPayload38;                // 0x38: receives &type-0 node payload
};

// Slot table + record stream walked by func_8005577C.
struct CActParamLinkTable {
    u8 _pad00[8];
    u32 mGate08;                   // 0x08: enables the name-list search
    u8 _pad0C[0x30 - 0x0C];
    u32 mSlots[0x10];              // owned-record words, indexed by selector
};
struct CActParamRec {
    u16 mOffset0;                  // 0x00: stride to the next record
    u16 mType2;                    // 0x02: 0=linkable, 1=stop, 2=yields slot
    u8 _pad04[4];
    u32 mSel8;                     // 0x08: slot selector
    u8 _pad0C[0x10 - 0x0C];
    u32 mField10;                  // 0x10: set to -1 by func_80055700
};
struct CActParamRecStream {
    u8 _pad00[0x14];
    u32 mFlag14;                   // 0x14: gate checked by func_80055700
    u8 _pad18[0x20 - 0x18];
    CActParamRec mFirst;
};

// Name-tagged record chain walked by func_80055960 / func_800557E8.
struct ActParamStrRec {
    u16 mOffset0;                  // 0x00: stride to the next record
    u16 mType2;                    // 0x02: 0=end, 1=stop, 2=named, 6/0x29=indexed
    u8 _pad04[4];
    char mName08[4];               // 0x08: NUL-terminated name (may run past 4)
    char mName0C[4];               // 0x0C: secondary name
    u8 _pad10[0x20 - 0x10];
    u32 mField20;                  // 0x20: receives the index for type-6 records
    u32 mField24;                  // 0x24: receives the index for type-0x29 records
};
extern "C" int func_80055960(CActParamLinkTable* table, ActParamStrRec* list, ActParamStrRec* rec);
int func_80056760(ActParamCallView* self, void* unused, u32 index);
bool func_80053F40(CActParamByteList* list, u8 value);
int func_80056828(u32 flags, CActParamNodeTable* table, CActParamSet1C38* dst, const CActParamWordSrc08* src);
void func_8005577C(CActParamLinkTable* table, CActParamRecStream* stream);
int func_80057828(u32 flags, CActParamByteList* list, u8* unused, const CActParamByteSrc08* src);

// ---- func_80057BA0 ----
struct ActParamT19ArgA { u8 _pad00[0x10]; u32 mField10; };
struct ActParamT19ArgB { u8 _pad00[0x54]; float mFloat54; };
struct ActParamT19ArgC {
    u8 _pad00[8];
    u32 mField08;                  // packed lo/hi halfwords
    u8 _pad0C[0x24 - 0x0C];
    u32 mField24;
    u16 mShort28;
    u16 mShort2A;
};
typedef int (*ActParamTbl19Fn)(u32, u32, u32);
extern const ActParamTbl19Fn lbl_eu_805705F0[];
// Handlers referenced by the dispatch tables but not yet decompiled.
int func_8005609C(u32, u32, u32);
int func_800567F0(u32, void*, void*, void*);
int func_800567F8(u32, void*, void*, void*);
int func_80056800(u32, void*, void*, void*);
// vtable-thunk handlers referenced by lbl_eu_805705F0 but not yet decompiled.
extern "C" int func_80056290(void*, u32);
extern "C" int func_800562A4(void*, u32);
extern "C" int func_800562B8(void*, u32);
extern "C" int func_800562E0(void*, u32);
extern "C" int func_800562F4(void*, u32);
extern "C" int func_80056308(void*, u32);
extern "C" int func_8005631C(void*, u32);
extern "C" int func_80056330(void*, u32);
extern "C" int func_80056344(void*, u32);
extern "C" int func_80056358(void*, u32);
extern "C" int func_8005636C(void*, u32);
extern "C" int func_80056380(void*, u32);
extern "C" int func_80056394(void*, u32);
extern "C" int func_800563A8(void*, u32);
extern "C" int func_800563D0(void*, u32);
extern "C" int func_800563E4(void*, u32);
extern "C" int func_800563F8(void*, u32);
extern "C" int func_8005640C(void*, u32);
extern "C" int func_80056420(void*, u32);
extern "C" int func_80056434(void*, u32);
extern "C" int func_80056448(void*, u32);
extern "C" int func_8005645C(void*, u32);
extern "C" int func_80056470(void*, u32);
extern "C" int func_80056484(void*, u32);
extern "C" int func_80056498(void*, u32);
extern "C" int func_800564C0(void*, u32);
int func_80057BA0(u32 flags, ActParamT19ArgA* a, ActParamT19ArgB* b, ActParamT19ArgC* c);

// ---- func_80053DE8 ----
float func_80053DE8(CActParamData* self, int sel);

// ---- func_80056A98 ----
// Object at host+0x04: carries the vt+0x14 callback and a threshold float at 0x388.
// First user virtual sits at vtable+0x08 (RTTI words at 0x00/0x04).
struct ActParamObj5 {
    virtual void dummy00(); virtual void dummy04(); virtual void dummy08();
    virtual double notify14();     // vt+0x14; returns the (double) time so
                                   // callers keep it live in f1 across the call
};
// Float-returning view of the same object: func_80057670 keeps the time in a
// single-precision float local (retail fsubs conversion) riding in f1.
struct ActParamObj5f {
    virtual void dummy00(); virtual void dummy04(); virtual void dummy08();
    virtual float notify14();      // vt+0x14
};
struct ActParamData388 {
    u8 _pad00[0x388];
    volatile float mFloat388;      // 0x388 threshold compared against F80
                                   // volatile: retail reloads it per test site
};
struct ActParamHost5 {
    u8 _pad00[4];
    void* mObj04;                  // 0x04: ActParamObj5 / ActParamData388 view
    u8 _pad08[0x24 - 0x08];
    struct ActParamCbView5* mCb24;     // 0x24
};
struct ActParamCbView5 {
    virtual void dummy00(); virtual void dummy04();
    virtual void invoke10(void* obj); // vt+0x10
};
struct ActParamVals5 {
    u32 mField00;                  // 0x00: flag mask updated on fire (func_80057084)
    u8 _pad04[8];
    u32 mField0C;                  // 0x0C: forwarded by func_80057490
    u32 _pad10;
    u32 mField14;                  // 0x14: gate checked by func_80057670
    u8 _pad18[0x50 - 0x18];
    float mFloat50;                // 0x50
    float mFloat54;                // 0x54
    s32 mField58;                  // 0x58: signed - retail compares with cmpi
};
struct ActParamSrc5 {
    u8 _pad00[8];
    union {
        u16 mShort08;                  // zero-extended and converted to float
        volatile u16 mShort08v;        // volatile view: blocks cross-call CSE
        struct {                       // byte view used by func_800568E8
            u8 _pad08[2];
            u8 mByte0A;
            u8 mByte0B;
        };
    };
    u32 mWord0C;                   // 0x0C: mask applied to vals+0x00 on fire
};
int func_80056A98(u32 flags, ActParamHost5* host, ActParamVals5* vals, ActParamSrc5* src);

// ---- func_80054980 walker ----
// Record with a halfword selector at 0x08 and bytes at 0x0A/0x0B (type 9).
struct CActParamWalkRec {
    u16 mOffset0;                  // 0x00: stride to next record
    u16 mType2;                    // 0x02
    u8 _pad04[4];
    u16 mSel08;                    // 0x08 (halfword)
    u8 mByte0A;                    // 0x0A
    u8 mByte0B;                    // 0x0B
};
// Interface whose second user virtual (vt+0x0C) receives the walk callback.
struct ActParamWalkView {
    virtual void vf08(u32 a, u8 b, u8 c);
    virtual void vf0C(u32 a, u8 b, u8 c);
};
// Type-punned view used only so the null-check load cannot be merged with
// the pointer reload below (retail loads offset 0x278 twice).
struct ActParamWalkHostRaw {
    u8 _pad00[0x278];
    u32 mRaw278;
};
struct ActParamWalkHost {
    u8 _pad00[0x04];
    u32 mField04;                  // 0x04: first callback arg
    u8 _pad08[0x1C];
    ActParamWalkView* mIf24;         // 0x24: callback receiver
    u8 _pad28[0x250];
    CActParamWalkRec* volatile mRec278; // 0x278: record chain head (volatile: retail re-reads per access)
};

// ---- func_80055AC4 stack push ----
struct ActParamStackNode {
    u8 _pad00[0x08];
    ActParamStackNode* mPtr08;     // 0x08
    ActParamStackNode* mPtr0C;     // 0x0C: next link
    u32 mMode10;                   // 0x10: registration mode (3 enables func_800555EC)
    u32 mWord14;                   // 0x14: mode word (<=1 enables splicing)
    ActParamStackNode* mSelf18;    // 0x18: back self-pointer when registered
    u8 _pad1C[4];
    u8 mBytes20;                   // 0x20: payload passed to func_800557E8
};
struct ActParamStack {
    u8 _pad00[8];
    u32 mGate08;                   // 0x08: enables the name-list search
    ActParamStackNode* mNode0C;    // 0x0C: list head
    u8 _pad10[0x1C0];
    ActParamStackNode* mSlots1D0[8]; // 0x1D0
    ActParamStackNode* mSlots1F0[8]; // 0x1F0
    u32 mCount210;                 // 0x210: number of pushed entries
};
// Record walked by func_800555EC: linkable entries claim table slots.
struct ActParamRegRec {
    u16 mOffset0;                  // 0x00: stride to the next record
    u16 mType2;                    // 0x02: 0=linkable, 1=stop
    u8 _pad04[4];
    u32 mSel8;                     // 0x08: slot selector (word, compared signed)
    u8 _pad0C[0x10 - 0x0C];
    u32 mField10;                  // 0x10: receives the anim lookup index
    s8 mByte14;                    // 0x14: name tag; nonzero triggers lookup
};
class CScnItemAnim;
// Name-lookup import (defined in libs/monolib/src/scn/CScnItemAnim.cpp);
// retail references it under an unmangled symbol.
extern "C" int func_8049E648(CScnItemAnim* self, const char* name);
// Register a stream's record chain into the table's slots; returns 0.
extern "C" __declspec(noinline) int func_800555EC(CActParamLinkTable* table, CScnItemAnim* anim, ActParamStackNode* node);
extern "C" int func_800557E8(ActParamStack* self, ActParamStrRec* dst, ActParamStrRec* src);
float func_80055DB8(CActParamData* self);  // mPtr18 entry float getter
float func_80055DD4(CActParamData* self);
void func_80055DF0(CActParamData* self);
void func_80053A90(CActParamData* self);
void func_80054980(ActParamWalkHost* host);

// ---- func_80056D00 / func_80057280 / func_800568E8 / func_80057670 ----
// Variants of func_80056A98 differing only in the callback invoked at the end.
struct ActParamCbView0C {
    virtual void dummy0();
    virtual void invoke0C(void* obj, u8 a, u8 b); // vt+0x0C
};
struct ActParamCbView14 {
    virtual void dummy0(); virtual void dummy1(); virtual void dummy2();
    virtual void invoke14(void* obj, void* src); // vt+0x14
};
struct ActParamCbView1C {
    virtual void dummy0(); virtual void dummy1(); virtual void dummy2(); virtual void dummy3(); virtual void dummy4();
    virtual void invoke1C(void* obj, void* src); // vt+0x1C
};
struct ActParamCbView20 {
    virtual void dummy0(); virtual void dummy1(); virtual void dummy2(); virtual void dummy3();
    virtual void dummy4(); virtual void dummy5();
    virtual void invoke20(void* obj, void* src); // vt+0x20
};
// Callback interface used by func_80056EC8: fires the vt+0x18 slot with
// the data object and the source record.
struct ActParamCbView18 {
    virtual void dummy00(); virtual void dummy04(); virtual void dummy08();
    virtual void dummy0C();
    virtual void invoke18(void* obj, void* src); // vt+0x18
};
// Callback interface used by func_80057490
// data object, the source mask word, two vals words, and the func_80053960
// tick value.
struct ActParamCbView28 {
    virtual void dummy00(); virtual void dummy04(); virtual void dummy08();
    virtual void dummy0C(); virtual void dummy10(); virtual void dummy14();
    virtual void dummy18(); virtual void dummy1C();
    virtual void invoke28(ActParamData388* obj, u32 mask, u32 flag14,
                          u32 flag0C, u32 tick); // vt+0x28
};
// Import from kyoshin/action/CActParamAnim (asm-side): per-entry tick getter.
extern "C" u32 func_80053960(ActParamData388* obj);

// View of the data object extended to the callback pointer at 0x3a0
// (used by func_8005789C).
struct ActParamData3A0 {
    u8 _pad00[0x388];
    volatile float mFloat388;
    u8 _pad38C[0x3A0 - 0x38C];
    void* mCb3A0;                  // 0x3a0: callback object checked/called on fire
};
// Callback interface for func_8005789C: fires the vt+0x28 slot with a pointer
// to the src mask word (src+0x0C) and the normalized src byte flag.
struct ActParamCbView28Time {
    virtual void dummy00(); virtual void dummy04(); virtual void dummy08();
    virtual void dummy0C(); virtual void dummy10(); virtual void dummy14();
    virtual void dummy18(); virtual void dummy1C(); virtual void dummy20();
    virtual void dummy24();
    virtual void invoke28(u32* wordPtr, u32 flag); // vt+0x28
};

// func_8005789C: like func_80056EC8 but gated on vals+0x14 and firing invokes
// the cb object held at obj04+0x3a0 (vt+0x28) with the time word pointer.
int func_8005789C(u32 flags, ActParamHost5* host, ActParamVals5* vals,
                  ActParamSrc5* src);
// func_80056EC8: like func_80056A98 but the range test clears the fire flag
// instead of setting it, and firing invokes the cb object's vt+0x18.
int func_80056EC8(u32 flags, ActParamHost5* host, ActParamVals5* vals, ActParamSrc5* src);

// func_80057084: like func_80056EC8 but firing applies src+0x0C as a set/clear
// mask on vals+0x00 (selected by the byte at src+0x0A) instead of a callback.
int func_80057084(u32 flags, ActParamHost5* host, ActParamVals5* vals, ActParamSrc5* src);

// func_80056D00: like func_80056A98 but
int func_80057280(u32 flags, ActParamHost5* host, ActParamVals5* vals, ActParamSrc5* src);
int func_800568E8(u32 flags, ActParamHost5* host, ActParamVals5* vals, ActParamSrc5* src);
int func_80057670(u32 flags, ActParamHost5* host, ActParamVals5* vals, ActParamSrc5* src);

// ---- func_80054D3C walker ----
// Record walked by func_80054D3C.
struct ActParamD3CRec {
    u16 mOffset0;                  // stride to the next record
    u16 mType2;                    // dispatch: 9, 0xF, 0x14
    u8 _pad04[4];
    u16 mShort08;                  // converted to double for the threshold tests
    u8 mByte0A;
    u8 mByte0B;
};
// Stream head referenced by member+0x6C.
struct ActParamD3CStream {
    u16 mHeadOff00;                // offset of the first record
    u8 _pad02[0x0C - 0x02];
    u32 mField0C;                  // bit 9 enables type-9 handling
};
// Time-word view of an ActParamD3CRec used by func_800550E8: the two
// threshold halfwords at +0x08/+0x0A are read independently.
struct ActParamD3CTimeRec {
    u8 _pad00[8];
    u16 mTime08;                   // +0x08: phase-A time word
    u16 mTime0A;                   // +0x0A: phase-B time word
};

// Callback interface at CActParamData+0x24 used by func_800550E8: fires the
// vt+0x24 slot with the entry object and a selector (0 = phase A, 1 = B).
struct ActParamCbViewSel24 {
    virtual void v08(); virtual void v0C(); virtual void v10();
    virtual void v14(); virtual void v18(); virtual void v1C();
    virtual void v20();
    virtual void invoke24(void* obj, u32 sel); // vt+0x24
};

// Anim-side state queries used by func_800550E8 (defined in the
// kyoshin/action/CActParamAnim split).
extern "C" int func_8004B8F8(void* obj, u32 sel);
extern "C" int func_8004B990(void* obj, u32 sel);

// Owner block walked by func_80054D3C (a CActParamData sub-object).
struct ActParamD3CMember {
    u8 _pad00[0x50];
    volatile float mFloat50;
    volatile float mFloat54;
    volatile s32 mField58;
    u8 _pad5C[0x6C - 0x5C];
    ActParamD3CStream* mStream6C;  // record-stream head
    volatile float mFloat70;
};
// Callback interface at CActParamData+0x24 used by func_80054D3C.
struct ActParamCbViewD3C {
    virtual void dummy08();
    virtual void invoke0C(void* obj, u8 a, u8 b); // vt+0x0C
    virtual void dummy10();
    virtual void dummy14();
    virtual void invoke18(void* obj, void* src);  // vt+0x18
};
extern "C" void func_80054D3C(CActParamData* self, ActParamD3CMember* member);
__declspec(noinline) int func_800550E8(CActParamData* self,
                                       ActParamD3CMember* member,
                                       ActParamD3CTimeRec* rec, int gate);

// ---- func_800547D4 ----
// Record-chain walker driven through the handler table lbl_eu_80570788.
struct ActParamT1Rec {
    u16 mOffset0;                  // stride to the next record
    u16 mType2;                    // handler-table index while > 1
};
// Node whose extent [+0,+size04) is tested against the current src pointer.
struct ActParamT1Node {
    u8 _pad00[4];
    u32 mSize04;
};
typedef int (*ActParamTbl1Fn)(u32, void*, void*, void*);
extern const ActParamTbl1Fn lbl_eu_80570788[];

// Chain-entry/source view: linked list of parameter entries.
struct ActParamT1Src {
    u16 mOffset0;
    u16 mType2;
    u8 _pad04[4];
    void* mPtr08;
    u32 mField0C;
};

// Destination block initialized wholesale by func_800547D4.
struct ActParamT1Dst {
    u32 mField00;
    u32 mField04;
    u8 _pad08[4];
    u32 mField0C;
    u8 _pad10[4];
    ActParamT1Src* mPtr14;
    ActParamT1Src* mPtr18;
    u32 mField1C;
    u32 mField20;
    float mFloat24;
    float mFloat28;
    float mFloat2C;
    u8 mByte30;
    u8 mByte31;
    u8 _pad32[2];
    u32 mField34;
    u32 mField38;
    u32 mField3C;
    float mFloat40;
    float mFloat44;
    u32 mField48;
    u32 mField4C;
    u8 _pad50[0x5C - 0x50];
    u32 mField5C;
    u32 mField60;
    u8 _pad64[4];
    float mFloat68;
    u8 _pad6C[0x74 - 0x6C];
    u32 mField74;
    u32 mField78;
    s16 mShort7C;
};

// Host carrying the slot arrays searched for the entry containing src.
struct ActParamT1Host {
    u8 _pad00[0x10];
    void* mObj10;                  // 0x10: interface object (returned)
    float mFloat14;
    u32 mField18;
    u32 mField1C;
    u32 mField20;
    u8 _pad24[0x1D0 - 0x24];
    ActParamT1Node* mSlots1D0[8];
    ActParamT1Node* mSlots1F0[8];
    u32 mCount210;
    u8 _pad214[2];
    u8 mByte216;
    u8 _pad217[0x25C - 0x217];
    u32 mField25C;
};
extern "C" void* func_800547D4(ActParamT1Host* host, ActParamT1Dst* dst, ActParamT1Src* src);
__declspec(noinline) void func_80055700(CActParamLinkTable* table, int flag, CActParamRecStream* stream);
void func_80055AC4(ActParamStack* self, ActParamStackNode* a, ActParamStackNode* b);
// Retail no-op stubs (blr only)
extern "C" void func_80054A20();
extern "C" void func_800550D8();
extern "C" void func_800550DC();
extern "C" void func_800550E0();
extern "C" void func_800550E4();
extern "C" void func_800554D0();
extern "C" void func_800554D4();
extern "C" void func_800554D8();
