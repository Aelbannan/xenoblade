#pragma once

#include <types.h>
#include "kyoshin/CSysWin.hpp"
#include "kyoshin/CBaseCur.hpp"

namespace nw4r {
namespace lyt {
    class Layout;
    class Pane;
    class AnimTransform;
    class ArcResourceAccessor;
    class DrawInfo;
}
}

// 0x2800-byte entry storage block, copied member-wise in clpStartLoads
struct CLPDataBlock {
    u8 v[0x2800];
};

// Stack temporary used to initialize the entry storage (matches retail frame layout)
struct CLPInitTemp {
    u8 _00;
    s8 _01;
    u8 pad[2];
    CLPDataBlock data;
};

// Full `this` struct for harness decomp of CCollepedia.cpp
// Entry within the sub-block array (stride 0xA)
struct CCollepediaEntry {
    u8 _00[0x08];
    u16 field_08; // u16 at offset +0x08 within entry
};

// Sub-range of the CCollepedia class from +0x00 to +0xE8
struct CCollepediaFull {
    u8 _00[0xD8];
    u8 field_D8;
    u8 field_D9;
    u8 _DA[0xE8 - 0xDA];
    u8 field_E8_start; // sub-array starts at +0xE8
};

// Container with a layout pointer at +0x04 (used by clpCalcCurPos)
struct LayoutContainer {
    u8 _00[0x04];
    nw4r::lyt::Layout* mpLayout; // at +0x04
};

// Cursor class derived from CBaseCur, used by CCollepedia for cursor display.
// Vtable at lbl_eu_80537474 (3 entries, size 0x18 with padding).
class CCLPCur : public CBaseCur {
public:
    CCLPCur(nw4r::lyt::ArcResourceAccessor* pAccessor);
};

// Cursor body shared by CCur07/CCur18/CCLPCur (everything after the vtable
// pointer); OnFileEvent builds cursors on the stack and copies this region
// into the embedded members.
struct CLPCurBody {
    void* field_04;
    void* field_08;
    void* field_0C;
    void* field_10;
    u8 mActive;   // +0x14
    u8 mVisible;  // +0x15
};

// Second-page setup record (built by clpInitPgInfo, finished by
// clpSetupDetPg): accessor, loaded layout, bound animation transform.
struct CLPPageSetup {
    nw4r::lyt::ArcResourceAccessor* mpAccessor;   // +0x00
    nw4r::lyt::Layout* mpLayout;                  // +0x04
    nw4r::lyt::AnimTransform* mpAnimTrans;        // +0x08
};

// Texture size header reachable from a lyt texture resource's +0x08 pointer.
struct CLPTexSize {
    u16 field_00;
    u16 field_02;
};

// Shared .sdata2 u32->f64 conversion magic (2^52); retail .sdata2 label (owning TU or extern).
extern double lbl_eu_80668808;

// POD mirror of nw4r::lyt::Size (two f32): using the real type would emit
// the default-constructor zeroing, which retail does not have.
struct CLPSize {
    f32 width;
    f32 height;
};


// Page-info record produced by clpInitPgInfo and stored at +0x28EC.
struct CLPPageInfo {
    u32 field_00;                // +0x28EC
    nw4r::lyt::Layout* mpLayout; // +0x28F0
    u32 field_08;                // +0x28F4
    u8 field_0C;                 // +0x28F8
    u8 field_0D;                 // +0x28F9
    u8 field_0E;                 // +0x28FA
};



