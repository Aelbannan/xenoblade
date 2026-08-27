#pragma once

#include <types.h>

#include "monolib/work/CProcess.hpp"
#include "monolib/scn/IScnRender.hpp"
#include <nw4r/lyt/lyt_drawInfo.h>

class CFileHandle;
class CScn;
class CEventFile;

// View of the CProcess base vptr slot (+0x10): the create() factory writes the
// temp IUIWindow vtable (lbl_eu_8052D238) then the CSysWinBuff composite
// vtable (lbl_eu_80537DC0) at +0x10, mirroring the CSystemWindow ctor.
struct CSysWinBuffVtSlot {
    u8 _0[0x10];
    u32 mpVtable;  // +0x10
};

// Byte-range shim over the CProcess base + owned fields so the create()
// factory can write the vtable (+0x10), the __ptmf_null callback slots
// (+0x3C..0x53) and the trailing scalars without raw pointer arithmetic.
// Matches CSysWinBuff's layout exactly (CProcess.hpp: CProcess :
// CChildListNode, size 0x3C).
struct CSysWinBuffCtorShim {
    u8 _00[0x10];
    void* vtable;        // 0x10 - CProcess vtable, overwritten by the factory
    u8 _14[0x28];        // 0x14-0x3B - rest of CProcess
    u32 callbacks[6];    // 0x3C-0x53 - __ptmf_null callback slots
    u32 field54;         // 0x54
    u32 field58;         // 0x58
    u32 field5C;         // 0x5C
    s32 index;           // 0x60 - init -1
    u8 flag64;           // 0x64
    u8 flag65;           // 0x65
    u8 flag66;           // 0x66
    u8 active;           // 0x67 - init 1
    u32 field68;         // 0x68
    u32 field6C;         // 0x6C - composite vtable + 0x24
    u32 field70;         // 0x70 - composite vtable + 0xac
    CFileHandle* fileHandle;  // 0x74
    CScn* scene;              // 0x78
    u8 mSysWin[0x3C];         // 0x7C - CSysWin storage
    u8 flagB8;                // 0xB8
    u16 argBA;                // 0xBA
};

/*
 * System window "buff" process (singleton, created by create__11CSysWinBuffFv).
 *
 * A CProcess subclass whose layout mirrors CSysWinSave / CSystemWindow's
 * Move/UI region, plus a loaded-arc file handle, the owning scene, an embedded
 * CSysWin and two trailing scalars:
 *   0x00  CProcess primary base (destroyed via ~CProcess)
 *   0x3C  IUIWindow/Move region (48 bytes)
 *   0x6C  IWorkEvent vtable slot
 *   0x70  IScnRender vtable slot (long-lived render callback subobject)
 *   0x74  CFileHandle* loaded System.arc handle
 *   0x78  CScn* owning scene
 *   0x7C  CSysWin storage (0x3C bytes)
 *   0xB8  state flag byte
 *   0xBA  window-kind argument (u16)
 *
 * The ctor/dtor are emitted as global functions carrying the retail symbol
 * names (create__11CSysWinBuffFv / __dt__11CSysWinBuffFv), so the member dtor
 * declared here is satisfied at link time by that global definition (same
 * idiom as CSysWinSave / CSysWinScenarioLog).
 */
class CSysWinBuff : public CProcess {
public:
    virtual ~CSysWinBuff();
    void Init();
    void Term();
    void Move();
    void cbRenderBefore();
    static CSysWinBuff* getInstance();

    // 0x3C IUIWindow/Move region (48 bytes, same as CSysWinSave / CSystemWindow).
    u32 ptmf0[3];                 // 0x3C null pointer-to-member-function
    u32 ptmf1[3];                 // 0x48 null pointer-to-member-function
    u32 field_54;                 // 0x54
    u32 field_58;                 // 0x58
    u32 field_5C;                 // 0x5C
    s32 field_60;                 // 0x60 (-1)
    u8  field_64;                 // 0x64
    u8  field_65;                 // 0x65
    u8  field_66;                 // 0x66
    u8  field_67;                 // 0x67 (1)
    u32 field_68;                 // 0x68
    u32 mWorkEvent;               // 0x6C IWorkEvent vtable slot
    u32 mScnRender;               // 0x70 IScnRender vtable slot
    CFileHandle* mFileHandle;     // 0x74 loaded System.arc handle
    CScn* mScene;                 // 0x78 owning scene
    u8  mSysWin[0x3C];            // 0x7C CSysWin storage
    u8  mFlagB8;                  // 0xB8
    u16 mArgBA;                   // 0xBA
};

// Abstract view into the embedded CSysWin vtable: MWCC adds 2 implicit entries
// (offset-to-top / RTTI) before the first user virtual, so virtual index 32
// lands at vtable +0x88 (the layout-build dispatch Init performs). Pure
// abstract -> MWCC emits no vtable for this view. Same idiom as
// CSysWinSave::CSysWinView / CCollepedia::CSysWinProxy.
struct CSysWinView {
    virtual void v00() = 0;
    virtual void v01() = 0;
    virtual void v02() = 0;
    virtual void v03() = 0;
    virtual void v04() = 0;
    virtual void v05() = 0;
    virtual void v06() = 0;
    virtual void v07() = 0;
    virtual void v08() = 0;
    virtual void v09() = 0;
    virtual void v0A() = 0;
    virtual void v0B() = 0;
    virtual void v0C() = 0;
    virtual void v0D() = 0;
    virtual void v0E() = 0;
    virtual void v0F() = 0;
    virtual void v10() = 0;
    virtual void v11() = 0;
    virtual void v12() = 0;
    virtual void v13() = 0;
    virtual void v14() = 0;
    virtual void v15() = 0;
    virtual void v16() = 0;
    virtual void v17() = 0;
    virtual void v18() = 0;
    virtual void v19() = 0;
    virtual void v1A() = 0;
    virtual void v1B() = 0;
    virtual void v1C() = 0;
    virtual void v1D() = 0;
    virtual void v1E() = 0;
    virtual void v1F() = 0;
    virtual void v20() = 0;  // virtual index 32 -> vtable +0x88 - Init layout build
};

