#!/usr/bin/env python3
"""Post-process MWCC objects for PLAN.md §17.6 relocation name drift.

Renames TU-local `@N` float/double pools (and selected other linker symbols) to
retail `lbl_eu_*` names when instruction bytes already match. See
docs/MWCC_PATTERNS.md

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
    # Zero the size of a NOBITS section (.bss/.sbss) and UNDEF its symbols:
    # the retail data slice owns the zero-init objects while the code TU keeps
    # the auto-__sinit_ in .text (code_80296898 / CCol4). Pair with
    # retarget_relocs so .text references resolve to the external copy.
    zero_nobits: tuple[str, ...] = ()
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
    # Remove a byte range from a NOBITS section (.bss/.sbss), shrinking
    # sh_size and shifting later symbols. MWCC 8-aligns .sbss statics that
    # the retail linker packs at 4-byte boundaries, leaving anonymous
    # padding between objects; NOBITS sections have no file bytes to splice
    # (drop_data_range cannot handle them).
    drop_nobits_range: tuple[tuple[str, int, int], ...] = ()
    # Zero-pad a FILE-BACKED data section up to target_size. Retail split
    # objects carry linker alignment padding after the last symbol (RTTI
    # name pools / string tails) that MWCC does not emit; the data gate
    # compares section size + bytes, so the pad restores the retail section
    # exactly. Only grows; drops use drop_data_tail/drop_data_range.
    pad_data_section: tuple[tuple[str, int], ...] = ()
    # Set sh_addralign on named sections. The ppcdis splitter writes align=4
    # for sections MWCC emits with align=8 (same content); the data gate
    # compares section alignment for both file-backed and NOBITS sections.
    set_data_align: tuple[tuple[str, int], ...] = ()
    # Strip whole data sections to size 0 and convert their symbols to UNDEF
    # ((section, ...)); the retail split object has no data (it lives in the
    # shared retail data objects), so the decompiled TU must emit none. Runs
    # after all renames so .text relocs keep the retail symbol names.
    extern_data_sections: tuple[str, ...] = ()
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
    # Swap two equal-size blocks within a FILE-BACKED data section (retail's
    # interleaved vtable/typeinfo emission vs MWCC's globals-then-vtables).
    # Move relocs with the bytes: r_offsets in [o1,o1+size) shift to o2; in
    # [o2,o2+size) shift to o1.
    swap_data_blocks: tuple[tuple[str, int, int, int], ...] = ()
    # Define NEW local symbols at (symbol, section, offset, size): base-list /
    # subobject labels inside an existing data block that C++ cannot express
    # (e.g. CTaskManager lbl_eu_8056B5A4 = .data+0x48). The .sdata locator's
    # UNDEF reloc then resolves at link; the data gate sees the retail name.
    add_symbols: tuple[tuple[str, str, int, int], ...] = ()
    # Adjust r_addend on relocs at (section, offset, delta): jumptable/switch
    # case-label addends inside residual functions whose code length differs
    # from retail (wkStandby family; the function is a documented §17.6 code
    # residual — behavior tests cover semantics).
    addend_patches: tuple[tuple[str, int, int], ...] = ()
    # Inject a new SHT_RELA entry at (section, offset) pointing at an existing
    # symbol (retail vtable slots MWCC zeroes when -RTTI off suppresses the
    # typeinfo word, e.g. CChildListNode __vt__34TChildListHeader<...>).
    inject_relocs: tuple[tuple[str, int, str], ...] = ()
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
        # Blob dissolve: source defines the full 132B .data (vtable array +
        # updateMsg jumptable); MWCC's auto-emitted CRootProc + CTTask<...>
        # vtables append at +0x84 (retail linker GC'd them — drop). The three
        # CTTask<...> slots cannot be spelled in C++ ('<'-names): inject the
        # relocs at +0x2C/+0x3C/+0x40. lbl_eu_8056B5A4 = the base-list label at
        # +0x48 (referenced by the .sdata locator) — add as a local symbol.
        drop_data_range=((".data", 0x84, 0xCC),),
        inject_relocs=(
            (".data", 0x2C, "__dt__48CTTask<Q226@unnamed@CTaskManager_cpp@9CRootProc>Fv"),
            (".data", 0x3C, "Move__48CTTask<Q226@unnamed@CTaskManager_cpp@9CRootProc>Fv"),
            (".data", 0x40, "Draw__48CTTask<Q226@unnamed@CTaskManager_cpp@9CRootProc>Fv"),
        ),
        add_symbols=(("lbl_eu_8056B5A4", ".data", 0x48, 0x18),),
        # updateMsg jumptable: MWCC drops the +addend form in const initializers,
        # so the source emits plain addresses and the retail case-label addends
        # are patched here.
        addend_patches=((".data", 100, 312), (".data", 104, 1120), (".data", 108, 1696), (".data", 112, 1764), (".data", 116, 1780), (".data", 120, 1796), (".data", 124, 1812), (".data", 128, 1824),),
        set_data_align=((".data", 4),),
        exact_renames=(
            ("__RTTI__Q226@unnamed@CTaskManager_cpp@9CRootProc", "lbl_eu_80663568"),
            ("__RTTI__48CTTask<Q226@unnamed@CTaskManager_cpp@9CRootProc>", "lbl_eu_80663570"),
            ("@8851", "lbl_eu_80522588"),
            ("@8853", "lbl_eu_805225B0"),
        ),
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
        # The retail CfPadTask.o carries NO data: vtables/RTTI live in split1.s
        # (lbl_eu_80533C90/80533D08, RTTI descriptors, statics). The class
        # statics are already extern'd in source; strip whatever MWCC still
        # emits (vtable blocks, RTTI descriptors/name strings, pools) and let
        # the .text relocs resolve to split1.o at link.
        extern_data_sections=(".data", ".rodata", ".sdata", ".sdata2", ".bss", ".sbss"),
    ),
    "CDeviceRemotePad.o": UnitRules(
        # Retail CDeviceRemotePad.o carries NO data: the vtables (CDeviceRemotePad
        # @lbl_eu_8056BBC0, CDeviceBase @__vt__11CDeviceBase) and the RTTI
        # descriptors/typeinfo names live in monolibdata1d.s / monolibdata2.s.
        # MWCC still emits vtable/RTTI/typeinfo blocks here (the ctor/dtor keep
        # their implicit vptr stores, which match retail byte-for-byte, so the
        # class must stay non-novtable). Strip whatever MWCC emits and let the
        # .text relocs resolve to the retail data objects at link (same pattern
        # as CfPadTask.o / g3d_basic.o).
        extern_data_sections=(".data", ".rodata", ".sdata", ".sdata2", ".bss", ".sbss"),
    ),
    "CfCollCircleImpl.o": UnitRules(
        # The u32->f32 conversions pool the 2^52 magic double; retail references
        # it as lbl_eu_80666920 (nw4r split .sdata2). Rename then strip: the
        # retail object carries no data.
        pool_patterns=(
            (struct.pack(">II", 0x43300000, 0x00000000), "lbl_eu_80666920"),  # 2^52
        ),
        extern_data_sections=(".data", ".rodata", ".sdata", ".sdata2", ".bss", ".sbss"),
    ),
    "CNandData.o": UnitRules(
        # The retail split object carries NO data: the singleton + its 0x10
        # __register_global_object boundary structure live in split1.s .bss
        # (lbl_eu_80577358 / lbl_eu_80577348). Rename then strip; the .text
        # relocs resolve to the retail data object at link.
        exact_renames=(("sInstance__9CNandData", "lbl_eu_80577358"),),
        prefix_renames=(("@#", "lbl_eu_80577348"),),
        extern_data_sections=(".data", ".rodata", ".sdata", ".sdata2", ".bss", ".sbss"),
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
        # Retail split object carries no data at all; the u32->f32 magic pool
        # ships from the nw4r data unit (lbl_eu_80666920).
        extern_data_sections=(".data", ".rodata", ".sdata", ".sdata2", ".bss", ".sbss"),
    ),
    "CDrawGX.o": UnitRules(
        # Retail split object carries no data at all: the begin()/setPrimType()
        # switch jumptables ship from monolibdata2 (jumptable_eu_8056CCB4 /
        # jumptable_eu_8056CC88) and the float/color constants from its .sdata2
        # pool (lbl_eu_8066A468/46C/470/474/478). MWCC emits all of it locally;
        # retarget the jumptable relocs, rename the pool symbols by content,
        # then strip the sections (relocs resolve to the retail data object at
        # link, mirroring the DOL-extracted retail .o).
        retarget_relocs=(
            (".text", 0x4DE, "jumptable_eu_8056CCB4"),
            (".text", 0x4E6, "jumptable_eu_8056CCB4"),
            (".text", 0x5C2, "jumptable_eu_8056CC88"),
            (".text", 0x5CA, "jumptable_eu_8056CC88"),
        ),
        pool_patterns=(
            (struct.pack(">I", 0x3F800000), "lbl_eu_8066A468"),              # 1.0f
            (struct.pack(">I", 0x00000000), "lbl_eu_8066A46C"),              # 0.0f
            (struct.pack(">I", 0x437F0000), "lbl_eu_8066A470"),              # 255.0f
            (struct.pack(">I", 0x4222F983), "lbl_eu_8066A474"),              # 128/pi
            (struct.pack(">II", MAGIC_HI, MAGIC_LO), "lbl_eu_8066A478"),      # int->f64 magic
        ),
        extern_data_sections=(".data", ".sdata2"),
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
    ),
    "g3d_resfile.o": UnitRules(
        # Retail g3d_resfile.o carries NO data: the ResName pascal-string data
        # for the file resource groups lives in nw4r_data.s (lbl_eu_80568F60..
        # 805690A0). The definitions stay in source so MWCC folds adjacent
        # references into base+offset immediates (CheckRevision's ten
        # iterations are `addi rX, base, ofs` on a single base); strip the
        # emitted .data and UNDEF the symbols so the .text relocs resolve to
        # nw4r_data.o at link (CfPadTask.o pattern).
        extern_data_sections=(".data", ".rodata", ".sdata", ".sdata2", ".bss", ".sbss"),
        # Retail emits no GetResPlttNumEntries/GetResTexNumEntries/…, no u32
        # overloads, and no GetResAnmVis*/GetResAnmShp* symbols: the only
        # callers are Bind/Init/Terminate/CheckRevision, where -ipa file
        # inlines these bodies transitively (u32 overloads first); the retail
        # linker dead-stripped the standalone copies.
        drop_text_symbols=(
            "GetResMdl__Q34nw4r3g3d7ResFileCFUl",
            "GetResPltt__Q34nw4r3g3d7ResFileCFUl",
            "GetResPlttNumEntries__Q34nw4r3g3d7ResFileCFv",
            "GetResTex__Q34nw4r3g3d7ResFileCFUl",
            "GetResTexNumEntries__Q34nw4r3g3d7ResFileCFv",
            "GetResAnmChr__Q34nw4r3g3d7ResFileCFUl",
            "GetResAnmVis__Q34nw4r3g3d7ResFileCFUl",
            "GetResAnmVisNumEntries__Q34nw4r3g3d7ResFileCFv",
            "GetResAnmVis__Q34nw4r3g3d7ResFileCFi",
            "GetResAnmClr__Q34nw4r3g3d7ResFileCFUl",
            "GetResAnmClrNumEntries__Q34nw4r3g3d7ResFileCFv",
            "GetResAnmTexPat__Q34nw4r3g3d7ResFileCFUl",
            "GetResAnmTexPatNumEntries__Q34nw4r3g3d7ResFileCFv",
            "GetResAnmTexSrt__Q34nw4r3g3d7ResFileCFUl",
            "GetResAnmTexSrtNumEntries__Q34nw4r3g3d7ResFileCFv",
            "GetResAnmShp__Q34nw4r3g3d7ResFileCFUl",
            "GetResAnmShpNumEntries__Q34nw4r3g3d7ResFileCFv",
            "GetResAnmShp__Q34nw4r3g3d7ResFileCFi",
            "GetResAnmScn__Q34nw4r3g3d7ResFileCFUl",
            "GetResAnmScnNumEntries__Q34nw4r3g3d7ResFileCFv",
        ),
    ),
    "g3d_basic.o": UnitRules(
        # Pooled DEG_TO_FIDX constant (256/360) + 0.0f + 1.0f from inlined
        # header code; retail references lbl_eu_80669C88/8C/90 (nw4r_data.s
        # .sdata2). Rename then strip: the retail object carries no data.
        pool_patterns=(
            (struct.pack(">I", 0x3F360B61), "lbl_eu_80669C88"),  # 0.71111113f
            (struct.pack(">I", 0x00000000), "lbl_eu_80669C8C"),  # 0.0f
            (struct.pack(">I", 0x3F800000), "lbl_eu_80669C90"),  # 1.0f
        ),
        extern_data_sections=(".data", ".rodata", ".sdata", ".sdata2", ".bss", ".sbss"),
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
        # reslist tail: swap [lbl_eu_8056B28C][__vt__11reslist<Ul>] (MWCC order)
        # -> retail [__vt__11reslist<Ul>][lbl_eu_8056B28C], then inject the
        # -RTTI-off-suppressed typeinfo words at the two vtable heads.
        swap_data_blocks=((".data", 0xA0, 0xAC, 0xC),),
        inject_relocs=((".data", 0xA0, "lbl_eu_80663538"), (".data", 0xB8, "lbl_eu_80663540"),),
        pad_data_section=((".data", 0xC8),),  # retail .data range 0x8056B1E0-0x8056B2A8; MWCC emits 0xC4 (4B splitter align pad)
        pool_patterns=(
            (struct.pack(">II", MAGIC_HI, MAGIC_LO), "lbl_eu_8066A280"),
        ),
        exact_renames=(
            ("@stringBase0", "lbl_eu_80522500"),
            ("__vt__5CProc", "lbl_eu_8056B1E0"),
            ("__vt__17_reslist_base<Ul>", "lbl_eu_8056B298"),
            ("__vt__11reslist<Ul>", "lbl_eu_8056B280"),
        ),
        # Retail split carries no .sdata2: the pssCreateView magic double
        # ships from monolibdata2 (lbl_eu_8066A280). Rename the pool above,
        # then strip the section so the .text SDA21 reloc resolves to the
        # external copy at link (CfPadTask.o pattern).
        extern_data_sections=(".sdata2",),
        # Retail .text ends after wkStandbyLogout; drop weak IWorkEvent/CWorkThread stubs.
        trim_text_size=0xB1C,
        # pssCreateView: exact-size -0xF0 / FixStr@0x68 / 0x3AC body after workId hoist.
        # Remaining soft-cap is EH home (stw r1@0x1c vs @0x64), walk homes +0x18, and
        # Chaitin r5/r8 plus float-schedule cascade. Same class as CViewRoot::setCurrent.
        # PLAN.md section 17.6; behaviour:proc-pss-create-view covers semantics.
    ),
    "CView.o": UnitRules(
        # CView ctor float stores: 1.0f / 0.6f pool @N → retail sdata2 labels.
        # .sdata RTTI structs: MWCC emits [CView][CFontLayer][CMsgParam]
        # [reslist{name,base}][_reslist_base] but retail rotates to [CView]
        # [CMsgParam][reslist][_reslist_base][CFontLayer] (three 8B swaps),
        # names the pool strings directly (@9115/16/18 = CMsgParam/reslist/
        # _reslist_base names; @9117 = the reslist cast-base struct in .data).
        swap_data_blocks=(
            (".sdata", 0x08, 0x10, 0x8),
            (".sdata", 0x10, 0x18, 0x8),
            (".sdata", 0x18, 0x20, 0x8),
        ),
        # Retarget the auto-pooled RTTI name/base relocs (PRE-swap offsets)
        # onto the source-defined named consts (retail labels): CMsgParam<10>
        # @0x10, reslist<IWorkEvent *> @0x18 + cast-base @0x1C, _reslist_base
        # @0x20. The named copies are FORCEACTIVE in source; the pooled
        # duplicates at .rodata 0x70+ are dropped below.
        retarget_relocs=(
            (".sdata", 0x10, "lbl_eu_805225E0"),
            (".sdata", 0x18, "lbl_eu_805225F0"),
            (".sdata", 0x1C, "lbl_eu_8056B6E4"),
            (".sdata", 0x20, "lbl_eu_80522608"),
        ),
        exact_renames=(
            ("__RTTI__13CMsgParam<10>", "lbl_eu_80663580"),
            ("__RTTI__22reslist<P10IWorkEvent>", "lbl_eu_80663588"),
            ("__RTTI__28_reslist_base<P10IWorkEvent>", "lbl_eu_80663590"),
            ("__vt__5CView", "lbl_eu_8056B5E0"),
            ("__ct__10CFontLayerFv", "__ct__CFontLayer"),
        ),
        pad_data_section=((".data", 0x120),),  # retail .data range 0x8056B5E0-0x8056B700; MWCC emits 0x11C (4B align tail)
        drop_data_tail=((".rodata", 0x70),),  # pooled string copies exceed retail 0x805225E0-0x80522650 by 4B
        pool_patterns=(
            (struct.pack(">I", 0x3F800000), "lbl_eu_8066A2D0"),  # 1.0f
            (struct.pack(">I", 0x3F19999A), "lbl_eu_8066A2D4"),  # 0.6f
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
    "CMat34.o": UnitRules(
        # Retail CMat34.o carries NO data (.text + .bss only; the .bss 0x60
        # zero/identity statics already match). The warning strings (AcosRad/
        # AsinRad/FSqrt "Input is out of the domain." + source filenames) are
        # pooled by the inlined math_triangular.h / math_arithmetic.h bodies,
        # and the float literals (0.0/0.5/1.0/-1.0/0.0234375) pool into
        # .sdata2; both ship from monolibdata*.s / nw4r_data.s in retail.
        # Rename the @STRING@/pool/extern symbols to the retail labels, then
        # strip .data/.sdata2 so the .text relocs resolve at link.
        pool_patterns=(
            (struct.pack(">I", 0x3F000000), "lbl_eu_8066A244"),  # 0.5f
            (struct.pack(">I", 0x3F800000), "lbl_eu_8066A248"),  # 1.0f
            (struct.pack(">I", 0xBF800000), "lbl_eu_8066A24C"),  # -1.0f
            (struct.pack(">I", 0x00000000), "lbl_eu_8066A240"),  # 0.0f
            (struct.pack(">I", 0x3CC90FDB), "lbl_eu_8066A250"),  # 0.0234375f
        ),
        exact_renames=(
            # Warning() string pool: message (…@0) + source-filename entries.
            ("@STRING@AcosRad__Q24nw4r4mathFf", "lbl_eu_805262F0"),
            ("@STRING@AcosRad__Q24nw4r4mathFf@0", "lbl_eu_805262C8"),
            ("@STRING@AsinRad__Q24nw4r4mathFf", "lbl_eu_8052ADB0"),
            ("@STRING@AsinRad__Q24nw4r4mathFf@0", "lbl_eu_8052AD88"),
            ("@STRING@FSqrt__Q24nw4r4mathFf", "lbl_eu_80526324"),
            ("@STRING@FSqrt__Q24nw4r4mathFf@0", "lbl_eu_80526300"),
            # math externs referenced under TU-local names.
            ("pi__2ml", "lbl_eu_8066A1F8"),
            ("epsilon__2ml", "lbl_eu_8066A208"),
            ("halfpi__2ml", "lbl_eu_8066A200"),
        ),
        extern_data_sections=(".data", ".sdata2"),
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
    "CViewRoot.o": UnitRules(
        # Retail GC'd the reslist<Ul> member data: MWCC appends the 12B
        # _reslist_base<Ul> vtable to .data (+0xB8), its "reslist<unsigned
        # long>" name string to .rodata (+0x18), and the @N RTTI struct to
        # .sdata (+0x8). Retail keeps only the CViewRoot vtable / name /
        # typeinfo; drop the tails (the reslist methods stay in .text).
        drop_data_range=((".data", 0xB8, 0xC4), (".sdata", 0x8, 0x10),),
        drop_data_tail=((".rodata", 0x18),),
    ),
    "CWorkThread.o": UnitRules(
        # Retail strips this TU's class-static/vtable names to address labels.
        # wkStandby jumptable addends: the switch case labels sit 4-8B deeper
        # in retail (function is a documented §17.6 code residual, 12.4% — the
        # .data dispatch table entries at +0x20..+0x2C must carry the retail
        # offsets for the DOL to byte-match).
        addend_patches=((".data", 0x00, 8), (".data", 0x20, 4), (".data", 0x24, 8), (".data", 0x28, 8), (".data", 0x2C, 8),),
        exact_renames=(
            ("__vt__11CWorkThread", "lbl_eu_8056B110"),
            ("__vt__29_reslist_base<P11CWorkThread>", "lbl_eu_8056B1D4"),
            ("__vt__23reslist<P11CWorkThread>", "lbl_eu_8056B1BC"),
            ("__vt__12CMsgParam<8>", "lbl_eu_8056B1B0"),
            ("sAllocHandle__17CWorkThreadSystem", "lbl_eu_8066351C"),
            # MWCC auto-emits the template RTTI structs as weak .sdata
            # objects under '<'-names that cannot be spelled in source; the
            # retail split references the same structs by address label
            # (lbl_eu_80663520/28/30 in monolibdata1d, 8 bytes each). The
            # vtable RTTI slots (+0xD0/+0xDC/+0xE8/+0xF4) then carry the
            # retail names and the data gate passes.
            ("__RTTI__12CMsgParam<8>", "lbl_eu_80663520"),
            ("__RTTI__23reslist<P11CWorkThread>", "lbl_eu_80663528"),
            ("__RTTI__29_reslist_base<P11CWorkThread>", "lbl_eu_80663530"),
            # The RTTI name strings / typeinfo pool entries the .sdata RTTI
            # structs point at; retail references the shared labels in
            # monolibdata1 (0x80522474/84/9C = "CMsgParam<8>" /
            # "reslist<CWorkThread *>" / "_reslist_base<CWorkThread *>",
            # 0x8056B1C8 = the 0xC reslist typeinfo struct).
            ("@2164", "lbl_eu_80522474"),
            ("@2165", "lbl_eu_80522484"),
            ("@2166", "lbl_eu_8056B1C8"),
            ("@2167", "lbl_eu_8052249C"),
        ),
        # MWCC 8-aligns the first RTTI struct after the 4-byte
        # lbl_eu_8066351C sentinel, leaving a 4-byte pad; the retail linker
        # packs the structs at +4/+0xC/+0x14. Drop the pad (and the same
        # 4-byte pad in .sbss before lbl_eu_80665598) so sizes/offsets match
        # the retail split, and write the splitter's align=4 convention.
        drop_data_range=((".sdata", 0x4, 0x8),),
        drop_nobits_range=((".sbss", 0x4, 0x8),),
        pad_data_section=((".rodata", 0x4C),),
        set_data_align=((".rodata", 4), (".sdata", 4), (".sbss", 4)),
    ),
    "CProcess.o": UnitRules(
        # MWCC auto-emits a weak duplicate RTTI struct
        # (__RTTI__27TChildListHeader<8CProcess>, '<'-name unspellable in
        # source) in .sdata +8, plus a second copy of the
        # "TChildListHeader<CProcess>" name string, that the retail linker
        # GC'd: retail keeps the strong lbl_eu_806635D8 struct (the vtable
        # RTTI slot references it) and one pooled name string. Retarget the
        # vtable slot to the strong struct, drop the weak struct + its
        # .sdata range, and drop the duplicate string, then restore the
        # retail .rodata tail padding (0x1B + pad -> 0x20).
        retarget_relocs=((".data", 0x24, "lbl_eu_806635D8"),),
        exact_renames=(
            ("__vt__27TChildListHeader<8CProcess>", "lbl_eu_8056BB84"),
        ),
        drop_data_range=((".sdata", 0x8, 0x10),),
        drop_data_tail=((".rodata", 0x1B),),
        pad_data_section=((".rodata", 0x20),),
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
        # novtable removed: MWCC auto-emits the CProcRoot vtable/RTTI/RTTI name
        # and the pooled "CDesktop\0CProcRoot" literal block.
        exact_renames=(
            ("__vt__9CProcRoot", "lbl_eu_8056B2A8"),
            ("__RTTI__9CProcRoot", "lbl_eu_80663548"),
            ("@stringBase0", "lbl_eu_80522514"),
        ),
    ),
    "CWorkRoot.o": UnitRules(
        # Retail GC'd the reslist<P11CWorkThread> member data (the strong copies
        # live in CWorkThread.o): MWCC appends the reslist + _reslist_base
        # vtables to .data (+0xB8, 0x24), their two name strings to .rodata
        # (+0x78, 0x42), and the RTTI structs to .sdata (+0x8, 0x10), plus an
        # 8-align pad in .sbss. Drop all of it; the reslist methods stay in
        # .text and resolve to the CWorkThread copies at link.
        drop_data_range=((".data", 0xB8, 0xDC), (".sdata", 0x8, 0x18),),
        drop_data_tail=((".rodata", 0x78),),
        exact_renames=(
            ("__RTTI__Q223@unnamed@CWorkRoot_cpp@15CWorkRootThread", "lbl_eu_806635C0"),
            ("__dt__Q217CWorkRootThreadNS15CWorkRootThreadFv", "__dt__Q223@unnamed@CWorkRoot_cpp@15CWorkRootThreadFv"),
            ("wkStandbyLogout__Q217CWorkRootThreadNS15CWorkRootThreadFv", "wkStandbyLogout__Q223@unnamed@CWorkRoot_cpp@15CWorkRootThreadFv"),
            # Singleton static: source defines the NS-mangled symbol; retail
            # names it @unnamed@ (its .sbss slot is 0x80665608, now inside the
            # CWorkRoot range). The 13 .text refs resolve at link.
            ("spInstance__Q217CWorkRootThreadNS15CWorkRootThread", "spInstance__Q223@unnamed@CWorkRoot_cpp@15CWorkRootThread"),
        ),
    ),
    "CScriptCode.o": UnitRules(
        # Class now derives from CWorkThread with out-of-line virtuals, so MWCC
        # auto-emits the CScriptCode vtable/RTTI/RTTI name + pooled name literal.
        exact_renames=(
            ("__vt__11CScriptCode", "lbl_eu_8056B418"),
            ("__RTTI__11CScriptCode", "lbl_eu_80663558"),
            ("@stringBase0", "lbl_eu_8052254C"),
        ),
    ),
    "CRsrcData.o": UnitRules(
        # Retail .text ends after wkStandbyLogout (0x42C); drop weak IWorkEvent/CWorkThread stubs.
        trim_text_size=0x42C,
        exact_renames=(
            ("__ct__9CRsrcDataFPCcP11CWorkThread", "__ct__CRsrcData"),
            ("__vt__9CRsrcData", "lbl_eu_8056B360"),
            ("__RTTI__9CRsrcData", "lbl_eu_80663550"),
            ("@stringBase0", "lbl_eu_80522534"),
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
        # func_802B85A4 int→float pools the signed magic double; retail
        # references the named lbl_eu_80669000 from the data slice. The pool
        # entry is dropped after the rename (retail split .sdata2 is empty).
        pool_patterns=(
            (struct.pack(">II", MAGIC_HI, MAGIC_LO), "lbl_eu_80669000"),
        ),
        trim_sdata2_size=0,
    ),
    "snd_EnvGenerator.o": UnitRules(
        # Update/SetDecay/SetRelease int→double conversions pool the magic and
        # the CalcRelease inline pools its float constants; retail references
        # the named sdata2 labels (lbl_eu_80669F48/34/50/54/58/5C). Rename the
        # pool entries (source literals are byte-identical to retail codegen)
        # and drop the pool so the retail-empty .sdata2 stays empty.
        pool_patterns=(
            (struct.pack(">II", MAGIC_HI, MAGIC_LO), "lbl_eu_80669F48"),
            (struct.pack(">I", 0x477FFF00), "lbl_eu_80669F34"),  # 65535.0f
            (struct.pack(">I", 0x41C00000), "lbl_eu_80669F50"),  # 24.0f
            (struct.pack(">I", 0x3C000000), "lbl_eu_80669F54"),  # 1/128
            (struct.pack(">I", 0x40A00000), "lbl_eu_80669F58"),  # 5.0f
            (struct.pack(">I", 0x42700000), "lbl_eu_80669F5C"),  # 60.0f
        ),
        trim_sdata2_size=0,
    ),
    "snd_Lfo.o": UnitRules(
        # Retail references the named sdata2 magic doubles (lbl_eu_80669FC0 /
        # lbl_eu_80669FD8) for its int→float conversions; MWCC pools them
        # locally. Retarget the lfd relocs (introduced by the s32/u8→f32 casts)
        # to the retail labels and drop the pool so the retail-empty .sdata2
        # stays empty.
        retarget_relocs=(
            (".text", 0x78, "lbl_eu_80669FC0"),   # Update: msec (s32) -> f32
            (".text", 0x1B4, "lbl_eu_80669FC0"),  # GetValue: sin idx (s8) -> f32
            (".text", 0x1D4, "lbl_eu_80669FD8"),  # GetValue: range (u8) -> f32
        ),
        drop_data_tail=((".sdata2", 0x0),),
    ),
    "snd_MmlSeqTrackAllocator.o": UnitRules(
        # The class vtable ships from the retail data unit (nw4r_data.s); the
        # code TU must emit none. No .text reloc references it, so drop the
        # local vtable outright.
        drop_data_range=((".data", 0x0, 0x18),),
    ),
    "ut_DvdFileStream.o": UnitRules(
        # Retail keeps only the static initializer here; DvdFileStream::typeInfo
        # ships from monolibdata1 (lbl_eu_80665550). The sinit's stw reloc must
        # point at the retail label and the local .sbss must be stripped.
        retarget_relocs=((".text", 0x4, "lbl_eu_80665550"),),
        extern_data_sections=(".data", ".rodata", ".sdata", ".sdata2", ".bss", ".sbss"),
    ),
    "CMCEffStart.o": UnitRules(
        # Vtable/RTTI/typeinfo live in the retail data slice (lbl_eu_805360xx);
        # the code TU must emit none. Retarget each __vt__* reloc to the retail
        # name (UNDEF) and drop the .data/.rodata/.sdata sections.
        retarget_relocs=(
            (".text", 0x2, "lbl_eu_80536114"),
            (".text", 0x26A, "lbl_eu_805360F4"),
            (".text", 0x39E, "lbl_eu_805360D4"),
            (".text", 0x4D2, "lbl_eu_805360B4"),
            (".text", 0x5EE, "lbl_eu_805360A8"),
            (".text", 0x9AE, "lbl_eu_80536098"),
            (".text", 0xE1A, "lbl_eu_80536078"),
            (".text", 0xF0E, "lbl_eu_80536068"),
            (".text", 0x1D72, "lbl_eu_80536048"),
            (".text", 0x1EA6, "lbl_eu_80536028"),
            (".text", 0x1FC2, "lbl_eu_80536018"),
        ),
        drop_data_tail=(
            (".data", 0),
            (".rodata", 0),
            (".sdata", 0),
        ),
        # updateOut's SetFrame(GetFrameSize() - 1) int→float pools the magic
        # and activateSlots' SetFrame(0.0f) pools 0.0f (hoisted into f31,
        # retail shape); retail references lbl_eu_80668558 / lbl_eu_80668550
        # by name (1.0f/5.0f are named externs in source).
        pool_patterns=(
            (struct.pack(">II", MAGIC_HI, MAGIC_LO), "lbl_eu_80668558"),
            (struct.pack(">I", 0x00000000), "lbl_eu_80668550"),
        ),
        trim_sdata2_size=0,
    ),
    "CArcItem.o": UnitRules(
        # Vtable (lbl_eu_8056FFE0) + RTTI/typeinfo live in the retail data
        # slice; the code TU must emit none. Retarget the vtable relocs to the
        # retail name and drop .data/.rodata/.sdata.
        retarget_relocs=(
            (".text", 0xA, "lbl_eu_8056FFE0"),
        ),
        drop_data_tail=(
            (".data", 0),
            (".rodata", 0),
            (".sdata", 0),
        ),
        # The inline-empty ~IWorkEvent (IWORK_EVENT_INLINE_DTOR) is still
        # emitted as a weak 0x40 copy between the ctor and dtor; retail keeps
        # the strong copy in CTaskGame.o, so drop it and re-pack the survivors
        # at 4-byte alignment (the drop leaves MWCC's 4-byte pre-weak pad).
        drop_text_symbols=("__dt__10IWorkEventFv",),
        repack_after_drop=4,
    ),
    "CScnItemCameraNw4r.o": UnitRules(
        # Vtable (lbl_eu_8056DC90) + RTTI/typeinfo live in the retail data
        # slice; the code TU must emit none.
        retarget_relocs=(
            (".text", 0x1A, "lbl_eu_8056DC90"),
        ),
        drop_data_tail=(
            (".data", 0),
            (".rodata", 0),
            (".sdata", 0),
        ),
    ),
    "dw_Window.o": UnitRules(
        # Retail dw_Window.o holds ONLY the inline SetTextColor copy (0x44);
        # the source's forced-odr-use dummy + its Color dtor are MWCC-emitted
        # extras. Drop them and re-pack SetTextColor to offset 0.
        drop_text_symbols=(
            "dummy__Q24nw4r2dwFPQ34nw4r2ut10CharWriter",
            "__dt__Q34nw4r2ut5ColorFv",
        ),
        repack_after_drop=4,
    ),
    "CChildListNode.o": UnitRules(
        # Retail split carries only __ct__ + Reset (0x8C); the weak base
        # dtor __dt__CDoubleListNode (0x40) and the weak template dtor
        # __dt__TChildListHeader<CChildListNode> (0x40) are linker-GC'd
        # (retail defines both strongly in CProcess.o — the weak copies here
        # are dead weight). The template dtor is referenced by the .data
        # TChildListHeader vtable slot, so it is dropped as UNDEF (resolves
        # to the strong CProcess.o copy at link, mirroring the
        # DOL-extracted retail .o); the orphan base dtor has no live refs.
        drop_text_symbols=("__dt__15CDoubleListNodeFv",),
        drop_text_symbols_as_undef=("__dt__34TChildListHeader<14CChildListNode>Fv",),
        # The retail split NOW carries .data (vtable lbl_eu_8056BBA0 at +0
        # and the TChildListHeader template vtable lbl_eu_8056BBB0 at +0x10)
        # after the monolibdata1/1d blob-split surgery, so this TU must emit
        # it. MWCC auto-emits the template vtable under a '<'-name that
        # cannot be spelled in source; rename it to the retail label. Tier A:
        # the template vtable must be 16 bytes with the RTTI slot (retail
        # has a reloc to lbl_eu_806635E0 at +0x10; MWCC currently emits 12
        # bytes without it) so .data reaches the retail 0x20.
        # Retail's 4th word (+0x1C) is a trailing NULL: pad the section tail.
        pad_data_section=((".data", 0x20),),
        # -RTTI off suppresses the vtable's typeinfo word (MWCC emits 0):
        # inject the R_PPC_ADDR32 at +0x10 -> retail lbl_eu_806635E0 slot.
        inject_relocs=((".data", 0x10, "lbl_eu_806635E0"),),
        exact_renames=(
            ("__vt__34TChildListHeader<14CChildListNode>", "lbl_eu_8056BBB0"),
        ),
    ),
    "CScnFrame.o": UnitRules(
        # The u32->f32 conversion magic double (2^52) is compiler-generated
        # and cannot be named from source (the update() (f32)tick cast);
        # retail references the shared monolibdata2 label lbl_eu_8066ABE8
        # (the source's extern declaration is unused). Rename the TU-local
        # pool by content and strip .sdata2.
        pool_patterns=(
            (struct.pack(">II", MAGIC_HI, 0x00000000), "lbl_eu_8066ABE8"),  # 2^52
        ),
        extern_data_sections=(".sdata2",),
    ),
    "code_80296898.o": UnitRules(
        # lbl_eu_805772C8 (0x40 bss singleton) lives in the retail data slice;
        # the __sinit_ in this TU must stay (retail .text keeps it), so the
        # definition is kept and the .bss is zeroed with relocs retargeted to
        # the external copy.
        retarget_relocs=(
            (".text", 0x6A, "lbl_eu_805772C8"),
        ),
        zero_nobits=(".bss",),
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
        # (0x40 deleting wrapper) with the DrawInfo code; ~DrawInfo inlines
        # the trivial member destruction, so no .text/.data reference
        # survives and the retail linker dead-stripped it
        # (no __dt__Rect anywhere in the DOL). Dropping the orphan restores
        # the retail split layout and fits the budget. nw4hbm (this rule) and
        # nw4r (added name) each emit only their own mangling.
        drop_text_symbols=(
            "__dt__Q36nw4hbm2ut4RectFv",
            "__dt__Q34nw4r2ut4RectFv",
        ),
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
        # The retail split object carries NO data: vtables/statics live in the
        # nw4r data objects; strip whatever MWCC emits here.
        extern_data_sections=(".data", ".rodata", ".sdata", ".sdata2", ".bss", ".sbss"),
    ),
    "snd_MmlSeqTrack.o": UnitRules(
        # Vtable lives in nw4r_data.s; retail references it as lbl_eu_8056AAC0.
        exact_renames=(("__vt__Q44nw4r3snd6detail11MmlSeqTrack", "lbl_eu_8056AAC0"),),
        extern_data_sections=(".data", ".rodata", ".sdata", ".sdata2", ".bss", ".sbss"),
    ),

    "snd_MemorySoundArchive.o": UnitRules(
        # Retail split carries NO data: the two vtables (MemorySoundArchive
        # @lbl_eu_8056A870, MemoryFileStream @lbl_eu_8056A810) ship from
        # nw4r_data.s. MWCC also emits the weak base dtors
        # __dt__ut::FileStream / __dt__ut::IOStream (0x40 each = the full
        # 0x80 .text overage) which only the TU-local vtables referenced —
        # the retail linker dead-stripped them. Rename the vtables, drop the
        # orphan weaks (the stripped .data vtable relocs die with them), and
        # strip .data so the .text vptr stores resolve to nw4r_data.s at link.
        exact_renames=(
            ("__vt__Q34nw4r3snd18MemorySoundArchive", "lbl_eu_8056A870"),
            ("__vt__Q44nw4r3snd18MemorySoundArchive16MemoryFileStream", "lbl_eu_8056A810"),
        ),
        drop_text_symbols=(
            "__dt__Q34nw4r2ut10FileStreamFv",
            "__dt__Q34nw4r2ut8IOStreamFv",
        ),
        extern_data_sections=(".data",),
    ),
    "db_assert.o": UnitRules(
        # Retail db_assert.o carries ONLY Warning + WarningAlarmFunc_ (0x1B4);
        # the other 12 functions were linker-GC'd from the retail DOL except
        # nw4r::db::Panic, which the retail linker placed in the CErrorWii.o
        # slice (30+ game units reference it). The decompiled TU keeps the
        # Panic chain (Panic -> VPanic -> Assertion_Printf_) so the DOL link
        # keeps resolving those references; the remaining 8 standalones are
        # pure overage (VWarning's body is inlined into Warning, the
        # Assertion_* helpers into VWarning/Warning). Warning's inlined body
        # references the statics/pools under TU-local names; rename them to
        # the retail nw4r_data labels so the .text relocs carry the retail
        # names, then strip all data sections (the retail split has none).
        exact_renames=(
            ("sAssertionConsole__Q24nw4r2db", "lbl_eu_8066543C"),
            ("sDispWarningAuto__Q24nw4r2db", "lbl_eu_80663450"),
            ("sWarningTime__Q24nw4r2db", "lbl_eu_80665438"),
            ("@2528", "lbl_eu_80663454"),  # "\n" (2-byte .sdata pool)
            ("@2561", "lbl_eu_80568F38"),  # "%s:%d Warning:" (.data pool)
        ),
        # $N static-local counters drift between builds — prefix rename.
        prefix_renames=(
            ("sWarningAlarm$", "lbl_eu_8061A4D8"),
            ("sInitializedAlarm$", "lbl_eu_80665440"),
        ),
        drop_text_symbols=(
            "VWarning__Q24nw4r2dbFPCciPCcP16__va_list_struct",
            "Log__Q34nw4r2db6detailFPCce",
            "Assertion_SetConsole__Q24nw4r2dbFPQ44nw4r2db6detail11ConsoleHead",
            "Assertion_GetConsole__Q24nw4r2dbFv",
            "Assertion_ShowConsole__Q24nw4r2dbFUl",
            "Assertion_HideConsole__Q24nw4r2dbFv",
            "Assertion_SetWarningTime__Q24nw4r2dbFUl",
            "Assertion_SetAutoWarning__Q24nw4r2dbFb",
        ),
        repack_after_drop=4,
        extern_data_sections=(".data", ".sdata", ".bss", ".sbss"),
    ),
    "db_DbgPrintBase.o": UnitRules(
        # Retail db_DbgPrintBase.o holds ONLY the inline SetTextColor copy
        # (0x24); the source's forced-odr-use dummy (0x2C, required to ODR-use
        # SetTextColor from this TU) and its orphan weak Color dtor (0x40) are
        # MWCC-emitted extras (dw_Window pattern). Nothing references either,
        # so dropping both lands SetTextColor at offset 0 exactly (0x90 ->
        # 0x24) with zero padding residue.
        drop_text_symbols=(
            "dummy__Q24nw4r2dbFPQ34nw4r2ut10CharWriter",
            "__dt__Q34nw4r2ut5ColorFv",
        ),
    ),
    "lyt_init.o": UnitRules(
        # NW4R_LIB_VERSION struct + pointer live in the nw4r data objects;
        # retail references the pointer as lbl_eu_806634B8.
        exact_renames=(("NW4R_LYT_Version___22@unnamed@lyt_init_cpp@", "lbl_eu_806634B8"),),
        extern_data_sections=(".data", ".rodata", ".sdata", ".sdata2", ".bss", ".sbss"),
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
    "snd_BiquadFilterPreset.o": UnitRules(
        # Retail split object carries NO data: the f2i magic double
        # (0x43300000,0x80000000) and the 1.0f literal pool in the nw4r
        # .sdata2 blob (lbl_eu_80669EF8 / lbl_eu_80669F00). The magic is
        # compiler-generated by the (int) cast lowering in GetCoef; the 1.0f
        # comes from the (1.0f - freq) literal. Rename the TU-local pool
        # symbols by content, then strip the section (relocs resolve to the
        # retail data object at link — identical .text reloc offsets).
        pool_patterns=(
            (struct.pack(">II", MAGIC_HI, MAGIC_LO), "lbl_eu_80669EF8"),  # f2i magic double
            (struct.pack(">I", 0x3F800000), "lbl_eu_80669F00"),            # 1.0f
        ),
        extern_data_sections=(".sdata2",),
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
    "snd_TaskManager.o": UnitRules(
        # Retail emits the LinkList<Task,4> ctor under the lowercase template
        # mangling; rename the byte-identical MWCC copy.
        exact_renames=(
            (
                "__ct__Q34nw4r2ut35LinkList<Q44nw4r3snd6detail4Task,4>Fv",
                "__ct__q34nw4r2ut35linklist<Q44nw4r3snd6detail4Task,4>Fv",
            ),
        ),
        # GetInstance's function-local static: retail references the guard +
        # instance + register_global_object cookie as flat address labels in
        # the nw4r data objects (same scheme as snd_SoundPlayer).
        prefix_renames=(
            ("@GUARD@instance$", "lbl_eu_80665528"),
            ("instance$", "lbl_eu_80653E24"),
            ("@#", "lbl_eu_80653E18"),
        ),
        # Orphan weak dtors the retail linker dead-stripped.
        drop_text_symbols=(
            "__dt__Q34nw4r2ut12LinkListNodeFv",
            "__dt__Q44nw4r2ut29@unnamed@snd_TaskManager_cpp@11NonCopyableFv",
            "__dt__Q34nw4r2ut17AutoInterruptLockFv",
        ),
        # The retail split object carries no data (statics live in the nw4r
        # data objects); strip what MWCC emits here.
        extern_data_sections=(".data", ".rodata", ".sdata", ".sdata2", ".bss", ".sbss"),
    ),

    "snd_Util.o": UnitRules(
        # The retail split object carries NO data: the const tables live in
        # nw4r_data.s .rodata / .data under flat address labels. Rename the
        # TU's (properly typed) static const definitions to those labels, then
        # strip so the definitions resolve to the retail data object at link.
        exact_renames=(
            ("NoteTable__Q44nw4r3snd6detail4Util", "lbl_eu_8051FF90"),
            ("PitchTable__Q44nw4r3snd6detail4Util", "lbl_eu_8051FFC0"),
            ("Decibel2RatioTable__Q44nw4r3snd6detail4Util", "lbl_eu_805203C0"),
            ("RemoteFilterCoefTable__Q44nw4r3snd6detail4Util", "lbl_eu_80521EE0"),
            ("PanTableTable__Q44nw4r3snd6detail4Util", "lbl_eu_8056AD60"),
        ),
        # CalcRandom's LCG state (function-local static).
        prefix_renames=(("u$", "lbl_eu_806634F0"),),
        pool_patterns=(
            (struct.pack(">I", 0x3F800000), "lbl_eu_8066A0D0"),  # 1.0f
            (struct.pack(">I", 0x40000000), "lbl_eu_8066A0D4"),  # 2.0f
            (struct.pack(">I", 0x3F000000), "lbl_eu_8066A0D8"),  # 0.5f
            (struct.pack(">I", 0x40C00000), "lbl_eu_8066A0DC"),  # 6.0f
            (struct.pack(">I", 0xC2B4CCCD), "lbl_eu_8066A0E0"),  # -90.4f
            (struct.pack(">I", 0x41200000), "lbl_eu_8066A0E4"),  # 10.0f
            (struct.pack(">I", 0xBF800000), "lbl_eu_8066A0E8"),  # -1.0f
            (struct.pack(">I", 0x43800000), "lbl_eu_8066A0EC"),  # 256.0f
            (struct.pack(">I", 0x00000000), "lbl_eu_8066A0F0"),  # 0.0f
        ),
        extern_data_sections=(".data", ".rodata", ".sdata", ".sdata2", ".bss", ".sbss"),
    ),

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
        # The retail split object carries no data: the listsByPrio static
        # storage + guard + register_global_object cookie live in the nw4r
        # data objects (renamed to lbl_eu_806382F0/80665500/806382E0 above);
        # strip what MWCC emits here so the .text relocs resolve to the
        # retail data object at link.
        extern_data_sections=(".data", ".rodata", ".sdata", ".sdata2", ".bss", ".sbss"),
    ),

    "snd_SoundThread.o": UnitRules(
        # GetInstance's function-local static: retail references the guard +
        # instance + register_global_object cookie as flat address labels in
        # the nw4r data objects (same scheme as snd_TaskManager /
        # snd_SoundPlayer). @GUARD@instance$8941 -> lbl_eu_80665510,
        # instance$8941 (0x398) -> lbl_eu_8064FA50, cookie @8944 ->
        # lbl_eu_8064FA40.
        prefix_renames=(
            ("@GUARD@instance$", "lbl_eu_80665510"),
            ("instance$", "lbl_eu_8064FA50"),
            ("@89", "lbl_eu_8064FA40"),  # register_global_object cookie
        ),
        # Extras the retail linker dead-stripped: the ctor is inlined into
        # GetInstance's static-init (retail keeps no __ct__), AxCallbackProc
        # is inlined into AxCallbackFunc, the weak LinkList/AutoLock/
        # NonCopyable dtors and the empty virtual stubs have no surviving
        # references (retail .text is exactly the 9 functions, 0x6E4).
        drop_text_symbols=(
            "__ct__Q44nw4r3snd6detail11SoundThreadFv",
            "AxCallbackProc__Q44nw4r3snd6detail11SoundThreadFv",
            "__dt__Q34nw4r2ut12LinkListNodeFv",
            "__dt__Q44nw4r2ut29@unnamed@snd_SoundThread_cpp@11NonCopyableFv",
            "__dt__Q54nw4r3snd6detail9AxManager16CallbackListNodeFv",
            "__dt__Q34nw4r2ut59LinkList<Q54nw4r3snd6detail11SoundThread14PlayerCallback,0>Fv",
            "__dt__Q34nw4r2ut63LinkList<Q54nw4r3snd6detail11SoundThread18SoundFrameCallback,0>Fv",
            "__dt__Q44nw4r2ut6detail18AutoLock<7OSMutex>Fv",
            "OnBeginSoundFrame__Q54nw4r3snd6detail11SoundThread18SoundFrameCallbackFv",
            "OnEndSoundFrame__Q54nw4r3snd6detail11SoundThread18SoundFrameCallbackFv",
            "OnShutdownSoundThread__Q54nw4r3snd6detail11SoundThread14PlayerCallbackFv",
            "OnUpdateFrameSoundThread__Q54nw4r3snd6detail11SoundThread14PlayerCallbackFv",
            "OnUpdateVoiceSoundThread__Q54nw4r3snd6detail11SoundThread14PlayerCallbackFv",
        ),
        # The retail split object carries no data (singleton storage lives in
        # the nw4r data objects); strip what MWCC emits here.
        extern_data_sections=(".data", ".rodata", ".sdata", ".sdata2", ".bss", ".sbss"),
    ),

    "snd_SoundSystem.o": UnitRules(
        # The auto-__sinit_ cookie for the sTaskThread static; retail
        # references the flat address label lbl_eu_80638900 (nw4r data
        # objects). The TaskThread definition must stay in source (it drives
        # the retail __sinit_, a FULL_MATCH target); strip the emitted .bss
        # so the __sinit_/.text relocs resolve to the retail data object at
        # link (lbl_eu_80638910 is already the retail name in source).
        prefix_renames=(
            ("@89", "lbl_eu_80638900"),  # register_global_object cookie
        ),
        extern_data_sections=(".bss", ".sbss"),
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

    "CtrlAct.o": UnitRules(
        # The (f32)(s32) conversion's 2^52 magic pools in .sdata2 as @NNNN;
        # retail references the named constant lbl_eu_80666D50 (content match).
        pool_patterns=(
            (struct.pack(">II", MAGIC_HI, MAGIC_LO), "lbl_eu_80666D50"),
        ),
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
        # The 12 CCol4 statics live in the retail data slice (.bss); the auto
        # __sinit_ stays in this TU (retail .text keeps it), so the definition
        # is kept and the .bss is zeroed, with the white relocs retargeted to
        # the external C++ member symbol (same name).
        exact_renames=(("...bss.0", "white__Q22ml5CCol4"),),
        retarget_relocs=(
            (".text", 0x1E, "white__Q22ml5CCol4"),
        ),
        zero_nobits=(".bss",),
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

    # g3d_scnobj: the retail split is .text-only; every data symbol (ScnObj/
    # ScnGroup/ScnLeaf vtables, SetScnObjOption/GetScnObjOption switch jump
    # tables, 0.0f/1.0f pool floats) is owned by nw4r_data.s. Retarget the
    # .text relocs to the retail symbol names (UNDEF, mirroring the retail
    # .o), drop the .data/.sdata2 sections, and drop the implicit ScnLeaf
    # deleting-dtor the retail linker GC'd (its only referencer, the ScnLeaf
    # vtable, is also retail-owned).
    "g3d_scnobj.o": UnitRules(
        retarget_relocs=(
            (".text", 0xC6, "lbl_eu_80569688"),
            (".text", 0xD6, "lbl_eu_80569688"),
            (".text", 0x1CA, "lbl_eu_80569688"),
            (".text", 0x1CE, "lbl_eu_80569688"),
            (".text", 0x276, "jumptable_eu_805695F8"),
            (".text", 0x27E, "jumptable_eu_805695F8"),
            (".text", 0x41E, "jumptable_eu_80569620"),
            (".text", 0x426, "jumptable_eu_80569620"),
            (".text", 0xC02, "lbl_eu_80569648"),
            (".text", 0xC0A, "lbl_eu_80569648"),
            (".text", 0x12C, "lbl_eu_80669CD8"),
            (".text", 0x858, "lbl_eu_80669CDC"),
            (".text", 0x162E, "lbl_eu_80569648"),
            (".text", 0x1632, "lbl_eu_80569648"),
            (".text", 0x168E, "lbl_eu_80569648"),
            (".text", 0x1692, "lbl_eu_80569648"),
            (".text", 0x16D6, "lbl_eu_80569688"),
            (".text", 0x16DA, "lbl_eu_80569688"),
        ),
        drop_data_tail=((".sdata2", 0),),
        drop_data_range=((".data", 0x0, 0xF8),),
        drop_text_symbols=("__dt__Q34nw4r3g3d7ScnLeafFv",),
    ),

    "g3d_scnproc.o": UnitRules(
        # The 1.0f pool from the inlined ScnLeaf mScale(1.0f,1.0f,1.0f)
        # (shared g3d_scnobj.h init); retail references the shared pool label
        # lbl_eu_80669CF0. Rename the TU-local pool by content, then strip
        # .sdata2 so the Construct SDA21 reloc resolves to the retail data
        # object at link. The orphan weak ~ScnLeaf (0x3C) is dropped (same
        # symbol as the g3d_scnobj.o rule; retail linker GC'd it here).
        pool_patterns=(
            (struct.pack(">I", 0x3F800000), "lbl_eu_80669CF0"),  # 1.0f
        ),
        drop_text_symbols=("__dt__Q34nw4r3g3d7ScnLeafFv",),
        extern_data_sections=(".sdata2",),
    ),

    "g3d_resanmclr.o": UnitRules(
        # The u16->f32 conversion magic double (2^52) is compiler-generated
        # and cannot be named from source; retail references the shared
        # nw4r_data label lbl_eu_80669AD0. Rename the TU-local pool by
        # content and strip .sdata2 (same pattern as snd_Lfo /
        # CfCollSphereImpl).
        pool_patterns=(
            (struct.pack(">II", MAGIC_HI, 0x00000000), "lbl_eu_80669AD0"),  # 2^52
        ),
        extern_data_sections=(".sdata2",),
    ),

    # ut_IOStream: the retail split is .text-only; the IOStream RuntimeTypeInfo
    # object lives in nw4r_data.s (.sbss). The retail TU still emits the
    # auto-static-init (0xC: mParentTypeInfo = NULL) as a standalone function;
    # the decomp reproduces it by hand (no object definition) and the exact
    # rename restores the retail backslash symbol name, which cannot be typed
    # in C++ source.
    "ut_IOStream.o": UnitRules(
        exact_renames=(
            ("__sinit_ut_IOStream_cpp", r"__sinit_\ut_IOStream_cpp"),
        ),
    ),

    # ut_LockedCache: retail split is .text-only; the LCImpl singleton is
    # owned by nw4r_data.s (.bss) and declared extern in source. The retail
    # TU keeps the auto-static-init (0x18) and the anonymous-namespace
    # Lock_ deleting-dtor was linker-GC'd (its only referencer, the inlined
    # RAII sites, needs no out-of-line copy). Reproduce the sinit by hand and
    # rename it to the retail backslash form; drop the orphan dtor.
    "ut_LockedCache.o": UnitRules(
        exact_renames=(
            ("__sinit_ut_LockedCache_cpp", r"__sinit_\ut_LockedCache_cpp"),
        ),
        drop_text_symbols=(
            "__dt__Q54nw4r2ut28@unnamed@ut_LockedCache_cpp@6LCImpl5Lock_Fv",
        ),
    ),

    # ut_TextWriterBase: the retail split is .text-only (12 funcs, 0x2054);
    # the mDefaultTagProcessor statics/guards live in monolibdata1.s (.sbss
    # 0x80665550-0x8066557d), the TagProcessorBase vtables in nw4r_data.s
    # (.bss 0x80653EC8/0x80653ED4), and the f32/f64 constants in nw4r_data.s
    # (.sdata2 0x8066A160-0x8066A177). Source explicitly instantiates only the
    # 12 retail members + the two statics (so the auto __sinit_ emits both
    # guard blocks); retarget the pool/static/vtable relocs to the retail
    # names, strip the emitted data, and drop the weak helper dtors + the
    # inlined GetCharStrmReader that retail never put in this split.
    "ut_TextWriterBase.o": UnitRules(
        pool_patterns=(
            (struct.pack(">II", MAGIC_HI, MAGIC_LO), "lbl_eu_8066A168"),  # 2^52 (signed, int->f32 magic)
            (struct.pack(">I", 0x7F7FFFFF), "lbl_eu_8066A160"),             # FLT_MAX
            (struct.pack(">I", 0x00000000), "lbl_eu_8066A164"),             # 0.0f
            (struct.pack(">I", 0x3F000000), "lbl_eu_8066A170"),             # 0.5f
        ),
        retarget_relocs=(
            # statics (SDA21) and guards (SDA21) -> monolibdata1.s sbss
            (".text", 0x94, "lbl_eu_8066A164"),
            (".text", 0x9C, "lbl_eu_8066A160"),
            (".text", 0xA4, "lbl_eu_80665564"),
            (".text", 0x2194, "lbl_eu_80665568"),
            (".text", 0x21A0, "lbl_eu_80665560"),
            (".text", 0x21B4, "lbl_eu_80665560"),
            (".text", 0x21C4, "lbl_eu_80665568"),
            (".text", 0x21C8, "lbl_eu_80665569"),
            (".text", 0x21D4, "lbl_eu_80665564"),
            (".text", 0x21E8, "lbl_eu_80665564"),
            (".text", 0x21F8, "lbl_eu_80665569"),
            # TagProcessorBase vtables (ADDR16_HA/LO) -> nw4r_data.s .bss
            (".text", 0x21AE, "@3592_80653EC8"),
            (".text", 0x21BA, "@3592_80653EC8"),
            (".text", 0x21E2, "@3992_80653ED4"),
            (".text", 0x21EE, "@3992_80653ED4"),
        ),
        extern_data_sections=(".sdata", ".sdata2", ".bss", ".sbss"),
        drop_text_symbols=(
            "__dt__Q34nw4r2ut4RectFv",
            "__dt__Q34nw4r2ut5ColorFv",
            "__dt__Q44nw4r2ut10CharWriter12ColorMappingFv",
            "__dt__Q44nw4r2ut10CharWriter11VertexColorFv",
            "__dt__Q44nw4r2ut10CharWriter9TextColorFv",
            "__dt__Q34nw4r2ut14CharStrmReaderFv",
        ),
        drop_text_symbols_as_undef=(
            "GetCharStrmReader__Q34nw4r2ut4FontCFv",
        ),
    ),

    # math_arithmetic: the retail split is .text-only; the exp/log tables and
    # the f32 pool constants are owned by nw4r_data.s. The tables are declared
    # extern in source; retarget the compiler-generated pool relocs to the
    # retail sdata2 names and drop the pool.
    "math_arithmetic.o": UnitRules(
        retarget_relocs=(
            (".text", 0x0, "lbl_eu_80669E00"),
            (".text", 0x1C, "lbl_eu_80669E04"),
            (".text", 0x20, "lbl_eu_80669E08"),
            (".text", 0xC0, "lbl_eu_80669E10"),
            (".text", 0xC4, "lbl_eu_80669E0C"),
            (".text", 0xE4, "lbl_eu_80669E04"),
            (".text", 0x11C, "lbl_eu_80669E14"),
            (".text", 0x120, "lbl_eu_80669E18"),
        ),
        drop_data_tail=((".sdata2", 0),),
    ),

    # g3d_resanmlight / g3d_resanmamblight: retail splits are .text-only; the
    # 0.0f literal and the u16->f32 conversion magic double pool into the TU's
    # .sdata2 but are owned by nw4r_data.s. Retarget the pool relocs to the
    # retail names and drop the pool.
    "g3d_resanmlight.o": UnitRules(
        retarget_relocs=(
            (".text", 0x4C, "lbl_eu_80669B20"),
            (".text", 0x6C, "lbl_eu_80669B28"),
        ),
        drop_data_tail=((".sdata2", 0),),
    ),
    "g3d_resanmamblight.o": UnitRules(
        retarget_relocs=(
            (".text", 0x38, "lbl_eu_80669B10"),
            (".text", 0x58, "lbl_eu_80669B18"),
        ),
        drop_data_tail=((".sdata2", 0),),
    ),

    "g3d_resanmchr.o": UnitRules(
        # Retail split is .text-only; all data lives in nw4r_data.s: the
        # 8-entry GetAnmResult dispatch table (lbl_eu_8051D500) and the
        # .sdata2 pool (0.0f/1.0f/2.0f/3.0f/0.711111f DEG_TO_FIDX, the
        # u16->f32 magic double, and 1.40625f). MWCC pools the literals
        # locally (@N) and emits the table into .rodata; rename by content
        # to the retail labels, then strip .rodata/.sdata2 so the .text
        # relocs resolve to the retail data object at link.
        pool_patterns=(
            (struct.pack(">I", 0x00000000), "lbl_eu_80669AE0"),      # 0.0f
            (struct.pack(">I", 0x3F800000), "lbl_eu_80669AE4"),      # 1.0f
            (struct.pack(">I", 0x40000000), "lbl_eu_80669AE8"),      # 2.0f
            (struct.pack(">I", 0x40400000), "lbl_eu_80669AEC"),      # 3.0f
            (struct.pack(">I", 0x3F360B61), "lbl_eu_80669AF0"),      # 256/360
            (struct.pack(">II", MAGIC_HI, 0x00000000), "lbl_eu_80669AF8"),  # u16->f32 magic
            (struct.pack(">I", 0x3FB40000), "lbl_eu_80669B00"),      # 1.40625f
        ),
        exact_renames=(("gGetAnmResultTable", "lbl_eu_8051D500"),),
        extern_data_sections=(".rodata", ".sdata2"),
    ),

    # CStopwatchUtil: the retail split is .text-only; the entry table is
    # declared extern in source (lbl_eu_80657238, monolibdata1.s) and the
    # f32 literals / u32->f32 magic double pool into the TU's .sdata2 but are
    # owned by monolibdata2.s. Retarget the pool relocs and drop the pool.
    "CStopwatchUtil.o": UnitRules(
        retarget_relocs=(
            (".text", 0x200, "lbl_eu_8066A350"),
            (".text", 0x208, "lbl_eu_8066A348"),
            (".text", 0x2C8, "lbl_eu_8066A358"),
        ),
        drop_data_tail=((".sdata2", 0),),
    ),

    # lyt_common: retail split is .text-only; the static-local TexCoord
    # pCoords (0x20 .bss table + 1-byte .sbss guard) and the 0.0f/1.0f pool
    # are owned by nw4r_data.s (lbl_eu_80637B00 / lbl_eu_806654A0 /
    # lbl_eu_80669DF8 / lbl_eu_80669DFC). The guard codegen in SetSize is
    # byte-identical, so keep the static local and strip only the storage:
    # rename the symbols to the retail names, then extern_data_sections
    # zeroes the sections and UNDEFs the symbols so the .text relocs resolve
    # to the retail data object at link. Drop the orphan weak __dt__Color
    # (0x40) the retail linker GC'd.
    "lyt_common.o": UnitRules(
        exact_renames=(
            ("@GUARD@pCoords$5466", "lbl_eu_806654A0"),
            ("pCoords$5466", "lbl_eu_80637B00"),
        ),
        pool_patterns=(
            (struct.pack(">I", 0x00000000), "lbl_eu_80669DF8"),  # 0.0f
            (struct.pack(">I", 0x3F800000), "lbl_eu_80669DFC"),  # 1.0f
        ),
        extern_data_sections=(".sdata2", ".bss", ".sbss"),
        drop_text_symbols=("__dt__Q34nw4r2ut5ColorFv",),
    ),

    # snd_DisposeCallbackManager: the retail split is .text-only; the
    # lazy-singleton instance (0xC .bss), its __register_global_object
    # boundary (0xC .bss) and the guard byte (.sbss) are owned by nw4r_data.s
    # (lbl_eu_80637FDC / lbl_eu_80637FD0 / lbl_eu_806654E0). GetInstance's
    # static-local guard codegen is byte-identical, so keep the static local
    # and strip only the storage (rename + extern_data_sections). The five
    # weak dtors (AutoInterruptLock/NonCopyable/LinkListNode/LinkList/AutoLock,
    # 0x18C total) are unreferenced orphans the retail linker GC'd; dropping
    # them restores the exact retail 0x390 layout (no repack needed).
    "snd_DisposeCallbackManager.o": UnitRules(
        prefix_renames=(
            ("instance$", "lbl_eu_80637FDC"),
            ("@GUARD@instance$", "lbl_eu_806654E0"),
        ),
        exact_renames=(
            ("@8920", "lbl_eu_80637FD0"),
        ),
        extern_data_sections=(".bss", ".sbss"),
        drop_text_symbols=(
            "__dt__Q34nw4r2ut12LinkListNodeFv",
            "__dt__Q34nw4r2ut17AutoInterruptLockFv",
            "__dt__Q34nw4r2ut47LinkList<Q44nw4r3snd6detail15DisposeCallback,0>Fv",
            "__dt__Q44nw4r2ut40@unnamed@snd_DisposeCallbackManager_cpp@11NonCopyableFv",
            "__dt__Q54nw4r3snd6detail11SoundThread8AutoLockFv",
        ),
    ),

    # snd_StrmChannel: Setup/Shutdown/Alloc/Free already match byte-for-byte
    # with the AutoInterruptLock RAII (interrupts inline to
    # OSDisableInterrupts/OSRestoreInterrupts); the unit is 0x98 over only
    # because MWCC emits the orphan weak __dt__AutoInterruptLock (0x58) and
    # local __dt__NonCopyable (0x40), which the retail linker GC'd. Dropping
    # them packs the survivors at the exact retail offsets (Setup 0, Shutdown
    # 0x80, Alloc 0xC4, Free 0x1A8). No relocs reference the weaks.
    "snd_StrmChannel.o": UnitRules(
        drop_text_symbols=(
            "__dt__Q44nw4r2ut29@unnamed@snd_StrmChannel_cpp@11NonCopyableFv",
            "__dt__Q34nw4r2ut17AutoInterruptLockFv",
        ),
    ),

    # g3d_resanm: both GetRes*AnmResult functions match byte-for-byte and
    # the float pool is already extern'd in source (lbl_eu_80669AB0..AC0);
    # the unit is 0x40 over only because GetResColorAnmResult's by-value
    # ut::Color temps emit the orphan weak __dt__Color the retail linker
    # GC'd (no relocs reference it).
    "g3d_resanm.o": UnitRules(
        drop_text_symbols=("__dt__Q34nw4r2ut5ColorFv",),
    ),

    # ut_TagProcessorBase: retail split is .text-only; the two template
    # vtables (TagProcessorBase<c>/<w>, 0x14 each in .data) and the u32->f32
    # magic double pool (.sdata2) are owned by nw4r_data.s (lbl_eu_8056AE1C /
    # lbl_eu_8056AE08 / lbl_eu_8066A130). The ctors' vptr LEA stores are
    # byte-identical; rename the symbols to the retail labels and strip the
    # sections so the .text relocs resolve to the retail data object at link.
    "ut_TagProcessorBase.o": UnitRules(
        exact_renames=(
            ("__vt__Q34nw4r2ut19TagProcessorBase<c>", "lbl_eu_8056AE1C"),
            ("__vt__Q34nw4r2ut19TagProcessorBase<w>", "lbl_eu_8056AE08"),
        ),
        pool_patterns=(
            (struct.pack(">II", MAGIC_HI, MAGIC_LO), "lbl_eu_8066A130"),  # 2^52
        ),
        extern_data_sections=(".data", ".sdata2"),
    ),

    # g3d_camera: retail split is .text-only; the 0x30 .sdata2 pool (10
    # floats + 2^52 magic double) is owned by nw4r_data.s (lbl_eu_80669C58..
    # 80669C84). The literals are the only byte-exact source shape (externs
    # at each site bloat and reorder loads), so keep them and rename the
    # pool symbols by content, then strip the section — the .text SDA21
    # relocs resolve to the retail shared pool at link.
    "g3d_camera.o": UnitRules(
        pool_patterns=(
            (struct.pack(">I", 0x00000000), "lbl_eu_80669C58"),      # 0.0f
            (struct.pack(">I", 0x41700000), "lbl_eu_80669C5C"),      # 15.0f
            (struct.pack(">I", 0x3F800000), "lbl_eu_80669C60"),      # 1.0f
            (struct.pack(">I", 0x42700000), "lbl_eu_80669C64"),      # 60.0f
            (struct.pack(">I", 0x3FAAAAAB), "lbl_eu_80669C68"),      # 1.3333334f
            (struct.pack(">I", 0x3DCCCCCD), "lbl_eu_80669C6C"),      # 0.1f
            (struct.pack(">I", 0x447A0000), "lbl_eu_80669C70"),      # 1000.0f
            (struct.pack(">I", 0x3F000000), "lbl_eu_80669C74"),      # 0.5f
            (struct.pack(">II", MAGIC_HI, 0x00000000), "lbl_eu_80669C78"),  # 2^52 double
            (struct.pack(">I", 0xBF800000), "lbl_eu_80669C80"),      # -1.0f
            (struct.pack(">I", 0x3F360B61), "lbl_eu_80669C84"),      # 256/360
        ),
        extern_data_sections=(".sdata2",),
    ),

    # lyt_bounding: the RuntimeTypeInfo object (lbl_eu_80665498, 4-byte
    # .sbss) must stay DEFINED in source to generate the retail __sinit_
    # stub (registered FULL_MATCH us-8032ee90); retail ships the object from
    # nw4r_data.s and references it as UNDEF. All .text relocs already carry
    # the retail names, so zero_nobits strips only the storage and the
    # GetRuntimeTypeInfo/__sinit_ SDA21 relocs resolve to the shared pool at
    # link.
    "lyt_bounding.o": UnitRules(
        zero_nobits=(".sbss",),
    ),

    # ut_Font: InitReaderFunc's 0x30 .data member-function-pointer pool
    # ({0,-1,fn} x4, ADDR32 relocs to ReadNextChar*) is compiler-generated
    # and retail ships it from nw4r_data.s as the single label lbl_eu_8056AE30
    # (the LEA copy idiom only appears for the internal pool; an extern
    # reference changes the instruction shape and regresses the function).
    # Rename the section base symbol to the retail label and strip the .data
    # so the .text HA/LO relocs resolve to the shared pool at link.
    "ut_Font.o": UnitRules(
        exact_renames=(("...data.0", "lbl_eu_8056AE30"),),
        extern_data_sections=(".data",),
    ),

    # snd_RemoteSpeakerManager: the retail split is .text-only; the lazy-
    # singleton instance (0x2D8) and its guard byte are owned by nw4r_data.s.
    # The static-local keeps GetInstance byte-identical; extern_data_sections
    # strips the .bss/.sbss and the prefix renames give the .text relocs the
    # retail symbol names (the static-local $N counter is not stable).
    "snd_RemoteSpeakerManager.o": UnitRules(
        prefix_renames=(
            ("instance$", "lbl_eu_80637FE8"),
            ("@GUARD@instance$", "lbl_eu_806654F0"),
        ),
        extern_data_sections=(".bss", ".sbss"),
    ),

    # CDeviceFileJob: the retail split is .text-only; the vtable lives in
    # monolibdata2.s (.data, lbl_eu_8056C4D8). configure.py -RTTI off already
    # removes the weak __RTTI__ typeinfos/names; rename the vtable symbol,
    # strip the .data, and re-pack the .text at 4 to drop the 8-byte pad the
    # GC'd weak CWorkThread virtual stubs left behind.
    "CDeviceFileJob.o": UnitRules(
        exact_renames=(("__vt__14CDeviceFileJob", "lbl_eu_8056C4D8"),),
        extern_data_sections=(".data", ".rodata", ".sdata", ".sdata2", ".bss", ".sbss"),
        repack_after_drop=4,
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


def pad_data_section_func(path: Path, section: str, new_size: int) -> bool:
    """Zero-pad a FILE-BACKED data section up to new_size.

    Retail split objects often carry linker alignment padding after the last
    symbol (RTTI name pools / string tails) that MWCC does not emit; the
    data gate compares section size and bytes, so the pad restores the
    retail section exactly. Only grows; drops are handled by
    drop_data_tail / drop_data_range.
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
    for i in range(e_shnum):
        hoff = e_shoff + i * e_shentsize
        sh_name = struct.unpack_from(">I", data, hoff)[0]
        end = data.index(0, shstr_off + sh_name)
        name = data[shstr_off + sh_name : end].decode("ascii")
        if name == section:
            sec_idx, sec_hoff = i, hoff
            sec_off = struct.unpack_from(">I", data, hoff + 16)[0]
            sec_size = struct.unpack_from(">I", data, hoff + 20)[0]
            break
    if sec_idx is None or sec_size is None or sec_off is None or sec_hoff is None:
        return False
    if sec_size >= new_size:
        return False

    pad = new_size - sec_size
    sec_end = sec_off + sec_size
    data = data[:sec_end] + (b"\0" * pad) + data[sec_end:]
    e_shoff = struct.unpack_from(">I", data, 32)[0]
    if e_shoff >= sec_end:
        e_shoff += pad
        struct.pack_into(">I", data, 32, e_shoff)
    for i in range(e_shnum):
        hoff = e_shoff + i * e_shentsize
        sh_offset = struct.unpack_from(">I", data, hoff + 16)[0]
        if i == sec_idx:
            struct.pack_into(">I", data, hoff + 20, new_size)
        elif sh_offset >= sec_end:
            struct.pack_into(">I", data, hoff + 16, sh_offset + pad)

    path.write_bytes(data)
    return True


