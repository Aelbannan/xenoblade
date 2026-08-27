#pragma once

#include <types.h>

#include "monolib/work/CProcess.hpp"
#include "monolib/work/IWorkEvent.hpp"
#include "monolib/lib/UnkClass_8045F564.hpp"
#include "monolib/scn/IScnRender.hpp"

#include <nw4r/lyt.h>

class CScn;

/*
 * CSimpleEveTalkWin - simple event-talk window process (singleton factory
 * func_801A20DC).
 *
 * REFACTORED to real bases: retail's 0x801A29B4/B4/C4 are compiler thunks
 * (subi r3,-off; b impl), not hand-written `self-0x6c` wrappers. The previous
 * plain-struct + manual `mProcHead/mVtable/mProcTail/mWorkEvent/mScnRender`
 * hid the inheritance and forced `((void(*)(void*))__dt__…)(self-off)` spoofs.
 *
 * Layout is now faithful: CProcess at 0x00 (vtable at 0x10), then the
 * intermediate storage that pushes the two interface bases to their retail
 * offsets, then the bases themselves. The intermediate storage is factored
 * into CSimpleEveTalkWinPre so the bases land at 0x6C/0x70 and the compiler
 * emits the thunks for us.
 *
 * Field layout (from ctor / Term / cbRenderBefore ASM):
 *   0x00  CProcess base (vtable at 0x10, written twice by ctor)
 *   0x3C  two null ptmf slots
 *   0x54  nw4r layout
 *   0x60  index, init -1
 *   0x67  active flag, init 1
 *   0x68  message id
 *   0x6C  IWorkEvent base (vtable = compVt+0x24) — thunk -0x6C
 *   0x70  IScnRender base (vtable = compVt+0xac) — thunks -0x70
 *   0x74  UnkClass_8045F564 region
 *   ...
 */
struct CSimpleEveTalkWinPre : CProcess {
    u32   ptmf0[3];                     // 0x3C null ptmf
    u32   ptmf1[3];                     // 0x48 null ptmf
    nw4r::lyt::Layout* mpLayout;        // 0x54
    u32   field_58;                     // 0x58
    u32   field_5C;                     // 0x5C
    s32   field_60;                     // 0x60 (init -1)
    u8    field_64;                     // 0x64
    u8    field_65;                     // 0x65
    u8    field_66;                     // 0x66
    u8    field_67;                     // 0x67 (init 1)
    u32   field_68;                     // 0x68 message id
    // 0x6C follows as base subobject
};

struct CSimpleEveTalkWin : CSimpleEveTalkWinPre, IWorkEvent, IScnRender {
    u8    mMemRegion[0x10];             // 0x74 UnkClass_8045F564 storage
    CScn* mScene;                       // 0x84 owning scene
    nw4r::lyt::AnimTransform* field_88; // 0x88 tag processor
    nw4r::lyt::AnimTransform* field_8C; // 0x8C anim 1
    nw4r::lyt::AnimTransform* field_90; // 0x90 anim 2
    nw4r::lyt::AnimTransform* field_94; // 0x94 anim 3
    u8    _98[0xA8 - 0x98];             // 0x98
    u8*   mMsgBuf;                      // 0xA8 message buffer
    u8    field_AC;                     // 0xAC ctor arg
    u8    field_AD;                     // 0xAD (init 1)
    u8    field_AE;                     // 0xAE (init 4)

    ~CSimpleEveTalkWin() override;
    void Init();
    void Term();
    void Move();
    void cbRenderBefore() override;
};

// --- Local opaque views used by Init / Move / func_801A2624 / func_801A2190.
// The retail objects these describe live in other TUs; only the vtable
// offsets and fields below are accessed from this one.

