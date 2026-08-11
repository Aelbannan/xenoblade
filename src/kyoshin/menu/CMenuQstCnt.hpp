#pragma once

#include <types.h>

namespace nw4r { namespace lyt {
    class Layout;
    class AnimTransform;
}}
class CScn;

// Mirror of the font object returned by CDeviceFont::func_80452C10. vtable
// slot 0x24 (index 9) yields the u32 bound into the layout's font pane via
// func_8013676C. Never instantiated, so no vtable is emitted; a genuine
// virtual call makes MWCC emit retail's r12 dispatch sequence
// (CSimpleFontObj / CMenuPTGaugeFont scheme).
class CMenuQstCntFont {
public:
    virtual ~CMenuQstCntFont();   // 0x00 (3 dtor slots)
    virtual void vf0C();          // 0x0C
    virtual void vf10();          // 0x10
    virtual void vf14();          // 0x14
    virtual void vf18();          // 0x18
    virtual void vf1C();          // 0x1C
    virtual void vf20();          // 0x20
    virtual u32 getFontHandle();  // 0x24
};

// 8-byte quest entry record (used at 0x94 and 0x194 inside CMenuQstCnt).
struct QstEntry {
    u16 f0;   // 0x00 quest id
    u16 f2;   // 0x02
    u8  f4;   // 0x04
    u8  f5;   // 0x05
    u8  f6;   // 0x06
};

// 8-byte quest-info record (element of the 0x400-element list in QstData).
struct QstInfo {
    u16 f0;
    u8  f2;
    u8  f3;
    u8  f4;
    u8  f5;
    u8  f6;
    u8  f7;
};

/*
 * Quest-status menu object (size 0x1A0).
 *
 * Retail stores the vtable pointer at +0x10 (CProcess-style layout). The base
 * IUIWindow subobject is destroyed OOL via __dt__9IUIWindowFv, and the ctor
 * directly runs CProcess's ctor then fills the IUIWindow region by hand, so
 * the struct is laid out explicitly rather than inheriting.
 */
struct CMenuQstCnt {
    u8  mPad0[0x10];                     // 0x00 link-list nodes
    u32 mVtab;                           // 0x10 vtable ptr
    u8  mPad14[0x3C - 0x14];             // 0x14
    u32 mPtmfFunc0;                      // 0x3C __ptmf_null
    u32 mPtmfThis0;                      // 0x40
    u32 mPtmfDelta0;                     // 0x44
    u32 mPtmfFunc1;                      // 0x48
    u32 mPtmfThis1;                      // 0x4C
    u32 mPtmfDelta1;                     // 0x50
    nw4r::lyt::Layout* mLayout;          // 0x54
    u32 mField58;                        // 0x58
    u32 mField5C;                        // 0x5C
    s32 mField60;                        // 0x60 (default -1)
    u8  mField64;                        // 0x64 (bool)
    u8  mField65;                        // 0x65
    u8  mField66;                        // 0x66
    u8  mField67;                        // 0x67 (default 1)
    u32 mField68;                        // 0x68
    u32 mOcc6C;                          // 0x6C vtable-adjusted trackers
    u32 mOcc70;                          // 0x70 (IScnRender pointer for removeRenderCB)
    CScn* mScene;                        // 0x74
    u8  mMemRegion[0x10];                // 0x78 UnkClass_8045F564 storage
    nw4r::lyt::AnimTransform* mAnim88;   // 0x88
    u32 mState8C;                        // 0x8C
    f32 mValue90;                        // 0x90
    QstEntry mEntries[32];               // 0x94 (0x100 bytes)
    QstEntry mSelEntry;                  // 0x194
    u8  mField19C;                       // 0x19C
    // sizeof = 0x1A0

    void Init();
    void Term();
    void Move();
    void cbRenderBefore();
};

// C-linkage imports (retail symbol names - keep linkage/signatures verbatim)
// ---------------------------------------------------------------------------

extern "C" int func_80144FF0();
extern "C" int func_80145030(void);
extern "C" nw4r::lyt::ArcResourceAccessor* func_801355F4();
extern "C" void initQstInfo(QstInfo* ptr);
extern "C" void* __dt__80227030(void* self, int mode);
extern "C" QstInfo* copyQstInfo(QstInfo* dst, const QstInfo* src);
extern "C" void func_80227660(QstInfo* dst, const QstInfo* src);
extern "C" void func_8022769C(QstInfo* dst, const QstInfo* src);
extern "C" void __ct__8CProcessFv(void*);
extern "C" void __dt__9IUIWindowFv(void*, int);
extern "C" void __ct__17UnkClass_8045F564Fv(void*);
extern "C" void __dt__17UnkClass_8045F564Fv(void*, int);
extern "C" void func_8045F778__17UnkClass_8045F564Fv(void*);
extern "C" void __dl__FPv(void*);
extern "C" void __construct_array(void*, void*, void*, u32, u32);
extern "C" void __destroy_arr(void*, void*, int, int);
extern "C" u32 getWorkMem__17CWorkThreadSystemFv();
extern "C" void* allocate__Q23mtl10MemManagerFUlUl(u32, u32);
extern "C" void Regist__8CProcessFP8CProcessb(void*, void*, bool);
extern "C" void waitForDrawDone__9CDeviceVIFv();
extern "C" void removeRenderCB__4CScnFP10IScnRender(void*, void*);
extern "C" int func_80293C10();
extern "C" int func_8029A658();
extern "C" int func_801B481C();
extern "C" int func_80124B78();
extern "C" int func_8011CD5C();
extern "C" char* func_801394D4(u16);
extern "C" u16 func_801380A0(u16);
extern "C" u32 func_80138138(u16);
extern "C" void __dt__Q34nw4r3lyt8DrawInfoFv(void*, int);
extern "C" void __ct__Q34nw4r3lyt8DrawInfoFv(void*);
extern "C" u32 __ptmf_null[];
extern "C" u32 lbl_eu_8052D238[];
extern "C" u32 lbl_eu_80536128[];
extern "C" u32 lbl_eu_80663E28;
extern "C" u32 lbl_eu_80663E24;
extern "C" CScn* lbl_eu_80663E14;
extern "C" f32 lbl_eu_8066856C;
extern "C" f32 lbl_eu_80668568;
extern "C" void* lbl_eu_80664094;
extern "C" void* lbl_eu_806640A8;
extern "C" u32 lbl_eu_80573D18[];
extern "C" char lbl_eu_80509A10[];
extern "C" char lbl_eu_80509AB4[];