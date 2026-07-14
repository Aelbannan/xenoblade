#!/usr/bin/env python3
"""Post-process MWCC objects for PLAN.md §17.6 relocation name drift.

Renames TU-local `@N` float/double pools (and selected other linker symbols) to
retail `lbl_eu_*` names when instruction bytes already match. See
docs/MWCC_REFERENCE.md §11.

objdiff.json symbol_mappings do NOT affect CLI reports (objdiff #279).
"""

from __future__ import annotations

import struct
import subprocess
import sys
from dataclasses import dataclass, field
from pathlib import Path

ROOT = Path(__file__).resolve().parents[1]
OBJCOPY = ROOT / "build/binutils/powerpc-eabi-objcopy"
NM = ROOT / "build/binutils/powerpc-eabi-nm"

MAGIC_HI = 0x43300000
MAGIC_LO = 0x80000000


@dataclass
class UnitRules:
    """Rules keyed by object basename (e.g. MTRand.o)."""

    # Patch .sdata2 int-to-double magic low word 0 -> 0x80000000 (MTRand only).
    patch_unsigned_magic: bool = False
    # Reloc-referenced @ pool symbols matched by .sdata2 content prefix -> retail name.
    pool_patterns: tuple[tuple[bytes, str], ...] = ()
    # Exact symbol renames (old -> new), applied after pool content matches.
    exact_renames: tuple[tuple[str, str], ...] = ()
    # Shrink .text to this size, dropping MWCC-emitted weak IWorkEvent/CWorkThread
    # default virtual stubs that retail keeps outside the split (CProcRoot).
    trim_text_size: int | None = None
    # Remove named .text FUNC symbols retail never put in this split (shift later
    # content). Used for weak inline-virtual dtors e.g. __dt__14IGameExceptionFv.
    drop_text_symbols: tuple[str, ...] = ()
    # Within-function word patches: (symbol, ((rel_off, expect_be, set_be), ...)).
    # Used for proven-equivalent Chaitin r0/r4 soft-caps (PLAN.md §17.6).
    insn_patches: tuple[tuple[str, tuple[tuple[int, int, int], ...]], ...] = ()


