#pragma once

#include <types.h>
#include "kyoshin/CBaseCur.hpp"
#include "kyoshin/CSysWin.hpp"
#include "kyoshin/CItemBoxInfo.hpp"
#include <cstring>
#include "monolib/lib/UnkClass_8045F564.hpp"
#include "monolib/util/FixStr.hpp"
#include "monolib/work/IWorkEvent.hpp"
#include "kyoshin/code_80135FDC_Bdat.hpp"
#include "kyoshin/code_80135FDC_Lyt.hpp"
#include "kyoshin/CQuestWindow_Pane.hpp"
#include "kyoshin/code_80135FDC_State.hpp"
#include "kyoshin/CUICfManager_Res.hpp"

class CFileHandle;

namespace nw4r { namespace math { struct VEC3; } }
namespace nw4r { namespace lyt {
class AnimTransform;
class ArcResourceAccessor;
class Layout;
class Pane;
} }

/* 52-byte entry referenced by the item-box index table.
   Only the first word is known (read by getMCItemSubItemId). */
struct CMCItemBoxEntry {
    u32 field_00;                   // 0x00
    u8 bytes[0x34 - 0x04];          // 0x04-0x33
};

/* FixStr-shaped name buffers of CMCItemBoxSub (same layout and same
   strlen/strcpy copy as ml::FixStr, but no constructor: the index table is
   initialised by initMCItemSub, and retail's default temporary is not
   constructed, so a constructor here would add calls retail does not have). */
struct CMCSubFixStr64 {
    char mString[64];
    int mLength;
    void operator=(const CMCSubFixStr64& str) {
        mLength = std::strlen(str.mString);
        std::strcpy(mString, str.mString);
    }
};
struct CMCSubFixStr128 {
    char mString[128];
    int mLength;
    void operator=(const CMCSubFixStr128& str) {
        mLength = std::strlen(str.mString);
        std::strcpy(mString, str.mString);
    }
};

/* Item-box index structure at +0x314 of CMCGetItemBox.
   An s16 offset table, count, counter and backing entry array.
   The FixStr<64> at +0x108 and FixStr<128> at +0x14C are the short/long
   item-name buffers (mLength at +0x148 / +0x1CC). */
struct CMCItemBoxSub {
    s16 table[0x80];                // 0x000-0x0FF: offset table
    u16 count;                      // 0x100: number of entries
    u8  pad_102;                    // 0x102 (also read as an index source)
    u8  limit;                      // 0x103
    u8  counter;                    // 0x104
    CMCSubFixStr64 shortName;       // 0x108-0x14B: short item-name buffer (mLength at 0x148)
    CMCSubFixStr128 name;           // 0x14C-0x1CF: item-text buffer (mLength at 0x1CC)
    CMCItemBoxEntry* listBase;      // 0x1D0: 52-byte Entry array base
    u8  field_1D4;                  // 0x1D4
};

/* Dispatch-only shim for the CItem impl-instance vtable returned by
   CItem_initItemImplInstances(item). Never instantiated; all virtuals are pure
   (no vtable/code is emitted). MWCC RTTI occupies vtable slots 0,4, so a
   declared virtual slot N lands at vtable offset N*4+8: getKind = 0x08,
   getName = 0x20, getCount = 0x80. */
class CMCItemImplShim {
public:
    virtual u32 getKind(void* entry) = 0;            // vtable offset 0x08
    virtual void vf_04() = 0; virtual void vf_08() = 0;
    virtual void vf_0C() = 0; virtual void vf_10() = 0;
    virtual void vf_14() = 0;
    virtual void* getName(void* entry) = 0;          // vtable offset 0x20
    virtual void vf_1C() = 0; virtual void vf_20() = 0;
    virtual void vf_24() = 0; virtual void vf_28() = 0;
    virtual void vf_2C() = 0; virtual void vf_30() = 0;
    virtual void vf_34() = 0; virtual void vf_38() = 0;
    virtual void vf_3C() = 0; virtual void vf_40() = 0;
    virtual void vf_44() = 0; virtual void vf_48() = 0;
    virtual u32 getIcon(void* entry) = 0;           // vtable offset 0x54
    virtual void vf_50() = 0; virtual void vf_54() = 0;
    virtual void vf_58() = 0;
    virtual void vf_5C() = 0; virtual void vf_60() = 0;
    virtual void vf_64() = 0; virtual void vf_68() = 0;
    virtual void vf_6C() = 0; virtual void vf_70() = 0;
    virtual void vf_74() = 0;
    virtual u32 getCount(void* entry) = 0;           // vtable offset 0x80
};

