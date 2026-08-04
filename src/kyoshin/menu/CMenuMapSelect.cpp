// Auto-scaffolded catalog TU for kyoshin/menu/CMenuMapSelect
// Replace stubs with high-level C/C++ during decomp.

#include "kyoshin/menu/CMenuMapSelect.hpp"

// ---------------------------------------------------------------------------
// External retail symbols
// ---------------------------------------------------------------------------
extern "C" {
void __ct__8CProcessFv(CProcess* self);
u32 getWorkMem__17CWorkThreadSystemFv();
void* allocate__Q23mtl10MemManagerFUlUl(u32 size, u32 handle);
void Regist__8CProcessFP8CProcessb(CProcess* self, CProcess* parent, bool insertTop);
void __ct__CBgTex(CBgTex* self, u8 arg);
void __ct__CTitleAHelp(CTitleAHelp* self, char* arg1, u8 arg2);
void __ct__7CMapSelFv(CMapSel* self);
void __ct__5CFadeFv(CFade* self);
void __ct__9CFloorMapFv(CFloorMap* self);

extern u32 __ptmf_null[3];
extern char lbl_eu_8052BF70[];
extern char lbl_eu_80536D28[];
extern u32 lbl_eu_80664790;
extern const f32 lbl_eu_80668728;
}

// Byte-range shim over the CProcess header + PTMF zone, so the constructor
// can write the vtable (+0x10) and callback slots (+0x3C..0x53) without raw
// pointer arithmetic. Layout matches CProcess (0x00-0x3B) exactly.
struct CMapSelCtorShim {
    u8 _00[0x10];
    void* vtable;          // 0x10 -- CProcess vtable, overwritten by this ctor
    u8 _14[0x28];          // 0x14-0x3B -- rest of CProcess
    u32 callbacks[6];      // 0x3C-0x53 -- PTMF callback slots (__ptmf_null copies)
    u8 field54;            // 0x54
    u8 field55;            // 0x55
};

// ---------------------------------------------------------------------------
// Target: us-802443d4 -- CMenuMapSelect constructor
//
// Factory-style constructor. Allocates a singleton CMenuMapSelect from work
// memory, constructs sub-objects in-place, registers with the parent process,
// and returns the singleton pointer. Returns 0 if the singleton already exists.
//
// Retail ASM: r3 (first arg) = parent for Regist, r4 (second arg) = parent
// stored as mParent. Both point to the same parent process in practice.
// ---------------------------------------------------------------------------
extern "C" CMenuMapSelect* __ct__CMenuMapSelect(CProcess* parent, CProcess* parent2) {
    // Singleton guard: if already created, return 0
    if (lbl_eu_80664790 != 0) {
        return 0;
    }

    // Get work memory handle
    u32 handle = getWorkMem__17CWorkThreadSystemFv();

    // Allocate memory for the object (0x34E4 bytes)
    CMapSelCtorShim* shim = (CMapSelCtorShim*)allocate__Q23mtl10MemManagerFUlUl(0x34E4, handle);

    if (shim != 0) {
        // Call CProcess base constructor on the allocated memory
        __ct__8CProcessFv((CProcess*)shim);

        // Set interim vtable at +0x10 (overwrites CProcess vtable set by ctor)
        shim->vtable = lbl_eu_8052BF70;

        // Copy __ptmf_null triplets into callback slots at +0x3C..0x53.
        // First triple: callbacks[0..2] -- loads interleaved with stores.
        u32* ptmf = __ptmf_null;
        char* vtFinal = lbl_eu_80536D28;

        u32 ptmf1 = ptmf[1];
        u32 ptmf0 = ptmf[0];

        // Final vtable + IScnRender vtable (final + 0x24)
        void* iscnVtbl = vtFinal + 0x24;

        shim->callbacks[0] = ptmf0;
        shim->callbacks[1] = ptmf1;

        u32 ptmf2 = ptmf[2];
        shim->callbacks[2] = ptmf2;

        // Reload from __ptmf_null for the second triple (callbacks[3..5])
        ptmf1 = ptmf[1];
        ptmf0 = ptmf[0];
        shim->callbacks[3] = ptmf0;
        shim->callbacks[4] = ptmf1;
        ptmf2 = ptmf[2];
        shim->callbacks[5] = ptmf2;

        // Zero the flag bytes
        shim->field54 = 0;
        shim->field55 = 0;

        // Set final vtables and parent pointer.
        // The IScnRender vtable is at +0x58, beyond the shim. The parent is
        // the CMenuMapSelect::mParent field at +0x5C.
        CMenuMapSelect* self = (CMenuMapSelect*)shim;
        shim->vtable = vtFinal;
        *(u32*)((u8*)self + 0x58) = (u32)iscnVtbl;
        self->mParent = parent2;

        // Construct sub-objects in declaration order
        __ct__CBgTex(&self->mBgTex, 0);
        __ct__CTitleAHelp(&self->mTitleAHelp, 0, 0);
        __ct__7CMapSelFv(&self->mMapSel);
        __ct__5CFadeFv(&self->mFade);
        __ct__9CFloorMapFv(&self->mFloorMap);

        // Initialize state and timer
        self->mState = 0;
        self->mTimer = lbl_eu_80668728;
    }

    // Store pointer (null or valid) in singleton guard
    lbl_eu_80664790 = (u32)shim;

    // Register with parent process (insertTop = false)
    // Retail uses the original r3 (first param) as parent for Regist
    Regist__8CProcessFP8CProcessb((CProcess*)shim, parent, 0);

    // Reload singleton pointer for return
    return (CMenuMapSelect*)lbl_eu_80664790;
}


// ---------------------------------------------------------------------------
// Stub: func_80242354 -- singleton guard (returns whether the singleton is set)
// ---------------------------------------------------------------------------
extern "C" u32 func_80242354() {
    return lbl_eu_80664790 != 0;
}

// ---------------------------------------------------------------------------
// Stub: func_80242524
// ---------------------------------------------------------------------------
void func_80242524(){}

// ---------------------------------------------------------------------------
// Stub: func_80242A28
// ---------------------------------------------------------------------------
void func_80242A28(){}

// ---------------------------------------------------------------------------
// IScnRender vtable this-adjusting thunk for cbRenderBefore.
// ---------------------------------------------------------------------------
void CMenuMapSelect::func_8024309C() {
    cbRenderBefore();
}

// ---------------------------------------------------------------------------
// IScnRender vtable this-adjusting thunk for destructor.
// ---------------------------------------------------------------------------
void CMenuMapSelect::func_802430A4() {
    this->~CMenuMapSelect();
}