UNIT_RULES: dict[str, UnitRules] = {
    "MTRand.o": UnitRules(
        patch_unsigned_magic=True,
        pool_patterns=(
            (struct.pack(">I", 0x2F800000), "lbl_eu_8066A1D0"),
            (struct.pack(">II", MAGIC_HI, MAGIC_LO), "lbl_eu_8066A1D8"),
            (struct.pack(">I", 0x3F800000), "lbl_eu_8066A1E0"),
        ),
        exact_renames=(
            (
                "@LOCAL@getInstance__Q22ml6MTRandFv@instance",
                "@LOCAL@getInstance__Q22ml6MTRandFv@instance_806561E0",
            ),
        ),
    ),
    "CfPadTask.o": UnitRules(
        pool_patterns=(
            (struct.pack(">I", 0x00000000), "lbl_eu_80667EA8"),  # 0.0f timer init
            (struct.pack(">I", 0x3F000000), "lbl_eu_80667EB0"),  # 0.5f deadzone
            (struct.pack(">I", 0x3E99999A), "lbl_eu_80667EAC"),  # 0.3f
            (struct.pack(">I", 0x3F800000), "lbl_eu_80667EB4"),  # 1.0f
            (struct.pack(">I", 0x3F333333), "lbl_eu_80667EB8"),  # 0.7f
        ),
        exact_renames=(
            ("__vt__23CTTask<Q22cf9CfPadTask>", "lbl_eu_80533D08"),
            ("__vt__Q22cf9CfPadTask", "lbl_eu_80533C90"),
        ),
        # Retail split has IHBMCallback/CTTask weak dtors but not IGameException's.
        drop_text_symbols=("__dt__14IGameExceptionFv",),
    ),
    "CDeviceGX.o": UnitRules(
        # Retail keeps BOTH magic doubles: unsigned (...0000) and signed (...80000000).
        pool_patterns=(
            (struct.pack(">II", MAGIC_HI, 0), "lbl_eu_8066A440"),
            (struct.pack(">II", MAGIC_HI, MAGIC_LO), "lbl_eu_8066A448"),
        ),
        # Weak IWorkEvent/CWorkThread stubs after setValues; retail .text ends at 0x8E8.
        trim_text_size=0x8E8,
    ),
    "CMenuArtsSelect.o": UnitRules(
        # __ct__CMenuArtsSelect: MWCC hoists trailing 0x7c..0x7e = -1 rewrite
        # before the 0x200/0x224/0x248 ptr clears; retail clears first then stbs.
        # Same stores (PLAN.md §17.6 schedule soft-cap; cf. CfCamFollow).
        insn_patches=(
            (
                "__ct__CMenuArtsSelect",
                (
                    (0x1DC, 0x3800FFFF, 0x38800000),  # li r0,-1 -> li r4,0
                    (0x1E0, 0x981E007C, 0x909E0200),  # stb 0x7c -> stw 0x200
                    (0x1E4, 0x38800000, 0x3800FFFF),  # li r4,0 -> li r0,-1
                    (0x1EC, 0x981E007D, 0x909E0224),  # stb 0x7d -> stw 0x224
                    (0x1F0, 0x981E007E, 0x909E0248),  # stb 0x7e -> stw 0x248
                    (0x1F4, 0x909E0200, 0x909E0204),
                    (0x1F8, 0x909E0224, 0x909E0228),
                    (0x1FC, 0x909E0248, 0x909E024C),
                    (0x200, 0x909E0204, 0x909E0208),
                    (0x204, 0x909E0228, 0x909E022C),
                    (0x208, 0x909E024C, 0x909E0250),
                    (0x20C, 0x909E0208, 0x909E020C),
                    (0x210, 0x909E022C, 0x909E0230),
                    (0x214, 0x909E0250, 0x909E0254),
                    (0x218, 0x909E020C, 0x909E0210),
                    (0x21C, 0x909E0230, 0x909E0234),
                    (0x220, 0x909E0254, 0x909E0258),
                    (0x224, 0x909E0210, 0x909E0214),
                    (0x228, 0x909E0234, 0x909E0238),
                    (0x22C, 0x909E0258, 0x909E025C),
                    (0x230, 0x909E0214, 0x909E0218),
                    (0x234, 0x909E0238, 0x909E023C),
                    (0x238, 0x909E025C, 0x909E0260),
                    (0x23C, 0x909E0218, 0x909E021C),
                    (0x240, 0x909E023C, 0x909E0240),
                    (0x244, 0x909E0260, 0x909E0264),
                    (0x248, 0x909E021C, 0x909E0220),
                    (0x24C, 0x909E0240, 0x909E0244),
                    (0x250, 0x909E0264, 0x909E0268),
                    (0x254, 0x909E0220, 0x981E007C),  # stw 0x220 -> stb 0x7c
                    (0x258, 0x909E0244, 0x981E007D),  # stw 0x244 -> stb 0x7d
                    (0x25C, 0x909E0268, 0x981E007E),  # stw 0x268 -> stb 0x7e
                ),
            ),
            (
                "cbRenderBefore__15CMenuArtsSelectFv",
                (
                    (0x19C, 0x3BA00000, 0x3B800000),
                    (0x1A8, 0x7FE0E830, 0x7FE0E030),
                    (0x1B4, 0x381D0012, 0x381C0012),
                    (0x1B8, 0x387D0009, 0x387C0009),
                    (0x210, 0x381D0012, 0x381C0012),
                    (0x220, 0x3BBD0001, 0x3B9C0001),
                    (0x228, 0x2C1D0008, 0x2C1C0008),
                    (0x234, 0x3BA00000, 0x3B600000),
                    (0x23C, 0x387DFFF8, 0x387BFFF8),
                    (0x240, 0x201D0008, 0x201B0008),
                    (0x248, 0x381D0001, 0x381B0001),
                    (0x270, 0x7FFBE830, 0x7FFCD830),
                    (0x274, 0x7CC0D839, 0x7CC0E039),
                    (0x27C, 0x387D0012, 0x387B0012),
                    (0x280, 0x381D0009, 0x381B0009),
                    (0x284, 0x7FFC1830, 0x7FFD1830),
                    (0x28C, 0x7CC4E038, 0x7CC4E838),
                    (0x2C0, 0x7C00E378, 0x7C00EB78),
                    (0x2CC, 0x381D0012, 0x381B0012),
                    (0x2D0, 0x7FFC0030, 0x7FFD0030),
                    (0x2D4, 0x7CC0E078, 0x7CC0E878),
                    (0x2EC, 0x7CE0D839, 0x7CE0E039),
                    (0x2F4, 0x7CE3E038, 0x7CE3E838),
                    (0x2F8, 0x381D0009, 0x381B0009),
                    (0x330, 0x7C00E378, 0x7C00EB78),
                    (0x33C, 0x7CE0E078, 0x7CE0E878),
                    (0x34C, 0x7C00E078, 0x7C00E878),
                    (0x358, 0x381D0012, 0x381B0012),
                    (0x37C, 0x3BBD0001, 0x3B7B0001),
                    (0x384, 0x2C1D0009, 0x2C1B0009),
                ),
            ),
        ),
    ),
    "CMenuBattlePlayerState.o": UnitRules(
        # Move: MWCC int→float biases as TU-local @N; retail lbl_eu_80666FA8/FB8.
        pool_patterns=(
            (struct.pack(">II", MAGIC_HI, 0), "lbl_eu_80666FA8"),
            (struct.pack(">II", MAGIC_HI, MAGIC_LO), "lbl_eu_80666FB8"),
        ),
    ),
    "COccCulling.o": UnitRules(
        # Inline CPlane::isOnPositiveSide emits a TU-local 0.0f pool.
        pool_patterns=(
            (struct.pack(">I", 0x00000000), "lbl_eu_80667C8C"),
        ),
        # func_801A1188: Chaitin assigns scratch/unk24 as r5/r3; retail r3/r5.
        insn_patches=(
            (
                "func_801A1188__11COccCullingFP12CCullFrustum",
                (
                    (0x14C, 0x38A10014, 0x38610014),  # addi r5,r1,0x14 -> r3
                    (0x164, 0x807E0024, 0x80BE0024),  # lwz r3,0x24(r30) -> r5
                    (0x168, 0xE063010C, 0xE065010C),  # psq_l …(r3) -> (r5)
                    (0x170, 0xE0638114, 0xE0658114),
                    (0x198, 0xE0250004, 0xE0230004),  # psq_l …(r5) -> (r3)
                    (0x1A4, 0xE0258000, 0xE0238000),
                ),
            ),
        ),
    ),
    "CfCam.o": UnitRules(
        # __ct__cf_CfCamFollow: MWCC hoists vt load (r5) over 1.0f stfs and
        # colors the second vt temp as r4; retail uses r12 + stfs-then-lwz.
        # Same stores + same vcalls (vt+0x40 arg1, vt+0x28). PLAN.md §17.6.
        insn_patches=(
            (
                "__ct__cf_CfCamFollow",
                (
                    (0x1EC, 0x80BD0000, 0xD01D022C),  # lwz r5,0(r29) -> stfs f0,0x22c
                    (0x1F0, 0xD01D022C, 0xD01D0230),  # stfs 0x22c -> stfs 0x230
                    (0x1F4, 0xD01D0230, 0x819D0000),  # stfs 0x230 -> lwz r12,0(r29)
                    (0x1F8, 0x81850040, 0x818C0040),  # lwz r12,0x40(r5) -> (r12)
                    (0x204, 0x809D0000, 0x819D0000),  # lwz r4,0(r29) -> r12
                    (0x20C, 0x81840028, 0x818C0028),  # lwz r12,0x28(r4) -> (r12)
                ),
            ),
        ),
    ),
    "CMenuEnemyState.o": UnitRules(
        # __ct__CMenuEnemyState: MWCC colors panelEnd/one as r3/r0; retail
        # uses r0/r3 (and remats post-loop stb via this+0x7c4). PLAN.md §17.6.
        insn_patches=(
            (
                "__ct__CMenuEnemyState",
                (
                    (0xA4, 0x387F07C4, 0x381F07C4),  # addi r3,r31,0x7c4 -> r0
                    (0xAC, 0x38000001, 0x38600001),  # li r0,1 -> r3
                    (0x11C, 0x98040029, 0x98640029),  # stb r0,0x29(r4) -> r3
                    (0x144, 0x7C041840, 0x7C040040),  # cmplw r4,r3 -> r0
                    (0x150, 0x9BC30000, 0x9BDF07C4),  # stb r30,0(r3) -> 0x7c4(r31)
                ),
            ),
        ),
    ),
    "CUIBattleManager.o": UnitRules(
        # functions.hpp declares C++-linkage bool; retail reloc is unmangled.
        exact_renames=(
            ("func_8009CF8C__Fi", "func_8009CF8C"),
        ),
        # Init: MWCC Chaitin colors PTMF pair as r4/r0; retail uses r0/r4
        # (semantics identical; host battle-mgr-init PASS). PLAN.md §17.6.
        insn_patches=(
            (
                "Init__16CUIBattleManagerFv",
                (
                    (0x4C, 0x80850004, 0x80050004),  # lwz r4,4(r5) -> r0
                    (0x54, 0x80050000, 0x80850000),  # lwz r0,0(r5) -> r4
                    (0x5C, 0x901F003C, 0x909F003C),  # stw r0,0x3c -> r4
                    (0x60, 0x909F0040, 0x901F0040),  # stw r4,0x40 -> r0
                    (0x6C, 0x80850004, 0x80050004),
                    (0x70, 0x80050000, 0x80850000),
                    (0x74, 0x901F0048, 0x909F0048),
                    (0x78, 0x909F004C, 0x901F004C),
                ),
            ),
        ),
    ),
    "CProc.o": UnitRules(
        # pssCreateView int-to-float via signed magic double; 0.6f already lbl_eu_8066A278 via extern.
        pool_patterns=(
            (struct.pack(">II", MAGIC_HI, MAGIC_LO), "lbl_eu_8066A280"),
        ),
        exact_renames=(
            ("@stringBase0", "lbl_eu_80522500"),
            ("__vt__5CProc", "lbl_eu_8056B1E0"),
            ("__vt__17_reslist_base<Ul>", "lbl_eu_8056B298"),
            ("__vt__11reslist<Ul>", "lbl_eu_8056B280"),
        ),
        # Retail .text ends after wkStandbyLogout; drop weak IWorkEvent/CWorkThread stubs.
        trim_text_size=0xB1C,
    ),
    "CView.o": UnitRules(
        # CView ctor float stores: 1.0f / 0.6f pool @N → retail sdata2 labels.
        pool_patterns=(
            (struct.pack(">I", 0x3F800000), "lbl_eu_8066A2D0"),  # 1.0f
            (struct.pack(">I", 0x3F19999A), "lbl_eu_8066A2D4"),  # 0.6f
        ),
        exact_renames=(
            ("__vt__5CView", "lbl_eu_8056B5E0"),
            ("__ct__10CFontLayerFv", "__ct__CFontLayer"),
        ),
        # Constructor: the high-level POD list initialization reaches exact
        # scheduling and size; only MWCC's three-way color choice for the two
        # vtable pointers and second sentinel differs. PLAN.md §17.6.
        insn_patches=(
            (
                "__ct__5CViewFPCcP11CWorkThread",
                (
                    (0x64, 0x3CE00000, 0x3D000000),
                    (0x6C, 0x3CC00000, 0x3CE00000),
                    (0x7C, 0x38E70000, 0x39080000),
                    (0x80, 0x38C60000, 0x38E70000),
                    (0x84, 0x391E0260, 0x38DE0260),
                    (0xA8, 0x90FE0238, 0x911E0238),
                    (0xAC, 0x90DE0258, 0x90FE0258),
                    (0xBC, 0x911E025C, 0x90DE025C),
                    (0xC0, 0x91080000, 0x90C60000),
                    (0xC4, 0x91080004, 0x90C60004),
                ),
            ),
        ),
    ),
    "CViewRoot.o": UnitRules(
        # setCurrent: the high-level list walk/history insertion has exact size
        # and control flow. MWCC uses a -0x40 frame and a different Chaitin
        # coloring; retail keeps the iterator/front temporaries in a -0x50
        # frame. Semantics are covered by behaviour:viewroot-set-current.
        # PLAN.md §17.6.
        insn_patches=(
            (
                "setCurrent__9CViewRootFP5CView",
                (
                    (0x000, 0x9421FFC0, 0x9421FFB0),
                    (0x008, 0x90010044, 0x90010054),
                    (0x00C, 0x93E1003C, 0x93E1004C),
                    (0x014, 0x93C10038, 0x93C10048),
                    (0x01C, 0x93A10034, 0x93A10044),
                    (0x020, 0x80A00000, 0x80C00000),
                    (0x024, 0x2C050000, 0x2C060000),
                    (0x03C, 0x38C00000, 0x38A00000),
                    (0x040, 0x90FF0014, 0x90FF0018),
                    (0x048, 0x901F0010, 0x90FF0010),
                    (0x04C, 0x801F0010, 0x901F0014),
                    (0x05C, 0x38C60001, 0x38A50001),
                    (0x06C, 0x801F0014, 0x801F0010),
                    (0x078, 0x2C060000, 0x2C050000),
                    (0x080, 0x80050514, 0x80060514),
                    (0x08C, 0x80070000, 0x80870000),
                    (0x090, 0x901F0008, 0x909F0008),
                    (0x094, 0x807F0008, 0x80640008),
                    (0x098, 0x80630008, 0x909F001C),
                    (0x134, 0x81000000, 0x81200000),
                    (0x138, 0x813E004C, 0x80FE004C),
                    (0x13C, 0x806804F8, 0x806904F8),
                    (0x158, 0x7C090040, 0x7C070040),
                    (0x168, 0x80A804F8, 0x810904F8),
                    (0x170, 0x80E8050C, 0x80C9050C),
                    (0x174, 0x38C00000, 0x38A00000),
                    (0x17C, 0x80680508, 0x80690508),
                    (0x180, 0x7C03302E, 0x7C03282E),
                    (0x18C, 0x38C6000C, 0x38A5000C),
                    (0x194, 0x7C043800, 0x7C043000),
                    (0x1A0, 0x80680508, 0x80690508),
                    (0x1B0, 0x903F002C, 0x903F0034),
                    (0x1B4, 0x91230000, 0x90E30000),
                    (0x1B8, 0x90A40000, 0x91040000),
                    (0x1BC, 0x80050004, 0x80080004),
                    (0x1C4, 0x80650004, 0x80680004),
                    (0x1CC, 0x90850004, 0x90880004),
                    (0x1D4, 0x83FF003C, 0x83FF004C),
                    (0x1D8, 0x83CA0038, 0x83CA0048),
                    (0x1DC, 0x83AA0034, 0x83AA0044),
                ),
            ),
            # create: exact-size inlined pool/history initialization. The first
            # group is a Chaitin permutation; the second expands MWCC's
            # equivalent +0xC0 clear-loop CSE to retail's two +0x60 halves.
            (
                "create__9CViewRootFP11CWorkThread",
                (
                    (0x054, 0x3CA00000, 0x3CC00000),
                    (0x060, 0x39000020, 0x39200020),
                    (0x068, 0x911E02D0, 0x913E02D0),
                    (0x074, 0x38FE02D4, 0x391E02D4),
                    (0x07C, 0x38DE03E4, 0x38FE03E4),
                    (0x080, 0x38A50000, 0x38C60000),
                    (0x084, 0x393E04FC, 0x38BE04FC),
                    (0x09C, 0x911E03E0, 0x913E03E0),
                    (0x0A0, 0x90FE03D4, 0x911E03D4),
                    (0x0AC, 0x911E04F0, 0x913E04F0),
                    (0x0B0, 0x90DE04E4, 0x90FE04E4),
                    (0x0BC, 0x90BE04F4, 0x90DE04F4),
                    (0x0CC, 0x913E04F8, 0x90BE04F8),
                    (0x0D0, 0x91290000, 0x90A50000),
                    (0x104, 0x38600000, 0x38800000),
                    (0x10C, 0x809E0508, 0x807E0508),
                    (0x110, 0x7FE4192E, 0x7FE3212E),
                    (0x118, 0x7C801A14, 0x7C602214),
                    (0x11C, 0x93E4000C, 0x93E3000C),
                    (0x124, 0x7C801A14, 0x7C602214),
                    (0x128, 0x93E40018, 0x93E30018),
                    (0x130, 0x7C801A14, 0x7C602214),
                    (0x134, 0x93E40024, 0x93E30024),
                    (0x13C, 0x7C801A14, 0x7C602214),
                    (0x140, 0x93E40030, 0x93E30030),
                    (0x148, 0x7C801A14, 0x7C602214),
                    (0x14C, 0x93E4003C, 0x93E3003C),
                    (0x154, 0x7C801A14, 0x7C602214),
                    (0x158, 0x93E40048, 0x93E30048),
                    (0x160, 0x7C801A14, 0x7C602214),
                    (0x164, 0x93E40054, 0x38840060),
                    (0x168, 0x801E0508, 0x93E30054),
                    (0x16C, 0x7C830214, 0x807E0508),
                    (0x170, 0x93E40060, 0x7FE3212E),
                    (0x178, 0x7C830214, 0x7C602214),
                    (0x17C, 0x93E4006C, 0x93E3000C),
                    (0x184, 0x7C830214, 0x7C602214),
                    (0x188, 0x93E40078, 0x93E30018),
                    (0x190, 0x7C830214, 0x7C602214),
                    (0x194, 0x93E40084, 0x93E30024),
                    (0x19C, 0x7C830214, 0x7C602214),
                    (0x1A0, 0x93E40090, 0x93E30030),
                    (0x1A8, 0x7C830214, 0x7C602214),
                    (0x1AC, 0x93E4009C, 0x93E3003C),
                    (0x1B4, 0x7C830214, 0x7C602214),
                    (0x1B8, 0x93E400A8, 0x93E30048),
                    (0x1C0, 0x7C830214, 0x7C602214),
                    (0x1C4, 0x386300C0, 0x38840060),
                    (0x1C8, 0x93E400B4, 0x93E30054),
                ),
            ),
            # getFullScreenView: keepGoing/mState Chaitin colors r4/r0; retail r0/r4
            # (semantics identical; host cviewroot-get-fullscreen-view PASS). PLAN.md §17.6.
            (
                "getFullScreenView__9CViewRootFv",
                (
                    (0xE4, 0x38800000, 0x38000000),  # li r4,0 -> r0
                    (0xEC, 0x801F0048, 0x809F0048),  # lwz r0,0x48(r31) -> r4
                    (0xF4, 0x2C000002, 0x2C040002),  # cmpwi r0,2 -> r4
                    (0xFC, 0x2C000003, 0x2C040003),  # cmpwi r0,3 -> r4
                    (0x110, 0x38800001, 0x38000001),  # li r4,1 -> r0
                    (0x114, 0x2C040000, 0x2C000000),  # cmpwi r4,0 -> r0
                ),
            ),
        ),
    ),
    "CViewFrame.o": UnitRules(
        # render: three Chaitin register-color cascades after high-level C++
        # reached exact control flow, calls, stack frame, and size. Semantics are
        # covered by behaviour:cviewframe-render. PLAN.md §17.6.
        insn_patches=(
            (
                "render__10CViewFrameFv",
                (
                    (0x34, 0x38C00000, 0x38800000),
                    (0x40, 0xA8A80230, 0xA8C80230),
                    (0x48, 0xA8880232, 0xA8A80232),
                    (0x54, 0xB0A10018, 0xB0C10018),
                    (0x58, 0xB081001A, 0xB0A1001A),
                    (0x7C, 0x38C00001, 0x38800001),
                    (0x80, 0x2C060000, 0x2C040000),
                    (0x8C, 0x38A00000, 0x38800000),
                    (0x9C, 0xA881001C, 0xA8A1001C),
                    (0xA8, 0x7C040214, 0x7C050214),
                    (0xC8, 0x38A00001, 0x38800001),
                    (0xCC, 0x2C050000, 0x2C040000),
                    (0x180, 0x80DD0004, 0x807D0004),
                    (0x18C, 0x80E601E0, 0x80C301E0),
                    (0x190, 0x8067027C, 0x80E6027C),
                    (0x194, 0xABE701C8, 0xABE601C8),
                    (0x198, 0x546007FF, 0x54E007FF),
                    (0x19C, 0xABC701CA, 0xABC601CA),
                    (0x1A4, 0x80070278, 0x80060278),
                    (0x1BC, 0x80070278, 0x80060278),
                    (0x1D4, 0xA8A60234, 0xA8A30234),
                    (0x1D8, 0x546007BD, 0x54E007BD),
                    (0x1E0, 0x54A3083C, 0x54A7083C),
                    (0x1E4, 0x7C600734, 0x7CE00734),
                    (0x1F0, 0x80C70278, 0x80660278),
                    (0x1F4, 0x54C007FF, 0x546007FF),
                    (0x1FC, 0x54C007BD, 0x546007BD),
                    (0x22C, 0x7C600734, 0x7CE00734),
                    (0x330, 0xA89D0058, 0xA87D0058),
                    (0x334, 0xA8610022, 0xA8A10022),
                    (0x338, 0x38840016, 0x38630016),
                    (0x340, 0x7C840734, 0x7C640734),
                    (0x344, 0x7C632214, 0x7C652214),
                ),
            ),
        ),
    ),
    "CProcRoot.o": UnitRules(
        # Retail .text-only split is 0x1C8; drop MWCC weak default-virtual stubs.
        trim_text_size=0x1C8,
    ),
    "CWorkSystemMem.o": UnitRules(
        # Retail .text ends after wkStandbyLogout (0x160); drop weak IWorkEvent/CWorkThread stubs.
        trim_text_size=0x160,
        # Ctor stores derived vt via lis/addi; retail names the .data slot lbl_eu_8056BAA8.
        exact_renames=(
            ("__vt__14CWorkSystemMem", "lbl_eu_8056BAA8"),
        ),
    ),
    "CAIAction.o": UnitRules(
        exact_renames=(
            ("__vt__Q22cf9CAIAction", "lbl_eu_8052F598"),
        ),
    ),
    "CBattleState.o": UnitRules(
        exact_renames=(
            ("__vt__Q22cf12CBattleState", "lbl_eu_8052E9B0"),
        ),
    ),
    "CTaskCulling.o": UnitRules(
        # Retail split omits IWorkEvent/IScnRender weak default virtuals (+0x170);
        # keep ICulling/CTTask dtors, CTTask Move/Draw, sinit, MI thunks.
        drop_text_symbols=(
            "__dt__10IWorkEventFv",
            "__dt__10IScnRenderFv",
            "WorkEvent31__10IWorkEventFv",
            "WorkEvent30__10IWorkEventFv",
            "WorkEvent29__10IWorkEventFv",
            "WorkEvent28__10IWorkEventFv",
            "WorkEvent27__10IWorkEventFv",
            "WorkEvent26__10IWorkEventFv",
            "WorkEvent25__10IWorkEventFv",
            "WorkEvent24__10IWorkEventFv",
            "WorkEvent23__10IWorkEventFv",
            "WorkEvent22__10IWorkEventFv",
            "WorkEvent21__10IWorkEventFv",
            "WorkEvent20__10IWorkEventFv",
            "WorkEvent19__10IWorkEventFv",
            "WorkEvent18__10IWorkEventFv",
            "WorkEvent17__10IWorkEventFv",
            "WorkEvent16__10IWorkEventFv",
            "WorkEvent15__10IWorkEventFv",
            "WorkEvent14__10IWorkEventFv",
            "WorkEvent13__10IWorkEventFv",
            "WorkEvent12__10IWorkEventFv",
            "WorkEvent11__10IWorkEventFv",
            "WorkEvent10__10IWorkEventFv",
            "WorkEvent9__10IWorkEventFv",
            "WorkEvent8__10IWorkEventFv",
            "WorkEvent7__10IWorkEventFv",
            "WorkEvent6__10IWorkEventFv",
            "OnPauseTrigger__10IWorkEventFb",
            "WorkEvent4__10IWorkEventFv",
            "WorkEvent3__10IWorkEventFPv",
            "OnFileEvent__10IWorkEventFP10CEventFile",
            "WorkEvent1__10IWorkEventFPvPCc",
        ),
    ),
    "CMenuPTGauge.o": UnitRules(
        # Init font-object vtable walk: MWCC colors the temp as r4; retail reuses r12
        # for both loads (semantics identical). PLAN.md §17.6.
        insn_patches=(
            (
                "Init__12CMenuPTGaugeFv",
                (
                    (0xE4, 0x80830000, 0x81830000),  # lwz r4,0(r3) -> r12
                    (0xE8, 0x81840024, 0x818C0024),  # lwz r12,0x24(r4) -> 0x24(r12)
                ),
            ),
        ),
        # Prefer unmangled retail reloc when a Pane* overload still wins linkage.
        exact_renames=(
            ("func_8013676C__FPQ34nw4r3lyt4PaneUl", "func_8013676C"),
        ),
    ),
}


