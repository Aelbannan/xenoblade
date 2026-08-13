#pragma once

#include <types.h>
#include "monolib/lib/UnkClass_8045F564.hpp"

namespace nw4r {
namespace lyt {
    class Layout;
    class AnimTransform;
    class ArcResourceAccessor;
}
namespace math {
    struct VEC3;
}
}

class CArtsInfo {
public:
    CArtsInfo();
    virtual ~CArtsInfo();
    void OnFileEvent();
    u8 getField48();
    u8 getField49();
    void setField54(u8 val);
    void setField55(u8 val);
    void setField56(u8 val);
    void setField58(u16 val);
    u32 getField5A();
    int isField44GE6();
    void initialize();

    // vtable pointer at 0x00 (implicit)
    /* 0x04 */ UnkClass_8045F564 mMemRegion;
    /* 0x14 */ int field_0x14;  // file handle 1
    /* 0x18 */ int field_0x18;  // file handle 2
    /* 0x1C */ int field_0x1C;  // arc resource accessor
    /* 0x20 */ nw4r::lyt::Layout* mpLayout1;
    /* 0x24 */ nw4r::lyt::AnimTransform* mpAnimTrans1;
    /* 0x28 */ nw4r::lyt::AnimTransform* mpAnimTrans2;
    /* 0x2C */ nw4r::lyt::AnimTransform* mpAnimTrans3;
    /* 0x30 */ nw4r::lyt::AnimTransform* mpAnimTrans4;
    /* 0x34 */ nw4r::lyt::Layout* mpLayout2;
    /* 0x38 */ nw4r::lyt::AnimTransform* mpAnimTrans5;
    /* 0x3C */ nw4r::lyt::AnimTransform* mpAnimTrans6;
    /* 0x40 */ u8 field_0x40;
    /* 0x41 */ u8 _41[3];      // padding
    /* 0x44 */ int field_0x44;  // state machine
    /* 0x48 */ u8 field_0x48;
    /* 0x49 */ u8 field_0x49;
    /* 0x4A */ u8 _4A[2];      // padding
    /* 0x4C */ int field_0x4C;
    /* 0x50 */ int field_0x50;
    /* 0x54 */ u8 field_0x54;
    /* 0x55 */ u8 field_0x55;
    /* 0x56 */ u8 field_0x56;
    /* 0x57 */ u8 _57;          // padding
    /* 0x58 */ u16 field_0x58;
    /* 0x5A */ s8 field_0x5A;
    /* 0x5B */ u8 _5B;          // padding
    /* 0x5C */ u8 mCursor[0x18]; // CCur18 embedded cursor
};

// Abstract view into the embedded CCur18 cursor vtable used by this unit
// (func_80236CF4). MWCC prefixes the vtable with offset-to-top + RTTI
// (2 entries), so user virtual index 2 sits at vtable +0x10 - the
// position-taking "Move" virtual, invoked with the VEC3 written by
// func_80137924.
class CArtsInfoCurView {
public:
    virtual void vf02() = 0;                    // index 0 -> +0x08
    virtual void vf03() = 0;                    // index 1 -> +0x0C
    virtual void vf04(const nw4r::math::VEC3*) = 0; // index 2 -> +0x10 - Move
};

