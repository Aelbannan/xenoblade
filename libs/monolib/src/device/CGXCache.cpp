// Auto-scaffolded catalog TU for monolib/src/device/CGXCache
// Replace stubs with high-level C/C++ during decomp.

#include <harness_catalog.h>
#include <decomp.h>
#include <string.h>
#include <revolution/GX.h>
#include "functions.hpp"
#include "monolib/math/CCol4.hpp"
#include "monolib/math/CMat34.hpp"

// ==== Dissolved monolibdata2 (blob surgery) data owned by this TU ====
// .rodata 0x80522A60-0x80522AA8, .sdata 0x80663620-0x80663680, and the
// vtables in .data 0x8056BFC8-0x8056C000. Each slice is emitted as a single
// forced-section word array whose bytes equal the retail bytes; pointer slots
// are written as extern-named refs so the .rela entries name the retail
// symbols (their real definitions resolve at DOL link via the CGXCache.o
// add_symbols manifest / the monolib shared blob).

// Retail-named externs referenced by the RTTI locators + vtables below.
extern "C" u32 lbl_eu_80522A60;   // .rodata RTTI "CGXCache"
extern "C" u32 lbl_eu_80522A6C;   // .rodata RTTI "CMsgParam<32>"
extern "C" u32 lbl_eu_80522A7C;   // .rodata RTTI "IStateCache"
extern "C" u32 lbl_eu_80522A88;   // .rodata RTTI "DeviceSystem1"
extern "C" u32 lbl_eu_80522A98;   // .rodata RTTI "DeviceSystem2"
extern "C" u32 lbl_eu_8056BFD8[];   // .data vtbl (CMsgParam-32 leafer)
extern "C" u32 lbl_eu_80663658;   // .sdata RTTI locator
extern "C" u32 lbl_eu_80663660;   // .sdata RTTI locator
extern "C" u32 lbl_eu_80663668;   // .sdata RTTI locator
extern "C" u32 lbl_eu_80663670;   // .sdata RTTI locator

// ---- .rodata (0x48): RTTI name strings ----
__declspec(section ".rodata") __attribute__((aligned(8)))
u32 rodata_CGXCache[18] = {
    0x43475843, 0x61636865, 0x00000000, 0x434d7367, 0x50617261, 0x6d3c3332, 0x3e000000, 0x49537461,
    0x74654361, 0x63686500, 0x44657669, 0x63655379, 0x7374656d, 0x31000000, 0x44657669, 0x63655379,
    0x7374656d, 0x32000000
};

// ---- .sdata (0x60): window/ring u32 literals + RTTI locators ----
__declspec(section ".sdata") __attribute__((aligned(8)))
u32 sdata_CGXCache[24] = {
    0x00000001, 0x00FFFFFF, 0x00000001, 0x00000001, 0x00000001, 0x00000001, 0x00000001, 0x00000000,
    0x00000000, 0x00000000, 0x00000001, 0x00000002, 0x00000003, 0x00000000,
    (u32)&lbl_eu_80522A60, (u32)&lbl_eu_8056BFD8,
    (u32)&lbl_eu_80522A6C, 0x00000000,
    (u32)&lbl_eu_80522A7C, 0x00000000,
    (u32)&lbl_eu_80522A88, (u32)&lbl_eu_80522A98,
    0xFFFFFFFF, 0xFFFFFFFF
};

