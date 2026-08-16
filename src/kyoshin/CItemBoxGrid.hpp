#pragma once

#include <types.h>
class CBaseCur;
class CItemImplInstances;
class UnkClass_8045F564;
namespace nw4r { namespace lyt {
class AnimTransform; class Layout; class ArcResourceAccessor; class DrawInfo;
} }

class CQuestItem {
public:
    virtual ~CQuestItem();

    // TODO: add fields
};

class CVisionItem {
public:
    virtual ~CVisionItem();

    // TODO: add fields
};

class CArtsBookItem {
public:
    CArtsBookItem();
    virtual ~CArtsBookItem();

    // TODO: add fields
};

// Cast-only class with a virtual destructor at vtable+0x08 (the first
// declared virtual after the 2 RTTI header entries). Used for delete-style
// release calls with mode 1 on the +0x44/+0x5C sub-objects: retail's
// `if (ptr) delete ptr` shows two identical null tests (the if plus the
// delete expansion's own guard), which the plain manual-cast form cannot
// reproduce.
struct CItemBoxObjVt08 {
    virtual ~CItemBoxObjVt08();  // vtable+0x08
};

// Cast-only vtable interface for the +0x3C/+0x40 accessor objects: method at
// vtable+0x0C (raw slot 3, takes a tag string id, an output buffer and a
// flag, returns the created pane). MWCC prepends 2 RTTI header entries, so
// the second declared virtual lands here.
struct CItemBoxAccVt0C {
    virtual void _v08();
    virtual void* _v0C(int tag, char* buf, int flag);  // vtable+0x0C
};

// Cast-only vtable interface for the +0x70..+0xD0 sub-objects: method at
// vtable+0x0C (raw slot 3, no args). MWCC prepends 2 RTTI header entries,
// so the second declared virtual lands here.
struct CItemBoxObjVt0C {
    virtual void _v08();
    virtual void _v0C();  // vtable+0x0C
};

// Cast-only vtable interface for the CItemImplInstances object: method at
// vtable+0x2C (raw slot 11). MWCC prepends 2 RTTI header entries, so 9
// dummy virtuals precede the method.
struct CItemInstVt2C {
    virtual void _v08();
    virtual void _v0C();
    virtual void _v10();
    virtual void _v14();
    virtual void _v18();
    virtual void _v1C();
    virtual void _v20();
    virtual void _v24();
    virtual void _v28();
    virtual void* _v2C(void* arg, u8 slot);  // vtable+0x2C
};

// Cast-only vtable interface for the CItemImplInstances object: method at
// vtable+0x40 (raw slot 16). MWCC prepends 2 RTTI header entries, so 14
// dummy virtuals precede the method.
struct CItemInstVt40 {
    virtual void _v08();
    virtual void _v0C();
    virtual void _v10();
    virtual void _v14();
    virtual void _v18();
    virtual void _v1C();
    virtual void _v20();
    virtual void _v24();
    virtual void _v28();
    virtual void _v2C();
    virtual void _v30();
    virtual void _v34();
    virtual void _v38();
    virtual void _v3C();
    virtual u32 _v40(void* arg, u8 slot);  // vtable+0x40
};

// Cast-only vtable interface for the +0x44 layout object: method at
// vtable+0x38 (raw slot 14). MWCC prepends 2 RTTI header entries, so 12
// dummy virtuals precede the method.
struct CItemBoxLayoutVt38 {
    virtual void _v08();
    virtual void _v0C();
    virtual void _v10();
    virtual void _v14();
    virtual void _v18();
    virtual void _v1C();
    virtual void _v20();
    virtual void _v24();
    virtual void _v28();
    virtual void _v2C();
    virtual void _v30();
    virtual void _v34();
    virtual void _v38(void* arg, int n);  // vtable+0x38
};

// Cast-only vtable interface for the +0xA0/+0xD0 sub-objects: with -RTTI on,
// the third declared virtual lands at vtable+0x10 (slots 0/4 hidden RTTI),
// matching retail's lwz r12,16(r12) dispatch that the manual-cast form
// colors r5 instead of r12.
struct CItemBoxObjA0Vt {
    virtual void _v08();
    virtual void _v0C();
    virtual void _v10(void* arg);  // vtable+0x10
};