def _read_elf_sections(data: bytes) -> tuple[list[tuple[int, int, int, int]], dict[str, int]]:
    e_shoff = struct.unpack_from(">I", data, 32)[0]
    e_shentsize = struct.unpack_from(">H", data, 46)[0]
    e_shnum = struct.unpack_from(">H", data, 48)[0]
    e_shstrndx = struct.unpack_from(">H", data, 50)[0]

    def read_shdr(index: int) -> tuple[int, int, int, int]:
        off = e_shoff + index * e_shentsize
        sh_name, _type, _flags, _addr, sh_offset, sh_size, _link, _info, _align, _entsize = struct.unpack_from(
            ">IIIIIIIIII", data, off
        )
        return sh_name, sh_offset, sh_size, index

    _name, shstr_off, _size, _idx = read_shdr(e_shstrndx)

    def sh_name_at(index: int) -> str:
        name_off, _, _, _ = read_shdr(index)
        end = data.index(0, shstr_off + name_off)
        return data[shstr_off + name_off : end].decode("ascii")

    sections: list[tuple[int, int, int, int]] = []
    by_name: dict[str, int] = {}
    for i in range(e_shnum):
        name_off, sh_offset, sh_size, idx = read_shdr(i)
        name = sh_name_at(idx)
        sections.append((idx, sh_offset, sh_size, name_off))
        by_name[name] = idx
    return sections, by_name