// ==== .sdata2 pool (0x8066A378-0x8066B52E, 0x11B6 = 4534 B) ====
// The shared float/double/RTTI-name constant pool owned by this TU. No
// internal relocs (pure floats/doubles/ASCII strings), so the raw bytes are
// emitted verbatim; alignment 8 matches retail.
__declspec(section ".sdata2") __attribute__((aligned(8)))
u8 sdata2_pool_CGXCache[4534] = {
    0x00, 0x00, 0x00, 0x00, 0x43, 0x7f, 0x00, 0x00, 0x3f, 0x80, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x43, 0x30, 0x00, 0x00,
    0x80, 0x00, 0x00, 0x00, 0x43, 0x30, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x3f, 0x72, 0x6e, 0x98, 0x40, 0x00, 0x00, 0x00,
    0x3d, 0xcc, 0xcc, 0xcd, 0x44, 0x7a, 0x00, 0x00, 0x43, 0x44, 0x65, 0x76, 0x69, 0x63, 0x65, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x43, 0x30, 0x00, 0x00, 0x80, 0x00, 0x00, 0x00, 0x43, 0x30, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x3f, 0x80, 0x00, 0x00, 0x3e, 0xcc, 0xcc, 0xcd, 0x3f, 0x19, 0x99, 0x9a, 0x44, 0x7a, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x3e, 0x99, 0x99, 0x9a, 0x3f, 0xa2, 0x06, 0x7b, 0x40, 0x00, 0x00, 0x00, 0x3f, 0x4c, 0xcc, 0xcd, 0x3f, 0x81, 0x9e, 0xc9,
    0x3f, 0x00, 0x00, 0x00, 0xcb, 0x80, 0x00, 0x00, 0xbf, 0x80, 0x00, 0x00, 0x43, 0x7f, 0x00, 0x00, 0x3f, 0x80, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x3f, 0x80, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xbf, 0x80, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x43, 0x30, 0x00, 0x00, 0x80, 0x00, 0x00, 0x00, 0x43, 0x30, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x47, 0x50, 0x43, 0x6f,
    0x73, 0x74, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x3f, 0x80, 0x00, 0x00, 0x40, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x43, 0x30, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x43, 0x30, 0x00, 0x00, 0x80, 0x00, 0x00, 0x00, 0x44, 0x65, 0x73, 0x6b,
    0x74, 0x6f, 0x70, 0x00, 0x3f, 0x00, 0x00, 0x00, 0x3f, 0x80, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x3f, 0x80, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x43, 0x7f, 0x00, 0x00, 0x42, 0x22, 0xf9, 0x83, 0x43, 0x30, 0x00, 0x00,
    0x80, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x3f, 0x4c, 0xcc, 0xcd, 0x3f, 0x80, 0x00, 0x00, 0x3f, 0x00, 0x00, 0x00,
    0x3f, 0xa2, 0x06, 0x7b, 0x40, 0x00, 0x00, 0x00, 0x3f, 0x81, 0x9e, 0xc9, 0xcb, 0x80, 0x00, 0x00, 0xbf, 0x80, 0x00, 0x00,
    0x43, 0x7f, 0x00, 0x00, 0x41, 0xe0, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x43, 0x30, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x43, 0x30, 0x00, 0x00, 0x80, 0x00, 0x00, 0x00, 0x3d, 0x23, 0xd7, 0x0a, 0x00, 0x00, 0x00, 0x00, 0x43, 0x4c, 0x69, 0x62,
    0x00, 0x00, 0x00, 0x00, 0x43, 0x4c, 0x69, 0x62, 0x43, 0x72, 0x69, 0x00, 0xff, 0x91, 0x00, 0x00, 0xff, 0x76, 0x00, 0x44,
    0x66, 0x00, 0xff, 0x32, 0x94, 0x00, 0x94, 0x94, 0xcb, 0x00, 0x05, 0xcf, 0x00, 0xff, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x3f, 0x80, 0x00, 0x00, 0x42, 0x48, 0x00, 0x00, 0x42, 0x6f, 0xc2, 0x8f, 0x2e, 0x61, 0x68, 0x78, 0x00, 0x00, 0x00, 0x00,
    0x3f, 0x80, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x42, 0xc8, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x43, 0x30, 0x00, 0x00,
    0x80, 0x00, 0x00, 0x00, 0x3d, 0x08, 0x88, 0x89, 0x41, 0x70, 0x00, 0x00, 0x43, 0x4c, 0x69, 0x62, 0x47, 0x33, 0x64, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x3f, 0xaf, 0x28, 0x6c, 0x3f, 0x80, 0x00, 0x00, 0x3f, 0x99, 0x99, 0x9a, 0x43, 0x30, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x3e, 0x80, 0x00, 0x00, 0x3f, 0xa0, 0x00, 0x00, 0x40, 0x80, 0x00, 0x00, 0x44, 0x2b, 0x80, 0x00,
    0x44, 0x20, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x43, 0x30, 0x00, 0x00, 0x80, 0x00, 0x00, 0x00, 0x43, 0x4c, 0x69, 0x62,
    0x48, 0x62, 0x6d, 0x00, 0x3d, 0x10, 0x00, 0x00, 0x3f, 0x35, 0x04, 0x81, 0x00, 0x00, 0x00, 0x00, 0x3f, 0x80, 0x00, 0x00,
    0xbf, 0x80, 0x00, 0x00, 0x3d, 0x80, 0x00, 0x00, 0x3f, 0x19, 0x99, 0x9a, 0x44, 0x27, 0x80, 0x00, 0x43, 0x64, 0x00, 0x00,
    0x43, 0x98, 0x00, 0x00, 0x43, 0xfa, 0x00, 0x00, 0x44, 0x2b, 0x80, 0x00, 0x43, 0xd0, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x43, 0x30, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x43, 0x4c, 0x69, 0x62, 0x56, 0x4d, 0x00, 0x00, 0x3f, 0x80, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x3f, 0x80, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x43, 0x30, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x43, 0x30, 0x00, 0x00, 0x80, 0x00, 0x00, 0x00, 0x40, 0x00, 0x00, 0x00, 0x40, 0x40, 0x00, 0x00, 0xc0, 0x00, 0x00, 0x00,
    0x3f, 0x00, 0x00, 0x00, 0x42, 0x22, 0xf9, 0x83, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xfe, 0x00, 0x00, 0x00, 0x00,
    0x3f, 0x80, 0x00, 0x00, 0x49, 0x74, 0x24, 0x00, 0x43, 0x30, 0x00, 0x00, 0x80, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xbf, 0x80, 0x00, 0x00, 0x46, 0x1c, 0x40, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x3f, 0x80, 0x00, 0x00, 0x43, 0x30, 0x00, 0x00, 0x80, 0x00, 0x00, 0x00, 0x3b, 0x80, 0x80, 0x81,
    0x00, 0x00, 0x00, 0x00, 0x43, 0x30, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x3f, 0x00, 0x00, 0x00, 0xbf, 0x00, 0x00, 0x00,
    0x3f, 0x80, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x3e, 0xaa, 0xaa, 0xab, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x3f, 0x80, 0x00, 0x00, 0x3c, 0x23, 0xd7, 0x0a, 0x3f, 0x00, 0x00, 0x00, 0x3e, 0x99, 0x99, 0x9a, 0x3d, 0xcc, 0xcc, 0xcd,
    0x3f, 0x19, 0x99, 0x9a, 0x41, 0x20, 0x00, 0x00, 0x43, 0x30, 0x00, 0x00, 0x80, 0x00, 0x00, 0x00, 0x3f, 0xc0, 0x00, 0x00,
    0x44, 0x7a, 0x00, 0x00, 0x41, 0xa0, 0x00, 0x00, 0x3e, 0xcc, 0xcc, 0xcd, 0x3d, 0xe1, 0x47, 0xae, 0x3b, 0x03, 0x12, 0x6f,
    0x3d, 0x08, 0x88, 0x89, 0x38, 0xd1, 0xb7, 0x17, 0x3c, 0xa3, 0xd7, 0x0a, 0x3c, 0xf5, 0xc2, 0x8f, 0x46, 0x1c, 0x40, 0x00,
    0x3e, 0x05, 0x1e, 0xb8, 0xbe, 0x99, 0x99, 0x9a, 0xbf, 0x7f, 0xf9, 0x72, 0x40, 0x00, 0x00, 0x00, 0xbf, 0x80, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x3a, 0x83, 0x12, 0x6f, 0x3f, 0xf3, 0x33, 0x33, 0x40, 0x00, 0x00, 0x00, 0x3f, 0x70, 0x62, 0x4d,
    0xd2, 0xf1, 0xa9, 0xfc, 0x3f, 0x80, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x43, 0x30, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x43, 0x30, 0x00, 0x00, 0x80, 0x00, 0x00, 0x00, 0xbf, 0x80, 0x00, 0x00, 0x3f, 0x00, 0x00, 0x00, 0x42, 0x22, 0xf9, 0x83,
    0x40, 0x00, 0x00, 0x00, 0x42, 0xb4, 0x00, 0x00, 0x3c, 0xc9, 0x0f, 0xdb, 0x42, 0xc8, 0x00, 0x00, 0x43, 0x7f, 0x00, 0x00,
    0x3d, 0xcc, 0xcc, 0xcd, 0x43, 0xb4, 0x00, 0x00, 0x3f, 0x80, 0x00, 0x00, 0x00, 0x00, 0x00, 0xff, 0x43, 0x7f, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x20, 0x00, 0x00, 0x00, 0x1f, 0x3d, 0x00, 0x00, 0x00, 0x41, 0x00, 0x00, 0x00,
    0x3e, 0x80, 0x00, 0x00, 0x40, 0x00, 0x00, 0x00, 0x43, 0x7f, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x3c, 0x23, 0xd7, 0x0a,
    0x3f, 0x00, 0x00, 0x00, 0x3f, 0x80, 0x00, 0x00, 0xc7, 0xc3, 0x50, 0x00, 0x3e, 0x99, 0x99, 0x9a, 0x3d, 0xcc, 0xcc, 0xcd,
    0x3f, 0x19, 0x99, 0x9a, 0x41, 0x20, 0x00, 0x00, 0x43, 0x30, 0x00, 0x00, 0x80, 0x00, 0x00, 0x00, 0x3f, 0xc0, 0x00, 0x00,
    0x38, 0xd1, 0xb7, 0x17, 0x3e, 0xcc, 0xcc, 0xcd, 0x46, 0x1c, 0x40, 0x00, 0x3e, 0x05, 0x1e, 0xb8, 0x3d, 0x08, 0x88, 0x89,
    0x3c, 0xa3, 0xd7, 0x0a, 0xbe, 0x99, 0x99, 0x9a, 0xbf, 0x7f, 0xf9, 0x72, 0x44, 0x7a, 0x00, 0x00, 0x41, 0xa0, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0xff, 0xff, 0xff, 0xff, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xff, 0xbf, 0x80, 0x00, 0x00,
    0x3f, 0x80, 0x00, 0x00, 0x3f, 0x00, 0x00, 0x00, 0x3c, 0xc9, 0x0f, 0xdb, 0x42, 0x22, 0xf9, 0x83, 0x40, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x43, 0x30, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xff, 0x00, 0x00, 0x00, 0xff,
    0x43, 0x7f, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x47, 0xc3, 0x50, 0x00, 0xc7, 0xc3, 0x50, 0x00, 0x00, 0x00, 0x00, 0x00,
    0xba, 0x83, 0x12, 0x6f, 0x3f, 0x80, 0x00, 0x00, 0x3a, 0x83, 0x12, 0x6f, 0x43, 0x30, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x3e, 0x4c, 0xcc, 0xcd, 0x3f, 0x4c, 0xcc, 0xcd, 0x43, 0x30, 0x00, 0x00, 0x80, 0x00, 0x00, 0x00, 0x40, 0x00, 0x00, 0x00,
    0x3f, 0x00, 0x00, 0x00, 0xc6, 0x1c, 0x40, 0x00, 0x3e, 0x80, 0x00, 0x00, 0x3c, 0x23, 0xd7, 0x0a, 0x41, 0x20, 0x00, 0x00,
    0x43, 0x48, 0x00, 0x00, 0x41, 0xf0, 0x00, 0x00, 0x42, 0x8c, 0x00, 0x00, 0x42, 0xc8, 0x00, 0x00, 0x42, 0x20, 0x00, 0x00,
    0x3d, 0xcc, 0xcc, 0xcd, 0x41, 0xa0, 0x00, 0x00, 0x43, 0x7f, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x3f, 0x80, 0x00, 0x00,
    0x3f, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x40, 0x80, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x3f, 0x80, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x40, 0x40, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x3f, 0x80, 0x00, 0x00,
    0x3f, 0x00, 0x00, 0x00, 0x3f, 0x80, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x3c, 0x23, 0xd7, 0x0a, 0x43, 0x30, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x3f, 0x80, 0x00, 0x00, 0x47, 0xc3, 0x50, 0x00, 0x3f, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x43, 0x30, 0x00, 0x00, 0x80, 0x00, 0x00, 0x00, 0x3e, 0x80, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x40, 0x00, 0x00, 0x00,
    0xbf, 0x80, 0x00, 0x00, 0xc7, 0xc3, 0x50, 0x00, 0x3e, 0xe6, 0x66, 0x66, 0x00, 0x00, 0x00, 0x00, 0x3f, 0x80, 0x00, 0x00,
    0xbf, 0x80, 0x00, 0x00, 0x3f, 0xc0, 0x00, 0x00, 0x62, 0x6c, 0x6f, 0x6f, 0x6d, 0x00, 0x00, 0x00, 0x6d, 0x6f, 0x76, 0x65,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x43, 0x7f, 0x00, 0x00, 0x41, 0xf0, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x43, 0x30, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x42, 0xc8, 0x00, 0x00, 0x3f, 0x80, 0x00, 0x00, 0x3a, 0x83, 0x12, 0x6f,
    0x3f, 0x00, 0x00, 0x00, 0x3d, 0xb8, 0x51, 0xec, 0x3e, 0x99, 0x99, 0x9a, 0xbf, 0x80, 0x00, 0x00, 0x3c, 0x23, 0xd7, 0x0a,
    0xbc, 0xa3, 0xd7, 0x0a, 0x3c, 0x23, 0xd7, 0x0b, 0x3d, 0xcc, 0xcc, 0xcd, 0xbf, 0x7f, 0xf9, 0x72, 0x40, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x53, 0x48, 0x49, 0x46, 0x00, 0x00, 0x00, 0x00, 0x53, 0x48, 0x54, 0x59, 0x00, 0x00, 0x00, 0x00,
    0x3f, 0x66, 0x66, 0x66, 0x3f, 0x80, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x43, 0x30, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x6d, 0x6f, 0x76, 0x65, 0x00, 0x00, 0x00, 0x00, 0x4a, 0x55, 0x61, 0x6d, 0x5f, 0x4c, 0x00, 0x00,
    0x4a, 0x55, 0x61, 0x6d, 0x5f, 0x52, 0x00, 0x00, 0x4a, 0x4c, 0x6c, 0x67, 0x5f, 0x4c, 0x00, 0x00, 0x4a, 0x4c, 0x6c, 0x67,
    0x5f, 0x52, 0x00, 0x00, 0x4a, 0x4c, 0x66, 0x74, 0x5f, 0x4c, 0x00, 0x00, 0x4a, 0x4c, 0x66, 0x74, 0x5f, 0x52, 0x00, 0x00,
    0x40, 0x00, 0x00, 0x00, 0x3d, 0xcc, 0xcc, 0xcd, 0x40, 0xa0, 0x00, 0x00, 0x3f, 0x33, 0x33, 0x33, 0x3e, 0x99, 0x99, 0x9a,
    0x00, 0x00, 0x00, 0x00, 0x3f, 0xf0, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x3c, 0x23, 0xd7, 0x0a, 0x3b, 0xa3, 0xd7, 0x0a, 0x3f, 0x00, 0x00, 0x00, 0x3c, 0xc9, 0x0f, 0xdb, 0x42, 0x22, 0xf9, 0x83,
    0xbd, 0xcc, 0xcc, 0xcd, 0x42, 0xc8, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x43, 0x53, 0x63, 0x6e, 0x4d, 0x65, 0x6d, 0x00,
    0x43, 0x53, 0x63, 0x6e, 0x00, 0x00, 0x00, 0x00, 0x41, 0xa0, 0x00, 0x00, 0x3c, 0x23, 0xd7, 0x0a, 0x3d, 0xa3, 0xd7, 0x0a,
    0x00, 0x00, 0x00, 0x00, 0x43, 0x30, 0x00, 0x00, 0x80, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x49, 0x74, 0x24, 0x00,
    0xc9, 0x74, 0x24, 0x00, 0x00, 0x00, 0x00, 0x00, 0x40, 0xa0, 0x00, 0x00, 0x3f, 0x00, 0x00, 0x00, 0x3f, 0x80, 0x00, 0x00,
    0xc0, 0x80, 0x00, 0x00, 0x40, 0x80, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x3e, 0xcc, 0xcc, 0xcd, 0x3e, 0xae, 0xae, 0xaf,
    0x3e, 0x96, 0x96, 0x97, 0xc0, 0x40, 0x00, 0x00, 0x3e, 0x80, 0x00, 0x00, 0xbf, 0x14, 0x09, 0xa2, 0x3f, 0x13, 0x54, 0x19,
    0x3f, 0x13, 0xd6, 0x52, 0x3f, 0x13, 0xbb, 0x1f, 0xbf, 0x10, 0x05, 0xe6, 0x3f, 0x0e, 0x77, 0x60, 0x3f, 0x1c, 0x83, 0x99,
    0x3f, 0x0d, 0xe3, 0xb9, 0x3f, 0x14, 0x03, 0xac, 0xbf, 0x19, 0x48, 0x02, 0x3d, 0xcc, 0xcc, 0xcd, 0x3a, 0xd8, 0x23, 0x42,
    0xbf, 0x65, 0x38, 0xef, 0x3e, 0xe3, 0xf8, 0xfd, 0x3f, 0x66, 0x66, 0x66, 0x3d, 0x23, 0xd7, 0x0a, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x3f, 0x80, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x43, 0x7f, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x3f, 0x80, 0x00, 0x00, 0x43, 0x7f, 0x00, 0x00, 0x43, 0x30, 0x00, 0x00, 0x80, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x3f, 0x00, 0x00, 0x00, 0x41, 0x20, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x3f, 0x80, 0x00, 0x00,
    0x3f, 0x99, 0x99, 0x9a, 0x00, 0x00, 0x00, 0x00, 0x3f, 0x80, 0x00, 0x00, 0x43, 0x30, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x43, 0x30, 0x00, 0x00, 0x80, 0x00, 0x00, 0x00, 0x3f, 0x00, 0x00, 0x00, 0x38, 0xd1, 0xb7, 0x17, 0x3f, 0x80, 0x00, 0x00,
    0x3f, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x43, 0x7f, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x3f, 0x80, 0x00, 0x00,
    0x43, 0x30, 0x00, 0x00, 0x80, 0x00, 0x00, 0x00, 0x3f, 0x80, 0x00, 0x00, 0x3f, 0x00, 0x00, 0x00, 0x3f, 0xc0, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x3f, 0xc9, 0x99, 0x99, 0xa0, 0x00, 0x00, 0x00, 0x3f, 0xe0, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x43, 0x30, 0x00, 0x00, 0x80, 0x00, 0x00, 0x00, 0x3f, 0x80, 0x00, 0x00, 0x3f, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x43, 0x30, 0x00, 0x00, 0x80, 0x00, 0x00, 0x00, 0x43, 0x7f, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x3c, 0x8e, 0xfa, 0x33, 0x00, 0x00, 0x00, 0x00, 0xc3, 0xfa, 0x00, 0x00, 0x3f, 0x80, 0x00, 0x00, 0x3f, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x43, 0x30, 0x00, 0x00, 0x80, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x3f, 0x72, 0x6e, 0x98,
    0x65, 0x70, 0x00, 0x00, 0x3f, 0x80, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x3b, 0x80, 0x00, 0x00, 0x3f, 0x80, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x43, 0x30, 0x00, 0x00, 0x80, 0x00, 0x00, 0x00, 0x3f, 0x99, 0x99, 0x9a, 0x00, 0x00, 0x00, 0x00,
    0x43, 0x30, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x43, 0x30, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x3f, 0x80, 0x00, 0x00, 0x44, 0x7a, 0x00, 0x00, 0x3d, 0xcc, 0xcc, 0xcd,
    0x42, 0xc8, 0x00, 0x00, 0x43, 0x7f, 0x00, 0x00, 0x3d, 0xea, 0x72, 0xeb, 0x3e, 0x99, 0x0a, 0xfe, 0x3f, 0x16, 0x2c, 0x23,
    0x00, 0x00, 0x00, 0x00, 0x43, 0x30, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x3f, 0x80, 0x00, 0x00, 0x3f, 0x00, 0x00, 0x00,
    0x43, 0x30, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x41, 0xf0, 0x00, 0x00, 0x3d, 0xcc, 0xcc, 0xcd,
    0x47, 0xa6, 0x04, 0x00, 0x40, 0x80, 0x00, 0x00, 0x3f, 0x72, 0x6e, 0x98, 0x43, 0x30, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x3f, 0x00, 0x00, 0x00, 0x3c, 0xc9, 0x0f, 0xdb, 0x42, 0x22, 0xf9, 0x83, 0x3f, 0x80, 0x00, 0x00, 0x40, 0x00, 0x00, 0x00,
    0xc0, 0x00, 0x00, 0x00, 0xbf, 0x80, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x43, 0x30, 0x00, 0x00, 0x80, 0x00, 0x00, 0x00,
    0x41, 0xa0, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x4e, 0x4f, 0x50, 0x00, 0x00, 0x00, 0x00, 0x00, 0x43, 0x4f, 0x4e, 0x53,
    0x54, 0x5f, 0x30, 0x00, 0x43, 0x4f, 0x4e, 0x53, 0x54, 0x5f, 0x31, 0x00, 0x43, 0x4f, 0x4e, 0x53, 0x54, 0x5f, 0x32, 0x00,
    0x43, 0x4f, 0x4e, 0x53, 0x54, 0x5f, 0x33, 0x00, 0x43, 0x4f, 0x4e, 0x53, 0x54, 0x5f, 0x34, 0x00, 0x43, 0x4f, 0x4e, 0x53,
    0x54, 0x5f, 0x49, 0x00, 0x4c, 0x44, 0x00, 0x00, 0x53, 0x54, 0x00, 0x00, 0x4c, 0x44, 0x5f, 0x41, 0x52, 0x47, 0x00, 0x00,
    0x53, 0x54, 0x5f, 0x41, 0x52, 0x47, 0x00, 0x00, 0x4c, 0x44, 0x5f, 0x30, 0x00, 0x00, 0x00, 0x00, 0x4c, 0x44, 0x5f, 0x31,
    0x00, 0x00, 0x00, 0x00, 0x4c, 0x44, 0x5f, 0x32, 0x00, 0x00, 0x00, 0x00, 0x4c, 0x44, 0x5f, 0x33, 0x00, 0x00, 0x00, 0x00,
    0x53, 0x54, 0x5f, 0x30, 0x00, 0x00, 0x00, 0x00, 0x53, 0x54, 0x5f, 0x31, 0x00, 0x00, 0x00, 0x00, 0x53, 0x54, 0x5f, 0x32,
    0x00, 0x00, 0x00, 0x00, 0x53, 0x54, 0x5f, 0x33, 0x00, 0x00, 0x00, 0x00, 0x4c, 0x44, 0x5f, 0x41, 0x52, 0x00, 0x00, 0x00,
    0x53, 0x54, 0x5f, 0x41, 0x52, 0x00, 0x00, 0x00, 0x4c, 0x44, 0x5f, 0x4e, 0x49, 0x4c, 0x00, 0x00, 0x4c, 0x44, 0x5f, 0x54,
    0x52, 0x55, 0x45, 0x00, 0x4c, 0x44, 0x5f, 0x46, 0x55, 0x4e, 0x43, 0x00, 0x4d, 0x49, 0x4e, 0x55, 0x53, 0x00, 0x00, 0x00,
    0x4e, 0x4f, 0x54, 0x00, 0x4c, 0x5f, 0x4e, 0x4f, 0x54, 0x00, 0x00, 0x00, 0x41, 0x44, 0x44, 0x00, 0x53, 0x55, 0x42, 0x00,
    0x4d, 0x55, 0x4c, 0x00, 0x44, 0x49, 0x56, 0x00, 0x4d, 0x4f, 0x44, 0x00, 0x4f, 0x52, 0x00, 0x00, 0x41, 0x4e, 0x44, 0x00,
    0x52, 0x5f, 0x53, 0x48, 0x49, 0x46, 0x54, 0x00, 0x4c, 0x5f, 0x53, 0x48, 0x49, 0x46, 0x54, 0x00, 0x45, 0x51, 0x00, 0x00,
    0x4e, 0x45, 0x00, 0x00, 0x47, 0x54, 0x00, 0x00, 0x4c, 0x54, 0x00, 0x00, 0x47, 0x45, 0x00, 0x00, 0x4c, 0x45, 0x00, 0x00,
    0x4c, 0x5f, 0x4f, 0x52, 0x00, 0x00, 0x00, 0x00, 0x4c, 0x5f, 0x41, 0x4e, 0x44, 0x00, 0x00, 0x00, 0x4a, 0x4d, 0x50, 0x00,
    0x4a, 0x50, 0x46, 0x00, 0x43, 0x41, 0x4c, 0x4c, 0x00, 0x00, 0x00, 0x00, 0x43, 0x41, 0x4c, 0x4c, 0x5f, 0x57, 0x00, 0x00,
    0x52, 0x45, 0x54, 0x00, 0x4e, 0x45, 0x58, 0x54, 0x00, 0x00, 0x00, 0x00, 0x50, 0x4c, 0x55, 0x47, 0x49, 0x4e, 0x00, 0x00,
    0x47, 0x45, 0x54, 0x5f, 0x4f, 0x43, 0x00, 0x00, 0x47, 0x45, 0x54, 0x54, 0x45, 0x52, 0x00, 0x00, 0x53, 0x45, 0x54, 0x54,
    0x45, 0x52, 0x00, 0x00, 0x53, 0x45, 0x4e, 0x44, 0x00, 0x00, 0x00, 0x00, 0x53, 0x45, 0x4e, 0x44, 0x5f, 0x57, 0x00, 0x00,
    0x54, 0x59, 0x50, 0x45, 0x4f, 0x46, 0x00, 0x00, 0x53, 0x49, 0x5a, 0x45, 0x4f, 0x46, 0x00, 0x00, 0x53, 0x57, 0x49, 0x54,
    0x43, 0x48, 0x00, 0x00, 0x49, 0x4e, 0x43, 0x00, 0x44, 0x45, 0x43, 0x00, 0x45, 0x58, 0x49, 0x54, 0x00, 0x00, 0x00, 0x00,
    0x42, 0x50, 0x00, 0x00, 0x6e, 0x69, 0x6c, 0x00, 0x74, 0x72, 0x75, 0x65, 0x00, 0x00, 0x00, 0x00, 0x66, 0x61, 0x6c, 0x73,
    0x65, 0x00, 0x00, 0x00, 0x69, 0x6e, 0x74, 0x00, 0x66, 0x69, 0x78, 0x65, 0x64, 0x00, 0x00, 0x00, 0x73, 0x74, 0x72, 0x69,
    0x6e, 0x67, 0x00, 0x00, 0x61, 0x72, 0x72, 0x61, 0x79, 0x00, 0x00, 0x00, 0x70, 0x6c, 0x75, 0x67, 0x69, 0x6e, 0x00, 0x00,
    0x4f, 0x43, 0x00, 0x00, 0x73, 0x79, 0x73, 0x00, 0x3f, 0x80, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x42, 0x22, 0xf9, 0x83,
    0x00, 0x00, 0x00, 0x00, 0x62, 0x61, 0x73, 0x65, 0x00, 0x00, 0x00, 0x00, 0x40, 0x00, 0x00, 0x00, 0x3f, 0x80, 0x00, 0x00,
    0x3f, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x43, 0x30, 0x00, 0x00, 0x80, 0x00, 0x00, 0x00, 0xbf, 0x80, 0x00, 0x00,
    0x3e, 0x4c, 0xcc, 0xcd, 0x40, 0x06, 0x66, 0x66, 0x37, 0x27, 0xc5, 0xac, 0x3c, 0xc9, 0x0f, 0xdb, 0xb7, 0x27, 0xc5, 0xac,
    0x47, 0xc3, 0x50, 0x00, 0x3a, 0x83, 0x12, 0x6f, 0x49, 0x74, 0x24, 0x00, 0xc9, 0x74, 0x24, 0x00, 0x41, 0xa0, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xc7, 0xc3, 0x50, 0x00,
    0x47, 0xc3, 0x50, 0x00, 0x42, 0x22, 0xf9, 0x83, 0xbf, 0x80, 0x00, 0x00, 0x3f, 0x00, 0x00, 0x00, 0x3f, 0x80, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x3f, 0x80, 0x00, 0x00,
    0x3f, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x3f, 0x80, 0x00, 0x00, 0x49, 0x74, 0x24, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x3f, 0x80, 0x00, 0x00, 0xbf, 0x80, 0x00, 0x00, 0xb8, 0xd1, 0xb7, 0x17,
    0xb7, 0x27, 0xc5, 0xac, 0x4c, 0xbe, 0xbc, 0x20, 0xc6, 0x1c, 0x40, 0x00, 0x3c, 0xc9, 0x0f, 0xdb, 0x3f, 0x00, 0x00, 0x00,
    0x49, 0x74, 0x24, 0x00, 0xc9, 0x74, 0x24, 0x00, 0x00, 0x00, 0x00, 0x00, 0x35, 0x86, 0x37, 0xbd, 0xc7, 0xc3, 0x50, 0x00,
    0x47, 0xc3, 0x50, 0x00, 0x00, 0x00, 0x00, 0x00, 0x3e, 0xaa, 0xaa, 0xab, 0x00, 0x00, 0x00, 0x00, 0x43, 0x7f, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x3f, 0x35, 0x04, 0x81, 0x00, 0x00, 0x00, 0x00, 0x3f, 0x80, 0x00, 0x00, 0x3f, 0x00, 0x00, 0x00,
    0x3d, 0x4c, 0xcc, 0xcd, 0x40, 0x00, 0x00, 0x00, 0x38, 0x80, 0x00, 0x00, 0x3e, 0x99, 0x99, 0x9a, 0xbe, 0x99, 0x99, 0x9a,
    0x00, 0x00, 0x00, 0x00, 0xba, 0x83, 0x12, 0x6f, 0x3f, 0x80, 0x00, 0x00, 0x3a, 0x83, 0x12, 0x6f, 0x00, 0x00, 0x00, 0x00,
    0x43, 0x30, 0x00, 0x00, 0x80, 0x00, 0x00, 0x00, 0xbd, 0x4c, 0xcc, 0xcd, 0x3d, 0x23, 0xd7, 0x0b, 0x3f, 0x7a, 0xe1, 0x48,
    0x3f, 0x00, 0x00, 0x00, 0x3d, 0xcc, 0xcc, 0xcd, 0xbe, 0xcc, 0xcc, 0xcd, 0x3f, 0x66, 0x66, 0x66, 0xbf, 0x4a, 0x3d, 0x71,
    0x00, 0x00, 0x00, 0x00, 0x3f, 0x00, 0x00, 0x00, 0x3f, 0x80, 0x00, 0x00, 0x43, 0x7f, 0x00, 0x00, 0x3b, 0x23, 0xd7, 0x0b,
    0x3e, 0x4c, 0xcc, 0xcd, 0x3d, 0x4c, 0xcc, 0xcd, 0x40, 0x00, 0x00, 0x00, 0x3d, 0xcc, 0xcc, 0xcd, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x3f, 0x00, 0x00, 0x00, 0x3f, 0x80, 0x00, 0x00, 0x46, 0x1c, 0x40, 0x00, 0x49, 0x74, 0x24, 0x00,
    0x42, 0x22, 0xf9, 0x83, 0x43, 0x7f, 0x00, 0x00, 0x43, 0x4c, 0x69, 0x67, 0x68, 0x74, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x3e, 0xcc, 0xcc, 0xcd, 0x3f, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x3f, 0x80, 0x00, 0x00, 0xff, 0xff, 0xff, 0xff,
    0x49, 0x74, 0x24, 0x00, 0x49, 0x74, 0x24, 0x10, 0x3d, 0x75, 0xc2, 0x8f, 0x3e, 0x19, 0x99, 0x9a, 0x00, 0x00, 0x00, 0x00,
    0x43, 0x30, 0x00, 0x00, 0x80, 0x00, 0x00, 0x00, 0xc1, 0xa0, 0x00, 0x00, 0x40, 0x00, 0x00, 0x00, 0x3f, 0xc0, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x3f, 0x80, 0x00, 0x00, 0x43, 0x30, 0x00, 0x00, 0x80, 0x00, 0x00, 0x00,
    0x41, 0x60, 0x00, 0x00, 0x3e, 0xcc, 0xcc, 0xcd, 0x3f, 0x66, 0x66, 0x66, 0x3e, 0xaa, 0xaa, 0xab, 0x43, 0x7f, 0x00, 0x00,
    0x3d, 0xea, 0x72, 0xeb, 0x3e, 0x99, 0x0a, 0xfe, 0x3f, 0x16, 0x2c, 0x23, 0x43, 0x30, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0xff, 0x40, 0x80, 0x00, 0x00, 0x42, 0xc8, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x3d, 0xcc, 0xcc, 0xcd,
    0x00, 0x00, 0x00, 0x00, 0x3f, 0x80, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x43, 0x30, 0x00, 0x00, 0x80, 0x00, 0x00, 0x00,
    0x40, 0x00, 0x00, 0x00, 0x40, 0x40, 0x00, 0x00, 0xc0, 0x00, 0x00, 0x00, 0xbf, 0x80, 0x00, 0x00, 0x42, 0x22, 0xf9, 0x83,
    0x00, 0x00, 0x00, 0x00, 0x3f, 0x80, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x3f, 0x4c, 0xcc, 0xcd, 0x40, 0xa0, 0x00, 0x00,
    0x43, 0xc5, 0x80, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x3f, 0x80, 0x00, 0x00, 0x42, 0xc8, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x43, 0x30, 0x00, 0x00, 0x80, 0x00, 0x00, 0x00, 0x43, 0x30, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x40, 0x00, 0x00, 0x00, 0x3f, 0x00, 0x00, 0x00, 0x43, 0x30, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x60, 0x60, 0x60, 0xff,
    0x00, 0x00, 0x00, 0x00, 0x3f, 0x80, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x4f, 0x00, 0x00, 0x00, 0x3f, 0x00, 0x00, 0x00,
    0x43, 0x30, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x43, 0x30, 0x00, 0x00, 0x80, 0x00, 0x00, 0x00, 0x3f, 0x99, 0x99, 0x9a,
    0x3c, 0xc9, 0x0f, 0xdb, 0x41, 0xf0, 0x00, 0x00, 0x3d, 0x4c, 0xcc, 0xcd, 0xb8, 0xd1, 0xb7, 0x17, 0x38, 0xd1, 0xb7, 0x17,
    0x3f, 0xc8, 0xf5, 0xc3, 0x3f, 0x66, 0x66, 0x66, 0x40, 0x00, 0x00, 0x00, 0x43, 0x7f, 0x00, 0x00, 0xbf, 0x80, 0x00, 0x00,
    0xff, 0xff, 0xff, 0xff, 0x44, 0x55, 0x80, 0x00, 0xbf, 0x00, 0x00, 0x00, 0x40, 0x40, 0x00, 0x00, 0xc0, 0x40, 0x00, 0x00,
    0x43, 0xb4, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x3f, 0x66, 0x66, 0x66, 0x40, 0x00, 0x00, 0x00,
    0x3f, 0x80, 0x00, 0x00, 0x43, 0x7f, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x3f, 0x80, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x40, 0x00, 0x00, 0x00, 0x40, 0x40, 0x00, 0x00, 0xc0, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x43, 0x30, 0x00, 0x00,
    0x80, 0x00, 0x00, 0x00, 0x3f, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x43, 0x30, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0xbf, 0x80, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x43, 0x45, 0x54, 0x72, 0x61, 0x69, 0x6c, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x3d, 0xcc, 0xcc, 0xcd, 0x3f, 0x7d, 0x70, 0xa4, 0x3f, 0x80, 0x00, 0x00, 0x40, 0x40, 0x00, 0x00, 0x47, 0xc3, 0x4f, 0x80,
    0x43, 0x30, 0x00, 0x00, 0x80, 0x00, 0x00, 0x00, 0x3f, 0x80, 0x00, 0x00, 0x41, 0xf0, 0x00, 0x00, 0x43, 0x7f, 0x00, 0x00,
    0x3f, 0x00, 0x00, 0x00, 0x43, 0x30, 0x00, 0x00, 0x80, 0x00, 0x00, 0x00, 0x43, 0x30, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x43, 0x30, 0x00, 0x00, 0x80, 0x00, 0x00, 0x00, 0x3f, 0xc9, 0x0f, 0xdb,
    0x00, 0x00, 0x00, 0x00, 0x4f, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x3f, 0x80, 0x00, 0x00, 0x42, 0x65, 0x2e, 0xe1,
    0x43, 0x30, 0x00, 0x00, 0x80, 0x00, 0x00, 0x00, 0x3f, 0x00, 0x00, 0x00, 0xbf, 0x80, 0x00, 0x00, 0x3c, 0x8e, 0xfa, 0x35,
    0x00, 0x00, 0x00, 0x00, 0x43, 0x4e, 0x61, 0x6e, 0x64, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x2f, 0x74, 0x6d, 0x70, 0x2f, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x4f, 0x00, 0x00, 0x00, 0x3f, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x43, 0x30, 0x00, 0x00, 0x80, 0x00, 0x00, 0x00, 0x3f, 0x80, 0x00, 0x00, 0xbf, 0x80, 0x00, 0x00,
    0x46, 0x1c, 0x58, 0x00, 0x00, 0x00, 0x00, 0x00, 0x43, 0x30, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x42, 0x65, 0x2e, 0xe1,
    0x00, 0x00, 0x00, 0x00, 0x43, 0x30, 0x00, 0x00, 0x80, 0x00, 0x00, 0x00, 0x3f, 0x80, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x3c, 0x8e, 0xfa, 0x35, 0x00, 0x00, 0x00, 0x00, 0x43, 0x30, 0x00, 0x00, 0x80, 0x00, 0x00, 0x00, 0x43, 0x30, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x43, 0x30, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x43, 0x30, 0x00, 0x00, 0x80, 0x00, 0x00, 0x00, 0x3f, 0x80, 0x00, 0x00, 0x4f, 0x00, 0x00, 0x00, 0x3f, 0x00, 0x00, 0x00,
    0x41, 0x20, 0x00, 0x00, 0x3a, 0x83, 0x12, 0x6f, 0x3d, 0xcc, 0xcc, 0xcd, 0x40, 0x00, 0x00, 0x00, 0x40, 0x40, 0x00, 0x00,
    0xc0, 0x00, 0x00, 0x00, 0xbf, 0x80, 0x00, 0x00, 0x43, 0x7f, 0x00, 0x00, 0x40, 0x80, 0x00, 0x00, 0x40, 0xc0, 0x00, 0x00,
    0x3f, 0x7f, 0xf9, 0x72, 0x3f, 0x80, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x43, 0x30, 0x00, 0x00, 0x80, 0x00, 0x00, 0x00,
    0x43, 0x30, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x48, 0x69, 0x64, 0x65, 0x00, 0x00, 0x00, 0x00, 0x50, 0x49, 0x44, 0x00,
    0x43, 0x7f, 0x00, 0x00, 0x4a, 0x55, 0x6a, 0x61, 0x77, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x3f, 0x80, 0x00, 0x00,
    0x3e, 0x4c, 0xcc, 0xcd, 0x3b, 0x03, 0x12, 0x6f, 0x3b, 0xc4, 0x9b, 0xa6, 0x00, 0x00, 0x00, 0x00, 0x43, 0x30, 0x00, 0x00,
    0x80, 0x00, 0x00, 0x00, 0x42, 0x22, 0xf9, 0x83, 0x42, 0x04, 0x00, 0x00, 0x40, 0x80, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x4a, 0x55, 0x76, 0x69, 0x65, 0x77, 0x00, 0x00, 0x6d, 0x74, 0x49, 0x72, 0x69, 0x73, 0x00, 0x00, 0x3f, 0x80, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x6d, 0x74, 0x45, 0x79, 0x65, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x3f, 0x80, 0x00, 0x00,
    0x41, 0x20, 0x00, 0x00, 0x3e, 0x4c, 0xcc, 0xcd, 0x00, 0x00, 0x00, 0x00, 0x41, 0xc8, 0x00, 0x00, 0x42, 0xa0, 0x00, 0x00,
    0x3d, 0xcc, 0xcc, 0xcd, 0x3f, 0x80, 0x00, 0x00, 0x42, 0x22, 0xf9, 0x83, 0x3f, 0x73, 0x33, 0x33, 0xbf, 0x7f, 0xf9, 0x72,
    0x40, 0x00, 0x00, 0x00, 0x3f, 0x00, 0x00, 0x00, 0x41, 0x20, 0x00, 0x00, 0x3c, 0x23, 0xd7, 0x0a, 0x3f, 0x66, 0x66, 0x66,
    0x3f, 0x4c, 0xcc, 0xcd, 0x3f, 0x7f, 0xff, 0x58, 0xbf, 0x7f, 0xbe, 0x77, 0xbf, 0x80, 0x00, 0x00, 0x3c, 0xc9, 0x0f, 0xdb,
    0x44, 0x43, 0x62, 0x61, 0x6c, 0x00, 0x00, 0x00, 0x44, 0x43, 0x70, 0x6c, 0x61, 0x00, 0x00, 0x00, 0x44, 0x43, 0x63, 0x79,
    0x6c, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x3f, 0x80, 0x00, 0x00, 0x3e, 0xb8, 0x51, 0xec, 0x3f, 0x19, 0x99, 0x9a,
    0x3e, 0xcc, 0xcc, 0xcd, 0x3f, 0x00, 0x00, 0x00, 0xbf, 0x80, 0x00, 0x00, 0x3c, 0xc9, 0x0f, 0xdb, 0x42, 0x22, 0xf9, 0x83,
    0x3f, 0x33, 0x33, 0x33, 0x3f, 0x66, 0x66, 0x66, 0x3d, 0xcc, 0xcc, 0xcd, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
    0x00, 0x00, 0x00, 0x00, 0x3f, 0x80, 0x00, 0x00, 0xbf, 0x80, 0x00, 0x00, 0x3f, 0x00, 0x00, 0x00, 0x3c, 0x8e, 0xfa, 0x35,
    0x42, 0x65, 0x2e, 0xe1, 0x43, 0x7f, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x43, 0x30, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x43, 0x30, 0x00, 0x00, 0x80, 0x00, 0x00, 0x00, 0x42, 0x34, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x3c, 0x23, 0xd7, 0x0a, 0x3f, 0x80, 0x00, 0x00, 0x3f, 0x81, 0x47, 0xae, 0x43, 0x7f, 0x00, 0x00, 0x3f, 0x00, 0x00, 0x00,
    0x40, 0x48, 0xf5, 0xc3, 0x43, 0x34, 0x00, 0x00, 0x3f, 0x72, 0x6e, 0x98, 0x40, 0x00, 0x00, 0x00, 0x43, 0x30, 0x00, 0x00,
    0x80, 0x00, 0x00, 0x00, 0x43, 0x30, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xff, 0xff, 0xff, 0xff,
    0xff, 0xff, 0xff, 0xff, 0x43, 0x7f, 0x00, 0x00, 0x3f, 0x80, 0x00, 0x00, 0x40, 0xc8, 0xf5, 0xc3, 0x40, 0x48, 0xf5, 0xc3,
    0x40, 0xc7, 0xae, 0x14, 0x43, 0x30, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x43, 0x30, 0x00, 0x00, 0x80, 0x00, 0x00, 0x00,
    0x3f, 0x00, 0x00, 0x00, 0x40, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x43, 0x30, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0x3f, 0x00, 0x00, 0x00, 0x43, 0x7f, 0x00, 0x00,
    0x3f, 0x80, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x43, 0x30, 0x00, 0x00, 0x80, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x3f, 0x80, 0x00, 0x00, 0x42, 0x22, 0xf9, 0x83, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x3f, 0x80, 0x00, 0x00,
    0x43, 0x7f, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x43, 0x30, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0xcb, 0x80, 0x00, 0x00, 0x3f, 0x80, 0x00, 0x00, 0x40, 0x00, 0x00, 0x00, 0x43, 0x30, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x43, 0x30, 0x00, 0x00, 0x80, 0x00, 0x00, 0x00, 0x2f, 0x74, 0x6d, 0x70, 0x2f, 0x00};