// Data body of the embedded CSysWin (0x3C bytes). Init rebuilds the storage by
// constructing a stack temp and memberwise-copying every field except the
// vtable word (+0) and the 0x29-0x2B padding, so MWCC emits the exact
// lwz/stw + lbz/stb sequence retail shows (CSysWinSave's copy also skips the
// vtable; this one additionally skips the 3 pad bytes retail leaves alone).
struct CSysWinDataBuff {
    u32 vtable;    // 0x00 (retained from construction, not copied)
    u32 f_04;      // 0x04 (UnkClass_8045F564 mem region, 4 words)
    u32 f_08;      // 0x08
    u32 f_0c;      // 0x0c
    u32 f_10;      // 0x10
    u32 f_14;      // 0x14 (mFileHandle)
    u32 f_18;      // 0x18 (mTagProcessor)
    u32 f_1c;      // 0x1c (mArcAccessor)
    u32 f_20;      // 0x20 (mLayout)
    u32 f_24;      // 0x24 (mAnimTrans)
    u8  f_28;      // 0x28 (ready flag)
    // 0x29-0x2B padding (not copied)
    u32 f_2c;      // 0x2c
    u32 f_30;      // 0x30
    u8  f_34;      // 0x34
    u8  f_35;      // 0x35
    u8  f_36;      // 0x36
    u8  f_37;      // 0x37
    u8  f_38;      // 0x38
    u8  f_39;      // 0x39
};

// Global data imports (MWCC does not mangle global-scope data names).
extern CSysWinBuff* lbl_eu_806648E0;  // singleton instance pointer (.sbss)
extern u32 lbl_eu_806648E4;           // loaded-FP / buff state word (.sbss)
extern u32 lbl_eu_806640E0;           // message-table word read by Move's label lookup
extern char lbl_eu_8052D238[];        // temp IUIWindow vtable (.data)
extern char lbl_eu_80537DC0[];        // CSysWinBuff composite vtable (.data)
extern char lbl_eu_8050EBC4[];        // shared string pool (.rodata)
extern u32 __ptmf_null[3];            // null pointer-to-member-function constant

// Struct view of the null pointer-to-member-function constant. Reading through
// named members keeps MWCC from folding the +0 access into a `lwz @l(rBase)`
// (retail materializes __ptmf_null once and uses plain base+disp loads).
struct PtmfNullWords {
    u32 w0;
    u32 w1;
    u32 w2;
};
extern PtmfNullWords __ptmf_null_words;

// C-ABI helper imports with retail verbatim (unmangled) names; the defining
// TUs (CSysWin.cpp / monolib / code_80135FDC.cpp) emit these flat symbols.
extern "C" {
void __ct__8CProcessFv(CProcess* self);
void __dt__8CProcessFv(CProcess* self, int flags);
void __ct__CSysWin(void* syswin, int arg);
void __dt__7CSysWinFv(void* syswin, int flags);
void func_8022B7F4(void* syswin);
void func_8022B7C8(void* syswin, nw4r::lyt::DrawInfo* drawInfo);
void getEntry__5CBdatFUl(u32 value);
void func_8003AA78__5CBdatFUlPv(u32 value, void* data);
void func_8003AA34();
void* getFP__FPCc(const char* name);
u8 func_8013B980();
u8 code80135FDC_getByte_64080();
void code80135FDC_postIncByte_64080();
s32 func_8029A658();
int func_8013BE50();
void setPresentationFlag__Q22cf13CfGameManagerFv(bool enable);

// CSysWin / system-window helper imports (C-linkage retail symbols, defined in
// CSysWin.cpp). func_8022BFC8 lives in CSysWin.hpp with a CSysWin* first arg.
u32 CSysWin_isReady(void* syswin);
int CSysWin_isActive(void* syswin);
void func_8022B9B4(void* syswin, void* str1, void* str2);
void func_8022B8B8(void* syswin);
void func_8022B8E4(void* syswin);
void func_8022B748(void* syswin);

// CDeviceFile common-archive loader + work-memory handle query (unmangled
// retail C symbols; declared with C linkage so the call reloc keeps the
// verbatim names, per the repo-wide convention).
void* readCommonArchiveFile__11CDeviceFileFUlPCcP10IWorkEventii(
    u32 handle, const char* path, void* workEvent, int a, int b);
u32 func_800A9D90();

// BDAT message lookup helpers (unmangled retail C symbols): func_80136254
// resolves a row id, func_8013639C resolves the message string for the id.
u32 func_80136254(void* obj, const char* key, u32 id);
char* func_8013639C(void* obj, const char* key, u16 id);
}

// C++-linkage helpers (MWCC mangles the plain names to the retail forms
// func_801390E0__FPP11CFileHandle / func_80137250__FPQ34nw4r3lyt8DrawInfo).
void func_801390E0(CFileHandle** handle);
void func_80137250(nw4r::lyt::DrawInfo* drawInfo);
