#pragma once

#include <types.h>

#include "monolib/work/CProcess.hpp"
#include "monolib/lib/UnkClass_8045F564.hpp"
#include "monolib/scn/IScnRender.hpp"
#include <nw4r/lyt.h>

/*
 * CTalkWindow - talk-window process (created by func_8012CC78 on the work
 * heap, registered as a CProcess under the caller's parent; sizeof 0xB8).
 *
 * Field layout (from the ctor / Term / cbRenderBefore ASM):
 *   0x00  CProcess storage (vtable at +0x10)
 *   0x3C  two null pointer-to-member-function callback slots
 *   0x54  nw4r layout (drawn by cbRenderBefore, deleted by Term)
 *   0x5C  tag-processor object (0x858 heap block, viewed as AnimTransform*)
 *   0x68  talk-source id (fed to findObjectById in cbRenderBefore)
 *   0x70  IScnRender vtable slot (render callback, passed to CScn)
 *   0x74  owning CScn
 *   0x78  UnkClass_8045F564 layout-build region (raw buffer: retail drives
 *         its ctor/dtor via C-linkage calls)
 *   0x88  5 layout animations (built by Init; page-state machine in Move)
 *   0x9C  message buffer pointer (ctor arg)
 *   0xA4  page index (pane-name table lookup into lbl_eu_8052DF70)
 *   0xB0  state/mode (1..5 dispatch in Move)
 *   0xB4  talk-enable flag (gates the character-voice logic)
 *
 * Deliberately NON-polymorphic (plain struct, CSystemWindow / CSysWinSave
 * idiom): the CProcess base and the UnkClass_8045F564 region are destroyed
 * by hand in the dtor, emitted as the extern "C" global __dt__11CTalkWindowFv
 * (CSysWinSave idiom - a member dtor cannot express the flags delete-check
 * placement inside the outer null-guard).
 */
struct CTalkWindow {
    u8    mProcHead[0x10];              // 0x00 CProcess storage head
    u32   mVtable;                      // 0x10 CProcess vtable slot
    u8    mProcTail[0x28];              // 0x14 CProcess storage tail
    u32   ptmf0[3];                     // 0x3C null pointer-to-member-function
    u32   ptmf1[3];                     // 0x48 null pointer-to-member-function
    nw4r::lyt::Layout* mpLayout;        // 0x54
    u32   field_58;                     // 0x58
    nw4r::lyt::AnimTransform* field_5C; // 0x5C
    u32   field_60;                     // 0x60 (init -1)
    u8    field_64;                     // 0x64
    u8    field_65;                     // 0x65
    u8    field_66;                     // 0x66
    u8    field_67;                     // 0x67 (init 1)
    u32   field_68;                     // 0x68 talk-source id
    u32   field_6C;                     // 0x6C
    u32   mScnRender;                   // 0x70 IScnRender vtable slot
    CScn* mScene;                       // 0x74 owning scene
    u8    mMemRegion[0x10];             // 0x78 UnkClass_8045F564 storage (raw)
    nw4r::lyt::AnimTransform* field_88; // 0x88 layout animation 1
    nw4r::lyt::AnimTransform* field_8C; // 0x8C layout animation 2
    nw4r::lyt::AnimTransform* field_90; // 0x90 layout animation 3
    nw4r::lyt::AnimTransform* field_94; // 0x94 layout animation 4
    nw4r::lyt::AnimTransform* field_98; // 0x98 layout animation 5
    u32   field_9C;                     // 0x9C message buffer pointer (ctor arg)
    u8    field_A0;                     // 0xA0
    u8    field_A1;                     // 0xA1
    u8    _A2[2];                       // 0xA2
    u32   field_A4;                     // 0xA4 page index (pane-name table)
    u32   field_A8;                     // 0xA8
    u32   field_AC;                     // 0xAC
    u32   field_B0;                     // 0xB0 state/mode (1..5 dispatch in Move)
    u8    field_B4;                     // 0xB4 talk-enable flag
    u8    _B5[3];                       // 0xB5 pad to size 0xB8

    void Init();
    void Term();
    void cbRenderBefore();
    void Move();
};