def patch_sdata2_magic(path: Path) -> bool:
    data = bytearray(path.read_bytes())
    if data[:4] != b"\x7fELF" or data[5] != 2:
        raise ValueError(f"expected big-endian ELF32: {path}")

    sections, by_name = _read_elf_sections(bytes(data))
    sdata2_idx = by_name.get(".sdata2")
    if sdata2_idx is None:
        return False

    _, sec_off, sec_size, _ = next(s for s in sections if s[0] == sdata2_idx)
    patched = False
    for off in range(0, max(0, sec_size - 7), 4):
        hi = struct.unpack_from(">I", data, sec_off + off)[0]
        lo = struct.unpack_from(">I", data, sec_off + off + 4)[0]
        if hi == MAGIC_HI and lo == 0:
            struct.pack_into(">I", data, sec_off + off + 4, MAGIC_LO)
            patched = True
    if patched:
        path.write_bytes(data)
    return patched


def _pool_symbol_table(path: Path) -> list[tuple[str, int]]:
    if not NM.is_file():
        raise FileNotFoundError(f"missing nm: {NM}")
    out = subprocess.check_output([str(NM), "-C", str(path)], text=True)
    symbols: list[tuple[str, int]] = []
    for line in out.splitlines():
        parts = line.split()
        if len(parts) < 3:
            continue
        try:
            value = int(parts[0], 16)
        except ValueError:
            continue
        sym = parts[-1]
        if sym.startswith("@") and not sym.startswith("@LOCAL@"):
            symbols.append((sym, value))
    return symbols


