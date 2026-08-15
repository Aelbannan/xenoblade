/* Data-only TU for monolibdata1f (retail: .bss 0x80656BEC..0x80656E88, 0x29C
 * bytes, align 4, NO relocs). Replaces the generated monolibdata1f.s dump.
 *
 * .bss layout (MWCC emits .bss in definition order; retail DOL addresses):
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
 * The static members are declared against ctor-less layout mirrors of the
 * real classes (libs/monolib/include/monolib/math/*.hpp) rather than the
 * headers themselves: the retail DOL zero-fills this .bss and the per-class
 * __sinit functions (which live in the .text units) fill the values at
 * runtime, so this data TU must emit NOBITS storage only. The real classes
 * have user-provided constructors, which make MWCC emit a __sinit + .ctors
 * for this TU -- the retail data unit has none. The mirrors keep the exact
 * field layout while staying ctor-less; the out-of-class static member
 * definitions make MWCC emit the identical mangled symbols (e.g.
 * zero__Q22ml5CVec4) that the retail link depends on.
 */

namespace ml {
    /* Ctor-less layout mirrors of CVec4.hpp / CCol4.hpp / CMat34.hpp /
     * CMat44.hpp / CQuat.hpp (identical field layouts, no constructors). */
    struct CVec4 {
        static CVec4 zero;
        static CVec4 unitX;
        static CVec4 unitY;
        static CVec4 unitZ;
        static CVec4 unit;
        float x, y, z, w;
    };
    struct CCol4 {
        static CCol4 white;
        static CCol4 gray;
        static CCol4 black;
        static CCol4 red;
        static CCol4 green;
        static CCol4 blue;
        static CCol4 yellow;
        static CCol4 cyan;
        static CCol4 magenta;
        static CCol4 salmon;
        static CCol4 orange;
        static CCol4 zero;
        float r, g, b, a;
    };
    struct CMat34 {
        static CMat34 zero;
        static CMat34 identity;
        float m[3][4];
    };
    struct CMat44 {
        static CMat44 zero;
        static CMat44 identity;
        float m[4][4];
    };
    struct CQuat {
        static CQuat zero;
        static CQuat identity;
        float x, y, z, w;
    };
}

ml::CVec4 ml::CVec4::zero;
ml::CVec4 ml::CVec4::unitX;
ml::CVec4 ml::CVec4::unitY;
ml::CVec4 ml::CVec4::unitZ;
ml::CVec4 ml::CVec4::unit;

/* ml::CCol3 static color group storage (see header comment). */
float lbl_eu_80656C40[6];   /* 0x18 bytes (2 * CCol3) */
float lbl_eu_80656C58[3];   /* 0x0C bytes (1 * CCol3) */
float lbl_eu_80656C64[25];  /* 0x64 bytes (8 * CCol3 + 4B pad to 8-align) */

ml::CCol4 ml::CCol4::white;
ml::CCol4 ml::CCol4::gray;
ml::CCol4 ml::CCol4::black;
ml::CCol4 ml::CCol4::red;
ml::CCol4 ml::CCol4::green;
ml::CCol4 ml::CCol4::blue;
ml::CCol4 ml::CCol4::yellow;
ml::CCol4 ml::CCol4::cyan;
ml::CCol4 ml::CCol4::magenta;
ml::CCol4 ml::CCol4::salmon;
ml::CCol4 ml::CCol4::orange;
ml::CCol4 ml::CCol4::zero;

ml::CMat34 ml::CMat34::zero;
ml::CMat34 ml::CMat34::identity;

ml::CMat44 ml::CMat44::zero;
ml::CMat44 ml::CMat44::identity;

ml::CQuat ml::CQuat::zero;
ml::CQuat ml::CQuat::identity;