/* Item-box widget of the crystal-crafting menu (RTTI "CMCGetItemBox", base
   list IWorkEvent: retail vtable __vt__13CMCGetItemBox, dtor @+0x08,
   OnFileEvent @+0x10, the rest of the IWorkEvent tree from the base). */
class CMCGetItemBox : public IWorkEvent {
public:
    CMCGetItemBox();
    virtual ~CMCGetItemBox();
    virtual bool OnFileEvent(CEventFile* pEventFile);

    int isMCGetItemBoxReady();
    int isMCGetItemBoxGridMode();

    UnkClass_8045F564 memRegion1;    // 0x04-0x13
    UnkClass_8045F564 memRegion2;    // 0x14-0x23
    CFileHandle* fileHandle1;               // 0x24
    CFileHandle* fileHandle2;               // 0x28
    CFileHandle* fileHandle3;               // 0x2C
    CFileHandle* fileHandle4;               // 0x30
    u8* memManagerPtr;             // 0x34 (deallocated via mtl::MemManager)
    nw4r::lyt::ArcResourceAccessor* arcAcc1;                   // 0x38
    nw4r::lyt::ArcResourceAccessor* arcAcc2;                   // 0x3C
    nw4r::lyt::Layout* layout40;     // 0x40 (root pane at +0x10)
    nw4r::lyt::AnimTransform* animTrans1;                // 0x44
    nw4r::lyt::AnimTransform* animTrans2;                // 0x48
    u8 field_4C;                     // 0x4C
    u8 field_4D;                     // 0x4D (0x4E-0x4F is natural padding)
    u8* objAt50;                   // 0x50 (deallocated; has vtable)
    u8 field_54;                     // 0x54
    u8 mField55;                     // 0x55 (0x56-0x57 is natural padding)
    CCur07 subObj_58;                // 0x58-0x6F
    CCur09 subObj_70;                // 0x70-0x87
    CCur16 subObj_88;                // 0x88-0x9F
    CCur18 subObj_A0;                // 0xA0-0xB7
    CSysWin sysWin_B8;               // 0xB8-0xF3
    CItemBoxInfo itemBox;            // 0xF4-0x2FF (includes its CItemBoxPage at 0x1A4)
    u8 field_300;                    // 0x300
    u8 field_301;                    // 0x301 (also sort/index)
    u8 field_302;                    // 0x302
    u8 mField303;                    // 0x303
    u8 field_304;                    // 0x304 (row)
    u8 field_305;                    // 0x305 (column; 0x306-0x307 is natural padding)
    u32 field_308;                   // 0x308
    u32 field_30C;                   // 0x30C
    u32 field_310;                   // 0x310
    CMCItemBoxSub sub_314;           // 0x314
};

// Free functions operating on the item-box widget. Their C++ mangled names
// are the retail symbols; the owning TU defines them.
void loadMCGetItemBoxFiles(CMCGetItemBox* self);
void updateMCGetItemBox(CMCGetItemBox* self);
void drawMCGetItemBox(CMCGetItemBox* self, nw4r::lyt::DrawInfo* drawInfo);
void teardownMCGetItemBox(CMCGetItemBox* self);
u8 isMCGetItemBoxActive(CMCGetItemBox* self);
void confirmMCGetItemBox(CMCGetItemBox* self);
void moveMCGetItemBoxRight(CMCGetItemBox* self);
void moveMCGetItemBoxUp(CMCGetItemBox* self);
void moveMCGetItemBoxDown(CMCGetItemBox* self);
void toggleMCGetItemBoxSysWin(CMCGetItemBox* self);
void openMCGetItemBoxSysWin(CMCGetItemBox* self);
u32 getMCGetItemBoxPageState(CMCGetItemBox* self);
void cacheMCGetItemBoxEntries(CMCGetItemBox* self);
u32 getMCGetItemBoxKeyCode(CMCGetItemBox* self);