// Cast-only vtable interface for the layout-pane sub-object reached via
// *(*(self+0x44)+0x10): method at raw vtable slot 15 (offset 0x3C). MWCC
// prepends 2 RTTI header entries, so 13 dummy virtuals precede the method.
struct CItemPaneObjVt {
    virtual void _v08();
    virtual void _v0C();
    virtual void _v10();
    virtual void _v14();
    virtual void _v18();
    virtual void _v1C();
    virtual void _v20();
    virtual void _v24();
    virtual void _v28();
    virtual void _v2C();
    virtual void _v30();
    virtual void _v34();
    virtual void _v38();
    virtual void* _v3C(char* buf, int n);  // vtable+0x3C
};

// Cast-only vtable interface for the layout-pane sub-object reached via
// *(self+0x44): method at raw vtable slot 11 (offset 0x2C). MWCC prepends
// 2 RTTI header entries, so 9 dummy virtuals precede the method.
struct CItemPaneAnimVt {
    virtual void _v08();
    virtual void _v0C();
    virtual void _v10();
    virtual void _v14();
    virtual void _v18();
    virtual void _v1C();
    virtual void _v20();
    virtual void _v24();
    virtual void _v28();
    virtual void* _v2C(void* arg, int n);  // vtable+0x2C
};

// Cast-only vtable interface for the CItemImplInstances object: method at
// vtable+0x08 (raw slot 2, the first declared virtual after the 2 RTTI
// header entries).
struct CItemInstVt08 {
    virtual u32 _v08(void* arg);  // vtable+0x08
};

// Cast-only vtable interface for the CItemImplInstances object: method at
// vtable+0x30 (raw slot 12). MWCC prepends 2 RTTI header entries, so 10
// dummy virtuals precede the method.
struct CItemInstVt30 {
    virtual void _v08();
    virtual void _v0C();
    virtual void _v10();
    virtual void _v14();
    virtual void _v18();
    virtual void _v1C();
    virtual void _v20();
    virtual void _v24();
    virtual void _v28();
    virtual void _v2C();
    virtual u32 _v30(void* arg);  // vtable+0x30
};

// Cast-only vtable interface for the CItemImplInstances object: method at
// vtable+0x90 (raw slot 36). MWCC prepends 2 RTTI header entries, so 34
// dummy virtuals precede the method.
struct CItemInstVt90 {
    virtual void _v08();
    virtual void _v0C();
    virtual void _v10();
    virtual void _v14();
    virtual void _v18();
    virtual void _v1C();
    virtual void _v20();
    virtual void _v24();
    virtual void _v28();
    virtual void _v2C();
    virtual void _v30();
    virtual void _v34();
    virtual void _v38();
    virtual void _v3C();
    virtual void _v40();
    virtual void _v44();
    virtual void _v48();
    virtual void _v4C();
    virtual void _v50();
    virtual void _v54();
    virtual void _v58();
    virtual void _v5C();
    virtual void _v60();
    virtual void _v64();
    virtual void _v68();
    virtual void _v6C();
    virtual void _v70();
    virtual void _v74();
    virtual void _v78();
    virtual void _v7C();
    virtual void _v80();
    virtual void _v84();
    virtual void _v88();
    virtual void _v8C();
    virtual u32 _v90(void* arg);  // vtable+0x90
};

// Cast-only vtable interface for the CItemImplInstances object: method at
// vtable+0x20 (raw slot 8). MWCC prepends 2 RTTI header entries, so 6 dummy
// virtuals precede the method.
struct CItemInstVt20 {
    virtual void _v08();
    virtual void _v0C();
    virtual void _v10();
    virtual void _v14();
    virtual void _v18();
    virtual void _v1C();
    virtual void* _v20(void* arg);  // vtable+0x20
};