// Stats sub-object embedded at +0x17C of the func_8009EC9C character data
// (same object as CPartySlotStats). The retail dispatch at vtable byte
// offset 0x224 (slot 0x89) returns a pointer to the stat block below. MWCC
// puts two implicit dtor slots at the start of every polymorphic vtable, so
// the target below (user virtual #135) lands at 0x224. All-pure; never
// constructed directly - only used to reproduce the retail r12 dispatch.
struct CArtsStatBlock;
class CArtsStatsV {
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
    virtual void v20() = 0;
    virtual void v21() = 0;
    virtual void v22() = 0;
    virtual void v23() = 0;
    virtual void v24() = 0;
    virtual void v25() = 0;
    virtual void v26() = 0;
    virtual void v27() = 0;
    virtual void v28() = 0;
    virtual void v29() = 0;
    virtual void v30() = 0;
    virtual void v31() = 0;
    virtual void v32() = 0;
    virtual void v33() = 0;
    virtual void v34() = 0;
    virtual void v35() = 0;
    virtual void v36() = 0;
    virtual void v37() = 0;
    virtual void v38() = 0;
    virtual void v39() = 0;
    virtual void v40() = 0;
    virtual void v41() = 0;
    virtual void v42() = 0;
    virtual void v43() = 0;
    virtual void v44() = 0;
    virtual void v45() = 0;
    virtual void v46() = 0;
    virtual void v47() = 0;
    virtual void v48() = 0;
    virtual void v49() = 0;
    virtual void v50() = 0;
    virtual void v51() = 0;
    virtual void v52() = 0;
    virtual void v53() = 0;
    virtual void v54() = 0;
    virtual void v55() = 0;
    virtual void v56() = 0;
    virtual void v57() = 0;
    virtual void v58() = 0;
    virtual void v59() = 0;
    virtual void v60() = 0;
    virtual void v61() = 0;
    virtual void v62() = 0;
    virtual void v63() = 0;
    virtual void v64() = 0;
    virtual void v65() = 0;
    virtual void v66() = 0;
    virtual void v67() = 0;
    virtual void v68() = 0;
    virtual void v69() = 0;
    virtual void v70() = 0;
    virtual void v71() = 0;
    virtual void v72() = 0;
    virtual void v73() = 0;
    virtual void v74() = 0;
    virtual void v75() = 0;
    virtual void v76() = 0;
    virtual void v77() = 0;
    virtual void v78() = 0;
    virtual void v79() = 0;
    virtual void v80() = 0;
    virtual void v81() = 0;
    virtual void v82() = 0;
    virtual void v83() = 0;
    virtual void v84() = 0;
    virtual void v85() = 0;
    virtual void v86() = 0;
    virtual void v87() = 0;
    virtual void v88() = 0;
    virtual void v89() = 0;
    virtual void v90() = 0;
    virtual void v91() = 0;
    virtual void v92() = 0;
    virtual void v93() = 0;
    virtual void v94() = 0;
    virtual void v95() = 0;
    virtual void v96() = 0;
    virtual void v97() = 0;
    virtual void v98() = 0;
    virtual void v99() = 0;
    virtual void v100() = 0;
    virtual void v101() = 0;
    virtual void v102() = 0;
    virtual void v103() = 0;
    virtual void v104() = 0;
    virtual void v105() = 0;
    virtual void v106() = 0;
    virtual void v107() = 0;
    virtual void v108() = 0;
    virtual void v109() = 0;
    virtual void v110() = 0;
    virtual void v111() = 0;
    virtual void v112() = 0;
    virtual void v113() = 0;
    virtual void v114() = 0;
    virtual void v115() = 0;
    virtual void v116() = 0;
    virtual void v117() = 0;
    virtual void v118() = 0;
    virtual void v119() = 0;
    virtual void v120() = 0;
    virtual void v121() = 0;
    virtual void v122() = 0;
    virtual void v123() = 0;
    virtual void v124() = 0;
    virtual void v125() = 0;
    virtual void v126() = 0;
    virtual void v127() = 0;
    virtual void v128() = 0;
    virtual void v129() = 0;
    virtual void v130() = 0;
    virtual void v131() = 0;
    virtual void v132() = 0;
    virtual void v133() = 0;
    virtual void v134() = 0;
    virtual CArtsStatBlock* getStatBlock() = 0;  // vtable +0x224
};

// Non-polymorphic layout view of the embedded stats sub-object (the real
// object is CArtsStatsV; the member slot is accessed via this struct's
// mVtbl field, then cast for the virtual dispatch).
struct CArtsStats {
    void* mVtbl;                            // +0x00
};

// Stat block returned by the vtable[0x224] dispatch.
struct CArtsStatBlock {
    u8 _00[0x10];
    float field_0x10;                   // +0x10
    u8 _14[0x08];                       // 0x14..0x1B
    s16 field_0x1C;                     // +0x1C
    s16 field_0x1E;                     // +0x1E
    s16 field_0x20;                     // +0x20
};

// Character-data object returned by func_8009EC9C (see CPartyCharData).
struct CArtsCharData {
    u8 _00[0x1C];
    s16 field_0x1C;                     // +0x1C
    s16 field_0x1E;                     // +0x1E
    s16 field_0x20;                     // +0x20
    s16 field_0x22;                     // +0x22
    s16 field_0x24;                     // +0x24
    s16 field_0x26;                     // +0x26 (weapon id; -1 = none)
    u8 _28[0x17C - 0x28];               // 0x28..0x17B
    CArtsStats stats;                   // +0x17C
};

// Entry object returned by func_80157C4C (a list/factory entry; the retail
// arts-info caller only reads the first word).
struct CArtsInfoListEntry {
    u32 field_0x0;
};

