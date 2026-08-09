#pragma once

#include <types.h>

#include "monolib/work/CProcess.hpp"
#include "monolib/scn/IScnRender.hpp"
#include <nw4r/lyt/lyt_drawInfo.h>

// Minimal view of cf::CfPadData for func_801250FC (offsets match retail).
struct CSysPadData {
    u8  _00[0x4];
    u32 field_04;       // +0x4 CPad::mPressedButtonFlags
    u8  _08[0x104 - 8];
    u32 field_104;      // +0x104 mTurboPressButtonFlags
};

/*
 * System window "select" process (singleton, created by func_80125070).
 *
 * A CProcess subclass whose layout mirrors CSystemWindow's Move/UI region,
 * with an embedded CSysWin and a CCur18 cursor:
 *   0x00  CProcess primary base (real C++ base - destroyed via ~CProcess)
 *   0x3C  IUIWindow/Move region (48 bytes, same as CSystemWindow)
 *   0x6C  IWorkEvent vtable slot
 *   0x70  IScnRender vtable slot (long-lived render callback subobject)
 *   0x74  CScn* owning scene
 *   0x78  CSysWin storage (0x3C bytes)
 *   0xB4  CCur18 cursor storage (0x18 bytes)
 *
 * CSysWinSelect's ctor/dtor are emitted as global functions carrying the
 * retail symbol names (__ct__CSysWinSelect / __dt__13CSysWinSelectFv), so the
 * member dtor declared here is satisfied at link time by that global
 * definition (CMapSel idiom).
 */
class CSysWinSelect : public CProcess {
public:
    CSysWinSelect(void* a2, void* a3, void* a4, void* a5);
    virtual ~CSysWinSelect();
    void Init();
    void Term();
    void Move();
    void cbRenderBefore();

    // 0x3C IUIWindow/Move region (48 bytes, same as CSystemWindow).
    u32 ptmf0[3];      // 0x3C null pointer-to-member-function
    u32 ptmf1[3];      // 0x48 null pointer-to-member-function
    u32 field_54;      // 0x54
    u32 field_58;      // 0x58
    u32 field_5C;      // 0x5C
    s32 field_60;      // 0x60 (-1)
    u8  field_64;      // 0x64
    u8  field_65;      // 0x65
    u8  field_66;      // 0x66
    u8  field_67;      // 0x67 (1)
    u32 field_68;      // 0x68
    u32 mWorkEvent;    // 0x6C IWorkEvent vtable slot
    u32 mScnRender;    // 0x70 IScnRender vtable slot
    CScn* mScene;      // 0x74 owning scene
    u8  mSysWin[0x3C]; // 0x78 CSysWin storage
    u8  mCur18[0x18];  // 0xB4 CCur18 cursor storage
    u8  mState;        // 0xCC selection state (0..3)
    u8  mCursorSel;    // 0xCD cursor selection index (1 = default)
    char mStrA[0x80];  // 0xCE first string (a3)
    char mStrB[0x80];  // 0x14E second string (a4)
    char mStrC[0x80];  // 0x1CE third string (a5)
    u8  field_24E;     // 0x24E
};

// Render-callback and scene helper used by Term/cbRenderBefore (retail CScn
// member; defined in monolib/src/scn/CScn).
class CScn {
public:
    void removeRenderCB(IScnRender* cb);
    void addRenderCB(IScnRender* cb, u32 prio, u32 flag);
};

// Minimal CTaskGame decl (retail global-namespace class, symbols
// getInstance__9CTaskGameFv / func_800426F0__9CTaskGameFv).
class CTaskGame {
public:
    static CTaskGame* getInstance();
    static bool func_800426F0();
};

// Abstract view into the embedded CCur18 vtable (slot 3 at +0x0C is the
// cursor update virtual invoked by Term). Matches CSaveLoad::CCur18Obj.
class CCursor18 {
public:
    virtual void vf2(int) = 0;  // slot 2 (0x08)
    virtual void vf3() = 0;     // slot 3 (0x0C)
};

// Full CCur18 vtable view invoked by Init/Move/func_801250FC so MWCC emits a
// genuine `lwz r12, slot(r12)` virtual dispatch (FULL_MATCH regalloc). MWCC
// inserts an offset-to-top + RTTI prefix (2 entries), so vtable offset =
// (virtual index + 2) * 4.
class CCur18View {
public:
    virtual void vf02() = 0;       // index 0 -> +0x08 - Init
    virtual void vf03(void*) = 0;  // index 1 -> +0x0C (unused here)
    virtual void vf04(void*) = 0;  // index 2 -> +0x10 - Move, func_801250FC
};

// CSysWin vtable view exposing the layout-build virtual at +0x88 (slot 34)
// invoked by Init. Leading pure virtuals occupy inherited slots so the target
// lands at virtual index 32 (-> vtable +0x88 after the RTTI prefix).
class CSysWinView {
public:
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
    virtual void v20() = 0;  // index 32 -> +0x88 - Init
};

// Ctor/dtor and C-ABI helper imports. These retail symbols are C-linkage
// (unmangled), so they are declared `extern "C"`: C++ linkage would make MWCC
// append the __F<argtypes> suffix and publish a different reloc name.
extern "C" {
CSysWinSelect* __ct__CSysWinSelect(CSysWinSelect* self, void* a2, void* a3,
                                   void* a4, void* a5);
CSysWinSelect* __dt__13CSysWinSelectFv(CSysWinSelect* _this, int flags);
void func_8022B7F4(void* syswin);
void func_8022B7C8(void* syswin, nw4r::lyt::DrawInfo* drawInfo);
void func_801D20B0(void* cursor, nw4r::lyt::DrawInfo* drawInfo);
void func_80137250(nw4r::lyt::DrawInfo* drawInfo);
int func_8013BE50();
void* func_801355F4();
void func_80138078__FUl(u32 op);
void* getCfPadData__Q22cf13CfGameManagerFv();
int func_80086F9C__Q22cf13CfGameManagerFv(int arg);
u32 CSysWin_isReady(void* syswin);
int CSysWin_isActive(void* syswin);
void func_8022B90C(void* syswin, int arg);
void func_8022B9B4(void* syswin, void* str1, void* str2);
void func_8022BF6C(void* syswin, void* a, void* b);
void func_8022BFC8(void* syswin, int kind);
void func_8022B8B8(void* syswin);
void func_8022B8E4(void* syswin);
void func_8022B748(void* syswin);
void func_8022C1B4(void* out, void* syswin, unsigned char sel);
void func_801D216C(void* cur, int val);
void func_801D202C(void* cur);
void __ct__CSysWin(void* syswin, int arg);
void __ct__8CProcessFv(CProcess* self);
void* __ct__CCur18(void* cursor, void* accessor);
void __dt__7CSysWinFv(void* syswin, int flags);
void __dt__6CCur18Fv(void* cursor, int flags);
void __dt__8CProcessFv(CProcess* self, int flags);
}

// Global data imports (MWCC does not mangle global-scope data names).
extern CSysWinSelect* lbl_eu_80663FDC;  // singleton instance pointer (.sbss)
extern u32 lbl_eu_80663E28;             // cbRenderBefore flag (.sbss)

extern u32 __ptmf_null[3];   // null pointer-to-member-function constant
// IUIWindow base vtable (written at +0x10 before the __ptmf_null copy, then
// overwritten by lbl_eu_8052D278). Plain byte arrays so we can take &arr+off.
extern char lbl_eu_8052D238[];
extern char lbl_eu_8052D278[];