// Render-callback and scene helper used by Term/cbRenderBefore (retail CScn
// member; defined in monolib/src/scn/CScn). Guarded so the kyoshin task
// headers can be included together in one TU.
#ifndef KYOSHIN_MINIMAL_CSCN_DECLARED
#define KYOSHIN_MINIMAL_CSCN_DECLARED
class CScn {
public:
    void removeRenderCB(IScnRender* cb);
    void addRenderCB(IScnRender* cb, u32 prio, u32 flag);
};
#endif

// Minimal CTaskGame decl (retail getInstance__9CTaskGameFv /
// isFlag01Set__9CTaskGameFv).
class CTaskGame {
public:
    static CTaskGame* getInstance();
    static bool isFlag01Set();
};

// Talk-window views of the talk-source objects (func_800BBC0C result). The
// character voice sub-object at +0x98 matches CSimpleTalkVoice in
// CSimpleEveTalkWin.hpp (play at vtable 0x58); the message-state object at
// +0xC4 carries the page flags and is fed to func_8004C5EC / func_8004B9D4.
class CTalkWinVoice {
public:
    virtual ~CTalkWinVoice();  // 0x00 (3 dtor slots)
    virtual void vfunc_0x0C(); // 0x0C
    virtual void vfunc_0x10(); // 0x10
    virtual void vfunc_0x14(); // 0x14
    virtual void vfunc_0x18(); // 0x18
    virtual void vfunc_0x1C(); // 0x1C
    virtual void vfunc_0x20(); // 0x20
    virtual void vfunc_0x24(); // 0x24
    virtual void vfunc_0x28(); // 0x28
    virtual void vfunc_0x2C(); // 0x2C
    virtual void vfunc_0x30(); // 0x30
    virtual void vfunc_0x34(); // 0x34
    virtual void vfunc_0x38(); // 0x38
    virtual void vfunc_0x3C(); // 0x3C
    virtual void vfunc_0x40(); // 0x40
    virtual void vfunc_0x44(); // 0x44
    virtual void vfunc_0x48(); // 0x48
    virtual void vfunc_0x4C(); // 0x4C
    virtual void vfunc_0x50(); // 0x50
    virtual void vfunc_0x54(); // 0x54
    virtual void play(int flag, int priority); // 0x58
};

struct CTalkWinTalkC4 {
    u8  _00[0x270];      // 0x00
    u32 field_270;       // 0x270 (talk-stop flag bit 0x80)
};

struct CTalkWinTalkSrc {
    u8  _00[0x98];                    // 0x00 vtable + data
    CTalkWinVoice* mVoice;            // 0x98 character voice
    u8  _9C[0xC4 - 0x9C];             // 0x9C
    CTalkWinTalkC4* mTalkC4;          // 0xC4 message state
};

// Talk-window anim frame slot: the retail talk-window AnimTransform variant
// keeps the current frame at +0x10 (the nw4r header does not declare it).
struct CTalkAnimFrame {
    u8  _00[0x10];   // 0x00 vtable + resource data
    f32 mFrame;      // 0x10
};

// Minimal pad-data view for func_8012DA6C (only the pressed-flag word at +0x4
// is read; see CSysPadData in CSysWinSave.hpp).
struct CTalkPadView {
    u8  _00[0x4];
    u32 field_04;   // +0x4 CPad::mPressedButtonFlags
};

// Position components read from the talk-source vfn120(name) result
// (0x10-byte stride: +0x0C / +0x1C / +0x2C).
struct CTalkWinPosObj {
    u8  _00[0x0C];
    f32 field_0x0C; // +0x0C
    u8  _10[0x1C - 0x10];
    f32 field_0x1C; // +0x1C
    u8  _20[0x2C - 0x20];
    f32 field_0x2C; // +0x2C
};