// CCollepedia is a non-virtual class in the decomp (layout is flat, no vtable ptr).
// Virtual destructor semantics are handled by __dt__<addr> thunks.
// The vtable pointer is stored at +0x00 as a regular field.
// CCollepedia is a non-virtual class in the decomp (layout is flat, no vtable ptr).
// Virtual destructor semantics are handled by __dt__<addr> thunks.
// The vtable pointer is stored at +0x00 as a regular field.
//
// NOTE: offset 0x0C is used as both a 4-byte state machine (via individual byte
// stores in state functions) and as a pointer in some rendering functions.
// The struct exposes it as a pointer; state functions access individual bytes
// via reinterpret_cast.
struct CCollepedia {
    /* 0x00 */ int field_0; // vtable ptr (set by ctor)
    /* 0x04 */ nw4r::lyt::Layout* field_4;
    /* 0x08 */ nw4r::lyt::Layout* field_8; // used as Layout* in rendering, AnimTransform* in state checks
    /* 0x0C */ nw4r::lyt::AnimTransform* field_c_ptr; // state machine (low bytes) or AnimTransform*
    /* 0x10 */ nw4r::lyt::AnimTransform* field_10_ptr;
    /* 0x14 */ u8 _14[0x24 - 0x14];
    /* 0x24 */ void* field_24; // CFileHandle* - file handle 1
    /* 0x28 */ void* field_28; // CFileHandle* - file handle 2
    /* 0x2C */ void* field_2C; // CFileHandle* - archive file handle
    /* 0x30 */ nw4r::lyt::ArcResourceAccessor* field_30;
    /* 0x34 */ nw4r::lyt::ArcResourceAccessor* field_34;
    /* 0x38 */ nw4r::lyt::Layout* field_38;
    /* 0x3C */ nw4r::lyt::AnimTransform* field_3C;
    /* 0x40 */ nw4r::lyt::AnimTransform* field_40; // AnimTransform* at 0x40
    /* 0x44 */ nw4r::lyt::AnimTransform* field_44; // AnimTransform* at 0x44
    /* 0x48 */ u8 field_48;
    /* 0x49 */ u8 field_49;
    /* 0x4A */ u8 _4A[0x4C - 0x4A];
    /* 0x4C */ u32 field_4C;
    /* 0x50 */ u8 field_50;
    /* 0x51 */ u8 field_51;
    /* 0x52 */ u8 _52[0x54 - 0x52];
    /* 0x54 */ u8 field_54[0x9C - 0x54]; // sub-object at 0x54 (used by Cur_SetVisible)
    /* 0x9C */ u8 field_9C; // CSysWin sub-object starts here
    /* 0x9D */ u8 _9D[0xD8 - 0x9D];
    /* 0xD8 */ u8 field_D8;
    /* 0xD9 */ u8 field_D9;
    /* 0xDA */ u8 field_DA;
    /* 0xDB */ u8 _DB; // padding
    /* 0xDC */ float field_DC[3]; // VEC3 copied from pane position (0xDC-0xE7)
    /* 0xE8 */ u8 field_E8; // sub-array starts here
    /* 0xE9 */ u8 _E9[0xEC - 0xE9];
    /* 0xEC */ CLPDataBlock field_EC; // entry storage (ends at 0x28EC)
    /* 0x28EC */ u8 field_28EC[0x28F0 - 0x28EC];
    /* 0x28F0 */ nw4r::lyt::Layout* field_28F0; // Layout* at 0x28F0 (used by clpDrawWindow)
    /* 0x28F4 */ u8 _28F4[0x28F9 - 0x28F4];
    /* 0x28F9 */ u8 field_28F9;
    /* 0x28FA */ u8 field_28FA;

    bool OnFileEvent(CEventFile* pEventFile);

    ~CCollepedia();
    CCollepedia();
};




// ---------------------------------------------------------------------------
// C-linkage imports (retail symbol names - keep linkage/signatures verbatim)
// ---------------------------------------------------------------------------
extern "C" nw4r::math::VEC3* code80135FDC_setVec3(float*, float, float, float);
extern "C" void __dl__FPv(void*);
extern "C" u32 CSysWin_isReady(u8*);
extern "C" u32 CSysWin_getUnk34(u8*);
extern "C" int CSysWin_isActive(u8*);
extern "C" u32 GetItemMulti_IsActiveFlag();
extern "C" u32 CItem_sumFamilyByte6(u16);
extern "C" char lbl_eu_8050C6E8[];
extern "C" void* lbl_eu_806640A8;
extern "C" void* lbl_eu_80664104;
extern "C" const float lbl_eu_80668800;
extern "C" const float lbl_eu_806687F0;
extern "C" const float lbl_eu_806687F4;
extern "C" const float lbl_eu_806687F8;
extern "C" const float lbl_eu_806687FC;
extern "C" const float lbl_eu_80668810;
extern "C" const float lbl_eu_80668814;
extern "C" const double lbl_eu_80668818;
extern "C" void playUISound__FUl(u32);
extern "C" void* lbl_eu_80537474[];
extern "C" void Cur_DrawLayout(void*, void*);
extern "C" void sysWinDrawLayout(void*, nw4r::lyt::DrawInfo*);
// u32 (not u16): retail returns the lookup result untruncated (tail-branch b
// in clpGetCelIcon); narrowing here would emit an extra rlwinm after the call.
extern "C" u32 BdatGetU16Direct(const void*, const void*, int);
extern "C" void UIWin_CreateItemMulti(u32, u32, u32, u32, u32, u32, u32, u32, u32);
extern "C" void func_80137924(nw4r::math::VEC3*, nw4r::lyt::Pane*, nw4r::lyt::Pane*, nw4r::lyt::Pane*);
extern "C" char* lbl_eu_806647DC;
extern "C" void* lbl_eu_806647D8;
extern "C" u32 CtrlObjectParam_GetStateBitFlag(u16);
extern "C" u16 lbl_eu_8050C6A0[];
extern "C" void Cur_SetVisible(void*, u8);
extern "C" void func_801D202C(void*); // CCur per-frame update
extern "C" u16 BdatGetItemId(u32);
extern "C" void sysWinDispatchPhase(void*); // CSysWin per-frame update
extern "C" u32 lbl_eu_806640EC;
extern "C" void __dt__7CSysWinFv(void*, int);
extern "C" void __dt__6CCur18Fv(void*, int);
extern "C" void __dt__6CCur07Fv(void*, int);
extern "C" void func_8022B9B4(void*, void*, int);
extern "C" void func_8022BFC8(CSysWin*, u8);
extern "C" void sysWinOpenPhase1(void*);
extern "C" void sysWinAdvancePhase3(void*);
extern "C" void sysWinTermLayout(void*);
extern "C" void getEntry__5CBdatFUl(u32);
extern "C" void closeFileHandle__FPP11CFileHandle(void*);
extern "C" void deleteRegion__17UnkClass_8045F564Fv(void*);
extern "C" void CtrlObjectParam_SetStateBitFlag(u16, u32);
extern "C" u32 CItem_findRecByFamily(u16);
extern "C" void CItemData_initFromFamily(void*, u16, u32);
extern "C" void incrementEventCounter__FUl(u32);
extern "C" void* getHandleMEM2__Q23mtl10MemManagerFv();
extern "C" void* readFile__11CDeviceFileFUlPCcP10IWorkEventii(u32, const char*, void*, int, int);
extern "C" void* readCommonArchiveFile__11CDeviceFileFUlPCcP10IWorkEventii(u32, const char*, void*, int, int);
extern "C" u32 KyoshinHeap_GetField44();
extern "C" float MenuSnd_GetMasterVol_95EC();
extern "C" void func_80043738(u32, const char*, void*, u32, u32, u32);
extern "C" void setPaneVisible(void*, u32);
extern "C" void copyVEC3(void*, const void*);

