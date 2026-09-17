#pragma once

#include <types.h>
#include "monolib/lib/UnkClass_8045F564.hpp"

namespace nw4r {
namespace lyt {
class Layout;
class DrawInfo;
class AnimTransform;
class ArcResourceAccessor;
class Pane;
}
}

// Full object layout for CNumSelect (used by C-linkage accessors)
class CFileHandle;
class CEventFile;

struct CNumSelectFull {
    u8 _00[0x2C];
    u8 field_2C;
    u8 field_2D;
    u8 field_2E;
};

// String pool in split .rodata ("pic_%02d" style format strings and archive
// paths used by the number-select HUD).
extern char lbl_eu_80506C14[];

// Name/label helpers from code_80135FDC (retail unmangled C symbols).
extern "C" char* BdatTouchStringCell(char*, char*, u32);
extern "C" void LayoutSetTextBoxFmtValue(nw4r::lyt::Layout*, char*, char*, int);
extern "C" void func_80136A1C(nw4r::lyt::Layout*, char*, char*, int);

/* Font-info objects from CDeviceFont::getFontInfo dispatch through
 * IDeviceFontInfo::getFont (+0x24); see monolib/device/CDeviceFont.hpp. */

/* 'timg' texture record chain resolved through the cursor accessor source:
 * obj->vf01(0x74696d67, name, 0) returns a record whose +0x8 points at a
 * pointer to the {u16 width, u16 height} pair used to size the panes. */
struct CNumSelectTimgDims {
    u16 m00;
    u16 m02;
};

class CNumSelectTimgRef {
public:
    CNumSelectTimgDims* dims; // 0x0
};

class CNumSelectTimg {
public:
    u32 unk0;                // 0x0
    u32 unk4;                // 0x4
    CNumSelectTimgRef* unk8; // 0x8
};

/* The 'timg' source from func_801355F4 is an nw4r::lyt::ArcResourceAccessor;
 * GetResource(+0x0C) resolves the named texture record. */

// code_80135FDC layout-build helpers (retail mangled C++ names).
void buildLayout(nw4r::lyt::Layout**, nw4r::lyt::ArcResourceAccessor*, const char*);
void bindLayoutAnimTransform(nw4r::lyt::Layout*, nw4r::lyt::AnimTransform**,
                   nw4r::lyt::ArcResourceAccessor*, char*);
void setLayoutTextBoxFont(nw4r::lyt::Layout*, char*, u32);

// Retail-unmangled C-ABI imports used by OnFileEvent.
extern "C" void* getFontInfo__11CDeviceFontFUlPQ34nw4r3lyt6Layout(u32, nw4r::lyt::Layout*);
extern "C" void func_8013676C(nw4r::lyt::Pane*, void*);
extern "C" void func_80124288(nw4r::lyt::Pane*, float*);
extern "C" char* func_801355BC();
extern "C" nw4r::lyt::ArcResourceAccessor* func_801355F4();
extern "C" char* MakeTplNameSysFile(u32);
extern "C" u16 BdatGetU16ByTableKey(const void*, const void*, u16);
extern "C" void PaneSetTexPaletteByName(nw4r::lyt::Layout*, const char*, u32);
extern "C" int isClassicController__Q22cf13CfGameManagerFv(int arg);

// u32->float cast-magic constant shared with the retail sdata2 pool.
extern const double lbl_eu_80668090;

// Draw helper (retail symbol keeps its C++ mangling).
void drawLayout(nw4r::lyt::Layout*, nw4r::lyt::DrawInfo*, int, int);
void setLayoutTextBoxNumber(nw4r::lyt::Layout*, char*, u8);

// Resource release / anim helpers (C++-mangled imports; see CItemBoxInfo.hpp).
void func_801390E0(CFileHandle**);
void releaseArcResourceAccessor(nw4r::lyt::ArcResourceAccessor*);
u32 advanceAnimTransform(nw4r::lyt::AnimTransform*, float);
// Retail symbol is unmangled (plain AnimRewindFrame), so keep C linkage.
extern "C" u32 AnimRewindFrame(nw4r::lyt::AnimTransform*, float);
// Retail symbol is unmangled (plain func_80124270), so keep C linkage.
extern "C" void func_80124270(nw4r::lyt::Pane*, u32);
void playUISound(u32);
extern const f32 lbl_eu_80668088;
extern const f32 lbl_eu_8066808C;

// (Layout teardown goes through `delete`: the deleting-dtor vcall at +0x08
// matches retail.)

// CNumSelect vtable (retail .data label; extern here, defined via retail linkage).
extern void* lbl_eu_80534BC0[];

/* Stores the retail vtable label at +0x00 before the members construct,
mirroring the CBattery/CLoad pattern (MWCC's implicit __vt__10CNumSelect
reloc name would drift from the retail lbl_eu_80534BC0). */
struct CNumSelectVtblBase {
    void* mVtbl; // +0x00
    CNumSelectVtblBase() : mVtbl(lbl_eu_80534BC0) {}
};

class CNumSelect : public CNumSelectVtblBase {
public:
    CNumSelect(u8 arg);
    ~CNumSelect();
    bool OnFileEvent(CEventFile* evt);

    void func_801EAE8C();
    void func_801EAF7C(nw4r::lyt::DrawInfo* drawInfo);
    void func_801EB030(char* str);
    void func_801EB064(int value);

    /* 0x04 */ UnkClass_8045F564 mMemRegion;
    /* 0x14 */ CFileHandle* field_14;
    /* 0x18 */ nw4r::lyt::ArcResourceAccessor* field_18;
    /* 0x1C */ nw4r::lyt::Layout* mpLayout;
    /* 0x20 */ nw4r::lyt::AnimTransform* field_20;
    /* 0x24 */ nw4r::lyt::AnimTransform* field_24;
    /* 0x28 */ nw4r::lyt::AnimTransform* field_28;
    /* 0x2C */ u8 field_2C;
    /* 0x2D */ u8 field_2D;
    /* 0x2E */ u8 field_2E;
    /* 0x2F */ u8 field_2F;
    /* 0x30 */ u8 field_30;
};