def pad_sdata2_section(path: Path, new_size: int) -> bool:
    """Zero-pad .sdata2 to new_size (retail 8-byte-aligned float tail)."""
    return pad_data_section_func(path, ".sdata2", new_size)


def set_section_align(path: Path, section: str, align: int) -> bool:
    """Set sh_addralign on *section*.

    The ppcdis retail splitter writes align=4 for sections MWCC emits with
    align=8 (identical content); the data gate compares section alignment
    for file-backed and NOBITS sections alike.
    """
    data = bytearray(path.read_bytes())
    if data[:4] != b"\x7fELF" or data[5] != 2:
        raise ValueError(f"expected big-endian ELF32: {path}")

    e_shoff = struct.unpack_from(">I", data, 32)[0]
    e_shentsize = struct.unpack_from(">H", data, 46)[0]
    e_shnum = struct.unpack_from(">H", data, 48)[0]
    e_shstrndx = struct.unpack_from(">H", data, 50)[0]
    shstr_off = struct.unpack_from(">I", data, e_shoff + e_shstrndx * e_shentsize + 16)[0]

    for i in range(e_shnum):
        hoff = e_shoff + i * e_shentsize
        sh_name = struct.unpack_from(">I", data, hoff)[0]
        end = data.index(0, shstr_off + sh_name)
        name = data[shstr_off + sh_name : end].decode("ascii")
        if name == section:
            if struct.unpack_from(">I", data, hoff + 32)[0] == align:
                return False
            struct.pack_into(">I", data, hoff + 32, align)
            path.write_bytes(data)
            return True
    return False