// findObjectById(id) result view for Init / func_8012CD38: dispatched virtuals
// at +0x40 (talk text), +0xAC (position fallback), +0x120 (named-position
// lookup) and +0x158 (voice stop). Field +0x8C carries the case-4 message id.
// Never instantiated - cast view only, so no vtable is emitted.
class CTalkWinSrc {
public:
    virtual ~CTalkWinSrc();            // 0x00 (3 dtor slots)
    virtual void vfunc_0x0C();         // 0x0C
    virtual void vfunc_0x10();         // 0x10
    virtual void vfunc_0x14();         // 0x14
    virtual void vfunc_0x18();         // 0x18
    virtual void vfunc_0x1C();         // 0x1C
    virtual void vfunc_0x20();         // 0x20
    virtual void vfunc_0x24();         // 0x24
    virtual void vfunc_0x28();         // 0x28
    virtual void vfunc_0x2C();         // 0x2C
    virtual void vfunc_0x30();         // 0x30
    virtual void vfunc_0x34();         // 0x34
    virtual void vfunc_0x38();         // 0x38
    virtual void vfunc_0x3C();         // 0x3C
    virtual char* getText();           // 0x40 talk text
    virtual void vfunc_0x44();         // 0x44
    virtual void vfunc_0x48();         // 0x48
    virtual void vfunc_0x4C();         // 0x4C
    virtual void vfunc_0x50();         // 0x50
    virtual void vfunc_0x54();         // 0x54
    virtual void vfunc_0x58();         // 0x58
    virtual void vfunc_0x5C();         // 0x5C
    virtual void vfunc_0x60();         // 0x60
    virtual void vfunc_0x64();         // 0x64
    virtual void vfunc_0x68();         // 0x68
    virtual void vfunc_0x6C();         // 0x6C
    virtual void vfunc_0x70();         // 0x70
    virtual void vfunc_0x74();         // 0x74
    virtual void vfunc_0x78();         // 0x78
    virtual void vfunc_0x7C();         // 0x7C
    virtual void vfunc_0x80();         // 0x80
    virtual void vfunc_0x84();         // 0x84
    virtual void vfunc_0x88();         // 0x88
    virtual void vfunc_0x8C();         // 0x8C
    virtual void vfunc_0x90();         // 0x90
    virtual void vfunc_0x94();         // 0x94
    virtual void vfunc_0x98();         // 0x98
    virtual void vfunc_0x9C();         // 0x9C
    virtual void vfunc_0xA0();         // 0xA0
    virtual void vfunc_0xA4();         // 0xA4
    virtual void vfunc_0xA8();         // 0xA8
    virtual nw4r::math::VEC3* vfnAC();          // 0xAC position fallback
    virtual void vfunc_0xB0();         // 0xB0
    virtual void vfunc_0xB4();         // 0xB4
    virtual void vfunc_0xB8();         // 0xB8
    virtual void vfunc_0xBC();         // 0xBC
    virtual void vfunc_0xC0();         // 0xC0
    virtual void vfunc_0xC4();         // 0xC4
    virtual void vfunc_0xC8();         // 0xC8
    virtual void vfunc_0xCC();         // 0xCC
    virtual void vfunc_0xD0();         // 0xD0
    virtual void vfunc_0xD4();         // 0xD4
    virtual void vfunc_0xD8();         // 0xD8
    virtual void vfunc_0xDC();         // 0xDC
    virtual void vfunc_0xE0();         // 0xE0
    virtual void vfunc_0xE4();         // 0xE4
    virtual void vfunc_0xE8();         // 0xE8
    virtual void vfunc_0xEC();         // 0xEC
    virtual void vfunc_0xF0();         // 0xF0
    virtual void vfunc_0xF4();         // 0xF4
    virtual void vfunc_0xF8();         // 0xF8
    virtual void vfunc_0xFC();         // 0xFC
    virtual void vfunc_0x100();        // 0x100
    virtual void vfunc_0x104();        // 0x104
    virtual void vfunc_0x108();        // 0x108
    virtual void vfunc_0x10C();        // 0x10C
    virtual void vfunc_0x110();        // 0x110
    virtual void vfunc_0x114();        // 0x114
    virtual void vfunc_0x118();        // 0x118
    virtual void vfunc_0x11C();        // 0x11C
    virtual CTalkWinPosObj* vfn120(const char* name);  // 0x120 named position
    virtual void vfunc_0x124();        // 0x124
    virtual void vfunc_0x128();        // 0x128
    virtual void vfunc_0x12C();        // 0x12C
    virtual void vfunc_0x130();        // 0x130
    virtual void vfunc_0x134();        // 0x134
    virtual void vfunc_0x138();        // 0x138
    virtual void vfunc_0x13C();        // 0x13C
    virtual void vfunc_0x140();        // 0x140
    virtual void vfunc_0x144();        // 0x144
    virtual void vfunc_0x148();        // 0x148
    virtual void vfunc_0x14C();        // 0x14C
    virtual void vfunc_0x150();        // 0x150
    virtual void vfunc_0x154();        // 0x154
    virtual void vfn158(u32 flag);     // 0x158 voice stop
    u8  _04[0x8C - 0x04];
    u16 field_0x8C;                    // 0x8C message id (case 4)
};