// Forward decls so the .data vtable's function-pointer slots name the exact
// retail symbols (the in-TU definitions are members / extern free functions
// with the same C-level names; these just declare them for the initializer).
class CGXCache;
extern "C" void func_80449D68__8CGXCacheFv(CGXCache*, u32, void*);
extern "C" void* __dt__CMsgParam_32(void*, int);
extern "C" void __dt__8CGXCacheFv(void*);
extern "C" void __dt__11IStateCacheFv(void*);

static void FORCEACTIVE_CGXCache_top(void) {
    (void)rodata_CGXCache; (void)sdata_CGXCache; (void)sdata2_pool_CGXCache;
}
// IStateCache: 4-byte polymorphic base (vtable only). Retail __dt__11IStateCacheFv
// is the empty delete-dtor shell; derived dtors inline the (empty) base call away.
// No C++ virtuals: the vptr is a plain member written explicitly by the ctor
// (retail __ct__8CGXCacheFv does `bl __ct__IStateCache; stw lbl_eu_8056BFC8`).
class __declspec(novtable) IStateCache {
public:
    ~IStateCache();
};

// CMsgParam message-queue slot (0x24 bytes; command at +0).
struct CMsgParamEntry {
    u32 command; //0x0 (message id / EVT value)
    u32 wid;     //0x4
    u32 unk8;
    u32 unkC;
    u32 unk10;
    u32 unk14;
    u32 unk18;
    u32 unk1C;
    u16 unk20;
    u8 unk22;
    u8 unk23;
};

// CMsgParam_32: 32-entry ring. Vtable slot at 0x0 (retail lbl_eu_8056BFE4, set
// by __ct__CMsgParam_32); entries inline at 0x4, so the retail dtor's
// `addic. r0,r3,4; beq` guard is the address test of mEntries.
// No member dtor: the retail symbol __dt__CMsgParam_32 has no length-prefix
// mangling, so it is written below as a free function; ~CGXCache destroys the
// member manually to reproduce retail's rebased `addic. r3,r3,4` guard.
class __declspec(novtable) CMsgParam_32 {
public:
    void* vtbl;                  //0x0 (vtable pointer)
    CMsgParamEntry mEntries[32]; //0x4
    CMsgParamEntry* mArrayPtr;   //0x484
    u32 mFront;                  //0x488
    u32 mSize;                   //0x48C
    u32 mCapacity;               //0x490
    u32 field6;                  //0x494
    u32 field7;                  //0x498
};

class __declspec(novtable) CGXCache : public IStateCache {
public:
    CGXCache();
    ~CGXCache();
    void func_80449D68();
    void func_8044A578();
    void func_8044A6C8(int param1, int param2);
    void func_8044A7F8();
    void func_8044A94C(int param1, int param2);
    void func_8044AA7C(int param1, int param2);
    void func_8044ABAC();
    void func_8044ACDC(const ml::CCol4& color, int flag);
    void func_8044AE8C(const ml::CCol4& color, int flag);
    void func_8044B03C(int param);
    void func_8044B168(int param);
    void func_8044B298(void* a, void* b, void* c);
    bool func_8044B4B8(GXTexObj* texObj, u16 w, u16 h);
    void func_8044B5B4();
    void func_8044B5C0();
    void func_8044B660();
    void func_8044B8CC(f32 fovy, f32 znear, f32 zfar);
    void func_8044BB20();
    s32 func_8044BD74(s32 param);
    void func_8044BE10();
    void* func_8044BE2C();
    void func_8044BE3C();
    void func_8044BFC0();
    void func_8044C034();
    void func_8044C1FC();

    //0x0: vtable (IStateCache)
    void* vtbl;               //0x0 (vtable pointer)
    CMsgParam_32 unk4;        //0x4
    s16 rect4A0[4];           //0x4A0..0x4A7 (scissor rect: x, y, w, h)
    s16 rect4A8[4];           //0x4A8
    s16 rect4B0[4];           //0x4B0
    u8 unk4B8[0x4];           //0x4B8..0x4BB
    s16 mScissorDeltaX;       //0x4BC
    s16 mScissorDeltaY;       //0x4BE
    Mtx44 mProjMtx;           //0x4C0..0x4FF
    s16 unk500;               //0x500
    s16 unk502;               //0x502..0x503
    GXTexObj* unk504;         //0x504 (last texture bound by func_8044B4B8)
    u32 unk508;               //0x508..0x50B
    u32 unk50C;               //0x50C
    s16 mRectLeft;            //0x510 (sub-rect left edge)
    s16 mRectTop;             //0x512 (sub-rect top edge)
    s16 mRectRight;           //0x514 (sub-rect right edge)
    s16 mRectBottom;          //0x516 (sub-rect bottom edge)
    u8 mAdjustProj;           //0x518 (refit projection to sub-rect when set)
    u8 unk519[3];             //0x519..0x51B
};

// Ring lookup used by func_8044B5B4 / func_8044BE10 / func_8044BD74. The
// retail symbol is func_8044CF74__8CGXCacheFv (zero-param mangling) even
// though the body reads the message id from r4, so it is written as an
// extern "C" free function with the exact retail name (same pattern as
// func_8044CE68 / func_8044CEF8 below).
extern "C" u32* func_8044CF74__8CGXCacheFv(CMsgParam_32* ring, u32 id);

// CDeviceGX::cacheInstance (retail .sbss sda symbol). The shared CDeviceGX
// header cannot be included here (local CGXCache class), so the mangled
// static is referenced by its literal name.
extern CGXCache* cacheInstance__9CDeviceGX;

// Ring command dispatcher (defined below); forward-declared here because the
// ring cache-update functions above use it before its definition.
// noinline lives on the DEFINITION below (MWCC 10176 rejects
// extern "C" + __declspec(noinline) on a pure prototype).
extern "C" void func_8044CE68__8CGXCacheFv(void* self, u32 cmd);

// Retail .sdata2 magic doubles
// the definitions live in the monolib shared data blob). Referenced by name
// through the s32ToF/u16ToF helpers below so the lfd relocs name the retail
// slots instead of MWCC's anonymous @N pool entries (docs/MWCC_CASES.md
// section 7i manual bit construction).
extern const double lbl_eu_8066A388;  // s32->f64 magic (2^52+2^31)
extern const double lbl_eu_8066A390;  // u32->f64 magic (2^52)
extern const f32 lbl_eu_8066A378;
extern const f32 lbl_eu_8066A380;

// Retail .sdata u32s used by the ring cache-update functions (imports; the
// definitions live in the monolib shared data blob, not this TU).
extern u32 lbl_eu_8066364C;
extern u32 lbl_eu_80663650;


// __ct__80449548 / func_8044954C are no-op helpers defined below (they take
// an address in r3 at the call site); the harness stubs that map to these
// symbols reference them rather than redefining.

// (mapped to __ct__80449548 above)

// (mapped to __ct__80449548 above)

// (mapped to __ct__80449548 above)

// Retail __dt__11IStateCacheFv: empty delete-dtor shell (delete wrapper only).
IStateCache::~IStateCache() {}

// Retail __dt__CMsgParam_32: clears the ring counters. The mEntries guard
// emits the `addic. r0,r3,4; beq` address test of the inline array at 0x4.
extern "C" void* __dt__CMsgParam_32(void* self, int deleting) {
    if (self != 0) {
        CMsgParam_32* p = static_cast<CMsgParam_32*>(self);
        if (p->mEntries) {
            p->mSize = 0;
            p->mFront = 0;
        }
        if (deleting > 0) {
            ::operator delete(self);
        }
    }
    return self;
}

// Retail __dt__804494D8: delete-dtor of the address-named class whose layout
// is CMsgParam_32 under an empty base at 0x0. The duplicated `self` check
// reproduces the inlined base-dtor call-site guard (bare beq reusing the
// entry CR0), which is what separates this from __dt__CMsgParam_32.
void* __dt__804494D8(void* self, int deleting) {
    if (self != 0) {
        if (self != 0) {
            CMsgParam_32* p = static_cast<CMsgParam_32*>(self);
            if (p->mEntries) {
                p->mSize = 0;
                p->mFront = 0;
            }
        }
        if (deleting > 0) {
            ::operator delete(self);
        }
    }
    return self;
}

// (mapped to __ct__80449548 above)

// CMsgParam<32>-ish ctor: vtable + ring-buffer init (retail data label).
// char[] form prevents SDA21 (label is >8B data; retail uses lis/addi).
// Declarations only - the definitions sit after __ct__8CGXCacheFv so MWCC
// -inline auto cannot inline them into the ctor (retail emits bl to each).
extern "C" u32 lbl_eu_8056BFE4[];
extern "C" void __ct__CMsgParam_32(void* self, u32 param);
extern "C" void __ct__80449534(void* self, s16 a, s16 b, s16 c, s16 d);
extern "C" void func_8044954C(void* self);
extern "C" void __ct__80449548(void* self);
extern "C" u32 lbl_eu_8056BFF0[];
extern "C" void __ct__IStateCache(void* self);

// Retail __dt__8CGXCacheFv: destroys the CMsgParam<32> ring member manually.
// The pointer-var + null-test reproduces retail's rebased `addic. r3,r3,4`
// guard (r3 = &unk4), and the duplicated null test is the inlined base-dtor
// entry check (bare beq reusing the addic CR).
CGXCache::~CGXCache() {
    CMsgParam_32* cache = &unk4;
    if (cache != NULL) {
        if (cache != NULL) {
            if (cache->mEntries) {
                cache->mSize = 0;
                cache->mFront = 0;
            }
        }
    }
}

