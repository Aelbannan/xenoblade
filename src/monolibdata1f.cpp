/* Data-only TU for monolibdata1f (retail: .bss 0x80656BEC..0x80656E88, 0x29C
 * bytes, align 4, NO relocs). Replaces the generated monolibdata1f.s dump.
 *
 * .bss layout (MWCC emits .bss in declaration order; retail DOL addresses):
 *   +0x80656BF0  ml::CVec4::zero/unitX/unitY/unitZ/unit        (5 * 0x10)
 *   +0x80656C40  lbl_eu_80656C40 / lbl_eu_80656C58 / lbl_eu_80656C64
 *          (0x18 + 0xC + 0x64 = 0x88) -- this is the ml::CCol3 static color
 *          group storage: __sinit_\CCol3_cpp (retail CCol3.o, linked) writes
 *          11 CCol3 objects (11 * 0xC = 0x84 bytes) from &lbl_eu_80656C40;
 *          the splitter chunked the range into the three anonymous symbols
 *          (the last 4 bytes are alignment pad to the 8-aligned CCol4 group).
 *          No named CCol3 statics exist in the retail symbol map and the only
 *          cross-unit reference is CCol3.o's reloc to lbl_eu_80656C40, so the
 *          splitter names are load-bearing.
 *   +0x80656CC8  ml::CCol4::white..zero                         (12 * 0x10)
 *   +0x80656D88  ml::CMat34::zero / identity                    (2 * 0x30)
 *   +0x80656DE8  ml::CMat44::zero / identity                    (2 * 0x40)
 *   +0x80656E68  ml::CQuat::zero / identity                     (2 * 0x10)
 *
 * The 4 bytes at 0x80656BEC..0x80656BF0 (retail split .o .bss+0..4, local pad
 * symbol) are NOT emitted here: they are a link-time alignment gap -- the
 * previous unit's .bss (ml::CVec3 group, CVec3.link.o) ends at 0x80656BEC and
 * this TU's .bss is 8-aligned (MWCC 8-aligns %8-sized objects like CVec4), so
 * the linker places this section at 0x80656BF0 and re-inserts the same 4
 * bytes as inter-unit alignment. Every symbol below lands at its exact retail
 * DOL address and the section ends at exactly 0x80656E88 (664 bytes).
 *
 * Why ctor-less mirror structs instead of the real header classes:
 * the retail DOL zero-fills this .bss and the per-class __sinit functions
 * (which live in the .text units) fill the values at runtime, so this data TU
 * must emit NOBITS storage only. The real classes (libs/monolib/include/
 * monolib/math/*.hpp) have user-provided constructors, which make MWCC emit
 * a __sinit + .ctors for this TU -- the retail data unit has none. These
 * mirrors keep the exact field layout while staying ctor-less. The objects
 * are declared at global scope so the symbol names are the literal retail
 * mangled names (C++ does not mangle global-scope data identifiers).
 */

namespace ml {
    /* Ctor-less layout mirrors of CVec4.hpp / CCol4.hpp / CMat34.hpp /
     * CMat44.hpp / CQuat.hpp (identical field layouts, no constructors). */
    struct CVec4 { float x, y, z, w; };
    struct CCol4 { float r, g, b, a; };
    struct CMat34 { float m[3][4]; };
    struct CMat44 { float m[4][4]; };
    struct CQuat { float x, y, z, w; };
}

ml::CVec4 zero__Q22ml5CVec4;    /* ml::CVec4::zero */
ml::CVec4 unitX__Q22ml5CVec4;   /* ml::CVec4::unitX */
ml::CVec4 unitY__Q22ml5CVec4;   /* ml::CVec4::unitY */
ml::CVec4 unitZ__Q22ml5CVec4;   /* ml::CVec4::unitZ */
ml::CVec4 unit__Q22ml5CVec4;    /* ml::CVec4::unit */

/* ml::CCol3 static color group storage (see header comment). */
float lbl_eu_80656C40[6];   /* 0x18 bytes (2 * CCol3) */
float lbl_eu_80656C58[3];   /* 0x0C bytes (1 * CCol3) */
float lbl_eu_80656C64[25];  /* 0x64 bytes (8 * CCol3 + 4B pad to 8-align) */

ml::CCol4 white__Q22ml5CCol4;   /* ml::CCol4::white */
ml::CCol4 gray__Q22ml5CCol4;    /* ml::CCol4::gray */
ml::CCol4 black__Q22ml5CCol4;   /* ml::CCol4::black */
ml::CCol4 red__Q22ml5CCol4;     /* ml::CCol4::red */
ml::CCol4 green__Q22ml5CCol4;   /* ml::CCol4::green */
ml::CCol4 blue__Q22ml5CCol4;    /* ml::CCol4::blue */
ml::CCol4 yellow__Q22ml5CCol4;  /* ml::CCol4::yellow */
ml::CCol4 cyan__Q22ml5CCol4;    /* ml::CCol4::cyan */
ml::CCol4 magenta__Q22ml5CCol4; /* ml::CCol4::magenta */
ml::CCol4 salmon__Q22ml5CCol4;  /* ml::CCol4::salmon */
ml::CCol4 orange__Q22ml5CCol4;  /* ml::CCol4::orange */
ml::CCol4 zero__Q22ml5CCol4;    /* ml::CCol4::zero */

ml::CMat34 zero__Q22ml6CMat34;      /* ml::CMat34::zero */
ml::CMat34 identity__Q22ml6CMat34;  /* ml::CMat34::identity */

ml::CMat44 zero__Q22ml6CMat44;      /* ml::CMat44::zero */
ml::CMat44 identity__Q22ml6CMat44;  /* ml::CMat44::identity */

ml::CQuat zero__Q22ml5CQuat;      /* ml::CQuat::zero */
ml::CQuat identity__Q22ml5CQuat;  /* ml::CQuat::identity */