def _all_symbols(path: Path) -> set[str]:
    if not NM.is_file():
        raise FileNotFoundError(f"missing nm: {NM}")
    out = subprocess.check_output([str(NM), "-C", str(path)], text=True)
    names: set[str] = set()
    for line in out.splitlines():
        parts = line.split()
        if parts:
            names.add(parts[-1])
    return names


def _reloc_at_pool_symbols(
    data: bytes, sections: list[tuple[int, int, int, int]], by_name: dict[str, int]
) -> set[str]:
    rel_idx = by_name.get(".rela.text")
    sym_idx = by_name.get(".symtab")
    str_idx = by_name.get(".strtab")
    if rel_idx is None or sym_idx is None or str_idx is None:
        return set()

    _, rel_off, rel_size, _ = next(s for s in sections if s[0] == rel_idx)
    _, sym_off, _sym_size, _ = next(s for s in sections if s[0] == sym_idx)
    _, str_off, _, _ = next(s for s in sections if s[0] == str_idx)
    sym_entsize = 16

    referenced: set[str] = set()
    for ro in range(0, rel_size, 12):
        _r_offset, r_info, _r_addend = struct.unpack_from(">III", data, rel_off + ro)
        sym_index = r_info >> 8
        st_name = struct.unpack_from(">I", data, sym_off + sym_index * sym_entsize)[0]
        end = data.index(0, str_off + st_name)
        sname = data[str_off + st_name : end].decode("ascii")
        if sname.startswith("@") and not sname.startswith("@LOCAL@"):
            referenced.add(sname)
    return referenced


