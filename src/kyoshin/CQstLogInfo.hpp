#pragma once

#include <types.h>
#include <nw4r/lyt.h>

#include "monolib/lib/UnkClass_8045F564.hpp"

// Shared data pools (retail names; global scope - no C++ mangling).
// const float: MWCC hoists the SDA21 load into the prologue (pos 2) like
// retail; a plain extern float schedules the load late (MWCC_REFERENCE:8787).
extern const float lbl_eu_80668590;   // animation-completion sentinel (.sdata2)
extern char lbl_eu_8050A0B4[];  // quest-log string pool (split1 .rodata)
extern char* lbl_eu_80536320[];  // quest-name table (30 entries, split1 .data)

// IWorkEvent-compatible vtable for CQstLogInfo (split1 .data; US/EU-stripped
// name of __vt__11CQstLogInfo). Stored via a base-class ctor so the vtable
// store lands before the member ctor (retail ctor order; see CBattery.cpp).
extern void* lbl_eu_80536478[];

// Base ctor runs first: sets mVtbl = lbl_eu_80536478, then the mMemRegion
// member ctor, then the body field-init (CBatteryVtblBase pattern).
struct CQstLogInfoVtblBase {
    void* mVtbl; // 0x0 - lbl_eu_80536478

    CQstLogInfoVtblBase() {
        mVtbl = lbl_eu_80536478;
    }
};

struct CFileHandle;
struct CEventFile;

// C-linkage imports used by func_80229510 (retail names are unmangled).
extern "C" u32 func_80138138(u16);
extern "C" u32 func_800A9D90();
// Random / name-table helpers (retail unmangled).
extern "C" u32 func_8009CF8C(u32);
extern u32 lbl_eu_80573D18[];  // quest-name table (split1 .rodata)
// Pane helpers (retail unmangled).
extern "C" void func_80137F88(nw4r::lyt::Pane*, const char*);
extern "C" void func_80124270(nw4r::lyt::Pane*, u32);

// BDAT text / name helpers (retail unmangled).
extern "C" u16 func_80136254(const void*, const void*, int);
extern "C" u32 func_801392E4(u32);
extern "C" u32 func_80139358(u32);
extern "C" void* func_80138DA4(const char*);
extern "C" char* func_801355BC();
extern "C" void* func_801412D0(u32);
extern "C" u32 func_80140854(void*, u32, u32);
// Font-info lookup (retail pre-mangled name).
extern "C" void* func_80452C10__11CDeviceFontFUlPQ34nw4r3lyt6Layout(u32, nw4r::lyt::Layout*);
// Layout-build helpers (retail unmangled).
extern "C" nw4r::lyt::ArcResourceAccessor* createArcResourceAccessor__10CLibLayoutFv();
extern "C" void* __ct__CTagProcessor(void*);
// BDAT manager (C-linkage free function; the CBdat:: form would mangle).
extern "C" void* func_8003AA34();

// BDAT table handles used by the quest-log sub-updates (.sbss).
extern u32 lbl_eu_806640D8;
extern u32 lbl_eu_806640EC;
extern u32 lbl_eu_80664104;
// Quest-category name table indexed by func_80138138 (split1 .data).
extern char* lbl_eu_80536398[];

// View into a nw4r::lyt::Pane's trailing user-data region: OnFileEvent stores
// the tag-processor pointer (field_0x2C) into each bound pane at +0xF8.
struct CQstLogInfoPaneUser {
    u8 pad[0xF8];
    u32 field_F8;
};

// Mirror of the font object returned by CDeviceFont::func_80452C10: vtable
// slot 9 (+0x24) yields the u32 bound into the layout's font pane. Never
// instantiated (all pure virtual) so no vtable is emitted.
struct CQstLogInfoFontView {
    virtual void sf2() = 0;
    virtual void sf3() = 0;
    virtual void sf4() = 0;
    virtual void sf5() = 0;
    virtual void sf6() = 0;
    virtual void sf7() = 0;
    virtual void sf8() = 0;
    virtual u32 sf9() = 0; // vtable offset 0x24
};

class CQstLogInfo;

// Same-TU helpers called by func_80229570 etc. (retail names unmangled).
// noinline: retail callers emit real `bl` branches - leaf bodies would
// otherwise be inlined away by MWCC. extern "C" binds the call relocs to the
// unmangled retail symbol names.
extern "C" __declspec(noinline) void func_80229770(CQstLogInfo* self);
extern "C" __declspec(noinline) void func_802297BC(CQstLogInfo* self);
extern "C" __declspec(noinline) void func_80229808(CQstLogInfo* self);
extern "C" __declspec(noinline) void func_80229854(CQstLogInfo* self);
extern "C" __declspec(noinline) void func_802298A0(CQstLogInfo* self);
extern "C" __declspec(noinline) void func_80229900(CQstLogInfo* self);
// Quest-log sub-widget updates (called from func_80229960).
extern "C" __declspec(noinline) void func_80229A0C(CQstLogInfo* self,
    const char* table, u32 key, u8 v);
extern "C" __declspec(noinline) void func_80229B54(CQstLogInfo* self,
    const char* table, u32 key);
extern "C" __declspec(noinline) void func_80229CA0(CQstLogInfo* self,
    const char* table, u32 key);
extern "C" __declspec(noinline) void func_80229CF0(CQstLogInfo* self,
    const char* table, u32 key, u8 v);
extern "C" __declspec(noinline) void func_8022A904(CQstLogInfo* self,
    const char* table, u32 key, u8 v);
extern "C" __declspec(noinline) void func_8022AFF8(CQstLogInfo* self);

class CQstLogInfo : public CQstLogInfoVtblBase {
public:
    CQstLogInfo();
    ~CQstLogInfo();
    int OnFileEvent(CEventFile* event);

    u8 func_802296D0();
    u8 func_802296D8();
    void func_80229768(u16 val);

    UnkClass_8045F564 mMemRegion;             // 0x04 - scratch region (0x10 bytes)
    CFileHandle* mFileHandle;                 // 0x14 - loaded quest-log arc handle
    CFileHandle* field_0x18;                  // 0x18 - common-archive handle
    u32 field_0x1C;                           // 0x1C - ArcResourceAccessor* (raw)
    nw4r::lyt::Layout* mUnk20;                // 0x20
    nw4r::lyt::AnimTransform* field_0x24;     // 0x24
    nw4r::lyt::AnimTransform* field_0x28;     // 0x28
    u32 field_0x2C;                           // 0x2C - CTagProcessor* (raw)
    u8 mUnk30;                                // 0x30
    u8 _31[3];                                // 0x31 - padding to 0x33
    s32 field_0x34;                           // 0x34 - state (signed compares)
    u8 mField38;                              // 0x38
    u8 mField39;                              // 0x39
    u16 mField3A;                             // 0x3A
    u32 field_0x3C;                           // 0x3C
};