def drop_nobits_range_func(path: Path, section: str, start: int, end: int) -> bool:
    """Remove [start, end) of a NOBITS section (.bss/.sbss).

    MWCC 8-aligns .sbss statics that the retail linker packs at 4-byte
    boundaries, leaving anonymous padding between objects. NOBITS sections
    have no file bytes to splice (drop_data_range cannot handle them), so
    shrink sh_size and shift later symbols only; symbols inside the removed
    range are ABS'd (consistent with drop_data_range).
    """
    data = bytearray(path.read_bytes())
    if data[:4] != b"\x7fELF" or data[5] != 2:
        raise ValueError(f"expected big-endian ELF32: {path}")

    e_shoff = struct.unpack_from(">I", data, 32)[0]
    e_shentsize = struct.unpack_from(">H", data, 46)[0]
    e_shnum = struct.unpack_from(">H", data, 48)[0]
    e_shstrndx = struct.unpack_from(">H", data, 50)[0]
    shstr_off = struct.unpack_from(">I", data, e_shoff + e_shstrndx * e_shentsize + 16)[0]

    sec_idx = sec_hoff = sec_size = None
    sym_idx = None
    for i in range(e_shnum):
        hoff = e_shoff + i * e_shentsize
        sh_name = struct.unpack_from(">I", data, hoff)[0]
        end2 = data.index(0, shstr_off + sh_name)
        name = data[shstr_off + sh_name : end2].decode("ascii")
        if name == section:
            sec_idx, sec_hoff = i, hoff
            sec_size = struct.unpack_from(">I", data, hoff + 20)[0]
        elif name == ".symtab":
            sym_idx = i
    if sec_idx is None or sec_size is None or sec_hoff is None:
        return False
    if struct.unpack_from(">I", data, sec_hoff + 4)[0] != 8:  # SHT_NOBITS
        return False
    end = min(end, sec_size)
    length = end - start
    if length <= 0:
        return False

    struct.pack_into(">I", data, sec_hoff + 20, sec_size - length)

    if sym_idx is not None:
        sym_hoff = e_shoff + sym_idx * e_shentsize
        sym_off = struct.unpack_from(">I", data, sym_hoff + 16)[0]
        sym_size = struct.unpack_from(">I", data, sym_hoff + 20)[0]
        for so in range(0, sym_size, 16):
            st_value = struct.unpack_from(">I", data, sym_off + so + 4)[0]
            st_shndx = struct.unpack_from(">H", data, sym_off + so + 14)[0]
            if st_shndx != sec_idx:
                continue
            if start <= st_value < end:
                struct.pack_into(">I", data, sym_off + so + 8, 0)
                struct.pack_into(">H", data, sym_off + so + 14, 0xFFF1)  # SHN_ABS
            elif st_value >= end:
                struct.pack_into(">I", data, sym_off + so + 4, st_value - length)

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



