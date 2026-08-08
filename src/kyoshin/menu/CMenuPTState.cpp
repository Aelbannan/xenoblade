// Cleaned TU for kyoshin/menu/CMenuPTState.
// FULL_MATCH: func_80192BE4, func_80192BEC (vtable adjustor thunks).

#include "kyoshin/menu/CMenuPTState.hpp"

// Retail CProcess / mem-manager / win-ctor imports (defined in other TUs).
extern "C" void __ct__8CProcessFv(CProcess* self);
extern "C" void Regist__8CProcessFP8CProcessb(CProcess* self, CProcess* parent, bool insertTop);
extern "C" void* getWorkMem__17CWorkThreadSystemFv();
extern "C" void* allocate__Q23mtl10MemManagerFUlUl(u32 size, u32 handle);
extern "C" void __ct__CBgTex(CBgTex* self, u8 arg);
extern "C" void __ct__14CPartyStateWinFUlUl(CPartyStateWin* self, u32 arg1, u32 arg2);

// Real member functions defined in code_8018F8D8.cpp -- retail slice names keep
// the exact (already-mangled) linker symbols so tail-call relocs match.
extern "C" void cbRenderBefore__12CMenuPTStateFv(CMenuPTState* self);
extern "C" void __dt__12CMenuPTStateFv(CMenuPTState* self);

// SDA globals (retail names; defined in common/sdata splits).
extern u32 lbl_eu_80664300;              // singleton pointer
extern u32 __ptmf_null[3];
extern char lbl_eu_8052BF70[];           // CProcess primary vtable
extern char lbl_eu_80532A38[];           // CMenuPTState final vtable
extern const f32 lbl_eu_80667AA8;        // float pool: timer decay multiplier

// Singleton factory constructor.
// MWCC allocating-ctor: not a real C++ ctor -- a free function that allocates a
// fresh CMenuPTState, builds it, registers it under `parent`, and returns it.
// Returns a null pointer if the singleton already exists.
extern "C" void __ct__CMenuPTState(CProcess* _this, CProcess* storedParent) {
    // If singleton already exists, return 0
    if (lbl_eu_80664300 != 0) {
        return;
    }

    // Allocate memory for the new object
    CMenuPTStateCtorShim* shim = (CMenuPTStateCtorShim*)allocate__Q23mtl10MemManagerFUlUl(
        0x6c70, (u32)getWorkMem__17CWorkThreadSystemFv());

    if (shim != 0) {
        // Construct CProcess (primary base) on the freshly allocated memory.
        __ct__8CProcessFv((CProcess*)shim);

        // vtable / terminal-state ptmf slots come from the retail's inlined
        // vtable-store sequence (see shim layout comment).
        shim->vtable = lbl_eu_8052BF70;
        u32* ptmf = __ptmf_null;
        char* vtFinal = lbl_eu_80532A38;

        u32 ptmf1 = ptmf[1];
        u32 ptmf0 = ptmf[0];
        void* iscnVtbl = vtFinal + 0x24;
        u32 ptmf2 = ptmf[2];

        shim->callbacks[0] = ptmf0;
        shim->callbacks[1] = ptmf1;
        shim->callbacks[2] = ptmf2;
        ptmf1 = ptmf[1];
        ptmf0 = ptmf[0];
        ptmf2 = ptmf[2];
        shim->callbacks[3] = ptmf0;
        shim->callbacks[4] = ptmf1;
        shim->callbacks[5] = ptmf2;

        shim->field54 = 0;
        shim->field55 = 0;

        CMenuPTState* obj = (CMenuPTState*)shim;
        shim->vtable = vtFinal;
        *(u32*)((u8*)obj + 0x58) = (u32)iscnVtbl;
        obj->mStoredParent = storedParent;

        __ct__CBgTex(&obj->mBgTex, 0);
        __ct__14CPartyStateWinFUlUl(&obj->mPartyStateWin, 0, 0);
        obj->mField_6C6C = 0;
    }

    // Store singleton
    lbl_eu_80664300 = (u32)shim;

    // Register with this (parent) -- insertTop = false
    Regist__8CProcessFP8CProcessb((CProcess*)shim, _this, false);
}

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