// Retail GX-state dispatcher: selects the GX command group by the u32 in r4
// (blend, copy-clear, TEV, viewport or scissor) with the argument block in
// r5. Retail symbol is Fv-mangled although r4/r5 are live register params
// (same pattern as func_8044CE68 / func_8044CF74).
extern "C" u32 VIGetNextField(void);
extern const f32 lbl_eu_8066A37C;
extern const f32 lbl_eu_8066A380;
extern "C" GXRenderModeObj* getRenderModeObj__9CDeviceVIFv(void);
extern "C" void* func_8044CEF8__8CGXCacheFv(void* self, u32 cmd);

static inline s32 minS32(s32 a, s32 b) { return a < b ? a : b; }
static inline s32 maxS32(s32 a, s32 b) { return a > b ? a : b; }
static inline GXColor scaleColor255(f32 scale, const ml::CCol4& c);

// Copy-clear with the color scaled to 0-255 through the inlined helper. The
// extra inline boundary reproduces retail's early arg-temp address setup.
static inline void setCopyClearScaled(f32 scale, const ml::CCol4& c, u32 z) {
    GXSetCopyClear(scaleColor255(scale, c), z);
}

extern "C" void func_80449D68__8CGXCacheFv(CGXCache* self, u32 sel, void* data) {
    GXRenderModeObj* rmo;
    s16* vp = (s16*)data;
    u32* vu = (u32*)data;
    f32 scale;
    s16 out[4];
    s32 x0, y0, w0, h0;

    // Real switch: MWCC emits retail's cmplwi/jumptable/mtctr-bctr dispatch
    // (the jumptable lands in .data exactly like retail).
    switch (sel) {
    case 0:
        GXSetBlendMode((GXBlendMode)vu[0], GX_BL_SRCALPHA, GX_BL_INVSRCALPHA, GX_LO_CLEAR);
        break;
    case 1:
        GXSetBlendMode((GXBlendMode)vu[0], (GXBlendFactor)vu[1], (GXBlendFactor)vu[2],
                       (GXLogicOp)vu[3]);
        break;
    case 2: {
        // Copy-clear: color block in the data (floats scaled to 0-255), z
        // depth from ring command 3. Ring lookup runs first so the 255
        // scale is loaded after it into a volatile FPR (retail shape).
        void* pz = func_8044CEF8__8CGXCacheFv(&self->unk4, 3);
        u32 z = *(u32*)pz;
        scale = lbl_eu_8066A37C;
        setCopyClearScaled(scale, *(const ml::CCol4*)data, z);
        break;
    }
    case 3: {
        // Copy-clear: color from ring command 2, z depth from the data word.
        const ml::CCol4* pcol = (const ml::CCol4*)func_8044CEF8__8CGXCacheFv(&self->unk4, 2);
        u32 z = vu[0];
        scale = lbl_eu_8066A37C;
        setCopyClearScaled(scale, *pcol, z);
        break;
    }
    case 4:
        GXSetZCompLoc((GXBool)(vu[0] & 0xff));
        break;
    case 13:
        GXSetCullMode(vu[0] ? GX_CULL_NONE : GX_CULL_BACK);
        break;
    case 5: {
        // Z-mode: test enable from the data byte, compare func from ring cmd 6.
        u8 test = *(u8*)data;
        GXSetZMode(1, (GXCompare)(*(u8*)func_8044CEF8__8CGXCacheFv(&self->unk4, 6) ? GX_LEQUAL : GX_ALWAYS),
                       (GXBool)test);
        break;
    }
    case 6:
        // Z-mode: compare func from the data byte, test enable from ring cmd 5.
        GXSetZMode(1, (GXCompare)(*(u8*)data ? GX_LEQUAL : GX_ALWAYS),
                   (GXBool)*(u8*)func_8044CEF8__8CGXCacheFv(&self->unk4, 5));
        break;
    case 7:
        GXSetColorUpdate((GXBool)*(u8*)data);
        break;
    case 8:
        GXSetAlphaUpdate((GXBool)*(u8*)data);
        break;
    case 9: {
        // TEV pipeline presets selected by the data word. Written as an
        // if/else-if chain so MWCC emits retail's bne-skip dispatch.
        u32 tsel = vu[0];
        if (tsel == 0) {
            GXSetNumChans(1);
            GXSetNumTexGens(0);
            GXSetNumTevStages(1);
            GXSetTevOrder(GX_TEVSTAGE0, GX_TEXCOORD_NULL, GX_TEXMAP_NULL, GX_COLOR_NULL);
            GXSetTevColorIn(GX_TEVSTAGE0, GX_CC_ZERO, GX_CC_ZERO, GX_CC_ZERO, GX_CC_C0);
            GXSetTevColorOp(GX_TEVSTAGE0, (GXTevOp)0, (GXTevBias)0, (GXTevScale)0, 0, GX_TEVPREV);
            GXSetTevAlphaIn(GX_TEVSTAGE0, GX_CA_ZERO, GX_CA_ZERO, GX_CA_ZERO, GX_CA_A0);
            GXSetTevAlphaOp(GX_TEVSTAGE0, (GXTevOp)0, (GXTevBias)0, (GXTevScale)0, 0, GX_TEVPREV);
        } else if (tsel == 1) {
            GXSetNumChans(1);
            GXSetNumTexGens(1);
            GXSetNumTevStages(1);
            GXSetTevOrder(GX_TEVSTAGE0, GX_TEXCOORD0, GX_TEXMAP0, GX_COLOR_NULL);
            GXSetTevColorIn(GX_TEVSTAGE0, GX_CC_ZERO, GX_CC_C0, GX_CC_TEXC, GX_CC_ZERO);
            GXSetTevColorOp(GX_TEVSTAGE0, (GXTevOp)0, (GXTevBias)0, (GXTevScale)0, 0, GX_TEVPREV);
            GXSetTevAlphaIn(GX_TEVSTAGE0, GX_CA_ZERO, GX_CA_A0, GX_CA_TEXA, GX_CA_ZERO);
            GXSetTevAlphaOp(GX_TEVSTAGE0, (GXTevOp)0, (GXTevBias)0, (GXTevScale)0, 0, GX_TEVPREV);
        } else if (tsel == 2) {
            GXSetNumChans(1);
            GXSetNumTexGens(0);
            GXSetNumTevStages(1);
            GXSetTevOrder(GX_TEVSTAGE0, GX_TEXCOORD_NULL, GX_TEXMAP_NULL, GX_COLOR0A0);
            GXSetTevColorIn(GX_TEVSTAGE0, GX_CC_ZERO, GX_CC_ZERO, GX_CC_ZERO, GX_CC_RASC);
            GXSetTevColorOp(GX_TEVSTAGE0, (GXTevOp)0, (GXTevBias)0, (GXTevScale)0, 0, GX_TEVPREV);
            GXSetTevAlphaIn(GX_TEVSTAGE0, GX_CA_ZERO, GX_CA_ZERO, GX_CA_ZERO, GX_CA_RASA);
            GXSetTevAlphaOp(GX_TEVSTAGE0, (GXTevOp)0, (GXTevBias)0, (GXTevScale)0, 0, GX_TEVPREV);
            GXSetChanCtrl(GX_COLOR0, GX_DISABLE, GX_SRC_VTX, GX_SRC_VTX, (GXLightID)0,
                          GX_DF_CLAMP, GX_AF_NONE);
            GXSetChanCtrl(GX_ALPHA0, GX_DISABLE, GX_SRC_VTX, GX_SRC_VTX, (GXLightID)0,
                          GX_DF_NONE, GX_AF_NONE);
        } else if (tsel == 3) {
            GXSetNumChans(1);
            GXSetNumTexGens(1);
            GXSetNumTevStages(1);
            GXSetTevOrder(GX_TEVSTAGE0, GX_TEXCOORD0, GX_TEXMAP0, GX_COLOR0A0);
            GXSetTevColorIn(GX_TEVSTAGE0, GX_CC_ZERO, GX_CC_TEXC, GX_CC_RASC, GX_CC_ZERO);
            GXSetTevColorOp(GX_TEVSTAGE0, (GXTevOp)0, (GXTevBias)0, (GXTevScale)0, 0, GX_TEVPREV);
            GXSetTevAlphaIn(GX_TEVSTAGE0, GX_CA_ZERO, GX_CA_TEXA, GX_CA_RASA, GX_CA_ZERO);
            GXSetTevAlphaOp(GX_TEVSTAGE0, (GXTevOp)0, (GXTevBias)0, (GXTevScale)0, 0, GX_TEVPREV);
            GXSetChanCtrl(GX_COLOR0, GX_DISABLE, GX_SRC_VTX, GX_SRC_VTX, (GXLightID)0,
                          GX_DF_CLAMP, GX_AF_NONE);
            GXSetChanCtrl(GX_ALPHA0, GX_DISABLE, GX_SRC_VTX, GX_SRC_VTX, (GXLightID)0,
                          GX_DF_NONE, GX_AF_NONE);
        }
        break;
    }
    case 0xa:
        GXSetTevColor(GX_TEVREG0, *(GXColor*)data);
        break;
    case 0xb:
        // Viewport from the 4 s16s; jitter when interlaced. The conversions
        // live inside the branches so MWCC does not spill the f32s to FPRs.
        rmo = getRenderModeObj__9CDeviceVIFv();
        if (rmo->field_rendering) {
            u32 field = VIGetNextField();
            GXSetViewportJitter((f32)vp[0], (f32)vp[1], (f32)vp[2], (f32)vp[3],
                                lbl_eu_8066A378, lbl_eu_8066A380, field);
        } else {
            GXSetViewport((f32)vp[0], (f32)vp[1], (f32)vp[2], (f32)vp[3],
                          lbl_eu_8066A378, lbl_eu_8066A380);
        }
        break;
    case 0xc: {
        // Scissor rect clamped against the full screen; zero-rect when the
        // result has no area. The overflow test reproduces retail's
        // xor/srawi/and/subf sign idiom for `(fbWidth+w) < width`. The rect
        // is stored in order [2],[0],[1],[3] (retail byte order).
        rmo = getRenderModeObj__9CDeviceVIFv();
        s32 h = (s16)rmo->efbHeight;
        rmo = getRenderModeObj__9CDeviceVIFv();
        s32 w = (s16)rmo->fbWidth;
        self->rect4A0[2] = (s16)w;
        self->rect4A0[0] = 0;
        self->rect4A0[1] = 0;
        self->rect4A0[3] = (s16)h;
        x0 = vp[0];
        w0 = vp[2];
        s32 sumX = x0 + w0;
        s32 wmx = (s16)sumX;
        if (w > wmx) wmx = w;
        u16 ux0 = (u16)x0;
        s32 wmn = x0;
        if (self->rect4A0[1] < (s16)ux0) wmn = 0;
        s32 r1 = (s16)wmx - (s16)wmn;
        s32 a1 = w + w0;
        s32 cond1 = (a1 < r1);
        if (cond1) {
            // Vertical term for the overflow test (mirror of the horizontal
            // one): csum is read before ysum so the rect-half loads precede
            // the vp loads, matching retail's load order.
            y0 = vp[1];
            s32 csum = (s16)(self->rect4A0[1] + self->rect4A0[3]);
            h0 = vp[3];
            s32 ysum = (s16)(y0 + h0);
            if (csum > ysum) ysum = csum;
            y0 = vp[1];
            if (self->rect4A0[1] < y0) y0 = self->rect4A0[1];
            s32 r2 = (s16)ysum - (s16)y0;
            s32 a2 = self->rect4A0[3] + vp[3];
            cond1 = (a2 < r2);
        }
        if (cond1 == 0) {
            out[0] = 0;
            out[1] = 0;
            out[2] = 0;
            out[3] = 0;
        } else {
            s32 ox = x0;
            if (x0 < self->rect4A0[0]) ox = 0;
            s32 oy = vp[1];
            if (self->rect4A0[1] < oy) oy = self->rect4A0[1];
            s32 wmx2 = (s16)sumX;
            if (w > wmx2) wmx2 = w;
            s32 oh = (s16)(oy + vp[3]);
            s32 lim = (s16)(self->rect4A0[1] + self->rect4A0[3]);
            if (lim < oh) oh = lim;
            s32 ow = wmx2 - ox;
            oh = oh - oy;
            out[0] = (s16)ox;
            out[1] = (s16)oy;
            out[2] = (s16)ow;
            out[3] = (s16)oh;
        }
        *(u32*)&self->rect4A0[0] = *(u32*)&out[0];
        *(u32*)&self->rect4A0[2] = *(u32*)&out[2];
        s32 noArea = 0;
        if (self->rect4A0[2] <= 0 || self->rect4A0[3] <= 0) {
            noArea = 1;
        }
        if (noArea == 0) {
            GXSetScissor(self->rect4A0[0], self->rect4A0[1], self->rect4A0[2], self->rect4A0[3]);
        }
        break;
    }
    }
}

// Retail .sdata2/.sdata imports used by the cache-update and projection
// functions below (definitions live in the monolib shared data blob).
extern const f32 lbl_eu_8066A37C;   // 255.0f color scale
// Aspect multiplier (scissor-rect ratio x this constant) and the znear scale
// used when refitting the projection matrix to the 0x510 sub-rect.
extern const f32 lbl_eu_8066A398;
extern const f32 lbl_eu_8066A39C;
extern u32 lbl_eu_80663644;
extern u32 lbl_eu_80663648;

// CDeviceVI static helpers used by the projection builders. The proper
// CDeviceVI.hpp header cannot be included here (it pulls CMsgParam.hpp,
// which collides with the local CMsgParam<N> template below), so the retail
// mangled names are imported directly like the other extern "C" imports in
// this file.
extern "C" f32 getWidthScale__9CDeviceVIFv(void);
extern "C" GXRenderModeObj* getRenderModeObj__9CDeviceVIFv(void);

// Ring search helper (inlined at every call site by -inline auto). The
// inlined `return i` / `return -1` reproduce retail's `bne skip; b found`
// two-branch search loop; a plain goto-found loop compiles to a merged
// `beq found` that is 4 bytes shorter and cannot match the retail bytes
// (docs/MWCC_CASES.md BD-address search loops).
template <u32 CMD>
static u32 ringFindIndex(CGXCache* self) {
    for (u32 i = 0; i < self->unk4.mSize; i++) {
        u32 idx = self->unk4.mFront + i;
        u32 slot = idx % self->unk4.mCapacity;
        if (self->unk4.mArrayPtr[slot].command == CMD) return i;
    }
    return 0xFFFFFFFF;
}

// Signed-command variant (func_8044A6C8): retail tests the command word with
// a signed `cmpwi` against 0 instead of the unsigned `cmplwi`.
template <s32 CMD>
static u32 ringFindIndexS(CGXCache* self) {
    for (u32 i = 0; i < self->unk4.mSize; i++) {
        u32 idx = self->unk4.mFront + i;
        u32 slot = idx % self->unk4.mCapacity;
        if ((s32)self->unk4.mArrayPtr[slot].command == CMD) return i;
    }
    return 0xFFFFFFFF;
}

// Ring command-5 cache update with a one-byte wid (bloom alpha value). Same
// shape as func_8044A94C (command 6): find the entry whose command is 5;
// unless (param2 == 0 && the entry's wid byte already equals param1) the
// entry is re-stamped (unk23 = 4, wid byte = val) and command 5 dispatched.
void CGXCache::func_8044AA7C(int param1, int param2) {
    u8 val = (u8)param1;
    u32 i = ringFindIndex<0x5>(this);
    if (param2 != 0) goto update;
    {
        u32 idx = unk4.mFront + i;
        u32 slot = idx % unk4.mCapacity;
        bool same = (memcmp(&unk4.mArrayPtr[slot].wid, &val, 1) == 0);
        if (same) goto end;
    }
update:
    {
        u32 idx = unk4.mFront + i;
        u32 slot = idx % unk4.mCapacity;
        unk4.mArrayPtr[slot].unk23 = 0x4;
        u32 idx2 = unk4.mFront + i;
        u32 slot2 = idx2 % unk4.mCapacity;
        *(u8*)&unk4.mArrayPtr[slot2].wid = val;
        func_8044CE68__8CGXCacheFv(&unk4, 0x5);
    }
end:
    ;
}

// Ring command-8 cache update with a one-byte wid. Retail symbol is
// func_8044ABAC__8CGXCacheFv (zero-param mangling) even though the body reads
// (int param1, int param2) from r4/r5 (CScnBloom passes them like the other
// ring setters), so it is written as an extern "C" free function with the
// exact retail name (same pattern as func_8044CE68 / func_8044CF74 below).
extern "C" void func_8044ABAC__8CGXCacheFv(CGXCache* self, int param1, int param2) {
    u8 val = (u8)param1;
    u32 i = ringFindIndex<0x8>(self);
    if (param2 != 0) goto update;
    {
        u32 idx = self->unk4.mFront + i;
        u32 slot = idx % self->unk4.mCapacity;
        bool same = (memcmp(&self->unk4.mArrayPtr[slot].wid, &val, 1) == 0);
        if (same) goto end;
    }
update:
    {
        u32 idx = self->unk4.mFront + i;
        u32 slot = idx % self->unk4.mCapacity;
        self->unk4.mArrayPtr[slot].unk23 = 0x4;
        u32 idx2 = self->unk4.mFront + i;
        u32 slot2 = idx2 % self->unk4.mCapacity;
        *(u8*)&self->unk4.mArrayPtr[slot2].wid = val;
        func_8044CE68__8CGXCacheFv(&self->unk4, 0x8);
    }
end:
    ;
}

// Ring command-2 cache update carrying a 16-byte CCol4 payload by pointer.
// Unless (flag == 0 && the entry's wid..unk10 already equal the color), the
// entry is re-stamped (unk23 = 0xb, wid..unk10 = color) and command 2 is
// dispatched. Retail name func_8044A578__8CGXCacheFv is zero-param mangled
// although r4 (color) / r5 (flag) are live register params.
extern "C" void func_8044A578__8CGXCacheFv(CGXCache* self, ml::CCol4* color, int flag) {
    u32 i = ringFindIndex<0x2>(self);
    if (flag != 0) goto update;
    {
        u32 idx = self->unk4.mFront + i;
        u32 slot = idx % self->unk4.mCapacity;
        bool same = (memcmp(&self->unk4.mArrayPtr[slot].wid, color, 0x10) == 0);
        if (same) goto end;
    }
update:
    {
        u32 c0;
        u32 c1;
        u32 idx = self->unk4.mFront + i;
        u32 slot = idx % self->unk4.mCapacity;
        self->unk4.mArrayPtr[slot].unk23 = 0xb;
        u32 idx2 = self->unk4.mFront + i;
        u32 slot2 = idx2 % self->unk4.mCapacity;
        CMsgParamEntry* e = &self->unk4.mArrayPtr[slot2];
        c1 = *(u32*)&color->g;
        c0 = *(u32*)&color->r;
        e->wid = c0;
        e->unk8 = c1;
        u32 c3 = *(u32*)&color->a;
        u32 c2 = *(u32*)&color->b;
        e->unkC = c2;
        e->unk10 = c3;
        func_8044CE68__8CGXCacheFv(&self->unk4, 0x2);
    }
end:
    ;
}