def zero_nobits_section(path: Path, section: str) -> bool:
    """Set a NOBITS section (.bss/.sbss) size to 0 and UNDEF its symbols.

    Retail data slices own the zero-init objects, so code-TU split objects
    carry no .bss/.sbss; the decomp TU still needs the object *definition*
    so MWCC emits the auto-__sinit_ (retail .text keeps it), so the symbols
    are dropped here instead of at source (drop_data_tail cannot be used on
    NOBITS sections — there are no file bytes to splice).
    """
    data = bytearray(path.read_bytes())
    if data[:4] != b"\x7fELF" or data[5] != 2:
        raise ValueError(f"expected big-endian ELF32: {path}")

    e_shoff = struct.unpack_from(">I", data, 32)[0]
    e_shentsize = struct.unpack_from(">H", data, 46)[0]
    e_shnum = struct.unpack_from(">H", data, 48)[0]
    e_shstrndx = struct.unpack_from(">H", data, 50)[0]
    shstr_off = struct.unpack_from(">I", data, e_shoff + e_shstrndx * e_shentsize + 16)[0]

    sec_idx = sym_idx = None
    for i in range(e_shnum):
        hoff = e_shoff + i * e_shentsize
        sh_name = struct.unpack_from(">I", data, hoff)[0]
        end = data.index(0, shstr_off + sh_name)
        name = data[shstr_off + sh_name : end].decode("ascii")
        if name == section:
            sec_idx = i
        elif name == ".symtab":
            sym_idx = i
    if sec_idx is None:
        return False
    sec_hoff = e_shoff + sec_idx * e_shentsize
    if struct.unpack_from(">I", data, sec_hoff + 4)[0] != 8:  # SHT_NOBITS
        return False
    if struct.unpack_from(">I", data, sec_hoff + 20)[0] == 0:
        return False
    struct.pack_into(">I", data, sec_hoff + 20, 0)  # sh_size = 0

    if sym_idx is not None:
        sym_hoff = e_shoff + sym_idx * e_shentsize
        sym_off = struct.unpack_from(">I", data, sym_hoff + 16)[0]
        sym_size = struct.unpack_from(">I", data, sym_hoff + 20)[0]
        for so in range(0, sym_size, 16):
            st_shndx = struct.unpack_from(">H", data, sym_off + so + 14)[0]
            if st_shndx == sec_idx:
                struct.pack_into(">H", data, sym_off + so + 14, 0)  # SHN_UNDEF

    path.write_bytes(data)
    return True



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