// Target: us-80194310 - FUNC_80192BF4 constructor.
// Initializes the counter/timer/wealth block. Byte-identical (implicit float pool).
cf::UnkClass_80192BF4::UnkClass_80192BF4() {
    field_0x00 = 0;
    field_0x04 = 0.0f;
    field_0x08 = -1.0f;
}

// Initializes a UnkClass_80192BF4-like struct at offsets 0x00/0x04/0x08.
extern "C" void __ct__80192C10(cf::UnkClass_80192BF4* self) {
    self->field_0x00 = 0;
    self->field_0x04 = 0.0f;
    self->field_0x08 = -1.0f;
}

// Target: us-80194348 - func_80192C2C.
// If the accumulator is still positive, bump the counter, then query the
// passed actor (vtable slot 0x308) for a count and store (count*2+6) into both
// the value and the timer fields.
extern "C" void func_80192C2C(cf::UnkClass_80192BF4* self, void* obj) {
    if (self->field_0x04 > 0.0f) {
        self->field_0x00++;
    }

    // Call virtual function at vtable slot 0x61 (offset 0x308): returns a count.
    int count = ((int (*)(void*))((void**)obj)[0x308 / 4])(obj);

    float val = (float)(count * 2 + 6);
    self->field_0x08 = val;
    self->field_0x04 = val;
}

// ---------------------------------------------------------------------------
// func_80192CB0: continuous PT-state timer update
// ---------------------------------------------------------------------------

// Opaque list holder used with func_80043D90 / func_80043F18 / __dt__80043E88.
typedef struct {
    void* list; // 0x0
    u32 handle; // 0x4
} CEnumListHolder;

// The list returned by func_80043F18; element count is at offset 0x620.
typedef struct {
    u8 _00[0x620];
    u32 count; // 0x620
} CEnumList;

extern "C" void func_80043D90(CEnumListHolder* holder);
extern "C" CEnumList* func_80043F18(CEnumListHolder* holder);
extern "C" void __dt__80043E88(CEnumListHolder* holder, s16 arg);
extern "C" void func_800F4A98(void* list, int type, int value);
extern "C" void* func_800F6EAC(void* list, u32 idx);
extern "C" int func_80148778(void* obj, int id);
extern "C" f32 func_80496288(void* scene);
extern void* lbl_eu_80663E14;

// Target: us-801943cc - func_80192CB0.
// If the timer is still running (>0), scan the battle object list for any actor
// carrying status 0x10 or 0xf. If none is found, decay the timer by the per-frame
// delta; when it crosses zero reset the whole block.
extern "C" void func_80192CB0(cf::UnkClass_80192BF4* self) {
    if (self->field_0x04 <= 0.0f) {
        return;
    }

    CEnumListHolder holder;
    func_80043D90(&holder);
    func_800F4A98(func_80043F18(&holder), 0x8000, 0);

    int found = 0;
    for (u32 i = 0; i < func_80043F18(&holder)->count; i++) {
        void* elem = func_800F6EAC(func_80043F18(&holder), i);
        u8* p = elem ? (u8*)elem - 0x3e9c : 0;
        if (func_80148778((u8*)p + 8, 0x10) || func_80148778((u8*)p + 8, 0xf)) {
            found = 1;
            break;
        }
    }
    __dt__80043E88(&holder, -1);
    if (found) {
        return;
    }

    // No matching actor: decay the timer; when it hits zero, reset the block.
    self->field_0x04 -= lbl_eu_80667AA8 * func_80496288(lbl_eu_80663E14);
    if (self->field_0x04 <= 0.0f) {
        self->field_0x00 = 0;
        self->field_0x04 = 0.0f;
        self->field_0x08 = -1.0f;
    }
}