// CTagProcessor field view for Init's early-init switch: vtable+0x14 is the
// message-setup call, +0x814..+0x81C the mode/flag/message fields.
class CTalkWinTagProc {
public:
    virtual ~CTalkWinTagProc();        // 0x00 (3 dtor slots)
    virtual void vfunc_0x0C();         // 0x0C
    virtual void vfunc_0x10();         // 0x10
    virtual void setMessage(f32 scaleA, f32 scaleB, u8* msgBuf, u32 text);  // 0x14
    u8  _04[0x814 - 0x04];   // 0x04..0x813 (vptr at 0x0)
    u8  field_0x814;                   // 0x814 switch type (0..8)
    u8  field_0x815;                   // 0x815 option bits (Init reads bits 0/1/2)
    u8  field_0x816;                   // 0x816 page index -> field_A4
    u8  field_0x817;                   // 0x817 page-loop flag -> field_A8
    u16 field_0x818;                   // 0x818 message id (cases 1/5)
    u8  field_0x81A;                   // 0x81A case-0 flag
    u8  _81B;                          // 0x81B
    f32 field_0x81C;                   // 0x81C case-0 scale
};

// nw4r Pane trailing-region view: the talk-window layout panes carry a flag
// byte at +0xBB (mFlag) and, for the 0xf1 pane, a tag-processor pointer at
// +0xF8. Custom virtuals at 0x74/0x78 are dispatched by Init (results
// discarded). Never instantiated - cast view only.
class CTalkWinPane {
public:
    virtual ~CTalkWinPane();           // 0x00 (3 dtor slots)
    virtual void vfunc_0x0C();         // 0x0C
    virtual void vfunc_0x10();         // 0x10
    virtual void vfunc_0x14();         // 0x14
    virtual void vfunc_0x18();         // 0x18
    virtual void vfunc_0x1C();         // 0x1C
    virtual void vfunc_0x20();         // 0x20
    virtual void vfunc_0x24();         // 0x24
    virtual void vfunc_0x28();         // 0x28
    virtual void vfunc_0x2C();         // 0x2C
    virtual void vfunc_0x30();         // 0x30
    virtual void vfunc_0x34();         // 0x34
    virtual void vfunc_0x38();         // 0x38
    virtual void vfunc_0x3C();         // 0x3C
    virtual void vfunc_0x40();         // 0x40
    virtual void vfunc_0x44();         // 0x44
    virtual void vfunc_0x48();         // 0x48
    virtual void vfunc_0x4C();         // 0x4C
    virtual void vfunc_0x50();         // 0x50
    virtual void vfunc_0x54();         // 0x54
    virtual void vfunc_0x58();         // 0x58
    virtual void vfunc_0x5C();         // 0x5C
    virtual void vfunc_0x60();         // 0x60
    virtual void vfunc_0x64();         // 0x64
    virtual void vfunc_0x68();         // 0x68
    virtual void vfunc_0x6C();         // 0x6C
    virtual void vfunc_0x70();         // 0x70
    virtual void vfunc_0x74(u32 a);    // 0x74
    virtual void vfunc_0x78();         // 0x78
    u8  _04[0xBB - 0x04];    // 0x04..0xBA (vptr at 0x0)
    u8  mFlag;                         // 0xBB
    u8  _BC[0xF8 - 0xBC];    // 0xBC..0xF7
    u32 field_F8;                      // 0xF8 tag-processor pointer
};

// getFontInfo(1, layout) result view: vtable+0x24 returns the font handle
// handed to func_8013676C.
class CTalkFontObj {
public:
    virtual ~CTalkFontObj();           // 0x00 (3 dtor slots)
    virtual void vfunc_0x0C();         // 0x0C
    virtual void vfunc_0x10();         // 0x10
    virtual void vfunc_0x14();         // 0x14
    virtual void vfunc_0x18();         // 0x18
    virtual void vfunc_0x1C();         // 0x1C
    virtual void vfunc_0x20();         // 0x20
    virtual u32 getFontHandle();       // 0x24
};

