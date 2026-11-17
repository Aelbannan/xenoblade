// TU: monolib/src/effect/code_804C8684
//
// These functions exist in the retail binary but are no-ops
// (each compiles to a single blr instruction).

#include <harness_catalog.h>

extern "C" void EffCtl_EmptyStub84(void) {}
extern "C" void EffCtl_EmptyStub88(void) {}
extern "C" void EffCtl_EmptyStub8C(void) {}

// ---------------------------------------------------------------------------
// EffCtl_StoreFlagAndParams: unconditionally store the flag byte (stb - the destination
// lbl_eu_806659A0 is a single u8) into lbl_eu_806659A0, then (when the source
// pointer is non-null) copy its 12-byte parameter block into the global
// lbl_eu_8065FC08. The retail copies the first 8 bytes as a 2-word pair
// (reversed store order) and the last word separately, so the source keeps
// that 2+1 split in the struct shape.
// ---------------------------------------------------------------------------

// EffCtl_StoreFlagAndParams's 12-byte copy block: an 8-byte pair chunk + one word.
// Sibling matched code (EffFxSetupSceneXforms) shows MWCC emits per-pair lwz/stw
// chunks (hi-before-lo) for u64 member copies.
struct CEffectParam {
    u64 pair;          // 0x00
    u32 field_0x08;    // 0x08
}; struct SInitBlk;  // complete below (sinit block); fwd for the FC08 extern.

extern u8 lbl_eu_806659A0;
extern SInitBlk lbl_eu_8065FC08;

extern "C" void EffCtl_StoreFlagAndParams(u8 flag, const CEffectParam* src) {
    lbl_eu_806659A0 = flag;
    if (src) {
        // 2+1 word split: load both pair words before the dst base addi and
        // store hi-before-lo, then the word at 0x08 (retail schedule).
        u32 lo = *(const u32*)src;
        u32 hi = *(const u32*)((const char*)src + 4);
        u32* dst = (u32*)&lbl_eu_8065FC08;
        dst[1] = hi;
        dst[0] = lo;
        dst[2] = src->field_0x08;
    }
}

// ---------------------------------------------------------------------------
// sinit_804C86C0: static-initializer writing the 11 float constants into the
// 3-block array rooted at lbl_eu_8065FBE8. The float constants are referenced
// through their retail sdata2 symbols so the lfs relocs stay pinned.
// Block 2 is addressed as p[2] through the local pointer (== FC08's address)
// so no FC08/FBE8+32 reloc is emitted; MWCC still emits one partial+LO store
// (extra table LO reloc: 9 relocs vs retail's 7). Reloc-exact is unreachable
// for the same reason as the bases (any +0 store takes the partial+LO form
// once a completed copy exists).
//
// KNOWN RESIDUAL (witness-closed, FULL-only): retail materializes four GPR
// bases (r6 = table, r5 = r6+0, r4 = r6+0x10, r3 = r6+0x20), splitting heads
// (via r6) from per-block tails. 30+ probe shapes under the TU's exact
// GC/3.0a5.2 -O4,p flags (see .scratch/sinit/) prove MWCC collapses every
// same-symbol constant-offset derivation into ONE base + ONE completed copy
// (2 bases max), regardless of: pointer decls (&arr[i]/base+i/chained p+1),
// subobject vs block pointers, casts, inlined helpers, unrolled loops,
// statement/decl order, volatile data, opt pragmas (opt_propagation/peephole/
// optimize_for_size), ipa modes, or compiler version (all GC 1.x-3.x + Wii).
// Two symbols give 2 bases each, confirming the per-symbol rule. The retired
// angles additionally included the btm-style mixed-form and reassignment
// shapes. Sibling sinit_8049FC60 has the same wall. Next angles: explicit
// unroll-pragma + per-iter-select loop nests; #pragma scheduling permutations
// for the lfs/store interleave once bases match. Emission here is
// semantically identical to retail (same 11 addresses + values); only GPR
// base allocation differs (2 vs 4), which the renaming witness cannot certify
// (gates: size equality + non-reg-field equality). Size stays in budget.
// ---------------------------------------------------------------------------

// One default-parameter block: head float plus a 3-float tail vector.
struct SInitV3 {
    f32 x, y, z;
};
struct SInitBlk {
    f32 head;
    SInitV3 tail;
};

extern SInitBlk lbl_eu_8065FBE8[2];

extern "C" void sinit_804C86C0() {
    // Every block address flows through the single table pointer (see the
    // residual note above for why only 2 of retail's 4 GPR bases survive).
    SInitBlk* p = lbl_eu_8065FBE8;
    p[0].head = 1.0f;
    p[0].tail.x = 0.0f;
    p[0].tail.y = 0.0f;
    p[0].tail.z = 0.8f;
    p[1].head = 1.0f;
    p[1].tail.x = 1.0f;
    p[1].tail.y = 1.0f;
    p[1].tail.z = 0.8f;
    p[2].head = 5.0f;
    p[2].tail.x = 395.0f;
    p[2].tail.y = 0.0f;
}

// --- hard-symbol stubs (scaffold_hard_symbols) ---

// ===== Dissolved monolibdata2 (blob surgery) data owned by this TU =====
// [.bss] 0x8065FBE8-0x8065FCA0 (0xB8 = 184B) zero-fill. Retail splits this
// contiguous zero block into lbl_eu_8065FBE8(32B) + lbl_eu_8065FC08(16B) +
// lbl_eu_8065FC18(136B); FC08 is one SInitBlk (16B), written by sinit as the
// third table block (retail sinit references both FBE8 and FC08).
SInitBlk lbl_eu_8065FBE8[2];   // 32B: table blocks 0-1
SInitBlk lbl_eu_8065FC08;      // 16B @0x20: table block 2 (also the 12B copy dst)
u8 lbl_eu_8065FC18[136];   // 136B
// [.sbss] 0x806659A0-0x806659A8 (8B)
u8 lbl_eu_806659A0;   // sbss byte 0
u32 lbl_eu_806659A4;   // sbss word 1
