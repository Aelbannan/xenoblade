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
    # Shrink .sdata2 to this size, dropping trailing pool entries that no
    # surviving relocation references (retail linker GC'd the weak constant).
    # e_pow: MWCC pools an extra orphaned 1.0 after two53 (0x110) that retail
    # never references (+8 over the 0x110 split slice).
    trim_sdata2_size: int | None = None
    # Drop the trailing tail of a data section ((section, keep_size)): weak
    # base-class typeinfo names/structs the retail linker GC'd. Surviving
    # relocs must be retargeted first (retarget_relocs) so live references
    # resolve to the strong copy in the retail-correct TU at link.
    drop_data_tail: tuple[tuple[str, int], ...] = ()
    # Retarget the reloc at (section, offset) to an UNDEF symbol (retail name):
    # ((section, offset, symbol_name), ...). Runs before drop_data_tail.
    retarget_relocs: tuple[tuple[str, int, str], ...] = ()
    # Point a reloc at a LOCAL symbol in another section (section, offset,
    # target_section, target_offset): ((str, int, str, int), ...).
    retarget_relocs_local: tuple[tuple[str, int, str, int], ...] = ()
    # Zero a data-section range ((section, start, end)) where MWCC placed a
    # weak typeinfo name inside what is zero padding in retail; the range's
    # relocs are dropped. Runs before drop_data_tail.
    zero_data_range: tuple[tuple[str, int, int], ...] = ()
    # Remove a data-section range ((section, start, end)), shifting later
    # symbols/relocs: weak base vtables/typeinfo the retail linker GC'd that
    # sit mid-section. Runs before drop_data_tail.
    drop_data_range: tuple[tuple[str, int, int], ...] = ()
    # Reloc-referenced @ pool symbols matched by .sdata2 content prefix -> retail name.
    pool_patterns: tuple[tuple[bytes, str], ...] = ()
    # Exact symbol renames (old -> new), applied after pool content matches.
    exact_renames: tuple[tuple[str, str], ...] = ()
    # Prefix renames: first symbol whose name starts with old_prefix -> new.
    # For MWCC static local numbering that drifts ($8802 vs $8817).
    prefix_renames: tuple[tuple[str, str], ...] = ()
    # Shrink .text to this size, dropping MWCC-emitted weak IWorkEvent/CWorkThread
    # default virtual stubs that retail keeps outside the split (CProcRoot).
    trim_text_size: int | None = None
    # Zero-pad .text up to this size (retail alignment tail after the last FUNC).
    pad_text_size: int | None = None
    # Remove named .text FUNC symbols retail never put in this split (shift later
    # content). Used for weak inline-virtual dtors e.g. __dt__14IGameExceptionFv.
    drop_text_symbols: tuple[str, ...] = ()
    # Like drop_text_symbols, but the dropped symbol is converted to UNDEF
    # (shndx=0) instead of SHN_ABS so surviving .data/.extabindex relocations
    # resolve to the strong copy in another TU at link (the retail linker's
    # GC keeps the strong definition; the DOL-extracted retail .o shows the
    # ref as UNDEF). Use when the dropped weak has a LIVE .data reference
    # (e.g. __dt__FontFv referenced by the Font vtable in ut_ResFontBase).
    drop_text_symbols_as_undef: tuple[str, ...] = ()
    # After drop_text_symbols, re-pack surviving .text FUNCs at this alignment
    # (e.g. 16). The drop shift preserves MWCC's pre-drop padding residue, while
    # the retail linker's GC re-lays survivors at their natural alignment
    # (lyt_group: weak template-dtor orphans in the middle leave a +0xC pad
    # that otherwise overshoots the split budget). 0/None = disabled.
    repack_after_drop: int = 0
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
    "CTaskManager.o": UnitRules(
        # Retail .data holds ONLY the CRootProc vtable (0x24, typeinfo ->
        # external __RTTI__/CProcess). MWCC also emits the CTTask<CRootProc>
        # base vtable (0x20) the retail linker GC'd (+0x24 .data over the
        # 0x24 slice); it is unreferenced (the derived vtable covers all
        # slots). Drop the mid-section base vtable.
        drop_data_range=((".data", 0x24, 0x48),),
    ),

    "MWRTTI.o": UnitRules(
        # The bad_cast typeinfo (.sdata `ta`) base ptr must reference the
        # bad_cast typeinfo struct in .data (+0x10); MWCC pointed it at a weak
        # local std::exception typeinfo (name in .rodata 0x10, structs in
        # .sdata/.sdata2) the retail linker GC'd (retail refs __RTTI__Q23std9
        # exception externally from New.o). Retarget the base ptr to the .data
        # struct, drop the weak name/structs, and zero the .sdata2.
        retarget_relocs_local=(
            (".sdata", 0xC, ".data", 0x10),
        ),
        drop_data_range=(
            (".rodata", 0x10, 0x20),
            (".sdata", 0x0, 0x8),
            (".sdata", 0x8, 0x10),
        ),
        drop_data_tail=((".sdata2", 0x0),),
    ),
    "lyt_picture.o": UnitRules(
        # Retail keeps Picture's typeinfo chain local but references the
        # Pane/PaneBase typeinfo NAMES externally (__RTTI__Q36nw4hbm3lyt4Pane
        # / __RTTI__Q46nw4hbm3lyt6detail8PaneBase, defined in the retail
        # lyt_pane.o which links as NonMatching). MWCC emits weak local copies
        # (+0x50 .data over the 0xA0 slice); retarget the surviving struct
        # name/base ptrs to the retail symbols and drop the weak tail.
        retarget_relocs=(
            (".data", 0x84, "__RTTI__Q36nw4hbm3lyt4Pane"),
            (".data", 0x8C, "__RTTI__Q46nw4hbm3lyt6detail8PaneBase"),
        ),
        drop_data_tail=((".data", 0xA0),),
    ),
    "lyt_resourceAccessor.o": UnitRules(
        # ResourceAccessor vtable typeinfo ptr (+0x0) references the base RTTI
        # externally in retail (__RTTI__Q36nw4hbm3lyt16ResourceAccessor, strong
        # copy in the lyt_arcResourceAccessor TU); MWCC emits a weak local copy
        # (+0x28 .data over the 0x18 slice). Retarget + drop the tail.
        retarget_relocs=(
            (".data", 0x0, "__RTTI__Q36nw4hbm3lyt16ResourceAccessor"),
        ),
        # The name string started at +0x14 inside the kept 0x18 (retail's
        # vtable is 6 words with a trailing NULL); zero the stub.
        zero_data_range=((".data", 0x14, 0x18),),
        drop_data_tail=((".data", 0x18),),
    ),
    "e_pow.o": UnitRules(
        # MWCC pools an orphaned 1.0 after two53 (0x110) the final code never
        # references (no .rela.sdata2 entry touches it); the retail linker GC'd
        # it. Trimming restores the 0x110 retail slice (+8 over before).
        trim_sdata2_size=0x110,
    ),
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
            ("lbl_805512B8", 0x1B),
            ("lbl_805512D4", 0x27),
            ("s_fd", 0x8),
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
    "buffer_io.o": UnitRules(
        # Retail dead-strips these empty/unused stubs; MWCC still emits them.
        drop_text_symbols=(
            "__convert_from_newlines",
            "__convert_to_newlines",
            "__load_buffer",
            "setvbuf",
            "setbuf",
        ),
    ),
    "msgbuf.o": UnitRules(
        # Retail dead-strips these unreferenced API wrappers; MWCC still emits
        # them. Drop them so decomp .text fits the retail split budget.
        drop_text_symbols=(
            "TRK_SetBufferUsed",
            "TRKAppendBuffer1_ui16",
            "TRKAppendBuffer1_ui128",
            "TRKAppendBuffer_ui16",
            "TRKAppendBuffer_ui64",
            "TRKAppendBuffer_ui128",
            "TRKReadBuffer1_ui8",
            "TRKReadBuffer1_ui16",
            "TRKReadBuffer1_ui32",
            "TRKReadBuffer1_ui128",
            "TRKReadBuffer_ui16",
            "TRKReadBuffer_ui64",
            "TRKReadBuffer_ui128",
        ),
    ),
    "CVec3.o": UnitRules(
        # __sinit_ loads the CVec3::zero base address via a section-relative
        # reloc (...bss.0); retail references the named symbol instead.
        exact_renames=(("...bss.0", "zero__Q22ml5CVec3"),),
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
    # FORCEACTIVE packs @stringBase0 (Restart,"",43,arc,brlyt) so "CGame" is +0x23;
    # drop the 0x1C thunk afterward — stubs already fill the split to 0xD08.
    "CGame.o": UnitRules(
        drop_text_symbols=("FORCEACTIVECGame_cpp_wkStandbyLogin__Fv",),
    ),
    # MWCC switch cookies for func_8003B6A0 / func_8003B800.
    # VM wrappers: retail asm keeps address-suffixed names; MWCC emits short names.
    "ocBdat.o": UnitRules(
        exact_renames=(
            # MWCC cookie ids drift with TU growth; size-0x24 switch tables.
            ("@1095", "jumptable_eu_80524D90"),
            ("@1252", "jumptable_eu_80524DB8"),
            ("bdat", "bdat_8003BD4C"),
            ("getVal", "getVal_8003BDB8"),
            ("getArrayVal", "getArrayVal_8003BE70"),
            ("getArrayCount", "getArrayCount_8003BF48"),
            ("getVarType", "getVarType_8003BFB8"),
            ("getIdCount", "getIdCount_8003C028"),
            ("getIdTop", "getIdTop_8003C074"),
            ("getFlagVal", "getFlagVal_8003C0C0"),
        ),
    ),
    "CfCollSphereImpl.o": UnitRules(
        pool_patterns=(
            (struct.pack(">II", MAGIC_HI, 0), "lbl_eu_80666920"),
        ),
    ),
    "code_802B9064.o": UnitRules(
        pool_patterns=(
            (struct.pack(">I", 0x3F800000), "float_8066B818"),
            (struct.pack(">I", 0x3F000000), "float_8066B81C"),
        ),
    ),
    "CChainTime.o": UnitRules(
        exact_renames=(
            ("__vt__Q22cf10CChainTime", "lbl_eu_8052BD74"),
        ),
        pool_patterns=(
            (struct.pack(">I", 0x00000000), "lbl_eu_80668A88"),
            (struct.pack(">I", 0xBF800000), "lbl_eu_80668A8C"),
        ),
    ),
    "CArtsInfo.o": UnitRules(
        exact_renames=(
            # Rename C++ mangled func_* symbols to their unmangled retail names
            ("func_80235814__FP9CArtsInfo", "func_80235814"),
            ("func_8023587C__FP9CArtsInfo", "func_8023587C"),
            ("func_80235958__FP9CArtsInfoPv", "func_80235958"),
            ("func_802359CC__FP9CArtsInfo", "func_802359CC"),
            ("func_80235A90__FP9CArtsInfo", "func_80235A90"),
            ("func_80235A98__FP9CArtsInfo", "func_80235A98"),
            ("func_80235AA0__FP9CArtsInfo", "func_80235AA0"),
            ("func_80235AC0__FP9CArtsInfo", "func_80235AC0"),
            ("func_80235AE0__FP9CArtsInfo", "func_80235AE0"),
            ("func_80235D24__FP9CArtsInfo", "func_80235D24"),
            ("func_80235DD8__FP9CArtsInfo", "func_80235DD8"),
            ("func_80235E84__FP9CArtsInfoUc", "func_80235E84"),
            ("func_80235E8C__FP9CArtsInfoUc", "func_80235E8C"),
            ("func_80235E94__FP9CArtsInfoUc", "func_80235E94"),
            ("func_80235E9C__FP9CArtsInfoUs", "func_80235E9C"),
            ("func_80235EA4__FP9CArtsInfo", "func_80235EA4"),
            ("func_80235EF0__FP9CArtsInfo", "func_80235EF0"),
            ("func_80235F14__FP9CArtsInfo", "func_80235F14"),
            ("func_80235F3C__FP9CArtsInfo", "func_80235F3C"),
            ("func_80235F50__FP9CArtsInfo", "func_80235F50"),
            ("func_80235F6C__FP9CArtsInfo", "func_80235F6C"),
            ("func_80236020__FP9CArtsInfo", "func_80236020"),
            ("func_8023606C__FP9CArtsInfo", "func_8023606C"),
            ("func_80236120__FP9CArtsInfo", "func_80236120"),
            ("func_8023616C__FP9CArtsInfo", "func_8023616C"),
            ("func_80236220__FP9CArtsInfo", "func_80236220"),
            ("func_802362D4__FP9CArtsInfo", "func_802362D4"),
            ("func_80236334__FP9CArtsInfo", "func_80236334"),
            ("func_80236408__FP9CArtsInfo", "func_80236408"),
            ("func_80236454__FP9CArtsInfo", "func_80236454"),
            ("func_80236508__FP9CArtsInfo", "func_80236508"),
        ),
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
    "MemManager.o": UnitRules(
        # MWCC emits these inline/accessor and ABI-wrapper bodies although the
        # retail MemManager split does not contain them. They are unreferenced
        # by the retained split functions.
        exact_renames=(
            ("lbl_eu_80653EE0", "sRegionBuffer__Q23mtl10MemManager"),
        ),
        drop_text_symbols=(
            "getRegion__Q23mtl10MemManagerFUl",
            "getMaxBlock__Q23mtl10MemManagerFUl",
            "empty__Q23mtl10MemManagerFUl",
            "getMEM1MaxSize__Q33mtl10MemManager9MemRegionFv",
            "getMEM2MaxSize__Q33mtl10MemManager9MemRegionFv",
            "isOptimalAlloc__Q23mtl10MemManagerFv",
            "setHandleMEM1__Q23mtl10MemManagerFUl",
            "setHandleMEM2__Q23mtl10MemManagerFUl",
            "setHandleStatic__Q23mtl10MemManagerFUl",
            "__dt__Q33mtl10MemManager9MemRegionFv__Fv",
            "__dl__FPv__Fv",
            "__dla__FPv__Fv",
        ),
    ),
    "CViewRoot.o": UnitRules(),
    "CWorkThread.o": UnitRules(
        # Retail strips this TU's class-static/vtable names to address labels.
        exact_renames=(
            ("__vt__11CWorkThread", "lbl_eu_8056B110"),
            ("__vt__29_reslist_base<P11CWorkThread>", "lbl_eu_8056B1D4"),
            ("__vt__23reslist<P11CWorkThread>", "lbl_eu_8056B1BC"),
            ("__vt__12CMsgParam<8>", "lbl_eu_8056B1B0"),
            ("sAllocHandle__17CWorkThreadSystem", "lbl_eu_8066351C"),
        ),
    ),
    "CWorkThreadSystem.o": UnitRules(
        # Retail names this TU's static storage by stripped address labels.
        # The MWCC class-static names resolve to the same objects/addresses.
        exact_renames=(
            ("scRegionName__17CWorkThreadSystem", "lbl_eu_80663518"),
            ("sAllocHandle__17CWorkThreadSystem", "lbl_eu_8066351C"),
            ("sMemAvailable__17CWorkThreadSystem", "lbl_eu_80665590"),
            ("sAllocFlags__17CWorkThreadSystem", "lbl_eu_80665594"),
            ("sWorkThreads__17CWorkThreadSystem", "lbl_eu_80665598"),
        ),
    ),
    "CViewFrame.o": UnitRules(
        # render: three Chaitin register-color cascades after high-level C++
        # reached exact control flow, calls, stack frame, and size. Semantics are
        # covered by behaviour:cviewframe-render. PLAN.md §17.6.
        
    ),
    "CProcRoot.o": UnitRules(
        # Retail .text-only split is 0x1C8; drop MWCC weak default-virtual stubs.
        trim_text_size=0x1C8,
    ),
    "CRsrcData.o": UnitRules(
        # Retail .text ends after wkStandbyLogout (0x42C); drop weak IWorkEvent/CWorkThread stubs.
        trim_text_size=0x42C,
        exact_renames=(
            ("__ct__9CRsrcDataFPCcP11CWorkThread", "__ct__CRsrcData"),
            ("__vt__9CRsrcData", "lbl_eu_8056B360"),
        ),
    ),
    "gki_time.o": UnitRules(
        # Retail trailing align pad after GKI_remove_from_timer_list (0xC).
        pad_text_size=0x590,
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
    "lyt_texMap.o": UnitRules(
        # TexMap::Get(GXTexObj*): MWCC @N pools vs retail SDA labels.
        # 1/256f (@5590), signed int→double magic (@5594), 256.0f (@5623).
        pool_patterns=(
            (struct.pack(">I", 0x3B800000), "lbl_eu_80669DB8"),
            (struct.pack(">II", MAGIC_HI, 0), "lbl_eu_80669DC0"),
            (struct.pack(">I", 0x43800000), "lbl_eu_80669DC8"),
        ),
    ),
    "lyt_animation.o": UnitRules(
        # MWCC emits the weak inline-virtual base dtor
        # __dt__Q36nw4hbm3lyt13AnimTransformFv (0x40 deleting wrapper) in this
        # TU with the AnimTransformBasic vtable; the retail linker dead-stripped
        # it (no __dt__AnimTransform anywhere in the DOL; derived dtor elides
        # the base call since the base dtor is inline-empty). Dropping the
        # orphan restores the retail split layout and fits the 0xBB0 budget.
        drop_text_symbols=("__dt__Q36nw4hbm3lyt13AnimTransformFv",),
        # The AnimTransform base vtable (0x20) + its RTTI struct sit between
        # AnimTransformBasic's typeinfo and name (+0x20 .data over the 0x80
        # slice); retail references AnimTransform's RTTI externally. Drop the
        # mid-section vtable; the RTTI struct's live refs (reloc at +0x40)
        # keep resolving to the shifted struct.
        drop_data_range=((".data", 0x58, 0x78),),
    ),
    "lyt_layout.o": UnitRules(
        # MWCC emits unreferenced weak orphans the retail linker GC'd: the
        # inline-virtual base dtor __dt__AnimTransformFv (0x40), the implicit
        # LinkList<AnimTransform,4>/LinkList<Group,4> template-dtor wrappers
        # (0x58 each, same pattern as lyt_group), and the inline RTTI accessor
        # GetRuntimeTypeInfo__PaneCFv (0xc, NW4R_UT_RTTI_DECL). Every call site
        # inlines them down to a direct ~LinkListImpl/~Pane call, so nothing in
        # the DOL references them. Dropping restores the retail split layout
        # (decomp .text 0xD9C -> 0xCA0) and fits the 0xCA0 budget.
        drop_text_symbols=(
            "GetRuntimeTypeInfo__Q36nw4hbm3lyt4PaneCFv",
            "__dt__Q36nw4hbm2ut40LinkList<Q36nw4hbm3lyt13AnimTransform,4>Fv",
            "__dt__Q36nw4hbm3lyt13AnimTransformFv",
            "__dt__Q36nw4hbm2ut31LinkList<Q36nw4hbm3lyt5Group,4>Fv",
        ),
        # The dropped weaks occupied mid-section 16-aligned slots; without
        # repacking, MWCC's pre-drop padding residue leaves later survivors off
        # the retail offsets (same fix as lyt_group/lyt_window).
        repack_after_drop=16,
    ),
    "lyt_drawInfo.o": UnitRules(
        # MWCC emits the unreferenced weak inline-empty ut::Rect dtor
        # (__dt__Q36nw4hbm2ut4RectFv, 0x40 deleting wrapper) with the DrawInfo
        # code; ~DrawInfo inlines the trivial member destruction, so no .text/
        # .data reference survives and the retail linker dead-stripped it
        # (no __dt__Rect anywhere in the DOL). Dropping the orphan restores
        # the retail split layout and fits the 0xC0 budget.
        drop_text_symbols=("__dt__Q36nw4hbm2ut4RectFv",),
    ),
    "lyt_group.o": UnitRules(
        # MWCC emits unreferenced weak in-charge dtors for the instantiated
        # ut::LinkList<Group,4> (GroupContainer::mGroupList) and
        # ut::LinkList<detail::PaneLink,0> (Group::mPaneLinkList) templates;
        # every call site inlines them down to a direct ~LinkListImpl call, so
        # nothing in the DOL references the 0x58 wrappers and the retail linker
        # dead-stripped them (retail split holds only the 5 surviving symbols).
        # repack_after_drop re-lays the survivors at 16-byte boundaries like
        # the retail GC, dropping the +0xC padding residue from the mid-section
        # drops.
        drop_text_symbols=(
            "__dt__Q36nw4hbm2ut31LinkList<Q36nw4hbm3lyt5Group,4>Fv",
            "__dt__Q36nw4hbm2ut41LinkList<Q46nw4hbm3lyt6detail8PaneLink,0>Fv",
        ),
        repack_after_drop=16,
    ),
    "HBMAnmController.o": UnitRules(
        # MWCC emits the unreferenced weak inline-empty base dtor
        # __dt__Q210homebutton15FrameControllerFv (0x40 deleting wrapper) with
        # the GroupAnmController vtable; nothing in the DOL references it (no
        # FrameController vtable/dtor anywhere in retail; the derived dtor
        # elides the base call), so the retail linker dead-stripped it.
        # Dropping the orphan restores the retail split layout and fits the
        # 0x110 budget.
        drop_text_symbols=("__dt__Q210homebutton15FrameControllerFv",),
        # GroupAnmController vtable typeinfo ptr (+0x30) references the base
        # FrameController RTTI externally in retail; MWCC emits a weak local
        # copy (+0x28 .data over the 0x48 slice). Retarget + drop the tail.
        retarget_relocs=(
            (".data", 0x30, "__RTTI__Q210homebutton15FrameController"),
        ),
        drop_data_tail=((".data", 0x48),),
    ),
    "lyt_arcResourceAccessor.o": UnitRules(
        # MWCC emits the unreferenced weak in-charge dtor of the implicit
        # ut::LinkList<FontRefLink,0> template (mFontList). The Arc dtor is
        # declared out-of-line (defined strong in HBMBase.o) so no call site
        # exists in this TU: every destruction path inlines down to a direct
        # ~LinkListImpl call, the vtable's dtor slot resolves to HBMBase's
        # strong copy at link, and nothing in the DOL references the 0x58
        # wrapper — the retail linker dead-stripped it (retail split holds
        # only the 6 surviving symbols). Dropping restores the retail split
        # layout (decomp .text 0x400 -> 0x3A0) and fits the 0x3A0 budget.
        drop_text_symbols=(
            "__dt__Q36nw4hbm2ut38LinkList<Q36nw4hbm3lyt11FontRefLink,0>Fv",
        ),
        repack_after_drop=16,
    ),
    "dvd_broadway.o": UnitRules(
        # DECOMP_FORCEACTIVE emitters (dvdContexts .bss anchor at line 155;
        # tmd strings + coverStatus/coverRegister pool anchor at line 495) are
        # unreferenced text the retail linker GC'd (the retail .o keeps the
        # .bss/.rodata anchors but no emitter functions). Dropping the two
        # orphans (0x10 + 0x28) restores the retail split layout and fits the
        # 0x26B0 budget.
        drop_text_symbols=(
            "FORCEACTIVEdvd_broadway_c155",
            "FORCEACTIVEdvd_broadway_c495",
        ),
    ),
    "ut_ResFontBase.o": UnitRules(
        # MWCC emits the weak inline-empty Font dtor
        # (__dt__Q36nw4hbm2ut4FontFv / __dt__Q34nw4r2ut4FontFv, 0x40 deleting
        # wrapper) wherever the Font vtable is emitted. The retail linker GC'd
        # the weak copies: the DOL-extracted retail .o shows the Font vtable's
        # dtor slot referencing the strong copy in the lyt_textBox TU (UNDEF
        # here; lyt_textBox.o defines __dt__FontFv at 0x1270/0x153c). Dropping
        # as UNDEF lets the live vtable ref resolve to that strong copy at
        # link (resFontBase ctor also references __vt__Font, so the vtable is
        # NOT orphaned — plain drop would leave a dangling ABS pointer).
        drop_text_symbols_as_undef=(
            "__dt__Q36nw4hbm2ut4FontFv",
            "__dt__Q34nw4r2ut4FontFv",
        ),
        # ResFontBase vtable typeinfo ptr (+0x10) and the Font vtable
        # typeinfo ptr (+0x88) reference the class RTTI externally in retail
        # (strong copies in the lyt_textBox TU); MWCC emits weak local names +
        # structs (+0x58 .data over the 0xC0 slice). Retarget, drop the
        # mid-section ResFontBase name (0x68..0x88), then cut the Font name +
        # RTTI tail.
        retarget_relocs=(
            (".data", 0x10, "__RTTI__Q46nw4hbm2ut6detail11ResFontBase"),
            (".data", 0x88, "__RTTI__Q36nw4hbm2ut4Font"),
        ),
        drop_data_range=((".data", 0x68, 0x88),),
        drop_data_tail=((".data", 0xC0),),
    ),
    "ut_RomFont.o": UnitRules(
        # Same weak inline-empty Font dtor orphan as ut_ResFontBase (nw4r
        # variant); strong copy lives in nw4r lyt_textBox.o.
        drop_text_symbols_as_undef=("__dt__Q34nw4r2ut4FontFv",),
    ),
    "lyt_window.o": UnitRules(
        # MWCC emits the unreferenced weak in-charge dtor of the nested
        # Window::Content (0x64: __destroy_arr of vtxColors[4] + delete-flag
        # wrapper). ~Window inlines the member destruction (retail sequence:
        # texCoordAry.Free + __destroy_arr), so no .text/.data reference
        # survives and the retail linker dead-stripped the orphan.
        drop_text_symbols=("__dt__Q46nw4hbm3lyt6Window7ContentFv",),
        # The dropped weak occupied a 16-aligned slot before ~Window; without
        # repacking, MWCC's pre-drop padding residue leaves ~Window at 0x21C
        # (retail 0x210) and the unit +0xC over budget. Re-lay survivors at
        # align(prev_end, 16) exactly like the retail linker GC (same fix as
        # lyt_group).
        repack_after_drop=16,
        # Window's typeinfo chain (base ptrs at +0xCC/+0xD4) references the
        # Pane/PaneBase RTTI EXTERNALLY in retail; MWCC emits weak local
        # copies (+0x50 .data over the 0xE8 slice). Retarget + drop the tail.
        retarget_relocs=(
            (".data", 0xCC, "__RTTI__Q46nw4hbm3lyt6detail8PaneBase"),
            (".data", 0xD4, "__RTTI__Q36nw4hbm3lyt4Pane"),
        ),
        drop_data_tail=((".data", 0xE8),),
    ),
    "lyt_pane.o": UnitRules(
        # MWCC emits unreferenced weak orphans the retail linker GC'd: the
        # PaneBase deleting-dtor wrapper (0x40; the base ctor/dtor are inline
        # in lyt_pane.h so ~Pane elides the base call), the implicit
        # LinkList<Pane,4>/LinkList<AnimationLink,0> template-dtor wrappers
        # (0x58 each, same pattern as lyt_group/lyt_layout — ~Pane inlines
        # them down to direct ~LinkListImpl calls), and the inline RTTI
        # accessor GetRuntimeTypeInfo__PaneCFv (0xc, NW4R_UT_RTTI_DECL; only
        # referenced via the vtable slot 0xC, and HBMGUIManager.o supplies the
        # weak definition the DOL link resolves). The only .data reference to
        # the dropped dtor sits in the equally-orphaned weak __vt__PaneBase
        # (linker-GC'd), so the full main.elf link is clean. Dropping restores
        # the retail split layout (decomp .text 0x1280 -> 0x1150) and fits the
        # 0x1150 budget.
        drop_text_symbols=(
            "__dt__Q46nw4hbm3lyt6detail8PaneBaseFv",
            "__dt__Q36nw4hbm2ut30LinkList<Q36nw4hbm3lyt4Pane,4>Fv",
            "__dt__Q36nw4hbm2ut40LinkList<Q36nw4hbm3lyt13AnimationLink,0>Fv",
            "GetRuntimeTypeInfo__Q36nw4hbm3lyt4PaneCFv",
        ),
        # The dropped weaks occupied mid-section 16-aligned slots; without
        # repacking, MWCC's pre-drop padding residue leaves later survivors off
        # the retail offsets (same fix as lyt_group/lyt_layout).
        repack_after_drop=16,
    ),
    "snd_BasicSound.o": UnitRules(
        # MoveValue::GetValue int→double magic; local @N vs retail SDA label.
        pool_patterns=(
            (struct.pack(">II", MAGIC_HI, MAGIC_LO), "lbl_eu_80669EF0"),
        ),
        exact_renames=(
            ("__vt__Q44nw4r3snd6detail10BasicSound", "lbl_eu_8056A710"),
        ),
    ),
    # SortPriorityList() static buckets + C++ guard; LinkList Ofs is 252 in
    # source (node@0xFC) but retail construct_array mangles 256.
    "snd_SoundPlayer.o": UnitRules(
        exact_renames=(
            (
                "__ct__Q34nw4r2ut44LinkList<Q44nw4r3snd6detail10BasicSound,252>Fv",
                "__ct__Q34nw4r2ut44LinkList<Q44nw4r3snd6detail10BasicSound,256>Fv",
            ),
            (
                "__dt__Q34nw4r2ut44LinkList<Q44nw4r3snd6detail10BasicSound,252>Fv",
                "__dt__Q34nw4r2ut44LinkList<Q44nw4r3snd6detail10BasicSound,256>Fv",
            ),
        ),
        prefix_renames=(
            ("@GUARD@listsByPrio$", "lbl_eu_80665500"),
            ("listsByPrio$", "lbl_eu_806382F0"),
            ("__arraydtor$", "__arraydtor$4226"),
            # register_global_object cookie immediately before the bucket array.
            ("@#", "lbl_eu_806382E0"),
        ),
        symbol_sizes=(
            ("lbl_eu_806382F0", 0x600),
        ),
        drop_text_symbols=(
            "__dt__Q34nw4r2ut12LinkListNodeFv",
            "__dt__Q34nw4r2ut44LinkList<Q44nw4r3snd6detail10BasicSound,244>Fv",
            "__dt__Q34nw4r2ut43LinkList<Q44nw4r3snd6detail10PlayerHeap,24>Fv",
            "__dt__Q44nw4r2ut29@unnamed@snd_SoundPlayer_cpp@11NonCopyableFv",
            "__dt__Q54nw4r3snd6detail11SoundThread8AutoLockFv",
        ),
    ),
    "snd_PlayerHeap.o": UnitRules(
        exact_renames=(
            ("__vt__Q44nw4r3snd6detail10PlayerHeap", "lbl_eu_8056AAE8"),
        ),
        drop_text_symbols=(
            "__dt__Q34nw4r2ut12LinkListNodeFv",
            "__dt__Q44nw4r2ut28@unnamed@snd_PlayerHeap_cpp@11NonCopyableFv",
            "__dt__Q54nw4r3snd6detail11SoundThread8AutoLockFv",
        ),
    ),

    "lyt_textBox.o": UnitRules(
        # MWCC emits unreferenced weak in-charge dtors of CharWriter's private
        # nested ColorMapping/VertexColor/TextColor structs (0x40 each) because
        # this TU copy-initializes WideTextWriter (implicit copy-ctor ODR-use);
        # every call site inlines the member destruction down to ~Color, so no
        # .text/.data reference survives and the retail linker dead-stripped
        # them (retail split holds only the 16 symbols; the nested structs use
        # ut::Color members, per the ut_TextWriterBase FULL_MATCH revert).
        # Dropping the orphans restores the retail split layout and fits the
        # 0x1450 budget (decomp .text 0x14DC -> 0x141C).
        drop_text_symbols=(
            "__dt__Q46nw4hbm2ut10CharWriter12ColorMappingFv",
            "__dt__Q46nw4hbm2ut10CharWriter11VertexColorFv",
            "__dt__Q46nw4hbm2ut10CharWriter9TextColorFv",
        ),
        # The dropped weaks were contiguous 0x40 blocks after DrawSelf; the
        # survivors stay 4-aligned (retail lyt_textBox is 4-aligned, no 16-byte
        # repack — repack_after_drop would grow .text here).
    ),

    "CItemBoxGrid.o": UnitRules(
        exact_renames=(
            ("ArrayGet12__FPCUsUc", "ArrayGet12"),
            ("CheckState4_Animate__FPc", "CheckState4_Animate"),
            ("CopyVec4s__FPsPCs", "CopyVec4s"),
            ("GetField52D__12CItemBoxGridFv", "GetField52D"),
            ("GetField549__12CItemBoxGridFv", "GetField549"),
            ("GetField61__12CItemBoxGridFv", "GetField61"),
            ("LookupIndexedByte__FPc", "LookupIndexedByte"),
            ("PushToList__12CItemBoxGridFUc", "PushToList"),
            ("func_801C4BB4__Fv", "func_801C4BB4"),
            ("func_801C51BC__FPvUl", "func_801C51BC"),
            ("func_801C5254__Fv", "func_801C5254"),
            ("func_801C53D8__Fv", "func_801C53D8"),
            ("func_801C562C__FPvPv", "func_801C562C"),
            ("func_801C56D8__Fv", "func_801C56D8"),
            ("func_801C5E5C__FPv", "func_801C5E5C"),
            ("func_801C5EF4__FP16CItemBoxGridFull", "func_801C5EF4"),
            ("func_801C5F20__FP16CItemBoxGridFull", "func_801C5F20"),
            ("func_801C5F48__FP16CItemBoxGridFullUs", "func_801C5F48"),
            ("func_801C5FC0__Fv", "func_801C5FC0"),
            ("func_801C6158__Fd", "func_801C6158"),
            ("func_801C618C__Fv", "func_801C618C"),
            ("func_801C62AC__FP16CItemBoxGridFullUs", "func_801C62AC"),
            ("func_801C631C__FP16CItemBoxGridFullUs", "func_801C631C"),
            ("func_801C6388__FP16CItemBoxGridFullUs", "func_801C6388"),
            ("func_801C6528__FP16CItemBoxGridFullUs", "func_801C6528"),
            ("func_801C65A0__FP16CItemBoxGridFullUs", "func_801C65A0"),
            ("func_801C6618__FP16CItemBoxGridFullUs", "func_801C6618"),
            ("func_801C6690__FP16CItemBoxGridFullUs", "func_801C6690"),
            ("func_801C6708__FP16CItemBoxGridFullUs", "func_801C6708"),
            ("func_801C673C__FP16CItemBoxGridFullUs", "func_801C673C"),
            ("func_801C6770__FP16CItemBoxGridFullUs", "func_801C6770"),
            ("func_801C68A0__FP16CItemBoxGridFull", "func_801C68A0"),
            ("func_801C6938__FPvUl", "func_801C6938"),
            ("func_801C6A44__Fv", "func_801C6A44"),
            ("func_801C6EC0__FP16CItemBoxGridFullUs", "func_801C6EC0"),
            ("func_801C7730__FP16CItemBoxGridFull", "func_801C7730"),
            ("func_801C7958__Fv", "func_801C7958"),
            ("func_801C7C7C__Fv", "func_801C7C7C"),
            ("func_801C7EF0__FP16CItemBoxGridFullUl", "func_801C7EF0"),
            ("func_801C81D0__FP16CItemBoxGridFull", "func_801C81D0"),
            ("func_801C82D0__FP16CItemBoxGridFull", "func_801C82D0"),
            ("func_801C83E4__FP16CItemBoxGridFull", "func_801C83E4"),
            ("func_801C8534__FP16CItemBoxGridFull", "func_801C8534"),
            ("func_801C8634__FP16CItemBoxGridFull", "func_801C8634"),
            ("func_801C87CC__FP16CItemBoxGridFull", "func_801C87CC"),
            ("func_801C88B0__FP16CItemBoxGridFull", "func_801C88B0"),
            ("func_801C8994__FP16CItemBoxGridFull", "func_801C8994"),
            ("func_801C8ACC__FP16CItemBoxGridFull", "func_801C8ACC"),
            ("func_801C8C58__FP16CItemBoxGridFull", "func_801C8C58"),
            ("func_801C8DE4__FP16CItemBoxGridFull", "func_801C8DE4"),
            ("func_801C8F04__FP16CItemBoxGridFull", "func_801C8F04"),
            ("func_801C9040__FP16CItemBoxGridFull", "func_801C9040"),
            ("func_801C9158__FP16CItemBoxGridFull", "func_801C9158"),
            ("func_801C9270__FP16CItemBoxGridFull", "func_801C9270"),
            ("func_801C9390__FP16CItemBoxGridFull", "func_801C9390"),
            ("func_801C94E0__FP16CItemBoxGridFull", "func_801C94E0"),
            ("func_801C9630__FP16CItemBoxGridFull", "func_801C9630"),
            ("func_801C9780__FP16CItemBoxGridFull", "func_801C9780"),
            ("func_801C98D0__FP16CItemBoxGridFull", "func_801C98D0"),
            ("func_801C9A3C__FP16CItemBoxGridFull", "func_801C9A3C"),
            ("func_801C9B8C__FP16CItemBoxGridFull", "func_801C9B8C"),
            ("func_801C9CCC__FP16CItemBoxGridFull", "func_801C9CCC"),
            ("func_801C9E1C__FP16CItemBoxGridFull", "func_801C9E1C"),
            ("func_801CA070__FPvPv", "func_801CA070"),
            ("func_801CA110__FPvPv", "func_801CA110"),
            ("func_801CAA6C__Fv", "func_801CAA6C"),
            ("func_801CABC8__Fv", "func_801CABC8"),
            ("func_801CAD8C__Fv", "func_801CAD8C"),
            ("func_801CAE9C__Fv", "func_801CAE9C"),
            ("func_801CB038__FPv", "func_801CB038"),
            ("func_801CB0FC__FPv", "func_801CB0FC"),
            ("func_801CB184__FPv", "func_801CB184"),
            ("func_801CB1E4__FPv", "func_801CB1E4"),
            ("func_801CB28C__FPv", "func_801CB28C"),
            ("func_801CB38C__FPv", "func_801CB38C"),
            ("func_801CB480__FPv", "func_801CB480"),
            ("func_801CB4E4__FPv", "func_801CB4E4"),
            ("func_801CB56C__FPv", "func_801CB56C"),
            ("func_801CB5F0__Fv", "func_801CB5F0"),
            ("func_801CB9D8__FPUlPvUl", "func_801CB9D8"),
            ("func_801CBA04__Fv", "func_801CBA04"),
            ("func_801CBDE8__Fv", "func_801CBDE8"),
            ("func_801CC0EC__Fv", "func_801CC0EC"),
            ("func_801CC3F4__FPv", "func_801CC3F4"),
            ("func_801CC4E8__FPv", "func_801CC4E8"),
            ("func_801CC5DC__Fv", "func_801CC5DC"),
            ("func_801CC7B0__Fv", "func_801CC7B0"),
            ("func_801CCAF0__Fv", "func_801CCAF0"),
            ("func_801CDB94__FPvUl", "func_801CDB94"),
            ("func_801CDBE0__FPv", "func_801CDBE0"),
            ("func_801CDC40__Fv", "func_801CDC40"),
            ("func_801CDEE8__FPv", "func_801CDEE8"),
            ("func_801CDFB4__FPv", "func_801CDFB4"),
            ("func_801CE108__FPv", "func_801CE108"),
            ("func_801CE1A0__Fv", "func_801CE1A0"),
            ("func_801CE2F8__FPv", "func_801CE2F8"),
            ("func_801CE3E8__Fv", "func_801CE3E8"),
            ("func_801CE4B4__FPv", "func_801CE4B4"),
            ("func_801CE524__Fv", "func_801CE524"),
            ("func_801CE974__FPv", "func_801CE974"),
            ("func_801CE9E8__FPv", "func_801CE9E8"),
            ("func_801CEA30__FPv", "func_801CEA30"),
            ("func_801CEAA0__FPv", "func_801CEAA0"),
            ("func_801CEAE8__FPv", "func_801CEAE8"),
            ("func_801CEB3C__FPv", "func_801CEB3C"),
            ("func_801CEBF0__FPv", "func_801CEBF0"),
            ("func_801CEC80__FPv", "func_801CEC80"),
            ("func_801CECD0__Fv", "func_801CECD0"),
            ("func_801CF240__Fv", "func_801CF240"),
            ("func_801CF71C__Fv", "func_801CF71C"),
            ("func_801CF900__Fv", "func_801CF900"),
            ("func_801CFA58__Fv", "func_801CFA58"),
            ("func_801CFCBC__FPvUl", "func_801CFCBC"),
            ("func_801CFD2C__Fv", "func_801CFD2C"),
            ("func_801CFF28__Fv", "func_801CFF28"),
            ("func_801D05D4__Fv", "func_801D05D4"),
            ("func_801D0950__Fv", "func_801D0950"),
            ("func_801D0BD8__Fv", "func_801D0BD8"),
            ("func_801D0E88__Fv", "func_801D0E88"),
            ("func_801D11B8__FPvPvi", "func_801D11B8"),
            ("func_801D1220__FPv", "func_801D1220"),
            ("func_801D12D4__FPvPv", "func_801D12D4"),
            ("func_801D1F9C__FPsUl", "func_801D1F9C"),
            ("sinit_801D1E30__Fv", "sinit_801D1E30"),
            ("OnFileEvent__12CItemBoxGridFP10CEventFile__Fv", "OnFileEvent__12CItemBoxGridFP10CEventFile"),
            ("__ct__801C5514__FPv", "__ct__801C5514"),
            ("__ct__CArtsBookItem__FPv", "__ct__CArtsBookItem"),
            ("__ct__CItemBoxGrid__Fv", "__ct__CItemBoxGrid"),
            ("__dt__10CQuestItemFv__FPvi", "__dt__10CQuestItemFv"),
            ("__dt__11CVisionItemFv__FPvi", "__dt__11CVisionItemFv"),
            ("__dt__12CItemBoxGridFv__Fv", "__dt__12CItemBoxGridFv"),
            ("__dt__13CArtsBookItemFv__FPvi", "__dt__13CArtsBookItemFv"),
            ("__dt__801C5670__FPvi", "__dt__801C5670"),
        ),
    ),
    "CVec4.o": UnitRules(
        exact_renames=(("...bss.0", "zero__Q22ml5CVec4"),),
    ),
    "CCol3.o": UnitRules(
        exact_renames=(("...bss.0", "lbl_eu_80656C40"),),
    ),
    "CCol4.o": UnitRules(
        exact_renames=(("...bss.0", "white__Q22ml5CCol4"),),
    ),
    "FloatUtils.o": UnitRules(
        exact_renames=(
            ("huge__2ml", "lbl_eu_8066A214"),
            ("hugeminus__2ml", "lbl_eu_8066A218"),
        ),
    ),
    "CCamUtil.o": UnitRules(
        pool_patterns=(
            (struct.pack(">I", 0x4222F983), "lbl_eu_8066A190"),  # 40.74366 (deg2rad)
            (struct.pack(">I", 0xBF800000), "lbl_eu_8066A194"),  # -1.0
            (struct.pack(">I", 0x3F800000), "lbl_eu_8066A198"),  # 1.0
            (struct.pack(">I", 0x3CC90FDB), "lbl_eu_8066A19C"),  # 0.02454369 (rad2FIdx)
            (struct.pack(">I", 0x00000000), "lbl_eu_8066A1A0"),  # 0.0
        ),
        exact_renames=(
            ("halfpi__2ml", "lbl_eu_8066A200"),
            ("@STRING@AsinRad__Q24nw4r4mathFf", "lbl_eu_8052ADB0"),
            ("@STRING@AsinRad__Q24nw4r4mathFf@0", "lbl_eu_8052AD88"),
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


def rename_by_prefix(path: Path, prefixes: tuple[tuple[str, str], ...]) -> bool:
    """Rename the first symbol matching each old prefix (MWCC $N drift).

    Prefix ``@#`` matches a lone ``@`` + digits cookie in ``.bss``
    (register_global_object).
    """
    if not prefixes:
        return False

    data = path.read_bytes()
    sections, by_name = _read_elf_sections(data)
    sym_idx = by_name.get(".symtab")
    str_idx = by_name.get(".strtab")
    bss_idx = by_name.get(".bss")
    if sym_idx is None or str_idx is None:
        return False

    _, sym_off, sym_size, _ = next(s for s in sections if s[0] == sym_idx)
    _, str_off, _, _ = next(s for s in sections if s[0] == str_idx)

    # name -> (shndx, ...)
    sym_shndx: dict[str, int] = {}
    present: list[str] = []
    for so in range(0, sym_size, 16):
        st_name, _st_value, _st_size, _st_info, _st_other, st_shndx = struct.unpack_from(
            ">IIIBBH", data, sym_off + so
        )
        if st_name == 0:
            continue
        end = data.index(0, str_off + st_name)
        sname = data[str_off + st_name : end].decode("ascii")
        present.append(sname)
        sym_shndx[sname] = st_shndx
    present_set = set(present)
    present_sorted = sorted(present)

    renames: list[tuple[str, str]] = []
    used_old: set[str] = set()
    used_new: set[str] = set()
    for prefix, new in prefixes:
        if new in used_new or new in present_set:
            continue
        for name in present_sorted:
            if name in used_old or name == new:
                continue
            if prefix == "@#":
                if (
                    bss_idx is not None
                    and sym_shndx.get(name) == bss_idx
                    and name.startswith("@")
                    and name[1:].isdigit()
                ):
                    renames.append((name, new))
                    used_old.add(name)
                    used_new.add(new)
                    break
            elif name.startswith(prefix):
                renames.append((name, new))
                used_old.add(name)
                used_new.add(new)
                break
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


def drop_text_symbols(
    path: Path, names: tuple[str, ...], as_undef: tuple[str, ...] = ()
) -> bool:
    """Remove named .text functions and compact the section.

    Weak inline-virtual dtors (IGameException) are emitted into every
    implementing TU; retail omits some from a given split. Deleting the
    bytes and shifting later symbols/relocs restores the retail budget
    without growing real derived dtors (out-of-line empty bases add a bl).

    Symbols in *as_undef* are converted to UNDEF (shndx=0) instead of SHN_ABS
    so surviving relocations (e.g. a live vtable entry) resolve to the strong
    definition in another TU at link, mirroring the DOL-extracted retail .o.
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

        # Invalidate the dropped symbol. as_undef names become UNDEF so live
        # .data/.extabindex relocations resolve to the strong copy elsewhere
        # (retail linker GC behaviour); everything else is ABS-empty (orphan
        # weaks whose referencing vtables are also GC'd).
        st_name = struct.unpack_from(">I", data, sym_off + so)[0]
        name_end = data.index(0, str_off + st_name)
        sname = data[str_off + st_name : name_end].decode("ascii")
        struct.pack_into(">I", data, sym_off + so + 8, 0)
        if sname in as_undef:
            struct.pack_into(">H", data, sym_off + so + 14, 0)  # SHN_UNDEF
        else:
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


def repack_text(path: Path, align: int) -> bool:
    """Re-pack surviving .text FUNCs at ``align`` boundaries after drops.

    drop_text_symbols removes weak orphan bytes and shifts later symbols by
    the dropped length, which keeps MWCC's pre-drop padding residue. The
    retail linker's GC re-lays the survivors at their natural alignment, so
    the shifted layout can overshoot the split budget by a few bytes (e.g.
    lyt_group +0xC from mid-section weak template-dtor drops). This pass
    rebuilds the .text layout exactly like the retail dead-strip: each
    survivor at align(prev_end, align), with symbols and relocs remapped.
    """
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
    sym_hdr = sh_hdr_off[sym_idx]
    sym_off = struct.unpack_from(">I", data, sym_hdr + 16)[0]
    sym_size = struct.unpack_from(">I", data, sym_hdr + 20)[0]
    str_hdr = sh_hdr_off[str_idx]
    str_off = struct.unpack_from(">I", data, str_hdr + 16)[0]

    # Collect surviving .text FUNC symbols in address order.
    fns: list[tuple[int, int, int]] = []  # (sym_entry_off, st_value, st_size)
    for so in range(0, sym_size, 16):
        st_name = struct.unpack_from(">I", data, sym_off + so)[0]
        st_value = struct.unpack_from(">I", data, sym_off + so + 4)[0]
        st_size = struct.unpack_from(">I", data, sym_off + so + 8)[0]
        st_info = data[sym_off + so + 12]
        st_shndx = struct.unpack_from(">H", data, sym_off + so + 14)[0]
        if st_shndx != text_idx or (st_info & 0xF) != 2 or st_size == 0:
            continue
        fns.append((so, st_value, st_size))
    if not fns:
        return False
    fns.sort(key=lambda t: t[1])

    # New layout: each survivor at align(prev_end, align).
    moves: list[tuple[int, int, int]] = []  # (old_start, new_start, size)
    cursor = 0
    for so, st_value, st_size in fns:
        new_start = (cursor + align - 1) & ~(align - 1)
        moves.append((st_value, new_start, st_size))
        cursor = new_start + st_size
    if all(o == n for o, n, _ in moves):
        return False

    old_text = bytes(data[text_off : text_off + text_size])
    new_text = bytearray()
    cursor = 0
    for old_start, new_start, st_size in moves:
        new_text.extend(b"\0" * (new_start - cursor))
        new_text.extend(old_text[old_start : old_start + st_size])
        cursor = new_start + st_size
    new_size = len(new_text)
    sect_size = (new_size + align - 1) & ~(align - 1)
    if sect_size > text_size:
        raise ValueError("repack_text grew .text")

    def remap(off: int) -> int:
        # Offset in the packed layout for an old .text offset (padding after
        # the containing FUNC keeps that FUNC's shift).
        delta = 0
        for old_start, new_start, _size in moves:
            if old_start <= off:
                delta = old_start - new_start
        return off - delta

    # Remap every .text symbol (FUNC and any local labels).
    for so in range(0, sym_size, 16):
        st_shndx = struct.unpack_from(">H", data, sym_off + so + 14)[0]
        if st_shndx != text_idx:
            continue
        st_value = struct.unpack_from(">I", data, sym_off + so + 4)[0]
        struct.pack_into(">I", data, sym_off + so + 4, remap(st_value))

    # Remap / drop .rela.text entries past the packed size.
    if rela_idx is not None:
        rela_hdr = sh_hdr_off[rela_idx]
        rela_off = struct.unpack_from(">I", data, rela_hdr + 16)[0]
        rela_size = struct.unpack_from(">I", data, rela_hdr + 20)[0]

        # Old span of each surviving FUNC: (start, end). Relocs that fall in
        # bytes NOT covered by any surviving function belong to anonymous
        # orphan blobs (weak copies the retail linker GC'd) that the packed
        # layout drops. remap() would otherwise collide them onto surviving
        # relocs (e.g. lyt_window 0x260E/0x261E -> both 0x2602, duplicate
        # .rela.text entries) — drop them instead.
        spans = [(o, n, s) for o, n, s in moves]

        def _containing(old_off: int) -> bool:
            for o, _n, s in spans:
                if o <= old_off < o + s:
                    return True
            return False

        keep = bytearray()
        for ro in range(0, rela_size, 12):
            r_offset = struct.unpack_from(">I", data, rela_off + ro)[0]
            if r_offset >= len(old_text):
                continue
            if not _containing(r_offset):
                continue  # reloc of a dropped anonymous orphan blob
            new_off = remap(r_offset)
            if new_off >= new_size:
                continue
            entry = bytearray(data[rela_off + ro : rela_off + ro + 12])
            struct.pack_into(">I", entry, 0, new_off)
            keep.extend(entry)
        data[rela_off : rela_off + rela_size] = b"\0" * rela_size
        data[rela_off : rela_off + len(keep)] = keep
        struct.pack_into(">I", data, rela_hdr + 20, len(keep))

    data[text_off : text_off + text_size] = new_text + b"\0" * (text_size - new_size)
    struct.pack_into(">I", data, text_hdr + 20, sect_size)
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


def drop_data_tail(path: Path, section: str, keep_size: int) -> bool:
    """Shrink *section* to keep_size, dropping trailing orphaned data.

    MWCC occasionally emits weak copies of data the retail linker GC'd (pool
    constants no relocation references, base-class typeinfo names/structs
    whose live references retail keeps external). Symbols past the cut are
    ABS'd; relocs pointing past it are dropped. Callers retarget surviving
    relocs to the retail symbols first (see retarget_reloc_to_symbol).
    """
    data = bytearray(path.read_bytes())
    if data[:4] != b"\x7fELF" or data[5] != 2:
        raise ValueError(f"expected big-endian ELF32: {path}")

    e_shoff = struct.unpack_from(">I", data, 32)[0]
    e_shentsize = struct.unpack_from(">H", data, 46)[0]
    e_shnum = struct.unpack_from(">H", data, 48)[0]
    e_shstrndx = struct.unpack_from(">H", data, 50)[0]
    shstr_off = struct.unpack_from(">I", data, e_shoff + e_shstrndx * e_shentsize + 16)[0]

    sec_idx = sec_hoff = sec_off = sec_size = None
    sym_idx = rela_idx = None
    for i in range(e_shnum):
        hoff = e_shoff + i * e_shentsize
        sh_name = struct.unpack_from(">I", data, hoff)[0]
        end = data.index(0, shstr_off + sh_name)
        name = data[shstr_off + sh_name : end].decode("ascii")
        if name == section:
            sec_idx, sec_hoff = i, hoff
            sec_off = struct.unpack_from(">I", data, hoff + 16)[0]
            sec_size = struct.unpack_from(">I", data, hoff + 20)[0]
        elif name == ".symtab":
            sym_idx = i
        elif name == ".rela" + section:
            rela_idx = i
    if sec_idx is None or sec_size is None or sec_hoff is None:
        return False
    if sec_size <= keep_size:
        return False

    # Drop trailing bytes.
    sec_end = sec_off + sec_size
    new_end = sec_off + keep_size
    data = data[:new_end] + data[sec_end:]
    e_shoff = struct.unpack_from(">I", data, 32)[0]
    if e_shoff >= sec_end:
        e_shoff -= (sec_end - new_end)
        struct.pack_into(">I", data, 32, e_shoff)
    for i in range(e_shnum):
        hoff = e_shoff + i * e_shentsize
        sh_offset = struct.unpack_from(">I", data, hoff + 16)[0]
        if i == sec_idx:
            struct.pack_into(">I", data, hoff + 20, keep_size)
        elif sh_offset >= sec_end:
            struct.pack_into(">I", data, hoff + 16, sh_offset - (sec_end - new_end))

    # ABS symbols past the cut (objdiff ignores bounds); drop relocs past it.
    if sym_idx is not None:
        sym_off = struct.unpack_from(">I", data, e_shoff + sym_idx * e_shentsize + 16)[0]
        sym_size = struct.unpack_from(">I", data, e_shoff + sym_idx * e_shentsize + 20)[0]
        for so in range(0, sym_size, 16):
            st_value = struct.unpack_from(">I", data, sym_off + so + 4)[0]
            st_shndx = struct.unpack_from(">H", data, sym_off + so + 14)[0]
            if st_shndx == sec_idx and st_value >= keep_size:
                struct.pack_into(">I", data, sym_off + so + 8, 0)  # st_size
                struct.pack_into(">H", data, sym_off + so + 14, 0xFFF1)  # SHN_ABS
    if rela_idx is not None:
        rela_hoff = e_shoff + rela_idx * e_shentsize
        rela_off = struct.unpack_from(">I", data, rela_hoff + 16)[0]
        rela_size = struct.unpack_from(">I", data, rela_hoff + 20)[0]
        keep = bytearray()
        for ro in range(0, rela_size, 12):
            r_offset = struct.unpack_from(">I", data, rela_off + ro)[0]
            if r_offset < keep_size:
                keep.extend(data[rela_off + ro : rela_off + ro + 12])
        data[rela_off : rela_off + rela_size] = b"\0" * rela_size
        data[rela_off : rela_off + len(keep)] = keep
        struct.pack_into(">I", data, rela_hoff + 20, len(keep))

    path.write_bytes(data)
    return True


def trim_sdata2_section(path: Path, keep_size: int) -> bool:
    """Shrink .sdata2 to keep_size (see drop_data_tail)."""
    return drop_data_tail(path, ".sdata2", keep_size)



def retarget_reloc_to_symbol(path: Path, section: str, offset: int, new_name: str) -> bool:
    """Point the reloc at (section, offset) at an UNDEF symbol *new_name*.

    Used with drop_data_tail: the dropped weak copy's live references must
    resolve to the retail-correct strong definition in another TU at link
    (NonMatching TUs link the extracted retail .o, so __RTTI__/typeinfo
    symbols resolve there). The reloc's target symbol becomes SHN_UNDEF and
    is renamed, mirroring the DOL-extracted retail .o.
    """
    data = bytearray(path.read_bytes())
    if data[:4] != b"\x7fELF" or data[5] != 2:
        raise ValueError(f"expected big-endian ELF32: {path}")

    e_shoff = struct.unpack_from(">I", data, 32)[0]
    e_shentsize = struct.unpack_from(">H", data, 46)[0]
    e_shnum = struct.unpack_from(">H", data, 48)[0]
    e_shstrndx = struct.unpack_from(">H", data, 50)[0]
    shstr_off = struct.unpack_from(">I", data, e_shoff + e_shstrndx * e_shentsize + 16)[0]

    sec_idx = sym_idx = rela_idx = str_idx = None
    for i in range(e_shnum):
        hoff = e_shoff + i * e_shentsize
        sh_name = struct.unpack_from(">I", data, hoff)[0]
        end = data.index(0, shstr_off + sh_name)
        name = data[shstr_off + sh_name : end].decode("ascii")
        if name == section:
            sec_idx = i
        elif name == ".symtab":
            sym_idx = i
        elif name == ".rela" + section:
            rela_idx = i
        elif name == ".strtab":
            str_idx = i
    if sec_idx is None or sym_idx is None or rela_idx is None or str_idx is None:
        return False

    rela_hoff = e_shoff + rela_idx * e_shentsize
    rela_off = struct.unpack_from(">I", data, rela_hoff + 16)[0]
    rela_size = struct.unpack_from(">I", data, rela_hoff + 20)[0]
    target_sym = None
    for ro in range(0, rela_size, 12):
        r_offset = struct.unpack_from(">I", data, rela_off + ro)[0]
        if r_offset == offset:
            target_sym = struct.unpack_from(">I", data, rela_off + ro + 4)[0] >> 8
            break
    if target_sym is None:
        return False

    sym_hoff = e_shoff + sym_idx * e_shentsize
    sym_off = struct.unpack_from(">I", data, sym_hoff + 16)[0]
    sym_entry = sym_off + target_sym * 16
    struct.pack_into(">I", data, sym_entry + 4, 0)  # st_value
    struct.pack_into(">I", data, sym_entry + 8, 0)  # st_size
    struct.pack_into(">H", data, sym_entry + 14, 0)  # SHN_UNDEF

    # Rename: append to .strtab (grow section + shift later sections).
    str_hoff = e_shoff + str_idx * e_shentsize
    str_off = struct.unpack_from(">I", data, str_hoff + 16)[0]
    str_size = struct.unpack_from(">I", data, str_hoff + 20)[0]
    name_bytes = new_name.encode("utf-8") + b"\0"
    old_end = str_off + str_size
    data = data[:old_end] + name_bytes + data[old_end:]
    e_shoff = struct.unpack_from(">I", data, 32)[0]
    if e_shoff >= old_end:
        e_shoff += len(name_bytes)
        struct.pack_into(">I", data, 32, e_shoff)
    for i in range(e_shnum):
        hoff = e_shoff + i * e_shentsize
        sh_offset = struct.unpack_from(">I", data, hoff + 16)[0]
        if i == str_idx:
            struct.pack_into(">I", data, hoff + 20, str_size + len(name_bytes))
        elif sh_offset >= old_end:
            struct.pack_into(">I", data, hoff + 16, sh_offset + len(name_bytes))
    struct.pack_into(">I", data, sym_entry + 0, str_size)  # st_name -> new string

    path.write_bytes(data)
    return True



def zero_data_range(path: Path, section: str, start: int, end: int) -> bool:
    """Zero bytes [start, end) of *section* and drop relocs pointing into it.

    Used when MWCC places a weak typeinfo NAME inside a region that is plain
    zero padding in retail (the name resolves externally there). Keeps section
    sizes and offsets stable; the region's relocs (the local name-ptr) are
    dropped, mirroring the retail .o.
    """
    data = bytearray(path.read_bytes())
    if data[:4] != b"\x7fELF" or data[5] != 2:
        raise ValueError(f"expected big-endian ELF32: {path}")

    e_shoff = struct.unpack_from(">I", data, 32)[0]
    e_shentsize = struct.unpack_from(">H", data, 46)[0]
    e_shnum = struct.unpack_from(">H", data, 48)[0]
    e_shstrndx = struct.unpack_from(">H", data, 50)[0]
    shstr_off = struct.unpack_from(">I", data, e_shoff + e_shstrndx * e_shentsize + 16)[0]

    sec_off = sec_size = None
    rela_idx = None
    for i in range(e_shnum):
        hoff = e_shoff + i * e_shentsize
        sh_name = struct.unpack_from(">I", data, hoff)[0]
        e = data.index(0, shstr_off + sh_name)
        name = data[shstr_off + sh_name : e].decode("ascii")
        if name == section:
            sec_off = struct.unpack_from(">I", data, hoff + 16)[0]
            sec_size = struct.unpack_from(">I", data, hoff + 20)[0]
        elif name == ".rela" + section:
            rela_idx = i
    if sec_off is None or sec_size is None:
        return False
    end = min(end, sec_size)
    if start >= end:
        return False

    # Zero the bytes (only if they aren't already).
    changed = any(data[sec_off + start : sec_off + end])
    data[sec_off + start : sec_off + end] = b"\0" * (end - start)

    # Drop relocs whose r_offset falls inside the range.
    if rela_idx is not None:
        rela_hoff = e_shoff + rela_idx * e_shentsize
        rela_off = struct.unpack_from(">I", data, rela_hoff + 16)[0]
        rela_size = struct.unpack_from(">I", data, rela_hoff + 20)[0]
        keep = bytearray()
        for ro in range(0, rela_size, 12):
            r_offset = struct.unpack_from(">I", data, rela_off + ro)[0]
            if not (start <= r_offset < end):
                keep.extend(data[rela_off + ro : rela_off + ro + 12])
        data[rela_off : rela_off + rela_size] = b"\0" * rela_size
        data[rela_off : rela_off + len(keep)] = keep
        struct.pack_into(">I", data, rela_hoff + 20, len(keep))

    if changed:
        path.write_bytes(data)
    return changed



def drop_data_range(path: Path, section: str, start: int, end: int) -> bool:
    """Remove bytes [start, end) of *section*, shifting later symbols/relocs.

    Used for weak base-class vtables/typeinfo the retail linker GC'd that sit
    in the middle of the section (lyt_animation: the AnimTransform vtable
    between AnimTransformBasic's typeinfo and name). Relocs inside the range
    are dropped; later reloc r_offsets and symbol values shift by -len.
    """
    data = bytearray(path.read_bytes())
    if data[:4] != b"\x7fELF" or data[5] != 2:
        raise ValueError(f"expected big-endian ELF32: {path}")

    e_shoff = struct.unpack_from(">I", data, 32)[0]
    e_shentsize = struct.unpack_from(">H", data, 46)[0]
    e_shnum = struct.unpack_from(">H", data, 48)[0]
    e_shstrndx = struct.unpack_from(">H", data, 50)[0]
    shstr_off = struct.unpack_from(">I", data, e_shoff + e_shstrndx * e_shentsize + 16)[0]

    sec_idx = sec_hoff = sec_off = sec_size = None
    sym_idx = rela_idx = None
    for i in range(e_shnum):
        hoff = e_shoff + i * e_shentsize
        sh_name = struct.unpack_from(">I", data, hoff)[0]
        e = data.index(0, shstr_off + sh_name)
        name = data[shstr_off + sh_name : e].decode("ascii")
        if name == section:
            sec_idx, sec_hoff = i, hoff
            sec_off = struct.unpack_from(">I", data, hoff + 16)[0]
            sec_size = struct.unpack_from(">I", data, hoff + 20)[0]
        elif name == ".symtab":
            sym_idx = i
        elif name == ".rela" + section:
            rela_idx = i
    if sec_idx is None or sec_size is None or sec_hoff is None:
        return False
    end = min(end, sec_size)
    length = end - start
    if length <= 0 or start > sec_size:
        return False

    # Remove bytes in place; shift later sections (the section is usually
    # followed by .rela + .symtab etc.).
    sec_end = sec_off + sec_size
    data = data[: sec_off + start] + data[sec_off + end :]
    e_shoff = struct.unpack_from(">I", data, 32)[0]
    if e_shoff >= sec_end:
        e_shoff -= length
        struct.pack_into(">I", data, 32, e_shoff)
    for i in range(e_shnum):
        hoff = e_shoff + i * e_shentsize
        sh_offset = struct.unpack_from(">I", data, hoff + 16)[0]
        if i == sec_idx:
            struct.pack_into(">I", data, hoff + 20, sec_size - length)
        elif sh_offset >= sec_end:
            struct.pack_into(">I", data, hoff + 16, sh_offset - length)

    # Shift symbols with value >= end; ABS symbols inside the range.
    if sym_idx is not None:
        sym_off = struct.unpack_from(">I", data, e_shoff + sym_idx * e_shentsize + 16)[0]
        sym_size = struct.unpack_from(">I", data, e_shoff + sym_idx * e_shentsize + 20)[0]
        for so in range(0, sym_size, 16):
            st_value = struct.unpack_from(">I", data, sym_off + so + 4)[0]
            st_shndx = struct.unpack_from(">H", data, sym_off + so + 14)[0]
            if st_shndx != sec_idx:
                continue
            if start <= st_value < end:
                struct.pack_into(">I", data, sym_off + so + 8, 0)
                struct.pack_into(">H", data, sym_off + so + 14, 0xFFF1)
            elif st_value >= end:
                struct.pack_into(">I", data, sym_off + so + 4, st_value - length)

    # Drop relocs inside the range; shift later r_offsets.
    if rela_idx is not None:
        rela_hoff = e_shoff + rela_idx * e_shentsize
        rela_off = struct.unpack_from(">I", data, rela_hoff + 16)[0]
        rela_size = struct.unpack_from(">I", data, rela_hoff + 20)[0]
        keep = bytearray()
        for ro in range(0, rela_size, 12):
            entry = bytearray(data[rela_off + ro : rela_off + ro + 12])
            r_offset = struct.unpack_from(">I", entry, 0)[0]
            if start <= r_offset < end:
                continue
            if r_offset >= end:
                struct.pack_into(">I", entry, 0, r_offset - length)
            keep.extend(entry)
        data[rela_off : rela_off + rela_size] = b"\0" * rela_size
        data[rela_off : rela_off + len(keep)] = keep
        struct.pack_into(">I", data, rela_hoff + 20, len(keep))

    path.write_bytes(data)
    return True



def retarget_reloc_to_local(path: Path, section: str, offset: int, target_sec: str, target_off: int) -> bool:
    """Point the reloc at (section, offset) at the LOCAL symbol in target_sec
    at target_off (e.g. MWCC's bad_cast typeinfo struct in .data that the
    .sdata base ptr must reference; anon @N names collide across sections)."""
    data = bytearray(path.read_bytes())
    if data[:4] != b"\x7fELF" or data[5] != 2:
        raise ValueError(f"expected big-endian ELF32: {path}")

    e_shoff = struct.unpack_from(">I", data, 32)[0]
    e_shentsize = struct.unpack_from(">H", data, 46)[0]
    e_shnum = struct.unpack_from(">H", data, 48)[0]
    e_shstrndx = struct.unpack_from(">H", data, 50)[0]
    shstr_off = struct.unpack_from(">I", data, e_shoff + e_shstrndx * e_shentsize + 16)[0]

    sec_idx = rela_idx = None
    sec_of = {}
    for i in range(e_shnum):
        hoff = e_shoff + i * e_shentsize
        sh_name = struct.unpack_from(">I", data, hoff)[0]
        e = data.index(0, shstr_off + sh_name)
        name = data[shstr_off + sh_name : e].decode("ascii")
        sec_of[name] = i
        if name == section:
            sec_idx = i
        elif name == ".rela" + section:
            rela_idx = i
        elif name == ".symtab":
            sym_idx = i
    if sec_idx is None or rela_idx is None or sym_idx is None:
        return False
    if target_sec not in sec_of:
        return False
    tgt_idx = sec_of[target_sec]

    sym_off = struct.unpack_from(">I", data, e_shoff + sym_idx * e_shentsize + 16)[0]
    sym_size = struct.unpack_from(">I", data, e_shoff + sym_idx * e_shentsize + 20)[0]
    tgt_sym = None
    for so in range(0, sym_size, 16):
        st_value = struct.unpack_from(">I", data, sym_off + so + 4)[0]
        st_shndx = struct.unpack_from(">H", data, sym_off + so + 14)[0]
        if st_shndx == tgt_idx and st_value == target_off:
            tgt_sym = so // 16
            break
    if tgt_sym is None:
        return False

    rela_off = struct.unpack_from(">I", data, e_shoff + rela_idx * e_shentsize + 16)[0]
    rela_size = struct.unpack_from(">I", data, e_shoff + rela_idx * e_shentsize + 20)[0]
    for ro in range(0, rela_size, 12):
        r_offset = struct.unpack_from(">I", data, rela_off + ro)[0]
        if r_offset == offset:
            info = struct.unpack_from(">I", data, rela_off + ro + 4)[0]
            struct.pack_into(">I", data, rela_off + ro + 4, (tgt_sym << 8) | (info & 0xFF))
            path.write_bytes(data)
            return True
    return False


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
    if rules.trim_sdata2_size is not None:
        changed = trim_sdata2_section(path, rules.trim_sdata2_size) or changed
    changed = rename_pool_symbols(path, rules.pool_patterns) or changed
    changed = rename_exact(path, rules.exact_renames) or changed
    changed = rename_by_prefix(path, rules.prefix_renames) or changed
    changed = patch_symbol_sizes(path, rules.symbol_sizes) or changed
    # A second objcopy pass for exact renames can uniquify a pool symbol when
    # the retail name also exists in another section. Re-apply content-based
    # pool naming last so @N numbering never becomes part of a unit rule.
    changed = rename_pool_symbols(path, rules.pool_patterns) or changed
    for sec, off, sym_name in rules.retarget_relocs:
        changed = retarget_reloc_to_symbol(path, sec, off, sym_name) or changed
    for sec, off, tsec, toff in rules.retarget_relocs_local:
        changed = retarget_reloc_to_local(path, sec, off, tsec, toff) or changed
    for sec, start, end in rules.zero_data_range:
        changed = zero_data_range(path, sec, start, end) or changed
    for sec, start, end in rules.drop_data_range:
        changed = drop_data_range(path, sec, start, end) or changed
    for sec, keep in rules.drop_data_tail:
        changed = drop_data_tail(path, sec, keep) or changed
    if rules.drop_text_symbols or rules.drop_text_symbols_as_undef:
        changed = (
            drop_text_symbols(
                path,
                rules.drop_text_symbols + rules.drop_text_symbols_as_undef,
                as_undef=rules.drop_text_symbols_as_undef,
            )
            or changed
        )
    if rules.repack_after_drop:
        changed = repack_text(path, rules.repack_after_drop) or changed
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