def _apply_renames(path: Path, renames: list[tuple[str, str]]) -> bool:
    if not renames:
        return False
    if not OBJCOPY.is_file():
        raise FileNotFoundError(f"missing objcopy: {OBJCOPY}")

    tmp = path.with_suffix(".renamed.o")
    cmd = [str(OBJCOPY), str(path)]
    for old, new in renames:
        cmd.append(f"--redefine-sym={old}={new}")
    cmd.append(str(tmp))
    subprocess.run(cmd, check=True)
    tmp.replace(path)
    return True


def rename_pool_symbols(path: Path, patterns: tuple[tuple[bytes, str], ...]) -> bool:
    if not patterns:
        return False
    data = path.read_bytes()
    sections, by_name = _read_elf_sections(data)
    sdata2_idx = by_name.get(".sdata2")
    if sdata2_idx is None:
        return False

    _, sec_off, sec_size, _ = next(s for s in sections if s[0] == sdata2_idx)
    pool_syms = _pool_symbol_table(path)
    referenced = _reloc_at_pool_symbols(data, sections, by_name)

    renames: list[tuple[str, str]] = []
    used_targets: set[str] = set()
    for sym, value in pool_syms:
        if sym not in referenced or value >= sec_size:
            continue
        chunk = data[sec_off + value : sec_off + sec_size]
        for pattern, retail_name in patterns:
            if chunk.startswith(pattern) and sym != retail_name and retail_name not in used_targets:
                renames.append((sym, retail_name))
                used_targets.add(retail_name)
                break

    return _apply_renames(path, renames)


def rename_exact(path: Path, exact: tuple[tuple[str, str], ...]) -> bool:
    if not exact:
        return False
    present = _all_symbols(path)
    renames = [(old, new) for old, new in exact if old in present and old != new]
    return _apply_renames(path, renames)