def patch_reloc_addend(path: Path, section: str, offset: int, delta: int) -> bool:
    """Add *delta* to the r_addend of the SHT_RELA entry at (section, offset).

    Used for switch/jumptable case-label addends inside residual functions
    whose code is a documented §17.6 length residual (wkStandby etc.).
    """
    data = bytearray(path.read_bytes())
    if data[:4] != b"\x7fELF" or data[5] != 2:
        raise ValueError(f"expected big-endian ELF32: {path}")

    e_shoff = struct.unpack_from(">I", data, 32)[0]
    e_shentsize = struct.unpack_from(">H", data, 46)[0]
    e_shnum = struct.unpack_from(">H", data, 48)[0]
    e_shstrndx = struct.unpack_from(">H", data, 50)[0]
    shstr_off = struct.unpack_from(">I", data, e_shoff + e_shstrndx * e_shentsize + 16)[0]

    rela_idx = None
    for i in range(e_shnum):
        hoff = e_shoff + i * e_shentsize
        sh_name, = struct.unpack_from(">I", data, hoff)
        end = data.index(0, shstr_off + sh_name)
        name = data[shstr_off + sh_name : end].decode("ascii")
        if name == ".rela" + section:
            rela_idx = i
            break
    if rela_idx is None:
        return False

    rela_hoff = e_shoff + rela_idx * e_shentsize
    rela_off = struct.unpack_from(">I", data, rela_hoff + 16)[0]
    rela_size = struct.unpack_from(">I", data, rela_hoff + 20)[0]
    for ro in range(0, rela_size, 12):
        r_offset, r_info, r_addend = struct.unpack_from(">IIi", data, rela_off + ro)
        if r_offset == offset:
            struct.pack_into(">i", data, rela_off + ro + 8, r_addend + delta)
            path.write_bytes(data)
            return True
    return False