// Ring command-1 cache update carrying a 16-byte payload by value in r4-r7.
// The words are spilled to a stack array so memcmp can compare them; unless
// (flag == 0 && the entry's wid..unk10 already match) the entry is re-stamped
// (unk23 = 9, wid..unk10 = payload) and command 1 is dispatched. Retail name
// func_8044A7F8__8CGXCacheFv is zero-param mangled although r4-r8 are live.
extern "C" void func_8044A7F8__8CGXCacheFv(CGXCache* self, u32 w0, u32 w1, u32 w2, u32 w3, int flag) {
    u32 payload[4] = {w0, w1, w2, w3};
    u32 i = ringFindIndex<0x1>(self);
    if (flag != 0) goto update;
    {
        u32 idx = self->unk4.mFront + i;
        u32 slot = idx % self->unk4.mCapacity;
        bool same = (memcmp(&self->unk4.mArrayPtr[slot].wid, payload, 0x10) == 0);
        if (same) goto end;
    }
update:
    {
        u32 idx = self->unk4.mFront + i;
        u32 slot = idx % self->unk4.mCapacity;
        self->unk4.mArrayPtr[slot].unk23 = 0x9;
        u32 idx2 = self->unk4.mFront + i;
        u32 slot2 = idx2 % self->unk4.mCapacity;
        CMsgParamEntry* e = &self->unk4.mArrayPtr[slot2];
        u32* dst = &e->wid;
        for (int j = 0; j < 4; j++) {
            dst[j] = payload[j];
        }
        func_8044CE68__8CGXCacheFv(&self->unk4, 0x1);
    }
end:
    ;
}

// Ring command-9 cache update carrying a 4-byte wid (the shared color
// multiplier). Unless (flag == 0 && the entry's wid already equals
// lbl_eu_80663644) the entry is re-stamped (unk23 = 3, wid =
// lbl_eu_80663644) and command 9 is dispatched; the color is then scaled to
// 0-255 and pushed as TEVREG1.
// Ring command-9 cache update carrying a 4-byte wid (the shared color
// multiplier). Unless (flag == 0 && the entry's wid already equals
// lbl_eu_80663644) the entry is re-stamped (unk23 = 3, wid =
// lbl_eu_80663644) and command 9 is dispatched; the color is then scaled to
// 0-255 and pushed as TEVREG0.
//
// The color is built through an inlined helper (not a named local) so MWCC
// allocates the value-assembly slot below the outgoing arg area, matching
// retail's stb/pack at sp+8 / arg copy at sp+0xc.
static inline GXColor scaleColor255(f32 scale, const ml::CCol4& c) {
    GXColor col;
    col.r = (u8)(s32)(scale * c.r);
    col.g = (u8)(s32)(scale * c.g);
    col.b = (u8)(s32)(scale * c.b);
    col.a = (u8)(s32)(scale * c.a);
    return col;
}

void CGXCache::func_8044ACDC(const ml::CCol4& color, int flag) {
    f32 scale;
    u32 i = ringFindIndex<0x9>(this);
    if (flag != 0) goto update;
    {
        u32 idx = unk4.mFront + i;
        u32 slot = idx % unk4.mCapacity;
        bool same = (memcmp(&unk4.mArrayPtr[slot].wid, &lbl_eu_80663644, 4) == 0);
        if (same) goto end;
    }
update:
    {
        u32 idx = unk4.mFront + i;
        u32 slot = idx % unk4.mCapacity;
        unk4.mArrayPtr[slot].unk23 = 0x3;
        u32 idx2 = unk4.mFront + i;
        u32 slot2 = idx2 % unk4.mCapacity;
        unk4.mArrayPtr[slot2].wid = lbl_eu_80663644;
        func_8044CE68__8CGXCacheFv(&unk4, 0x9);
    }
end:
    {
        // Scale the color components to 0-255 and push as TEVREG0.
        scale = lbl_eu_8066A37C;
        GXSetTevColor(GX_TEVREG0, scaleColor255(scale, color));
    }
}

// Same cache update as func_8044ACDC but for the lbl_eu_80663648 slot.
void CGXCache::func_8044AE8C(const ml::CCol4& color, int flag) {
    f32 scale;
    u32 i = ringFindIndex<0x9>(this);
    if (flag != 0) goto update;
    {
        u32 idx = unk4.mFront + i;
        u32 slot = idx % unk4.mCapacity;
        bool same = (memcmp(&unk4.mArrayPtr[slot].wid, &lbl_eu_80663648, 4) == 0);
        if (same) goto end;
    }
update:
    {
        u32 idx = unk4.mFront + i;
        u32 slot = idx % unk4.mCapacity;
        unk4.mArrayPtr[slot].unk23 = 0x3;
        u32 idx2 = unk4.mFront + i;
        u32 slot2 = idx2 % unk4.mCapacity;
        unk4.mArrayPtr[slot2].wid = lbl_eu_80663648;
        func_8044CE68__8CGXCacheFv(&unk4, 0x9);
    }
end:
    {
        scale = lbl_eu_8066A37C;
        GXSetTevColor(GX_TEVREG0, scaleColor255(scale, color));
    }
}

void func_8044B294__8CGXCacheFUl(void) {}

extern "C" void func_8044CE68__8CGXCacheFv(void* self, u32 cmd);
struct C1FCMsgEntry {
    u32 command;
    u32 wid;
    u32 unk8;
    u32 unkC;
    u32 unk10;
    u32 unk14;
    u32 unk18;
    u32 unk1C;
    u16 unk20;
    u8 unk22;
    u8 unk23;
};

struct C1FCCacheLayout {
    u8 pad0[0x488];
    C1FCMsgEntry* mArrayPtr;
    u32 mFront;
    u32 mSize;
    u32 mCapacity;
    u32 field6;
    u32 field7;
    u8 pad1[8]; // 0x4A0..0x4A7 so rect4A8 lands at 0x4A8
    s16 rect4A8[4];
    s16 rect4B0[4];
    u8 pad2[0x50C - 0x4B8 - 4];
    s16 mScissorDeltaX; // 0x4BC: scissor origin delta
    s16 mScissorDeltaY; // 0x4BE: scissor origin delta
    u32 unk50C;
};

// Optional ring pair pointers in r4-r6 update cached scissor rects, then enqueue
// GX commands for tags 0xB / 0xC via func_8044CE68 (this+4).
void CGXCache::func_8044B298(void* a, void* b, void* c) {
    C1FCCacheLayout* cache = (C1FCCacheLayout*)this;
    u32* insetPair;
    s16 stack[4];
    u32 i;
    u32 idx;
    u32 slot;
    C1FCMsgEntry* entry;

    // Retail null-tests a, c and b in that order around the rect copies;
    // b (r31) falls back to &rect4A8 when null.
    if (a != NULL) {
        u32* a32 = (u32*)a;
        u32 w0 = a32[0];
        u32 w1 = a32[1];
        *(u32*)&cache->rect4A8[2] = w1;
        *(u32*)&cache->rect4A8[0] = w0;
    }
    insetPair = (u32*)b;
    if (c != NULL) {
        u32* c32 = (u32*)c;
        u32 w0 = c32[0];
        u32 w1 = c32[1];
        *(u32*)&cache->rect4B0[2] = w1;
        *(u32*)&cache->rect4B0[0] = w0;
    }
    if (insetPair == NULL) {
        insetPair = reinterpret_cast<u32*>(cache->rect4A8);
    }

    stack[0] = (s16)(cache->rect4A8[0] - cache->rect4B0[0]);
    stack[1] = (s16)(cache->rect4A8[1] - cache->rect4B0[1]);
    stack[2] = cache->rect4B0[2];
    stack[3] = cache->rect4B0[3];
    cache->mScissorDeltaX = stack[2];
    cache->mScissorDeltaY = stack[3];

    u32 count = cache->mSize;
    for (i = 0; i < count; i++) {
        idx = cache->mFront + i;
        slot = idx - (idx / cache->mCapacity) * cache->mCapacity;
        if (cache->mArrayPtr[slot].command == 0xb) {
            goto found_b;
        }
    }
    i = (u32)-1;
found_b:
    idx = cache->mFront + i;
    slot = idx - (idx / cache->mCapacity) * cache->mCapacity;
    cache->mArrayPtr[slot].unk23 = 0xd;
    idx = cache->mFront + i;
    slot = idx - (idx / cache->mCapacity) * cache->mCapacity;
    entry = &cache->mArrayPtr[slot];
    entry->wid = *(u32*)&stack[0];
    entry->unk8 = *(u32*)&stack[2];
    // Retail recomputes this+4 at each call site (addi before bl).
    func_8044CE68__8CGXCacheFv(&unk4, 0xb);

    count = cache->mSize;
    for (i = 0; i < count; i++) {
        idx = cache->mFront + i;
        slot = idx - (idx / cache->mCapacity) * cache->mCapacity;
        if (cache->mArrayPtr[slot].command == 0xc) {
            goto found_c;
        }
    }
    i = (u32)-1;
found_c:
    idx = cache->mFront + i;
    slot = idx - (idx / cache->mCapacity) * cache->mCapacity;
    cache->mArrayPtr[slot].unk23 = 0xd;
    idx = cache->mFront + i;
    slot = idx - (idx / cache->mCapacity) * cache->mCapacity;
    entry = &cache->mArrayPtr[slot];
    entry->wid = insetPair[0];
    entry->unk8 = insetPair[1];
    {
        // volatile blocks CSE of (this+4) into a third saved GPR (+4B over).
        void* volatile vself = this;
        func_8044CE68__8CGXCacheFv(&static_cast<CGXCache*>(vself)->unk4, 0xc);
    }
}

// Ring command-9 cache update (texture slot 0x6364C). Searches the ring for
// the entry whose command is 9; unless (param == 0 && the entry's wid already
// equals lbl_eu_8066364C) it re-stamps the entry (unk23 = 3, wid =
// lbl_eu_8066364C) and dispatches command 9 to the device.
void CGXCache::func_8044B03C(int param) {
    u32 i = ringFindIndex<0x9>(this);
    if (param == 0) {
        u32 slot = (unk4.mFront + i) % unk4.mCapacity;
        bool same = memcmp(&unk4.mArrayPtr[slot].wid, &lbl_eu_8066364C, 4) == 0;
        if (same)
            return;
    }
    u32 slot = (unk4.mFront + i) % unk4.mCapacity;
    unk4.mArrayPtr[slot].unk23 = 0x3;
    u32 slot2 = (unk4.mFront + i) % unk4.mCapacity;
    unk4.mArrayPtr[slot2].wid = lbl_eu_8066364C;
    func_8044CE68__8CGXCacheFv(&unk4, 0x9);
}

// Same cache-update as func_8044B03C but for texture slot 0x63650.
void CGXCache::func_8044B168(int param) {
    u32 i = ringFindIndex<0x9>(this);
    if (param == 0) {
        u32 slot = (unk4.mFront + i) % unk4.mCapacity;
        bool same = memcmp(&unk4.mArrayPtr[slot].wid, &lbl_eu_80663650, 4) == 0;
        if (same)
            return;
    }
    u32 slot = (unk4.mFront + i) % unk4.mCapacity;
    unk4.mArrayPtr[slot].unk23 = 0x3;
    u32 slot2 = (unk4.mFront + i) % unk4.mCapacity;
    unk4.mArrayPtr[slot2].wid = lbl_eu_80663650;
    func_8044CE68__8CGXCacheFv(&unk4, 0x9);
}

// Ring command-0 cache update. Same shape as func_8044B03C: the entry's wid
// is compared (when param2 == 0) against param1 and re-stamped (unk23 = 3,
// wid = param1) before dispatching command 0.
void CGXCache::func_8044A6C8(int param1, int param2) {
    u32 i = ringFindIndexS<0>(this);
    if (param2 == 0) {
        u32 slot = (unk4.mFront + i) % unk4.mCapacity;
        bool same = memcmp(&unk4.mArrayPtr[slot].wid, &param1, 4) == 0;
        if (same)
            return;
    }
    u32 slot = (unk4.mFront + i) % unk4.mCapacity;
    unk4.mArrayPtr[slot].unk23 = 0x3;
    u32 slot2 = (unk4.mFront + i) % unk4.mCapacity;
    unk4.mArrayPtr[slot2].wid = param1;
    func_8044CE68__8CGXCacheFv(&unk4, 0x0);
}

// Ring command-6 cache update with a one-byte wid. The byte value of param1
// is stamped (unk23 = 4, wid byte = val) and command 6 dispatched.
void CGXCache::func_8044A94C(int param1, int param2) {
    u8 val = (u8)param1;
    u32 i = ringFindIndex<0x6>(this);
    if (param2 == 0) {
        u32 slot = (unk4.mFront + i) % unk4.mCapacity;
        bool same = memcmp(&unk4.mArrayPtr[slot].wid, &val, 1) == 0;
        if (same)
            return;
    }
    u32 slot = (unk4.mFront + i) % unk4.mCapacity;
    unk4.mArrayPtr[slot].unk23 = 0x4;
    u32 slot2 = (unk4.mFront + i) % unk4.mCapacity;
    *(u8*)&unk4.mArrayPtr[slot2].wid = val; // only the low byte is written
    func_8044CE68__8CGXCacheFv(&unk4, 0x6);
}

// Binds a texture to TEXMAP0 and installs the tex-gen matrix that maps a
// w x h texture into tex-coord space (c = lbl_eu_8066A380): m[0][0] = c/w,
// m[1][1] = c/h, m[2][2] = c, all other entries 1.0. Remembers the texture at
// 0x504 and returns true.
// s32 -> f32 through the shared 2^52+2^31 magic double (lbl_eu_8066A388).
// Manual bit construction so the lfd reloc names the retail .sdata2 slot
// instead of MWCC's anonymous @N pool entry (docs/MWCC_CASES.md section 7i).
static inline f32 s32ToF(s32 v) {
    union {
        double d;
        u32 w[2];
    } c;
    c.w[1] = (u32)v ^ 0x80000000;
    c.w[0] = 0x43300000u;
    return (f32)(c.d - lbl_eu_8066A388);
}

// u32 -> f32 through the shared 2^52 magic double (lbl_eu_8066A390). Unsigned
// variant: no top-bit flip, so the low word is stored unmodified.
static inline f32 u32ToF(u32 v) {
    union {
        double d;
        u32 w[2];
    } c;
    c.w[1] = v;
    c.w[0] = 0x43300000u;
    return (f32)(c.d - lbl_eu_8066A390);
}

// u16 -> f32 through the shared 2^52 magic double (lbl_eu_8066A390). Same
// reloc-naming rationale as s32ToF.
static inline f32 u16ToF(u16 v) {
    union {
        double d;
        u32 w[2];
    } c;
    c.w[1] = v;
    c.w[0] = 0x43300000u;
    return (f32)(c.d - lbl_eu_8066A390);
}

// Caller-owned scratch-slot variants: the union lives at the call site so
// consecutive conversions reuse one stack slot (retail reuses two slots,
// one per matrix column of conversions).
typedef union {
    double d;
    u32 w[2];
} F64Cvt;

static inline f32 cvtS32Slot(F64Cvt* c, s32 v) {
    c->w[1] = (u32)v ^ 0x80000000;
    return (f32)(c->d - lbl_eu_8066A388);
}

static inline f32 cvtU32Slot(F64Cvt* c, u32 v) {
    c->w[1] = v;
    return (f32)(c->d - lbl_eu_8066A390);
}


bool CGXCache::func_8044B4B8(GXTexObj* texObj, u16 w, u16 h) {
    GXLoadTexObj(texObj, GX_TEXMAP0);
    Mtx mtx = {
        { lbl_eu_8066A380 / (f32)w, lbl_eu_8066A378, lbl_eu_8066A378, lbl_eu_8066A378 },
        { lbl_eu_8066A378, lbl_eu_8066A380 / (f32)h, lbl_eu_8066A378, lbl_eu_8066A378 },
        { lbl_eu_8066A378, lbl_eu_8066A378, lbl_eu_8066A380, lbl_eu_8066A378 },
    };
    GXLoadTexMtxImm(mtx, GX_TEXMTX0, GX_MTX_2x4);
    GXSetNumTexGens(1);
    GXSetTexCoordGen2(GX_TEXCOORD0, GX_TG_MTX2x4, GX_TG_TEX0, GX_TEXMTX0, GX_DISABLE, 0x7D);
    unk504 = texObj;
    return true;
}

void CGXCache::func_8044B5B4() { func_8044CF74__8CGXCacheFv(&unk4, 2); }

// Rebuild the ortho projection from the current scissor deltas and z depth:
// t/l/n = lbl_eu_8066A378, b = deltaY, r = deltaX, f = -unk500 (retail FPRs).
void CGXCache::func_8044B5C0() {
    CGXCache* cache = cacheInstance__9CDeviceGX;
    f32 c = lbl_eu_8066A378;
    f32 far = (f32)(-unk500);
    f32 right = (f32)cache->mScissorDeltaX;
    f32 bottom = (f32)cache->mScissorDeltaY;
    C_MTXOrtho(mProjMtx, c, bottom, c, right, c, far);
    GXSetProjection(mProjMtx, GX_ORTHOGRAPHIC);
}

void CGXCache::func_8044B660() {
    if (mAdjustProj) {
        // Letterboxed projection: scale the sub-rect edges (0x510..0x516,
        // given as fractions of the framebuffer) by the scissor deltas, and
        // use -unk500 as the far plane (same near=1.0 / ortho shape as the
        // non-adjusted path). Each delta is loaded before the getRenderModeObj
        // call that follows so it survives the call in r30 (retail ping-pong).
        // Conversions use two caller-owned magic-double slots: the signed
        // edge goes through one slot, and the unsigned fb dimension AND the
        // following delta share the second slot (the delta store overwrites
        // the dimension and is re-read after the division, exactly like
        // retail's stw/lfd pair).
        GXRenderModeObj* rmo;
        F64Cvt su, ss;
        su.w[0] = 0x43300000u;
        ss.w[0] = 0x43300000u;
        s32 dx = cacheInstance__9CDeviceGX->mScissorDeltaX;
        f32 right, left, bottom, top;
        rmo = getRenderModeObj__9CDeviceVIFv();
        f32 den = cvtU32Slot(&su, rmo->fbWidth);
        f32 num = cvtS32Slot(&ss, mRectRight);
        f32 del = cvtS32Slot(&su, dx);
        right = num / den * del;
        dx = cacheInstance__9CDeviceGX->mScissorDeltaY;
        rmo = getRenderModeObj__9CDeviceVIFv();
        den = cvtU32Slot(&su, rmo->fbWidth);
        num = cvtS32Slot(&ss, mRectLeft);
        del = cvtS32Slot(&su, dx);
        left = num / den * del;
        dx = cacheInstance__9CDeviceGX->mScissorDeltaY;
        rmo = getRenderModeObj__9CDeviceVIFv();
        den = cvtU32Slot(&su, rmo->efbHeight);
        num = cvtS32Slot(&ss, mRectBottom);
        del = cvtS32Slot(&su, dx);
        bottom = num / den * del;
        dx = cacheInstance__9CDeviceGX->mScissorDeltaY;
        rmo = getRenderModeObj__9CDeviceVIFv();
        den = cvtU32Slot(&su, rmo->efbHeight);
        num = cvtS32Slot(&ss, mRectTop);
        del = cvtS32Slot(&su, dx);
        top = num / den * del;
        f32 far = cvtS32Slot(&ss, -unk500);
        C_MTXOrtho(mProjMtx, top, bottom, left, right, lbl_eu_8066A378, far);
    } else {
        // Plain ortho: top/left/near = 1.0, bottom = deltaY, right = deltaX,
        // far = -unk500 (same shape as func_8044B5C0).
        F64Cvt cs;
        cs.w[0] = 0x43300000u;
        f32 far = cvtS32Slot(&cs, -unk500);
        f32 right = cvtS32Slot(&cs, cacheInstance__9CDeviceGX->mScissorDeltaX);
        f32 bottom = cvtS32Slot(&cs, cacheInstance__9CDeviceGX->mScissorDeltaY);
        C_MTXOrtho(mProjMtx, lbl_eu_8066A378, bottom, lbl_eu_8066A378, right,
                   lbl_eu_8066A378, far);
    }
    GXSetProjection(mProjMtx, GX_ORTHOGRAPHIC);
}