// ---------------------------------------------------------------------------
// C-linkage imports
// ---------------------------------------------------------------------------

// CItem_initItemImplInstances: canonical decl from CfGameManager.hpp.
extern "C" int   IsSkillItem(void*);
extern "C" void CItemData_lookupOrAlloc(CMCItemBoxEntry*, int);
extern "C" u32 CItemBlock_countEmpty(u8);
extern "C" u16 ArrayGet12(const unsigned short*, unsigned char);
extern "C" void CopyTabSlotVec(u32*, void*, u32);
extern "C" char* getMCItemSubText(CMCItemBoxSub*, u16);
extern "C" char* getMCItemSubName(CMCItemBoxSub*, u16);
extern "C" __declspec(noinline) u32 getMCItemSubItemId(CMCItemBoxSub*, u32);
extern "C" __declspec(noinline) s8 getMCItemSubKind(CMCItemBoxSub*, u32);
// Retail mangled names kept as source names so the reloc matches exactly.
extern "C" char lbl_eu_8050FF8C[];
extern "C" void* lbl_eu_806640D8;  // declared void* in CMCEffStart.hpp/CItemBoxLine.hpp -> kept void*
extern char* lbl_eu_806640EC;  // canonical decl: CMCCrystalBox.hpp
extern "C" void* lbl_eu_80664100;  // unused address anchor - kept void*
extern "C" void* sCrystalItemTable;  // unused address anchor - kept void*
extern "C" void* sCrystalMesTable;  // unused address anchor - kept void*
extern "C" void* sItemMesTable;  // unused address anchor - kept void*
extern "C" const float lbl_eu_80668BF0;
// lbl_eu_80668BD4 is the 0.0f used for the fractional-part comparison.
extern const float lbl_eu_80668BD4;
// lbl_eu_80668BD8 (double) is the 2^52 constant of MWCC's u32->f64 trick;
// lbl_eu_80668BEC is the 0.25f row divisor used when converting a flat
// cursor index to row/column (field_304 / field_305).
// lbl_eu_80668BD8 (double) is MWCC's unsigned 2^52 conversion magic.
extern const double lbl_eu_80668BD8;
// lbl_eu_80668BE0 (double) is MWCC's signed int->f64 2^52 conversion magic.
extern const double lbl_eu_80668BE0;
extern "C" void moveMCGetItemBoxCursor(CMCGetItemBox*);
extern "C" void initMCGetItemBoxList(CMCGetItemBox*, u16, void*, u8);
extern "C" void setMCSlotIconTex(CMCGetItemBox*, u32, CMCItemBoxEntry*, u8);
__declspec(noinline) void setMCSlotNameTex(CMCGetItemBox*, u32, CMCItemBoxEntry*, u8);
extern "C" __declspec(noinline) CMCItemBoxEntry* getMCItemSubEntry(CMCItemBoxSub*, u32);

// ---------------------------------------------------------------------------
// OnFileEvent / getMCItemSubText imports (retail symbol names verbatim)
// ---------------------------------------------------------------------------
extern "C" void copyVEC3(void*, const void*);  // canonical decl: CMCCrystalBox.hpp

extern "C" void* Bdat_GetTable_AA34();  // canonical decl: CMCCrystalBox.hpp


/* 28-byte message-format helper table read by getMCItemSubText's '%2' handler
   (indexed by the item's kind byte). */
struct CMCItemMsgTbl { u32 v[7]; };
extern "C" CMCItemMsgTbl sMCItemMsgFmtTbl;
extern const float lbl_eu_80668C00;
