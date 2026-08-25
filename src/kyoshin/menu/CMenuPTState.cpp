// kyoshin/menu/CMenuPTState -- PT (photo/theater?) state menu TU.
// FULL_MATCH: func_80192BD0, func_80192BE4/BEC (vtable adjustor thunks),
// __ct__80192C10, func_80192C2C, func_80192CB0.
// __ct__CMenuPTState: 94.3% static, residual = 4 pure r6/r7 reg-swaps
// (register-color ceiling; witness-certified equivalent).

#include "kyoshin/menu/CMenuPTState.hpp"
#include "monolib/scn/CScnTimeApi.hpp"
#include "kyoshin/cf/CfGameManagerData.hpp"  // H3 label-owner decl (lbl_eu_80663E14; lbl_eu_80663E24)
#include "kyoshin/cf/object/CAIAction.hpp"   // battle-list helpers (func_800F6EAC / func_80148778)

// Retail CProcess / mem-manager / win-ctor imports (defined in other TUs).
// Retail exports these under their verbatim-mangled spellings, so C linkage is
// required to bind the calls to the exact linker symbols.
extern "C" void __ct__8CProcessFv(CProcess* self);
extern "C" void Regist__8CProcessFP8CProcessb(CProcess* self, CProcess* parent, bool insertTop);
extern "C" void* getWorkMem__17CWorkThreadSystemFv();
extern "C" void* allocate__Q23mtl10MemManagerFUlUl(u32 size, u32 handle);
extern "C" void __ct__CBgTex(CBgTex* self, u8 arg);
extern "C" void __ct__CPartyStateWin(CPartyStateWin* self, u32 arg1, u32 arg2);

// Real member functions defined in code_8018F8D8.cpp -- retail slice names keep
// the exact (already-mangled) linker symbols so tail-call relocs match.
extern "C" void cbRenderBefore__12CMenuPTStateFv(CMenuPTState* self);
extern "C" void __dt__12CMenuPTStateFv(CMenuPTState* self);

// SDA globals (retail names; defined in common/sdata splits). Global-scope
// variables are never mangled by MWCC.
extern u32 lbl_eu_80664300;              // singleton pointer
extern u32 __ptmf_null[3];
extern char lbl_eu_8052BF70[];           // CProcess primary vtable
extern char lbl_eu_80532A38[];           // CMenuPTState final vtable
extern const f32 lbl_eu_80667AA8;        // float pool: timer decay multiplier

// Singleton factory constructor.
// MWCC allocating-ctor: not a real C++ ctor -- a free function that allocates a
// fresh CMenuPTState, builds it, registers it under `parent`, and returns the
// singleton (null if it already existed).
extern "C" CMenuPTState* __ct__CMenuPTState(CProcess* _this, CProcess* storedParent) {
    CMenuPTState* result;
    int zero = 0;
    if (lbl_eu_80664300 != 0) {
        // Singleton already exists: return null.
        result = 0;
    } else {
        // Allocate memory for the new object
        CMenuPTStateCtorShim* shim = (CMenuPTStateCtorShim*)allocate__Q23mtl10MemManagerFUlUl(
            0x6c70, (u32)getWorkMem__17CWorkThreadSystemFv());

        if (shim != 0) {
            // Construct CProcess (primary base) on the freshly allocated memory.
            __ct__8CProcessFv((CProcess*)shim);

            // vtable / terminal-state ptmf slots come from the retail's inlined
            // vtable-store sequence (see shim layout comment).
            shim->vtable = lbl_eu_8052BF70;
            const PtmfNullWords* ptmf = reinterpret_cast<const PtmfNullWords*>(__ptmf_null);
            u32 ptmfWord1 = ptmf->w[1];
            u32 ptmfWord0 = ptmf->w[0];
            char* vtFinal = lbl_eu_80532A38;

            shim->callbacks[0] = ptmfWord0;
            char* iscnVtbl = vtFinal + 0x24;
            shim->callbacks[1] = ptmfWord1;
            u32 ptmfWord2 = ptmf->w[2];

            shim->callbacks[2] = ptmfWord2;
            ptmfWord1 = ptmf->w[1];
            ptmfWord0 = ptmf->w[0];
            shim->callbacks[3] = ptmfWord0;
            shim->callbacks[4] = ptmfWord1;
            ptmfWord2 = ptmf->w[2];
            shim->callbacks[5] = ptmfWord2;
            shim->field54 = zero;
            shim->field55 = zero;

            // The compiled CMenuPTState layout puts the IScnRender vptr at
            // 0x3C (MWCC MI ordering), but retail places it at 0x58 -- so all
            // trailing stores go through the shim's explicit offsets instead
            // of the typed members.
            shim->vtable = vtFinal;
            shim->iscnVtbl = iscnVtbl;
            shim->storedParent = storedParent;

            __ct__CBgTex((CBgTex*)shim->bgTex, 0);
            __ct__CPartyStateWin((CPartyStateWin*)shim->_80, 0, 0);
            shim->field6C6C = 0;
        }

        // Store singleton
        lbl_eu_80664300 = (u32)shim;

        // Register with this (parent) -- insertTop = false
        Regist__8CProcessFP8CProcessb((CProcess*)shim, _this, false);
        result = (CMenuPTState*)lbl_eu_80664300;
    }

    return result;
}