// Builds the perspective projection into mProjMtx. The horizontal fov is
// widened for 16:9 by scaling the aspect (scissor-rect ratio x fixed
// multiplier) with CDeviceVI::getWidthScale(). When mAdjustProj is set the
// matrix is refit to the sub-rect at 0x510..0x516 (frustum translation and
// scale for a letterboxed viewport).
void CGXCache::func_8044B8CC(f32 fovy, f32 znear, f32 zfar) {
    f32 aspect = lbl_eu_8066A398 * ((f32)mScissorDeltaX / (f32)mScissorDeltaY);
    C_MTXPerspective(mProjMtx, fovy, aspect * getWidthScale__9CDeviceVIFv(), znear, zfar);
    if (mAdjustProj) {
        f32 r = (f32)((s32)mRectRight - (s32)((u32)getRenderModeObj__9CDeviceVIFv()->fbWidth >> 1));
        f32 l = (f32)((s32)mRectLeft - (s32)((u32)getRenderModeObj__9CDeviceVIFv()->fbWidth >> 1));
        f32 t = (f32)((s32)mRectTop - (s32)((u32)getRenderModeObj__9CDeviceVIFv()->efbHeight >> 1));
        f32 b = (f32)((s32)mRectBottom - (s32)((u32)getRenderModeObj__9CDeviceVIFv()->efbHeight >> 1));
        mProjMtx[0][2] = (r + l) / (r - l);
        // Overwrite znear in place: retail reuses the param's own callee-save
        // (fmuls f31,f0,f31) rather than giving the scaled copy a new range.
        znear = lbl_eu_8066A39C * znear;
        mProjMtx[1][2] = (t + b) / (t - b);
        mProjMtx[0][0] = znear / (znear / mProjMtx[0][0] * ((f32)((s32)mRectRight - (s32)mRectLeft) / (f32)(u32)getRenderModeObj__9CDeviceVIFv()->fbWidth));
        mProjMtx[1][1] = znear / (znear / mProjMtx[1][1] * ((f32)((s32)mRectBottom - (s32)mRectTop) / (f32)(u32)getRenderModeObj__9CDeviceVIFv()->efbHeight));
    }
    GXSetProjection(mProjMtx, GX_ORTHOGRAPHIC);
}

// Same projection build as func_8044B8CC but writes into an external matrix
// (used for camera items) instead of mProjMtx, and does not push the matrix
// to GX. Retail symbol is Fv-mangled although the body reads
// (CGXCache*, f32[4][4], f32 fovY, f32 nearZ, f32 farZ) from r3/r4/f1-f3
// (same pattern as func_8044ABAC / func_8044CE68).
extern "C" void func_8044BB20__8CGXCacheFv(CGXCache* self, f32 projOut[4][4], f32 fovy, f32 nearZ, f32 farZ) {
    // Plain float casts reproduce retail's exact schedule; the conversion
    // magics land in MWCC's anonymous .sdata2 pool (@N) - they need
    // retarget_relocs/exact_renames onto lbl_eu_8066A388/A390 in the unit's
    // postprocess rule (same pattern as the existing .text 0x8CC entries).
    // NOTE: retail passes float param 2 (nearZ) as C_MTXPerspective's near
    // plane and scales IT by lbl_eu_8066A39C; param 3 is the far plane.
    f32 aspect = lbl_eu_8066A398 * ((f32)self->mScissorDeltaX / (f32)self->mScissorDeltaY);
    C_MTXPerspective(projOut, fovy, aspect * getWidthScale__9CDeviceVIFv(), nearZ, farZ);
    if (self->mAdjustProj) {
        f32 r = (f32)((s32)self->mRectRight - (s32)((u32)getRenderModeObj__9CDeviceVIFv()->fbWidth >> 1));
        f32 l = (f32)((s32)self->mRectLeft - (s32)((u32)getRenderModeObj__9CDeviceVIFv()->fbWidth >> 1));
        f32 t = (f32)((s32)self->mRectTop - (s32)((u32)getRenderModeObj__9CDeviceVIFv()->efbHeight >> 1));
        f32 b = (f32)((s32)self->mRectBottom - (s32)((u32)getRenderModeObj__9CDeviceVIFv()->efbHeight >> 1));
        projOut[0][2] = (r + l) / (r - l);
        // Overwrite the near plane in place: retail reuses the param's own
        // callee-save (fmuls f31,f0,f31) rather than giving the scaled copy
        // a new range.
        nearZ = lbl_eu_8066A39C * nearZ;
        projOut[1][2] = (t + b) / (t - b);
        // Retail evaluates each ratio's getRenderModeObj() call before
        // touching the matrix diagonal, so hoist the ratios.
        f32 xr = (f32)((s32)self->mRectRight - (s32)self->mRectLeft) /
                 (f32)(u32)getRenderModeObj__9CDeviceVIFv()->fbWidth;
        projOut[0][0] = nearZ / (nearZ / projOut[0][0] * xr);
        f32 yr = (f32)((s32)self->mRectBottom - (s32)self->mRectTop) /
                 (f32)(u32)getRenderModeObj__9CDeviceVIFv()->efbHeight;
        projOut[1][1] = nearZ / (nearZ / projOut[1][1] * yr);
    }
}

// Ring entry 3's payload word is the scissor width; scale it by z depth:
// (s16)unk500 * param / wid (int->float math, truncated back to int).
s32 CGXCache::func_8044BD74(s32 param) {
    // Retail shape: ring lookup first; the s16 field at 0x500 converts via the
    // signed 2^52+2^31 magic, the width payload word is loaded only after the
    // conversion stores, and both param/width convert via the unsigned 2^52
    // magic before the single-precision mul/div.
    u32 wid = *func_8044CF74__8CGXCacheFv(&unk4, 3);
    // Plain float casts reproduce retail's exact 156B schedule; the s16 field
    // converts through the signed magic (lha/xoris/2^52+2^31) and param/wid
    // through the unsigned 2^52 magic. NOTE: MWCC pools these as anonymous
    // .sdata2 constants (@N) - they need retarget_relocs/exact_renames onto
    // lbl_eu_8066A388/A390 in the unit's postprocess rule (same pattern as
    // the existing .text 0x8CC / 0x1A48 entries).
    f32 f = (f32)unk500 * (f32)(u32)param / (f32)wid;
    return (s32)f;
}

void CGXCache::func_8044BE10() { func_8044CF74__8CGXCacheFv(&unk4, 11); }

// retail: addi r3,r3,0x510 (returns &unk510)
void* func_8044BE1C__8CGXCacheFv(void* self) { return (u8*)self + 0x510; }

// retail: lbz r3,0x518(r3) (unk510[8])
u8 func_8044BE24__8CGXCacheFv(void* self) { return *(u8*)((u8*)self + 0x518); }

void* CGXCache::func_8044BE2C(void) { return (void*)0xFFFFFF; }

// Retail is a pure tail to func_8044C1FC. Keep callee undefined here so MWCC
// cannot inline an empty same-TU stub into a lone blr.
void func_8044C1FC__8CGXCacheFv(void* self);
void func_8044BE38__8CGXCacheFv(void* self) {
    func_8044C1FC__8CGXCacheFv(self);
}

void CGXCache::func_8044BE3C() {
    (void)this;
    GXSetTexCoordGen2(GX_TEXCOORD0, GX_TG_MTX2x4, GX_TG_TEX0, 0x3C, GX_DISABLE, 0x7D);
    GXSetTexCoordGen2(GX_TEXCOORD1, GX_TG_MTX2x4, GX_TG_TEX1, 0x3C, GX_DISABLE, 0x7D);
    GXSetTexCoordGen2(GX_TEXCOORD2, GX_TG_MTX2x4, GX_TG_TEX2, 0x3C, GX_DISABLE, 0x7D);
    GXSetTexCoordGen2(GX_TEXCOORD3, GX_TG_MTX2x4, GX_TG_TEX3, 0x3C, GX_DISABLE, 0x7D);
    GXSetTexCoordGen2(GX_TEXCOORD4, GX_TG_MTX2x4, GX_TG_TEX4, 0x3C, GX_DISABLE, 0x7D);
    GXSetTexCoordGen2(GX_TEXCOORD5, GX_TG_MTX2x4, GX_TG_TEX5, 0x3C, GX_DISABLE, 0x7D);
    GXSetTexCoordGen2(GX_TEXCOORD6, GX_TG_MTX2x4, GX_TG_TEX6, 0x3C, GX_DISABLE, 0x7D);
    GXSetTexCoordGen2(GX_TEXCOORD7, GX_TG_MTX2x4, GX_TG_TEX7, 0x3C, GX_DISABLE, 0x7D);
    GXSetNumTexGens(1);
    GXEnableTexOffsets(GX_TEXCOORD0, GX_DISABLE, GX_DISABLE);
    GXEnableTexOffsets(GX_TEXCOORD1, GX_DISABLE, GX_DISABLE);
    GXEnableTexOffsets(GX_TEXCOORD2, GX_DISABLE, GX_DISABLE);
    GXEnableTexOffsets(GX_TEXCOORD3, GX_DISABLE, GX_DISABLE);
    GXEnableTexOffsets(GX_TEXCOORD4, GX_DISABLE, GX_DISABLE);
    GXEnableTexOffsets(GX_TEXCOORD5, GX_DISABLE, GX_DISABLE);
    GXEnableTexOffsets(GX_TEXCOORD6, GX_DISABLE, GX_DISABLE);
    GXEnableTexOffsets(GX_TEXCOORD7, GX_DISABLE, GX_DISABLE);
}

void CGXCache::func_8044BFC0() {
    GXInvalidateVtxCache();
    GXInvalidateTexAll();
    GXClearVtxDesc();
    GXLoadPosMtxImm(*ml::CMat34::identity, GX_PNMTX0);
    GXLoadNrmMtxImm(*ml::CMat34::identity, GX_PNMTX0);
    GXSetCurrentMtx(GX_PNMTX0);
    GXLoadTexMtxImm(*ml::CMat34::identity, 0x3C, GX_MTX_3x4);
    GXLoadTexMtxImm(*ml::CMat34::identity, 0x7D, GX_MTX_3x4);
}

void CGXCache::func_8044C034() {
    (void)this;
    int tevStage;
    GXSetTevOrder(GX_TEVSTAGE0, GX_TEXCOORD0, GX_TEXMAP0, GX_COLOR0A0);
    GXSetTevOrder(GX_TEVSTAGE1, GX_TEXCOORD1, GX_TEXMAP1, GX_COLOR0A0);
    GXSetTevOrder(GX_TEVSTAGE2, GX_TEXCOORD2, GX_TEXMAP2, GX_COLOR0A0);
    GXSetTevOrder(GX_TEVSTAGE3, GX_TEXCOORD3, GX_TEXMAP3, GX_COLOR0A0);
    GXSetTevOrder(GX_TEVSTAGE4, GX_TEXCOORD4, GX_TEXMAP4, GX_COLOR0A0);
    GXSetTevOrder(GX_TEVSTAGE5, GX_TEXCOORD5, GX_TEXMAP5, GX_COLOR0A0);
    GXSetTevOrder(GX_TEVSTAGE6, GX_TEXCOORD6, GX_TEXMAP6, GX_COLOR0A0);
    GXSetTevOrder(GX_TEVSTAGE7, GX_TEXCOORD7, GX_TEXMAP7, GX_COLOR0A0);
    GXSetTevOrder(GX_TEVSTAGE8, GX_TEXCOORD_NULL, GX_TEXMAP_NULL, GX_COLOR_NULL);
    GXSetTevOrder(GX_TEVSTAGE9, GX_TEXCOORD_NULL, GX_TEXMAP_NULL, GX_COLOR_NULL);
    GXSetTevOrder(GX_TEVSTAGE10, GX_TEXCOORD_NULL, GX_TEXMAP_NULL, GX_COLOR_NULL);
    GXSetTevOrder(GX_TEVSTAGE11, GX_TEXCOORD_NULL, GX_TEXMAP_NULL, GX_COLOR_NULL);
    GXSetTevOrder(GX_TEVSTAGE12, GX_TEXCOORD_NULL, GX_TEXMAP_NULL, GX_COLOR_NULL);
    GXSetTevOrder(GX_TEVSTAGE13, GX_TEXCOORD_NULL, GX_TEXMAP_NULL, GX_COLOR_NULL);
    GXSetTevOrder(GX_TEVSTAGE14, GX_TEXCOORD_NULL, GX_TEXMAP_NULL, GX_COLOR_NULL);
    GXSetTevOrder(GX_TEVSTAGE15, GX_TEXCOORD_NULL, GX_TEXMAP_NULL, GX_COLOR_NULL);
    GXSetNumTevStages(1);
    GXSetTevOp(GX_TEVSTAGE0, GX_REPLACE);
    tevStage = 0;
    while (tevStage < 0x10) {
        GXSetTevKColorSel((GXTevStageID)tevStage, (GXTevKColorSel)6);
        GXSetTevKAlphaSel((GXTevStageID)tevStage, GX_TEV_KASEL_1);
        GXSetTevSwapMode((GXTevStageID)tevStage, GX_TEV_SWAP0, GX_TEV_SWAP0);
        tevStage++;
    }
    tevStage = 0;
    while (tevStage < 0x10) {
        GXSetTevDirect((GXTevStageID)tevStage);
        tevStage++;
    }
}

extern "C" GXRenderModeObj* getRenderModeObj__9CDeviceVIFv(void);
extern const f32 lbl_eu_8066A37C;
extern const f32 lbl_eu_8066A3A0;
extern const f32 lbl_eu_8066A3A4;
extern u32 lbl_eu_80663644;
extern "C" void func_8044BE3C__8CGXCacheFv(void* self);
extern "C" void func_8044C034__8CGXCacheFv(void* self);
extern "C" void func_8044CE68__8CGXCacheFv(void* self, u32 cmd);
extern "C" void* func_8044CEF8__8CGXCacheFv(void* self, u32 cmd);

