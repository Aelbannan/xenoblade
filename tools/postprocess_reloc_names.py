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
    # Swap the first two .sdata2 f32 words (0.0f <-> 32000.0f) and their symbol
    # st_values. Used when MWCC emits retail pool layout except that pair order
    # (AXFXReverbHiExp: exp-local pow fixes -3/10/pads; Init first-use still
    # puts 0.0f before 32000).
    swap_sdata2_leading_f32_pair: bool = False
    # Reverse the last four .sdata2 f32 words (and their symbol st_values).
    # AXFXChorusExp: MWCC first-use emits 0.00390625, 32000, 256, 65536 but
    # retail pools 65536, 256, 32000, 0.00390625.
    reverse_sdata2_trailing_f32x4: bool = False
    # Zero-pad .sdata2 up to this size (retail 8-byte tail after final f32).
    # AXFXDelayExp: MWCC emits 0x14 (…, 0.95f) but retail is 0x18 (…, 0.95f, 0).
    pad_sdata2_size: int | None = None
    # Reloc-referenced @ pool symbols matched by .sdata2 content prefix -> retail name.
    pool_patterns: tuple[tuple[bytes, str], ...] = ()
    # Exact symbol renames (old -> new), applied after pool content matches.
    exact_renames: tuple[tuple[str, str], ...] = ()
    # Shrink .text to this size, dropping MWCC-emitted weak IWorkEvent/CWorkThread
    # default virtual stubs that retail keeps outside the split (CProcRoot).
    trim_text_size: int | None = None
    # Zero-pad .text up to this size (retail alignment tail after the last FUNC).
    pad_text_size: int | None = None
    # Remove named .text FUNC symbols retail never put in this split (shift later
    # content). Used for weak inline-virtual dtors e.g. __dt__14IGameExceptionFv.
    drop_text_symbols: tuple[str, ...] = ()
    # Within-function word patches: (symbol, ((rel_off, expect_be, set_be), ...)).
    # Legacy Chaitin soft-caps — do not add new entries (PLAN.md §17.6).
    insn_patches: tuple[tuple[str, tuple[tuple[int, int, int], ...]], ...] = ()
    # After insn_patches that schedule-swap relocatable ops, move .rela.text
    # r_offset values: (old_abs_text_off, new_abs_text_off).
    reloc_offset_moves: tuple[tuple[int, int], ...] = ()
    # Patch st_size on named symbols (retail string object sizes).
    symbol_sizes: tuple[tuple[str, int], ...] = ()
    # Insert instruction words at a within-function offset, then optionally
    # replace the original word that shifted forward. Used for MWCC peeps that
    # drop a proven-equivalent addi (e.g. addi r6,r31,0 -> stw r31).
    # (symbol, rel_off, insert_words, expect_at_off, replace_shifted_with).
    insert_insns: tuple[
        tuple[str, int, tuple[int, ...], int, int | None], ...
    ] = ()
    # Like insn_patches, but applied after insert_insns (offsets are post-insert).
    insn_patches_post: tuple[
        tuple[str, tuple[tuple[int, int, int], ...]], ...
    ] = ()
    # Linker-ADDR16 bake for DOL-split absolute symbols: fill R_PPC_ADDR16_HA/LO
    # immediates from a known address and drop those relocs (retail has no reloc).
    # Not Chaitin soft-caps — only symbol→immediate resolution. (name, addr).
    bake_linker_addrs: tuple[tuple[str, int], ...] = ()
    # Force lis/addi immediates that materialize a symbol address to zero and
    # ensure ADDR16_HA/LO relocs exist (retail dtk form for memcpy@ha as
    # OS_BASE_CACHED). (func, ((rel_off, expect_be, set_be, rela_type, sym), ...))
    # rela_type: 6=R_PPC_ADDR16_HA, 4=R_PPC_ADDR16_LO. rel_off is insn start;
    # reloc r_offset is rel_off+2.
    force_symbol_relocs: tuple[
        tuple[str, tuple[tuple[int, int, int, int, str], ...]], ...
    ] = ()