// Opaque func_80496264(scene, -1) pose block passed to func_8049B59C.
struct CTalkWinPose;

// C-ABI function imports: these retail symbols are unmangled (or carry a
// pre-mangled name), so they must be declared `extern "C"` to stop MWCC
// appending a __F<argtypes> suffix to the emitted reloc names.
extern "C" {
void __ct__8CProcessFv(CProcess* self);
void __dt__8CProcessFv(CProcess* self, int flags);
void __ct__17UnkClass_8045F564Fv(UnkClass_8045F564* self);
void __dt__17UnkClass_8045F564Fv(UnkClass_8045F564* self, int flags);
void func_80136400(const char* src, u16* dst, u32 destLen);
int func_8013BE50();
void __ct__Q34nw4r3lyt8DrawInfoFv(nw4r::lyt::DrawInfo* drawInfo);
void __dt__Q34nw4r3lyt8DrawInfoFv(nw4r::lyt::DrawInfo* drawInfo, int flags);
void func_8012CD38(CTalkWindow* self);
void func_8012D8C0(CTalkWindow* self);
void func_8012DA6C(CTalkWindow* self);
void func_8012D3D8(CTalkWindow* self);
int code80135FDC_getByte_64058();
int code80135FDC_getByte_64059();
int func_8013BF78();
int isClassicController__Q22cf13CfGameManagerFv(int arg);
CTalkWinTalkSrc* func_800BBC0C();
int func_8004C5EC(CTalkWinTalkC4* talkC4);
void func_8004B9D4(CTalkWinTalkC4* talkC4, int a, int b, int c, int d);
int func_801276F4(nw4r::lyt::AnimTransform* tag, nw4r::lyt::Pane* a,
                  nw4r::lyt::Pane* b, nw4r::lyt::Pane* c);
int func_8012615C(nw4r::lyt::AnimTransform* tag, nw4r::lyt::Pane* a,
                  nw4r::lyt::Pane* b, nw4r::lyt::Pane* c);
void func_80127E74(nw4r::lyt::AnimTransform* tag, nw4r::lyt::Pane* a,
                   nw4r::lyt::Pane* b, nw4r::lyt::Pane* c);
void func_80135464(u8 a, int b, float c, float d, float e);
void func_8013DA60(u16 id, int a, int b);
void func_8013E104(u16 id);
void func_8013E204(u16 id);
void func_801342B0();
nw4r::lyt::ArcResourceAccessor* func_801355F4();
char* func_80138DA4(const char* msg);
void func_80136B4C(nw4r::lyt::Layout* layout, const char* name, const char* text,
                   u32 flag);
void func_80137E7C(nw4r::lyt::Layout* layout, const char* name, void* res);
void func_80127764(void* tagProc, nw4r::lyt::Pane* a, nw4r::lyt::Pane* b,
                    nw4r::lyt::Pane* c, int flag);
void func_8013676C(nw4r::lyt::Pane* rootPane, u32 fontHandle);
void* __ct__CTagProcessor(void* self);
u32 getAllocHandle__10CLibLayoutFv();
extern "C" void* getFontInfo__11CDeviceFontFUlPQ34nw4r3lyt6Layout(
    u32 arg, nw4r::lyt::Layout* layout);
void buildLayout__FPPQ34nw4r3lyt6LayoutPQ34nw4r3lyt19ArcResourceAccessorPCc(
    nw4r::lyt::Layout** ppLayout, nw4r::lyt::ArcResourceAccessor* accessor,
    const char* name);
void bindLayoutAnimTransform__FPQ34nw4r3lyt6LayoutPPQ34nw4r3lyt13AnimTransformPQ34nw4r3lyt19ArcResourceAccessorPc(
    nw4r::lyt::Layout* layout, nw4r::lyt::AnimTransform** ppAnimTrans,
    nw4r::lyt::ArcResourceAccessor* accessor, char* name);
CTalkWinPose* func_80496264(CScn* scene, int index);
void func_8049B59C(nw4r::math::VEC3* out, CTalkWinPose* pose,
                   const nw4r::math::VEC3* in);
}