#pragma dont_inline on
void CGXCache::func_8044C1FC() {
    C1FCCacheLayout* cache = (C1FCCacheLayout*)this;
    void* msgSelf = &unk4;
    GXColor gxCol;
    GXRenderModeObj* rmo;
    f32 yScale;
    u16 halfVi;
    u32 fieldMode;
    ml::CCol4* pCol;
    u32 zTex;
    s16 d0;
    s16 d1;
    s16 stack4A8[4];
    s16* stackScissor;
    f32 scale;

    GXSetCoPlanar(GX_DISABLE);
    GXSetCullMode(GX_CULL_BACK);
    GXSetClipMode(GX_CLIP_ENABLE);
    func_8044BE3C__8CGXCacheFv(this);
    GXInvalidateVtxCache();
    GXInvalidateTexAll();
    GXClearVtxDesc();

    GXLoadPosMtxImm(*ml::CMat34::identity, GX_PNMTX0);
    GXLoadNrmMtxImm(*ml::CMat34::identity, GX_PNMTX0);
    GXSetCurrentMtx(GX_PNMTX0);
    GXLoadTexMtxImm(*ml::CMat34::identity, 0x3C, GX_MTX_3x4);
    GXLoadTexMtxImm(*ml::CMat34::identity, 0x7D, GX_MTX_3x4);

    scale = lbl_eu_8066A37C;
    gxCol.r = (u8)(s32)(scale * ml::CCol4::red.r);
    gxCol.g = (u8)(s32)(scale * ml::CCol4::red.g);
    gxCol.b = (u8)(s32)(scale * ml::CCol4::red.b);
    gxCol.a = (u8)(s32)(scale * ml::CCol4::red.a);
    GXSetFog(GX_FOG_NONE, gxCol, lbl_eu_8066A3A0, lbl_eu_8066A3A4, lbl_eu_8066A37C, lbl_eu_8066A3A4);

    GXSetNumChans(0);
    GXSetChanCtrl(GX_COLOR0A0, GX_DISABLE, GX_SRC_REG, GX_SRC_REG, GX_LIGHT0, GX_DF_NONE, GX_AF_NONE);
    GXSetChanCtrl(GX_COLOR1A1, GX_DISABLE, GX_SRC_REG, GX_SRC_REG, GX_LIGHT0, GX_DF_NONE, GX_AF_NONE);

    gxCol.r = (u8)(s32)(scale * ml::CCol4::black.r);
    gxCol.g = (u8)(s32)(scale * ml::CCol4::black.g);
    gxCol.b = (u8)(s32)(scale * ml::CCol4::black.b);
    gxCol.a = (u8)(s32)(scale * ml::CCol4::black.a);
    GXSetChanAmbColor(GX_COLOR0A0, gxCol);
    gxCol.r = (u8)(s32)(scale * ml::CCol4::white.r);
    gxCol.g = (u8)(s32)(scale * ml::CCol4::white.g);
    gxCol.b = (u8)(s32)(scale * ml::CCol4::white.b);
    gxCol.a = (u8)(s32)(scale * ml::CCol4::white.a);
    GXSetChanMatColor(GX_COLOR0A0, gxCol);
    gxCol.r = (u8)(s32)(scale * ml::CCol4::black.r);
    gxCol.g = (u8)(s32)(scale * ml::CCol4::black.g);
    gxCol.b = (u8)(s32)(scale * ml::CCol4::black.b);
    gxCol.a = (u8)(s32)(scale * ml::CCol4::black.a);
    GXSetChanAmbColor(GX_COLOR1A1, gxCol);
    gxCol.r = (u8)(s32)(scale * ml::CCol4::white.r);
    gxCol.g = (u8)(s32)(scale * ml::CCol4::white.g);
    gxCol.b = (u8)(s32)(scale * ml::CCol4::white.b);
    gxCol.a = (u8)(s32)(scale * ml::CCol4::white.a);
    GXSetChanMatColor(GX_COLOR1A1, gxCol);

    GXSetAlphaCompare(GX_ALWAYS, 0, GX_AOP_AND, GX_ALWAYS, 0);
    GXSetZTexture(GX_ZT_DISABLE, GX_TF_Z8, 0);

    func_8044C034__8CGXCacheFv(this);

    GXSetNumIndStages(0);
    GXSetIndTexCoordScale(GX_INDTEXSTAGE0, GX_ITS_1, GX_ITS_1);
    GXSetIndTexCoordScale(GX_INDTEXSTAGE1, GX_ITS_1, GX_ITS_1);
    GXSetIndTexCoordScale(GX_INDTEXSTAGE2, GX_ITS_1, GX_ITS_1);
    GXSetIndTexCoordScale(GX_INDTEXSTAGE3, GX_ITS_1, GX_ITS_1);

    rmo = getRenderModeObj__9CDeviceVIFv();
    yScale = GXGetYScaleFactor(rmo->efbHeight, rmo->xfbHeight);
    GXSetDispCopyYScale(yScale);
    halfVi = (u16)(rmo->xfbHeight << 1);
    fieldMode = rmo->viHeight - halfVi;
    fieldMode = fieldMode ? 1 : 0;
    rmo = getRenderModeObj__9CDeviceVIFv();
    GXSetFieldMode(rmo->field_rendering, fieldMode);
    rmo = getRenderModeObj__9CDeviceVIFv();
    GXSetDispCopySrc(0, 0, rmo->fbWidth, rmo->efbHeight);
    rmo = getRenderModeObj__9CDeviceVIFv();
    GXSetDispCopyDst(rmo->fbWidth, (u16)yScale);
    GXSetDispCopyGamma(cache->unk50C);

    pCol = (ml::CCol4*)func_8044CEF8__8CGXCacheFv(msgSelf, 3);
    zTex = *(u32*)func_8044CEF8__8CGXCacheFv(msgSelf, 2);
    gxCol.r = (u8)(s32)(scale * pCol->r);
    gxCol.g = (u8)(s32)(scale * pCol->g);
    gxCol.b = (u8)(s32)(scale * pCol->b);
    gxCol.a = (u8)(s32)(scale * pCol->a);
    GXSetCopyClear(gxCol, zTex);

    rmo = getRenderModeObj__9CDeviceVIFv();
    stack4A8[0] = 0;
    stack4A8[1] = 0;
    stack4A8[2] = (s16)rmo->fbWidth;
    stack4A8[3] = (s16)rmo->efbHeight;
    *(u32*)&cache->rect4A8[0] = *(u32*)&stack4A8[0];
    *(u32*)&cache->rect4A8[2] = *(u32*)&stack4A8[2];
    *(u32*)&cache->rect4B0[0] = *(u32*)&stack4A8[0];
    *(u32*)&cache->rect4B0[2] = *(u32*)&stack4A8[2];
    stackScissor = stack4A8;
    d0 = cache->rect4A8[0] - cache->rect4B0[0];
    d1 = cache->rect4A8[1] - cache->rect4B0[1];
    cache->rect4B0[2] = stack4A8[2];
    cache->rect4B0[3] = stack4A8[3];
    {
        s32 found_B = -1;
        u32 i_B;
        for (i_B = 0; i_B < cache->mSize; i_B++) {
            u32 idx_B = cache->mFront + i_B;
            u32 slot_B = idx_B - (idx_B / cache->mCapacity) * cache->mCapacity;
            if (cache->mArrayPtr[slot_B].command == 0x0D) {
                found_B = (s32)i_B;
                break;
            }
        }
        if (found_B != -1) {
            
            u32 idx2_B = cache->mFront + (u32)found_B;
            u32 slot2_B = idx2_B - (idx2_B / cache->mCapacity) * cache->mCapacity;
            C1FCMsgEntry* e_B = &cache->mArrayPtr[slot2_B];
            e_B->unk23 = 0x0D;
            e_B->unk8 = (u32)d0;
            e_B->unk10 = (u32)d1;
        }
        func_8044CE68__8CGXCacheFv(msgSelf, 0xB);
    }
    {
        s32 found_C = -1;
        u32 i_C;
        for (i_C = 0; i_C < cache->mSize; i_C++) {
            u32 idx_C = cache->mFront + i_C;
            u32 slot_C = idx_C - (idx_C / cache->mCapacity) * cache->mCapacity;
            if (cache->mArrayPtr[slot_C].command == 0x0D) {
                found_C = (s32)i_C;
                break;
            }
        }
        if (found_C != -1) {
            
            u32 idx2_C = cache->mFront + (u32)found_C;
            u32 slot2_C = idx2_C - (idx2_C / cache->mCapacity) * cache->mCapacity;
            C1FCMsgEntry* e_C = &cache->mArrayPtr[slot2_C];
            e_C->unk23 = 0x0D;
            e_C->unk8 = *(u32*)&stackScissor[0];
            e_C->unk10 = *(u32*)&stackScissor[2];
        }
        func_8044CE68__8CGXCacheFv(msgSelf, 0xC);
    }
    {
        s32 found_4 = -1;
        u32 i_4;
        for (i_4 = 0; i_4 < cache->mSize; i_4++) {
            u32 idx_4 = cache->mFront + i_4;
            u32 slot_4 = idx_4 - (idx_4 / cache->mCapacity) * cache->mCapacity;
            if (cache->mArrayPtr[slot_4].command == 0x03) {
                found_4 = (s32)i_4;
                break;
            }
        }
        if (found_4 != -1) {
            
            u32 idx2_4 = cache->mFront + (u32)found_4;
            u32 slot2_4 = idx2_4 - (idx2_4 / cache->mCapacity) * cache->mCapacity;
            C1FCMsgEntry* e_4 = &cache->mArrayPtr[slot2_4];
            e_4->unk23 = 0x03;
            e_4->unk8 = 1;
        }
        func_8044CE68__8CGXCacheFv(msgSelf, 0x4);
    }
    {
        s32 found_5 = -1;
        u32 i_5;
        for (i_5 = 0; i_5 < cache->mSize; i_5++) {
            u32 idx_5 = cache->mFront + i_5;
            u32 slot_5 = idx_5 - (idx_5 / cache->mCapacity) * cache->mCapacity;
            if (cache->mArrayPtr[slot_5].command == 0x04) {
                found_5 = (s32)i_5;
                break;
            }
        }
        if (found_5 != -1) {
            
            u32 idx2_5 = cache->mFront + (u32)found_5;
            u32 slot2_5 = idx2_5 - (idx2_5 / cache->mCapacity) * cache->mCapacity;
            C1FCMsgEntry* e_5 = &cache->mArrayPtr[slot2_5];
            e_5->unk23 = 0x04;
            e_5->unk8 = 1;
        }
        func_8044CE68__8CGXCacheFv(msgSelf, 0x5);
    }
    {
        s32 found_6 = -1;
        u32 i_6;
        for (i_6 = 0; i_6 < cache->mSize; i_6++) {
            u32 idx_6 = cache->mFront + i_6;
            u32 slot_6 = idx_6 - (idx_6 / cache->mCapacity) * cache->mCapacity;
            if (cache->mArrayPtr[slot_6].command == 0x04) {
                found_6 = (s32)i_6;
                break;
            }
        }
        if (found_6 != -1) {
            
            u32 idx2_6 = cache->mFront + (u32)found_6;
            u32 slot2_6 = idx2_6 - (idx2_6 / cache->mCapacity) * cache->mCapacity;
            C1FCMsgEntry* e_6 = &cache->mArrayPtr[slot2_6];
            e_6->unk23 = 0x04;
            e_6->unk8 = 1;
        }
        func_8044CE68__8CGXCacheFv(msgSelf, 0x6);
    }
    {
        s32 found_7 = -1;
        u32 i_7;
        for (i_7 = 0; i_7 < cache->mSize; i_7++) {
            u32 idx_7 = cache->mFront + i_7;
            u32 slot_7 = idx_7 - (idx_7 / cache->mCapacity) * cache->mCapacity;
            if (cache->mArrayPtr[slot_7].command == 0x04) {
                found_7 = (s32)i_7;
                break;
            }
        }
        if (found_7 != -1) {
            
            u32 idx2_7 = cache->mFront + (u32)found_7;
            u32 slot2_7 = idx2_7 - (idx2_7 / cache->mCapacity) * cache->mCapacity;
            C1FCMsgEntry* e_7 = &cache->mArrayPtr[slot2_7];
            e_7->unk23 = 0x04;
            e_7->unk8 = 1;
        }
        func_8044CE68__8CGXCacheFv(msgSelf, 0x7);
    }
    {
        s32 found_8 = -1;
        u32 i_8;
        for (i_8 = 0; i_8 < cache->mSize; i_8++) {
            u32 idx_8 = cache->mFront + i_8;
            u32 slot_8 = idx_8 - (idx_8 / cache->mCapacity) * cache->mCapacity;
            if (cache->mArrayPtr[slot_8].command == 0x04) {
                found_8 = (s32)i_8;
                break;
            }
        }
        if (found_8 != -1) {
            
            u32 idx2_8 = cache->mFront + (u32)found_8;
            u32 slot2_8 = idx2_8 - (idx2_8 / cache->mCapacity) * cache->mCapacity;
            C1FCMsgEntry* e_8 = &cache->mArrayPtr[slot2_8];
            e_8->unk23 = 0x04;
            e_8->unk8 = 1;
        }
        func_8044CE68__8CGXCacheFv(msgSelf, 0x8);
    }
    {
        s32 found_0 = -1;
        u32 i_0;
        for (i_0 = 0; i_0 < cache->mSize; i_0++) {
            u32 idx_0 = cache->mFront + i_0;
            u32 slot_0 = idx_0 - (idx_0 / cache->mCapacity) * cache->mCapacity;
            if (cache->mArrayPtr[slot_0].command == 0x03) {
                found_0 = (s32)i_0;
                break;
            }
        }
        if (found_0 != -1) {
            
            u32 idx2_0 = cache->mFront + (u32)found_0;
            u32 slot2_0 = idx2_0 - (idx2_0 / cache->mCapacity) * cache->mCapacity;
            C1FCMsgEntry* e_0 = &cache->mArrayPtr[slot2_0];
            e_0->unk23 = 0x03;
            e_0->unk8 = 1;
        }
        func_8044CE68__8CGXCacheFv(msgSelf, 0x0);
    }
    {
        s32 found_9 = -1;
        u32 i_9;
        for (i_9 = 0; i_9 < cache->mSize; i_9++) {
            u32 idx_9 = cache->mFront + i_9;
            u32 slot_9 = idx_9 - (idx_9 / cache->mCapacity) * cache->mCapacity;
            if (cache->mArrayPtr[slot_9].command == 0x03) {
                found_9 = (s32)i_9;
                break;
            }
        }
        if (found_9 != -1) {
            
            u32 idx2_9 = cache->mFront + (u32)found_9;
            u32 slot2_9 = idx2_9 - (idx2_9 / cache->mCapacity) * cache->mCapacity;
            C1FCMsgEntry* e_9 = &cache->mArrayPtr[slot2_9];
            e_9->unk23 = 0x03;
            e_9->unk8 = lbl_eu_80663644;
        }
        func_8044CE68__8CGXCacheFv(msgSelf, 0x9);
    }

    gxCol.r = (u8)(s32)(scale * ml::CCol4::white.r);
    gxCol.g = (u8)(s32)(scale * ml::CCol4::white.g);
    gxCol.b = (u8)(s32)(scale * ml::CCol4::white.b);
    gxCol.a = (u8)(s32)(scale * ml::CCol4::white.a);
    GXSetTevColor(GX_TEVREG1, gxCol);
}
#pragma dont_inline reset

struct MsgParam32Entry {
    u32 command;
    u32 wid;
    u32 unk8;
    u32 unkC;
    u32 unk10;
    u32 unk14;
    u32 unk18;
    u32 unk1C;
    u16 unk20;
    u8 unk22;
    u8 unk23;
};

struct MsgParam32Ring {
    u8 pad[0x484];
    MsgParam32Entry* mArrayPtr;
    u32 mFront;
    u32 mSize;
    u32 mCapacity;
    u32 field6;
    void* field7;
};

// Ring search: returns the loop index of the first entry with command == cmd,
// or 0xFFFFFFFF. The return-in-loop shape is what makes MWCC emit retail's
// `bne skip; b found` two-branch exit (docs/MWCC_CASES.md BD-address search
// loops); the caller then recomputes the slot from the returned index.
static u32 msgRingFind(MsgParam32Ring* ring, u32 cmd) {
    for (u32 i = 0; i < ring->mSize; i++) {
        u32 idx = ring->mFront + i;
        u32 slot = idx % ring->mCapacity;
        if (ring->mArrayPtr[slot].command == cmd)
            return i;
    }
    return 0xFFFFFFFF;
}

// Message-target object stored at ring+0x498. Vtable slots 0-1 are the
// destructor pair; dispatch is word slot 3.
class __declspec(novtable) CGxMessageTarget {
public:
    virtual ~CGxMessageTarget();
    virtual void dispatch(u32 cmd, void* data);
};

// Ring command dispatcher: finds the entry with command == cmd and
// tail-calls the object at ring+0x498 through vtable slot 3 with the payload.
// noinline: retail emits a bl to the dispatcher from every ring setter; IPA
// must not inline it into them.
#pragma push
#pragma auto_inline off
// Inlined-helper form: the `return i` inside the loop is what makes MWCC
// emit the two-branch exit (`bne next; b found`) of the retail search loop.
static inline u32 gxCacheFindCmd(MsgParam32Ring* ring, u32 cmd) {
    for (u32 i = 0; i < ring->mSize; i++) {
        u32 idx = ring->mFront + i;
        u32 slot = idx % ring->mCapacity;
        if (ring->mArrayPtr[slot].command == cmd)
            return i;
    }
    return 0xFFFFFFFF;
}

extern "C" __declspec(noinline) void func_8044CE68__8CGXCacheFv(void* self, u32 cmd) {
    MsgParam32Ring* ring = (MsgParam32Ring*)self;
    u32 i = gxCacheFindCmd(ring, cmd);
    // Statement order reproduces retail's schedule; the virtual-call syntax
    // pins the target object to r3 and pushes the ring copy into r9.
    u32 idx = ring->mFront + i;
    void* obj = ring->field7;
    u32 slot2 = idx % ring->mCapacity;
    MsgParam32Entry* entry = &ring->mArrayPtr[slot2];
    static_cast<CGxMessageTarget*>(obj)->dispatch(cmd, &entry->wid);
}
#pragma pop

#pragma push
#pragma auto_inline off
extern "C" void* func_8044CEF8__8CGXCacheFv(void* self, u32 cmd) {
    MsgParam32Ring* ring = (MsgParam32Ring*)self;
    u32 i = msgRingFind(ring, cmd);
    u32 idx = ring->mFront + i;
    u32 slot = idx % ring->mCapacity;
    return &ring->mArrayPtr[slot].wid;
}
#pragma pop
#pragma push
#pragma auto_inline off
// Find the ring entry whose command == id and return a pointer to its payload
// word (wid). If not found, the slot is computed with i = -1 (wraps to the
// last ring slot), mirroring retail's fallthrough li r8, -1.
extern "C" u32* func_8044CF74__8CGXCacheFv(CMsgParam_32* ringp, u32 id) {
    MsgParam32Ring* ring = (MsgParam32Ring*)ringp;
    u32 i = msgRingFind(ring, id);
    u32 idx = ring->mFront + i;
    u32 slot = idx % ring->mCapacity;
    return &ring->mArrayPtr[slot].wid;
}
#pragma pop

// --- hard-symbol stubs (scaffold_hard_symbols) ---
// CMsgParam<32> ring. Layout mirrors the shared CMsgParam.hpp (vtable at 0x0,
// entries at 0x4, ring state at 0x484+ for N=32). Only func_80449B94 is
// instantiated here; the sibling members stay declared-but-undefined.
template <int N> class CMsgParam {
public:
    void* vtbl;                  //0x0 (vtable pointer)
    CMsgParamEntry mEntries[N];  //0x4
    CMsgParamEntry* mArrayPtr;   //0x4 + N*0x24
    u32 mFront;                  //0x8 + N*0x24
    u32 mSize;                   //0xC + N*0x24
    u32 mCapacity;               //0x10 + N*0x24
    u32 field6;                  //0x14 + N*0x24 (last found index)
    u32 field7;                  //0x18 + N*0x24

    void func_804495C4(unsigned long, u32*);
    void func_8044972C(unsigned long, u8*);
    void func_804498A4(unsigned long, u32*);
    void func_80449A1C(unsigned long, u32*);
    void func_80449B94(unsigned long, u32*);
};
// (mapped to __ct__80449548 above)
// (mapped to __ct__80449548 above)
// (mapped to __ct__80449548 above)
// (mapped to __ct__80449548 above)

// Ring search helper for CMsgParam<N> (inlined at every call site by
// -inline auto). The inlined `return i` / `return 0xFFFFFFFF` reproduce
// retail's `bne skip; b found` two-branch search loop, same rationale as
// ringFindIndex above.
template <int N>
static u32 cmsgFindIndex(CMsgParam<N>* self, u32 msg) {
    // Loop bound kept inline so MWCC hoists it into a short-lived temp
    // register (retail holds the count in r0 for mtctr+cmplwi).
    for (u32 i = 0; i < self->mSize; i++) {
        u32 idx = self->mFront + i;
        u32 slot = idx % self->mCapacity;
        // Compare order matches retail's reversed cmplw r4(msg), r0(command).
        if (msg == self->mArrayPtr[slot].command) return i;
    }
    return 0xFFFFFFFF;
}

// Ring push for a message whose payload comes from a volatile, uninitialized
// stack entry (retail reads 0xc..0x2a(r1) verbatim - MWCC keeps the volatile
// struct at 8(r1) and emits the uninitialized reads). Found entries get
// field6 = i; otherwise a new entry is appended at (mFront+mSize)%mCapacity
// (signed divw) with unk23 = 0. Either way the entry at field6 is then
// stamped unk23 = 3 and wid = *widSrc (the caller passes a pointer to the
// wid source, e.g. lbl_eu_8066362x). Retail symbol is FUl-mangled but the
// body also reads r5, so the declared signature carries the extra u32*.
template <> void CMsgParam<32>::func_80449B94(unsigned long msg, u32* widSrc) {
    u32 i = cmsgFindIndex<32>(this, msg);
    if ((s32)i < 0) goto append;
    field6 = i;
    goto stamp;
append:
    {
        // Retail reloads mFront/mSize/mCapacity/mArrayPtr here instead of
        // reusing the search-loop registers, in exactly that order.
        u32 front = *(volatile u32*)&this->mFront;
        u32 curSize = *(volatile u32*)&this->mSize;
        int index = (int)(front + curSize) % (int)*(volatile u32*)&this->mCapacity;
        CMsgParamEntry* arr = this->mArrayPtr;
        volatile CMsgParamEntry entry;
        u32 wid = entry.wid;
        u32 value8 = entry.unk8;
        u32 valueC = entry.unkC;
        u32 value10 = entry.unk10;
        u32 value14 = entry.unk14;
        u32 value18 = entry.unk18;
        u32 value1C = entry.unk1C;
        u16 value20 = entry.unk20;
        u8 value22 = entry.unk22;
        CMsgParamEntry* e = &arr[index];
        e->command = (u32)msg;
        e->wid = wid;
        e->unk8 = value8;
        e->unkC = valueC;
        e->unk10 = value10;
        e->unk14 = value14;
        e->unk18 = value18;
        e->unk1C = value1C;
        e->unk20 = value20;
        e->unk22 = value22;
        e->unk23 = 0;
        mSize++;
        field6 = mSize - 1;
    }
stamp:
    {
        // Each store recomputes the slot from freshly reloaded members.
        u32 slot = (mFront + field6) % mCapacity;
        mArrayPtr[slot].unk23 = 3;
        u32 slot2 = (mFront + field6) % mCapacity;
        mArrayPtr[slot2].wid = *widSrc;    }
}

