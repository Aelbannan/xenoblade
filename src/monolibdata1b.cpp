/* Data-only TU for monolibdata1b (retail: .sbss 0x80665581..0x80665588, 7
 * bytes, align 1, NO relocs, single LOCAL pad symbol pad_10_80665581_sbss).
 * Replaces the generated monolibdata1b.s dump.
 *
 * The 7 bytes are a byte-unaligned .sbss gap between lbl_eu_80665580 (1B,
 * preceding unit) and lbl_eu_80665588 (8B, monolib CRect16.cpp); no retail
 * symbol owns them. Static so the binding is LOCAL like retail; kept from
 * -ipa file elimination with #pragma force_active (7 bytes <= the 8-byte
 * small-data threshold, so it lands in .sbss). NB: MWCC min-aligns NOBITS
 * sections to 8, so the .sbss section comes out align 8 while the retail
 * split .o (slice starts at the odd address 0x80665581) is align 1 -- a
 * splitter slice-address artifact, not reproducible by any MWCC TU.
 */

#pragma force_active on
static unsigned char pad_80665581[7];
#pragma force_active off