// C++-linkage imports (retail emits the mangled forms).
void func_80137250(nw4r::lyt::DrawInfo* drawInfo);
void drawLayout(nw4r::lyt::Layout* layout, nw4r::lyt::DrawInfo* drawInfo,
                   int a, int b);
void* findObjectById(int id);
u32 advanceAnimTransform(nw4r::lyt::AnimTransform* anim, float frame);
void playUISound(unsigned long id);

// The CTalkWindow ctor is a C-ABI global (retail `__ct__CTalkWindow`, no
// class-length mangling); defined in CTalkWindow.cpp - the factory
// func_8012CC78 forwards into it by this name.
extern "C" CTalkWindow* __ct__CTalkWindow(CTalkWindow* self, u32 arg1,
                                          u32 arg2, u8* buf, u32 arg3,
                                          u32 arg4, u32 arg5);

// Global data imports (MWCC does not mangle global-scope data names).
extern u32 lbl_eu_80663E28;    // cbRenderBefore flag (.sbss)
extern u32 __ptmf_null[3];     // null pointer-to-member-function constant
extern char lbl_eu_8052D238[]; // temp base vtable
extern "C" unsigned char __data_blob[];
extern "C" unsigned char __rodata_blob[];
extern "C" unsigned char __sdata_blob[];
extern "C" unsigned char __sdata2_blob[];
extern "C" unsigned char __bss_blob[];
extern "C" unsigned char __sbss_blob[];

#define lbl_eu_8052DFA8 ((char*)&__data_blob[0x38])
#define lbl_eu_8052E068 ((char*)&__data_blob[0xF8])
#define lbl_eu_8052E0A8 ((char*)&__data_blob[0x138])
#define lbl_eu_804FFCA4 ((char*)&__rodata_blob[0x0C])
#define lbl_eu_8052DF70 ((const char**)&__data_blob[0x00])
#define lbl_eu_80662170 ((char*)&__sdata_blob[0x00])
#define lbl_eu_80667268 ((char*)&__sdata2_blob[0x00])
#define lbl_eu_80667270 ((char*)&__sdata2_blob[0x08])
#define lbl_eu_80667278 (*((f32*)&__sdata2_blob[0x10]))
#define lbl_eu_8066727C (*((f32*)&__sdata2_blob[0x14]))
#define lbl_eu_80667280 (*((f32*)&__sdata2_blob[0x18]))
#define lbl_eu_80667284 (*((f32*)&__sdata2_blob[0x1C]))
#define lbl_eu_80667288 (*((f32*)&__sdata2_blob[0x20]))
#define lbl_eu_8066728C (*((f32*)&__sdata2_blob[0x24]))
#define lbl_eu_80667290 (*((f32*)&__sdata2_blob[0x28]))
#define lbl_eu_80667294 (*((f32*)&__sdata2_blob[0x2C]))
#define lbl_eu_80667298 (*((f32*)&__sdata2_blob[0x30]))
#define lbl_eu_8066729C (*((f32*)&__sdata2_blob[0x34]))
#define lbl_eu_806672A0 (*((f32*)&__sdata2_blob[0x38]))
#define lbl_eu_806672A4 (*((f32*)&__sdata2_blob[0x3C]))
#define lbl_eu_806672A8 (*((f32*)&__sdata2_blob[0x40]))
#define lbl_eu_806672AC (*((f32*)&__sdata2_blob[0x44]))
#define lbl_eu_806672B0 (*((f32*)&__sdata2_blob[0x48]))
#define lbl_eu_806672B4 (*((f32*)&__sdata2_blob[0x4C]))
#define lbl_eu_806672B8 (*((f32*)&__sdata2_blob[0x50]))
#define lbl_eu_806672BC (*((f32*)&__sdata2_blob[0x54]))
#define lbl_eu_806672C0 (*((f32*)&__sdata2_blob[0x58]))
#define lbl_eu_806672C4 (*((f32*)&__sdata2_blob[0x5C]))
#define lbl_eu_80573A70 ((f32*)&__bss_blob[0x00])
#define lbl_eu_80664040 (*((u8*)&__sbss_blob[0x00]))
#define lbl_eu_80664044 (*((u32*)&__sbss_blob[0x04]))
// func_8012CD38 projection / clamp constants.