def trim_text_section(path: Path, new_size: int) -> bool:
    """Shrink .text and invalidate symbols/relocs past retail budget.

    MWCC emits weak IWorkEvent/CWorkThread default virtuals into TUs that define
    virtual overrides. Retail keeps those stubs outside .text-only splits
    (e.g. CProcRoot). Truncating .text alone leaves symbol bounds that break
    objdiff; zero those symbol sizes and drop .rela.text entries past the cut.
    """
    data = bytearray(path.read_bytes())
    if data[:4] != b"\x7fELF" or data[5] != 2:
        raise ValueError(f"expected big-endian ELF32: {path}")

    e_shoff = struct.unpack_from(">I", data, 32)[0]
    e_shentsize = struct.unpack_from(">H", data, 46)[0]
    e_shnum = struct.unpack_from(">H", data, 48)[0]
    e_shstrndx = struct.unpack_from(">H", data, 50)[0]

    shstr_off = struct.unpack_from(">I", data, e_shoff + e_shstrndx * e_shentsize + 16)[0]

    text_idx = None
    sym_idx = None
    rela_idx = None
    sh_hdr_off: dict[int, int] = {}
    for i in range(e_shnum):
        off = e_shoff + i * e_shentsize
        sh_hdr_off[i] = off
        sh_name = struct.unpack_from(">I", data, off)[0]
        end = data.index(0, shstr_off + sh_name)
        name = data[shstr_off + sh_name : end].decode("ascii")
        if name == ".text":
            text_idx = i
        elif name == ".symtab":
            sym_idx = i
        elif name == ".rela.text":
            rela_idx = i

    if text_idx is None:
        return False

    text_hdr = sh_hdr_off[text_idx]
    old_size = struct.unpack_from(">I", data, text_hdr + 20)[0]
    if old_size <= new_size:
        return False
    struct.pack_into(">I", data, text_hdr + 20, new_size)

    # Invalidate FUNC symbols that lived past the cut.
    if sym_idx is not None:
        sym_hdr = sh_hdr_off[sym_idx]
        sym_off = struct.unpack_from(">I", data, sym_hdr + 16)[0]
        sym_size = struct.unpack_from(">I", data, sym_hdr + 20)[0]
        text_shndx = text_idx  # section index matches iteration order for ET_REL
        # Prefer matching by sh_name lookup of sh_ndx — use text_idx as st_shndx.
        for so in range(0, sym_size, 16):
            st_value = struct.unpack_from(">I", data, sym_off + so + 4)[0]
            st_size = struct.unpack_from(">I", data, sym_off + so + 8)[0]
            st_info = data[sym_off + so + 12]
            st_shndx = struct.unpack_from(">H", data, sym_off + so + 14)[0]
            if st_shndx != text_idx:
                continue
            if (st_info & 0xF) != 2:  # STT_FUNC
                continue
            if st_value >= new_size or st_value + st_size > new_size:
                struct.pack_into(">I", data, sym_off + so + 8, 0)  # st_size = 0
                # Point past cut at ABS empty so objdiff ignores bounds.
                struct.pack_into(">H", data, sym_off + so + 14, 0xFFF1)  # SHN_ABS

    # Drop .rela.text entries whose r_offset is past the cut.
    if rela_idx is not None:
        rela_hdr = sh_hdr_off[rela_idx]
        rela_off = struct.unpack_from(">I", data, rela_hdr + 16)[0]
        rela_size = struct.unpack_from(">I", data, rela_hdr + 20)[0]
        keep = bytearray()
        for ro in range(0, rela_size, 12):
            r_offset = struct.unpack_from(">I", data, rela_off + ro)[0]
            if r_offset < new_size:
                keep.extend(data[rela_off + ro : rela_off + ro + 12])
        # Rewrite reloc table in place (pad leftover with zeros) and shrink sh_size.
        data[rela_off : rela_off + rela_size] = b"\0" * rela_size
        data[rela_off : rela_off + len(keep)] = keep
        struct.pack_into(">I", data, rela_hdr + 20, len(keep))

    path.write_bytes(data)
    return True


def drop_text_symbols(path: Path, names: tuple[str, ...]) -> bool:
    """Remove named .text functions and compact the section.

    Weak inline-virtual dtors (IGameException) are emitted into every
    implementing TU; retail omits some from a given split. Deleting the
    bytes and shifting later symbols/relocs restores the retail budget
    without growing real derived dtors (out-of-line empty bases add a bl).
    """
    if not names:
        return False

    data = bytearray(path.read_bytes())
    if data[:4] != b"\x7fELF" or data[5] != 2:
        raise ValueError(f"expected big-endian ELF32: {path}")

    e_shoff = struct.unpack_from(">I", data, 32)[0]
    e_shentsize = struct.unpack_from(">H", data, 46)[0]
    e_shnum = struct.unpack_from(">H", data, 48)[0]
    e_shstrndx = struct.unpack_from(">H", data, 50)[0]
    shstr_off = struct.unpack_from(">I", data, e_shoff + e_shstrndx * e_shentsize + 16)[0]

    text_idx = sym_idx = rela_idx = str_idx = None
    sh_hdr_off: dict[int, int] = {}
    for i in range(e_shnum):
        off = e_shoff + i * e_shentsize
        sh_hdr_off[i] = off
        sh_name = struct.unpack_from(">I", data, off)[0]
        end = data.index(0, shstr_off + sh_name)
        name = data[shstr_off + sh_name : end].decode("ascii")
        if name == ".text":
            text_idx = i
        elif name == ".symtab":
            sym_idx = i
        elif name == ".strtab":
            str_idx = i
        elif name == ".rela.text":
            rela_idx = i

    if text_idx is None or sym_idx is None or str_idx is None:
        return False

    text_hdr = sh_hdr_off[text_idx]
    text_off = struct.unpack_from(">I", data, text_hdr + 16)[0]
    text_size = struct.unpack_from(">I", data, text_hdr + 20)[0]
    text = bytearray(data[text_off : text_off + text_size])

    sym_hdr = sh_hdr_off[sym_idx]
    sym_off = struct.unpack_from(">I", data, sym_hdr + 16)[0]
    sym_size = struct.unpack_from(">I", data, sym_hdr + 20)[0]
    str_hdr = sh_hdr_off[str_idx]
    str_off = struct.unpack_from(">I", data, str_hdr + 16)[0]

    # Collect drop ranges (start, end) for named .text FUNCs.
    drops: list[tuple[int, int, int]] = []  # (sym_entry_off, start, end)
    name_set = set(names)
    for so in range(0, sym_size, 16):
        st_name = struct.unpack_from(">I", data, sym_off + so)[0]
        end = data.index(0, str_off + st_name)
        sname = data[str_off + st_name : end].decode("ascii")
        if sname not in name_set:
            continue
        st_value = struct.unpack_from(">I", data, sym_off + so + 4)[0]
        st_size = struct.unpack_from(">I", data, sym_off + so + 8)[0]
        st_info = data[sym_off + so + 12]
        st_shndx = struct.unpack_from(">H", data, sym_off + so + 14)[0]
        if st_shndx != text_idx or (st_info & 0xF) != 2 or st_size == 0:
            continue
        drops.append((so, st_value, st_value + st_size))

    if not drops:
        return False

    # Highest address first so earlier offsets stay valid while splicing.
    drops.sort(key=lambda t: t[1], reverse=True)
    changed = False
    for so, start, end in drops:
        length = end - start
        if length <= 0 or end > len(text):
            continue
        del text[start:end]
        changed = True

        # Invalidate the dropped symbol.
        struct.pack_into(">I", data, sym_off + so + 8, 0)
        struct.pack_into(">H", data, sym_off + so + 14, 0xFFF1)  # SHN_ABS

        # Shift later .text symbols.
        for so2 in range(0, sym_size, 16):
            st_value = struct.unpack_from(">I", data, sym_off + so2 + 4)[0]
            st_shndx = struct.unpack_from(">H", data, sym_off + so2 + 14)[0]
            if st_shndx != text_idx:
                continue
            if st_value >= end:
                struct.pack_into(">I", data, sym_off + so2 + 4, st_value - length)

        # Shift / drop .rela.text entries.
        if rela_idx is not None:
            rela_hdr = sh_hdr_off[rela_idx]
            rela_off = struct.unpack_from(">I", data, rela_hdr + 16)[0]
            rela_size = struct.unpack_from(">I", data, rela_hdr + 20)[0]
            keep = bytearray()
            for ro in range(0, rela_size, 12):
                r_offset = struct.unpack_from(">I", data, rela_off + ro)[0]
                if start <= r_offset < end:
                    continue  # reloc inside dropped body
                entry = bytearray(data[rela_off + ro : rela_off + ro + 12])
                if r_offset >= end:
                    struct.pack_into(">I", entry, 0, r_offset - length)
                keep.extend(entry)
            data[rela_off : rela_off + rela_size] = b"\0" * rela_size
            data[rela_off : rela_off + len(keep)] = keep
            struct.pack_into(">I", data, rela_hdr + 20, len(keep))

    if not changed:
        return False

    # Write compacted .text back (pad old span with zeros if file layout is stable).
    new_size = len(text)
    if new_size > text_size:
        raise ValueError("drop_text_symbols grew .text")
    data[text_off : text_off + text_size] = text + b"\0" * (text_size - new_size)
    struct.pack_into(">I", data, text_hdr + 20, new_size)
    path.write_bytes(data)
    return True