// Sub-entry returned by the slot-info virtual (vtable +0x2C). The arts list
// loop reads an id field at +0x4 (bits 4..15 of the u16) and a value at
// +0x0 (bits 10..20 of the u32).
struct CArtsItemInfo {
    u32 field_0x0;                      // +0x00
    u16 field_0x4;                      // +0x04
};

// Dispatch-only view of the item-implementation object returned by
// CItem_initItemImplInstances (see CMenuGetItemMulti / CItemBoxGrid). The
// arts list loop dispatches through vtable slots 0x2C/0x30/0x40 (slot-info,
// slot-count, slot-id of an entry) and 0x54/0x90 (sub-entry rank/value);
// fillers keep those slots at their retail offsets.
class CItemImplV {
public:
    virtual void v00() = 0;                              // +0x08
    virtual void v01() = 0;                              // +0x0C
    virtual void v02() = 0;                              // +0x10
    virtual void v03() = 0;                              // +0x14
    virtual void v04() = 0;                              // +0x18
    virtual void v05() = 0;                              // +0x1C
    virtual void v06() = 0;                              // +0x20
    virtual void v07() = 0;                              // +0x24
    virtual void v08() = 0;                              // +0x28
    virtual CArtsItemInfo* vf11(CArtsInfoListEntry*, u32) = 0; // +0x2C slot info
    virtual u8 vf12(CArtsInfoListEntry*) = 0;            // +0x30 slot count
    virtual void v13() = 0;                              // +0x34
    virtual void v14() = 0;                              // +0x38
    virtual void v15() = 0;                              // +0x3C
    virtual s16 vf16(CArtsInfoListEntry*, u32) = 0;      // +0x40 slot id
    virtual void v17() = 0;                              // +0x44
    virtual void v18() = 0;                              // +0x48
    virtual void v19() = 0;                              // +0x4C
    virtual void v20() = 0;                              // +0x50
    virtual u16 vf21(CArtsInfoListEntry*) = 0;           // +0x54 rank count
    virtual void v22() = 0;                              // +0x58
    virtual void v23() = 0;                              // +0x5C
    virtual void v24() = 0;                              // +0x60
    virtual void v25() = 0;                              // +0x64
    virtual void v26() = 0;                              // +0x68
    virtual void v27() = 0;                              // +0x6C
    virtual void v28() = 0;                              // +0x70
    virtual void v29() = 0;                              // +0x74
    virtual void v30() = 0;                              // +0x78
    virtual void v31() = 0;                              // +0x7C
    virtual void v32() = 0;                              // +0x80
    virtual void v33() = 0;                              // +0x84
    virtual void v34() = 0;                              // +0x88
    virtual void v35() = 0;                              // +0x8C
    virtual int vf36(CArtsInfoListEntry*) = 0;           // +0x90 value
};

// Six-byte item slot-type flags for the arts list loop (func_80236E6C): a
// u32 + u16 loaded from the sdata objects at 0x80668688/0x8066868C, then
// byte-indexed per loop iteration. The union gives the byte view without
// pointer arithmetic.
union CArtsSlotFlags {
    struct {
        u32 field_0x0;                  // +0x00
        u16 field_0x4;                  // +0x04
    } w;
    u8 b[6];
};