// Additional imports for the CCollepedia constructor
extern void* lbl_eu_805373E0[]; // CCollepedia vtable (retail .data)
extern "C" void __ct__CCur07(void* self, void* param);   // CCur07 ctor (+0x54)
extern "C" void __ct__CCur18(void* self, void* param);   // CCur18 ctor (+0x84)
extern "C" void __ct__CSysWin(CSysWin* self, int arg);   // CSysWin ctor

// Imports used by OnFileEvent / func_80253B3C
extern "C" u32 Bdat_GetMaxRow_B1EC(void*);                     // bdat row count
extern "C" u32 BdatGetItemType(u16);                       // item kind lookup
extern "C" u32 CtrlRemote_TouchBitByArg(u32);                       // unlock-flag lookup
extern "C" void* lbl_eu_806640A0;                        // bdat table A
extern "C" u32 lbl_eu_80664184;                          // default category id
extern "C" void setBdatEntry__5CBdatFUlPv(u32, void*);  // BDAT archive release
extern "C" void* Bdat_GetTable_AA34();                        // BDAT table unload
extern "C" void* getFontInfo__11CDeviceFontFUlPQ34nw4r3lyt6Layout(u32,
    nw4r::lyt::Layout*);                                 // CDeviceFont helper
// Shared tag string; retail symbol is the MANGLED getPackedFont__Fv, so this
// import must keep C++ linkage (MWCC appends __Fv).
void* getPackedFont();
extern "C" nw4r::lyt::ArcResourceAccessor* CUICfManager_getArcResourceAccessor(); // CLibLayout accessor
extern "C" void* CUICfManager_getPackedFont9C();                           // shared tag string
// Verbatim-mangled import: the real retail CfGameManager controller-type check
extern "C" int isClassicController__Q22cf13CfGameManagerFv(int arg);
extern "C" void func_8018B0FC(void* dst, void* src);     // cursor body copy

// Mirror of CSysWin +0x04..end (everything after the vtable pointer), used to
// member-wise copy a stack-constructed CSysWin into the embedded window at
// +0x9C the way retail does in the CCollepedia constructor.
struct CLPSysWinBody {
    UnkClass_8045F564 mMemRegion;                  // +0x04
    void* mFileHandle;                             // +0x14
    void* mTagProcessor;                           // +0x18
    nw4r::lyt::ArcResourceAccessor* mArcAccessor;  // +0x1C
    nw4r::lyt::Layout* mLayout;                    // +0x20
    nw4r::lyt::AnimTransform* mAnimTrans;          // +0x24
    u8 field_28;                                   // +0x28
    u32 field_2C;                                  // +0x2C
    u32 field_30;                                  // +0x30
    u8 field_34;                                   // +0x34
    u8 field_35;
    u8 field_36;
    u8 field_37;
    u8 field_38;
    u8 field_39;
};