// CTagProcessorSE view: vtable+0x14 = message setup (two scales, message
// buffer, text id). The retail ctor is C-ABI (__ct__CTagProcessorSE, no
// class-length mangling) and the object is a raw 0x840-byte heap block, so
// this is a plain cast view, never constructed here.
//
// MWCC vtable note: a virtual dtor occupies THREE slots (0x00/0x04/0x08), so
// the dummy virtuals below are named by their ACTUAL vtable offset.
class CSimpleTagProc {
public:
    virtual ~CSimpleTagProc();                   // 0x00 (3 dtor slots)
    virtual void vfunc_0x0C();                   // 0x0C
    virtual void vfunc_0x10();                   // 0x10
    virtual void setMessage(float scaleA, float scaleB, u8* msgBuf,
                            u32 text);           // 0x14
};

// CTagProcessorSE field view for the early-init check fields.
struct CSimpleTagProcFields {
    u8  _pad0[0x814];
    u8  field_0x814;              // 0x814 layout-needed flag (== 1 -> early out)
    u8  _pad1[0x81A - 0x815];
    u8  field_0x81A;              // 0x81A
    u8  _pad2[0x81C - 0x81B];
    f32 field_0x81C;              // 0x81C
};

// findObjectById(id) result view (retail CfObject-ish "talk source"): the
// vtable+0x40 getter returns the talk text, and +0x98 is a voice sub-object
// whose vtable+0x58 plays/stops the character voice. Data members start at
// vptr+4 (standard C++ model), hence the 0x04-based pad.
class CSimpleTalkVoice {          // voice sub-object at src+0x98
public:
    virtual ~CSimpleTalkVoice();  // 0x00 (3 dtor slots)
    virtual void vfunc_0x0C();    // 0x0C
    virtual void vfunc_0x10();    // 0x10
    virtual void vfunc_0x14();    // 0x14
    virtual void vfunc_0x18();    // 0x18
    virtual void vfunc_0x1C();    // 0x1C
    virtual void vfunc_0x20();    // 0x20
    virtual void vfunc_0x24();    // 0x24
    virtual void vfunc_0x28();    // 0x28
    virtual void vfunc_0x2C();    // 0x2C
    virtual void vfunc_0x30();    // 0x30
    virtual void vfunc_0x34();    // 0x34
    virtual void vfunc_0x38();    // 0x38
    virtual void vfunc_0x3C();    // 0x3C
    virtual void vfunc_0x40();    // 0x40
    virtual void vfunc_0x44();    // 0x44
    virtual void vfunc_0x48();    // 0x48
    virtual void vfunc_0x4C();    // 0x4C
    virtual void vfunc_0x50();    // 0x50
    virtual void vfunc_0x54();    // 0x54
    virtual void play(int flag, int priority);  // 0x58
};

class CSimpleTalkSrc {            // findObjectById(id) result
public:
    virtual ~CSimpleTalkSrc();    // 0x00 (3 dtor slots)
    virtual void vfunc_0x0C();    // 0x0C
    virtual void vfunc_0x10();    // 0x10
    virtual void vfunc_0x14();    // 0x14
    virtual void vfunc_0x18();    // 0x18
    virtual void vfunc_0x1C();    // 0x1C
    virtual void vfunc_0x20();    // 0x20
    virtual void vfunc_0x24();    // 0x24
    virtual void vfunc_0x28();    // 0x28
    virtual void vfunc_0x2C();    // 0x2C
    virtual void vfunc_0x30();    // 0x30
    virtual void vfunc_0x34();    // 0x34
    virtual void vfunc_0x38();    // 0x38
    virtual void vfunc_0x3C();    // 0x3C
    virtual char* getText();      // 0x40 talk text getter
    u8  _pad[0x98 - 0x04];        // 0x04..0x97
    CSimpleTalkVoice* field_0x98; // 0x98 voice sub-object
};

// getFontInfo(1, layout) result view: vtable+0x24 returns the font handle
// handed to func_8013676C (CDeviceFont/root-pane font binding).
class CSimpleFontObj {
public:
    virtual ~CSimpleFontObj();    // 0x00 (3 dtor slots)
    virtual void vfunc_0x0C();    // 0x0C
    virtual void vfunc_0x10();    // 0x10
    virtual void vfunc_0x14();    // 0x14
    virtual void vfunc_0x18();    // 0x18
    virtual void vfunc_0x1C();    // 0x1C
    virtual void vfunc_0x20();    // 0x20
    virtual u32 getFontHandle();  // 0x24
};