// Cast-only vtable interface for the CItemImplInstances object: method at
// vtable+0x4C (raw slot 19, takes the item and a u8 slot index). MWCC
// prepends 2 RTTI header entries, so 17 dummy virtuals precede the method.
struct CItemInstVt4C {
    virtual void _v08();
    virtual void _v0C();
    virtual void _v10();
    virtual void _v14();
    virtual void _v18();
    virtual void _v1C();
    virtual void _v20();
    virtual void _v24();
    virtual void _v28();
    virtual void _v2C();
    virtual void _v30();
    virtual void _v34();
    virtual void _v38();
    virtual void _v3C();
    virtual void _v40();
    virtual void _v44();
    virtual void _v48();
    virtual u32 _v4C(void* arg, u8 slot);  // vtable+0x4C
};

// Cast-only vtable interface for the CItemImplInstances object: method at
// vtable+0x80 (raw slot 32). MWCC prepends 2 RTTI header entries, so 30
// dummy virtuals precede the method.
struct CItemInstVt80 {
    virtual void _v08();
    virtual void _v0C();
    virtual void _v10();
    virtual void _v14();
    virtual void _v18();
    virtual void _v1C();
    virtual void _v20();
    virtual void _v24();
    virtual void _v28();
    virtual void _v2C();
    virtual void _v30();
    virtual void _v34();
    virtual void _v38();
    virtual void _v3C();
    virtual void _v40();
    virtual void _v44();
    virtual void _v48();
    virtual void _v4C();
    virtual void _v50();
    virtual void _v54();
    virtual void _v58();
    virtual void _v5C();
    virtual void _v60();
    virtual void _v64();
    virtual void _v68();
    virtual void _v6C();
    virtual void _v70();
    virtual void _v74();
    virtual void _v78();
    virtual void _v7C();
    virtual u32 _v80(void* arg);  // vtable+0x80
};

// Cast-only vtable interface for the CItemImplInstances object: method at
// vtable+0x54 (raw slot 21). MWCC prepends 2 RTTI header entries, so 19
// dummy virtuals precede the method.
struct CItemInstVt54 {
    virtual void _v08();
    virtual void _v0C();
    virtual void _v10();
    virtual void _v14();
    virtual void _v18();
    virtual void _v1C();
    virtual void _v20();
    virtual void _v24();
    virtual void _v28();
    virtual void _v2C();
    virtual void _v30();
    virtual void _v34();
    virtual void _v38();
    virtual void _v3C();
    virtual void _v40();
    virtual void _v44();
    virtual void _v48();
    virtual void _v4C();
    virtual void _v50();
    virtual u32 _v54(void* arg);  // vtable+0x54
};

// 10-byte grid cell: stored item id + 8 per-cell flag bytes
struct CItemBoxGridEntry {
    s16 id;        // +0x00 item id (or -1 for an empty cell)
    u8 flags[8];   // +0x02 per-cell flag bytes
};

// Full object layout for CItemBoxGrid (used by C-linkage accessors)
struct CItemBoxGridFull {
    union {
        CItemBoxGridEntry entries[0x280];  // cells 0x0000..0x2800
        struct {
            u8 _00[0x61];
            u8 field_61;
            u8 _62[0x52D - 0x62];
            u8 field_52D;
            u8 _52E[0x549 - 0x52E];
            u8 field_549;
            u8 _54A[0x2800 - 0x54A];
        } bytes;
    };
    u16 field_2800;  // entry count
    u8 field_2802;   // current category
    u8 field_2803;   // row count
    u8 field_2804;   // page index
};

class CItemBoxGrid {
public:
    CItemBoxGrid();
    virtual ~CItemBoxGrid();
    void OnFileEvent();

    u8 GetField61();
    u8 GetField549();
    u8 GetField52D();
    void PushToList(unsigned char val);

    // TODO: add fields
};