// Singleton-existence query (retail-unmangled symbol; see func_80192C2C).
extern "C" unsigned long func_80192BD0() { return lbl_eu_80664300 != 0; }
// FULL_MATCH: vtable adjustor thunks (IScnRender secondary base at offset +0x58).
// Called through the IScnRender subobject vtable slot, `this` points at
// CMenuPTState + 0x58, so subtract 0x58 before forwarding to the full-object
// member. The retail emits exactly subi r3,r3,0x58; b <member>.
void CMenuPTState::func_80192BE4() {
    cbRenderBefore__12CMenuPTStateFv((CMenuPTState*)((char*)this - 0x58));
}

void CMenuPTState::func_80192BEC() {
    __dt__12CMenuPTStateFv((CMenuPTState*)((char*)this - 0x58));
}

// ---------------------------------------------------------------------------
// UnkClass_80192BF4: PT-state FX/timer accumulator
// ---------------------------------------------------------------------------

// us-80194310 - FUNC_80192BF4 constructor.
// Initializes the counter/timer/wealth block. Byte-identical (implicit float pool).
cf::UnkClass_80192BF4::UnkClass_80192BF4() {
    field_0x00 = 0;
    field_0x04 = 0.0f;
    field_0x08 = -1.0f;
}

// Initializes a UnkClass_80192BF4-like struct at offsets 0x00/0x04/0x08.
// Retail-unmangled symbol name (see func_80192C2C).
extern "C" void __ct__80192C10(cf::UnkClass_80192BF4* self) {
    self->field_0x00 = 0;
    self->field_0x04 = 0.0f;
    self->field_0x08 = -1.0f;
}

extern const f32 lbl_eu_80667A98;         // float pool: zero threshold
extern const f32 lbl_eu_80667A9C;         // float pool: reset value (-1.0f)
extern const double lbl_eu_80667AA0;       // double pool: 0x4330000080000000 magic

// us-80194348 - func_80192C2C.
// If the accumulator is still positive, bump the counter, then query the
// passed actor (vtable slot 0x308) for a count and store (count*2+6) into both
// the value and the timer fields.
// Retail-unmangled symbol: MWCC would mangle a plain free function here, so
// C linkage keeps the exact linker name.
extern "C" void func_80192C2C(cf::UnkClass_80192BF4* self, void* obj) {
    if (self->field_0x04 > lbl_eu_80667A98) {
        self->field_0x00++;
    }

    // Virtual call through slot 0x308 (getCount) -- MWCC stages the vtable
    // through r12 for real member virtual calls.
    int count = ((cf::CMenuPtStateActor*)obj)->getCount();

    // Signed int->float conversion (MWCC 0x4330000080000000 magic; the
    // builtin conversion is the only frsp-free byte-exact shape).
    f32 val = (f32)(count * 2 + 6);
    self->field_0x08 = val;
    self->field_0x04 = val;
}

// ---------------------------------------------------------------------------
// func_80192CB0: continuous PT-state timer update
// ---------------------------------------------------------------------------

// Opaque list holder/list views (CPartyStateWin.hpp declares the shared
// retail-unmangled helper family with void* parameters).
struct CEnumListHolder {
    void* list; // 0x0
    u32 handle; // 0x4
};

// The list returned by func_80043F18; element count is at offset 0x620.
struct CEnumList {
    u8 _00[0x620];
    u32 count; // 0x620
};

// us-801943cc - func_80192CB0.
// If the timer is still running (>threshold), scan the battle object list for any
// actor carrying status 0x10 or 0xf. If none is found, decay the timer by the
// per-frame delta; when it crosses zero reset the whole block.
// NOTE: the element address is (elem - 0x3e9c) + 8, i.e. the status getter is
// invoked on a fixed header offset within the battle-actor record.
// Retail-unmangled symbol (see func_80192C2C).
extern "C" void func_80192CB0(cf::UnkClass_80192BF4* self) {
    // Written as a '>' guard so MWCC tests the GT bit (plain bge-style skip),
    // matching retail; '<' forms emit an extra cror.
    if (self->field_0x04 > lbl_eu_80667A98) {
        CEnumListHolder holder;
        func_80043D90(&holder);
        func_800F4A98(func_80043F18(&holder), 0x80000000, 0);

        int found;
        u32 i = 0;
        // List handle is re-fetched twice per iteration (count check + element
        // fetch); never cached in a register across calls.
        while (((CEnumList*)func_80043F18(&holder))->count > i) {
            // Test-before-copy shape: retail tests r3 straight from the call,
            // copies it, then subtracts the record offset when non-null.
            u32 base = (u32)func_800F6EAC(func_80043F18(&holder), i);
            if (base != 0) {
                base -= 0x3e9c;
            }
            if (func_80148778((u8*)(base + 8), 0x10) != 0 ||
                func_80148778((u8*)(base + 8), 0xf) != 0) {
                __dt__80043E88(&holder, -1);
                found = 1;
                goto after;
            }
            i++;
        }
        __dt__80043E88(&holder, -1);
        found = 0;
    after:
        if (found == 0) {
            // No matching actor: decay the timer; when it crosses the threshold,
            // reset the block.
            // The product is materialized separately -- writing it as one
            // expression makes MWCC contract to fnmsubs, which retail lacks.
            f32 decay = lbl_eu_80667AA8 * func_80496288(lbl_eu_80663E14);
            self->field_0x04 -= decay;
            if (self->field_0x04 <= lbl_eu_80667A98) {
                self->field_0x00 = 0;
                self->field_0x04 = lbl_eu_80667A98;
                self->field_0x08 = lbl_eu_80667A9C;
            }
        }
    }
}