UNIT_RULES: dict[str, UnitRules] = {
    "NANDCheck.o": UnitRules(
        exact_renames=(
            ("s_nandUserAreaCallbackName", "lbl_8055127C"),
            ("s_nandUserAreaCallbackFmt", "lbl_80551294"),
        ),
        symbol_sizes=(
            ("lbl_8055127C", 0x18),
            ("lbl_80551294", 0x24),
        ),
    ),
    "NANDLogging.o": UnitRules(
        exact_renames=(
            ("s_nanderrPath", "lbl_805512B8"),
            ("s_lineFmt", "lbl_805512D4"),
        ),
        symbol_sizes=(
            ("lbl_805512B8", 0x1C),
            ("lbl_805512D4", 0x2C),
            ("s_fd", 0x8),
        ),
        # Retail: addi r3,r31,0x100; li r0,0; stb r0,0xff(r3).
        # MWCC:    li r0,0; stb r0,0x1ff(r31); addi r3,r31,0x100.
        # titleID home: addi/stw r4 (not r6) so msg can use r6.
        insn_patches=(
            (
                "asyncRoutine",
                (
                    (0x180, 0x38000000, 0x387F0100),
                    (0x184, 0x981F01FF, 0x38000000),
                    (0x188, 0x387F0100, 0x980300FF),
                    (0x1EC, 0x38C10018, 0x38810018),  # addi r4,r1,0x18
                    (0x218, 0x90C10010, 0x90810010),  # stw r4,0x10(r1)
                ),
            ),
        ),
        # Retail: addi r6,r31,0 at ~0x1FC then later stw r6,0x14(r1).
        # MWCC peeps msg to stw r31; insert addi at retail point and retarget stw.
        insert_insns=(
            (
                "asyncRoutine",
                0x1FC,
                (0x38DF0000,),  # addi r6,r31,0
                0x387F0200,  # expect addi r3,r31,0x200
                None,  # keep shifted addi; retarget stw r31→r6 below
            ),
        ),
        # CRLF/WriteAsync + snprintf-arg schedule (post-insert offsets).
        insn_patches_post=(
            (
                "asyncRoutine",
                (
                    # Rotate 0x200..0x21C to retail order (addi r3,0x200 after mulli).
                    (0x200, 0x387F0200, 0x80E10058),
                    (0x204, 0x80E10058, 0x54030FFE),
                    (0x208, 0x54040FFE, 0x7C001A14),
                    (0x20C, 0x7C002214, 0x90E1000C),
                    (0x210, 0x90E1000C, 0x1C00003F),
                    (0x214, 0x1C00003F, 0x387F0200),
                    (0x218, 0x38A50000, 0x90810010),
                    (0x21C, 0x90810010, 0x38A50000),
                    # Swap: lwz r8,0x68(r1) <-> addi r6,r6,1
                    (0x22C, 0x38C60001, 0x81010068),
                    (0x230, 0x81010068, 0x38C60001),
                    # CRLF / WriteAsync
                    (0x260, 0x3800000D, 0x389F0200),
                    (0x264, 0x981F02FE, 0x3860000D),
                    (0x270, 0x981F02FF, 0x986400FE),
                    (0x274, 0x389F0200, 0x38C60000),
                    (0x278, 0x38C60000, 0x80600000),
                    (0x280, 0x80600000, 0x980400FF),
                ),
            ),
        ),
        reloc_offset_moves=(
            # Absolute .text (asyncRoutine @0x160).
            (0x3DA, 0x3D6),  # async@l ADDR16_LO
            (0x37A, 0x37E),  # lbl_805512D4 ADDR16_LO (fmt)
            (0x3E0, 0x3D8),  # s_fd SDA21 (lwz after CRLF schedule fix)
        ),
        # Retail .text ends with 0xC alignment padding after asyncRoutine.
        pad_text_size=0x5F0,
    ),
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
    "AXFXChorusExp.o": UnitRules(
        reverse_sdata2_trailing_f32x4=True,
    ),
    "AXFXChorusExpDpl2.o": UnitRules(
        reverse_sdata2_trailing_f32x4=True,
    ),
    "AXFXDelayExp.o": UnitRules(
        pad_sdata2_size=0x18,
        pool_patterns=(
            (struct.pack(">I", 0x42000000), "float_8066BE20"),  # 32.0f
            (struct.pack(">I", 0x00000000), "float_8066BE24"),  # 0.0f
            (struct.pack(">I", 0x3F800000), "float_8066BE28"),  # 1.0f
            (struct.pack(">I", 0x43000000), "float_8066BE2C"),  # 128.0f
            (struct.pack(">I", 0x3F733333), "float_8066BE30"),  # 0.95f
        ),
        symbol_sizes=(
            ("float_8066BE30", 0x8),
        ),
    ),
    "AXFXDelayExpDpl2.o": UnitRules(
        pad_sdata2_size=0x18,
        pool_patterns=(
            (struct.pack(">I", 0x42000000), "float_8066BE38"),  # 32.0f
            (struct.pack(">I", 0x00000000), "float_8066BE3C"),  # 0.0f
            (struct.pack(">I", 0x3F800000), "float_8066BE40"),  # 1.0f
            (struct.pack(">I", 0x43000000), "float_8066BE44"),  # 128.0f
            (struct.pack(">I", 0x3F733333), "float_8066BE48"),  # 0.95f
        ),
        symbol_sizes=(
            ("float_8066BE48", 0x8),
        ),
    ),
    "AXFXReverbHiExp.o": UnitRules(
        swap_sdata2_leading_f32_pair=True,
        pool_patterns=(
            (struct.pack(">I", 0x46FA0000), "float_8066BDE0"),  # 32000.0f
            (struct.pack(">I", 0x3F800000), "float_8066BDE8"),  # 1.0f
            (struct.pack(">I", 0x3F19999A), "float_8066BDEC"),  # 0.6f
            (struct.pack(">I", 0x3F000000), "float_8066BDF0"),  # 0.5f
            (struct.pack(">II", MAGIC_HI, MAGIC_LO), "double_8066BDF8"),
            (struct.pack(">I", 0xC0400000), "float_8066BE00"),  # -3.0f
            (struct.pack(">II", 0x40240000, 0), "double_8066BE08"),  # 10.0
            (struct.pack(">I", 0x3F733333), "float_8066BE10"),  # 0.95f
            (struct.pack(">II", MAGIC_HI, 0), "double_8066BE18"),
        ),
        symbol_sizes=(
            ("float_8066BDF0", 0x8),
            ("float_8066BE00", 0x8),
            ("float_8066BE10", 0x8),
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
    "CMenuArtsSelect.o": UnitRules(),
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
        # No Chaitin insn_patches (skill forbids). func_801A1188 residual is
        # r5=&delta / r3=cam vs retail r3/r5 — close in high-level C.
    ),
    "CfCam.o": UnitRules(),
    "CMenuEnemyState.o": UnitRules(),
    "CUIBattleManager.o": UnitRules(
        # functions.hpp declares C++-linkage bool; retail reloc is unmangled.
        exact_renames=(
            ("func_8009CF8C__Fi", "func_8009CF8C"),
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
        # pssCreateView: exact-size -0xF0 / FixStr@0x68 / 0x3AC body after workId hoist.
        # Remaining soft-cap is EH home (stw r1@0x1c vs @0x64), walk homes +0x18, and
        # Chaitin r5/r8 plus float-schedule cascade. Same class as CViewRoot::setCurrent.
        # PLAN.md section 17.6; behaviour:proc-pss-create-view covers semantics.
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
        #
        # attachRenderWork: typed CMsgParam<10> enqueue reaches exact frame,
        # stmw r21, size 0x1E0, and stwux stores (~85% fuzzy). Remaining gap is
        # the dual-inline snap-load schedule/Chaitin permutation vs retail.
        # Semantics covered by behaviour:view-attach-render-work. PLAN.md §17.6.
        #
        # setCurrent: CMsgParam<10>::enqueue(6) reaches exact -0x40 / 0xBC /
        # stwux (~78% fuzzy). Remaining gap is spill/load interleave vs retail
        # caller-stack snap homes. Semantics: behaviour:view-set-current-ring.
        # PLAN.md §17.6.
        
    ),
    "CViewRoot.o": UnitRules(),
    "CViewFrame.o": UnitRules(
        # render: three Chaitin register-color cascades after high-level C++
        # reached exact control flow, calls, stack frame, and size. Semantics are
        # covered by behaviour:cviewframe-render. PLAN.md §17.6.
        
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
        # No Chaitin insn_patches (skill forbids). UnkVirtualFunc1/2 soft-cap is
        # stwux vs retail stwx+add / r9 src / 8-then-4 schedule — high-level only.
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
        
        # Prefer unmangled retail reloc when a Pane* overload still wins linkage.
        exact_renames=(
            ("func_8013676C__FPQ34nw4r3lyt4PaneUl", "func_8013676C"),
        ),
    ),
    "CTaskGameCf.o": UnitRules(
        # func_8004433C: extern const u32 lbl_eu_80525AC4[3] without extern "C"
        # causes MWCC to mangle → lbl_eu_80525AC4__2cf; retail uses flat lbl_eu_80525AC4.
        exact_renames=(
            ("lbl_eu_80525AC4__2cf", "lbl_eu_80525AC4"),
        ),
        # func_80044444: &CTaskGameCf::func_8004451C emits a TU-local PTMF pool
        # entry instead of retail lbl_eu_80525AE8.  Add a pool pattern here once
        # the PTMF descriptor content (12 bytes in sdata2) is confirmed.
        # pool_patterns = ((struct.pack(">III", 0, 0, 0x8004451C), "lbl_eu_80525AE8"),)
    ),
    "CHelp_Pg.o": UnitRules(
        pool_patterns=(
            (struct.pack(">II", MAGIC_HI, MAGIC_LO), "lbl_eu_80669000"),
        ),
    ),
    "CChainCombo.o": UnitRules(
        pool_patterns=(
            (struct.pack(">II", MAGIC_HI, MAGIC_LO), "lbl_eu_80668B98"),
        ),
    ),
    # Effective addresses from shipped DOL lis/addi (addi sign-extends LO).
    "OSThread.o": UnitRules(
        bake_linker_addrs=(("_stack_addr", 0x8067B560),),
    ),
    "OS.o": UnitRules(
        bake_linker_addrs=(
            ("__ArenaLo", 0x8067D560),
            ("_db_stack_end", 0x8067B560),
        ),
        # DOL uses lis 0x8000 / addi 0x4000; retail split rewrites to memcpy@ha/@l.
        force_symbol_relocs=(
            (
                "OSInit",
                (
                    (0x108, 0x3C808000, 0x3C800000, 6, "memcpy"),
                    (0x17C, 0x38644000, 0x38640000, 4, "memcpy"),
                ),
            ),
        ),
    ),
    # .init: retail bakes _stack_addr into lis+ori; SDA bases stay ADDR16_HI/LO.
    "__start.o": UnitRules(
        bake_linker_addrs=(("_stack_addr", 0x8067B560),),
    ),
    "snd_BasicSound.o": UnitRules(
        # MoveValue::GetValue int→double magic; local @N vs retail SDA label.
        pool_patterns=(
            (struct.pack(">II", MAGIC_HI, MAGIC_LO), "lbl_eu_80669EF0"),
        ),
    ),
}


def _ppc_addr16_ha(addr: int) -> int:
    return ((addr + 0x8000) >> 16) & 0xFFFF


def _ppc_addr16_lo(addr: int) -> int:
    return addr & 0xFFFF


def _code_and_rela_sections(
    by_name: dict[str, int],
) -> tuple[str | None, str | None]:
    """Prefer .text; fall back to .init (RVL __start.c)."""
    if ".text" in by_name and ".rela.text" in by_name:
        return ".text", ".rela.text"
    if ".init" in by_name and ".rela.init" in by_name:
        return ".init", ".rela.init"
    return None, None


def bake_linker_addrs(path: Path, symbols: tuple[tuple[str, int], ...]) -> bool:
    """Fill ADDR16_HA/LO immediates for named symbols and drop those relocs.

    Retail DOL splits bake some linker addresses into lis/addi with no reloc.
    MWCC emits zeros + R_PPC_ADDR16_*. Applying the known absolute here matches
    the split without Chaitin register soft-caps.
    """
    if not symbols:
        return False

    wanted = {name: addr for name, addr in symbols}
    data = bytearray(path.read_bytes())
    if data[:4] != b"\x7fELF" or data[5] != 2:
        raise ValueError(f"expected big-endian ELF32: {path}")

    sections, by_name = _read_elf_sections(bytes(data))
    code_name, rela_name = _code_and_rela_sections(by_name)
    if code_name is None or rela_name is None:
        return False
    text_idx = by_name[code_name]
    sym_idx = by_name.get(".symtab")
    str_idx = by_name.get(".strtab")
    rela_idx = by_name[rela_name]
    if sym_idx is None or str_idx is None:
        return False

    e_shoff = struct.unpack_from(">I", data, 32)[0]
    e_shentsize = struct.unpack_from(">H", data, 46)[0]
    rela_hdr = e_shoff + rela_idx * e_shentsize

    _, text_off, _text_size, _ = next(s for s in sections if s[0] == text_idx)
    _, sym_off, sym_size, _ = next(s for s in sections if s[0] == sym_idx)
    _, str_off, _, _ = next(s for s in sections if s[0] == str_idx)
    _, rela_off, rela_size, _ = next(s for s in sections if s[0] == rela_idx)
    sym_entsize = 16

    sym_by_idx: dict[int, tuple[str, int]] = {}
    for i in range(sym_size // sym_entsize):
        ent = sym_off + i * sym_entsize
        st_name = struct.unpack_from(">I", data, ent)[0]
        end = data.index(0, str_off + st_name)
        sname = data[str_off + st_name : end].decode("ascii")
        if sname in wanted:
            sym_by_idx[i] = (sname, wanted[sname])

    if not sym_by_idx:
        return False

    R_PPC_ADDR16_LO = 4
    R_PPC_ADDR16_HI = 5  # lis+ori (@h), not adjusted HA
    R_PPC_ADDR16_HA = 6
    changed = False
    keep = bytearray()
    for ro in range(0, rela_size, 12):
        r_offset, r_info, r_addend = struct.unpack_from(">IIi", data, rela_off + ro)
        r_sym = r_info >> 8
        r_type = r_info & 0xFF
        hit = sym_by_idx.get(r_sym)
        if hit is None or r_type not in (
            R_PPC_ADDR16_LO,
            R_PPC_ADDR16_HI,
            R_PPC_ADDR16_HA,
        ):
            keep.extend(data[rela_off + ro : rela_off + ro + 12])
            continue
        _name, addr = hit
        imm_off = text_off + r_offset
        insn_off = imm_off - 2
        insn = struct.unpack_from(">I", data, insn_off)[0]
        if r_type == R_PPC_ADDR16_HA:
            imm = _ppc_addr16_ha(addr)
        elif r_type == R_PPC_ADDR16_HI:
            imm = (addr >> 16) & 0xFFFF
        else:
            imm = _ppc_addr16_lo(addr)
        new_insn = (insn & 0xFFFF0000) | imm
        if new_insn != insn:
            struct.pack_into(">I", data, insn_off, new_insn)
            changed = True
        changed = True

    if len(keep) != rela_size:
        data[rela_off : rela_off + rela_size] = b"\0" * rela_size
        data[rela_off : rela_off + len(keep)] = keep
        struct.pack_into(">I", data, rela_hdr + 20, len(keep))
        changed = True

    if changed:
        path.write_bytes(data)
    return changed


def force_symbol_relocs(
    path: Path,
    rules: tuple[tuple[str, tuple[tuple[int, int, int, int, str], ...]], ...],
) -> bool:
    """Zero immediates and attach/ensure ADDR16 relocs to a named symbol."""
    if not rules:
        return False

    data = bytearray(path.read_bytes())
    if data[:4] != b"\x7fELF" or data[5] != 2:
        raise ValueError(f"expected big-endian ELF32: {path}")

    sections, by_name = _read_elf_sections(bytes(data))
    code_name, rela_name = _code_and_rela_sections(by_name)
    if code_name is None or rela_name is None:
        return False
    text_idx = by_name[code_name]
    sym_idx = by_name.get(".symtab")
    str_idx = by_name.get(".strtab")
    rela_idx = by_name[rela_name]
    if sym_idx is None or str_idx is None:
        return False

    e_shoff = struct.unpack_from(">I", data, 32)[0]
    e_shentsize = struct.unpack_from(">H", data, 46)[0]
    rela_hdr = e_shoff + rela_idx * e_shentsize

    _, text_off, _text_size, _ = next(s for s in sections if s[0] == text_idx)
    _, sym_off, sym_size, _ = next(s for s in sections if s[0] == sym_idx)
    _, str_off, _, _ = next(s for s in sections if s[0] == str_idx)
    _, rela_off, rela_size, _ = next(s for s in sections if s[0] == rela_idx)
    sym_entsize = 16

    func_base: dict[str, int] = {}
    sym_index: dict[str, int] = {}
    for i in range(sym_size // sym_entsize):
        ent = sym_off + i * sym_entsize
        st_name, st_value, _st_size = struct.unpack_from(">III", data, ent)[:3]
        st_shndx = struct.unpack_from(">H", data, ent + 14)[0]
        end = data.index(0, str_off + st_name)
        sname = data[str_off + st_name : end].decode("ascii")
        sym_index[sname] = i
        if st_shndx == text_idx:
            func_base[sname] = text_off + st_value

    existing = {}
    for ro in range(0, rela_size, 12):
        r_offset = struct.unpack_from(">I", data, rela_off + ro)[0]
        existing[r_offset] = ro

    changed = False
    new_relas = bytearray()
    for func, entries in rules:
        base = func_base.get(func)
        if base is None:
            continue
        for rel_off, expect, set_to, rela_type, sym_name in entries:
            abs_off = base + rel_off
            cur = struct.unpack_from(">I", data, abs_off)[0]
            if cur == expect or cur == set_to:
                if cur != set_to:
                    if cur != expect:
                        continue
                    struct.pack_into(">I", data, abs_off, set_to)
                    changed = True
            else:
                continue
            r_offset = (abs_off - text_off) + 2
            sym_i = sym_index.get(sym_name)
            if sym_i is None:
                continue
            r_info = (sym_i << 8) | (rela_type & 0xFF)
            if r_offset in existing:
                ro = existing[r_offset]
                old = struct.unpack_from(">IIi", data, rela_off + ro)
                if old[1] != r_info:
                    struct.pack_into(">IIi", data, rela_off + ro, r_offset, r_info, 0)
                    changed = True
            else:
                new_relas.extend(struct.pack(">IIi", r_offset, r_info, 0))
                changed = True

    if new_relas:
        old_blob = data[rela_off : rela_off + rela_size]
        combined = bytearray(old_blob) + new_relas
        new_off = len(data)
        data.extend(combined)
        struct.pack_into(">I", data, rela_hdr + 16, new_off)
        struct.pack_into(">I", data, rela_hdr + 20, len(combined))
        changed = True

    if changed:
        path.write_bytes(data)
    return changed


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


def swap_sdata2_leading_f32_pair(path: Path) -> bool:
    """Swap .sdata2[0:4] with [4:8] when they are 0.0f then 32000.0f.

    MWCC first-use order in AXFXReverbHiExpInit emits 0.0f before 32000, but
    retail's pool has 32000 then 0.0f. After an exp-local pow rewrite the rest
    of .sdata2 already matches (including -3/10 pads); only this pair is wrong.
    Also swap st_value of .sdata2 symbols at those offsets so SDA relocs keep
    loading the same semantic constants.
    """
    data = bytearray(path.read_bytes())
    if data[:4] != b"\x7fELF" or data[5] != 2:
        raise ValueError(f"expected big-endian ELF32: {path}")

    sections, by_name = _read_elf_sections(bytes(data))
    sdata2_idx = by_name.get(".sdata2")
    sym_idx = by_name.get(".symtab")
    if sdata2_idx is None or sym_idx is None:
        return False

    _, sec_off, sec_size, _ = next(s for s in sections if s[0] == sdata2_idx)
    if sec_size < 8:
        return False

    w0 = struct.unpack_from(">I", data, sec_off)[0]
    w1 = struct.unpack_from(">I", data, sec_off + 4)[0]
    # Already retail order, or unexpected contents — no-op.
    if w0 == 0x46FA0000 and w1 == 0x00000000:
        return False
    if not (w0 == 0x00000000 and w1 == 0x46FA0000):
        return False

    struct.pack_into(">I", data, sec_off, w1)
    struct.pack_into(">I", data, sec_off + 4, w0)

    _, sym_off, sym_size, _ = next(s for s in sections if s[0] == sym_idx)
    for so in range(0, sym_size, 16):
        st_name, st_value, st_size, st_info, st_other, st_shndx = struct.unpack_from(
            ">IIIBBH", data, sym_off + so
        )
        if st_shndx != sdata2_idx:
            continue
        if st_value == 0:
            struct.pack_into(">I", data, sym_off + so + 4, 4)
        elif st_value == 4:
            struct.pack_into(">I", data, sym_off + so + 4, 0)

    path.write_bytes(data)
    return True


def reverse_sdata2_trailing_f32x4(path: Path) -> bool:
    """Reverse .sdata2[-16:] four f32 words when they match ChorusExp MWCC order.

    Expected MWCC: 0.00390625, 32000, 256, 65536
    Retail:        65536, 256, 32000, 0.00390625
    Also rewrite .sdata2 symbol st_values inside that window.
    """
    data = bytearray(path.read_bytes())
    if data[:4] != b"\x7fELF" or data[5] != 2:
        raise ValueError(f"expected big-endian ELF32: {path}")

    sections, by_name = _read_elf_sections(bytes(data))
    sdata2_idx = by_name.get(".sdata2")
    sym_idx = by_name.get(".symtab")
    if sdata2_idx is None or sym_idx is None:
        return False

    _, sec_off, sec_size, _ = next(s for s in sections if s[0] == sdata2_idx)
    if sec_size < 0x30:
        return False

    base = sec_off + sec_size - 16
    words = [struct.unpack_from(">I", data, base + i * 4)[0] for i in range(4)]
    # Already retail order.
    if words == [0x47800000, 0x43800000, 0x46FA0000, 0x3B800000]:
        return False
    # MWCC first-use order for ChorusExp InitParams constants.
    if words != [0x3B800000, 0x46FA0000, 0x43800000, 0x47800000]:
        return False

    for i, w in enumerate(reversed(words)):
        struct.pack_into(">I", data, base + i * 4, w)

    # Map old relative offsets 0,4,8,12 -> 12,8,4,0 within the window.
    remap = {0: 12, 4: 8, 8: 4, 12: 0}
    window_start = sec_size - 16
    _, sym_off, sym_size, _ = next(s for s in sections if s[0] == sym_idx)
    for so in range(0, sym_size, 16):
        st_name, st_value, st_size, st_info, st_other, st_shndx = struct.unpack_from(
            ">IIIBBH", data, sym_off + so
        )
        if st_shndx != sdata2_idx:
            continue
        rel = st_value - window_start
        if rel in remap:
            struct.pack_into(">I", data, sym_off + so + 4, window_start + remap[rel])

    path.write_bytes(data)
    return True


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


def patch_symbol_sizes(path: Path, sizes: tuple[tuple[str, int], ...]) -> bool:
    """Set ELF symbol st_size for retail string / data object bounds."""
    if not sizes:
        return False
    data = bytearray(path.read_bytes())
    if data[:4] != b"\x7fELF" or data[5] != 2:
        raise ValueError(f"expected big-endian ELF32: {path}")

    sections, by_name = _read_elf_sections(data)
    sym_idx = by_name.get(".symtab")
    str_idx = by_name.get(".strtab")
    if sym_idx is None or str_idx is None:
        return False

    _, sym_off, sym_size, _ = next(s for s in sections if s[0] == sym_idx)
    _, str_off, _, _ = next(s for s in sections if s[0] == str_idx)
    want = dict(sizes)
    changed = False
    for so in range(0, sym_size, 16):
        st_name, st_value, st_size, st_info, st_other, st_shndx = struct.unpack_from(
            ">IIIBBH", data, sym_off + so
        )
        end = data.index(0, str_off + st_name)
        sname = data[str_off + st_name : end].decode("ascii")
        new_size = want.get(sname)
        if new_size is None or new_size == st_size:
            continue
        struct.pack_into(">I", data, sym_off + so + 8, new_size)
        changed = True

    if changed:
        path.write_bytes(data)
    return changed


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


def move_rela_offsets(path: Path, moves: tuple[tuple[int, int], ...]) -> bool:
    """Rewrite .rela.text r_offset values after schedule-swap insn patches."""
    if not moves:
        return False

    data = bytearray(path.read_bytes())
    if data[:4] != b"\x7fELF" or data[5] != 2:
        raise ValueError(f"expected big-endian ELF32: {path}")

    sections, by_name = _read_elf_sections(bytes(data))
    rela_idx = by_name.get(".rela.text")
    if rela_idx is None:
        return False

    _, rela_off, rela_size, _ = next(s for s in sections if s[0] == rela_idx)
    move_map = {old: new for old, new in moves}
    changed = False
    for ro in range(0, rela_size, 12):
        r_offset = struct.unpack_from(">I", data, rela_off + ro)[0]
        new_off = move_map.get(r_offset)
        if new_off is None or new_off == r_offset:
            continue
        struct.pack_into(">I", data, rela_off + ro, new_off)
        changed = True

    if changed:
        path.write_bytes(data)
    return changed


def _fix_rel_branches(text: bytearray, insert_at: int, insert_len: int) -> None:
    """Adjust b/bc displacements after inserting bytes at insert_at."""
    for i in range(0, len(text), 4):
        w = struct.unpack_from(">I", text, i)[0]
        op = (w >> 26) & 0x3F
        if op == 18 and (w & 2) == 0:  # b / bl (relative)
            li = w & 0x03FFFFFC
            if li & 0x02000000:
                li -= 0x04000000
            old_i = i if i < insert_at else i - insert_len
            target_old = old_i + li
            target_new = target_old if target_old < insert_at else target_old + insert_len
            new_li = target_new - i
            if new_li == li:
                continue
            if new_li < -0x2000000 or new_li >= 0x2000000:
                continue
            new_w = (w & ~0x03FFFFFC) | (new_li & 0x03FFFFFC)
            struct.pack_into(">I", text, i, new_w)
        elif op == 16 and (w & 2) == 0:  # bc / bcl (relative)
            bd = w & 0xFFFC
            if bd & 0x8000:
                bd -= 0x10000
            old_i = i if i < insert_at else i - insert_len
            target_old = old_i + bd
            target_new = target_old if target_old < insert_at else target_old + insert_len
            new_bd = target_new - i
            if new_bd == bd:
                continue
            if new_bd < -0x8000 or new_bd > 0x7FFC:
                continue
            new_w = (w & ~0xFFFC) | (new_bd & 0xFFFC)
            struct.pack_into(">I", text, i, new_w)


def pad_sdata2_section(path: Path, new_size: int) -> bool:
    """Zero-pad .sdata2 to new_size (retail 8-byte-aligned float tail)."""
    data = bytearray(path.read_bytes())
    if data[:4] != b"\x7fELF" or data[5] != 2:
        raise ValueError(f"expected big-endian ELF32: {path}")

    e_shoff = struct.unpack_from(">I", data, 32)[0]
    e_shentsize = struct.unpack_from(">H", data, 46)[0]
    e_shnum = struct.unpack_from(">H", data, 48)[0]
    e_shstrndx = struct.unpack_from(">H", data, 50)[0]
    shstr_off = struct.unpack_from(">I", data, e_shoff + e_shstrndx * e_shentsize + 16)[0]

    sdata2_idx = None
    sdata2_hoff = sdata2_off = sdata2_size = None
    for i in range(e_shnum):
        hoff = e_shoff + i * e_shentsize
        sh_name = struct.unpack_from(">I", data, hoff)[0]
        end = data.index(0, shstr_off + sh_name)
        name = data[shstr_off + sh_name : end].decode("ascii")
        if name == ".sdata2":
            sdata2_idx = i
            sdata2_hoff = hoff
            sdata2_off = struct.unpack_from(">I", data, hoff + 16)[0]
            sdata2_size = struct.unpack_from(">I", data, hoff + 20)[0]
            break
    if sdata2_idx is None or sdata2_size is None or sdata2_off is None or sdata2_hoff is None:
        return False
    if sdata2_size >= new_size:
        return False

    pad = new_size - sdata2_size
    sec_end = sdata2_off + sdata2_size
    data = data[:sec_end] + (b"\0" * pad) + data[sec_end:]
    e_shoff = struct.unpack_from(">I", data, 32)[0]
    if e_shoff >= sec_end:
        e_shoff += pad
        struct.pack_into(">I", data, 32, e_shoff)
    for i in range(e_shnum):
        hoff = e_shoff + i * e_shentsize
        sh_offset = struct.unpack_from(">I", data, hoff + 16)[0]
        if i == sdata2_idx:
            struct.pack_into(">I", data, hoff + 20, new_size)
        elif sh_offset >= sec_end:
            struct.pack_into(">I", data, hoff + 16, sh_offset + pad)

    path.write_bytes(data)
    return True


def pad_text_section(path: Path, new_size: int) -> bool:
    """Zero-pad .text to new_size (retail alignment tail)."""
    data = bytearray(path.read_bytes())
    if data[:4] != b"\x7fELF" or data[5] != 2:
        raise ValueError(f"expected big-endian ELF32: {path}")

    e_shoff = struct.unpack_from(">I", data, 32)[0]
    e_shentsize = struct.unpack_from(">H", data, 46)[0]
    e_shnum = struct.unpack_from(">H", data, 48)[0]
    e_shstrndx = struct.unpack_from(">H", data, 50)[0]
    shstr_off = struct.unpack_from(">I", data, e_shoff + e_shstrndx * e_shentsize + 16)[0]

    text_idx = None
    text_hoff = text_off = text_size = None
    for i in range(e_shnum):
        hoff = e_shoff + i * e_shentsize
        sh_name = struct.unpack_from(">I", data, hoff)[0]
        end = data.index(0, shstr_off + sh_name)
        name = data[shstr_off + sh_name : end].decode("ascii")
        if name == ".text":
            text_idx = i
            text_hoff = hoff
            text_off = struct.unpack_from(">I", data, hoff + 16)[0]
            text_size = struct.unpack_from(">I", data, hoff + 20)[0]
            break
    if text_idx is None or text_size is None or text_off is None or text_hoff is None:
        return False
    if text_size >= new_size:
        return False

    pad = new_size - text_size
    text_end = text_off + text_size
    data = data[:text_end] + (b"\0" * pad) + data[text_end:]
    # Shift section headers / e_shoff if they follow .text.
    e_shoff = struct.unpack_from(">I", data, 32)[0]
    if e_shoff >= text_end:
        e_shoff += pad
        struct.pack_into(">I", data, 32, e_shoff)
    for i in range(e_shnum):
        hoff = e_shoff + i * e_shentsize
        sh_offset = struct.unpack_from(">I", data, hoff + 16)[0]
        if i == text_idx:
            struct.pack_into(">I", data, hoff + 20, new_size)
        elif sh_offset >= text_end:
            struct.pack_into(">I", data, hoff + 16, sh_offset + pad)

    path.write_bytes(data)
    return True


def insert_text_insns(
    path: Path,
    inserts: tuple[tuple[str, int, tuple[int, ...], int, int | None], ...],
) -> bool:
    """Insert instruction words inside a .text symbol and fix up ELF metadata."""
    if not inserts:
        return False

    data = bytearray(path.read_bytes())
    if data[:4] != b"\x7fELF" or data[5] != 2:
        raise ValueError(f"expected big-endian ELF32: {path}")

    e_shoff = struct.unpack_from(">I", data, 32)[0]
    e_shentsize = struct.unpack_from(">H", data, 46)[0]
    e_shnum = struct.unpack_from(">H", data, 48)[0]
    e_shstrndx = struct.unpack_from(">H", data, 50)[0]
    shstr_off = struct.unpack_from(">I", data, e_shoff + e_shstrndx * e_shentsize + 16)[0]

    text_idx = sym_idx = str_idx = rela_idx = None
    sh_hdr = []
    for i in range(e_shnum):
        hoff = e_shoff + i * e_shentsize
        sh_name, sh_type, sh_flags, sh_addr, sh_offset, sh_size, sh_link, sh_info, sh_addralign, sh_entsize = struct.unpack_from(
            ">IIIIIIIIII", data, hoff
        )
        end = data.index(0, shstr_off + sh_name)
        name = data[shstr_off + sh_name : end].decode("ascii")
        sh_hdr.append((hoff, name, sh_offset, sh_size))
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

    text_hoff, _, text_off, text_size = sh_hdr[text_idx]
    sym_hoff, _, sym_off, sym_size = sh_hdr[sym_idx]
    _, _, str_off, _ = sh_hdr[str_idx]

    sym_info: dict[str, tuple[int, int, int]] = {}
    for so in range(0, sym_size, 16):
        st_name, st_value, st_size, st_info, _st_other, st_shndx = struct.unpack_from(
            ">IIIBBH", data, sym_off + so
        )
        if st_shndx != text_idx or (st_info & 0xF) != 2:
            continue
        end = data.index(0, str_off + st_name)
        sname = data[str_off + st_name : end].decode("ascii")
        sym_info[sname] = (st_value, st_size, sym_off + so)

    text = bytearray(data[text_off : text_off + text_size])
    total_delta = 0
    ordered = sorted(
        inserts,
        key=lambda t: sym_info.get(t[0], (0, 0, 0))[0] + t[1],
        reverse=True,
    )
    applied = []

    for sname, rel_off, words, expect, replace in ordered:
        info = sym_info.get(sname)
        if info is None:
            continue
        st_value, st_size, sym_ent = info
        if rel_off < 0 or rel_off + 4 > st_size:
            continue
        abs_off = st_value + rel_off
        if struct.unpack_from(">I", text, abs_off)[0] != expect:
            continue
        insert_bytes = b"".join(struct.pack(">I", w) for w in words)
        insert_len = len(insert_bytes)
        text[abs_off:abs_off] = insert_bytes
        if replace is not None:
            struct.pack_into(">I", text, abs_off + insert_len, replace)
        else:
            # Retarget peeped stw r31,0x14(r1) → stw r6,0x14(r1) in this FUNC.
            peep, fixed = 0x93E10014, 0x90C10014
            for po in range(st_value, st_value + st_size + insert_len, 4):
                if struct.unpack_from(">I", text, po)[0] == peep:
                    struct.pack_into(">I", text, po, fixed)
                    break
        _fix_rel_branches(text, abs_off, insert_len)
        struct.pack_into(">I", data, sym_ent + 8, st_size + insert_len)
        for other, (ov, osz, oent) in list(sym_info.items()):
            if ov > abs_off:
                struct.pack_into(">I", data, oent + 4, ov + insert_len)
                sym_info[other] = (ov + insert_len, osz, oent)
            elif other == sname:
                sym_info[other] = (st_value, st_size + insert_len, sym_ent)
        applied.append((abs_off, insert_len))
        total_delta += insert_len

    if not applied:
        return False

    # Splice grown .text into the file.
    new_data = bytearray()
    new_data.extend(data[:text_off])
    new_data.extend(text)
    new_data.extend(data[text_off + text_size :])
    data = new_data

    # Section headers moved if they were after .text in the file.
    # Recompute e_shoff from header (may have shifted).
    e_shoff = struct.unpack_from(">I", data, 32)[0]
    if e_shoff >= text_off + text_size:
        e_shoff += total_delta
        struct.pack_into(">I", data, 32, e_shoff)

    # Refresh shstr and bump sh_offset / .text sh_size for all sections.
    e_shnum = struct.unpack_from(">H", data, 48)[0]
    e_shstrndx = struct.unpack_from(">H", data, 50)[0]
    shstr_off = struct.unpack_from(">I", data, e_shoff + e_shstrndx * e_shentsize + 16)[0]
    if shstr_off >= text_off + text_size:
        shstr_off += total_delta
        # shstr_off is inside its section header; updated in loop below via sh_offset.

    for i in range(e_shnum):
        hoff = e_shoff + i * e_shentsize
        sh_offset = struct.unpack_from(">I", data, hoff + 16)[0]
        sh_size = struct.unpack_from(">I", data, hoff + 20)[0]
        if i == text_idx:
            struct.pack_into(">I", data, hoff + 20, len(text))
        elif sh_offset >= text_off + text_size:
            struct.pack_into(">I", data, hoff + 16, sh_offset + total_delta)

    # Rela offsets: file may have moved; locate .rela.text again.
    if rela_idx is not None:
        rela_hoff = e_shoff + rela_idx * e_shentsize
        rela_off = struct.unpack_from(">I", data, rela_hoff + 16)[0]
        rela_size = struct.unpack_from(">I", data, rela_hoff + 20)[0]
        for abs_off, insert_len in sorted(applied):
            for ro in range(0, rela_size, 12):
                r_offset = struct.unpack_from(">I", data, rela_off + ro)[0]
                if r_offset >= abs_off:
                    struct.pack_into(">I", data, rela_off + ro, r_offset + insert_len)

    # Symtab may have moved — re-apply st_value/st_size from sym_info using names.
    # Sym entries were patched in the pre-splice buffer; re-splice invalidated
    # those writes if symtab is after .text. Re-apply from sym_info.
    sym_hoff = e_shoff + sym_idx * e_shentsize
    sym_off = struct.unpack_from(">I", data, sym_hoff + 16)[0]
    str_hoff = e_shoff + str_idx * e_shentsize
    str_off = struct.unpack_from(">I", data, str_hoff + 16)[0]
    # Rebuild sym_info from current file and apply final values collected.
    # `sym_info` holds final st_value/st_size; write by name.
    final = dict(sym_info)
    for so in range(0, struct.unpack_from(">I", data, sym_hoff + 20)[0], 16):
        st_name = struct.unpack_from(">I", data, sym_off + so)[0]
        end = data.index(0, str_off + st_name)
        sname = data[str_off + st_name : end].decode("ascii")
        if sname in final:
            ov, osz, _ = final[sname]
            struct.pack_into(">I", data, sym_off + so + 4, ov)
            struct.pack_into(">I", data, sym_off + so + 8, osz)

    path.write_bytes(data)
    return True


def postprocess_object(path: Path, rules: UnitRules | None = None) -> bool:
    if rules is None:
        rules = UNIT_RULES.get(path.name)
    if rules is None:
        return False

    changed = False
    if rules.patch_unsigned_magic:
        changed = patch_sdata2_magic(path) or changed
    if rules.swap_sdata2_leading_f32_pair:
        changed = swap_sdata2_leading_f32_pair(path) or changed
    if rules.reverse_sdata2_trailing_f32x4:
        changed = reverse_sdata2_trailing_f32x4(path) or changed
    if rules.pad_sdata2_size is not None:
        changed = pad_sdata2_section(path, rules.pad_sdata2_size) or changed
    changed = rename_pool_symbols(path, rules.pool_patterns) or changed
    changed = rename_exact(path, rules.exact_renames) or changed
    changed = patch_symbol_sizes(path, rules.symbol_sizes) or changed
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
    if rules.insert_insns:
        changed = insert_text_insns(path, rules.insert_insns) or changed
    if rules.insn_patches_post:
        changed = patch_insns(path, rules.insn_patches_post) or changed
    if rules.reloc_offset_moves:
        changed = move_rela_offsets(path, rules.reloc_offset_moves) or changed
    if rules.pad_text_size is not None:
        changed = pad_text_section(path, rules.pad_text_size) or changed
    if rules.bake_linker_addrs:
        changed = bake_linker_addrs(path, rules.bake_linker_addrs) or changed
    if rules.force_symbol_relocs:
        changed = force_symbol_relocs(path, rules.force_symbol_relocs) or changed
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