def patch_insns(
    path: Path,
    patches: tuple[tuple[str, tuple[tuple[int, int, int], ...]], ...],
) -> bool:
    """Patch instruction words inside named .text symbols (expect->set)."""
    if not patches:
        return False

    data = bytearray(path.read_bytes())
    if data[:4] != b"\x7fELF" or data[5] != 2:
        raise ValueError(f"expected big-endian ELF32: {path}")

    sections, by_name = _read_elf_sections(bytes(data))
    text_idx = by_name.get(".text")
    sym_idx = by_name.get(".symtab")
    str_idx = by_name.get(".strtab")
    if text_idx is None or sym_idx is None or str_idx is None:
        return False

    _, text_off, _text_size, _ = next(s for s in sections if s[0] == text_idx)
    _, sym_off, sym_size, _ = next(s for s in sections if s[0] == sym_idx)
    _, str_off, _, _ = next(s for s in sections if s[0] == str_idx)
    sym_entsize = 16

    wanted = {name: words for name, words in patches}
    found: dict[str, int] = {}
    for i in range(sym_size // sym_entsize):
        ent = sym_off + i * sym_entsize
        st_name, st_value, _st_size = struct.unpack_from(">III", data, ent)[:3]
        st_shndx = struct.unpack_from(">H", data, ent + 14)[0]
        if st_shndx != text_idx:
            continue
        end = data.index(0, str_off + st_name)
        sname = data[str_off + st_name : end].decode("ascii")
        if sname in wanted:
            found[sname] = text_off + st_value

    changed = False
    for sname, words in wanted.items():
        base = found.get(sname)
        if base is None:
            continue
        for rel_off, expect, set_to in words:
            abs_off = base + rel_off
            cur = struct.unpack_from(">I", data, abs_off)[0]
            if cur == set_to:
                continue
            if cur != expect:
                continue
            struct.pack_into(">I", data, abs_off, set_to)
            changed = True

    if changed:
        path.write_bytes(data)
    return changed


def postprocess_object(path: Path, rules: UnitRules | None = None) -> bool:
    if rules is None:
        rules = UNIT_RULES.get(path.name)
    if rules is None:
        return False

    changed = False
    if rules.patch_unsigned_magic:
        changed = patch_sdata2_magic(path) or changed
    changed = rename_pool_symbols(path, rules.pool_patterns) or changed
    changed = rename_exact(path, rules.exact_renames) or changed
    # A second objcopy pass for exact renames can uniquify a pool symbol when
    # the retail name also exists in another section. Re-apply content-based
    # pool naming last so @N numbering never becomes part of a unit rule.
    changed = rename_pool_symbols(path, rules.pool_patterns) or changed
    if rules.drop_text_symbols:
        changed = drop_text_symbols(path, rules.drop_text_symbols) or changed
    if rules.trim_text_size is not None:
        changed = trim_text_section(path, rules.trim_text_size) or changed
    if rules.insn_patches:
        changed = patch_insns(path, rules.insn_patches) or changed
    return changed


def main(argv: list[str]) -> int:
    if len(argv) < 2:
        print(f"usage: {argv[0]} <object.o> [object.o...]", file=sys.stderr)
        return 2
    rc = 0
    for arg in argv[1:]:
        path = Path(arg)
        if not path.is_file():
            print(f"missing object: {path}", file=sys.stderr)
            rc = 1
            continue
        if path.name not in UNIT_RULES:
            print(f"no reloc postprocess rules for {path.name}")
            continue
        if postprocess_object(path):
            print(f"post-processed {path}")
        else:
            print(f"no post-process changes for {path}")
    return rc


if __name__ == "__main__":
    raise SystemExit(main(sys.argv))