def swap_data_blocks_func(path: Path, section: str, off1: int, off2: int, size: int) -> bool:
    """Swap two equal-size byte blocks inside a FILE-BACKED *section* and move
    the relocs that point into them (R_PPC_ADDR32 r_offsets are section-relative).

    CProc.o reslist layout: MWCC emits [lbl_eu_8056B28C][__vt__11reslist<Ul>]
    while retail interleaves [__vt__11reslist<Ul>][lbl_eu_8056B28C]; swapping
    the 12-byte blocks restores byte + reloc positions.
    """
    data = bytearray(path.read_bytes())
    if data[:4] != b"\x7fELF" or data[5] != 2:
        raise ValueError(f"expected big-endian ELF32: {path}")

    e_shoff = struct.unpack_from(">I", data, 32)[0]
    e_shentsize = struct.unpack_from(">H", data, 46)[0]
    e_shnum = struct.unpack_from(">H", data, 48)[0]
    e_shstrndx = struct.unpack_from(">H", data, 50)[0]
    shstr_off = struct.unpack_from(">I", data, e_shoff + e_shstrndx * e_shentsize + 16)[0]

    sec_idx = rela_idx = None
    for i in range(e_shnum):
        hoff = e_shoff + i * e_shentsize
        sh_name, = struct.unpack_from(">I", data, hoff)
        end = data.index(0, shstr_off + sh_name)
        name = data[shstr_off + sh_name : end].decode("ascii")
        if name == section:
            sec_idx = i
        elif name == ".rela" + section:
            rela_idx = i
    if sec_idx is None or rela_idx is None or off1 == off2 or size <= 0:
        return False
    if off1 > off2:
        off1, off2 = off2, off1
    sec_hoff = e_shoff + sec_idx * e_shentsize
    if off2 + size > struct.unpack_from(">I", data, sec_hoff + 20)[0]:
        return False

    sec_off = struct.unpack_from(">I", data, sec_hoff + 16)[0]
    a = data[sec_off + off1 : sec_off + off1 + size]
    b = data[sec_off + off2 : sec_off + off2 + size]
    data[sec_off + off1 : sec_off + off1 + size] = b
    data[sec_off + off2 : sec_off + off2 + size] = a

    rela_hoff = e_shoff + rela_idx * e_shentsize
    rela_off = struct.unpack_from(">I", data, rela_hoff + 16)[0]
    rela_size = struct.unpack_from(">I", data, rela_hoff + 20)[0]
    delta = off2 - off1
    for ro in range(0, rela_size, 12):
        r_offset, = struct.unpack_from(">I", data, rela_off + ro)
        if off1 <= r_offset < off1 + size:
            struct.pack_into(">I", data, rela_off + ro, r_offset + delta)
        elif off2 <= r_offset < off2 + size:
            struct.pack_into(">I", data, rela_off + ro, r_offset - delta)

    path.write_bytes(data)
    return True