// Ring command-0xD cache update carrying an 8-byte payload (two words from
// r5, e.g. a 4x s16 scissor rect read as words). Same shape as
// func_80449B94: find the entry whose command is msg; if found field6 = i,
// otherwise append a new entry at (mFront+mSize)%mCapacity (signed divw,
// uninitialized stack payload via the volatile entry) with unk23 = 0; either
// way the entry at field6 is then stamped unk23 = 0xd and wid/unk8 =
// payload[0..1]. Retail symbol is FUl-mangled but the body also reads r5.
template <> void CMsgParam<32>::func_804495C4(unsigned long msg, u32* payload) {
    u32 i = cmsgFindIndex<32>(this, msg);
    if ((s32)i < 0) goto append;
    field6 = i;
    goto stamp;
append:
    {
        // Retail member-reload order: size, capacity, front (matches its
        // register coloring r6/r7/r8); quotient signed; array pointer
        // reloaded before the division.
        u32 curSize = *(volatile u32*)&this->mSize;
        u32 cap = this->mCapacity;
        u32 front = *(volatile u32*)&this->mFront;
        s32 sum = (s32)(front + curSize);
        // Pinned here: retail issues the array-pointer reload between the
        // index sum and the signed division.
        CMsgParamEntry* arr = (CMsgParamEntry*)*(volatile void**)&this->mArrayPtr;
        s32 q = sum / (s32)cap;
        volatile CMsgParamEntry entry;
        u32 wid = entry.wid;
        u32 value8 = entry.unk8;
        u32 valueC = entry.unkC;
        u32 value10 = entry.unk10;
        u32 value14 = entry.unk14;
        u32 value18 = entry.unk18;
        u32 value1C = entry.unk1C;
        u16 value20 = entry.unk20;
        u8 value22 = entry.unk22;
        CMsgParamEntry* e = &arr[sum - q * (s32)cap];
        e->command = (u32)msg;
        e->wid = wid;
        e->unk8 = value8;
        e->unkC = valueC;
        e->unk10 = value10;
        e->unk14 = value14;
        e->unk18 = value18;
        e->unk1C = value1C;
        e->unk20 = value20;
        e->unk22 = value22;
        e->unk23 = 0;
        mSize++;
        field6 = mSize - 1;
    }
stamp:
    {
        // First slot pass stamps unk23 = 0xd; the payload words are read
        // around the division (p0 before, p1 after). The second pass
        // recomputes the slot from freshly reloaded members.
        u32 p0 = payload[0];
        u32 idx = mFront + field6;
        u32 q1 = idx / mCapacity;
        u32 p1 = payload[1];
        mArrayPtr[idx - q1 * mCapacity].unk23 = 0xd;

        u32 idx2 = mFront + field6;
        u32 q2 = idx2 / mCapacity;
        CMsgParamEntry* t = &mArrayPtr[idx2 - q2 * mCapacity];
        t->wid = p0;
        t->unk8 = p1;
    }
}

// Ring command-0x5 cache update carrying a 4-byte color payload (byte-wise
// copy into wid). Same find/append/stamp shape as func_804495C4.
template <> void CMsgParam<32>::func_8044972C(unsigned long msg, u8* payload) {
    u32 i = cmsgFindIndex<32>(this, msg);
    if ((s32)i < 0) goto append;
    field6 = i;
    goto stamp;
append:
    {
        // Retail reload order: front, size, capacity, sum, array ptr, signed
        // division.
        u32 front = *(volatile u32*)&this->mFront;
        u32 curSize = *(volatile u32*)&this->mSize;
        s32 cap = *(volatile u32*)&this->mCapacity;
        s32 sum = (s32)(front + curSize);
        CMsgParamEntry* arr = (CMsgParamEntry*)*(volatile void**)&this->mArrayPtr;
        volatile CMsgParamEntry entry;
        CMsgParamEntry* e = &arr[sum % cap];
        e->command = (u32)msg;
        e->wid = entry.wid;
        e->unk8 = entry.unk8;
        e->unkC = entry.unkC;
        e->unk10 = entry.unk10;
        e->unk14 = entry.unk14;
        e->unk18 = entry.unk18;
        e->unk1C = entry.unk1C;
        e->unk20 = entry.unk20;
        e->unk22 = entry.unk22;
        e->unk23 = 0;
        mSize++;
        field6 = mSize - 1;
    }
stamp:
    {
        // Payload bytes are read around the first division; the second pass
        // recomputes the slot from freshly reloaded members.
        u32 idx = mFront + field6;
        u32 q = idx / mCapacity;
        u8 b0 = payload[0];
        u8 b1 = payload[1];
        u8 b2 = payload[2];
        u8 b3 = payload[3];
        mArrayPtr[idx - q * mCapacity].unk23 = 0x5;

        u32 idx2 = mFront + field6;
        u32 q2 = idx2 / mCapacity;
        u8* dst = (u8*)&mArrayPtr[idx2 - q2 * mCapacity].wid;
        dst[0] = b0;
        dst[1] = b1;
        dst[2] = b2;
        dst[3] = b3;
    }
}

// Ring command-0xB cache update carrying a 16-byte payload (four words from
// r5). Same find/append/stamp shape as func_804495C4.
template <> void CMsgParam<32>::func_804498A4(unsigned long msg, u32* payload) {
    u32 i = cmsgFindIndex<32>(this, msg);
    if ((s32)i < 0) goto append;
    field6 = i;
    goto stamp;
append:
    {
        // Retail reloads mFront/mSize/mCapacity/mArrayPtr here (in that
        // order); append slot uses signed divw.
        u32 front = *(volatile u32*)&this->mFront;
        u32 curSize = *(volatile u32*)&this->mSize;
        u32 cap = *(volatile u32*)&this->mCapacity;
        CMsgParamEntry* e = &this->mArrayPtr[(int)(front + curSize) % (int)cap];
        volatile CMsgParamEntry entry;
        u32 wid = entry.wid;
        u32 value8 = entry.unk8;
        u32 valueC = entry.unkC;
        u32 value10 = entry.unk10;
        u32 value14 = entry.unk14;
        u32 value18 = entry.unk18;
        u32 value1C = entry.unk1C;
        u16 value20 = entry.unk20;
        u8 value22 = entry.unk22;
        e->command = (u32)msg;
        e->wid = wid;
        e->unk8 = value8;
        e->unkC = valueC;
        e->unk10 = value10;
        e->unk14 = value14;
        e->unk18 = value18;
        e->unk1C = value1C;
        e->unk20 = value20;
        e->unk22 = value22;
        e->unk23 = 0;
        mSize++;
        field6 = mSize - 1;
    }
stamp:
    {
        // Retail stamps unk23 on the ring slot, then recomputes the same slot
        // with fully reloaded members to write the four payload words.
        u32 f = *(volatile u32*)&this->mFront;
        u32 last = *(volatile u32*)&this->field6;
        u32 cap = *(volatile u32*)&this->mCapacity;
        u32 p0 = payload[0];
        u32 idx = f + last;
        u32 q = idx / cap;
        u32 p1 = payload[1];
        u32 p2 = payload[2];
        u32 p3 = payload[3];
        this->mArrayPtr[idx - q * cap].unk23 = 0xb;

        u32 f2 = *(volatile u32*)&this->mFront;
        u32 last2 = *(volatile u32*)&this->field6;
        u32 cap2 = *(volatile u32*)&this->mCapacity;
        u32 idx2 = f2 + last2;
        CMsgParamEntry* e = &this->mArrayPtr[idx2 - (idx2 / cap2) * cap2];
        e->wid = p0;
        e->unk8 = p1;
        e->unkC = p2;
        e->unk10 = p3;
    }
}

// Ring command-0x9 cache update carrying a 16-byte payload (four words from
// r5). Same find/append/stamp shape as func_804495C4. Retail symbol is
// Fv-mangled although the body reads msg (r4) and payload (r5).
template <> void CMsgParam<32>::func_80449A1C(unsigned long msg, u32* payload) {
    u32 i = cmsgFindIndex<32>(this, msg);
    if ((s32)i < 0) goto append;
    field6 = i;
    goto stamp;
append:
    {
        // Volatile keeps the uninitialized-entry reads alive and pins their
        // relative order; arr is volatile-loaded before them so the scheduler
        // cannot sink it past the modulo chain.
        u32 front = *(volatile u32*)&this->mFront;
        u32 curSize = *(volatile u32*)&this->mSize;
        u32 capV = *(volatile u32*)&this->mCapacity;
        CMsgParamEntry* arr = *(CMsgParamEntry* volatile*)&this->mArrayPtr;
        u32 sum = front + curSize;
        s32 q = (s32)sum / (s32)capV;
        volatile CMsgParamEntry entry;
        u32 wid = entry.wid;
        u32 value8 = entry.unk8;
        u32 valueC = entry.unkC;
        u32 value10 = entry.unk10;
        u32 value14 = entry.unk14;
        q *= (s32)capV;
        u32 value18 = entry.unk18;
        u32 value1C = entry.unk1C;
        u16 value20 = entry.unk20;
        u8 value22 = entry.unk22;
        q = (s32)sum - q;
        CMsgParamEntry* e = &arr[q];
        e->command = (u32)msg;
        e->wid = wid;
        e->unk8 = value8;
        e->unkC = valueC;
        e->unk10 = value10;
        e->unk14 = value14;
        e->unk18 = value18;
        e->unk1C = value1C;
        e->unk20 = value20;
        e->unk22 = value22;
        e->unk23 = 0;
        mSize++;
        field6 = mSize - 1;
    }
stamp:
    {
        // Each store recomputes the slot from freshly reloaded members.
        u32 w0 = payload[0];
        u32 w1 = payload[1];
        u32 w2 = payload[2];
        u32 w3 = payload[3];
        u32 slot = (mFront + field6) % mCapacity;
        mArrayPtr[slot].unk23 = 0x9;
        u32 slot2 = (mFront + field6) % mCapacity;
        mArrayPtr[slot2].wid = w0;
        mArrayPtr[slot2].unk8 = w1;
        mArrayPtr[slot2].unkC = w2;
        mArrayPtr[slot2].unk10 = w3;
    }
}

// Pack a color as (r<<24)|(g<<16)|(b<<8)|a with 255-scaled components.
// Products are computed into temporaries first so MWCC defers the frame setup
// until after the multiplies (retail: stwu after the 3rd fmul), then the four
// u8 conversions, then the byte pack. The byte-array assembly makes MWCC emit
// the retail stb/lwz pack (a shift-based expression would use rlwinm/or).
// OPEN ITEM: retail sinks stwu sp,-48 below the 9 lead FPU ops (scheduler
// fingerprint); every source shape tried so far pins it at entry.
u32 __declspec(noinline) func_80449550(ml::CCol4& c) {
    const f32 scale = lbl_eu_8066A37C;
    u8 out[4];
    out[0] = (u8)(s32)(scale * c.r);
    out[1] = (u8)(s32)(scale * c.g);
    out[2] = (u8)(s32)(scale * c.b);
    out[3] = (u8)(s32)(scale * c.a);
    return *(u32*)&out[0];
}

// CGXCache vtable (retail .data label, overwrites the IStateCache vtable).
// lbl_eu_8056BFC8 declared below as dissolved .data array; forward decl as u32[] for vtable store
 extern "C" u32 lbl_eu_8056BFC8[];

// Retail .sdata wid sources for the initial ring command cache (imports).
extern u32 lbl_eu_80663620;
extern u32 lbl_eu_80663624;
extern u32 lbl_eu_80663628;
extern u32 lbl_eu_8066362C;
extern u32 lbl_eu_80663630;
extern u32 lbl_eu_80663634;
extern u32 lbl_eu_80663638;
extern u32 lbl_eu_8066363C;
extern u32 lbl_eu_80663640;

// Full-screen rect helper: reads the render mode again for fbWidth (retail
// loads it straight into the arg register) and stores the 4 s16s at dst.
// Height is a parameter so the caller controls which callee-saved register
// each rect group's height lives in (retail splits r30/r31).
static inline s16 getEFBHeight(void) {
    return *(s16*)&getRenderModeObj__9CDeviceVIFv()->efbHeight;   // retail lha
}
static inline void initFullRect(void* dst, s16 h) {
    s16 w = *(s16*)&getRenderModeObj__9CDeviceVIFv()->fbWidth;     // retail lha
    __ct__80449534(dst, 0, 0, w, h);
}

// Retail __ct__8CGXCacheFv: base ctor (vtable rebind), CMsgParam<32> ring +
// five full-screen rects, identity projection, then the initial ring command
// cache entries (cmds 0..0xd).
CGXCache::CGXCache() {
    u32 payload[4];
    func_800407C8_tmp v4;
    u32 col;
    s16 rect[4];
    s16 fullH;   // heights for the four full-screen rects (retail r30)
    s16 subH;    // heights for the sub-rect + stack rect temps (retail r31)

    // Base ctor stores the IStateCache vtable; rebind to the CGXCache vtable.
    __ct__IStateCache(this);
    vtbl = (void*)lbl_eu_8056BFC8;

    // Ring member: entries inline, owner back-pointer in field7.
    __ct__CMsgParam_32(&unk4, (u32)this);

    // Five full-screen rects (0x4A0, 0x4A8, 0x4B0, 0x4B8) + identity mtx.
    subH = getEFBHeight();
    initFullRect(&rect4A0, subH);
    subH = getEFBHeight();
    initFullRect(&rect4A8, subH);
    subH = getEFBHeight();
    initFullRect(&rect4B0, subH);
    subH = getEFBHeight();
    initFullRect(&unk4B8, subH);
    __ct__80449548(&mProjMtx);

    unk500 = 0x7fff;
    unk502 = 0;
    unk504 = 0;
    unk508 = 0;
    unk50C = 0;

    fullH = getEFBHeight();
    initFullRect(&mRectLeft, fullH);   // 0x510 sub-rect
    mAdjustProj = 0;

    // Ring command cache entries: cmds 0, 3..9, 0xd carry a wid pointer; 1
    // carries a 16-byte payload; 2 a CCol4; 0xa a packed color; 0xb/0xc the
    // full-screen rect words.
    ((CMsgParam<32>*)&unk4)->func_80449B94(0, &lbl_eu_80663620);

    ((void (*)(void*))func_8044954C)(payload);
    payload[0] = 1;
    payload[1] = 4;
    payload[2] = 5;
    payload[3] = 0;
    ((CMsgParam<32>*)&unk4)->func_80449A1C(1, payload);

    func_800407C8(&v4, lbl_eu_8066A378, lbl_eu_8066A378, lbl_eu_8066A378, lbl_eu_8066A378);
    ((CMsgParam<32>*)&unk4)->func_804498A4(2, (u32*)&v4);

    ((CMsgParam<32>*)&unk4)->func_80449B94(3, &lbl_eu_80663624);
    ((CMsgParam<32>*)&unk4)->func_80449B94(4, &lbl_eu_80663628);
    ((CMsgParam<32>*)&unk4)->func_80449B94(5, &lbl_eu_8066362C);
    ((CMsgParam<32>*)&unk4)->func_80449B94(6, &lbl_eu_80663630);
    ((CMsgParam<32>*)&unk4)->func_80449B94(7, &lbl_eu_80663634);
    ((CMsgParam<32>*)&unk4)->func_80449B94(8, &lbl_eu_80663638);
    ((CMsgParam<32>*)&unk4)->func_80449B94(9, &lbl_eu_8066363C);

    col = func_80449550(ml::CCol4::white);
    ((CMsgParam<32>*)&unk4)->func_8044972C(0xa, (u8*)&col);

    fullH = getEFBHeight();
    initFullRect(rect, fullH);
    ((CMsgParam<32>*)&unk4)->func_804495C4(0xb, (u32*)rect);

    fullH = getEFBHeight();
    initFullRect(rect, fullH);
    ((CMsgParam<32>*)&unk4)->func_804495C4(0xc, (u32*)rect);

    ((CMsgParam<32>*)&unk4)->func_80449B94(0xd, &lbl_eu_80663640);
}

// ---- tiny ctor helpers (defined after __ct__8CGXCacheFv so they are not
// inlined; retail emits a bl to each) ----

#pragma push
#pragma auto_inline off
extern "C" __declspec(noinline) void __ct__CMsgParam_32(void* self, u32 param) {
    u8* s = (u8*)self;
    *(void**)s = lbl_eu_8056BFE4;
    *(u32*)(s + 0x490) = 32;
    *(u32*)(s + 0x484) = (u32)(s + 4);
    *(u32*)(s + 0x48C) = 0;
    *(u32*)(s + 0x488) = 0;
    *(u32*)(s + 0x494) = 0;
    *(u32*)(s + 0x498) = param;
}

extern "C" __declspec(noinline) void __ct__80449534(void* self, s16 a, s16 b, s16 c, s16 d) {
    u8* s = (u8*)self;
    *(u16*)(s + 0) = a;
    *(u16*)(s + 2) = b;
    *(u16*)(s + 4) = c;
    *(u16*)(s + 6) = d;
}

// No-op helpers called with an address in r3 (retail `addi r3,..; bl`).
__declspec(noinline) void func_8044954C(void* self) { (void)self; }
__declspec(noinline) void __ct__80449548(void* self) { (void)self; }
#pragma pop

// IStateCache ctor: vtable store (retail data label). char[] avoids SDA21.
#pragma push
#pragma auto_inline off
extern "C" __declspec(noinline) void __ct__IStateCache(void* self) {
    *(void**)self = lbl_eu_8056BFF0;
}
#pragma pop

// ==== .data dissolved (retail bytes/relocs from build/us/asm/monolib/src/device/CGXCache.s) ====
// .data 0x8056BF90 0x38 jumptable + 0x8056BFC8 0x10 CGXCache vtable + 0x8056BFD8 0xC + 0x8056BFE4 0xC + 0x8056BFF0 0x10
// Emitted as extern "C" u32 arrays with (u32)&extern, rodata align, DECOMP_FORCEACTIVE, novtable.
// Jumptable addends dumped from data diff retail: 76,100,124,264,404,448,500,552,564,576,1348,1372,1592,420
// (verified against CGXCache.s .rel lines). Uses (u32)((char*)&extern+offset) for correct R_PPC_ADDR32 addends.
// func_80449D68__8CGXCacheFv already declared at top with (CGXCache*,u32,void*) signature; reuse it for jumptable addends.
// (MWCC now emits the func_80449D68 jumptable itself.)
extern "C" u32 lbl_eu_8056BFC8[4] = {
    (u32)&lbl_eu_80663658, 0x00000000,
    (u32)&__dt__8CGXCacheFv, (u32)&func_80449D68__8CGXCacheFv,
};
extern "C" __attribute__((aligned(4))) u32 lbl_eu_8056BFD8[3] = {
    (u32)&lbl_eu_80663668, 0x00000000, 0x00000000,
};
extern "C" __attribute__((aligned(4))) u32 lbl_eu_8056BFE4[3] = {
    (u32)&lbl_eu_80663660, 0x00000000,
    (u32)&__dt__CMsgParam_32,
};
extern "C" u32 lbl_eu_8056BFF0[4] = {
    (u32)&lbl_eu_80663668, 0x00000000,
    (u32)&__dt__11IStateCacheFv, 0x00000000
};
DECOMP_FORCEACTIVE(CGXCache_cpp, lbl_eu_8056BFC8, lbl_eu_8056BFD8, lbl_eu_8056BFE4, lbl_eu_8056BFF0, rodata_CGXCache, sdata_CGXCache, sdata2_pool_CGXCache);