// C-linkage imports (retail symbol names - keep linkage/signatures verbatim)
extern "C" int func_8022DB74(void*);
extern "C" void __dl__FPv(void*);
extern "C" void* func_801394D4(u32);
extern "C" void func_801D4AE0(void*, int, void*);
extern "C" void func_801CFF28(void*);
extern "C" int CSysWin_isActive(void*);
extern "C" int CSysWin_getUnk34(void*);
extern "C" void func_801D216C(void*, u8);
extern "C" __declspec(noinline) void func_801D0950(void*);
extern "C" u32 func_80137510(nw4r::lyt::AnimTransform*, float);
extern "C" void* func_80157C4C(u32, s16);
extern "C" u32 func_801392E4(u32);
extern "C" u32 func_801361E8(u32, const char*, u32);
extern "C" u16 func_80139358(u32);
extern "C" CItemImplInstances* CItem_initItemImplInstances(void*);
extern "C" u32 func_801D3320(void*);
extern "C" void func_80158118(void*, u32);
extern "C" u32 func_80208360(void*);
extern "C" void func_80208760(void*, void*);
extern "C" void func_8022E498(void*, void*);
extern "C" void func_8022D0F8(void*, void*, u8);
extern "C" u32 func_801D32DC(void*);
extern "C" u32 getItemBoxState__FP12CItemBoxInfo(void*);
extern "C" u32 func_801EB018(void*);
extern "C" u32 CSysWin_isReady(void*);
extern "C" u32 func_801EB020(void*);
extern "C" u32 func_80208358(void*);
extern "C" u32 func_8022DB6C(void*);
extern "C" void advanceItemBoxState__FP12CItemBoxInfo(void*);
extern "C" void func_801EB410(void*, int);
extern "C" void func_801EB0D4(void*);
extern "C" void func_801D0328(void*);
extern "C" void func_801CFFEC(void*);
extern "C" u32 func_801D1220(void*);
extern "C" char* func_801C6A44(void*, u16);
extern "C" void func_801D05D4(void*, int);
extern "C" void func_80138078__FUl(u32);
extern "C" u16 func_8015780C(int);
extern "C" int getLanguage__9CDeviceSCFv();
extern "C" u32 func_801D3328(void*);
extern "C" void func_801D3454(void*, void*);
extern "C" void func_801D3430(void*, void*);
extern "C" void func_801D353C(void*, u8);
extern "C" void func_801D3330(void*);
extern "C" u32 func_801D3C74(void*, u32);
extern "C" void func_801D4BDC(void*, u32, u32);
extern "C" void func_801D3620(void*);
extern "C" void func_801D3698(void*);
extern "C" void func_801D3724(void*);
extern "C" void func_801D377C(void*);
extern "C" void func_801D3408(void*);
extern "C" u32 func_801EB028(void*);
extern "C" void func_801EB064(void*, u32);
extern "C" void func_801EB04C(void*, u8);
extern "C" u32 func_801EB218(void*);
extern "C" u32 func_801EB314(void*);
extern "C" void func_801EB178(void*);
extern "C" void func_8020844C(void*);
extern "C" u32 func_802087B8(void*);
extern "C" char* func_80138F78(u32);
extern "C" int* func_8009ECB0(void);
extern "C" void func_80157824(u8, u32);
extern "C" void* func_801571FC(void);
extern "C" void func_80157184(int);
extern "C" void func_801EB030(void*, void*);
extern "C" void func_8013B428__FUl(u32);
extern "C" void* func_80452C10__11CDeviceFontFUlPQ34nw4r3lyt6Layout(u32, nw4r::lyt::Layout*);
extern "C" void func_8003AA8C__5CBdatFUl(u32);
extern "C" void func_801390E0__FPP11CFileHandle(void**);
extern "C" void func_80139124__FPQ34nw4r3lyt19ArcResourceAccessor(void*);
extern "C" void func_8045F778__17UnkClass_8045F564Fv(void*);
extern "C" void func_8022B7F4(void*);
extern "C" void func_802083A4(void*);
extern "C" void func_80208838(void*);
extern "C" void func_8022E3A8(void*);
extern "C" int func_8022E490(void*);
extern "C" void func_8022E2F8(void*);
extern "C" void func_8022E3AC(void*);
extern "C" u32 func_8022E488(void*);
extern "C" void func_8022DD68(void*);
extern "C" void func_8022D0D0(void*);
extern "C" void func_8022B8E4(void*);
extern "C" void func_8022C1B4(void*, void*, u8);
extern "C" void func_801375A0(void*, void*);
extern "C" void func_801D47D4(void*, u32, u32, u32);
extern "C" u32 func_801D421C(void*);
extern "C" void func_801D4C3C(void*, u32);
extern "C" void func_80124270(void*, u32);
extern "C" void func_80137E7C(void*, void*, void*);
extern "C" void func_80137924(void*, void*, void*, void*);
extern "C" u32 func_8013600C(void*, const char*, u32);
extern "C" void func_80136B4C(nw4r::lyt::Layout*, char*, char*, u32);
extern "C" void func_80136A1C(nw4r::lyt::Layout*, char*, char*, u32);
extern "C" char* func_8013639C(const void*, const void*, int);
extern "C" u32 func_800A32BC();
extern "C" void copyVEC3(void*, const void*);
extern "C" u32 func_8003B1EC(void*);
extern "C" u32 func_8009CF8C(u32);
extern "C" void func_80043D90(void*);
extern "C" void* func_80043F18(void*);
extern "C" void func_800F4A98(void*, u32, u32);
extern "C" void* func_800F6EC0(void*, u32);
extern "C" void* func_800BFC68__FPQ22cf12CfObjectMove(void*);
extern "C" void func_800BFDE0(void*, u32);
extern "C" void func_80043E88(void*, int);
extern "C" void func_8009E0A8(void*, int);
extern "C" void func_8009E024(void*, int);
extern "C" void func_8009E030(void*, int);
extern "C" void func_8009E03C(void*, int);
extern "C" void func_8009E048(void*, int);
extern "C" void func_8009E054(void*, int);
extern "C" void func_80139A18(void*, void*, void*, void*);
extern "C" char* func_80136190(const void*, const void*, int);
extern "C" void func_801C5158(void*, u32);
extern "C" u8 func_801392B4(u32);
extern "C" u32 func_801393CC(u32);
extern "C" int func_8026178C(void*, u32);
extern "C" u32 func_8025FB10(void*, u32);
extern "C" u32 func_802083CC(void*);
extern "C" void* func_8009EC9C(u32);
extern "C" u32 func_800A082C(void);
extern "C" u8 code80135FDC_getByte_64077();
extern "C" void func_801C4B60(void*, s16, s16, s16, s16);
extern "C" void func_801D1F9C(void*, u32);
extern "C" u32 func_801C62AC(void*, u32);
extern "C" u32 func_801C631C(void*, u32);
extern "C" u16 ArrayGet12(const unsigned short*, unsigned char);
extern "C" char* func_eu_802B148C(void);
extern "C" char* func_eu_802B1474(void);
extern "C" int func_801C51BC(void* obj, u16 id);
extern "C" u8 func_801C67F8(CItemBoxGridFull* self);
extern "C" void func_8022B90C(void*, int);
extern "C" void func_8022B9B4(void*, u32, int);
extern "C" void func_8022BFC8(void*, int);
extern "C" void func_8022B8B8(void*);
extern "C" int func_801C6E90(void*);
extern "C" u32 func_801D4260(void*, u8);
extern "C" void func_801D0BD8(void*);
extern "C" void func_801CFD2C(void*);
extern "C" u32 func_80157C20(u8);
extern "C" void func_801CE390(void*);
extern "C" float func_801C9F88(void*, void*);
extern "C" u32 func_801CA070(void*, void*);
extern "C" u32 func_801C7958(void*, void*);
extern "C" u32 func_80137444__FPQ34nw4r3lyt13AnimTransformf(void*, float);
extern "C" char* CopyEntry9Bytes(char*, const char*);
extern "C" void* getHandleMEM2__Q23mtl10MemManagerFv();
extern "C" void* getHandleMEM1__Q23mtl10MemManagerFv();
extern "C" u32 getMaxAllocSize__Q23mtl10MemManagerFUl(u32);
extern "C" void deallocate__Q23mtl10MemManagerFPv(void*);
extern "C" u32 func_80212480();
extern "C" int func_800A9D90();
extern "C" void* readFile__11CDeviceFileFUlPCcP10IWorkEventii(u32, const char*, void*, int, int);
extern "C" void* readCommonArchiveFile__11CDeviceFileFUlPCcP10IWorkEventii(u32, const char*, void*, int, int);
extern "C" void func_801D4154(void*);
extern "C" void func_80137038__FPQ34nw4r3lyt6LayoutPQ34nw4r3lyt8DrawInfoii(nw4r::lyt::Layout*, nw4r::lyt::DrawInfo*, int, int);
extern "C" void func_801D20B0(void*, void*);
extern "C" void func_801D31F8(void*, void*);
extern "C" void func_802082D0(void*, int);
extern "C" void func_8022CFEC(void*, int);
extern "C" void func_8022B7C8(void*, void*);
extern "C" void func_8022DAD8(void*, int);
extern "C" void func_801EAF7C(void*, void*);
extern "C" void func_801D202C(void*);
extern "C" void func_801D3160(void*);
extern "C" void func_801D40C4(void*);
extern "C" void func_801EAED4(void*);
extern "C" void func_80208260(void*);
extern "C" void func_8022CF7C(void*);
extern "C" void func_8022DA58(void*);
extern "C" void func_8022B748(void*);
extern "C" void func_801D3064(void*);
extern "C" void func_801D4054(void*);
extern "C" void func_801EAE8C(void*);
extern "C" void func_8022CF2C(void*);
extern "C" void func_801D3258(void*);
extern "C" void func_801D4174(void*);
extern "C" void func_801EAF9C(void*);
extern "C" void func_802082F0(void*);
extern "C" void func_8022D018(void*);
extern "C" void func_8022DB04(void*);
extern "C" void* getFP__FPCc(const char*);
extern "C" u16 func_80136254(const void*, const void*, int);
extern "C" void __ct__CVisionItem(void*);
extern "C" void __ct__CArtsBookItem(void*);
extern "C" void SetEntry9Bytes(unsigned char*, short, unsigned char, unsigned char, unsigned char, unsigned char, unsigned char, unsigned char, unsigned char);
extern "C" void func_801C562C(void*, char*);
extern "C" void func_801C7EF0(CItemBoxGridFull*, u32);
extern "C" void func_801C8ACC(CItemBoxGridFull*, u32);
extern "C" void func_801C8C58(CItemBoxGridFull*, u32);
extern "C" void func_801C98D0(CItemBoxGridFull*, u32);
extern "C" void func_801C9E1C(CItemBoxGridFull*, u32);
extern "C" u32 func_801576C8(u8);
extern "C" void func_8022E204(void*, u32);
extern "C" void func_8022DB7C(void*);
extern "C" void func_8022DD90(void*);
extern "C" u32 func_8022E4FC(void*);
extern "C" u32 func_8022E504(void*);
extern "C" u32 func_801D37F4(void*);
extern "C" u32 func_801D3808(void*);
extern "C" u32 func_801D3810(void*);
extern "C" void func_8022D0A4(void*);
extern "C" void func_8022D19C(void*, char*, u32);
extern "C" void func_80208368(void*);
extern "C" void func_802084D4(void*, u32);
extern "C" void func_801D11B8(void*, void*, int);
extern "C" void func_8022BF6C(void*, u32, u32);
extern "C" u32 func_801D12D4(void*, u32);
extern "C" void func_801586D4(u32, u32);
extern "C" void func_80140E00(u32, u32, u32);
extern "C" u32 func_8008235C__Q22cf13CfGameManagerFv(u32);
extern "C" void func_800A18A4(void*, u8);
extern "C" u32 func_801380A0(u16);
extern "C" u32 func_80138138(u16);
extern "C" void* func_801412D0(u32);
extern "C" u32 func_80140854(void*, u32, u32);
extern "C" void* getHandleMEM2__Q23mtl10MemManagerFv(void);
extern "C" void* createRegion__17UnkClass_8045F564FiiPCci(void*, int, int, const char*, int);
extern "C" void __ct__14Class_8045F858FP17UnkClass_8045F564(void*, void*);
extern "C" void func_80434A4C__Q23mtl10MemManagerFb(bool);
extern "C" void* getAllocHandle__10CLibLayoutFv(void);
extern "C" void* allocate__Q23mtl10MemManagerFUlUl(u32, u32);
extern "C" void* __ct__CTagProcessor(void*);
extern "C" nw4r::lyt::ArcResourceAccessor* createArcResourceAccessor__10CLibLayoutFv();
extern "C" bool Attach__Q34nw4r3lyt19ArcResourceAccessorFPvPCc(nw4r::lyt::ArcResourceAccessor*, void*, const char*);
extern "C" void func_80136E84__FPPQ34nw4r3lyt6LayoutPQ34nw4r3lyt19ArcResourceAccessorPCc(nw4r::lyt::Layout**, nw4r::lyt::ArcResourceAccessor*, const char*);
extern "C" void func_80136F08__FPQ34nw4r3lyt6LayoutPPQ34nw4r3lyt13AnimTransformPQ34nw4r3lyt19ArcResourceAccessorPc(nw4r::lyt::Layout*, nw4r::lyt::AnimTransform**, nw4r::lyt::ArcResourceAccessor*, char*);
extern "C" void func_8013676C(void*, u32);
extern "C" void* func_801355A0__Fv(void);
extern "C" void* func_801355BC(void);
extern "C" void func_801368C0__FPQ34nw4r3lyt6LayoutPcUl(nw4r::lyt::Layout*, char*, u32);
extern "C" void func_80139198(u32);
extern "C" u16 func_8013606C(const void*, const void*, u32);
extern "C" nw4r::lyt::ArcResourceAccessor* func_801355F4();
extern "C" void func_80139658(void*, const char*, u32);
extern "C" void CopyVec4s(void*, void*);
extern "C" CBaseCur* __ct__CCur07(void*, void*);
extern "C" void __ct__CSortMenu(void*);
extern "C" void __ct__CItemBoxInfo(void*, int, int);
extern "C" void __ct__CNumSelect(void*, int);
extern "C" void __ct__CItemBoxGridSubMenu(void*);
extern "C" void __ct__CExchangeWin(void*);
extern "C" void __ct__CPresentWin(void*);
extern "C" void __ct__CSysWin(void*, int);
extern "C" void __ct__UnkClass_8011C974(void*, void*);
extern "C" void func_8016742C(void*, void*);
extern "C" void func_8013B2D4();
extern "C" void* __ct__801C5514(void*);
extern "C" CBaseCur* __ct__CCur09(void*, void*);
extern "C" CBaseCur* __ct__CCur18(void*, void*);
extern "C" CBaseCur* __ct__CCur16(void*, void*);
extern "C" CBaseCur* __ct__CCur11(void*, void*);
extern "C" void __dt__6CCur07Fv(void*, int);
extern "C" void __dt__6CCur09Fv(void*, int);
extern "C" void __dt__6CCur18Fv(void*, int);
extern "C" void __dt__6CCur16Fv(void*, int);
extern "C" void __dt__6CCur11Fv(void*, int);
extern "C" void __dt__7CSysWinFv(void*, int);
extern "C" void __dt__11CPresentWinFv(void*, int);
extern "C" void __dt__12CExchangeWinFv(void*, int);
extern "C" void __dt__19CItemBoxGridSubMenuFv(void*, int);
extern "C" void __dt__10CNumSelectFv(void*, int);
extern "C" void __dt__12CItemBoxInfoFv(void*, int);
extern "C" void __dt__9CSortMenuFv(void*, int);
extern "C" void __dt__17UnkClass_8045F564Fv(void*, int);
extern "C" void func_8018B0FC(void*, void*);
extern "C" void func_8018B130(void*, void*);
extern "C" void code80135FDC_setVec3(float*, float, float, float);
extern "C" void func_801D24E8(void*, void*, void*);
extern "C" void func_8022D614(void*, void*);
extern "C" void func_80207FC8(void*, void*);
extern "C" CEquipBoxFourShorts func_801397AC(void*, u32);
extern "C" int func_80086F9C__Q22cf13CfGameManagerFv(int);