// Minimal CScn decl (retail addRenderCB__4CScnFP10IScnRenderUlUl /
// removeRenderCB__4CScnFP10IScnRender).
class CScn {
public:
    void addRenderCB(IScnRender* cb, u32 prio, u32 flag);
    void removeRenderCB(IScnRender* cb);
};

// Minimal CTaskGame decl (retail getInstance__9CTaskGameFv /
// isFlag01Set__9CTaskGameFv).
class CTaskGame {
public:
    static CTaskGame* getInstance();
    static bool isFlag01Set();
};

// C-ABI imports (retail symbols are unmangled or pre-mangled; C linkage stops
// MWCC appending a __F<argtypes> suffix to the emitted reloc names).
extern "C" {
void __ct__8CProcessFv(CProcess* self);
void __dt__8CProcessFv(CProcess* self, int flags);
void __ct__17UnkClass_8045F564Fv(UnkClass_8045F564* self);
void __dt__17UnkClass_8045F564Fv(UnkClass_8045F564* self, int flags);
void deleteRegion__17UnkClass_8045F564Fv(UnkClass_8045F564* self);
void func_80136400(const char* src, u16* dst, u32 destLen);
// arg1 is the singleton pointer left in r3 by the caller's flag test (retail
// does not set r3 for the call).
void func_801A2190(CSimpleEveTalkWin* owner, u32 textId, u8* msgBuf);
void func_801A2624(CSimpleEveTalkWin* self);
void __ct__Q34nw4r3lyt8DrawInfoFv(nw4r::lyt::DrawInfo* drawInfo);
void __dt__Q34nw4r3lyt8DrawInfoFv(nw4r::lyt::DrawInfo* drawInfo, int flags);
void* __ct__CTagProcessorSE(void* self);  // returns this (r3 survives)
void __ct__14Class_8045F858FP17UnkClass_8045F564(void* self,
                                                  UnkClass_8045F564* base);
void __dt__14Class_8045F858Fv(void* self, int flags);
void func_80135464(u8 a, int b, float c, float d, float e);
int func_8013BE50();
bool isInitialized__10CMenuPauseFv();
int isClassicController__Q22cf13CfGameManagerFv(int arg);
void* getFontInfo__11CDeviceFontFUlPQ34nw4r3lyt6Layout(
    u32, nw4r::lyt::Layout*);
nw4r::lyt::ArcResourceAccessor* func_801355F4();
int func_801286E0(void* tagProc, nw4r::lyt::Pane* pane);
int func_80127FB4(void* tagProc, nw4r::lyt::Pane* pane);
int func_80128740(void* tagProc, nw4r::lyt::Pane* pane);
void func_801287BC(void* tagProc, nw4r::lyt::Pane* pane, int flag);
}

// C++-linkage import: retail symbol is the mangled findObjectById__Fi.
void* findObjectById(int id);

// Global data imports (MWCC does not mangle global-scope data names).
extern CSimpleEveTalkWin* lbl_eu_80664320;  // singleton instance (.sbss)
extern u8 lbl_eu_80664324;                  // Term flag (.sbss)
extern u32 lbl_eu_80663E28;                 // cbRenderBefore flag (.sbss)
extern u32 __ptmf_null[3];                  // null pointer-to-member-function
extern char lbl_eu_8052D238[];              // temp base vtable
extern char lbl_eu_80532EE0[];              // composite vtable
extern char lbl_eu_80503E14[];              // rodata string blob
extern const f32 lbl_eu_80667CA0;           // tag-proc message scales
extern const f32 lbl_eu_80667CA4;           // early-init float
extern const f32 lbl_eu_80667CA8;           // anim frame
extern const f32 lbl_eu_80667CAC;           // translate Y
extern const f32 lbl_eu_80667CB0;           // anim frame target