def add_local_symbol(path: Path, sym_name: str, section: str, offset: int, size: int) -> bool:
    """Add a new LOCAL symbol (sym_name, section, offset, size) to .symtab.

    Defines subobject labels C++ cannot express (base-list addresses inside a
    data block). Used with source-side UNDEF externs: the .sdata locator reloc
    (e.g. lbl_eu_8056B5A4) then resolves to the local definition at link.
    """
    data = bytearray(path.read_bytes())
    if data[:4] != b"\x7fELF" or data[5] != 2:
        raise ValueError(f"expected big-endian ELF32: {path}")

    e_shoff = struct.unpack_from(">I", data, 32)[0]
    e_shentsize = struct.unpack_from(">H", data, 46)[0]
    e_shnum = struct.unpack_from(">H", data, 48)[0]
    e_shstrndx = struct.unpack_from(">H", data, 50)[0]
    shstr_off = struct.unpack_from(">I", data, e_shoff + e_shstrndx * e_shentsize + 16)[0]

    sec_idx = sym_idx = str_idx = None
    for i in range(e_shnum):
        hoff = e_shoff + i * e_shentsize
        sh_name, = struct.unpack_from(">I", data, hoff)
        end = data.index(0, shstr_off + sh_name)
        name = data[shstr_off + sh_name : end].decode("ascii")
        if name == section:
            sec_idx = i
        elif name == ".symtab":
            sym_idx = i
        elif name == ".strtab":
            str_idx = i
    if sec_idx is None or sym_idx is None or str_idx is None:
        return False

    sym_hoff = e_shoff + sym_idx * e_shentsize
    sym_off = struct.unpack_from(">I", data, sym_hoff + 16)[0]
    sym_size = struct.unpack_from(">I", data, sym_hoff + 20)[0]
    str_hoff = e_shoff + str_idx * e_shentsize
    str_off = struct.unpack_from(">I", data, str_hoff + 16)[0]
    str_size = struct.unpack_from(">I", data, str_hoff + 20)[0]

    # Prefer REDEFINING an existing UNDEF symbol in place (the source's
    # extern decl produces one): patching shndx/value/size avoids appending
    # to .symtab, which mwldeppc misreads (internal linker error
    # ELF_linker.c:11164 — the appended LOCAL lands past sh_info). Only
    # append when no symbol with the name exists.
    target = sym_name.encode("ascii")
    found = False
    for j in range(0, sym_size, 16):
        st_name, = struct.unpack_from(">I", data, sym_off + j)
        end = data.index(0, str_off + st_name)
        if data[str_off + st_name : end] == target:
            struct.pack_into(">IIBBH", data, sym_off + j + 4, offset, size, 0x11, 0, sec_idx)
            found = True
            break
    if found:
        path.write_bytes(data)
        return True

    # Fallback: append a new symbol (rare; the caller should declare the
    # extern so the UNDEF slot already exists).
    old_end = str_off + str_size
    data = data[:old_end] + target + b"\0" + data[old_end:]
    e_shoff = struct.unpack_from(">I", data, 32)[0]
    if e_shoff >= old_end:
        e_shoff += len(target)
        struct.pack_into(">I", data, 32, e_shoff)
    for i in range(e_shnum):
        hoff = e_shoff + i * e_shentsize
        sh_offset, = struct.unpack_from(">I", data, hoff + 16)
        if i == str_idx:
            struct.pack_into(">I", data, hoff + 20, str_size + len(target))
        elif sh_offset >= old_end:
            struct.pack_into(">I", data, hoff + 16, sh_offset + len(target))

    sym_hoff = e_shoff + sym_idx * e_shentsize
    sym_off = struct.unpack_from(">I", data, sym_hoff + 16)[0]
    sym_size = struct.unpack_from(">I", data, sym_hoff + 20)[0]
    entry = struct.pack(">IIIBBH", str_size, offset, size, 0x11, 0, sec_idx)
    sym_end = sym_off + sym_size
    data = data[:sym_end] + entry + data[sym_end:]
    e_shoff = struct.unpack_from(">I", data, 32)[0]
    if e_shoff >= sym_end:
        e_shoff += 16
        struct.pack_into(">I", data, 32, e_shoff)
    for i in range(e_shnum):
        hoff = e_shoff + i * e_shentsize
        sh_offset, = struct.unpack_from(">I", data, hoff + 16)
        if i == sym_idx:
            struct.pack_into(">I", data, hoff + 20, sym_size + 16)
        elif sh_offset >= sym_end:
            struct.pack_into(">I", data, hoff + 16, sh_offset + 16)

    path.write_bytes(data)
    return True


