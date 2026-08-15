// TU: monolib/src/effect/code_804C8684
//
// These functions exist in the retail binary but are no-ops
// (each compiles to a single blr instruction).

#include <harness_catalog.h>

void func_804C8684(void) {}
void func_804C8688(void) {}
extern "C" void func_804C868C(void) {}

// ---------------------------------------------------------------------------
// func_804C8690: unconditionally store the flag byte into lbl_eu_806659A0,
// then (when the source pointer is non-null) copy its 3-word parameter block
// into the global lbl_eu_8065FC08. The retail copies the first 8 bytes as a
// 2-word pair (reversed store order) and the last word separately, so the
// source keeps that 2+1 split in the struct shape.
// ---------------------------------------------------------------------------

// func_804C8690's 12-byte copy block: an 8-byte pair chunk + one word.
// Sibling matched code (func_804CC808) shows MWCC emits per-pair lwz/stw
// chunks (hi-before-lo) for u64 member copies.
struct CEffectParam {
    u64 pair;          // 0x00
    u32 field_0x08;    // 0x08
};

extern u8 lbl_eu_806659A0;
extern CEffectParam lbl_eu_8065FC08;

void func_804C8690(u8 flag, const CEffectParam* src) {
    lbl_eu_806659A0 = flag;
    if (src) {
        // Explicit pair local + pointer: MWCC emits the pair loads, then the
        // base addi, then the reversed pair stores, then the word load+store
        // (retail: lwz 0; lwz 4; addi; stw 4; stw 0; lwz 8; stw 8).
        u64 pair = *(const u64*)src;
        u8* dst = (u8*)&lbl_eu_8065FC08;
        *(u64*)dst = pair;
        *(u32*)(dst + 8) = src->field_0x08;
    }
}

// ---------------------------------------------------------------------------
// sinit_804C86C0: static-initializer writing the 11 float constants into the
// 3-block array rooted at lbl_eu_8065FBE8. The float constants are referenced
// through their retail sdata2 symbols so the lfs relocs stay pinned.
// ---------------------------------------------------------------------------

struct CEffectBlk {
    f32 field_0x00;
    f32 field_0x04;
    f32 field_0x08;
    f32 field_0x0c;
};

extern f32 lbl_eu_8065FBE8[12];
extern f32 lbl_eu_8066B088;  // 1.0f
extern f32 lbl_eu_8066B08C;  // 0.0f
extern f32 lbl_eu_8066B090;  // 0.8f
extern f32 lbl_eu_8066B094;  // 5.0f
extern f32 lbl_eu_8066B098;  // 393.0f

void sinit_804C86C0() {
    // Default-parameter table at lbl_eu_8065FBE8: three 16-byte blocks.
    // Retail keeps one address register per block (addi rX, r6, K) with each
    // block's first field folded back into the base. The array-decay pointer
    // form reproduces that (address-of forms fold into one register); same
    // shape as the matched sinit_8049FC60.
    f32* v0 = lbl_eu_8065FBE8;
    f32* v1 = v0 + 4;
    f32* v2 = v0 + 8;

    v0[0] = lbl_eu_8066B088;
    v0[1] = lbl_eu_8066B08C;
    v0[2] = lbl_eu_8066B08C;
    v0[3] = lbl_eu_8066B090;
    v1[0] = lbl_eu_8066B088;
    v1[1] = lbl_eu_8066B088;
    v1[2] = lbl_eu_8066B088;
    v1[3] = lbl_eu_8066B090;
    v2[0] = lbl_eu_8066B094;
    v2[1] = lbl_eu_8066B098;
    v2[2] = lbl_eu_8066B08C;
}

// --- hard-symbol stubs (scaffold_hard_symbols) ---
