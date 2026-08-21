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

extern u32 lbl_eu_806659A0;
extern CEffectParam lbl_eu_8065FC08;

void func_804C8690(u8 flag, const CEffectParam* src) {
    lbl_eu_806659A0 = flag;
    if (src) {
        // 2+1 word split: load both pair words before the dst base addi and
        // store hi-before-lo, then the word at 0x08 (retail schedule).
        u32 lo = *(const u32*)src;
        u32 hi = *(const u32*)((const char*)src + 4);
        u8* dst = (u8*)&lbl_eu_8065FC08;
        *(u32*)(dst + 4) = hi;
        *(u32*)dst = lo;
        *(u32*)(dst + 8) = src->field_0x08;
    }
}

// ---------------------------------------------------------------------------
// sinit_804C86C0: static-initializer writing the 11 float constants into the
// 3-block array rooted at lbl_eu_8065FBE8. The float constants are referenced
// through their retail sdata2 symbols so the lfs relocs stay pinned.
// ---------------------------------------------------------------------------

// One default-parameter block: head float plus a 3-float tail.
struct SInitBlk {
    f32 head;
    f32 tail[3];
};

extern SInitBlk lbl_eu_8065FBE8[3];
extern f32 lbl_eu_8066B088;  // 1.0f
extern f32 lbl_eu_8066B08C;  // 0.0f
extern f32 lbl_eu_8066B090;  // 0.8f
extern f32 lbl_eu_8066B094;  // 5.0f
extern f32 lbl_eu_8066B098;  // 393.0f

void sinit_804C86C0() {
    // Default-parameter table at lbl_eu_8065FBE8: three 16-byte blocks.
    // Retail keeps one address register per block (addi rX, r6, K); the head
    // float of each block is written through a flat float cursor while the
    // 3-float tails go through the per-block struct pointers.
    SInitBlk* t0 = lbl_eu_8065FBE8;
    SInitBlk* t1 = t0 + 1;
    SInitBlk* t2 = t0 + 2;
    f32* h = &t0->head;

    h[0] = lbl_eu_8066B08C;
    t0->tail[0] = lbl_eu_8066B088;
    t0->tail[1] = lbl_eu_8066B08C;
    t0->tail[2] = lbl_eu_8066B090;
    h[4] = lbl_eu_8066B088;
    t1->tail[0] = lbl_eu_8066B088;
    t1->tail[1] = lbl_eu_8066B088;
    t1->tail[2] = lbl_eu_8066B090;
    h[8] = lbl_eu_8066B094;
    t2->tail[0] = lbl_eu_8066B098;
    t2->tail[1] = lbl_eu_8066B08C;
}

// --- hard-symbol stubs (scaffold_hard_symbols) ---

// ===== Dissolved monolibdata2 (blob surgery) data owned by this TU =====
// [.bss] 0x8065FBE8-0x8065FCA0 (0xB8 = 184B) zero-fill. Retail splits this
// contiguous zero block into lbl_eu_8065FBE8(32B) + lbl_eu_8065FC08(16B) +
// lbl_eu_8065FC18(136B); the first two are contiguous (sinit writes 12 floats
// from 8065FBE8 across both). We emit 48B from 8065FBE8 + 136B at 8065FC18 so
// the total bss sits at the retail 184B; lbl_eu_8065FC08 stays an extern alias
// into the 8065FBE8 tail (resolved at link to the same bss region).
SInitBlk lbl_eu_8065FBE8[3];   // 48B covers retail 8065FBE8(32) + 8065FC08(16)
u8 lbl_eu_8065FC18[136];   // 136B
// [.sbss] 0x806659A0-0x806659A8 (8B)
u32 lbl_eu_806659A0;   // sbss word 0
u32 lbl_eu_806659A4;   // sbss word 1