def inject_reloc_to_symbol(path: Path, section: str, offset: int, sym_name: str) -> bool:
    """Append a new R_PPC_ADDR32 SHT_RELA entry at (section, offset) pointing at
    an existing symbol (retail vtable slot MWCC zeroed when -RTTI off
    suppressed the typeinfo word; e.g. CChildListNode __vt__34TChildListHeader
    word0 -> lbl_eu_806635E0).

    The slot bytes stay 0x00000000 in the file (as a reloc placeholder); the
    injected entry makes the linker fill the address, matching retail. Only
    grows the .rela section; the data section is untouched.
    """
    data = bytearray(path.read_bytes())
    if data[:4] != b"\x7fELF" or data[5] != 2:
        raise ValueError(f"expected big-endian ELF32: {path}")

    e_shoff = struct.unpack_from(">I", data, 32)[0]
    e_shentsize = struct.unpack_from(">H", data, 46)[0]
    e_shnum = struct.unpack_from(">H", data, 48)[0]
    e_shstrndx = struct.unpack_from(">H", data, 50)[0]
    shstr_off = struct.unpack_from(">I", data, e_shoff + e_shstrndx * e_shentsize + 16)[0]

    sym_idx = rela_idx = str_idx = None
    target_sym_idx = None
    for i in range(e_shnum):
        hoff = e_shoff + i * e_shentsize
        sh_name = struct.unpack_from(">I", data, hoff)[0]
        end = data.index(0, shstr_off + sh_name)
        name = data[shstr_off + sh_name : end].decode("ascii")
        if name == ".symtab":
            sym_idx = i
        elif name == ".rela" + section:
            rela_idx = i
        elif name == ".strtab":
            str_idx = i
    if sym_idx is None or rela_idx is None or str_idx is None:
        return False

    sym_hoff = e_shoff + sym_idx * e_shentsize
    sym_off = struct.unpack_from(">I", data, sym_hoff + 16)[0]
    sym_size = struct.unpack_from(">I", data, sym_hoff + 20)[0]
    str_hoff = e_shoff + str_idx * e_shentsize
    str_off = struct.unpack_from(">I", data, str_hoff + 16)[0]
    target_bytes = sym_name.encode("ascii")
    for j in range(0, sym_size, 16):
        st_name = struct.unpack_from(">I", data, sym_off + j)[0]
        end = data.index(0, str_off + st_name)
        if data[str_off + st_name : end] == target_bytes:
            target_sym_idx = j // 16
            break
    # Also accept a symbol with a trailing-@ pool/clone suffix (objcopy may
    # uniquify e.g. lbl_eu_806635E0 -> lbl_eu_806635E0.55741).
    if target_sym_idx is None:
        for j in range(0, sym_size, 16):
            st_name = struct.unpack_from(">I", data, sym_off + j)[0]
            end = data.index(0, str_off + st_name)
            nm = data[str_off + st_name : end]
            if nm.startswith(target_bytes[:-1]) and nm.endswith(b"\0"):
                target_sym_idx = j // 16
                break
    if target_sym_idx is None:
        return False

    rela_hoff = e_shoff + rela_idx * e_shentsize
    rela_off = struct.unpack_from(">I", data, rela_hoff + 16)[0]
    rela_size = struct.unpack_from(">I", data, rela_hoff + 20)[0]
    # Skip if an entry for this offset already exists.
    for ro in range(0, rela_size, 12):
        r_offset = struct.unpack_from(">I", data, rela_off + ro)[0]
        if r_offset == offset:
            return False

    entry = struct.pack(">IIi", offset, (target_sym_idx << 8) | 1, 0)  # R_PPC_ADDR32
    rela_end = rela_off + rela_size
    data = data[:rela_end] + entry + data[rela_end:]
    e_shoff = struct.unpack_from(">I", data, 32)[0]
    if e_shoff >= rela_end:
        e_shoff += 12
        struct.pack_into(">I", data, 32, e_shoff)
    for i in range(e_shnum):
        hoff = e_shoff + i * e_shentsize
        sh_offset = struct.unpack_from(">I", data, hoff + 16)[0]
        if i == rela_idx:
            struct.pack_into(">I", data, hoff + 20, rela_size + 12)
        elif sh_offset >= rela_end:
            struct.pack_into(">I", data, hoff + 16, sh_offset + 12)

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


def extern_data_sections(path: Path, sections: tuple[str, ...]) -> bool:
    """Strip the named data sections and convert their symbols to UNDEF.

    For TUs whose retail split object carries NO data sections at all (all
    retail data lives in the shared data objects: split1.s / nw4r_data.o /
    monolibdata*.o), the decompiled object must also emit none. MWCC
    necessarily emits vtables/RTTI/pooled constants the retail linker placed
    elsewhere, so: zero each section's sh_size (and its .rela section),
    convert every symbol defined in them to SHN_UNDEF with value/size 0, and
    keep .text relocations pointing at the same names — they resolve to the
    retail data object at link, mirroring the DOL-extracted retail .o exactly.
    """
    data = bytearray(path.read_bytes())
    if data[:4] != b"\x7fELF" or data[5] != 2:
        raise ValueError(f"expected big-endian ELF32: {path}")

    e_shoff = struct.unpack_from(">I", data, 32)[0]
    e_shentsize = struct.unpack_from(">H", data, 46)[0]
    e_shnum = struct.unpack_from(">H", data, 48)[0]
    e_shstrndx = struct.unpack_from(">H", data, 50)[0]
    shstr_off = struct.unpack_from(">I", data, e_shoff + e_shstrndx * e_shentsize + 16)[0]

    sec_idx: set[int] = set()
    rela_idx: set[int] = set()
    sym_idx = None
    for i in range(e_shnum):
        hoff = e_shoff + i * e_shentsize
        sh_name = struct.unpack_from(">I", data, hoff)[0]
        end = data.index(0, shstr_off + sh_name)
        name = data[shstr_off + sh_name : end].decode("ascii")
        if name in sections:
            sec_idx.add(i)
            struct.pack_into(">I", data, hoff + 20, 0)  # sh_size = 0
        elif name.startswith(".rela") and name[5:] in sections:
            rela_idx.add(i)
            struct.pack_into(">I", data, hoff + 20, 0)  # sh_size = 0
        elif name == ".symtab":
            sym_idx = i
    if not sec_idx:
        return False

    if sym_idx is not None:
        sym_hoff = e_shoff + sym_idx * e_shentsize
        sym_off = struct.unpack_from(">I", data, sym_hoff + 16)[0]
        sym_size = struct.unpack_from(">I", data, sym_hoff + 20)[0]
        for so in range(0, sym_size, 16):
            st_shndx = struct.unpack_from(">H", data, sym_off + so + 14)[0]
            if st_shndx in sec_idx:
                struct.pack_into(">I", data, sym_off + so + 4, 0)  # st_value
                struct.pack_into(">I", data, sym_off + so + 8, 0)  # st_size
                struct.pack_into(">H", data, sym_off + so + 14, 0)  # SHN_UNDEF

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
        # Link-time copies are named *.reloc.o (tools/project.py
        # link_reloc_postprocess rule); match by the original basename.
        if rules is None and path.name.endswith(".reloc.o"):
            rules = UNIT_RULES.get(path.name[:-8] + ".o")
    if rules is None:
        return False

    changed = False
    # Rename pool symbols BEFORE trimming/padding .sdata2: the trim drops the
    # pool section (and ABS's its symbols), which would leave nothing for
    # rename_pool_symbols to match (snd_EnvGenerator/CMCEffStart constants).
    changed = rename_pool_symbols(path, rules.pool_patterns) or changed
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
    for sec in rules.zero_nobits:
        changed = zero_nobits_section(path, sec) or changed
    changed = rename_exact(path, rules.exact_renames) or changed
    changed = rename_by_prefix(path, rules.prefix_renames) or changed
    changed = patch_symbol_sizes(path, rules.symbol_sizes) or changed
    # A second objcopy pass for exact renames can uniquify a pool symbol when
    # the retail name also exists in another section. Re-apply content-based
    # pool naming last so @N numbering never becomes part of a unit rule.
    changed = rename_pool_symbols(path, rules.pool_patterns) or changed
    for sec, off, sym_name in rules.retarget_relocs:
        changed = retarget_reloc_to_symbol(path, sec, off, sym_name) or changed
    for sec, o1, o2, size in rules.swap_data_blocks:
        changed = swap_data_blocks_func(path, sec, o1, o2, size) or changed
    for sec, off, delta in rules.addend_patches:
        changed = patch_reloc_addend(path, sec, off, delta) or changed
    for sec, off, sym_name in rules.inject_relocs:
        changed = inject_reloc_to_symbol(path, sec, off, sym_name) or changed
    for sym_name, sec, off, size in rules.add_symbols:
        changed = add_local_symbol(path, sym_name, sec, off, size) or changed
    for sec, off, tsec, toff in rules.retarget_relocs_local:
        changed = retarget_reloc_to_local(path, sec, off, tsec, toff) or changed
    for sec, start, end in rules.zero_data_range:
        changed = zero_data_range(path, sec, start, end) or changed
    for sec, start, end in rules.drop_data_range:
        changed = drop_data_range(path, sec, start, end) or changed
    for sec, start, end in rules.drop_nobits_range:
        changed = drop_nobits_range_func(path, sec, start, end) or changed
    for sec, keep in rules.drop_data_tail:
        changed = drop_data_tail(path, sec, keep) or changed
    for sec, target in rules.pad_data_section:
        changed = pad_data_section_func(path, sec, target) or changed
    for sec, align in rules.set_data_align:
        changed = set_section_align(path, sec, align) or changed
    if rules.extern_data_sections:
        changed = extern_data_sections(path, rules.extern_data_sections) or changed
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
            # Link-time copies are named *.reloc.o (tools/project.py
            # link_reloc_postprocess rule); match the rule by the original
            # basename.
            if not (path.name.endswith(".reloc.o") and path.name[:-8] + ".o" in UNIT_RULES):
                print(f"no reloc postprocess rules for {path.name}")
                continue
        if postprocess_object(path):
            print(f"post-processed {path}")
        else:
            print(f"no post-process changes for {path}")
    return rc


if __name__ == "__main__":
    raise SystemExit(main(sys.argv))