// C-linkage imports (retail symbol names - keep linkage/signatures verbatim)
// func_eu_80136F90 is defined in code_80135FDC.cpp (C linkage);
// func_eu_8023D490 is a same-unit retail symbol (unmangled, C linkage).
extern "C" void func_eu_80136F90(char* str);
extern "C" void func_eu_8023D490(CArtsInfo*, u32, char*);
extern "C" int getLanguage__9CDeviceSCFv();
extern "C" s8 func_801360CC(const void*, const char*, u8); // BDAT string -> s8
extern "C" void* getHandleMEM2__Q23mtl10MemManagerFv();
extern "C" void* readFile__11CDeviceFileFUlPCcP10IWorkEventii(u32, const char*, void*, int, int);
extern "C" u32 func_800A9D90();
extern "C" void* readCommonArchiveFile__11CDeviceFileFUlPCcP10IWorkEventii(u32, const char*, void*, int, int);
extern "C" void func_8003AA8C__5CBdatFUl(u32);
extern "C" void func_801390E0__FPP11CFileHandle(void*);
extern "C" void func_80139124__FPQ34nw4r3lyt19ArcResourceAccessor(void*);
extern "C" void func_8045F778__17UnkClass_8045F564Fv(void*);
extern "C" UnkClass_8045F564* __ct__17UnkClass_8045F564Fv(UnkClass_8045F564*);
extern "C" void __dt__17UnkClass_8045F564Fv(void*, int);
extern "C" void __ct__CCur18(void*, void*);
extern "C" void __dt__6CCur18Fv(void*, int);
extern "C" void __dl__FPv(void*);
extern "C" u32 func_80137444__FPQ34nw4r3lyt13AnimTransformf(nw4r::lyt::AnimTransform*, float);
extern "C" u32 func_80137510(nw4r::lyt::AnimTransform*, float);
extern "C" void func_80136B4C(nw4r::lyt::Layout*, char*, char*, u32);
extern "C" void func_80136A1C(nw4r::lyt::Layout*, char*, char*, u32);
extern "C" char* func_80136190(const void*, const void*, int); // caller-tuned (see code_80135FDC.hpp)
extern "C" void func_80138078__FUl(u32);
extern "C" u8 func_801361E8(u32, const char*, u32);
// func_8013606C/8013600C take byte-keyed lookups: retail masks the 3rd arg
// to 8 bits at the call site, so the param is u8.
extern "C" u16 func_8013606C(const void*, const void*, u8);
extern "C" u8 func_8013600C(void*, const char*, u8);
extern "C" s16 func_80136130(const void*, const void*, u8);
extern "C" void func_801D1F9C(void*, u32);
extern "C" void func_801C4B60(void*, s16, s16, s16, s16); // colour init
extern "C" void func_80139A18(void*, void*, void*, void*);
extern "C" u32 func_80137924(void*, void*, void*, void*);
extern "C" void* func_8009EC9C(u32);
extern "C" u32 func_800A082C(CArtsCharData*);
extern "C" u16 func_80139358(u32);
extern "C" u32 func_80136254(const void*, const void*, int); // wider-than-u16 in retail (CSysWinScenarioLog.hpp)
extern "C" CArtsInfoListEntry* func_80157C4C(u32);
extern "C" void func_801D202C(void*);
extern "C" void func_801D20B0(void*, void*);
extern "C" void func_801D216C(void*, int);
extern "C" int sprintf(char*, const char*, ...);

// Item-implementation object for an entry (dispatch-only view CItemImplV);
// the retail call sites pass the entry in r3 and the loop dispatches
// through CItemImplV's vtable slots.
extern "C" CItemImplV* CItem_initItemImplInstances(CArtsInfoListEntry* entry);

// Imports from the func_8025FB10 accessor family (code_8025FB10.cpp).
// Retail call sites bind to the unmangled names, so keep C linkage
// (same style as CItemBoxGrid.hpp).
extern "C" int func_8026178C(void*, u32);
extern "C" u32 func_8025FB10(void*, u32);

// Float constant in the small data area (sda21-accessed via lfs, promoted to
// double for the arts-info sprintf vararg).
extern float lbl_eu_806686A0;

// Double constants in the small data area (sda21-accessed via lfd, used as
// vararg doubles in the arts-info sprintf calls).
extern double lbl_eu_806686C0;
extern double lbl_eu_806686C8;

// Double constant in the small data area (sda21-accessed via lfd): the
// 2^52 + 2^31 bias (4503601774854144.0) used by MWCC's s32->double
// conversion trick (func_8023B280 / func_8023A460). The conversion relocs
// reference a TU-local pool entry (cannot be named in source — see
// makecrystal/code_80213488.cpp) so they register as un-fixable reloc drift.
extern double lbl_eu_80668698;

// Float constant in the small data area (sda21-accessed via lfs) scaling the
// converted double in func_8023A460.
extern float lbl_eu_806686A4;

// Float constants in the small data area (sda21-accessed via lfs): scaling
// factors for the arts-level percentage computations in func_80237238.
extern float lbl_eu_80668690;
extern float lbl_eu_80668694;

// Float scale for the arts-level percentage computation in func_80239FC4
// (percentage = 806686BC * level + base, sda21-accessed via lfs).
extern float lbl_eu_806686BC;

// Item slot-type flags for the func_80236E6C list loop (6 bytes: a u32
// followed by a u16 at 0x8066868C). Loaded as lwz + lhz, byte-indexed via
// the CArtsSlotFlags union.
extern u32 lbl_eu_80668688;
extern u16 lbl_eu_8066868C;

// Arts bdat file pointer used by func_80236E6C's final row lookup
// (func_80136254, value passed as the first argument).
extern u32 lbl_eu_806640D8;
