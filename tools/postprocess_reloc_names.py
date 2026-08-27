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
    # Permute whole .sdata2 4-byte words to retail order: entry j gives the
    # SOURCE word index whose value belongs at position j (KPAD-style units
    # whose literal pool is emitted in decomp text first-use order while their
    # .text is still unmatched; symbols follow their values, so text loads
    # stay correct).
    permute_sdata2_words: tuple[int, ...] = ()
    # Swap .sdata2[0:4] with [4:8] for an arbitrary decomp word-order guard
    # (mtx: MWCC emits 0.0f then 1.0f; retail pools 1.0f first), also swapping
    # the two symbols' st_values. Generalizes swap_sdata2_leading_f32_pair
    # without touching its hard-coded 0.0f/32000.0f guard.
    swap_sdata2_leading_f32_words: tuple[int, int] | None = None
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
    # Reloc-referenced anonymous @ pool symbols matched by FILE-BACKED
    # data-section content prefix -> retail name ((section, content, name),
    # ...). Generalizes pool_patterns (.sdata2-only) to any section; used for
    # RTTI name strings / typeinfo structs MWCC emits under unspellable @N
    # names whose numbering drifts with every source change. Each pattern
    # must match exactly one referenced @ symbol in its section.
    data_pool_patterns: tuple[tuple[str, bytes, str], ...] = ()
    # Exact symbol renames (old -> new), applied after pool content matches.
    exact_renames: tuple[tuple[str, str], ...] = ()
    # Prefix renames: first symbol whose name starts with old_prefix -> new.
    # For MWCC static local numbering that drifts ($8802 vs $8817).
    prefix_renames: tuple[tuple[str, str], ...] = ()
    # Promote every symtab entry named here to GLOBAL binding (objcopy
    # --globalize-symbol). Rename+strip rules leave relocated pool/jumptable
    # references on LOCAL UNDEF entries; mwldeppc cannot resolve those against
    # the shared data object's global definition at final link (verified:
    # "undefined" errors), while GLOBAL UNDEF resolves by name like any
    # extern. Mirrors tools/project.py's --globalize-symbol link transform.
    globalize_symbols: tuple[str, ...] = ()
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
    # Move .rela.<section> r_offset values for DATA sections: (section,
    # old_off, new_off). Used when MWCC emits data blocks in a different
    # order than retail but every content word is a link-time reloc or zero
    # (bytes already compare equal) -- only the reloc positions/names need
    # remapping onto the retail layout.
    data_reloc_offset_moves: tuple[tuple[str, int, int], ...] = ()
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
    # Set r_addend absolutely at (section, offset, value): same use case as
    # addend_patches, but pins the retail value instead of a relative delta
    # so the gate passes regardless of which ambient -ipa codegen variant
    # produced the object (case-label offsets drift per variant).
    addend_sets: tuple[tuple[str, int, int], ...] = ()
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
    "code_804B2FF0.o": UnitRules(
        # monolib coli dissolve (novtable TU): all class data ships from the
        # dissolved blocks in code_804B2FF0.cpp. The hand-built CTask<>
        # vtable slots spell the template-specialization members with <> in
        # the retail mangled names - not legal C++ identifiers - so the
        # source uses legal stand-ins; retarget the three slots below.
        retarget_relocs=(
            (".data", 0xBC, "__dt__26CTTask<16CTaskColiManager>Fv"),
            (".data", 0xCC, "Move__26CTTask<16CTaskColiManager>Fv"),
            (".data", 0xD0, "Draw__26CTTask<16CTaskColiManager>Fv"),
        ),
        # Retail split packs .data at align 4 and the NOBITS slices at 8/4;
        # MWCC derives section alignment from the widest member.
        set_data_align=((".data", 4), (".bss", 8), (".sbss", 4)),
        # The .sdata "ref" needle is defined under a stand-in spelling (the
        # retail const char[4] type conflicts with the char[8] va-arg decl).
        exact_renames=(("sdata_ref_needle", "lbl_eu_80663AA8"),),
    ),
    "CErrorWii.o": UnitRules(
        # Retail CErrorWii.o carries NO data: the IErrorWii vtable/RTTI (+ its
        # .rodata typeinfo-name pool) and the CErrorWii class statics
        # (spInstance / sPowerCallbackCalled / sResetCallbackCalled /
        # sUnkFlag) ship from the shared monolibdata objects. Rename the
        # statics onto their monolibdata labels (retail_reloc_map.json: all
        # SDA21 refs are unique to this unit), then strip whatever MWCC emits
        # so the .text relocs resolve to the retail data objects at link
        # (same rename-then-strip pattern as CNandData.o).
        exact_renames=(
            ("spInstance__9CErrorWii", "lbl_eu_80665A60"),
            ("sPowerCallbackCalled__9CErrorWii", "lbl_eu_80665A64"),
            ("sResetCallbackCalled__9CErrorWii", "lbl_eu_80665A65"),
            ("sUnkFlag__9CErrorWii", "lbl_eu_80665A66"),
        ),
        extern_data_sections=(".data", ".rodata", ".sdata", ".sdata2", ".bss", ".sbss"),
    ),
    "CTaskLOD.o": UnitRules(
        # Data dissolve: retail split owns this class's data (vtables
        # lbl_eu_8056D678/69C/6C0/6E4 + RTTI base tables in .data, typeinfo
        # descriptors + "ref" strings in .sdata, RTTI name strings in .rodata,
        # singleton pointer slot in .sbss) — all hand-built in source (see the
        # data block in CTaskLOD.cpp).  The CTTask<CTaskLOD> template members
        # referenced by lbl_eu_8056D678/6C0 cannot be spelled in source ('<'
        # in the mangled names), so their stand-in Tail__8CProcessFv relocs are
        # retargeted at the local .text definitions.
        retarget_relocs_local=(
            (".data", 0x1C, ".text", 0x848),  # Draw__17CTask<8CTaskLOD>Fv
            (".data", 0x50, ".text", 0x890),  # __dt__17CTTask<8CTaskLOD>Fv
            (".data", 0x60, ".text", 0x800),  # Move__17CTTask<8CTaskLOD>Fv
            (".data", 0x64, ".text", 0x848),  # Draw__17CTTask<8CTaskLOD>Fv
        ),
        # func_80462C14/F2C/FF4 load their 1.0f/0.0f constants from the shared
        # nw4r pool; rename the lone local pool entries and strip the storage.
        pool_patterns=(
            (struct.pack(">I", 0x3F800000), "lbl_eu_8066A5B8"),  # 1.0f
            (struct.pack(">I", 0x00000000), "lbl_eu_8066A5BC"),  # 0.0f
        ),
        extern_data_sections=(".sdata2",),
    ),
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
            ("__RTTI__48CTask<Q226@unnamed@CTaskManager_cpp@9CRootProc>", "lbl_eu_80663570"),
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
        # The surviving bad_cast typeinfo name/struct keep anonymous @ names
        # whose numbering differs from the retail splitter's (@260/@261);
        # rename by content (sizes pinned by the matcher).
        data_pool_patterns=(
            (".rodata", b"std::bad_cast\x00", "@260"),
            (".data", b"\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00", "@261"),
        ),
        drop_data_tail=((".sdata2", 0x0),),
    ),

    "New.o": UnitRules(
        # Retail New.o is data-only std::exception RTTI: the .rodata name
        # string "std::exception" is an anonymous local "@86" while MWCC
        # spells it from the unnamed-namespace static. Same bytes both sides;
        # rename so the __RTTI__Q23std9exception .sdata reloc carries the
        # retail name.
        exact_renames=(
            ("exceptionName__16@unnamed@New_cp@", "@86"),
        ),
    ),
    "locale.o": UnitRules(
        # Byte-identical .data/.rodata/.sdata2; only the anonymous pool
        # numbering drifts: this TU's MWCC counter runs one ahead of the
        # retail splitter's (same 9 objects, same emission order, uniform
        # +1 shift). Chain-shift the names down so each @N matches the
        # retail label for its content (@140="." @141="" @142="AM|PM",
        # @143..@148 = date/month format strings).
        exact_renames=(
            ("@141", "@140"), ("@142", "@141"), ("@143", "@142"),
            ("@144", "@143"), ("@145", "@144"), ("@146", "@145"),
            ("@147", "@146"), ("@148", "@147"), ("@149", "@148"),
        ),
    ),
    "lyt_picture.o": UnitRules(
        # Retail keeps Picture's typeinfo chain local but references the
        # Pane/PaneBase typeinfo NAMES externally (__RTTI__Q36nw4hbm3lyt4Pane
        # / __RTTI__Q46nw4hbm3lyt6detail8PaneBase, defined in the retail
        # lyt_pane.o which links as NonMatching). MWCC emits weak local copies
        # (+0x50 .data over the 0xA0 slice); retarget the surviving struct
        # name/base ptrs to the retail symbols and drop the weak tail.
        retarget_relocs=(
            # NOTE order matters: retail's chain is PaneBase@+0x84 / Pane@+0x8C
            # (verified against the retail split reloc list; the previous
            # mapping had the two swapped).
            (".data", 0x84, "__RTTI__Q46nw4hbm3lyt6detail8PaneBase"),
            (".data", 0x8C, "__RTTI__Q36nw4hbm3lyt4Pane"),
        ),
        drop_data_tail=((".data", 0xA0),),
        # merged from nw4r same-basename unit (nw4r_data.s shared pool)
        pool_patterns=(
            (struct.pack(">I", 0x00000000), "lbl_eu_80669A68"),
        ),
        extern_data_sections=(".sdata2",),
        # Reloc-name drift on byte-identical .data: retail names the anon
        # locals at +0x98/+0xA0 lbl_8054D9A4 / lbl_8054D9BC. @N numbering
        # drifts with source edits.
        exact_renames=(
            ("@5172", "lbl_8054D9A4"),
            ("@5173", "lbl_8054D9BC"),
        ),
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
        # it. Trimming restores the 0x110 retail slice.
        trim_sdata2_size=0x110,
    ),
    "NANDCheck.o": UnitRules(
        # RVL NANDCheck (MWCC_CASES "RVL NANDCheck"): cosmetic renames of the
        # callback statics onto their retail labels, plus MWCC emits one extra
        # trailing NUL in .data over the retail 0xE7 slice.
        exact_renames=(
            ("s_nandUserAreaCallbackName", "lbl_8055127C"),
            ("s_nandUserAreaCallbackFmt", "lbl_80551294"),
        ),
        # USER_DIR_LIST path strings are anonymous @N objects in MWCC output;
        # rename by content onto the retail labels.
        data_pool_patterns=(
            (".sdata", b"/meta\x00", "lbl_80665B10"),
            (".sdata", b"/ticket\x00", "lbl_80665B18"),
            (".data", b"/shared2/title\x00", "lbl_80551240"),
            (".data", b"/title/00010000\x00", "lbl_805511D0"),
            (".data", b"/title/00010001\x00", "lbl_805511E0"),
            (".data", b"/title/00010003\x00", "lbl_805511F0"),
            (".data", b"/title/00010004\x00", "lbl_80551200"),
            (".data", b"/title/00010005\x00", "lbl_80551210"),
            (".data", b"/title/00010006\x00", "lbl_80551220"),
            (".data", b"/title/00010007\x00", "lbl_80551230"),
        ),
        drop_data_tail=((".data", 0xE7),),
    ),
    "CEquipItemBox.o": UnitRules(
        # Retail declares CItem_initItemImplInstances with C linkage
        # (unmangled reloc); this TU keeps the C++-linkage declaration to
        # coexist with CCol6System.hpp's overload-free spelling - rename the
        # mangled UNDEF to retail's plain symbol at post-process. Merged with
        # the dispatch-jumptable renames below (single exact_renames tuple).
        exact_renames=(
            ("CItem_initItemImplInstances__FPv", "CItem_initItemImplInstances"),
            ("@12277", "jumptable_eu_80538598"),
            ("@13378", "jumptable_eu_805385C0"),
            ("@13793", "jumptable_eu_805385F8"),
            ("@13932", "jumptable_eu_8053861C"),
        ),
        # pool-coupled: MWCC materializes its own s32->f32 conversion magic in
        # .sdata2 for (f32)(int) casts: 43300000_80000000 = signed correction
        # (2^52+2^31) and 43300000_00000000 = unsigned (2^52). Retail shares
        # the module pool at lbl_eu_80668B18 / lbl_eu_80668B10; rename to
        # those and strip local storage.
        pool_patterns=(
            (struct.pack(">II", 0x43300000, 0x80000000), "lbl_eu_80668B18"),
            (struct.pack(">II", 0x43300000, 0x00000000), "lbl_eu_80668B10"),
        ),
        # Data dissolve: the five dispatch jumptables for func_8028345C /
        # func_8028A374 / func_8028B7CC / func_8028D0EC ship from split1.s
        # (jumptable exact_renames merged into the tuple above.)
        extern_data_sections=(".sdata2", ".data"),
    ),
    "CETrail.o": UnitRules(
        set_data_align=((".sbss", 4),),
        # pool-coupled: lone int->double conversion magic (2^52 + 2^31,
        # 43300000_80000000) MWCC materializes in .sdata2 for func_804D8160's
        # s32->f32 clamp cast. Retail has no local copy - it loads the same
        # 8 bytes straight from the shared monolib .sdata2 pool via lfd
        # against lbl_eu_8066B1B0; rename to that and strip.
        pool_patterns=(
            (struct.pack(">II", 0x43300000, 0x80000000), "lbl_eu_8066B1B0"),
        ),
        extern_data_sections=(".sdata2",),
    ),
    "code_80468434.o": UnitRules(
        # monolibdata2 dissolve: retail .sbss slice align 4, MWCC emits 8.
        set_data_align=((".sbss", 4),),
    ),
    "CScnFilter.o": UnitRules(
        set_data_align=((".rodata", 4),),
        # vtable slot +0x0C: retail uses the flat label func_8049C868 (foreign
        # TUs declare it extern "C"); MWCC mangles our definition instead.
        exact_renames=(
            ("func_8049C868__FP10CScnFilterP14CScnFilterHost", "func_8049C868"),
        ),
        # func_8049C868 int->float conversions pool the 2^52 magic double
        # locally (@N); retail references the shared monolibdata label
        # lbl_eu_8066ABA8 from the CGXCache-owned .sdata2 slice. Retarget the
        # pool symtab entries onto the shared name and drop the local pool.
        pool_patterns=(
            (struct.pack(">II", 0x43300000, 0x00000000), "lbl_eu_8066ABA8"),
        ),
        drop_data_tail=((".sdata2", 0),),
    ),
    "CScnTexWorkMan.o": UnitRules(
        # MWCC 4-pads the assert/format string literals in .data; the retail
        # split keeps the unpadded st_size on each lbl_eu_ object. Trim the
        # symbol sizes to the retail bounds so functionRelocDiffs=data_value
        # sees identical reloc targets.
        symbol_sizes=(
            ("lbl_eu_8056E7F8", 38),
            ("lbl_eu_8056E820", 17),
            ("lbl_eu_8056E834", 26),
            ("lbl_eu_8056E850", 17),
        ),
    ),
    "CMdlAnmUV.o": UnitRules(
        # monolibdata2 dissolve: retail .sdata slice is align 4, MWCC emits 8.
        set_data_align=((".sdata", 4),),
    ),
    "CNBanner.o": UnitRules(
        # Compiler-emitted vtable/RTTI TU (derives IWorkEvent, overrides
        # OnFileEvent). MWCC orders .data [hand blobs][vtable][base list]
        # while retail interleaves [vtable][base list][probe vtable][tail];
        # every content word is a link-time reloc or zero, so the section
        # bytes already compare equal and only the reloc positions need
        # remapping onto the retail layout (data_reloc_offset_moves).
        # Hand-spelling IWorkEvent slot names in this TU trips MWCC 10322
        # under -ipa file, so the vtable itself is NOT hand-written.
        # NOTE: the map is not idempotent -- hexdiff re-applies rules to
        # build/us/src on every run; touch the cpp (force a fresh build)
        # before evaluating.
        data_reloc_offset_moves=(
            # probe vtable lbl_eu_80570410 + tail lbl_eu_80570420 -> 0x98/0xA8
            (".data", 0x00, 0x98),
            (".data", 0x08, 0xA0),
            (".data", 0x0C, 0xA4),
            (".data", 0x10, 0xA8),
            # __vt__8CNBanner: typeinfo ptr word + dtor/slot words -> -0x20
            (".data", 0x20, 0x00),
            (".data", 0x28, 0x08),
            (".data", 0x2C, 0x0C),
            (".data", 0x30, 0x10),
            (".data", 0x34, 0x14),
            (".data", 0x38, 0x18),
            (".data", 0x3C, 0x1C),
        )
        + tuple((".data", 0x40 + k * 4, 0x20 + k * 4) for k in range(26))
        + (
            # base list -> retail lbl_eu_80570400 slot @0x88
            (".data", 0xA8, 0x88),
        ),
        # The compiler's anonymous RTTI base-list local stays unnamed; the
        # retail name lbl_eu_80570400 is added at the moved base-list slot so
        # lbl_eu_80663CE0's .sdata UNDEF reference resolves to it at link.
        add_symbols=(
            ("lbl_eu_80570400", ".data", 0x88, 0x10),
        ),
        # Vtable word0 must reference the hand-written .sdata typeinfo pair.
        retarget_relocs=(
            (".data", 0x00, "lbl_eu_80663CE0"),
        ),
        # MWCC's local typeinfo copies (__RTTI__8CNBanner/__RTTI__10IWorkEvent)
        # trail the hand pairs; under -sdata 8 they land in .sdata (raw),
        # otherwise in .data. Retail keeps neither in this split.
        drop_data_range=((".data", 0xB8, 0xC8),),
        drop_data_tail=((".rodata", 0x23), (".sdata", 0x10),),
        # FORCEACTIVE keep-alive stub for lbl_eu_80665A98 (.sbss) -- retail's
        # split carries no such function.
        drop_text_symbols=("FORCEACTIVECNBanner_cpp385__Fv",),
        repack_after_drop=4,
    ),
    "CNReqtaskSave.o": UnitRules(
        # Retail .rodata slice is align 4 (all string literals, no aligned
        # member); MWCC emits align 8. Same as CNReqtaskSaveBanner.
        set_data_align=((".rodata", 4),),
        # Retail .sbss keeps a 3-byte anonymous linker gap at +1..+4
        # (gap_10_806659D1_sbss, between the 1-byte lbl_eu_806659D0 and the
        # 4-aligned lbl_eu_806659D4); MWCC packs .sbss with no interior pad,
        # so its section is 0x15 vs retail 0x18. Every symbol offset already
        # matches - just grow sh_size to 0x18 (NOBITS: no file bytes).
        pad_data_section=((".sbss", 0x18),),
    ),
    "CNReqtaskSaveBanner.o": UnitRules(
        set_data_align=((".rodata", 4),),
    ),
    "UnkClass_80460C34.o": UnitRules(
        # retail .data 0x8056D5B8-0x8056D630 = 0x78; MWCC emits 0x74 (4B splitter pad).
        pad_data_section=((".data", 0x78),),
        # func_80460F58's switch jumptable: MWCC auto-emits it byte-identical to
        # retail but under the internal @1832 name (see cpp comment at the .data
        # block). Rename onto the retail jumptable_eu_8056D600 name so the two
        # bcctr-dispatch relocs match (line-3328 precedent). Remaining residual:
        # slot-27 lbl_80526B70 ADDR16_HI vs ADDR16_HA address-formation shape.
        exact_renames=(
            ("@1832", "jumptable_eu_8056D600"),
        ),
    ),
    "UnkClass_8046368C.o": UnitRules(
        # Retail split .data is align 4; MWCC emits align 8 for the fn-ptr
        # table (the __declspec(align(4)) in the cpp loses to the earlier
        # unaligned extern declaration), so force it here.
        set_data_align=((".data", 4),),
    ),
    "UnkClass_8047E110.o": UnitRules(
        # pool-coupled: this TU plants the conversion-magic double and the 1.0f
        # constant under their retail shared-pool names so the s32->f32 paths
        # resolve to lbl_eu_8066A8A0/lbl_eu_8066A890; MWCC still materializes
        # identical anonymous .sdata2 literals (@3613 = 3F800000 1.0f x7 refs,
        # @3658 = 43300000_80000000 magic x14 refs). Rename both onto the
        # in-pool retail names (same bytes as the CGXCache pool entries at
        # 0x8066A890/0x8066A8A0) and strip .sdata2 storage to externs —
        # retail UnkClass_8047E110.o carries no .sdata2 of its own.
        pool_patterns=(
            (struct.pack(">I", 0x3F800000), "lbl_eu_8066A890"),
            (struct.pack(">II", 0x43300000, 0x80000000), "lbl_eu_8066A8A0"),
        ),
        extern_data_sections=(".sdata2",),
    ),

    "UnkClass_80471EC8.o": UnitRules(
        # pool-coupled: local .sdata2 pool -> CGXCache pool symbols (lbl_eu_8066Axxx).
        # Retail UnkClass_80471EC8.o carries no .sdata2 of its own; MWCC still
        # materializes anonymous literals for the int->double conversion paths
        # and the (f32)1/(f32)2 casts in func_804724DC. Rename each onto the
        # same-bytes CGXCache pool entry the retail code resolves against and
        # strip the section to externs:
        #   @8314 3F800000 1.0f          -> lbl_eu_8066A750 (retail 1.0f here)
        #   @8315 40000000 2.0f          -> lbl_eu_8066A39C (pool 2.0f)
        #   @8318 43300000_80000000      -> lbl_eu_8066A768 (signed conv magic)
        #   @8539 00000000_00000000      -> lbl_eu_8066A744 (0.0f, 8B zero slot)
        #   @8581 43300000_00000000      -> lbl_eu_8066A7C8 (unsigned conv magic)
        pool_patterns=(
            (struct.pack(">I", 0x3F800000), "lbl_eu_8066A750"),
            (struct.pack(">I", 0x40000000), "lbl_eu_8066A39C"),
            (struct.pack(">II", 0x43300000, 0x80000000), "lbl_eu_8066A768"),
            (struct.pack(">II", 0x00000000, 0x00000000), "lbl_eu_8066A744"),
            (struct.pack(">II", 0x43300000, 0x00000000), "lbl_eu_8066A7C8"),
        ),
        extern_data_sections=(".sdata2",),
    ),

    "CLibCriStreamingPlay.o": UnitRules(
        # Defensive (UnkClass_80471EC8 template): MWCC's int->float conversion
        # magic (2^52 + 2^31, 43300000_80000000) flickers between resolving
        # against the shared monolibdata2 pool (lbl_eu_8066A518) and
        # materializing a TU-local .sdata2 literal depending on unrelated TU
        # perturbations. When the local copy appears, rename it onto the shared
        # retail symbol and strip the section.
        pool_patterns=(
            (struct.pack(">II", 0x43300000, 0x80000000), "lbl_eu_8066A518"),
        ),
        extern_data_sections=(".sdata2",),
    ),
    "CGXCache.o": UnitRules(
        # monolibdata2 dissolve: CGXCache owns the shared .sdata2 pool
        # 0x8066A378-0x8066B52E (0x11B6) + .data/.rodata/.sdata slices.
        # Named inner symbols (link side): other TUs extern-ref these
        # lbl_eu_8066Axxx; the source defines them (or declares UNDEF
        # externs) and add_local_symbol redefines each in place.
        # .sdata2 tail (0x11B6..0x11C8) = 2 anonymous magic doubles pooled by
        # FULL_MATCH int->f32 conversions (@4441 s32:43300000 80000000,
        # @4654 u32:43300000 00000000). Retarget their SHARED symtab entries
        # onto the in-pool lbl_eu_8066A388/A390 (pool+0x10/+0x18, same bytes);
        # add_symbols then redefines those UNDEFs in the pool, and
        # drop_data_tail trims the section back to the retail 0x11B6. No
        # instruction bytes change, so the FULL_MATCH functions stay matched.
        retarget_relocs=(
            (".text", 0x08CC, "lbl_eu_8066A388"),
            (".text", 0x1A48, "lbl_eu_8066A390"),
        ),
        # func_80449D68 jumptable (updateMsg switch): HIGH_MATCH §17.6
        # code-length residual — layout tracks the concurrent agent's
        # in-progress rewrites of the function body; deltas below verified
        # against a FRESH RAW build of the current source (do not diagnose
        # against the shared build/ object — hexdiff leaves it postprocessed
        # in place). Patch r_addend to retail here.
        # 2026-08-25 (re-derived from fresh build): sel3..13 raw == retail-4
        # (case3/case2 bodies each one insn short of retail); sel0..2 exact.
        # 2026-08-25 re-derivation via marker experiment (bake applies once;
        # data-diff temp-copy applies a second time — never diagnose against
        # an already-baked shared object): raw sels 3 (@12) and 0..2 are
        # EXACT vs retail; sels 4..13 (@16..52) are uniformly retail-4.
        addend_patches=(
            (".data", 16, 4),
            (".data", 20, 4),
            (".data", 24, 4),
            (".data", 28, 4),
            (".data", 32, 4),
            (".data", 36, 4),
            (".data", 40, 4),
            (".data", 44, 4),
            (".data", 48, 4),
            (".data", 52, 4),
        ),
        drop_data_tail=((".sdata2", 0x11B6),),

        add_symbols=(
            # .data: 5 symbols
            ("jumptable_eu_8056BF90", ".data", 0, 56),
            ("lbl_eu_8056BFC8", ".data", 56, 16),
            ("lbl_eu_8056BFD8", ".data", 72, 12),
            ("lbl_eu_8056BFE4", ".data", 84, 12),
            ("lbl_eu_8056BFF0", ".data", 96, 16),
            # .rodata: 9 symbols
            ("lbl_eu_80522A60", ".rodata", 0, 9),
            ("gap_06_80522A69_rodata", ".rodata", 9, 3),
            ("lbl_eu_80522A6C", ".rodata", 12, 14),
            ("gap_06_80522A7A_rodata", ".rodata", 26, 2),
            ("lbl_eu_80522A7C", ".rodata", 28, 12),
            ("lbl_eu_80522A88", ".rodata", 40, 14),
            ("gap_06_80522A96_rodata", ".rodata", 54, 2),
            ("lbl_eu_80522A98", ".rodata", 56, 14),
            ("gap_06_80522AA6_rodata", ".rodata", 70, 2),
            # .sdata: 20 symbols
            ("lbl_eu_80663620", ".sdata", 0, 4),
            ("lbl_eu_80663624", ".sdata", 4, 4),
            ("lbl_eu_80663628", ".sdata", 8, 4),
            ("lbl_eu_8066362C", ".sdata", 12, 4),
            ("lbl_eu_80663630", ".sdata", 16, 4),
            ("lbl_eu_80663634", ".sdata", 20, 4),
            ("lbl_eu_80663638", ".sdata", 24, 4),
            ("lbl_eu_8066363C", ".sdata", 28, 4),
            ("lbl_eu_80663640", ".sdata", 32, 4),
            ("lbl_eu_80663644", ".sdata", 36, 4),
            ("lbl_eu_80663648", ".sdata", 40, 4),
            ("lbl_eu_8066364C", ".sdata", 44, 4),
            ("lbl_eu_80663650", ".sdata", 48, 8),
            ("lbl_eu_80663658", ".sdata", 56, 8),
            ("lbl_eu_80663660", ".sdata", 64, 8),
            ("lbl_eu_80663668", ".sdata", 72, 8),
            ("lbl_eu_80663670", ".sdata", 80, 4),
            ("lbl_eu_80663674", ".sdata", 84, 4),
            ("lbl_eu_80663678", ".sdata", 88, 4),
            ("lbl_eu_8066367C", ".sdata", 92, 4),
            # .sdata2: 938 symbols
            ("lbl_eu_8066A378", ".sdata2", 0, 4),
            ("lbl_eu_8066A37C", ".sdata2", 4, 4),
            ("lbl_eu_8066A380", ".sdata2", 8, 4),
            ("lbl_eu_8066A388", ".sdata2", 16, 8),
            ("lbl_eu_8066A390", ".sdata2", 24, 8),
            ("lbl_eu_8066A398", ".sdata2", 32, 4),
            ("lbl_eu_8066A39C", ".sdata2", 36, 4),
            ("lbl_eu_8066A3A0", ".sdata2", 40, 4),
            ("lbl_eu_8066A3A4", ".sdata2", 44, 4),
            ("lbl_eu_8066A3A8", ".sdata2", 48, 8),
            ("lbl_eu_8066A3B0", ".sdata2", 56, 8),
            ("lbl_eu_8066A3B8", ".sdata2", 64, 8),
            ("lbl_eu_8066A3C0", ".sdata2", 72, 8),
            ("lbl_eu_8066A3C8", ".sdata2", 80, 4),
            ("lbl_eu_8066A3CC", ".sdata2", 84, 4),
            ("lbl_eu_8066A3D0", ".sdata2", 88, 4),
            ("lbl_eu_8066A3D4", ".sdata2", 92, 4),
            ("lbl_eu_8066A3D8", ".sdata2", 96, 4),
            ("lbl_eu_8066A3DC", ".sdata2", 100, 4),
            ("lbl_eu_8066A3E0", ".sdata2", 104, 4),
            ("lbl_eu_8066A3E4", ".sdata2", 108, 4),
            ("lbl_eu_8066A3E8", ".sdata2", 112, 4),
            ("lbl_eu_8066A3EC", ".sdata2", 116, 4),
            ("lbl_eu_8066A3F0", ".sdata2", 120, 4),
            ("lbl_eu_8066A3F4", ".sdata2", 124, 4),
            ("lbl_eu_8066A3F8", ".sdata2", 128, 4),
            ("lbl_eu_8066A3FC", ".sdata2", 132, 4),
            ("lbl_eu_8066A400", ".sdata2", 136, 8),
            ("lbl_eu_8066A408", ".sdata2", 144, 4),
            ("lbl_eu_8066A40C", ".sdata2", 148, 4),
            ("lbl_eu_8066A410", ".sdata2", 152, 4),
            ("lbl_eu_8066A418", ".sdata2", 160, 8),
            ("lbl_eu_8066A420", ".sdata2", 168, 8),
            ("lbl_eu_8066A428", ".sdata2", 176, 7),
            ("lbl_eu_8066A430", ".sdata2", 184, 4),
            ("lbl_eu_8066A434", ".sdata2", 188, 4),
            ("lbl_eu_8066A438", ".sdata2", 192, 4),
            ("lbl_eu_8066A440", ".sdata2", 200, 8),
            ("lbl_eu_8066A448", ".sdata2", 208, 8),
            ("lbl_eu_8066A450", ".sdata2", 216, 8),
            ("lbl_eu_8066A458", ".sdata2", 224, 4),
            ("lbl_eu_8066A45C", ".sdata2", 228, 4),
            ("lbl_eu_8066A460", ".sdata2", 232, 8),
            ("lbl_eu_8066A468", ".sdata2", 240, 4),
            ("lbl_eu_8066A46C", ".sdata2", 244, 4),
            ("lbl_eu_8066A470", ".sdata2", 248, 4),
            ("lbl_eu_8066A474", ".sdata2", 252, 4),
            ("lbl_eu_8066A478", ".sdata2", 256, 8),
            ("lbl_eu_8066A480", ".sdata2", 264, 4),
            ("lbl_eu_8066A484", ".sdata2", 268, 4),
            ("lbl_eu_8066A488", ".sdata2", 272, 4),
            ("lbl_eu_8066A48C", ".sdata2", 276, 4),
            ("lbl_eu_8066A490", ".sdata2", 280, 4),
            ("lbl_eu_8066A494", ".sdata2", 284, 4),
            ("lbl_eu_8066A498", ".sdata2", 288, 4),
            ("lbl_eu_8066A49C", ".sdata2", 292, 4),
            ("lbl_eu_8066A4A0", ".sdata2", 296, 4),
            ("lbl_eu_8066A4A4", ".sdata2", 300, 4),
            ("lbl_eu_8066A4A8", ".sdata2", 304, 4),
            ("lbl_eu_8066A4B0", ".sdata2", 312, 8),
            ("lbl_eu_8066A4B8", ".sdata2", 320, 8),
            ("lbl_eu_8066A4C0", ".sdata2", 328, 8),
            ("lbl_eu_8066A4C8", ".sdata2", 336, 5),
            ("gap_11_8066A4CD_sdata2", ".sdata2", 341, 3),
            ("lbl_eu_8066A4D0", ".sdata2", 344, 8),
            ("lbl_eu_8066A4D8", ".sdata2", 352, 4),
            ("lbl_eu_8066A4DC", ".sdata2", 356, 4),
            ("lbl_eu_8066A4E0", ".sdata2", 360, 4),
            ("lbl_eu_8066A4E4", ".sdata2", 364, 4),
            ("lbl_eu_8066A4E8", ".sdata2", 368, 4),
            ("lbl_eu_8066A4EC", ".sdata2", 372, 4),
            ("lbl_eu_8066A4F0", ".sdata2", 376, 4),
            ("lbl_eu_8066A4F4", ".sdata2", 380, 4),
            ("lbl_eu_8066A4F8", ".sdata2", 384, 4),
            ("lbl_eu_8066A4FC", ".sdata2", 388, 4),
            ("lbl_eu_8066A500", ".sdata2", 392, 5),
            ("lbl_eu_8066A508", ".sdata2", 400, 4),
            ("lbl_eu_8066A50C", ".sdata2", 404, 4),
            ("lbl_eu_8066A510", ".sdata2", 408, 4),
            ("lbl_eu_8066A518", ".sdata2", 416, 8),
            ("lbl_eu_8066A520", ".sdata2", 424, 4),
            ("lbl_eu_8066A524", ".sdata2", 428, 4),
            ("lbl_eu_8066A528", ".sdata2", 432, 8),
            ("lbl_eu_8066A530", ".sdata2", 440, 4),
            ("lbl_eu_8066A534", ".sdata2", 444, 4),
            ("lbl_eu_8066A538", ".sdata2", 448, 4),
            ("lbl_eu_8066A53C", ".sdata2", 452, 4),
            ("lbl_eu_8066A540", ".sdata2", 456, 8),
            ("lbl_eu_8066A548", ".sdata2", 464, 4),
            ("lbl_eu_8066A54C", ".sdata2", 468, 4),
            ("lbl_eu_8066A550", ".sdata2", 472, 4),
            ("lbl_eu_8066A554", ".sdata2", 476, 4),
            ("lbl_eu_8066A558", ".sdata2", 480, 4),
            ("lbl_eu_8066A560", ".sdata2", 488, 8),
            ("lbl_eu_8066A568", ".sdata2", 496, 8),
            ("lbl_eu_8066A570", ".sdata2", 504, 4),
            ("lbl_eu_8066A574", ".sdata2", 508, 4),
            ("lbl_eu_8066A578", ".sdata2", 512, 4),
            ("lbl_eu_8066A57C", ".sdata2", 516, 4),
            ("lbl_eu_8066A580", ".sdata2", 520, 4),
            ("lbl_eu_8066A584", ".sdata2", 524, 4),
            ("lbl_eu_8066A588", ".sdata2", 528, 4),
            ("lbl_eu_8066A58C", ".sdata2", 532, 4),
            ("lbl_eu_8066A590", ".sdata2", 536, 4),
            ("lbl_eu_8066A594", ".sdata2", 540, 4),
            ("lbl_eu_8066A598", ".sdata2", 544, 4),
            ("lbl_eu_8066A59C", ".sdata2", 548, 4),
            ("lbl_eu_8066A5A0", ".sdata2", 552, 4),
            ("lbl_eu_8066A5A8", ".sdata2", 560, 8),
            ("lbl_eu_8066A5B0", ".sdata2", 568, 7),
            ("lbl_eu_8066A5B8", ".sdata2", 576, 4),
            ("lbl_eu_8066A5BC", ".sdata2", 580, 4),
            ("lbl_eu_8066A5C0", ".sdata2", 584, 4),
            ("lbl_eu_8066A5C4", ".sdata2", 588, 4),
            ("lbl_eu_8066A5C8", ".sdata2", 592, 8),
            ("lbl_eu_8066A5D0", ".sdata2", 600, 8),
            ("lbl_eu_8066A5D8", ".sdata2", 608, 4),
            ("lbl_eu_8066A5DC", ".sdata2", 612, 4),
            ("lbl_eu_8066A5E0", ".sdata2", 616, 4),
            ("lbl_eu_8066A5E4", ".sdata2", 620, 4),
            ("lbl_eu_8066A5E8", ".sdata2", 624, 8),
            ("lbl_eu_8066A5F0", ".sdata2", 632, 4),
            ("lbl_eu_8066A5F4", ".sdata2", 636, 4),
            ("lbl_eu_8066A5F8", ".sdata2", 640, 4),
            ("lbl_eu_8066A5FC", ".sdata2", 644, 4),
            ("lbl_eu_8066A600", ".sdata2", 648, 8),
            ("lbl_eu_8066A608", ".sdata2", 656, 8),
            ("lbl_eu_8066A610", ".sdata2", 664, 8),
            ("lbl_eu_8066A618", ".sdata2", 672, 4),
            ("lbl_eu_8066A61C", ".sdata2", 676, 4),
            ("lbl_eu_8066A620", ".sdata2", 680, 4),
            ("lbl_eu_8066A624", ".sdata2", 684, 4),
            ("lbl_eu_8066A628", ".sdata2", 688, 8),
            ("lbl_eu_8066A630", ".sdata2", 696, 4),
            ("lbl_eu_8066A638", ".sdata2", 704, 8),
            ("lbl_eu_8066A640", ".sdata2", 712, 4),
            ("lbl_eu_8066A644", ".sdata2", 716, 4),
            ("lbl_eu_8066A648", ".sdata2", 720, 4),
            ("lbl_eu_8066A64C", ".sdata2", 724, 4),
            ("lbl_eu_8066A650", ".sdata2", 728, 8),
            ("lbl_eu_8066A658", ".sdata2", 736, 4),
            ("lbl_eu_8066A65C", ".sdata2", 740, 4),
            ("lbl_eu_8066A660", ".sdata2", 744, 4),
            ("lbl_eu_8066A664", ".sdata2", 748, 4),
            ("lbl_eu_8066A668", ".sdata2", 752, 4),
            ("lbl_eu_8066A66C", ".sdata2", 756, 4),
            ("lbl_eu_8066A670", ".sdata2", 760, 4),
            ("lbl_eu_8066A674", ".sdata2", 764, 4),
            ("lbl_eu_8066A678", ".sdata2", 768, 8),
            ("lbl_eu_8066A680", ".sdata2", 776, 4),
            ("lbl_eu_8066A684", ".sdata2", 780, 4),
            ("lbl_eu_8066A688", ".sdata2", 784, 4),
            ("lbl_eu_8066A68C", ".sdata2", 788, 4),
            ("lbl_eu_8066A690", ".sdata2", 792, 4),
            ("lbl_eu_8066A694", ".sdata2", 796, 4),
            ("lbl_eu_8066A698", ".sdata2", 800, 4),
            ("lbl_eu_8066A69C", ".sdata2", 804, 4),
            ("lbl_eu_8066A6A0", ".sdata2", 808, 4),
            ("lbl_eu_8066A6A4", ".sdata2", 812, 4),
            ("lbl_eu_8066A6A8", ".sdata2", 816, 4),
            ("lbl_eu_8066A6AC", ".sdata2", 820, 4),
            ("lbl_eu_8066A6B0", ".sdata2", 824, 4),
            ("lbl_eu_8066A6B4", ".sdata2", 828, 4),
            ("lbl_eu_8066A6B8", ".sdata2", 832, 4),
            ("lbl_eu_8066A6BC", ".sdata2", 836, 4),
            ("lbl_eu_8066A6C0", ".sdata2", 840, 4),
            ("lbl_eu_8066A6C4", ".sdata2", 844, 4),
            ("lbl_eu_8066A6C8", ".sdata2", 848, 8),
            ("lbl_eu_8066A6D0", ".sdata2", 856, 8),
            ("lbl_eu_8066A6D8", ".sdata2", 864, 4),
            ("lbl_eu_8066A6E0", ".sdata2", 872, 8),
            ("lbl_eu_8066A6E8", ".sdata2", 880, 8),
            ("lbl_eu_8066A6F0", ".sdata2", 888, 4),
            ("lbl_eu_8066A6F4", ".sdata2", 892, 4),
            ("lbl_eu_8066A6F8", ".sdata2", 896, 4),
            ("lbl_eu_8066A6FC", ".sdata2", 900, 4),
            ("lbl_eu_8066A700", ".sdata2", 904, 4),
            ("lbl_eu_8066A704", ".sdata2", 908, 4),
            ("lbl_eu_8066A708", ".sdata2", 912, 4),
            ("lbl_eu_8066A70C", ".sdata2", 916, 4),
            ("lbl_eu_8066A710", ".sdata2", 920, 4),
            ("lbl_eu_8066A714", ".sdata2", 924, 4),
            ("lbl_eu_8066A718", ".sdata2", 928, 4),
            ("lbl_eu_8066A71C", ".sdata2", 932, 4),
            ("lbl_eu_8066A720", ".sdata2", 936, 8),
            ("lbl_eu_8066A728", ".sdata2", 944, 4),
            ("lbl_eu_8066A72C", ".sdata2", 948, 4),
            ("lbl_eu_8066A730", ".sdata2", 952, 4),
            ("lbl_eu_8066A734", ".sdata2", 956, 4),
            ("lbl_eu_8066A738", ".sdata2", 960, 4),
            ("lbl_eu_8066A73C", ".sdata2", 964, 4),
            ("lbl_eu_8066A740", ".sdata2", 968, 4),
            ("lbl_eu_8066A744", ".sdata2", 972, 4),
            ("lbl_eu_8066A748", ".sdata2", 976, 4),
            ("lbl_eu_8066A74C", ".sdata2", 980, 4),
            ("lbl_eu_8066A750", ".sdata2", 984, 4),
            ("lbl_eu_8066A754", ".sdata2", 988, 4),
            ("lbl_eu_8066A758", ".sdata2", 992, 4),
            ("lbl_eu_8066A75C", ".sdata2", 996, 4),
            ("lbl_eu_8066A760", ".sdata2", 1000, 4),
            ("lbl_eu_8066A764", ".sdata2", 1004, 4),
            ("lbl_eu_8066A768", ".sdata2", 1008, 8),
            ("lbl_eu_8066A770", ".sdata2", 1016, 4),
            ("lbl_eu_8066A774", ".sdata2", 1020, 4),
            ("lbl_eu_8066A778", ".sdata2", 1024, 4),
            ("lbl_eu_8066A77C", ".sdata2", 1028, 4),
            ("lbl_eu_8066A780", ".sdata2", 1032, 4),
            ("lbl_eu_8066A784", ".sdata2", 1036, 4),
            ("lbl_eu_8066A788", ".sdata2", 1040, 4),
            ("lbl_eu_8066A78C", ".sdata2", 1044, 4),
            ("lbl_eu_8066A790", ".sdata2", 1048, 4),
            ("lbl_eu_8066A794", ".sdata2", 1052, 4),
            ("lbl_eu_8066A798", ".sdata2", 1056, 8),
            ("lbl_eu_8066A7A0", ".sdata2", 1064, 4),
            ("lbl_eu_8066A7A4", ".sdata2", 1068, 4),
            ("lbl_eu_8066A7A8", ".sdata2", 1072, 4),
            ("lbl_eu_8066A7AC", ".sdata2", 1076, 4),
            ("lbl_eu_8066A7B0", ".sdata2", 1080, 4),
            ("lbl_eu_8066A7B4", ".sdata2", 1084, 4),
            ("lbl_eu_8066A7B8", ".sdata2", 1088, 4),
            ("lbl_eu_8066A7BC", ".sdata2", 1092, 4),
            ("lbl_eu_8066A7C0", ".sdata2", 1096, 4),
            ("lbl_eu_8066A7C8", ".sdata2", 1104, 8),
            ("lbl_eu_8066A7D0", ".sdata2", 1112, 4),
            ("lbl_eu_8066A7D4", ".sdata2", 1116, 4),
            ("lbl_eu_8066A7D8", ".sdata2", 1120, 8),
            ("lbl_eu_8066A7E0", ".sdata2", 1128, 4),
            ("lbl_eu_8066A7E4", ".sdata2", 1132, 4),
            ("lbl_eu_8066A7E8", ".sdata2", 1136, 4),
            ("lbl_eu_8066A7EC", ".sdata2", 1140, 4),
            ("lbl_eu_8066A7F0", ".sdata2", 1144, 4),
            ("lbl_eu_8066A7F4", ".sdata2", 1148, 4),
            ("lbl_eu_8066A7F8", ".sdata2", 1152, 8),
            ("lbl_eu_8066A800", ".sdata2", 1160, 4),
            ("lbl_eu_8066A804", ".sdata2", 1164, 4),
            ("lbl_eu_8066A808", ".sdata2", 1168, 8),
            ("lbl_eu_8066A810", ".sdata2", 1176, 4),
            ("lbl_eu_8066A814", ".sdata2", 1180, 4),
            ("lbl_eu_8066A818", ".sdata2", 1184, 4),
            ("lbl_eu_8066A81C", ".sdata2", 1188, 4),
            ("lbl_eu_8066A820", ".sdata2", 1192, 4),
            ("lbl_eu_8066A824", ".sdata2", 1196, 4),
            ("lbl_eu_8066A828", ".sdata2", 1200, 4),
            ("lbl_eu_8066A82C", ".sdata2", 1204, 4),
            ("lbl_eu_8066A830", ".sdata2", 1208, 4),
            ("lbl_eu_8066A834", ".sdata2", 1212, 4),
            ("lbl_eu_8066A838", ".sdata2", 1216, 4),
            ("lbl_eu_8066A83C", ".sdata2", 1220, 4),
            ("lbl_eu_8066A840", ".sdata2", 1224, 4),
            ("lbl_eu_8066A844", ".sdata2", 1228, 4),
            ("lbl_eu_8066A848", ".sdata2", 1232, 4),
            ("lbl_eu_8066A84C", ".sdata2", 1236, 4),
            ("lbl_eu_8066A850", ".sdata2", 1240, 8),
            ("lbl_eu_8066A858", ".sdata2", 1248, 8),
            ("lbl_eu_8066A860", ".sdata2", 1256, 4),
            ("lbl_eu_8066A864", ".sdata2", 1260, 4),
            ("lbl_eu_8066A868", ".sdata2", 1264, 8),
            ("lbl_eu_8066A870", ".sdata2", 1272, 4),
            ("lbl_eu_8066A874", ".sdata2", 1276, 4),
            ("lbl_eu_8066A878", ".sdata2", 1280, 4),
            ("lbl_eu_8066A87C", ".sdata2", 1284, 4),
            ("lbl_eu_8066A880", ".sdata2", 1288, 4),
            ("lbl_eu_8066A884", ".sdata2", 1292, 4),
            ("lbl_eu_8066A888", ".sdata2", 1296, 8),
            ("lbl_eu_8066A890", ".sdata2", 1304, 4),
            ("lbl_eu_8066A894", ".sdata2", 1308, 4),
            ("lbl_eu_8066A898", ".sdata2", 1312, 4),
            ("lbl_eu_8066A8A0", ".sdata2", 1320, 8),
            ("lbl_eu_8066A8A8", ".sdata2", 1328, 4),
            ("lbl_eu_8066A8AC", ".sdata2", 1332, 4),
            ("lbl_eu_8066A8B0", ".sdata2", 1336, 4),
            ("lbl_eu_8066A8B4", ".sdata2", 1340, 4),
            ("lbl_eu_8066A8B8", ".sdata2", 1344, 4),
            ("lbl_eu_8066A8BC", ".sdata2", 1348, 4),
            ("lbl_eu_8066A8C0", ".sdata2", 1352, 4),
            ("lbl_eu_8066A8C4", ".sdata2", 1356, 4),
            ("lbl_eu_8066A8C8", ".sdata2", 1360, 4),
            ("lbl_eu_8066A8CC", ".sdata2", 1364, 4),
            ("lbl_eu_8066A8D0", ".sdata2", 1368, 6),
            ("gap_11_8066A8D6_sdata2", ".sdata2", 1374, 2),
            ("lbl_eu_8066A8D8", ".sdata2", 1376, 5),
            ("lbl_eu_8066A8E0", ".sdata2", 1384, 4),
            ("lbl_eu_8066A8E4", ".sdata2", 1388, 4),
            ("lbl_eu_8066A8E8", ".sdata2", 1392, 4),
            ("lbl_eu_8066A8F0", ".sdata2", 1400, 8),
            ("lbl_eu_8066A8F8", ".sdata2", 1408, 4),
            ("lbl_eu_8066A8FC", ".sdata2", 1412, 4),
            ("lbl_eu_8066A900", ".sdata2", 1416, 4),
            ("lbl_eu_8066A904", ".sdata2", 1420, 4),
            ("lbl_eu_8066A908", ".sdata2", 1424, 4),
            ("lbl_eu_8066A90C", ".sdata2", 1428, 4),
            ("lbl_eu_8066A910", ".sdata2", 1432, 4),
            ("lbl_eu_8066A914", ".sdata2", 1436, 4),
            ("lbl_eu_8066A918", ".sdata2", 1440, 4),
            ("lbl_eu_8066A91C", ".sdata2", 1444, 4),
            ("lbl_eu_8066A920", ".sdata2", 1448, 4),
            ("lbl_eu_8066A924", ".sdata2", 1452, 4),
            ("lbl_eu_8066A928", ".sdata2", 1456, 8),
            ("lbl_eu_8066A930", ".sdata2", 1464, 5),
            ("gap_11_8066A935_sdata2", ".sdata2", 1469, 3),
            ("lbl_eu_8066A938", ".sdata2", 1472, 5),
            ("lbl_eu_8066A940", ".sdata2", 1480, 4),
            ("lbl_eu_8066A944", ".sdata2", 1484, 4),
            ("lbl_eu_8066A948", ".sdata2", 1488, 4),
            ("lbl_eu_8066A950", ".sdata2", 1496, 8),
            ("lbl_eu_8066A958", ".sdata2", 1504, 5),
            ("gap_11_8066A95D_sdata2", ".sdata2", 1509, 3),
            ("lbl_eu_8066A960", ".sdata2", 1512, 7),
            ("gap_11_8066A967_sdata2", ".sdata2", 1519, 1),
            ("lbl_eu_8066A968", ".sdata2", 1520, 7),
            ("gap_11_8066A96F_sdata2", ".sdata2", 1527, 1),
            ("lbl_eu_8066A970", ".sdata2", 1528, 7),
            ("gap_11_8066A977_sdata2", ".sdata2", 1535, 1),
            ("lbl_eu_8066A978", ".sdata2", 1536, 7),
            ("gap_11_8066A97F_sdata2", ".sdata2", 1543, 1),
            ("lbl_eu_8066A980", ".sdata2", 1544, 7),
            ("gap_11_8066A987_sdata2", ".sdata2", 1551, 1),
            ("lbl_eu_8066A988", ".sdata2", 1552, 7),
            ("lbl_eu_8066A990", ".sdata2", 1560, 4),
            ("lbl_eu_8066A994", ".sdata2", 1564, 4),
            ("lbl_eu_8066A998", ".sdata2", 1568, 4),
            ("lbl_eu_8066A99C", ".sdata2", 1572, 4),
            ("lbl_eu_8066A9A0", ".sdata2", 1576, 4),
            ("lbl_eu_8066A9A8", ".sdata2", 1584, 8),
            ("lbl_eu_8066A9B0", ".sdata2", 1592, 8),
            ("lbl_eu_8066A9B8", ".sdata2", 1600, 4),
            ("lbl_eu_8066A9BC", ".sdata2", 1604, 4),
            ("lbl_eu_8066A9C0", ".sdata2", 1608, 4),
            ("lbl_eu_8066A9C4", ".sdata2", 1612, 4),
            ("lbl_eu_8066A9C8", ".sdata2", 1616, 4),
            ("lbl_eu_8066A9CC", ".sdata2", 1620, 4),
            ("lbl_eu_8066A9D0", ".sdata2", 1624, 8),
            ("lbl_eu_8066A9D8", ".sdata2", 1632, 8),
            ("lbl_eu_8066A9E0", ".sdata2", 1640, 5),
            ("lbl_eu_8066A9E8", ".sdata2", 1648, 4),
            ("lbl_eu_8066A9EC", ".sdata2", 1652, 4),
            ("lbl_eu_8066A9F0", ".sdata2", 1656, 4),
            ("lbl_eu_8066A9F4", ".sdata2", 1660, 4),
            ("lbl_eu_8066A9F8", ".sdata2", 1664, 8),
            ("lbl_eu_8066AA00", ".sdata2", 1672, 4),
            ("lbl_eu_8066AA04", ".sdata2", 1676, 4),
            ("lbl_eu_8066AA08", ".sdata2", 1680, 8),
            ("lbl_eu_8066AA10", ".sdata2", 1688, 4),
            ("lbl_eu_8066AA14", ".sdata2", 1692, 4),
            ("lbl_eu_8066AA18", ".sdata2", 1696, 4),
            ("lbl_eu_8066AA1C", ".sdata2", 1700, 4),
            ("lbl_eu_8066AA20", ".sdata2", 1704, 4),
            ("lbl_eu_8066AA24", ".sdata2", 1708, 4),
            ("lbl_eu_8066AA28", ".sdata2", 1712, 4),
            ("lbl_eu_8066AA2C", ".sdata2", 1716, 4),
            ("lbl_eu_8066AA30", ".sdata2", 1720, 4),
            ("lbl_eu_8066AA34", ".sdata2", 1724, 4),
            ("lbl_eu_8066AA38", ".sdata2", 1728, 4),
            ("lbl_eu_8066AA3C", ".sdata2", 1732, 4),
            ("lbl_eu_8066AA40", ".sdata2", 1736, 4),
            ("lbl_eu_8066AA44", ".sdata2", 1740, 4),
            ("lbl_eu_8066AA48", ".sdata2", 1744, 4),
            ("lbl_eu_8066AA4C", ".sdata2", 1748, 4),
            ("lbl_eu_8066AA50", ".sdata2", 1752, 4),
            ("lbl_eu_8066AA54", ".sdata2", 1756, 4),
            ("lbl_eu_8066AA58", ".sdata2", 1760, 4),
            ("lbl_eu_8066AA5C", ".sdata2", 1764, 4),
            ("lbl_eu_8066AA60", ".sdata2", 1768, 4),
            ("lbl_eu_8066AA64", ".sdata2", 1772, 4),
            ("lbl_eu_8066AA68", ".sdata2", 1776, 4),
            ("lbl_eu_8066AA6C", ".sdata2", 1780, 4),
            ("lbl_eu_8066AA70", ".sdata2", 1784, 4),
            ("lbl_eu_8066AA74", ".sdata2", 1788, 4),
            ("lbl_eu_8066AA78", ".sdata2", 1792, 8),
            ("lbl_eu_8066AA80", ".sdata2", 1800, 8),
            ("lbl_eu_8066AA88", ".sdata2", 1808, 4),
            ("lbl_eu_8066AA8C", ".sdata2", 1812, 4),
            ("lbl_eu_8066AA90", ".sdata2", 1816, 8),
            ("lbl_eu_8066AA98", ".sdata2", 1824, 4),
            ("lbl_eu_8066AA9C", ".sdata2", 1828, 4),
            ("lbl_eu_8066AAA0", ".sdata2", 1832, 8),
            ("lbl_eu_8066AAA8", ".sdata2", 1840, 4),
            ("lbl_eu_8066AAAC", ".sdata2", 1844, 4),
            ("lbl_eu_8066AAB0", ".sdata2", 1848, 4),
            ("lbl_eu_8066AAB4", ".sdata2", 1852, 4),
            ("lbl_eu_8066AAB8", ".sdata2", 1856, 4),
            ("lbl_eu_8066AABC", ".sdata2", 1860, 4),
            ("lbl_eu_8066AAC0", ".sdata2", 1864, 4),
            ("lbl_eu_8066AAC4", ".sdata2", 1868, 4),
            ("lbl_eu_8066AAC8", ".sdata2", 1872, 8),
            ("lbl_eu_8066AAD0", ".sdata2", 1880, 8),
            ("lbl_eu_8066AAD8", ".sdata2", 1888, 4),
            ("lbl_eu_8066AADC", ".sdata2", 1892, 4),
            ("lbl_eu_8066AAE0", ".sdata2", 1896, 4),
            ("lbl_eu_8066AAE4", ".sdata2", 1900, 4),
            ("lbl_eu_8066AAE8", ".sdata2", 1904, 4),
            ("lbl_eu_8066AAEC", ".sdata2", 1908, 4),
            ("lbl_eu_8066AAF0", ".sdata2", 1912, 4),
            ("lbl_eu_8066AAF4", ".sdata2", 1916, 4),
            ("lbl_eu_8066AAF8", ".sdata2", 1920, 8),
            ("lbl_eu_8066AB00", ".sdata2", 1928, 4),
            ("lbl_eu_8066AB04", ".sdata2", 1932, 4),
            ("lbl_eu_8066AB08", ".sdata2", 1936, 4),
            ("lbl_eu_8066AB0C", ".sdata2", 1940, 4),
            ("lbl_eu_8066AB10", ".sdata2", 1944, 8),
            ("lbl_eu_8066AB18", ".sdata2", 1952, 8),
            ("lbl_eu_8066AB20", ".sdata2", 1960, 8),
            ("lbl_eu_8066AB28", ".sdata2", 1968, 4),
            ("lbl_eu_8066AB2C", ".sdata2", 1972, 4),
            ("lbl_eu_8066AB30", ".sdata2", 1976, 4),
            ("lbl_eu_8066AB38", ".sdata2", 1984, 8),
            ("lbl_eu_8066AB40", ".sdata2", 1992, 8),
            ("lbl_eu_8066AB48", ".sdata2", 2000, 8),
            ("lbl_eu_8066AB50", ".sdata2", 2008, 4),
            ("lbl_eu_8066AB54", ".sdata2", 2012, 4),
            ("lbl_eu_8066AB58", ".sdata2", 2016, 4),
            ("lbl_eu_8066AB60", ".sdata2", 2024, 8),
            ("lbl_eu_8066AB68", ".sdata2", 2032, 4),
            ("lbl_eu_8066AB6C", ".sdata2", 2036, 4),
            ("lbl_eu_8066AB70", ".sdata2", 2040, 3),
            ("lbl_eu_8066AB74", ".sdata2", 2044, 4),
            ("lbl_eu_8066AB78", ".sdata2", 2048, 4),
            ("lbl_eu_8066AB7C", ".sdata2", 2052, 4),
            ("lbl_eu_8066AB80", ".sdata2", 2056, 4),
            ("lbl_eu_8066AB88", ".sdata2", 2064, 8),
            ("lbl_eu_8066AB90", ".sdata2", 2072, 4),
            ("lbl_eu_8066AB98", ".sdata2", 2080, 8),
            ("lbl_eu_8066ABA0", ".sdata2", 2088, 4),
            ("lbl_eu_8066ABA8", ".sdata2", 2096, 8),
            ("lbl_eu_8066ABB0", ".sdata2", 2104, 4),
            ("lbl_eu_8066ABB4", ".sdata2", 2108, 4),
            ("lbl_eu_8066ABB8", ".sdata2", 2112, 4),
            ("lbl_eu_8066ABBC", ".sdata2", 2116, 4),
            ("lbl_eu_8066ABC0", ".sdata2", 2120, 4),
            ("lbl_eu_8066ABC4", ".sdata2", 2124, 4),
            ("lbl_eu_8066ABC8", ".sdata2", 2128, 4),
            ("lbl_eu_8066ABCC", ".sdata2", 2132, 4),
            ("lbl_eu_8066ABD0", ".sdata2", 2136, 4),
            ("lbl_eu_8066ABD8", ".sdata2", 2144, 8),
            ("lbl_eu_8066ABE0", ".sdata2", 2152, 4),
            ("lbl_eu_8066ABE4", ".sdata2", 2156, 4),
            ("lbl_eu_8066ABE8", ".sdata2", 2160, 8),
            ("lbl_eu_8066ABF0", ".sdata2", 2168, 4),
            ("lbl_eu_8066ABF4", ".sdata2", 2172, 4),
            ("lbl_eu_8066ABF8", ".sdata2", 2176, 4),
            ("lbl_eu_8066ABFC", ".sdata2", 2180, 4),
            ("lbl_eu_8066AC00", ".sdata2", 2184, 4),
            ("lbl_eu_8066AC04", ".sdata2", 2188, 4),
            ("lbl_eu_8066AC08", ".sdata2", 2192, 8),
            ("lbl_eu_8066AC10", ".sdata2", 2200, 4),
            ("lbl_eu_8066AC14", ".sdata2", 2204, 4),
            ("lbl_eu_8066AC18", ".sdata2", 2208, 4),
            ("lbl_eu_8066AC1C", ".sdata2", 2212, 4),
            ("lbl_eu_8066AC20", ".sdata2", 2216, 4),
            ("lbl_eu_8066AC24", ".sdata2", 2220, 4),
            ("lbl_eu_8066AC28", ".sdata2", 2224, 4),
            ("lbl_eu_8066AC30", ".sdata2", 2232, 8),
            ("lbl_eu_8066AC38", ".sdata2", 2240, 8),
            ("lbl_eu_8066AC40", ".sdata2", 2248, 4),
            ("gap_11_8066AC44_sdata2", ".sdata2", 2252, 4),
            ("lbl_eu_8066AC48", ".sdata2", 2256, 8),
            ("lbl_eu_8066AC50", ".sdata2", 2264, 8),
            ("lbl_eu_8066AC58", ".sdata2", 2272, 8),
            ("lbl_eu_8066AC60", ".sdata2", 2280, 8),
            ("lbl_eu_8066AC68", ".sdata2", 2288, 8),
            ("lbl_eu_8066AC70", ".sdata2", 2296, 8),
            ("lbl_eu_8066AC78", ".sdata2", 2304, 3),
            ("gap_11_8066AC7B_sdata2", ".sdata2", 2307, 1),
            ("lbl_eu_8066AC7C", ".sdata2", 2308, 3),
            ("gap_11_8066AC7F_sdata2", ".sdata2", 2311, 1),
            ("lbl_eu_8066AC80", ".sdata2", 2312, 7),
            ("gap_11_8066AC87_sdata2", ".sdata2", 2319, 1),
            ("lbl_eu_8066AC88", ".sdata2", 2320, 7),
            ("gap_11_8066AC8F_sdata2", ".sdata2", 2327, 1),
            ("lbl_eu_8066AC90", ".sdata2", 2328, 5),
            ("gap_11_8066AC95_sdata2", ".sdata2", 2333, 3),
            ("lbl_eu_8066AC98", ".sdata2", 2336, 5),
            ("gap_11_8066AC9D_sdata2", ".sdata2", 2341, 3),
            ("lbl_eu_8066ACA0", ".sdata2", 2344, 5),
            ("gap_11_8066ACA5_sdata2", ".sdata2", 2349, 3),
            ("lbl_eu_8066ACA8", ".sdata2", 2352, 5),
            ("gap_11_8066ACAD_sdata2", ".sdata2", 2357, 3),
            ("lbl_eu_8066ACB0", ".sdata2", 2360, 5),
            ("gap_11_8066ACB5_sdata2", ".sdata2", 2365, 3),
            ("lbl_eu_8066ACB8", ".sdata2", 2368, 5),
            ("gap_11_8066ACBD_sdata2", ".sdata2", 2373, 3),
            ("lbl_eu_8066ACC0", ".sdata2", 2376, 5),
            ("gap_11_8066ACC5_sdata2", ".sdata2", 2381, 3),
            ("lbl_eu_8066ACC8", ".sdata2", 2384, 5),
            ("gap_11_8066ACCD_sdata2", ".sdata2", 2389, 3),
            ("lbl_eu_8066ACD0", ".sdata2", 2392, 6),
            ("gap_11_8066ACD6_sdata2", ".sdata2", 2398, 2),
            ("lbl_eu_8066ACD8", ".sdata2", 2400, 6),
            ("gap_11_8066ACDE_sdata2", ".sdata2", 2406, 2),
            ("lbl_eu_8066ACE0", ".sdata2", 2408, 7),
            ("gap_11_8066ACE7_sdata2", ".sdata2", 2415, 1),
            ("lbl_eu_8066ACE8", ".sdata2", 2416, 8),
            ("lbl_eu_8066ACF0", ".sdata2", 2424, 8),
            ("lbl_eu_8066ACF8", ".sdata2", 2432, 6),
            ("gap_11_8066ACFE_sdata2", ".sdata2", 2438, 2),
            ("lbl_eu_8066AD00", ".sdata2", 2440, 4),
            ("lbl_eu_8066AD04", ".sdata2", 2444, 6),
            ("gap_11_8066AD0A_sdata2", ".sdata2", 2450, 2),
            ("lbl_eu_8066AD0C", ".sdata2", 2452, 4),
            ("lbl_eu_8066AD10", ".sdata2", 2456, 4),
            ("lbl_eu_8066AD14", ".sdata2", 2460, 4),
            ("lbl_eu_8066AD18", ".sdata2", 2464, 4),
            ("lbl_eu_8066AD1C", ".sdata2", 2468, 4),
            ("lbl_eu_8066AD20", ".sdata2", 2472, 3),
            ("gap_11_8066AD23_sdata2", ".sdata2", 2475, 1),
            ("lbl_eu_8066AD24", ".sdata2", 2476, 4),
            ("lbl_eu_8066AD28", ".sdata2", 2480, 8),
            ("lbl_eu_8066AD30", ".sdata2", 2488, 8),
            ("lbl_eu_8066AD38", ".sdata2", 2496, 3),
            ("gap_11_8066AD3B_sdata2", ".sdata2", 2499, 1),
            ("lbl_eu_8066AD3C", ".sdata2", 2500, 3),
            ("gap_11_8066AD3F_sdata2", ".sdata2", 2503, 1),
            ("lbl_eu_8066AD40", ".sdata2", 2504, 3),
            ("gap_11_8066AD43_sdata2", ".sdata2", 2507, 1),
            ("lbl_eu_8066AD44", ".sdata2", 2508, 3),
            ("gap_11_8066AD47_sdata2", ".sdata2", 2511, 1),
            ("lbl_eu_8066AD48", ".sdata2", 2512, 3),
            ("gap_11_8066AD4B_sdata2", ".sdata2", 2515, 1),
            ("lbl_eu_8066AD4C", ".sdata2", 2516, 3),
            ("gap_11_8066AD4F_sdata2", ".sdata2", 2519, 1),
            ("lbl_eu_8066AD50", ".sdata2", 2520, 5),
            ("gap_11_8066AD55_sdata2", ".sdata2", 2525, 3),
            ("lbl_eu_8066AD58", ".sdata2", 2528, 6),
            ("gap_11_8066AD5E_sdata2", ".sdata2", 2534, 2),
            ("lbl_eu_8066AD60", ".sdata2", 2536, 4),
            ("lbl_eu_8066AD64", ".sdata2", 2540, 4),
            ("lbl_eu_8066AD68", ".sdata2", 2544, 5),
            ("gap_11_8066AD6D_sdata2", ".sdata2", 2549, 3),
            ("lbl_eu_8066AD70", ".sdata2", 2552, 7),
            ("gap_11_8066AD77_sdata2", ".sdata2", 2559, 1),
            ("lbl_eu_8066AD78", ".sdata2", 2560, 4),
            ("lbl_eu_8066AD7C", ".sdata2", 2564, 5),
            ("gap_11_8066AD81_sdata2", ".sdata2", 2569, 3),
            ("lbl_eu_8066AD84", ".sdata2", 2572, 7),
            ("gap_11_8066AD8B_sdata2", ".sdata2", 2579, 1),
            ("lbl_eu_8066AD8C", ".sdata2", 2580, 7),
            ("gap_11_8066AD93_sdata2", ".sdata2", 2587, 1),
            ("lbl_eu_8066AD94", ".sdata2", 2588, 7),
            ("gap_11_8066AD9B_sdata2", ".sdata2", 2595, 1),
            ("lbl_eu_8066AD9C", ".sdata2", 2596, 7),
            ("gap_11_8066ADA3_sdata2", ".sdata2", 2603, 1),
            ("lbl_eu_8066ADA4", ".sdata2", 2604, 5),
            ("gap_11_8066ADA9_sdata2", ".sdata2", 2609, 3),
            ("lbl_eu_8066ADAC", ".sdata2", 2612, 7),
            ("gap_11_8066ADB3_sdata2", ".sdata2", 2619, 1),
            ("lbl_eu_8066ADB4", ".sdata2", 2620, 7),
            ("gap_11_8066ADBB_sdata2", ".sdata2", 2627, 1),
            ("lbl_eu_8066ADBC", ".sdata2", 2628, 7),
            ("gap_11_8066ADC3_sdata2", ".sdata2", 2635, 1),
            ("lbl_eu_8066ADC4", ".sdata2", 2636, 7),
            ("gap_11_8066ADCB_sdata2", ".sdata2", 2643, 1),
            ("lbl_eu_8066ADCC", ".sdata2", 2644, 4),
            ("lbl_eu_8066ADD0", ".sdata2", 2648, 4),
            ("lbl_eu_8066ADD4", ".sdata2", 2652, 5),
            ("gap_11_8066ADD9_sdata2", ".sdata2", 2657, 3),
            ("lbl_eu_8066ADDC", ".sdata2", 2660, 3),
            ("gap_11_8066ADDF_sdata2", ".sdata2", 2663, 1),
            ("lbl_eu_8066ADE0", ".sdata2", 2664, 4),
            ("lbl_eu_8066ADE4", ".sdata2", 2668, 5),
            ("gap_11_8066ADE9_sdata2", ".sdata2", 2673, 3),
            ("lbl_eu_8066ADEC", ".sdata2", 2676, 6),
            ("gap_11_8066ADF2_sdata2", ".sdata2", 2682, 2),
            ("lbl_eu_8066ADF4", ".sdata2", 2684, 4),
            ("lbl_eu_8066ADF8", ".sdata2", 2688, 6),
            ("gap_11_8066ADFE_sdata2", ".sdata2", 2694, 2),
            ("lbl_eu_8066AE00", ".sdata2", 2696, 7),
            ("gap_11_8066AE07_sdata2", ".sdata2", 2703, 1),
            ("lbl_eu_8066AE08", ".sdata2", 2704, 6),
            ("gap_11_8066AE0E_sdata2", ".sdata2", 2710, 2),
            ("lbl_eu_8066AE10", ".sdata2", 2712, 7),
            ("gap_11_8066AE17_sdata2", ".sdata2", 2719, 1),
            ("lbl_eu_8066AE18", ".sdata2", 2720, 3),
            ("gap_11_8066AE1B_sdata2", ".sdata2", 2723, 1),
            ("lbl_eu_8066AE1C", ".sdata2", 2724, 4),
            ("lbl_eu_8066AE20", ".sdata2", 2728, 4),
            ("lbl_eu_8066AE24", ".sdata2", 2732, 4),
            ("lbl_eu_8066AE28", ".sdata2", 2736, 8),
            ("lbl_eu_8066AE30", ".sdata2", 2744, 5),
            ("lbl_eu_8066AE38", ".sdata2", 2752, 4),
            ("lbl_eu_8066AE3C", ".sdata2", 2756, 4),
            ("lbl_eu_8066AE40", ".sdata2", 2760, 4),
            ("lbl_eu_8066AE44", ".sdata2", 2764, 4),
            ("lbl_eu_8066AE48", ".sdata2", 2768, 8),
            ("lbl_eu_8066AE50", ".sdata2", 2776, 4),
            ("lbl_eu_8066AE54", ".sdata2", 2780, 4),
            ("lbl_eu_8066AE58", ".sdata2", 2784, 4),
            ("lbl_eu_8066AE5C", ".sdata2", 2788, 4),
            ("lbl_eu_8066AE60", ".sdata2", 2792, 4),
            ("lbl_eu_8066AE64", ".sdata2", 2796, 4),
            ("lbl_eu_8066AE68", ".sdata2", 2800, 4),
            ("lbl_eu_8066AE6C", ".sdata2", 2804, 4),
            ("lbl_eu_8066AE70", ".sdata2", 2808, 4),
            ("lbl_eu_8066AE74", ".sdata2", 2812, 4),
            ("lbl_eu_8066AE78", ".sdata2", 2816, 8),
            ("lbl_eu_8066AE80", ".sdata2", 2824, 8),
            ("lbl_eu_8066AE88", ".sdata2", 2832, 4),
            ("lbl_eu_8066AE8C", ".sdata2", 2836, 4),
            ("lbl_eu_8066AE90", ".sdata2", 2840, 4),
            ("lbl_eu_8066AE94", ".sdata2", 2844, 4),
            ("lbl_eu_8066AE98", ".sdata2", 2848, 4),
            ("lbl_eu_8066AE9C", ".sdata2", 2852, 4),
            ("lbl_eu_8066AEA0", ".sdata2", 2856, 8),
            ("lbl_eu_8066AEA8", ".sdata2", 2864, 8),
            ("lbl_eu_8066AEB0", ".sdata2", 2872, 4),
            ("lbl_eu_8066AEB4", ".sdata2", 2876, 4),
            ("lbl_eu_8066AEB8", ".sdata2", 2880, 8),
            ("lbl_eu_8066AEC0", ".sdata2", 2888, 4),
            ("lbl_eu_8066AEC4", ".sdata2", 2892, 4),
            ("lbl_eu_8066AEC8", ".sdata2", 2896, 8),
            ("lbl_eu_8066AED0", ".sdata2", 2904, 4),
            ("lbl_eu_8066AED4", ".sdata2", 2908, 4),
            ("lbl_eu_8066AED8", ".sdata2", 2912, 4),
            ("lbl_eu_8066AEDC", ".sdata2", 2916, 4),
            ("lbl_eu_8066AEE0", ".sdata2", 2920, 4),
            ("lbl_eu_8066AEE4", ".sdata2", 2924, 4),
            ("lbl_eu_8066AEE8", ".sdata2", 2928, 4),
            ("lbl_eu_8066AEEC", ".sdata2", 2932, 4),
            ("lbl_eu_8066AEF0", ".sdata2", 2936, 4),
            ("lbl_eu_8066AEF4", ".sdata2", 2940, 4),
            ("lbl_eu_8066AEF8", ".sdata2", 2944, 4),
            ("lbl_eu_8066AEFC", ".sdata2", 2948, 4),
            ("lbl_eu_8066AF00", ".sdata2", 2952, 4),
            ("lbl_eu_8066AF04", ".sdata2", 2956, 4),
            ("lbl_eu_8066AF08", ".sdata2", 2960, 8),
            ("lbl_eu_8066AF10", ".sdata2", 2968, 4),
            ("lbl_eu_8066AF14", ".sdata2", 2972, 4),
            ("lbl_eu_8066AF18", ".sdata2", 2976, 8),
            ("lbl_eu_8066AF20", ".sdata2", 2984, 4),
            ("lbl_eu_8066AF24", ".sdata2", 2988, 4),
            ("lbl_eu_8066AF28", ".sdata2", 2992, 4),
            ("lbl_eu_8066AF2C", ".sdata2", 2996, 4),
            ("lbl_eu_8066AF30", ".sdata2", 3000, 4),
            ("lbl_eu_8066AF34", ".sdata2", 3004, 4),
            ("lbl_eu_8066AF38", ".sdata2", 3008, 4),
            ("lbl_eu_8066AF3C", ".sdata2", 3012, 4),
            ("lbl_eu_8066AF40", ".sdata2", 3016, 4),
            ("lbl_eu_8066AF44", ".sdata2", 3020, 4),
            ("lbl_eu_8066AF48", ".sdata2", 3024, 4),
            ("lbl_eu_8066AF4C", ".sdata2", 3028, 4),
            ("lbl_eu_8066AF50", ".sdata2", 3032, 4),
            ("lbl_eu_8066AF58", ".sdata2", 3040, 8),
            ("lbl_eu_8066AF60", ".sdata2", 3048, 4),
            ("lbl_eu_8066AF64", ".sdata2", 3052, 4),
            ("lbl_eu_8066AF68", ".sdata2", 3056, 4),
            ("lbl_eu_8066AF6C", ".sdata2", 3060, 4),
            ("lbl_eu_8066AF70", ".sdata2", 3064, 4),
            ("lbl_eu_8066AF74", ".sdata2", 3068, 4),
            ("lbl_eu_8066AF78", ".sdata2", 3072, 4),
            ("lbl_eu_8066AF7C", ".sdata2", 3076, 4),
            ("lbl_eu_8066AF80", ".sdata2", 3080, 4),
            ("lbl_eu_8066AF84", ".sdata2", 3084, 4),
            ("lbl_eu_8066AF88", ".sdata2", 3088, 4),
            ("lbl_eu_8066AF8C", ".sdata2", 3092, 4),
            ("lbl_eu_8066AF90", ".sdata2", 3096, 4),
            ("lbl_eu_8066AF94", ".sdata2", 3100, 4),
            ("lbl_eu_8066AF98", ".sdata2", 3104, 4),
            ("lbl_eu_8066AF9C", ".sdata2", 3108, 4),
            ("lbl_eu_8066AFA0", ".sdata2", 3112, 8),
            ("lbl_eu_8066AFA8", ".sdata2", 3120, 4),
            ("lbl_eu_8066AFAC", ".sdata2", 3124, 4),
            ("lbl_eu_8066AFB0", ".sdata2", 3128, 4),
            ("lbl_eu_8066AFB4", ".sdata2", 3132, 4),
            ("lbl_eu_8066AFB8", ".sdata2", 3136, 4),
            ("lbl_eu_8066AFBC", ".sdata2", 3140, 4),
            ("lbl_eu_8066AFC0", ".sdata2", 3144, 4),
            ("lbl_eu_8066AFC4", ".sdata2", 3148, 7),
            ("gap_11_8066AFCC_sdata2", ".sdata2", 3156, 4),
            ("lbl_eu_8066AFD0", ".sdata2", 3160, 4),
            ("lbl_eu_8066AFD4", ".sdata2", 3164, 4),
            ("lbl_eu_8066AFD8", ".sdata2", 3168, 4),
            ("lbl_eu_8066AFDC", ".sdata2", 3172, 4),
            ("lbl_eu_8066AFE0", ".sdata2", 3176, 4),
            ("lbl_eu_8066AFE4", ".sdata2", 3180, 4),
            ("lbl_eu_8066AFE8", ".sdata2", 3184, 4),
            ("lbl_eu_8066AFEC", ".sdata2", 3188, 4),
            ("lbl_eu_8066AFF0", ".sdata2", 3192, 4),
            ("lbl_eu_8066AFF8", ".sdata2", 3200, 8),
            ("lbl_eu_8066B000", ".sdata2", 3208, 4),
            ("lbl_eu_8066B004", ".sdata2", 3212, 4),
            ("lbl_eu_8066B008", ".sdata2", 3216, 8),
            ("lbl_eu_8066B010", ".sdata2", 3224, 4),
            ("lbl_eu_8066B014", ".sdata2", 3228, 4),
            ("lbl_eu_8066B018", ".sdata2", 3232, 8),
            ("lbl_eu_8066B020", ".sdata2", 3240, 4),
            ("lbl_eu_8066B024", ".sdata2", 3244, 4),
            ("lbl_eu_8066B028", ".sdata2", 3248, 4),
            ("lbl_eu_8066B02C", ".sdata2", 3252, 4),
            ("lbl_eu_8066B030", ".sdata2", 3256, 4),
            ("lbl_eu_8066B034", ".sdata2", 3260, 4),
            ("lbl_eu_8066B038", ".sdata2", 3264, 4),
            ("lbl_eu_8066B03C", ".sdata2", 3268, 4),
            ("lbl_eu_8066B040", ".sdata2", 3272, 8),
            ("lbl_eu_8066B048", ".sdata2", 3280, 4),
            ("lbl_eu_8066B04C", ".sdata2", 3284, 4),
            ("lbl_eu_8066B050", ".sdata2", 3288, 8),
            ("lbl_eu_8066B058", ".sdata2", 3296, 4),
            ("lbl_eu_8066B05C", ".sdata2", 3300, 4),
            ("lbl_eu_8066B060", ".sdata2", 3304, 4),
            ("lbl_eu_8066B068", ".sdata2", 3312, 8),
            ("lbl_eu_8066B070", ".sdata2", 3320, 4),
            ("lbl_eu_8066B074", ".sdata2", 3324, 4),
            ("lbl_eu_8066B078", ".sdata2", 3328, 4),
            ("lbl_eu_8066B07C", ".sdata2", 3332, 4),
            ("lbl_eu_8066B080", ".sdata2", 3336, 8),
            ("lbl_eu_8066B088", ".sdata2", 3344, 4),
            ("lbl_eu_8066B08C", ".sdata2", 3348, 4),
            ("lbl_eu_8066B090", ".sdata2", 3352, 4),
            ("lbl_eu_8066B094", ".sdata2", 3356, 4),
            ("lbl_eu_8066B098", ".sdata2", 3360, 8),
            ("lbl_eu_8066B0A0", ".sdata2", 3368, 4),
            ("lbl_eu_8066B0A4", ".sdata2", 3372, 4),
            ("lbl_eu_8066B0A8", ".sdata2", 3376, 4),
            ("lbl_eu_8066B0B0", ".sdata2", 3384, 8),
            ("lbl_eu_8066B0B8", ".sdata2", 3392, 8),
            ("lbl_eu_8066B0C0", ".sdata2", 3400, 4),
            ("lbl_eu_8066B0C4", ".sdata2", 3404, 4),
            ("lbl_eu_8066B0C8", ".sdata2", 3408, 8),
            ("lbl_eu_8066B0D0", ".sdata2", 3416, 8),
            ("lbl_eu_8066B0D8", ".sdata2", 3424, 4),
            ("lbl_eu_8066B0DC", ".sdata2", 3428, 4),
            ("lbl_eu_8066B0E0", ".sdata2", 3432, 4),
            ("lbl_eu_8066B0E4", ".sdata2", 3436, 4),
            ("lbl_eu_8066B0E8", ".sdata2", 3440, 8),
            ("lbl_eu_8066B0F0", ".sdata2", 3448, 8),
            ("lbl_eu_8066B0F8", ".sdata2", 3456, 4),
            ("lbl_eu_8066B0FC", ".sdata2", 3460, 4),
            ("lbl_eu_8066B100", ".sdata2", 3464, 4),
            ("lbl_eu_8066B104", ".sdata2", 3468, 4),
            ("lbl_eu_8066B108", ".sdata2", 3472, 4),
            ("lbl_eu_8066B10C", ".sdata2", 3476, 4),
            ("lbl_eu_8066B110", ".sdata2", 3480, 4),
            ("lbl_eu_8066B114", ".sdata2", 3484, 4),
            ("lbl_eu_8066B118", ".sdata2", 3488, 4),
            ("lbl_eu_8066B11C", ".sdata2", 3492, 4),
            ("lbl_eu_8066B120", ".sdata2", 3496, 4),
            ("lbl_eu_8066B124", ".sdata2", 3500, 4),
            ("lbl_eu_8066B128", ".sdata2", 3504, 4),
            ("lbl_eu_8066B12C", ".sdata2", 3508, 4),
            ("lbl_eu_8066B130", ".sdata2", 3512, 4),
            ("lbl_eu_8066B134", ".sdata2", 3516, 4),
            ("lbl_eu_8066B138", ".sdata2", 3520, 8),
            ("lbl_eu_8066B140", ".sdata2", 3528, 4),
            ("lbl_eu_8066B144", ".sdata2", 3532, 4),
            ("lbl_eu_8066B148", ".sdata2", 3536, 4),
            ("lbl_eu_8066B14C", ".sdata2", 3540, 4),
            ("lbl_eu_8066B150", ".sdata2", 3544, 8),
            ("lbl_eu_8066B158", ".sdata2", 3552, 4),
            ("lbl_eu_8066B15C", ".sdata2", 3556, 4),
            ("lbl_eu_8066B160", ".sdata2", 3560, 4),
            ("lbl_eu_8066B164", ".sdata2", 3564, 4),
            ("lbl_eu_8066B168", ".sdata2", 3568, 4),
            ("lbl_eu_8066B170", ".sdata2", 3576, 8),
            ("lbl_eu_8066B178", ".sdata2", 3584, 4),
            ("lbl_eu_8066B180", ".sdata2", 3592, 8),
            ("lbl_eu_8066B188", ".sdata2", 3600, 8),
            ("lbl_eu_8066B190", ".sdata2", 3608, 8),
            ("lbl_eu_8066B198", ".sdata2", 3616, 4),
            ("lbl_eu_8066B19C", ".sdata2", 3620, 4),
            ("lbl_eu_8066B1A0", ".sdata2", 3624, 4),
            ("lbl_eu_8066B1A4", ".sdata2", 3628, 4),
            ("lbl_eu_8066B1A8", ".sdata2", 3632, 4),
            ("lbl_eu_8066B1AC", ".sdata2", 3636, 4),
            ("lbl_eu_8066B1B0", ".sdata2", 3640, 8),
            ("lbl_eu_8066B1B8", ".sdata2", 3648, 4),
            ("lbl_eu_8066B1BC", ".sdata2", 3652, 4),
            ("lbl_eu_8066B1C0", ".sdata2", 3656, 4),
            ("lbl_eu_8066B1C4", ".sdata2", 3660, 4),
            ("lbl_eu_8066B1C8", ".sdata2", 3664, 8),
            ("lbl_eu_8066B1D0", ".sdata2", 3672, 8),
            ("lbl_eu_8066B1D8", ".sdata2", 3680, 4),
            ("lbl_eu_8066B1E0", ".sdata2", 3688, 8),
            ("lbl_eu_8066B1E8", ".sdata2", 3696, 4),
            ("lbl_eu_8066B1EC", ".sdata2", 3700, 4),
            ("lbl_eu_8066B1F0", ".sdata2", 3704, 4),
            ("lbl_eu_8066B1F4", ".sdata2", 3708, 4),
            ("lbl_eu_8066B1F8", ".sdata2", 3712, 4),
            ("lbl_eu_8066B1FC", ".sdata2", 3716, 4),
            ("lbl_eu_8066B200", ".sdata2", 3720, 8),
            ("lbl_eu_8066B208", ".sdata2", 3728, 4),
            ("lbl_eu_8066B20C", ".sdata2", 3732, 4),
            ("lbl_eu_8066B210", ".sdata2", 3736, 8),
            ("lbl_eu_8066B218", ".sdata2", 3744, 6),
            ("gap_11_8066B21E_sdata2", ".sdata2", 3750, 2),
            ("lbl_eu_8066B220", ".sdata2", 3752, 8),
            ("lbl_eu_8066B228", ".sdata2", 3760, 6),
            ("lbl_eu_8066B230", ".sdata2", 3768, 4),
            ("lbl_eu_8066B234", ".sdata2", 3772, 4),
            ("lbl_eu_8066B238", ".sdata2", 3776, 4),
            ("lbl_eu_8066B240", ".sdata2", 3784, 8),
            ("lbl_eu_8066B248", ".sdata2", 3792, 4),
            ("lbl_eu_8066B24C", ".sdata2", 3796, 4),
            ("lbl_eu_8066B250", ".sdata2", 3800, 4),
            ("lbl_eu_8066B258", ".sdata2", 3808, 8),
            ("lbl_eu_8066B260", ".sdata2", 3816, 4),
            ("lbl_eu_8066B268", ".sdata2", 3824, 8),
            ("lbl_eu_8066B270", ".sdata2", 3832, 4),
            ("lbl_eu_8066B274", ".sdata2", 3836, 4),
            ("lbl_eu_8066B278", ".sdata2", 3840, 4),
            ("lbl_eu_8066B280", ".sdata2", 3848, 8),
            ("lbl_eu_8066B288", ".sdata2", 3856, 8),
            ("lbl_eu_8066B290", ".sdata2", 3864, 4),
            ("lbl_eu_8066B298", ".sdata2", 3872, 8),
            ("lbl_eu_8066B2A0", ".sdata2", 3880, 8),
            ("lbl_eu_8066B2A8", ".sdata2", 3888, 4),
            ("lbl_eu_8066B2AC", ".sdata2", 3892, 4),
            ("lbl_eu_8066B2B0", ".sdata2", 3896, 4),
            ("lbl_eu_8066B2B4", ".sdata2", 3900, 4),
            ("lbl_eu_8066B2B8", ".sdata2", 3904, 4),
            ("lbl_eu_8066B2BC", ".sdata2", 3908, 4),
            ("lbl_eu_8066B2C0", ".sdata2", 3912, 4),
            ("lbl_eu_8066B2C4", ".sdata2", 3916, 4),
            ("lbl_eu_8066B2C8", ".sdata2", 3920, 4),
            ("lbl_eu_8066B2CC", ".sdata2", 3924, 4),
            ("lbl_eu_8066B2D0", ".sdata2", 3928, 4),
            ("lbl_eu_8066B2D4", ".sdata2", 3932, 4),
            ("lbl_eu_8066B2D8", ".sdata2", 3936, 4),
            ("lbl_eu_8066B2DC", ".sdata2", 3940, 4),
            ("lbl_eu_8066B2E0", ".sdata2", 3944, 4),
            ("lbl_eu_8066B2E4", ".sdata2", 3948, 4),
            ("lbl_eu_8066B2E8", ".sdata2", 3952, 8),
            ("lbl_eu_8066B2F0", ".sdata2", 3960, 8),
            ("lbl_eu_8066B2F8", ".sdata2", 3968, 5),
            ("gap_11_8066B2FD_sdata2", ".sdata2", 3973, 3),
            ("lbl_eu_8066B300", ".sdata2", 3976, 4),
            ("lbl_eu_8066B304", ".sdata2", 3980, 4),
            ("lbl_eu_8066B308", ".sdata2", 3984, 6),
            ("lbl_eu_8066B310", ".sdata2", 3992, 4),
            ("lbl_eu_8066B314", ".sdata2", 3996, 4),
            ("lbl_eu_8066B318", ".sdata2", 4000, 4),
            ("lbl_eu_8066B31C", ".sdata2", 4004, 4),
            ("lbl_eu_8066B320", ".sdata2", 4008, 4),
            ("lbl_eu_8066B328", ".sdata2", 4016, 8),
            ("lbl_eu_8066B330", ".sdata2", 4024, 4),
            ("lbl_eu_8066B334", ".sdata2", 4028, 4),
            ("lbl_eu_8066B338", ".sdata2", 4032, 8),
            ("lbl_eu_8066B340", ".sdata2", 4040, 7),
            ("gap_11_8066B347_sdata2", ".sdata2", 4047, 1),
            ("lbl_eu_8066B348", ".sdata2", 4048, 7),
            ("lbl_eu_8066B350", ".sdata2", 4056, 4),
            ("lbl_eu_8066B354", ".sdata2", 4060, 4),
            ("lbl_eu_8066B358", ".sdata2", 4064, 6),
            ("lbl_eu_8066B360", ".sdata2", 4072, 4),
            ("lbl_eu_8066B364", ".sdata2", 4076, 4),
            ("lbl_eu_8066B368", ".sdata2", 4080, 4),
            ("lbl_eu_8066B36C", ".sdata2", 4084, 4),
            ("lbl_eu_8066B370", ".sdata2", 4088, 4),
            ("lbl_eu_8066B374", ".sdata2", 4092, 4),
            ("lbl_eu_8066B378", ".sdata2", 4096, 4),
            ("lbl_eu_8066B37C", ".sdata2", 4100, 4),
            ("lbl_eu_8066B380", ".sdata2", 4104, 4),
            ("lbl_eu_8066B384", ".sdata2", 4108, 4),
            ("lbl_eu_8066B388", ".sdata2", 4112, 4),
            ("lbl_eu_8066B38C", ".sdata2", 4116, 4),
            ("lbl_eu_8066B390", ".sdata2", 4120, 4),
            ("lbl_eu_8066B394", ".sdata2", 4124, 4),
            ("lbl_eu_8066B398", ".sdata2", 4128, 4),
            ("lbl_eu_8066B39C", ".sdata2", 4132, 4),
            ("lbl_eu_8066B3A0", ".sdata2", 4136, 4),
            ("lbl_eu_8066B3A4", ".sdata2", 4140, 4),
            ("lbl_eu_8066B3A8", ".sdata2", 4144, 4),
            ("lbl_eu_8066B3AC", ".sdata2", 4148, 4),
            ("lbl_eu_8066B3B0", ".sdata2", 4152, 4),
            ("lbl_eu_8066B3B4", ".sdata2", 4156, 4),
            ("lbl_eu_8066B3B8", ".sdata2", 4160, 6),
            ("gap_11_8066B3BE_sdata2", ".sdata2", 4166, 2),
            ("lbl_eu_8066B3C0", ".sdata2", 4168, 6),
            ("gap_11_8066B3C6_sdata2", ".sdata2", 4174, 2),
            ("lbl_eu_8066B3C8", ".sdata2", 4176, 6),
            ("lbl_eu_8066B3D0", ".sdata2", 4184, 4),
            ("lbl_eu_8066B3D4", ".sdata2", 4188, 4),
            ("lbl_eu_8066B3D8", ".sdata2", 4192, 4),
            ("lbl_eu_8066B3DC", ".sdata2", 4196, 4),
            ("lbl_eu_8066B3E0", ".sdata2", 4200, 4),
            ("lbl_eu_8066B3E4", ".sdata2", 4204, 4),
            ("lbl_eu_8066B3E8", ".sdata2", 4208, 4),
            ("lbl_eu_8066B3EC", ".sdata2", 4212, 4),
            ("lbl_eu_8066B3F0", ".sdata2", 4216, 4),
            ("lbl_eu_8066B3F4", ".sdata2", 4220, 4),
            ("lbl_eu_8066B3F8", ".sdata2", 4224, 4),
            ("lbl_eu_8066B3FC", ".sdata2", 4228, 4),
            ("lbl_eu_8066B400", ".sdata2", 4232, 4),
            ("lbl_eu_8066B404", ".sdata2", 4236, 4),
            ("lbl_eu_8066B408", ".sdata2", 4240, 4),
            ("lbl_eu_8066B40C", ".sdata2", 4244, 4),
            ("lbl_eu_8066B410", ".sdata2", 4248, 4),
            ("lbl_eu_8066B414", ".sdata2", 4252, 4),
            ("lbl_eu_8066B418", ".sdata2", 4256, 4),
            ("lbl_eu_8066B41C", ".sdata2", 4260, 4),
            ("lbl_eu_8066B420", ".sdata2", 4264, 4),
            ("lbl_eu_8066B428", ".sdata2", 4272, 8),
            ("lbl_eu_8066B430", ".sdata2", 4280, 8),
            ("lbl_eu_8066B438", ".sdata2", 4288, 8),
            ("lbl_eu_8066B440", ".sdata2", 4296, 4),
            ("lbl_eu_8066B444", ".sdata2", 4300, 4),
            ("lbl_eu_8066B448", ".sdata2", 4304, 4),
            ("lbl_eu_8066B44C", ".sdata2", 4308, 4),
            ("lbl_eu_8066B450", ".sdata2", 4312, 4),
            ("lbl_eu_8066B454", ".sdata2", 4316, 4),
            ("lbl_eu_8066B458", ".sdata2", 4320, 4),
            ("lbl_eu_8066B45C", ".sdata2", 4324, 4),
            ("lbl_eu_8066B460", ".sdata2", 4328, 4),
            ("lbl_eu_8066B464", ".sdata2", 4332, 4),
            ("lbl_eu_8066B468", ".sdata2", 4336, 8),
            ("lbl_eu_8066B470", ".sdata2", 4344, 8),
            ("lbl_eu_8066B478", ".sdata2", 4352, 4),
            ("lbl_eu_8066B47C", ".sdata2", 4356, 4),
            ("lbl_eu_8066B480", ".sdata2", 4360, 4),
            ("lbl_eu_8066B484", ".sdata2", 4364, 4),
            ("lbl_eu_8066B488", ".sdata2", 4368, 4),
            ("lbl_eu_8066B48C", ".sdata2", 4372, 4),
            ("lbl_eu_8066B490", ".sdata2", 4376, 4),
            ("lbl_eu_8066B494", ".sdata2", 4380, 4),
            ("lbl_eu_8066B498", ".sdata2", 4384, 8),
            ("lbl_eu_8066B4A0", ".sdata2", 4392, 8),
            ("lbl_eu_8066B4A8", ".sdata2", 4400, 4),
            ("lbl_eu_8066B4AC", ".sdata2", 4404, 4),
            ("lbl_eu_8066B4B0", ".sdata2", 4408, 4),
            ("lbl_eu_8066B4B8", ".sdata2", 4416, 8),
            ("lbl_eu_8066B4C0", ".sdata2", 4424, 4),
            ("lbl_eu_8066B4C4", ".sdata2", 4428, 4),
            ("lbl_eu_8066B4C8", ".sdata2", 4432, 4),
            ("lbl_eu_8066B4CC", ".sdata2", 4436, 4),
            ("lbl_eu_8066B4D0", ".sdata2", 4440, 4),
            ("lbl_eu_8066B4D8", ".sdata2", 4448, 8),
            ("lbl_eu_8066B4E0", ".sdata2", 4456, 4),
            ("lbl_eu_8066B4E4", ".sdata2", 4460, 4),
            ("lbl_eu_8066B4E8", ".sdata2", 4464, 8),
            ("lbl_eu_8066B4F0", ".sdata2", 4472, 4),
            ("lbl_eu_8066B4F4", ".sdata2", 4476, 4),
            ("lbl_eu_8066B4F8", ".sdata2", 4480, 4),
            ("lbl_eu_8066B500", ".sdata2", 4488, 8),
            ("lbl_eu_8066B508", ".sdata2", 4496, 4),
            ("lbl_eu_8066B50C", ".sdata2", 4500, 4),
            ("lbl_eu_8066B510", ".sdata2", 4504, 4),
            ("lbl_eu_8066B514", ".sdata2", 4508, 4),
            ("lbl_eu_8066B518", ".sdata2", 4512, 8),
            ("lbl_eu_8066B520", ".sdata2", 4520, 8),
            ("lbl_eu_8066B528", ".sdata2", 4528, 6),
        ),
    ),
    "code_804F0258.o": UnitRules(
        # pool-coupled: func_804F06C4's convU16ToF/convS32ToF magic doubles pool
        # locally (@ symbols) in .sdata2; retail keeps the section empty and the
        # code reads the monolibdata2 pool aliases instead. Rename the two
        # anonymous entries onto the shared pool symbols, then extern the
        # section so the three lfd relocs resolve at link.
        # func_804F1F18: MWCC unified its internal (f32)u16 2^52 constant with
        # the B498 pool alias; retail's copy of that function reads B4B8.
        # Retarget the two lfd reloc sites onto the retail name (§17.6).
        retarget_relocs=(
            (".text", 0x05F0, "lbl_eu_8066B4B8"),
            (".text", 0x060C, "lbl_eu_8066B4B8"),
        ),
        # func_804F10A0 draft: our C++-mangled trail-emitter calls vs retail
        # plain name; anon pooled constants verified as 6.24 / s->f magic.
        # NOTE: @N names drift on source edits - refresh after each iteration.
        exact_renames=(
            ("@3693", "lbl_eu_8066B494"),
            ("@3694", "lbl_eu_8066B4A0"),
            ("func_804F1B88__FfPQ22ml5CVec3PQ22ml5CVec3PQ22ml5CVec3lPUcP19CMarkerDistProviderlb",
             "func_804F1B88"),
        ),
        pool_patterns=(
            (struct.pack(">II", 0x43300000, 0x00000000), "lbl_eu_8066B498"),
            (struct.pack(">II", 0x43300000, 0x80000000), "lbl_eu_8066B4A0"),
        ),
        extern_data_sections=(".sdata2",),
    ),
    "CPlane.o": UnitRules(
        # pool-coupled: local .sdata2 pool -> CGXCache pool symbols (lbl_eu_8066Axxx).
        pool_patterns=(
            (struct.pack(">I", 0x00000000), "lbl_eu_8066A378"),
        ),
        extern_data_sections=(".sdata2",),
    ),
    "CMat44.o": UnitRules(
        # pool-coupled: local .sdata2 pool -> CGXCache pool symbols (lbl_eu_8066Axxx).
        pool_patterns=(
            (struct.pack(">I", 0x00000000), "lbl_eu_8066A378"),
            (struct.pack(">I", 0x3F800000), "lbl_eu_8066A380"),
        ),
        extern_data_sections=(".sdata2",),
    ),
    "CScn_80496B0C.o": UnitRules(
        # monolibdata2 dissolve, post blob-split: the retail slice now carries
        # REAL relocations (the vtable pointer words live here, not in the
        # blob copy) and owns the chr-anm panic strings. The source spells the
        # three vtables with explicit &reloc words plus the aligned strings;
        # the only residual shapes are:
        #  - the three unspellable CTTask<CScn> slot names (<...> cannot be
        #    declared in C++): literal-zero words at .data+0xD4/0xE4/0xE8 get
        #    injected ADDR32 relocs against their retail symbols (CDevice.o
        #    pattern); and
        #  - the 5-byte linker tail pad after "g3d_resanmchr_ac.h"
        #    (.data 0x12B..0x130), restored by pad_data_section.
        inject_relocs=(
            (".data", 0xD4, "__dt__13CTask<4CScn>Fv"),
            (".data", 0xE4, "Move__13CTask<4CScn>Fv"),
            (".data", 0xE8, "Draw__13CTask<4CScn>Fv"),
        ),
        set_data_align=((".data", 8),),
        pad_data_section=((".data", 0x130),),
        # func_80497544 calls the C++-linkage func_80498288 defined in this
        # TU; retail keeps a plain unmangled linker name for it.
        # C++-linkage in-TU callees retail keeps as plain unmangled linker
        # names (func_80497544/func_80498288/func_80498A5C call sites).
        exact_renames=(
            ("func_80498288__FP17CScnChild80496B0CPlP7VTargetP12CScnItemAnimUlUl",
             "func_80498288"),
            ("func_80498A5C__FP17CScnChild80496B0CPQ22ml5CVec3PQ22ml5CVec3PQ22ml5CVec3Ul",
             "func_80498A5C"),
            # CTTask<CScn> vtable slots: the tail stubs carry the unspellable
            # retail names through the symtab for inject_relocs, then
            # drop_text_symbols_as_undef turns the definitions UNDEF so the
            # slots resolve to CScn.o's implementations at link.
            ("__ct_CTask_CScn_name_carrier_1", "__dt__13CTask<4CScn>Fv"),
            ("__ct_CTask_CScn_name_carrier_2", "Move__13CTask<4CScn>Fv"),
            ("__ct_CTask_CScn_name_carrier_3", "Draw__13CTask<4CScn>Fv"),
        ),
        drop_text_symbols_as_undef=(
            "__dt__13CTask<4CScn>Fv",
            "Move__13CTask<4CScn>Fv",
            "Draw__13CTask<4CScn>Fv",
        ),
        # pool-coupled: MWCC pools the two int->f64 conversion magic doubles
        # locally (signed 43300000_80000000 for the (s32) frame quantize in
        # func_80497AA8, unsigned 43300000_00000000 for the (s16)/(u32)
        # conversions) but retail keeps the TU .sdata2 EMPTY - retail loads
        # the same 8 bytes straight from the CGXCache pool via lfd against
        # lbl_eu_8066A388 / lbl_eu_8066A3C0; rename and strip.
        pool_patterns=(
            (struct.pack(">II", 0x43300000, 0x80000000), "lbl_eu_8066A388"),
            (struct.pack(">II", 0x43300000, 0x00000000), "lbl_eu_8066A3C0"),
        ),
        extern_data_sections=(".sdata2",),
    ),
    "MPFDrawDisplayList.o": UnitRules(
        # monolibdata2 dissolve: MWCC pools the signed int->f64 conversion
        # magic double locally (@5185 = 43300000_80000000, loaded via lfd in
        # func_80476344 @+42C and func_80476E50 @+67C); retail loads the same
        # 8 bytes from the shared CGXCache/monolibdata2 pool against
        # lbl_eu_8066A808 (both functions' lfd sites). The second local entry
        # (@one = 00000001_00000000) has NO relocations anywhere - an orphan
        # MWCC literal - so stripping .sdata2 removes it.
        pool_patterns=(
            (struct.pack(">II", 0x43300000, 0x80000000), "lbl_eu_8066A808"),
        ),
        extern_data_sections=(".sdata2",),
    ),
    "CScnCameraMan.o": UnitRules(
        # monolibdata2 dissolve: lone signed int->f64 conversion magic double
        # (@6287 = 43300000_80000000, the TU's only .sdata2 symbol); retail
        # loads it via lfd against lbl_eu_8066AB60 (the split's single
        # lfd...@sda21 site, 0x8049F7C0/0x8049F854). Rename + strip.
        pool_patterns=(
            (struct.pack(">II", 0x43300000, 0x80000000), "lbl_eu_8066AB60"),
        ),
        extern_data_sections=(".sdata2",),
    ),
    "CQuat.o": UnitRules(
        # pool-coupled: local .sdata2 pool -> CGXCache pool symbols (lbl_eu_8066Axxx).
        pool_patterns=(
            (struct.pack(">I", 0x4222F983), "lbl_eu_8066A474"),
            (struct.pack(">I", 0x00000000), "lbl_eu_8066A378"),
            (struct.pack(">I", 0x3F800000), "lbl_eu_8066A380"),
        ),
        extern_data_sections=(".sdata2",),
    ),
    "code_804645CC.o": UnitRules(
        # pool-coupled: local .sdata2 pool -> CGXCache pool symbols (lbl_eu_8066Axxx).
        # Retail loads the s32->f32 conversion magic via lfd against
        # lbl_eu_8066A628 (func_80465730/6577C sites); A388 belongs to other units.
        pool_patterns=(
            (struct.pack(">II", 0x43300000, 0x80000000), "lbl_eu_8066A628"),
        ),
        extern_data_sections=(".sdata2",),
    ),
    "code_8046A530.o": UnitRules(
        # pool-coupled: lone int->float conversion magic (2^52 + 2^31,
        # 43300000_80000000) MWCC materializes in .sdata2 for the s32->f32
        # casts in the LOD rand() helpers. Retail loads the same 8 bytes
        # straight from the CGXCache pool via lfd against lbl_eu_8066A678;
        # rename to that and strip.
        pool_patterns=(
            (struct.pack(">II", 0x43300000, 0x80000000), "lbl_eu_8066A678"),
        ),
        extern_data_sections=(".sdata2",),
    ),
    "LODMemMan.o": UnitRules(
        # pool-coupled: lone u32->f64 conversion magic (2^52,
        # 43300000_00000000) MWCC materializes in .sdata2 for the mtRand
        # casts in func_8046D264. Retail loads the same 8 bytes straight
        # from the CGXCache pool (lbl_eu_8066A6E0); rename to that and strip.
        pool_patterns=(
            (struct.pack(">II", 0x43300000, 0x00000000), "lbl_eu_8066A6E0"),
        ),
        # The IWorkEvent vtable references symbols MWCC will not let us
        # declare under -ipa (the member/RTTI asm names __dt__Q23LOD...,
        # OnFileEvent__Q23LOD..., __RTTI__10IWorkEvent collide with the
        # compiler-generated objects; lbl_eu_80663848 lives in another TU).
        # The source emits a lodmm_vt_placeholder UND at each slot; retarget
        # those relocs (.data offsets relative to the vtable base at 0x10)
        # onto the retail names here.
        retarget_relocs=(
            (".data", 0x10, "lbl_eu_80663848"),
            (".data", 0x18, "__dt__Q23LOD9LODMemManFv"),
            (".data", 0x20, "OnFileEvent__Q23LOD9LODMemManFP10CEventFile"),
            (".data", 0x98, "__RTTI__10IWorkEvent"),
        ),
        extern_data_sections=(".sdata2",),
    ),
    "CMdlMouth.o": UnitRules(
        # ppcdis splitter writes the retail .rodata slice at align 4 (the
        # slice starts at 0x805247B4, 4-mod-8); MWCC defaults the section to
        # 8.
        set_data_align=((".rodata", 4),),
        # MWCC 4-aligns the "CMdlMouth" .rodata string after the 12-byte SRT
        # table, inserting 4 zero pad bytes (+0xC..+0xF) that retail does not
        # have; remove them and shift the string symbol back.
        drop_data_range=((".rodata", 0xC, 0x10),),
        # (The old DECOMP_FORCEACTIVE keep-alive for lbl_eu_80663C70 was
        # removed from the source: the external-linkage .sdata definition
        # survives without a text anchor, so there is no emitter to drop.)

        # 43300000_80000000) MWCC materializes in .sdata2 for the
        # (f32)ml::math::mtRand(100) casts. Retail lfds the same 8 bytes
        # straight from the shared pool via lbl_eu_8066B328; rename to that
        # and strip.
        pool_patterns=(
            (struct.pack(">II", 0x43300000, 0x80000000), "lbl_eu_8066B328"),
        ),
        extern_data_sections=(".sdata2",),
    ),
    "code_8047D2AC.o": UnitRules(
        # pool-coupled: local .sdata2 pool -> CGXCache pool symbols (lbl_eu_8066Axxx).
        pool_patterns=(
            (struct.pack(">I", 0x00000000), "lbl_eu_8066A378"),
        ),
        extern_data_sections=(".sdata2",),
    ),
    "CLight.o": UnitRules(
        # pool-coupled: local .sdata2 pool -> CGXCache pool symbols (lbl_eu_8066Axxx).
        pool_patterns=(
            (struct.pack(">I", 0x00000000), "lbl_eu_8066A378"),
            (struct.pack(">I", 0x3F800000), "lbl_eu_8066A380"),
        ),
        extern_data_sections=(".sdata2",),
        exact_renames=(
            # Retail tail-merges InitLightDistAttn under its virtual Fv symbol;
            # our fake-Fv free-function definition (self+fff params) carries an
            # MWCC param-suffix alias. Rename so func_804C09E0's b-thunk and
            # extabindex resolve like retail.
            (
                "InitLightDistAttn__Q34nw4r3g3d8LightObjFff13_GXDistAttnFn__FPvffi",
                "InitLightDistAttn__Q34nw4r3g3d8LightObjFff13_GXDistAttnFn",
            ),
        ),
    ),
    "code_804D854C.o": UnitRules(
        # pool-coupled: local .sdata2 pool -> CGXCache pool symbols (lbl_eu_8066Axxx).
        pool_patterns=(
            (struct.pack(">II", 0x43300000, 0x80000000), "lbl_eu_8066A388"),
        ),
        extern_data_sections=(".sdata2",),
    ),
    "code_804DB938.o": UnitRules(
        # pool-coupled: local .sdata2 pool -> CGXCache pool symbols. Every
        # lbl_eu sdata2 ref in this unit's retail split (30 sites across
        # func_804DB980..func_804DD5B0) targets lbl_eu_8066B268; none use
        # the byte-identical copy at lbl_eu_8066A388.
        pool_patterns=(
            (struct.pack(">II", 0x43300000, 0x80000000), "lbl_eu_8066B268"),
        ),
        extern_data_sections=(".sdata2",),
    ),
    "code_804DEDA8.o": UnitRules(
        # pool-coupled: MWCC materializes TU-local int->double conversion
        # magics in .sdata2 for the schedule anim helpers (func_804E04D4 /
        # E0990 / E0B94 / E0CF0 / E17A4 / E2A5C / E30F0). Retail loads the same
        # 8 bytes straight from the shared lbl_eu_8066Bxxx pool: 2^52 via
        # lbl_eu_8066B298 (u32->f64 casts) and 2^52+2^31 via lbl_eu_8066B2A0
        # (s32->f32 casts).
        pool_patterns=(
            (struct.pack(">II", 0x43300000, 0x00000000), "lbl_eu_8066B298"),
            (struct.pack(">II", 0x43300000, 0x80000000), "lbl_eu_8066B2A0"),
        ),
        extern_data_sections=(".sdata2",),
    ),
    "code_804E36DC.o": UnitRules(
        # pool-coupled: local .sdata2 pool -> CGXCache pool symbols (lbl_eu_8066Axxx).
        pool_patterns=(
            # unsigned 2^52 int->f32 magic: retail references it as
            # lbl_eu_8066B2F0 (nw4r_data.s blob; the former A390 mapping was
            # a stale mine — retail code_804E36DC.s has no A390 refs).
            (struct.pack(">II", 0x43300000, 0x00000000), "lbl_eu_8066B2F0"),
            (struct.pack(">I", 0x00000000), "lbl_eu_8066A378"),
            (struct.pack(">I", 0x3F800000), "lbl_eu_8066A380"),
            (struct.pack(">II", 0x43300000, 0x80000000), "lbl_eu_8066A388"),
        ),
        extern_data_sections=(".sdata2",),
    ),
    "code_804D9274.o": UnitRules(
        # pool-coupled: local .sdata2 pool -> CGXCache pool symbols (lbl_eu_8066Axxx).
        pool_patterns=(
            (struct.pack(">I", 0x00000000), "lbl_eu_8066A378"),
            (struct.pack(">I", 0x3F800000), "lbl_eu_8066A380"),
            (struct.pack(">II", 0x43300000, 0x80000000), "lbl_eu_8066A388"),
            (struct.pack(">I", 0x3F000000), "lbl_eu_8066A3F0"),
            (struct.pack(">I", 0xBF800000), "lbl_eu_8066A3F8"),
        ),
        extern_data_sections=(".sdata2",),
    ),
    "code_804B59C8.o": UnitRules(
        # pool-coupled: lone 0.0f literal MWCC pools in .sdata2 (@4694, six
        # lfs sites in func_804B91E0/func_804B9818/func_804B9E14). Retail has
        # no local copy - it loads the shared monolib zero const
        # lbl_eu_8066AED0 (the same symbol the TU's other zero compares use);
        # rename and strip.
        pool_patterns=(
            (struct.pack(">I", 0x00000000), "lbl_eu_8066AED0"),
        ),
        extern_data_sections=(".sdata2",),
    ),
    "CDeviceFont.o": UnitRules(
        # pool-coupled: lone int->double magic constant (2^52, 43300000_00000000)
        # MWCC materializes in .sdata2 for wkRender's static_cast<f64>(fbWidth)
        # conversions. Retail loads the same 8 bytes straight from the CGXCache
        # pool via lfd against lbl_eu_8066A3C0; rename to that and strip.
        pool_patterns=(
            (struct.pack(">II", 0x43300000, 0x00000000), "lbl_eu_8066A3C0"),
        ),
        extern_data_sections=(".sdata2",),
    ),
    "CScnBloom.o": UnitRules(
        # pool-coupled: lone int->double magic constant (2^52, 43300000_00000000)
        # MWCC materializes in .sdata2 for the mThreshold /(f64)(u32) width
        # conversions. Retail loads the same 8 bytes straight from the CGXCache
        # pool via lfd against lbl_eu_8066A3C0; rename to that and strip.
        pool_patterns=(
            (struct.pack(">II", 0x43300000, 0x00000000), "lbl_eu_8066A3C0"),
        ),
        extern_data_sections=(".sdata2",),
    ),
    "CMdlDynamics.o": UnitRules(
        # merged: .data align 4 (was a shadowed early stub) + pool retarget.
        # pool-coupled: local .sdata2 pair (1.0f, -1.0f) from func_804EC514's
        # clamp/Atan2-denominator literals -> CGXCache pool symbols
        # lbl_eu_8066B3D4 (1.0f) / lbl_eu_8066B3E8 (-1.0f); strip.
        set_data_align=((".data", 4),),
        pool_patterns=(
            (struct.pack(">I", 0x3F800000), "lbl_eu_8066B3D4"),
            (struct.pack(">I", 0xBF800000), "lbl_eu_8066B3E8"),
        ),
        extern_data_sections=(".sdata2",),
    ),
    "CScnItemCamera.o": UnitRules(
        # pool-coupled: lone signed int->double magic constant
        # (43300000_80000000) MWCC materializes in .sdata2 for
        # static_cast<f64> conversions. Retail loads the same 8 bytes straight
        # from the shared monolib pool via lfd; retail's reloc for the s16
        # conversion magic here targets lbl_eu_8066AC30 (signed int->float
        # magic, 43300000_80000000), not the CGXCache A388 entry.
        # Current TU pools the constant in .sdata2 and references it via SDA21
        # from the s16 conversion site; rename to the retail pool label, then
        # trim .sdata2 back to the empty retail size.
        pool_patterns=(
            (struct.pack(">II", 0x43300000, 0x80000000), "lbl_eu_8066AC30"),
            (struct.pack(">II", 0x43300000, 0x00000000), "lbl_eu_8066AC08"),
        ),
        extern_data_sections=(".sdata2",),
        drop_data_tail=((".sdata2", 0x0),),
    ),
    "CVirtualLightObj.o": UnitRules(
        # pool-coupled: lone unsigned int->double magic constant
        # (43300000_00000000) MWCC materializes in .sdata2 for
        # static_cast<f64> conversions. Retail loads the same 8 bytes straight
        # from the CGXCache pool via lfd against lbl_eu_8066A3C0; rename and strip.
        pool_patterns=(
            (struct.pack(">II", 0x43300000, 0x00000000), "lbl_eu_8066A3C0"),
        ),
        extern_data_sections=(".sdata2",),
    ),
    "code_804BD8E8.o": UnitRules(
        # pool-coupled: local .sdata2 (dbl 0.0, signed int->double magic,
        # 1.0f, unsigned int->double magic) -> CGXCache pool symbols; retail
        # keeps the TU's .sdata2 EMPTY. Also fix the float-callee name MWCC
        # mangled locally (func_804BF274__Fl -> retail func_804BF274).
        pool_patterns=(
            (struct.pack(">II", 0, 0), "lbl_eu_8066A378"),
            (struct.pack(">II", 0x43300000, 0x80000000), "lbl_eu_8066A388"),
            (struct.pack(">I", 0x3F800000), "lbl_eu_8066A380"),
            (struct.pack(">II", 0x43300000, 0x00000000), "lbl_eu_8066A3C0"),
        ),
        exact_renames=(("func_804BF274__Fl", "func_804BF274"),),
        extern_data_sections=(".sdata2",),
        # retail .sbss is align 4; MWCC emits 8 without this. Keep together
        # with the pool rule - do NOT split into a second entry.
        set_data_align=((".sbss", 4),),
    ),
    "CScnMaruShadowNw4r.o": UnitRules(
        # pool-coupled: MWCC pools four literals in a local .sdata2 (@1892
        # 0.0f, @1893 double 1.0, @1897 2^52 int->f64 magic, @1939 1.0f) but
        # retail has an EMPTY .sdata2 - every load is sda21/lfd against the
        # shared monolib pool symbols this TU already references:
        #   0.0f   -> lbl_eu_8066A948   (lfs sites throughout)
        #   dbl1.0 -> lbl_eu_8066A9A8   (the >= 1.0 wrap clamp lfd)
        #   2^52   -> lbl_eu_8066A950   (rgba alpha u8->f64 conversion lfd)
        #   1.0f   -> lbl_eu_8066A944   (lfs sites throughout)
        pool_patterns=(
            (struct.pack(">I", 0x00000000), "lbl_eu_8066A948"),
            (struct.pack(">II", 0x3FF00000, 0), "lbl_eu_8066A9A8"),
            (struct.pack(">II", 0x43300000, 0), "lbl_eu_8066A950"),
            (struct.pack(">I", 0x3F800000), "lbl_eu_8066A944"),
        ),
        extern_data_sections=(".sdata2",),
    ),
    "CDeviceGX.o": UnitRules(
        # merged: .data/.sbss align 4 (was a shadowed early stub) + full dissolve.
        # monolibdata2 dissolve: retail .data = the CDeviceGX vtable
        # RTTI -> external lbl_eu_80663758); MWCC also emits base vtables
        # (+0xC0) and local RTTI. Drop the base vtables + rename the RTTI,
        # strip the extra sdata/sdata2/rodata.
        # .sdata tail: MWCC emits local weak copies of every RTTI struct the
        # kept vtable base-list references (CDeviceGX/CDeviceVICb/CDeviceBase/
        # CWorkThread/IWorkEvent). Retail defines none of them here - the
        # base-list words are extern refs resolved by the owning TUs. Drop all
        # five; exact_renames below retarget VICb/Base onto the retail pool
        # labels, the rest keep their mangled names.
        drop_data_range=((".data", 0xE8, 0x1A8), (".sdata", 0x10, 0x38),),
        drop_data_tail=((".rodata", 0x10),),
        extern_data_sections=(".sdata2",),
        # FORCEACTIVE anchor stub (unreferenced) + weak inline-base-dtor
        # over-emission: retail split carries neither. The base dtor keeps a
        # LIVE .data vtable reference -> UNDEF (strong copy links from
        # CDevice.cpp). Dropping both frees 0x78 so drawSyncCallback /
        # setValues / @456@ thunks fit inside the 0x8E8 split.
        drop_text_symbols=("FORCEACTIVECDeviceGX_cpp74__Fv",),
        drop_text_symbols_as_undef=("__dt__11CDeviceBaseFv", "viBeforeDrawDone__11CDeviceVICbFv"),
        repack_after_drop=4,
        exact_renames=(
            ("__RTTI__9CDeviceGX", "lbl_eu_80663758"),
            # compiler-generated base-list entries -> retail pool names
            ("__RTTI__11CDeviceVICb", "lbl_eu_80663618"),
            ("__RTTI__11CDeviceBase", "lbl_eu_806635F0"),
        ),
        # Retail .data/.sbss are align 8; MWCC emits 4 for this TU.
        set_data_align=((".data", 8), (".sbss", 8)),
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
        # Data dissolve: retail CGame.o KEEPS its own data sections, so no
        # stripping - only positional renames so both sides' reloc lists
        # match. scViewName -> "XENOBLADE" pool label; typeinfo descriptor
        # name/vtable words map onto the retail split's local @4183..@4191
        # objects positionally; sdata2 float slots onto @4482/@4566.
        exact_renames=(
            # @N indices drift with every CGame.cpp edit - refresh from the
            # data-diff reloc list when it regresses.
            ("@14924", "lbl_eu_804FA1E0"),
            ("@15405", "@4183"),
            ("@15406", "@4184"),
            ("@15407", "@4185"),
            ("@15408", "@4186"),
            ("@15411", "@4187"),
            ("@15412", "@4188"),
            ("@15413", "@4189"),
            ("@15414", "@4190"),
            ("@15415", "@4191"),
        ),
        # MWCC skips the 7 tail NULs after "CGame\\0" that retail keeps
        # inside @stringBase0's 0x30 footprint.
        pad_data_section=((".rodata", 0x78),),
    ),
    # MWCC switch cookies for func_8003B6A0 / func_8003B800.
    # VM wrappers: retail asm keeps address-suffixed names; MWCC emits short names.
    "ocBdat.o": UnitRules(
        exact_renames=(
            # MWCC cookie ids drift with TU growth; size-0x24 switch tables.
            # (2026-08: cookies renumbered @1095/@1252 -> @862/@995 after TU
            # growth; both are ABS size-0 remnants of the stripped .data
            # jumptables.)
            ("@862", "jumptable_eu_80524D90"),
            ("@995", "jumptable_eu_80524DB8"),
            ("bdat", "bdat_8003BD4C"),
            ("getVal", "getVal_8003BDB8"),
            ("getArrayVal", "getArrayVal_8003BE70"),
            ("getArrayCount", "getArrayCount_8003BF48"),
            ("getVarType", "getVarType_8003BFB8"),
            ("getIdCount", "getIdCount_8003C028"),
            ("getIdTop", "getIdTop_8003C074"),
            ("getFlagVal", "getFlagVal_8003C0C0"),
        ),
        # Data dissolve: the two 0x24 switch cookies live in the retail split
        # object's .data; strip the MWCC copy after renaming so both sides are
        # empty and the .text relocs resolve to the retail jumptables.
        extern_data_sections=(".data",),
    ),
    # --- kyoshin data-dissolve batch (retail split objects carry NO data;
    # everything ships from split1.s / monolibdata blobs; mappings verified
    # against each unit's split-asm @sda21/@ha/@l annotations) ---
    "ocUnit.o": UnitRules(
        # Content-matched pool slots (unique per pattern in this TU):
        # 0.019592438f -> monolibdata lbl_eu_8066A20C (CfObject::
        # UnkVirtualFunc34 multiplier), int->double magic -> lbl_eu_80665C38,
        # 2048.0f stand-in slot -> lbl_eu_80665C30 (retail lfs target).
        pool_patterns=(
            (struct.pack(">I", 0x3CA08052), "lbl_eu_8066A20C"),
            (struct.pack(">II", MAGIC_HI, MAGIC_LO), "lbl_eu_80665C38"),
            (struct.pack(">I", 0x45000000), "lbl_eu_80665C30"),
        ),
        exact_renames=(
            # dynamic_cast RTTI descriptor: retail references the .sdata blob
            # label lbl_eu_806618D8 (= __RTTI__Q22cf13CfObjectActor, func_8003EB64).
            ("__RTTI__Q22cf13CfObjectActor", "lbl_eu_806618D8"),
        ),
        extern_data_sections=(".sdata2", ".sdata"),
    ),
    "pluginEve.o": UnitRules(
        # fadeIn/fadeOut fade-color loads. Retail sites read the plugin
        # command-string gap objects in split1.s: "@1254" (0x80665DF8, 0.0f
        # pair) and "@1255" (0x80665DFC, 1.0f pair). Site correspondence puts
        # the decomp 1.0f slot on "@1254" and the 0.0f slot on "@1255" (the
        # source stand-in values are swapped vs retail - §17.6 residual), so
        # the content patterns intentionally cross.
        pool_patterns=(
            (struct.pack(">I", 0x3F800000), "@1254"),
            (struct.pack(">I", 0x00000000), "@1255"),
        ),
        extern_data_sections=(".sdata2",),
    ),
    "pluginCfs.o": UnitRules(
        # int->double magic -> lbl_eu_80665E40 (lfd ground truth); the extra
        # unmatched-residual 1.0f slot maps to the value-equal split1 float
        # lbl_eu_80668358.
        pool_patterns=(
            (struct.pack(">II", MAGIC_HI, MAGIC_LO), "lbl_eu_80665E40"),
            (struct.pack(">I", 0x3F800000), "lbl_eu_80668358"),
        ),
        extern_data_sections=(".sdata2",),
    ),
    "pluginCam.o": UnitRules(
        # int->double magic -> lbl_eu_80666170 (the TU already carries a
        # source-declared lbl_eu_80666170 copy; both resolve to the blob).
        pool_patterns=(
            (struct.pack(">II", MAGIC_HI, MAGIC_LO), "lbl_eu_80666170"),
        ),
        extern_data_sections=(".sdata2",),
    ),
    "CfTFile.o": UnitRules(
        # 2^52 double constant -> lbl_eu_80666220 (content + site verified).
        pool_patterns=(
            (struct.pack(">d", 4503599627370496.0), "lbl_eu_80666220"),
        ),
        extern_data_sections=(".sdata2",),
    ),
    "CfCamEvent_1.o": UnitRules(
        # Camera-event float pools: 2^52 -> lbl_eu_80666420, int->double magic
        # -> lbl_eu_80666438 (both content-equal unit-ref'd labels); the lone
        # zero-word slot maps to lbl_eu_805273C8 per site correspondence.
        pool_patterns=(
            (struct.pack(">d", 4503599627370496.0), "lbl_eu_80666420"),
            (struct.pack(">II", MAGIC_HI, MAGIC_LO), "lbl_eu_80666438"),
            (struct.pack(">I", 0x00000000), "lbl_eu_805273C8"),
        ),
        extern_data_sections=(".sdata2",),
    ),
    "CMenuCollepedia.o": UnitRules(
        # retail's CMenuCollepedia dtor calls the game-side CProcess dtor
        # wrapper __dt__800FED0C (unit .s has 3 refs, zero __dt__8CProcessFv);
        # MWCC canonicalizes any CProcess-dtor-shaped extern name against the
        # declared virtual ~CProcess, so the rename can't be done in source.
        exact_renames=(("__dt__8CProcessFv", "__dt__800FED0C"),),
        # merged from a former second entry: dead MWCC-emitted string/float
        # pools with no kept-section relocs.
        extern_data_sections=(".data", ".rodata", ".sdata"),
    ),
    "CtrlMoveBase.o": UnitRules(
        # Lone zero-word float slot -> lbl_eu_806665A0 (site correspondence).
        pool_patterns=(
            (struct.pack(">I", 0x00000000), "lbl_eu_806665A0"),
        ),
        extern_data_sections=(".sdata2",),
    ),
    "CtrlMoveNpc.o": UnitRules(
        # retail exports the ctor under the friendly map name __ct__cf_CtrlMoveNpc
        # (symbols.txt .text:0x801A0DAC); the TU defines the real C++ member ctor
        # which MWCC mangles __ct__Q22cf12CCtrlMoveNpcFv - rename in postprocess
        # (same recipe as CDeviceFileCri's ctor rename, MWCC_CASES ref:66fef79dfa).
        exact_renames=(("__ct__Q22cf12CCtrlMoveNpcFv", "__ct__cf_CtrlMoveNpc"),),
    ),
    "CtrlMoveEne.o": UnitRules(
        # magic -> lbl_eu_80666620, 2^52 -> lbl_eu_80666650 (content-equal
        # unit-ref'd labels); the extra unmatched-residual zero slot gets the
        # value-equal lbl_eu_80665E4C.
        pool_patterns=(
            (struct.pack(">II", MAGIC_HI, MAGIC_LO), "lbl_eu_80666620"),
            (struct.pack(">d", 4503599627370496.0), "lbl_eu_80666650"),
            (struct.pack(">I", 0x00000000), "lbl_eu_80665E4C"),
        ),
        extern_data_sections=(".sdata2",),
    ),
    "CtrlNpc.o": UnitRules(
        # magic -> lbl_eu_806666C0 (content match); zero slot ->
        # lbl_eu_80666698 (verified by ref-index alignment in func_800964EC).
        pool_patterns=(
            (struct.pack(">II", MAGIC_HI, MAGIC_LO), "lbl_eu_806666C0"),
            (struct.pack(">I", 0x00000000), "lbl_eu_80666698"),
        ),
        extern_data_sections=(".sdata2",),
    ),
    "CtrlRemote.o": UnitRules(
        # int->double magic -> lbl_eu_80666740 (content match).
        pool_patterns=(
            (struct.pack(">II", MAGIC_HI, MAGIC_LO), "lbl_eu_80666740"),
        ),
        extern_data_sections=(".sdata2",),
    ),
    "CtrlObjectParam.o": UnitRules(
        # 2^52 double -> lbl_eu_80666778 (content match).
        pool_patterns=(
            (struct.pack(">d", 4503599627370496.0), "lbl_eu_80666778"),
        ),
        extern_data_sections=(".sdata2",),
    ),
    "code_800A3B24.o": UnitRules(
        # magic -> lbl_eu_806667E0 (content match); debug-draw residual 1.0f
        # slots have no retail counterpart -> value-equal lbl_eu_80668358.
        pool_patterns=(
            (struct.pack(">II", MAGIC_HI, MAGIC_LO), "lbl_eu_806667E0"),
            (struct.pack(">I", 0x3F800000), "lbl_eu_80668358"),
        ),
        extern_data_sections=(".sdata2",),
    ),
    "code_800A75FC.o": UnitRules(
        # Data dissolve: the lone anonymous 4-byte .sdata2 slot has no live
        # .text reference (unmatched residual only); strip to the empty
        # retail section.
        extern_data_sections=(".sdata2",),
    ),
    "CTaskGame.o": UnitRules(
        # Data dissolve: vtables/typeinfo/string pools ship from split1.s
        # (lbl_eu_80525868/98C vtables, 80663D18-D34 sbss statics, 80665D6C/
        # D80 sdata2 pool). Pool order: 1.0f -> lbl_eu_80665D6C, the -1.0f
        # stand-in slot sits where retail loads the singleton pointer
        # lbl_eu_80663D18 (ctor site alignment), magic -> lbl_eu_80665D80.
        pool_patterns=(
            (struct.pack(">I", 0x3F800000), "lbl_eu_80665D6C"),
            (struct.pack(">I", 0xBF800000), "lbl_eu_80663D18"),
            (struct.pack(">II", MAGIC_HI, MAGIC_LO), "lbl_eu_80665D80"),
        ),
        exact_renames=(
            ("CTaskGame_cLoadInstance", "lbl_eu_80663D24"),
            ("lbl_80666624", "lbl_eu_80663D30"),
            ("lbl_80666630", "lbl_eu_80663D34"),
            ("__vt__9CTaskGame", "lbl_eu_80525868"),
            ("__vt__18CTask<9CTaskGame>", "lbl_eu_8052598C"),
        ),
        extern_data_sections=(".data", ".rodata", ".sdata", ".sdata2", ".sbss"),
    ),
    "CTaskGameEff.o": UnitRules(
        # _reslist_base<CScn>/CEffRenderHighPrio vtables+RTTI+typeinfo names
        # all ship from split1.s (lbl_eu_80525BFC..CAC); no .text relocs
        # reference the local copies beyond those already renamed.
        extern_data_sections=(".data", ".rodata", ".sdata", ".sdata2"),
    ),
    "CTaskGameEffAfter.o": UnitRules(
        # CTTask<CTaskGameEffAfter> vtable family ships from split1.s; no
        # live .text refs to the local copies.
        extern_data_sections=(".data", ".rodata", ".sdata"),
    ),
    "CActParamAnim.o": UnitRules(
        # CActParamAnim vtable/RTTI ship from split1.s lbl_eu_805261C8. Pool:
        # int->double magic -> lbl_eu_80665EE8, 2^52 -> lbl_eu_80665EA0 (site
        # correspondence in func_8004CF00 / func_80052934).
        pool_patterns=(
            (struct.pack(">II", MAGIC_HI, MAGIC_LO), "lbl_eu_80665EE8"),
            (struct.pack(">d", 4503599627370496.0), "lbl_eu_80665EA0"),
        ),
        extern_data_sections=(".data", ".rodata", ".sdata", ".sdata2"),
    ),
    "CActParamData.o": UnitRules(
        # magic -> lbl_eu_80665F98, 2^52 -> lbl_eu_80665F90 (content-equal
        # unit-ref'd labels); vtable/RTTI from lbl_eu_80526338.
        pool_patterns=(
            (struct.pack(">II", MAGIC_HI, MAGIC_LO), "lbl_eu_80665F98"),
            (struct.pack(">d", 4503599627370496.0), "lbl_eu_80665F90"),
        ),
        extern_data_sections=(".data", ".rodata", ".sdata", ".sdata2"),
    ),
    "CActParamAnimGame.o": UnitRules(
        # Animation-constant pool: every slot content-matches exactly one
        # unit-referenced split1.s label (block at lbl_eu_80666040..108);
        # ±1.0472 (60 deg) slots have no local blob twin - mapped to their
        # site-correspondent labels per func_8005BC14 window alignment.
        pool_patterns=(
            (struct.pack(">I", 0x00000000), "lbl_eu_80666040"),
            (struct.pack(">I", 0x3F800000), "lbl_eu_80666044"),
            (struct.pack(">d", 4503599627370496.0), "lbl_eu_806660A8"),
            (struct.pack(">I", 0x3F000000), "lbl_eu_80666080"),
            (struct.pack(">I", 0x40000000), "lbl_eu_80666050"),
            (struct.pack(">I", 0x4222F983), "lbl_eu_806660CC"),
            (struct.pack(">I", 0x3F99999A), "lbl_eu_806660D0"),
            (struct.pack(">I", 0x3DCCCCCD), "lbl_eu_806660D4"),
            (struct.pack(">I", 0xBFC00000), "lbl_eu_806660D8"),
            (struct.pack(">I", 0x40400000), "lbl_eu_806660DC"),
            (struct.pack(">I", 0x3F350481), "lbl_eu_8066AF20"),
            (struct.pack(">I", 0x3CC90FDB), "lbl_eu_806660E0"),
            (struct.pack(">I", 0x38D1B717), "lbl_eu_806660E4"),
            (struct.pack(">I", 0x3E99999A), "lbl_eu_80666068"),
            (struct.pack(">I", 0x3F860A92), "lbl_eu_806660E8"),
            (struct.pack(">I", 0xBF860A92), "lbl_eu_80663D6C"),
            (struct.pack(">I", 0x447A0000), "lbl_eu_80666070"),
            (struct.pack(">I", 0x45610000), "lbl_eu_806660F0"),
            (struct.pack(">I", 0x3C23D70A), "lbl_eu_8066606C"),
            (struct.pack(">I", 0x3A83126F), "lbl_eu_806660F4"),
            (struct.pack(">I", 0x3E800000), "lbl_eu_806660F8"),
            (struct.pack(">I", 0x3BD1B717), "lbl_eu_806660FC"),
            (struct.pack(">I", 0x3DA3D70A), "lbl_eu_80666100"),
            (struct.pack(">I", 0x3E4CCCCD), "lbl_eu_80666104"),
            (struct.pack(">I", 0x3ECCCCCD), "lbl_eu_806660C0"),
            (struct.pack(">I", 0x3F666666), "lbl_eu_806660C4"),
            (struct.pack(">I", 0x3FB33333), "lbl_eu_80666108"),
        ),
        extern_data_sections=(".sdata2", ".bss", ".sbss"),
    ),
    "CfRes.o": UnitRules(
        # Switch cookie lives in the retail split object's .data
        # (jumptable_eu_805266F8); strip the MWCC copy after renaming.
        # @6104 is the same cookie after TU growth renamed the anon slot.
        exact_renames=(
            ("@6107", "jumptable_eu_805266F8"),
            ("@6104", "jumptable_eu_805266F8"),
        ),
        extern_data_sections=(".data",),
    ),
    "IResInfo.o": UnitRules(
        # Switch cookie lives in the retail split object's .data
        # (jumptable_eu_805268C8); strip the MWCC copy after renaming.
        exact_renames=(
            ("@5342", "jumptable_eu_805268C8"),
        ),
        extern_data_sections=(".data",),
    ),
    "CfTaskMain.o": UnitRules(
        # CTTask<CfTaskMain> vtable family ships from split1.s; no live
        # .text refs to the local copies.
        extern_data_sections=(".data", ".rodata", ".sdata"),
    ),
    "CfCamEvent.o": UnitRules(
        # Static camera-interface instances ship from the retail sbss slice;
        # source already names them lbl_eu_* (strip keeps UNDEF refs).
        extern_data_sections=(".bss", ".sbss"),
    ),
    "CfGameManager.o": UnitRules(
        # int->double magic -> lbl_eu_80666530 (content match); all other
        # statics are source-declared lbl_eu_* names resolving to the blob.
        pool_patterns=(
            (struct.pack(">II", MAGIC_HI, MAGIC_LO), "lbl_eu_80666530"),
        ),
        extern_data_sections=(".sdata2", ".bss", ".sbss"),
    ),
    "CtrlEnemy.o": UnitRules(
        # int->double magic -> lbl_eu_80666588 (content match); CtrlAct/
        # CtrlBase vtable+RTTI copies are internally wired only.
        pool_patterns=(
            (struct.pack(">II", MAGIC_HI, MAGIC_LO), "lbl_eu_80666588"),
        ),
        extern_data_sections=(".data", ".rodata", ".sdata", ".sdata2"),
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
        # Retail split carries no .sdata2; after the content renames above
        # both slots resolve to monolibdata floats at link. Strip.
        extern_data_sections=(".sdata2",),
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
            # Data dissolve: retail keeps ALL data in the split blobs.
            # - func_8023587C / func_802375A8 switch jumptables ship from
            #   split1.o (site-confirmed: lis/addi jumptable_eu_805369A0 at
            #   802377A0, jumptable_eu_805369D4 at 8023955C).
            ("@7793", "jumptable_eu_805369A0"),
            ("@8143", "jumptable_eu_805369D4"),
        ),
        # int->double conversion magics live in split1.s .sdata2 (bytes
        # verified: 80668698 = 2^52+2^31, 806686A8 = 2^52).
        pool_patterns=(
            (struct.pack(">II", 0x43300000, 0x80000000), "lbl_eu_80668698"),
            (struct.pack(">II", 0x43300000, 0x00000000), "lbl_eu_806686A8"),
        ),
        extern_data_sections=(".sdata2", ".data"),
    ),
    # ---- kyoshin data-dissolve batch (retail keeps ALL data in split blobs;
    # each entry strips the TU-local copies and retargets live refs) ----
"code_802405F4.o": UnitRules(
        # Move__14CMenuMapSelectFv dispatch jumptable ships from split1.s
        # (site-confirmed: lis/addi jumptable_eu_80536CF8 at 80244034).
        # Size-keyed (12 all-reloc zero words) so MWCC anon renumbering
        # across TU rebuilds cannot stale the key.
        data_pool_patterns=(
            (".data", bytes(0x30), "jumptable_eu_80536CF8"),
        ),
        extern_data_sections=(".data",),
    ),
    "CMapSel.o": UnitRules(
        # __ct__CMapSel dispatch jumptable ships from split1.s
        # (site-confirmed: lis/addi jumptable_eu_80536D98 at 802453C0).
        exact_renames=(
            ("@10279", "jumptable_eu_80536D98"),
        ),
        # lone 1.0f pool constant -> shared .sdata2 (bytes verified).
        pool_patterns=(
            (struct.pack(">I", 0x3F800000), "lbl_eu_8066873C"),
        ),
        extern_data_sections=(".sdata2", ".data"),
    ),
    "CCollepedia.o": UnitRules(
        # func_80254A20 / func_802553AC dispatch jumptables ship from
        # split1.s (site-confirmed lis/addi jumptable_eu_80537378 at
        # 80256C90; 805373BC slots spell func_802553AC case labels).
        exact_renames=(
            ("@8943", "jumptable_eu_80537378"),
            ("@9159", "jumptable_eu_805373BC"),
        ),
        # int->double conversion magic 2^52 -> shared .sdata2 (bytes verified).
        pool_patterns=(
            (struct.pack(">II", 0x43300000, 0x00000000), "lbl_eu_80668808"),
        ),
        extern_data_sections=(".sdata2", ".data", ".rodata", ".sdata"),
    ),
"CMenuKizunagram.o": UnitRules(
        # Move__15CMenuKizunagramFv dispatch jumptable ships from split1.s
        # (site-confirmed: lis/addi jumptable_eu_80537480 at 80259320).
        # Size-keyed (11 all-reloc zero words) against anon renumbering.
        data_pool_patterns=(
            (".data", bytes(0x2c), "jumptable_eu_80537480"),
        ),
        extern_data_sections=(".data",),
    ),
    "CKizunagram.o": UnitRules(
        # func_8025BA38 dispatch jumptable ships from split1.s
        # (site-confirmed: lis/addi jumptable_eu_80537520 at 8025DF70).
        exact_renames=(
            ("@11189", "jumptable_eu_80537520"),
        ),
        # int->double conversion magic 2^52+2^31 -> shared .sdata2
        # (bytes verified against lbl_eu_80668840).
        pool_patterns=(
            (struct.pack(">II", 0x43300000, 0x80000000), "lbl_eu_80668840"),
        ),
        extern_data_sections=(".sdata2", ".data", ".rodata", ".sdata"),
    ),
    "CPcKizunagram.o": UnitRules(
        # int->double conversion magic 2^52 -> shared .sdata2
        # (bytes verified against lbl_eu_806688B0).
        pool_patterns=(
            (struct.pack(">II", 0x43300000, 0x00000000), "lbl_eu_806688B0"),
        ),
        extern_data_sections=(".sdata2",),
    ),
    "code_8025FB10.o": UnitRules(
        # int->double conversion magics -> shared .sdata2 (bytes verified:
        # 806688C8 = 2^52+2^31 signed, 806688E8 = 2^52 unsigned).
        pool_patterns=(
            (struct.pack(">II", 0x43300000, 0x80000000), "lbl_eu_806688C8"),
            (struct.pack(">II", 0x43300000, 0x00000000), "lbl_eu_806688E8"),
        ),
        # .sbss defines are already spelled lbl_eu_* (extern-style decls);
        # stripping turns them into UNDEFs resolving against split1.o.
        extern_data_sections=(".sdata2", ".sbss"),
    ),
    "CfNandManager.o": UnitRules(
        # func_8023EB78's pending-type switch: MWCC lowers the current source
        # shape to an in-TU jumptable (@NNN over its own case labels), but the
        # retail form is a compare chain with NO data object anywhere in the
        # splits - the table dies together with its two lis/addi refs once the
        # function's codegen is brought onto the retail shape (source-level
        # follow-up). Until then there is nothing legitimate to rename onto,
        # so only the storage is stripped.
        extern_data_sections=(".data",),
    ),
    "CMenuPause.o": UnitRules(
        # Lone dead MWCC float constant; retail has no local copy and no
        # kept-section reloc references it.
        extern_data_sections=(".sdata2",),
    ),
    "CMenuPassiveSkill.o": UnitRules(
        # Dead MWCC-emitted string/float pools; no kept-section relocs.
        extern_data_sections=(".data", ".rodata", ".sdata"),
    ),
"CFloorMap.o": UnitRules(
        # func_8024F1FC dispatch jumptable ships from split1.s
        # (jumptable_eu_80536FB0; 29 slots spelling func_8024F1FC cases).
        # Size-keyed so MWCC anon renumbering cannot stale the key.
        data_pool_patterns=(
            (".data", bytes(0x74), "jumptable_eu_80536FB0"),
        ),
        # int->double magic 2^52+2^31 -> shared pool lbl_eu_80668770
        # (bytes verified; the like-named local def is skipped by the
        # content matcher and its refs resolve to the blob at link).
        pool_patterns=(
            (struct.pack(">II", 0x43300000, 0x80000000), "lbl_eu_80668770"),
        ),
        extern_data_sections=(".sdata2", ".data"),
    ),
    "CPassiveSkill.o": UnitRules(
        # func_80267360 dispatch jumptable ships from split1.s
        # (jumptable_eu_80537920; 17 slots spelling func_80267360 cases).
        exact_renames=(
            ("@11825", "jumptable_eu_80537920"),
        ),
        # int->double magic 2^52 -> shared pool lbl_eu_80668910 (bytes
        # verified; like-named local def skipped, resolves to blob).
        pool_patterns=(
            (struct.pack(">II", 0x43300000, 0x00000000), "lbl_eu_80668910"),
        ),
        extern_data_sections=(".sdata2", ".data", ".sbss2"),
    ),
    "CfGimmickEne.o": UnitRules(
        # int->double magic 2^52 and read-only 1.0f -> the shared .sdata2
        # pool (bytes verified: lbl_eu_80668980 / lbl_eu_80668968; note
        # lbl_eu_80662784 is a MUTABLE .sdata static, not this constant).
        # The lone -1.0f (@3645) has no referenced blob counterpart - retail
        # codegen for the site function loads neither - so it is stripped
        # unrenamed and dies with the pending code match.
        pool_patterns=(
            (struct.pack(">I", 0x3F800000), "lbl_eu_80668968"),
            (struct.pack(">II", 0x43300000, 0x00000000), "lbl_eu_80668980"),
        ),
        extern_data_sections=(".sdata2",),
    ),
    "CKizunaTalkList.o": UnitRules(
        # Lone 2.0f MWCC materialization with no referenced blob counterpart
        # (retail func_80273984 loads no float label) - strip only.
        extern_data_sections=(".sdata2",),
    ),
    "CMenuPlayAward.o": UnitRules(
        # int->double magic 2^52 -> shared .sdata2 (bytes verified against
        # lbl_eu_806689B0).
        pool_patterns=(
            (struct.pack(">II", 0x43300000, 0x00000000), "lbl_eu_806689B0"),
        ),
        extern_data_sections=(".sdata2",),
    ),
    "CMenuPTChangeNotice.o": UnitRules(
        # int->double magic 2^52 -> shared .sdata2 (bytes verified against
        # lbl_eu_80668B88).
        pool_patterns=(
            (struct.pack(">II", 0x43300000, 0x00000000), "lbl_eu_80668B88"),
        ),
        extern_data_sections=(".sdata2",),
    ),
    "CChainActorPc.o": UnitRules(
        # int->double magic 2^52+2^31 -> shared .sdata2 (bytes verified
        # against lbl_eu_80668AF8).
        pool_patterns=(
            (struct.pack(">II", 0x43300000, 0x80000000), "lbl_eu_80668AF8"),
        ),
        extern_data_sections=(".sdata2",),
    ),
"CChain.o": UnitRules(
        # func_80276D30 / func_80277B38 dispatch jumptables ship from
        # split1.s (jumptable_eu_805381D4 = 11 slots, jumptable_eu_80538200
        # = 27 slots over the same functions' case labels). Size-keyed
        # (distinct 0x2c/0x6c word counts) against anon renumbering.
        data_pool_patterns=(
            (".data", bytes(0x2c), "jumptable_eu_805381D4"),
            (".data", bytes(0x6c), "jumptable_eu_80538200"),
        ),
        # int->double magic 2^52+2^31 -> shared .sdata2 (bytes verified
        # against lbl_eu_80668A38).
        pool_patterns=(
            (struct.pack(">II", 0x43300000, 0x80000000), "lbl_eu_80668A38"),
        ),
        extern_data_sections=(".sdata2", ".data"),
    ),
    "CChainTimer.o": UnitRules(
        # Class data dissolves to split blobs: vtables identified by their
        # __dt__ slots (80538348 = Flusher<CfObjectActor>, 80538278 =
        # CChainTimer); RTTI name strings by content (8050EDF0 / 8050ED30);
        # int->double magic 2^52+2^31 -> lbl_eu_80668AA0.
        exact_renames=(
            ("__vt__Q22cf29Flusher<Q22cf13CfObjectActor>", "lbl_eu_80538348"),
            ("__vt__Q22cf11CChainTimer", "lbl_eu_80538278"),
            ("@9207", "lbl_eu_8050ED30"),
        ),
        pool_patterns=(
            (struct.pack(">II", 0x43300000, 0x80000000), "lbl_eu_80668AA0"),
        ),
        extern_data_sections=(".sdata2", ".data", ".rodata", ".sdata"),
    ),
    "CChainActorList.o": UnitRules(
        # Vtables identified by their __dt__ slots (80538308 =
        # CChainActorList, 80538314 = reslist<CChainActor*>, 8053832C =
        # _reslist_base<CChainActor*>); RTTI name string @7307 =
        # "_reslist_base<cf::CChainActor *>" -> lbl_eu_8050ED98. .sbss
        # define is already spelled lbl_eu_80668A80 (resolves via blob).
        exact_renames=(
            ("__vt__Q22cf15CChainActorList", "lbl_eu_80538308"),
            ("__vt__28reslist<PQ22cf11CChainActor>", "lbl_eu_80538314"),
            ("__vt__34_reslist_base<PQ22cf11CChainActor>", "lbl_eu_8053832C"),
            ("@7307", "lbl_eu_8050ED98"),
        ),
        extern_data_sections=(".data", ".rodata", ".sdata", ".sbss"),
    ),
    "CSaveLoad.o": UnitRules(
        # func_8028F2CC dispatch jumptable ships from split1.s
        # (jumptable_eu_80538780; 13 slots spelling func_8028F2CC cases).
        exact_renames=(
            ("@11415", "jumptable_eu_80538780"),
        ),
        extern_data_sections=(".data",),
    ),
    "CMenuMapSelectSC.o": UnitRules(
        # Move__16CMenuMapSelectSCFv dispatch jumptable ships from split1.s
        # (jumptable_eu_80537110; 8 slots spelling Move case labels). The
        # 1-byte static guard in .sbss has no retail counterpart (retail
        # func_80251294 touches no such byte) - stripped unrenamed.
        exact_renames=(
            ("@10881", "jumptable_eu_80537110"),
        ),
        extern_data_sections=(".data", ".sbss"),
    ),
    "CMenuKizunaTalkList.o": UnitRules(
        # Dead MWCC-emitted string/float pools; no kept-section relocs.
        extern_data_sections=(".data", ".rodata", ".sdata"),
        # ~CMenuKizunaTalkList (compiler-generated destroying dtor) calls the
        # base CProcess dtor; MWCC binds it to the locally emitted
        # __dt__8CProcessFv copy, but retail calls the game-side wrapper
        # __dt__800FED0C (defined in CMainMenu). The local copy has exactly
        # this one referencer, so an exact rename suffices.
        exact_renames=(
            ("__dt__8CProcessFv", "__dt__800FED0C"),
        ),
    ),
    "code_8027513C.o": UnitRules(
        # Dead MWCC-emitted pools; no kept-section relocs reference them.
        extern_data_sections=(".data", ".rodata", ".sdata"),
    ),
    "CMenuArtsSelect.o": UnitRules(
        # Five switch jumptables map 1:1 by case-target function:
        #   Move dispatch       -> jumptable_eu_8052BFB0
        #   func_801072E0 lo/hi -> jumptable_eu_8052BFD4 / 8052BFF8
        #   func_80107580 lo/hi -> jumptable_eu_8052C02C / 8052C050
        # Two 0x10 id tables load at retail sites 80106D94 (FD128) and
        # 801080A4 (FD138); WIP decomp contents differ from retail but the
        # storage class/size match. Float cluster F28..F38 maps by content;
        # the -140.0f slot has no unit-ref'd counterpart (WIP constant),
        # mapped to the matching-value blob label lbl_eu_80666DA0.
        exact_renames=(
            ("@17086", "jumptable_eu_8052BFB0"),
            ("@17962", "jumptable_eu_8052BFD4"),
            ("@17961", "jumptable_eu_8052BFF8"),
            ("@18007", "jumptable_eu_8052C02C"),
            ("@18006", "jumptable_eu_8052C050"),
            ("@14641", "lbl_eu_804FD128"),
            ("@14737", "lbl_eu_804FD138"),
        ),
        pool_patterns=(
            (struct.pack(">I", 0x00000000), "lbl_eu_80666F28"),
            (struct.pack(">I", 0xC3120000), "lbl_eu_80666DA0"),
            (struct.pack(">I", 0x3F800000), "lbl_eu_80666F2C"),
            (struct.pack(">II", MAGIC_HI, MAGIC_LO), "lbl_eu_80666F30"),
            (struct.pack(">II", MAGIC_HI, 0x00000000), "lbl_eu_80666F38"),
        ),
        extern_data_sections=(".data", ".rodata", ".sdata", ".sdata2"),
    ),
    "CMenuBattlePlayerState.o": UnitRules(
        # Move: MWCC int→float biases as TU-local @N; retail lbl_eu_80666FA8/FB8.
        # Refreshed: the two magic doubles now carry their retail names as
        # LOCAL defs -> globalize so the strip resolves externally; the
        # remaining floats map content-first (21.75->F9C, 1.0->F90, 0->F94).
        pool_patterns=(
            (struct.pack(">I", 0x42AC0000), "lbl_eu_80666F9C"),
            (struct.pack(">I", 0x3F800000), "lbl_eu_80666F90"),
            (struct.pack(">I", 0x00000000), "lbl_eu_80666F94"),
        ),
        globalize_symbols=("lbl_eu_80666FB8", "lbl_eu_80666FA8"),
        extern_data_sections=(".data", ".rodata", ".sdata", ".sdata2", ".bss"),
    ),
    "COccCulling.o": UnitRules(
        # Inline CPlane::isOnPositiveSide emits a TU-local 0.0f pool.
        pool_patterns=(
            (struct.pack(">I", 0x00000000), "lbl_eu_80667C8C"),
        ),
        # Data dissolve: string table + vtable ship from split1.s
        # ("OCC_CULL\0" @80503DF4, vtable @80532ED0); strip the TU copies.
        exact_renames=(
            ("@stringBase0", "lbl_eu_80503DF4"),
            ("__vt__11COccCulling", "lbl_eu_80532ED0"),
        ),
        extern_data_sections=(".rodata", ".data", ".sdata", ".sdata2"),
        # The __sinit_ corner-point table (lbl_eu_805757F0, 0x30) is owned by
        # the shared retail data slice, not this TU's split: zero the NOBITS
        # copy and UNDEF the symbol so sinit/setFrustum refs link externally
        # (same shape as code_80296898 / CCol4).
        zero_nobits=(".bss",),
    ),
    "code_80187F14.o": UnitRules(
        # Data dissolve: func_80189C88 pools a TU-local 0.0f; retail loads
        # shared .sdata2 lbl_eu_80667A08 (per-fn asm refs). The remaining
        # @cM1/@cZero/@z/@m1 locals are unreferenced phantoms.
        pool_patterns=(
            (struct.pack(">I", 0x00000000), "lbl_eu_80667A08"),
        ),
        extern_data_sections=(".sdata2",),
    ),
    "code_8018F8D8.o": UnitRules(
        # Data dissolve: only live constant is the int->float magic double
        # (retail lbl_eu_80667A80); .data/.rodata/.sdata have no kept-section
        # references at all (weak/phantom copies of shared retail data).
        pool_patterns=(
            (struct.pack(">II", 0x43300000, 0x00000000), "lbl_eu_80667A80"),
        ),
        extern_data_sections=(".data", ".rodata", ".sdata", ".sdata2"),
    ),
    "CMenuPTState.o": UnitRules(
        # Data dissolve: ctor/Move float pools; retail refs lbl_eu_80667A98
        # (0.0f) and the magic double lbl_eu_80667AA0. The TU's -1.0f has no
        # unit-referenced blob label (source drift vs retail's 1.0f loads,
        # resolved during function matching); retarget to a value-exact blob
        # label so the link stays live.
        pool_patterns=(
            (struct.pack(">I", 0x00000000), "lbl_eu_80667A98"),
            (struct.pack(">II", 0x43300000, 0x80000000), "lbl_eu_80667AA0"),
        ),
        retarget_relocs=(
            (".text", 0x144, "lbl_eu_80665DB8"),   # -1.0f placeholder
            (".text", 0x160, "lbl_eu_80665DB8"),
            (".text", 0x2FC, "lbl_eu_80665DB8"),
        ),
        extern_data_sections=(".sdata2",),
    ),
    "CPartsChange.o": UnitRules(
        # Data dissolve: two magic doubles; retail pools them twice with
        # identical bytes (lbl_eu_80667AC8 = {magic,0} / AF0+B58 = {magic,-1}).
        # Per-function asm refs split the -1 double: Move manager fns use AF0,
        # CfPartsMoveSrc fns use B58.
        pool_patterns=(
            (struct.pack(">II", 0x43300000, 0x00000000), "lbl_eu_80667AC8"),
            (struct.pack(">II", 0x43300000, 0x80000000), "lbl_eu_80667B58"),
        ),
        retarget_relocs=(
            (".text", 0x1010, "lbl_eu_80667AF0"),
            (".text", 0x1BA4, "lbl_eu_80667AF0"),
        ),
        extern_data_sections=(".data", ".rodata", ".sdata", ".sdata2"),
    ),
    "CtrlMovePC.o": UnitRules(
        # Data dissolve: 0.0f -> lbl_eu_80667B60, int->float magic ->
        # lbl_eu_80667BE8 (common to every referencing retail function).
        pool_patterns=(
            (struct.pack(">I", 0x00000000), "lbl_eu_80667B60"),
            (struct.pack(">II", 0x43300000, 0x80000000), "lbl_eu_80667BE8"),
        ),
        extern_data_sections=(".sdata2",),
    ),
    "CVision.o": UnitRules(
        # Data dissolve: sdata2 magics resolve to lbl_eu_80667D28/D58; the TU
        # also emits a local copy literally named lbl_eu_80667D28 which the
        # strip converts to UNDEF (resolving to split1.s at link).
        pool_patterns=(
            (struct.pack(">II", 0x43300000, 0x80000000), "lbl_eu_80667D28"),
            (struct.pack(">II", 0x43300000, 0x00000000), "lbl_eu_80667D58"),
        ),
        extern_data_sections=(".data", ".rodata", ".sdata", ".sdata2"),
    ),
    "pluginSnd.o": UnitRules(
        # Data dissolve: every VMThread Bgm/Se helper shares one int->float
        # magic double; retail pools it as lbl_eu_80667D98.
        pool_patterns=(
            (struct.pack(">II", 0x43300000, 0x80000000), "lbl_eu_80667D98"),
        ),
        extern_data_sections=(".sdata2",),
    ),
    "CMenuVision.o": UnitRules(
        # Data dissolve: three zero-filled .data jump tables used by the big
        # Move switch (func_801AD504). Retail keeps jumptable_eu_805334A0/D0/
        # 504; slot sizes + first-use order map decomp tables 1:1 (13/13/12).
        retarget_relocs=(
            (".text", 0x342, "jumptable_eu_80533504"),
            (".text", 0x34A, "jumptable_eu_80533504"),
            (".text", 0xFB2, "jumptable_eu_805334D0"),
            (".text", 0xFBA, "jumptable_eu_805334D0"),
            (".text", 0x165A, "jumptable_eu_805334A0"),
            (".text", 0x1662, "jumptable_eu_805334A0"),
        ),
        extern_data_sections=(".data",),
    ),
    "CPcEffect07.o": UnitRules(
        # Data dissolve: single magic double -> lbl_eu_80667DF8.
        pool_patterns=(
            (struct.pack(">II", 0x43300000, 0x80000000), "lbl_eu_80667DF8"),
        ),
        extern_data_sections=(".sdata2",),
    ),
    "CMenuGetItemMulti.o": UnitRules(
        # Data dissolve: sdata2 magics -> lbl_eu_80667E08/E18; six zero .data
        # jump tables across Init/Move/func_801B4830/func_801B69F4 map to
        # jumptable_eu_80533788/7C0/7F8/828/860/898 (slot size + first-use order).
        pool_patterns=(
            (struct.pack(">II", 0x43300000, 0x00000000), "lbl_eu_80667E08"),
            (struct.pack(">II", 0x43300000, 0x80000000), "lbl_eu_80667E18"),
        ),
        retarget_relocs=(
            (".text", 0xC36, "jumptable_eu_805337C0"),
            (".text", 0xD9A, "jumptable_eu_805337C0"),
            (".text", 0x11C6, "jumptable_eu_80533788"),
            (".text", 0x13B2, "jumptable_eu_80533788"),
            (".text", 0x1A02, "jumptable_eu_805337F8"),
            (".text", 0x1A0A, "jumptable_eu_805337F8"),
            (".text", 0x24BE, "jumptable_eu_80533860"),
            (".text", 0x275E, "jumptable_eu_80533860"),
            (".text", 0x2AB6, "jumptable_eu_80533828"),
            (".text", 0x2C86, "jumptable_eu_80533828"),
            (".text", 0x47C6, "jumptable_eu_80533898"),
            (".text", 0x47CE, "jumptable_eu_80533898"),
        ),
        extern_data_sections=(".data", ".sdata2"),
    ),
    "CSuddenCommu.o": UnitRules(
        # Data dissolve: lone magic double has no unit-referenced blob label
        # (retail fn loads none — source drift); value-exact placeholder.
        pool_patterns=(
            (struct.pack(">II", 0x43300000, 0x00000000), "lbl_eu_80668140"),
        ),
        extern_data_sections=(".data", ".rodata", ".sdata", ".sdata2"),
    ),
    "CMenuKizunaTalk.o": UnitRules(
        # Data dissolve: no kept-section references; all data sections are
        # phantom copies of shared retail data.
        extern_data_sections=(".sdata2",),
    ),
    "CMenuItemExchange.o": UnitRules(
        # Data dissolve: Move switch table -> jumptable_eu_80533B98.
        retarget_relocs=(
            (".text", 0x842, "jumptable_eu_80533B98"),
            (".text", 0x84A, "jumptable_eu_80533B98"),
        ),
        extern_data_sections=(".data",),
    ),
    "CfSoundMan.o": UnitRules(
        # Data dissolve: magic double -> lbl_eu_80667E90.
        pool_patterns=(
            (struct.pack(">II", 0x43300000, 0x80000000), "lbl_eu_80667E90"),
        ),
        extern_data_sections=(".sdata2",),
    ),
    "CfPadTask.o": UnitRules(
        # Data dissolve: statics keep their retail names (split1.s defines
        # sMainPadType__Q22cf9CfPadTask etc.); vtables ship from split1.s;
        # 0.0f pool -> lbl_eu_80667EA8.
        pool_patterns=(
            (struct.pack(">I", 0x00000000), "lbl_eu_80667EA8"),
        ),
        exact_renames=(
            ("__vt__Q22cf9CfPadTask", "lbl_eu_80533C90"),
            ("__vt__23CTask<Q22cf9CfPadTask>", "lbl_eu_80533D08"),
        ),
        extern_data_sections=(".rodata", ".data", ".sdata", ".sdata2", ".bss", ".sbss"),
    ),
    "code_801C2C14.o": UnitRules(
        # Data dissolve: weak nw4r snd Fx dtors reference vtables that retail
        # keeps in nw4r_data.s (lbl_eu_8056A798/A7C0/A7E8); rest is phantom.
        exact_renames=(
            ("__vt__Q34nw4r3snd11FxDelayDpl2", "lbl_eu_8056A7C0"),
            ("__vt__Q34nw4r3snd12FxChorusDpl2", "lbl_eu_8056A798"),
            ("__vt__Q34nw4r3snd15FxReverbStdDpl2", "lbl_eu_8056A7E8"),
        ),
        extern_data_sections=(".data", ".rodata", ".sdata"),
    ),
    "CTitleAHelp.o": UnitRules(
        # Data dissolve: string pool ships as lbl_eu_805054BC (content drift
        # vs decomp strings is a source-level residual); 1.0f pool ->
        # lbl_eu_80667F28; magic double sites load lbl_eu_80667F20 in retail.
        pool_patterns=(
            (struct.pack(">I", 0x3F800000), "lbl_eu_80667F28"),
        ),
        exact_renames=(
            ("@stringBase0", "lbl_eu_805054BC"),
        ),
        retarget_relocs=(
            (".text", 0x3F0, "lbl_eu_80667F20"),
            (".text", 0x5E0, "lbl_eu_80667F20"),
        ),
        extern_data_sections=(".data", ".rodata", ".sdata", ".sdata2"),
    ),
    "CCur.o": UnitRules(
        # Data dissolve: 1.0f pool -> lbl_eu_80667FA0.
        pool_patterns=(
            (struct.pack(">I", 0x3F800000), "lbl_eu_80667FA0"),
        ),
        extern_data_sections=(".sdata2",),
    ),
    "CSortMenu.o": UnitRules(
        # Data dissolve: 1.0f pool -> lbl_eu_80668000; the magic0 double site
        # in func_801D353C has no unit-referenced label (value-exact ph).
        pool_patterns=(
            (struct.pack(">I", 0x3F800000), "lbl_eu_80668000"),
        ),
        retarget_relocs=(
            (".text", 0x7B0, "lbl_eu_80668140"),
        ),
        extern_data_sections=(".sdata2",),
    ),
    "CItemBoxInfo.o": UnitRules(
        # Data dissolve: dense sdata2 pool all confirmed by per-fn refs
        # (func_801D8E34/func_801E43BC); INT_MIN literal has no blob label
        # (retail fns load lbl_eu_80668010 there); one jump table.
        pool_patterns=(
            (struct.pack(">II", 0x43300000, 0x00000000), "lbl_eu_80668020"),
            (struct.pack(">II", 0x43300000, 0x80000000), "lbl_eu_80668028"),
            (struct.pack(">I", 0x3C23D70A), "lbl_eu_80668040"),   # 0.01f
            (struct.pack(">I", 0x42C80000), "lbl_eu_80668044"),   # 100.0f
            (struct.pack(">I", 0x3F000000), "lbl_eu_80668048"),   # 0.5f
        ),
        retarget_relocs=(
            (".text", 0x5B8, "lbl_eu_80668010"),
            (".text", 0xD418, "lbl_eu_80668010"),
            (".text", 0xD4E8, "lbl_eu_80668010"),
            (".text", 0x1E32, "jumptable_eu_80534A68"),
            (".text", 0x1E3A, "jumptable_eu_80534A68"),
        ),
        extern_data_sections=(".data", ".sdata2"),
    ),
    "CNumSelect.o": UnitRules(
        # Data dissolve: magic0 double -> lbl_eu_80668090.
        pool_patterns=(
            (struct.pack(">II", 0x43300000, 0x00000000), "lbl_eu_80668090"),
        ),
        extern_data_sections=(".sdata2",),
    ),
    "CItemBoxLine.o": UnitRules(
        # Data dissolve: magic0 -> lbl_eu_80668108; magic-1 sites load
        # lbl_eu_80668100 in retail; three .data switch tables.
        pool_patterns=(
            (struct.pack(">II", 0x43300000, 0x00000000), "lbl_eu_80668108"),
        ),
        retarget_relocs=(
            (".text", 0x47FC, "lbl_eu_80668100"),
            (".text", 0x4844, "lbl_eu_80668100"),
            (".text", 0x65EC, "lbl_eu_80668100"),
            (".text", 0x4FC6, "jumptable_eu_80534CC0"),
            (".text", 0x4FCE, "jumptable_eu_80534CC0"),
            (".text", 0x10BE, "jumptable_eu_80534C58"),
            (".text", 0x10C6, "jumptable_eu_80534C58"),
            (".text", 0x37F6, "jumptable_eu_80534C88"),
            (".text", 0x37FE, "jumptable_eu_80534C88"),
        ),
        extern_data_sections=(".data", ".sdata2"),
    ),
    "CScrollBar.o": UnitRules(
        # Data dissolve: both magic-double sites load lbl_eu_80668140/-48 in
        # the corresponding retail functions.
        retarget_relocs=(
            (".text", 0x80, "lbl_eu_80668140"),
            (".text", 0x600, "lbl_eu_80668140"),
            (".text", 0x674, "lbl_eu_80668148"),
        ),
        extern_data_sections=(".sdata2",),
    ),
    "CMenuBattleMode.o": UnitRules(
        # Data dissolve: no kept-section references; all data sections are
        # phantom copies of shared retail data.
        extern_data_sections=(".data", ".rodata", ".sdata", ".sdata2"),
    ),
    "CfCam.o": UnitRules(
        # Data dissolve: camera-follow statics/vtables ship from split1.s;
        # both anon double slots resolve to lbl_eu_806662A8 (site
        # correspondence; retail loads the same magic double at both pools').
        # @7259 uses an exact rename because pool_patterns cannot assign the
        # same target twice; re-check numbering after TU growth.
        exact_renames=(
            ("@7259", "lbl_eu_806662A8"),
        ),
        pool_patterns=(
            (struct.pack(">II", MAGIC_HI, MAGIC_LO), "lbl_eu_806662A8"),
        ),
        extern_data_sections=(".sdata2", ".bss", ".sbss"),
    ),
    "CMenuEnemyState.o": UnitRules(),
    "CUIBattleManager.o": UnitRules(
        # functions.hpp declares C++-linkage bool; retail reloc is unmangled.
        exact_renames=(
            ("func_8009CF8C__Fi", "func_8009CF8C"),
        ),
        # Data dissolve: retail split carries no data — vtables/RTTI
        # locators/typeinfo-name strings/ptmf pool all ship from split1.s
        # (lbl_eu_8052E0A8..E248); the ctors already store vptrs from the blob
        # labels directly and no reloc references the local copies. Strip.
        extern_data_sections=(".data", ".rodata", ".sdata", ".bss", ".sbss"),
    ),
    "CScnItemPool.o": UnitRules(
        # monolibdata2 dissolve: the TU hand-spells the CScnItemPool vtable
        # (+CDeviceVICb sub-vtable group), RTTI base-list block and class-name
        # string (CLibCriMoviePlay pattern). The weak reslist<CScnItem*>
        # template tail keeps MWCC's order ([__vt__reslist][anon base-list]
        # [__vt___reslist_base]) which matches retail; retarget the -RTTI-on
        # typeinfo head relocs to the retail .sdata locator names (the structs
        # themselves live in monolibdata2, so the local .sdata is stripped),
        # rename the two template vtables, label the anonymous base-list block
        # at +0xC4, and pad the section tails to the retail sizes (splitter
        # align pads: .data 0xDC->0xE0, .rodata 0x3E->0x40).
        retarget_relocs=(
            (".data", 0xB8, "lbl_eu_80663940"),  # __vt__19reslist head
            (".data", 0xC4, "lbl_eu_80663948"),  # anon reslist base-list head
            (".data", 0xD0, "lbl_eu_80663948"),  # __vt__25_reslist_base head
        ),
        exact_renames=(
            ("__vt__19reslist<P8CScnItem>", "lbl_eu_8056E540"),
            ("__vt__25_reslist_base<P8CScnItem>", "lbl_eu_8056E558"),
            # standalone reslist<CScnItem*> default-ctor: retail label
            # __ct__reslist_CScnItem (registry target us-8048ffb8)
            ("__ct__19reslist<P8CScnItem>Fv", "__ct__reslist_CScnItem"),
        ),
        add_symbols=(("lbl_eu_8056E54C", ".data", 0xC4, 0xC),),
        data_pool_patterns=(
            (".rodata", b"reslist<CScnItem *>\\x00", "lbl_eu_80523F28"),
            (".rodata", b"_reslist_base<CScnItem *>\\x00", "lbl_eu_80523F3C"),
        ),
        pad_data_section=((".data", 0xE0), (".rodata", 0x40)),
        extern_data_sections=(".sdata",),
    ),
    "math_types.o": UnitRules(
        # pool-coupled: local .sdata2 (0.0f, 1.0f, 0.02454369f) -> nw4r shared
        # pool nw4r_data.s slots lbl_eu_80669E40/E44/E48 (retail math_types.s
        # lfs sites); retail keeps the TU .sdata2 EMPTY.
        pool_patterns=(
            (struct.pack(">I", 0x00000000), "lbl_eu_80669E40"),
            (struct.pack(">I", 0x3F800000), "lbl_eu_80669E44"),
            (struct.pack(">I", 0x3CC90FDB), "lbl_eu_80669E48"),
        ),
        extern_data_sections=(".sdata2",),
    ),
    "math_geometry.o": UnitRules(
        # pool-coupled: local .sdata2 (0.5f, 0.0f, 0.02454369f, 0.7111111f,
        # 1.0f) -> nw4r shared pool nw4r_data.s slots
        # lbl_eu_80669E58/E5C/E60/E64/E68 (retail math_geometry.s lfs sites);
        # retail keeps the TU .sdata2 EMPTY. The .sdata 806634D0 (0.0001f)
        # refs are already extern in source.
        pool_patterns=(
            (struct.pack(">I", 0x3F000000), "lbl_eu_80669E58"),
            (struct.pack(">I", 0x00000000), "lbl_eu_80669E5C"),
            (struct.pack(">I", 0x3CC90FDB), "lbl_eu_80669E60"),
            (struct.pack(">I", 0x3F360B61), "lbl_eu_80669E64"),
            (struct.pack(">I", 0x3F800000), "lbl_eu_80669E68"),
        ),
        extern_data_sections=(".sdata2",),
    ),
    "btu_hcif.o": UnitRules(
        # retail .data carries one trailing pad byte (0x8E vs MWCC 0x8D).
        pad_data_section=((".data", 0x8E),),
        # Pool-order anchor (see btu_hcif.c): the char*const table MWCC emits
        # into .sdata2 is a retail linker GC'd orphan - strip it so both
        # .sdata2 sides are empty. Also align the four .data string names with
        # the retail split's local labels (§17.6 reloc naming).
        extern_data_sections=(".sdata2",),
        exact_renames=(
            ("BTU_str_hw_error", "@2212"),
            ("BTU_str_event_mismatch", "@2352"),
            ("BTU_str_cmd_timeout", "@2368"),
            ("BTU_str_hci_timeout", "@2369"),
        ),
    ),
    "hcicmds.o": UnitRules(
        # MWCC emits only the 10-byte inquiry filter constant
        # (ff ff ff ff ff ff ff 7f); the retail split rounds the .rodata tail
        # up to 0x10 with zero padding.
        pad_data_section=((".rodata", 0x10),),
    ),
    "dvdFatal.o": UnitRules(
        # The 12 error-message strings in .data are byte-identical on both
        # sides; MWCC numbers the anonymous pools @1863..@1874 while the
        # retail split uses @768..@779. Pure name drift - rename.
        exact_renames=(
            ("@1863", "@768"), ("@1864", "@769"), ("@1865", "@770"),
            ("@1866", "@771"), ("@1867", "@772"), ("@1868", "@773"),
            ("@1869", "@774"), ("@1870", "@775"), ("@1871", "@776"),
            ("@1872", "@777"), ("@1873", "@778"), ("@1874", "@779"),
        ),
    ),
    "encutility.o": UnitRules(
        # __ENCVersion points at the same 70-byte version string in .data on
        # both sides; MWCC numbers it @393, retail @366. Name drift only.
        exact_renames=(("@393", "@366"),),
    ),
    "dvdDeviceError.o": UnitRules(
        # The 7 language-message pointers in .rodata and the strings in .data
        # are byte-identical; MWCC numbers the anonymous pools @1930..@1936,
        # retail @835..@841. Name drift only. .sdata2: MWCC emits 4 extra
        # zero bytes after ffffff00.
        exact_renames=(
            ("@1930", "@835"), ("@1931", "@836"), ("@1932", "@837"),
            ("@1933", "@838"), ("@1934", "@839"), ("@1935", "@840"),
            ("@1936", "@841"),
        ),
        drop_data_tail=((".sdata2", 0x4),),
    ),
    "fs.o": UnitRules(
        # MWCC zero-pads .data after the IPC-arena error string (0x28);
        # retail ends at 0x21.
        drop_data_tail=((".data", 0x21),),
    ),
    "gap_utils.o": UnitRules(
        # MWCC pads .bss to 8 (0x3B0); retail ends at 0x3AC.
        drop_nobits_range=((".bss", 0x3AC, 0x3B0),),
    ),
    "hidh_api.o": UnitRules(
        # MWCC 4-pads .data (0x118 vs retail 0x113) and .bss (0x408 vs 0x404).
        drop_data_tail=((".data", 0x113),),
        drop_nobits_range=((".bss", 0x404, 0x408),),
    ),
    "l2c_api.o": UnitRules(
        # MWCC pads .data to 8 (0x790); retail split ends at 0x78B.
        drop_data_tail=((".data", 0x78B),),
    ),
    "l2c_csm.o": UnitRules(
        # MWCC pads .data to 4 (0x838); retail ends at 0x837.
        drop_data_tail=((".data", 0x837),),
    ),
    "l2c_utils.o": UnitRules(
        # MWCC pads .data to 4 (0xE8 vs 0xE7) and .sdata to 8 (0x10 vs 0xE).
        drop_data_tail=((".data", 0xE7), (".sdata", 0xE),),
    ),
    "rfc_mx_fsm.o": UnitRules(
        # MWCC pads .data to 4 (0x2B8); retail ends at 0x2B6. One jumptable
        # case-label addend drifts (rfc_mx_sm_state_disc_wait_ua dispatch at
        # +0x270: retail 300 vs decomp 296) - pin the retail value.
        drop_data_tail=((".data", 0x2B6),),
        addend_sets=((".data", 624, 300),),
    ),
    "rfc_port_fsm.o": UnitRules(
        # MWCC pads .data to 8 (0x328); retail ends at 0x326. One jumptable
        # case-label addend drifts (rfc_port_sm_orig_wait_sec_check dispatch
        # at +0x228: retail 192 vs decomp 272) - pin the retail value.
        drop_data_tail=((".data", 0x326),),
        addend_sets=((".data", 552, 192), (".data", 556, 272),),
    ),
    "rfc_ts_frames.o": UnitRules(
        # MWCC pads .sdata to 8 (0x10); retail ends at 0xF.
        drop_data_tail=((".sdata", 0xF),),
    ),
    "sdp_server.o": UnitRules(
        # (strings now declared in retail order in source - no rule needed;
        # kept as anchor in case MWCC re-pads).
    ),
    "btm_acl.o": UnitRules(
        # dead 8-byte zero pool; retail .sdata2 empty.
        drop_data_tail=((".sdata2", 0x0),),
    ),
    "bta_hh_utils.o": UnitRules(
        # dead 32-byte zero .rodata blob; retail .rodata empty.
        drop_data_tail=((".rodata", 0x0),),
    ),
    "btm_pm.o": UnitRules(
        # MWCC pads .data to 4 (0x40); retail split ends at 0x3D.
        drop_data_tail=((".data", 0x3D),),
    ),
    "bta_sys_main.o": UnitRules(
        # MWCC pads .data to 4 (0x38); retail split ends at 0x35.
        drop_data_tail=((".data", 0x35),),
    ),
    "bta_dm_act.o": UnitRules(
        # MWCC pads .data to 4 (0x140 vs retail 0x13B) and .bss to 4
        # (0x30 vs retail 0x2D); drop both tails.
        drop_data_tail=((".data", 0x13B),),
        drop_nobits_range=((".bss", 0x2D, 0x30),),
    ),
    "btu_init.o": UnitRules(
        # MWCC emits three -1 f32 words (0x6); retail keeps the same three
        # plus a zero pad word (0x8).
        pad_sdata2_size=8,
    ),
    "btm_sec.o": UnitRules(
        # String-pool pointer orphans (btm_sec.c): the retail linker GC'd the
        # BTM_SetSecurityMode/BTM_SetEncryption char*const anchor tables (the
        # .data strings themselves survive in this unit's split) - strip the
        # MWCC-emitted .rodata copies. .sdata2: six 0xFF bytes plus a retail
        # zero-pad word (same shape as btu_init).
        extern_data_sections=(".rodata",),
        pad_sdata2_size=8,
    ),
    "port_rfc.o": UnitRules(
        # dead 8-byte zero pool; retail .sdata2 empty.
        drop_data_tail=((".sdata2", 0x0),),
    ),
    "ai.o": UnitRules(
        # RVL SDK version-string dissolve: the AI banner literal is anon @N in
        # both objects; __AIVersion's .sdata reloc must carry the retail name.
        exact_renames=(("@395", "@364"),),
    ),
    "AX.o": UnitRules(
        # RVL SDK version-string dissolve (same pattern as ai.o): the banner
        # literal is anon @339 in retail; MWCC spells it s_AXVersionStr.
        # __AXVersion's .sdata reloc must carry the retail name.
        exact_renames=(("s_AXVersionStr", "@339"),),
    ),
    "AXVPB.o": UnitRules(
        # Literal-pool first-use drift: MWCC pools 6.2831855,32000 before the
        # 2.0 that retail leads with (all f32s byte-identical, code FULL_MATCH).
        # Permute to retail order [2, 2pi, 32000, 1, 32768] (+ pad/double tail);
        # symbols follow values so text loads stay correct (KPAD pattern).
        permute_sdata2_words=(
            2, 0, 1, 3, 4, 5, 6, 7,
        ),
    ),
    "AXFXDelayExp.o": UnitRules(
        # Retail keeps an 8-aligned zero pad word after the last literal
        # (0.95f); MWCC stops at it (0x14 vs 0x18).
        pad_sdata2_size=0x18,
    ),
    "AXCL.o": UnitRules(
        # retail .bss slice (__AXCommandList) is 32-byte aligned; MWCC emits 8.
        set_data_align=((".bss", 32),),
    ),
    "mtx.o": UnitRules(
        # MWCC pools 0.0f before 1.0f (first-use order); retail .sdata2 leads
        # with 1.0f. Swap the leading pair (and their st_values).
        swap_sdata2_leading_f32_words=(0x00000000, 0x3F800000),
    ),
    "KPAD.o": UnitRules(
        # Version-string pointer: same .data address both sides, MWCC names it
        # kpad_version_str, retail lbl_80550490.
        exact_renames=(("kpad_version_str", "lbl_80550490"),),
        # MWCC pools duplicate copies of 0.99902344/0.001953125/0.74902344
        # (each use site gets its own literal); retail keeps one of each.
        # Drop the three adjacent duplicates at words 14/15/16, then permute
        # retail order (decomp order follows its unmatched text; symbols move
        # with their values so text loads stay correct).
        drop_data_range=((".sdata2", 0x38, 0x44),),
        permute_sdata2_words=(
            4, 10, 0, 1, 2, 3, 6, 7, 8, 9, 13, 12, 11, 14,
            15, 16, 17, 18, 19, 20, 5, 21, 22, 23, 24, 25, 26, 27,
        ),
    ),
    "DSPCode.o": UnitRules(
        # retail axDspSlave .data slice is 32-byte aligned; MWCC emits 8.
        set_data_align=((".data", 32),),
    ),
    "AXFXReverbHiExp.o": UnitRules(
        # Init-pool leading pair: MWCC still emits 0.0f before 32000.0f for
        # the exp-local pow constants; retail starts at 32000.0f.
        swap_sdata2_leading_f32_pair=True,
    ),
    "AXFXChorusExp.o": UnitRules(
        # InitParams trailing f32 window: MWCC emits 256,32000,0.00390625,
        # 65536; retail pools 65536,256,32000,0.00390625 (rotate-right).
        # Three swaps realize the 4-cycle: (p0 p1)(p0 p2)(p0 p3).
        swap_data_blocks=(
            (".sdata2", 0x20, 0x24, 4),
            (".sdata2", 0x20, 0x28, 4),
            (".sdata2", 0x20, 0x2C, 4),
        ),
    ),
    "AXFXChorusExpDpl2.o": UnitRules(
        # same trailing-window rotation as AXFXChorusExp.
        swap_data_blocks=(
            (".sdata2", 0x20, 0x24, 4),
            (".sdata2", 0x20, 0x28, 4),
            (".sdata2", 0x20, 0x2C, 4),
        ),
    ),
    "gki_buffer.o": UnitRules(
        # MWCC pads the string pool to 8 (0x148); retail split ends at 0x142.
        drop_data_tail=((".data", 0x142),),
    ),
    "uusb_ppc.o": UnitRules(
        # retail .bss slice (usb + fiber stacks) is 32-byte aligned; MWCC 8.
        set_data_align=((".bss", 32),),
    ),
    "btm_devctl.o": UnitRules(
        # MWCC pads .sdata to 8 (0x18); retail split ends at 0x12 (after the
        # FALSE string). The missing WriteVoiceSettings/EnableTestMode log
        # strings are supplied by source (dead retail pool entries).
        drop_data_tail=((".sdata", 0x12),),
    ),
    "dsp_task.o": UnitRules(
        # MWCC pads .data to 8 (0x140); retail split ends at 0x13B.
        drop_data_tail=((".data", 0x13B),),
    ),
    "OSContext.o": UnitRules(
        # MWCC pads .data to 8 (0x1E0); retail split ends at 0x1DB.
        drop_data_tail=((".data", 0x1DB),),
    ),
    "dsp.o": UnitRules(
        # MWCC pads .data to 8 (0x80); retail split ends at 0x7D. The .sdata
        # SDA21 pool slot is the same constant under drifted @N numbering.
        drop_data_tail=((".data", 0x7D),),
        exact_renames=(("@386", "@355"),),
    ),
    "GXInit.o": UnitRules(
        # MWCC keeps the GX version string as a local static; retail names it
        # lbl_8054B680 in the shared data blob.
        exact_renames=(("s_GXVersionStr", "lbl_8054B680"),),
    ),
    "dvd.o": UnitRules(
        # SDA21 pool slot under drifted @N numbering.
        exact_renames=(("@592", "@533"),),
    ),
    "NANDCore.o": UnitRules(
        # MWCC keeps the NAND version string as __NANDVersion; retail names
        # it lbl_80551080 in the shared data blob.
        exact_renames=(("__NANDVersion", "lbl_80551080"),),
    ),
    "EXIBios.o": UnitRules(
        # SDA21 pool slot under drifted @N numbering.
        exact_renames=(("@507", "lbl_8054B610"),),
    ),
    "dvdfs.o": UnitRules(
        # MWCC pads .sbss to 8 (0x38); retail ends at 0x20.
        drop_nobits_range=((".sbss", 0x20, 0x38),),
    ),
    "OSError.o": UnitRules(
        # MWCC pads .data to 8 (0x2E0 vs retail 0x2D9) and .bss (0x50 vs 0x44).
        drop_data_tail=((".data", 0x2D9),),
        drop_nobits_range=((".bss", 0x44, 0x50),),
    ),
    "OSFont.o": UnitRules(
        # MWCC pads .data to 8 (0xB10); retail split ends at 0xB0A.
        drop_data_tail=((".data", 0xB0A),),
    ),
    "OSRtc.o": UnitRules(
        # MWCC pads .bss to 8 (0x58); retail ends at 0x54.
        drop_nobits_range=((".bss", 0x54, 0x58),),
    ),
    "OSStateTM.o": UnitRules(
        # MWCC pads .data to 8 (0xC8); retail ends at 0xC6.
        drop_data_tail=((".data", 0xC6),),
    ),
    "OSNandbootInfo.o": UnitRules(
        # MWCC pads .data to 8 (0x20); retail split ends at 0x1A.
        drop_data_tail=((".data", 0x1A),),
    ),
    "scapi_prdinfo.o": UnitRules(
        # MWCC pads .sdata to 8 (0x10); retail ends at 0xD.
        drop_data_tail=((".sdata", 0xD),),
    ),
    "SIBios.o": UnitRules(
        # SDA21 pool slot under drifted @N numbering.
        exact_renames=(("@1024", "lbl_8055F138"),),
    ),
    "usb.o": UnitRules(
        # MWCC pads .data to 8 (0x830 vs retail 0x82F) and .sbss (0x10 vs 9).
        drop_data_tail=((".data", 0x82F),),
        drop_nobits_range=((".sbss", 0x9, 0x10),),
    ),
    "CScnEnvLgtCtrl.o": UnitRules(
        # pool-coupled: lone unsigned int->double magic (2^52) -> CGXCache
        # lbl_eu_8066A3C0; retail keeps TU .sdata2 EMPTY. CScnBloom pattern.
        pool_patterns=(
            (struct.pack(">II", 0x43300000, 0x00000000), "lbl_eu_8066A3C0"),
        ),
        extern_data_sections=(".sdata2",),
    ),
    "CTaskEnvironment.o": UnitRules(
        # unsigned int->double magic -> split1 lbl_eu_80665FF0 via SDA21;
        # drop the local two-magic pool after the rename.
        exact_renames=(("@3905", "lbl_eu_80665FF0"),),
        drop_data_tail=((".sdata2", 0x0),),
    ),
    "pluginUi.o": UnitRules(
        # lone signed int->double magic; retail keeps TU .sdata2 empty and
        # loads lbl_eu_80665DC0 (split1 sdata2) via SDA21 - rename the @N
        # pool symbol and drop the local copy.
        exact_renames=(("@2082", "lbl_eu_80665DC0"),),
        drop_data_tail=((".sdata2", 0x0),),
    ),
    "bte_logmsg.o": UnitRules(
        # MWCC pads the "%s\\n" sdata string to 8 (retail keeps 4) and spills
        # a 2-byte @LOCAL@LogMsg__FUlPCce@tmp into .bss at +0x7D0 (retail ends
        # there); drop both tails.
        drop_data_tail=((".sdata", 4),),
        drop_nobits_range=((".bss", 0x7D0, 0x7E0),),
    ),
    "bte_main.o": UnitRules(
        # Retail .bss is 32-aligned (BT stack control blocks); MWCC emits align 8.
        set_data_align=((".bss", 32),),
    ),
    "CERand.o": UnitRules(
        # pool-coupled: local .sdata2 int->double conversion magics
        # (unsigned 2^52 + signed 43300000_80000000) -> CGXCache shared pool
        # lbl_eu_8066A3C0 / lbl_eu_8066A388; retail keeps TU .sdata2 EMPTY.
        # CScnBloom.o / CScnItemCamera.o pattern.
        pool_patterns=(
            (struct.pack(">II", 0x43300000, 0x00000000), "lbl_eu_8066A3C0"),
            (struct.pack(">II", 0x43300000, 0x80000000), "lbl_eu_8066A388"),
        ),
        # The TU hand-builds the retail vtable/RTTI layout (lbl_eu_80524658
        # strings, lbl_eu_8056FE08/20/30/48 data, lbl_eu_80663BB0/BB8/BC0
        # locators) FIRST in each section; MWCC's own auto __vt__/__RTTI__/
        # class-name copies (kept alive so the out-of-line inline virtuals are
        # emitted here, like retail) trail it and are tail-dropped.
        drop_data_tail=(
            (".rodata", 0x30),
            (".data", 0x50),
            (".sdata", 0x18),
        ),
        # The Simple ctor stores the auto __vt__18CERandomizerSimple; point
        # those two lis/addi relocs at the hand-built retail-named vtable.
        retarget_relocs=(
            (".text", 0x2, "lbl_eu_8056FE08"),
            (".text", 0xA, "lbl_eu_8056FE08"),
        ),
        extern_data_sections=(".sdata2",),
    ),
    "code_804CC2B8.o": UnitRules(
        # pool-coupled: lone unsigned int->double magic constant
        # (43300000_00000000) MWCC materializes in .sdata2 for a f64
        # conversion; retail keeps the TU .sdata2 EMPTY and loads the same 8
        # bytes from the CGXCache pool via lfd against lbl_eu_8066A3C0.
        # CScnBloom.o / CVirtualLightObj.o pattern.
        pool_patterns=(
            (struct.pack(">II", 0x43300000, 0x00000000), "lbl_eu_8066A3C0"),
            # s16->f32 signed-conversion magic (func_804CE79C texgen pair
            # scaling); retail loads the shared monolibdata2/CGXCache pool
            # entry lbl_eu_8066B0F0 by name.
            (struct.pack(">II", 0x43300000, 0x80000000), "lbl_eu_8066B0F0"),
        ),
        extern_data_sections=(".sdata2",),
    ),
    "code_804BC9EC.o": UnitRules(
        # Jumptable addends: func_804BC9F4__FPvUl is a documented §17.6 code
        # residual whose dense 20-entry switch jumptable lives in this TU's
        # .data. The case-label offsets drift with the ambient -ipa codegen
        # variant (observed first-word variants: 544 / 388 / 440), so relative
        # deltas cannot survive a rebuild - pin ABSOLUTE retail addends.
        # Retail values: 492,168,308,492,492,208,228,248,268,492,396,328,
        # 356,376,492,492,288,424,448,468 (+0x14 = 208 needs no write).
        addend_sets=(
            (".data", 0x00, 492),
            (".data", 0x04, 168),
            (".data", 0x08, 308),
            (".data", 0x0C, 492),
            (".data", 0x10, 492),
            (".data", 0x18, 228),
            (".data", 0x1C, 248),
            (".data", 0x20, 268),
            (".data", 0x24, 492),
            (".data", 0x28, 396),
            (".data", 0x2C, 328),
            (".data", 0x30, 356),
            (".data", 0x34, 376),
            (".data", 0x38, 492),
            (".data", 0x3C, 492),
            (".data", 0x40, 288),
            (".data", 0x44, 424),
            (".data", 0x48, 448),
            (".data", 0x4C, 468),
        ),
        # Retail split tails: .data carries a 6-byte splitter align pad
        # (0xC2 -> 0xC8); the lbl_eu_8065F32C bss block is 4-aligned in
        # retail while MWCC emits align 8.
        pad_data_section=((".data", 0xC8),),
        set_data_align=((".bss", 4),),
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
        # Post-2025-08-25 rework state (data_vtables.hpp dropped; template
        # typeinfo emitted by MWCC itself). Raw layout vs retail:
        # .data  [vtable+B6B0][jumptable@EC(0x20)][rlvt@10C][rlb-blk@118]
        #        [rbvt@124][mpvt@130]
        # retail [vtable+B6B0][mpvt@EC][rlvt@F8][rlb-blk@104][rbvt@110] (0x120)
        #   -> swap M into R's slot, then R into NB's slot (symbols ride with
        #      their blocks so the lbl_eu_8056B6CC/D8/F0 renames land on the
        #      right offsets), drop the jumptable, pad to 0x120.
        # .sdata raw [A2D8][B6B0][22624][_][reslist-nm][B6E4][rlb-nm][_][CMsg-nm]
        # retail [A2D8][B6B0][CMsg-nm][_][reslist-nm][B6E4][rlb-nm][22624](0x24)
        #   -> one word swap + retargets onto the retail name strings.
        # .rodata raw [CF][SJIS][RL][RB][MP] -> retail [MP][RL][RB][CF][SJIS]:
        #   block swaps home each string (pure bytes, no relocs), zero the
        #   leftover junk, tail-trim to the retail 0x70.
        # NOTE: several of these transforms are not idempotent -- hexdiff
        # re-applies rules to build/us/src on every invocation. Touch this
        # unit's cpp / force a fresh build before evaluating the gate.
        swap_data_blocks=(
            (".sdata", 0x08, 0x20, 0x4),
            (".data", 0x10C, 0x130, 0xC),   # rlvt <-> mpvt
            (".data", 0x118, 0x130, 0xC),   # r <-> rlb-blk
            (".data", 0x124, 0x130, 0xC),   # rbvt <-> rlb-blk
            # .rodata: SJIS home first (frees RL's slot), then RL, RB head
            # (mid already lands at 0x30), MP, CF, and the SJIS tail-word fix.
            (".rodata", 0x10, 0x50, 0x14),
            (".rodata", 0x10, 0x30, 0x16),
            (".rodata", 0x28, 0x48, 0x08),
            (".rodata", 0x00, 0x64, 0x0E),
            (".rodata", 0x44, 0x64, 0x0B),
            (".rodata", 0x26, 0x66, 0x02),
        ),
        retarget_relocs=(
            # PRE-swap offsets: retargets run before swap_data_blocks. The
            # CMsgParam name pool word sits at 0x20 raw and swaps into 0x08.
            (".sdata", 0x20, "lbl_eu_805225E0"),   # CMsgParam<10> name
            (".sdata", 0x10, "lbl_eu_805225F0"),   # reslist<IWorkEvent*> name
            (".sdata", 0x14, "lbl_eu_8056B6E4"),   # reslist cast-base struct (blob .data)
            (".sdata", 0x18, "lbl_eu_80522608"),   # _reslist_base<IWorkEvent*> name
            # updateMsg switch jumptable lives OUTSIDE this unit's split, in
            # the blob range 0x8056B5C0 (dissolved into CTaskManager.o's
            # lbl_eu_8056B55C[33] tail); point the lis/addi table base at the
            # retail symbol instead of MWCC's TU-local @NNN table.
            (".text", 0xC86, "jumptable_eu_8056B5C0"),
            (".text", 0xC8E, "jumptable_eu_8056B5C0"),
        ),
        exact_renames=(
            ("__RTTI__13CMsgParam<10>", "lbl_eu_80663580"),
            ("__RTTI__22reslist<P10IWorkEvent>", "lbl_eu_80663588"),
            ("__RTTI__28_reslist_base<P10IWorkEvent>", "lbl_eu_80663590"),
            # Template vtables land at the retail offsets via the swaps above;
            # rename the defined symbols onto the retail in-split labels.
            ("__vt__13CMsgParam<10>", "lbl_eu_8056B6CC"),
            ("__vt__22reslist<P10IWorkEvent>", "lbl_eu_8056B6D8"),
            ("__vt__28_reslist_base<P10IWorkEvent>", "lbl_eu_8056B6F0"),
            ("__vt__5CView", "lbl_eu_8056B5E0"),
            ("__ct__10CFontLayerFv", "__ct__CFontLayer"),
        ),
        # updateMsg switch jumptable (addend relocs into updateMsg): retail
        # keeps that table in the blob (see .text retargets above).
        drop_data_range=((".data", 0xEC, 0x10C),),
        pad_data_section=((".data", 0x120),),  # retail .data range 0x8056B5E0-0x8056B700
        set_data_align=((".sdata", 8),),
        drop_data_tail=(
            (".rodata", 0x70),  # pooled string copies exceed retail 0x805225E0-0x80522650
        ),
        pool_patterns=(
            (struct.pack(">I", 0x3F800000), "lbl_eu_8066A2D0"),  # 1.0f
            (struct.pack(">I", 0x3F19999A), "lbl_eu_8066A2D4"),  # 0.6f
        ),
        # Constructor: the high-level POD list initialization reaches exact
        # scheduling and size; only MWCC's three-way color choice for the two
        # vtable pointers and second sentinel differs. PLAN.md Â§17.6.
        #
        # attachRenderWork: typed CMsgParam<10> enqueue reaches exact frame,
        # stmw r21, size 0x1E0, and stwux stores (~85% fuzzy). Remaining gap is
        # the dual-inline snap-load schedule/Chaitin permutation vs retail.
        # Semantics covered by behaviour:view-attach-render-work. PLAN.md Â§17.6.
        #
        # setCurrent: CMsgParam<10>::enqueue(6) reaches exact -0x40 / 0xBC /
        # stwux (~78% fuzzy). Remaining gap is spill/load interleave vs retail
        # caller-stack snap homes. Semantics: behaviour:view-set-current-ring.
        # PLAN.md Â§17.6.

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
        # wkStandby jumptable addends: the switch case labels sit 4-8B
        # deeper in retail (wkStandby body is a documented §17.6 code
        # residual, 13.2%); the .data dispatch entries at +0x00/+0x20..+0x2C
        # must carry the retail offsets for the DOL to byte-match.
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
        ),
        # The RTTI name strings / typeinfo pool entries the .sdata RTTI
        # structs point at, renamed content-based (their @N numbering drifts
        # with every source change); retail references the shared labels in
        # monolibdata1 (0x80522474/84/9C = "CMsgParam<8>" /
        # "reslist<CWorkThread *>" / "_reslist_base<CWorkThread *>",
        # 0x8056B1C8 = the 0xC reslist typeinfo struct {ptr, 0, 0}).
        data_pool_patterns=(
            (".rodata", b"CMsgParam<8>\x00", "lbl_eu_80522474"),
            (".rodata", b"reslist<CWorkThread *>\x00", "lbl_eu_80522484"),
            (".rodata", b"_reslist_base<CWorkThread *>\x00", "lbl_eu_8052249C"),
            (
                ".data",
                struct.pack(">III", 0, 0, 0),
                "lbl_eu_8056B1C8",
            ),  # reloc words are 0 in-file; sole referenced @ in .data
        ),
        # MWCC 8-aligns the first auto-emitted RTTI struct after the 4-byte
        # lbl_eu_8066351C sentinel, leaving a 4-byte pad; the retail linker
        # packs the structs at +0x4/+0xC/+0x14. Drop the pad (and the same
        # 4-byte pad in .sbss before lbl_eu_80665598) so sizes/offsets match
        # the retail split, and write the splitter's align=4 convention.
        drop_data_range=((".sdata", 0x4, 0x8),),
        drop_nobits_range=((".sbss", 0x4, 0x8),),
        pad_data_section=((".rodata", 0x4C),),
        set_data_align=((".rodata", 4), (".sdata", 4), (".sbss", 4)),
    ),
    "CLibCri.o": UnitRules(
        # monolibdata2 dissolve: (1) the dissolved vtable blob references MWCC's
        # multiple-inheritance this-adjust thunks (@452@/@456@ prefixed —
        # unspellable in C++); the source defines the four 2-insn thunks under
        # placeholder names. (2) The ScnGroup RTTI chain slots reference
        # __RTTI__10IWorkEvent / __RTTI__11CWorkThread, which cannot be spelled
        # in a -RTTI-on TU whose include chain defines those classes (MWCC
        # 10322) — stand-in names rtti_* per the CLibLayout.o recipe.
        exact_renames=(
            ("thunk452_viBeginFrame", "@452@viBeginFrame__7CLibCriFv"),
            ("thunk452_dt", "@452@__dt__7CLibCriFv"),
            ("thunk456_errorWiiCB", "@456@errorWiiCB__7CLibCriFv"),
            ("thunk456_dt", "@456@__dt__7CLibCriFv"),
            ("rtti_10IWorkEvent", "__RTTI__10IWorkEvent"),
            ("rtti_11CWorkThread", "__RTTI__11CWorkThread"),
        ),
    ),
    "CLib.o": UnitRules(
        # monolibdata2 dissolve: the TU defines the retail vtable
        # (lbl_eu_8056CDA0, class is __declspec(novtable)), the RTTI base-list
        # array (lbl_eu_8056CE40) and the .sdata typeinfo pair (lbl_eu_80663788)
        # in source. The string literals MWCC pools into local .rodata ship
        # from the CException.cpp pool lbl_eu_80522F88 — rename then strip so
        # the relocs resolve to the shared pool at link.
        exact_renames=(("@stringBase0", "lbl_eu_80522F88"),),
        extern_data_sections=(".rodata",),
        # Retail linker-GC'd both standalone copies: the ctor is fully
        # inlined into create() and createLibs into wkStandbyLogin(); neither
        # symbol exists in the retail split (CfPadTask recipe). Dropping them
        # recovers exactly the 0x1A4 split-budget overflow.
        drop_text_symbols=(
            "__ct__4CLibFPCcP11CWorkThread",
            "createLibs__4CLibFv",
        ),
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
    "yvm2.o": UnitRules(
        # monolibdata2 dissolve: the TU now emits only its retail-owned data —
        # .data = vmc_op_int switch jumptable + lbl_eu_8056F038 handler table,
        # .bss = vmState + 0x14 tail (vmStateTail). The opcode-name table
        # (lbl_eu_8056ECE8) and type-name array (lbl_eu_8056EFE8) ship from
        # another TU; the source externs them under their retail labels.
        # Retail .data section align is 4; MWCC emits 8.
        set_data_align=((".data", 4),),
        # MWCC pads +4 between the switch jumptable and the handler table;
        # retail packs them back-to-back (lbl_eu_8056F038 at .data+0x24).
        drop_data_range=((".data", 0x24, 0x28),),
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
    "CDesktop.o": UnitRules(
        # monolibdata2 dissolve: all class data ships from the dissolved blocks in
        # CDesktop.cpp. The anonymous-namespace thread-class methods are defined
        # here under their retail @unnamed@ mangling, but those symbols (and the
        # base typeinfo objects) cannot be spelled in source: declaring an
        # __RTTI__* name in a TU with novtable-predeclared anonymous-namespace
        # classes trips an MWCC -ipa file ICE, and the @unnamed@ mangled spellings
        # are not legal C++ identifiers. The hand-built vtables therefore use
        # legal stand-in names; retarget the 13 vtable slots below.
        retarget_relocs=(
            (".data", 0x8, "__dt__Q222@unnamed@CDesktop_cpp@17CDesktopExceptionFv"),
            (".data", 0x98, "wkStandbyLogout__Q222@unnamed@CDesktop_cpp@17CDesktopExceptionFv"),
            (".data", 0xC8, "__dt__Q222@unnamed@CDesktop_cpp@18CDesktopBackGroundFv"),
            (".data", 0x158, "wkStandbyLogout__Q222@unnamed@CDesktop_cpp@18CDesktopBackGroundFv"),
            (".data", 0xA0, "__RTTI__10IWorkEvent"),
            (".data", 0xA8, "__RTTI__11CWorkThread"),
            (".data", 0xB0, "__RTTI__5CProc"),
            (".data", 0x160, "__RTTI__10IWorkEvent"),
            (".data", 0x168, "__RTTI__11CWorkThread"),
            (".data", 0x170, "__RTTI__5CProc"),
            (".data", 0x220, "__RTTI__10IWorkEvent"),
            (".data", 0x228, "__RTTI__11CWorkThread"),
            (".data", 0x230, "__RTTI__5CProc"),
        ),
        # MWCC emits .sbss align 8; the retail split packs the two singletons
        # plus tail padding at 4-byte boundaries.
        set_data_align=((".sbss", 4),),
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
        # Retail GC'd three MWCC-only text orphans: dummy1 (a source-side
        # isRunning-emission forcing helper retail never had) and the
        # reslist/_reslist_base dtors (strong copies live in CWorkThread.o;
        # any surviving refs resolve there via UNDEF, same as retail).
        drop_text_symbols=("dummy1__9CWorkRootFP11CWorkThread",),
        drop_text_symbols_as_undef=(
            "__dt__23reslist<P11CWorkThread>Fv",
            "__dt__29_reslist_base<P11CWorkThread>Fv",
        ),
        repack_after_drop=16,
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
    "CWorkSystemCache.o": UnitRules(
        # Retail .sdata2 is empty; MWCC still materializes a dead 4-byte
        # @LOCAL@wkUpdate@zero pool constant that nothing references (wkUpdate
        # has no float ops). Strip the section back to retail size.
        drop_data_tail=((".sdata2", 0x0),),
        # DECOMP_FORCEACTIVE keep-alive stubs for the RTTI-name/locator pools
        # were removed (all six symbols have live in-TU references); no
        # drop_text_symbols needed anymore.
    ),
    "CAIAction.o": UnitRules(
        exact_renames=(
            ("__vt__Q22cf9CAIAction", "lbl_eu_8052F598"),
            # Five state-dispatch jump tables (all .rela.data words target own
            # .text): retail keeps each in split1.s with func+addend slots that
            # resolve to our definitions at link (site-mapped per function).
            ("@3813", "jumptable_eu_8052F504"),   # func_801522C4 dispatch 2
            ("@3814", "jumptable_eu_8052F238"),   # func_801522C4 dispatch 1
            ("@4210", "jumptable_eu_8052F1AC"),   # func_80150828 dispatch 1
            ("@4211", "jumptable_eu_8052EEE0"),   # func_80150828 dispatch 2
            ("@4212", "jumptable_eu_8052EDEC"),   # func_80150828 tail dispatch
            # Float pools (site/content-mapped against unit retail refs).
            ("@3806", "lbl_eu_80667434"),   # 10.0f
            ("@3807", "lbl_eu_80667454"),   # 1.0f
            ("@3808", "lbl_eu_8066745C"),   # 0.2f
            ("@3815", "lbl_eu_80667440"),   # {128.0f,-0.0} magic double
            ("@4205", "lbl_eu_80667428"),   # {0.0,...} tail
            ("@4213", "lbl_eu_80667438"),   # {128.0f,0.0}
        ),
        # Data dissolve: strip the TU-local copies once renamed.
        extern_data_sections=(".data", ".sdata2"),
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
        # Data dissolve (split1.s owns all retail data for this TU):
        # - sinit float pool -> lbl_eu_80667CB8..CCC (per-site asm refs)
        # - cf::CTaskCulling vtable -> lbl_eu_80532FE0, CTask<..> -> 805330E8
        # - static array ...bss.0 -> lbl_eu_80575820 (.bss slice)
        # - static lbl_eu_80664328 already carries its retail name
        pool_patterns=(
            (struct.pack(">I", 0x40A00000), "lbl_eu_80667CB8"),   # 5.0f
            (struct.pack(">I", 0x41F00000), "lbl_eu_80667CBC"),   # 30.0f
            (struct.pack(">I", 0x42480000), "lbl_eu_80667CC0"),   # 50.0f
            (struct.pack(">I", 0x00000000), "lbl_eu_80667CC4"),   # 0.0f
            (struct.pack(">I", 0x3F800000), "lbl_eu_80667CC8"),   # 1.0f
            (struct.pack(">I", 0x3E4CCCCD), "lbl_eu_80667CCC"),   # 0.2f
        ),
        exact_renames=(
            ("__vt__Q22cf12CTaskCulling", "lbl_eu_80532FE0"),
            ("__vt__27CTask<Q22cf12CTaskCulling>", "lbl_eu_805330E8"),
            ("...bss.0", "lbl_eu_80575820"),
        ),
        extern_data_sections=(".rodata", ".data", ".sdata", ".sdata2", ".bss", ".sbss"),
    ),
    "CMenuPTGauge.o": UnitRules(
        # Init font-object vtable walk: MWCC colors the temp as r4; retail reuses r12
        # for both loads (semantics identical). PLAN.md §17.6.

        # Prefer unmangled retail reloc when a Pane* overload still wins linkage.
        exact_renames=(
            ("func_8013676C__FPQ34nw4r3lyt4PaneUl", "func_8013676C"),
            # Live phantom pools: {1.0f} -> lbl_eu_806679EC; magic pair ->
            # lbl_eu_806679F0 (content-equal, unit-ref'd blob labels).
            ("@12968", "lbl_eu_806679EC"),
            ("@12971", "lbl_eu_806679F0"),
        ),
        extern_data_sections=(".sdata2",),
    ),
    "CTaskGameCf.o": UnitRules(
        # func_8004433C: extern const u32 lbl_eu_80525AC4[3] without extern "C"
        # causes MWCC to mangle → lbl_eu_80525AC4__2cf; retail uses flat lbl_eu_80525AC4.
        exact_renames=(
            ("lbl_eu_80525AC4__2cf", "lbl_eu_80525AC4"),
            # Data dissolve: mission-command entries are 12-byte {0,-1,fn}
            # records in split1.s (.data lbl_eu_80525AB8..B48); each MWCC copy
            # is identified by its third-word function pointer:
            # AC4=startContinue AD0=func_800444DC ADC=initNewGame AE8/B00=
            # func_8004451c twins AF4=initContinue B0C=func_800444FC
            # B24=func_800447B4 B30=beginExit B3C=waitExit B48=finishExit.
            ("spInstance__Q22cf11CTaskGameCf", "lbl_eu_80663D38"),
            ("@12590", "lbl_eu_80525AC4"),
            ("@12593", "lbl_eu_80525AD0"),
            ("@12606", "lbl_eu_80525ADC"),
            ("@12609", "lbl_eu_80525AE8"),
            ("@12612", "lbl_eu_80525AF4"),
            ("@12615", "lbl_eu_80525B00"),
            ("@12618", "lbl_eu_80525B0C"),
            ("@12632", "lbl_eu_80525B24"),
            ("@12700", "lbl_eu_80525B30"),
            ("@12712", "lbl_eu_80525B3C"),
            ("@12729", "lbl_eu_80525B48"),
        ),
        # Remaining sdata2 pool: zero word -> lbl_eu_80665D88, 1.0f ->
        # lbl_eu_80665D8C (site correspondence).
        pool_patterns=(
            (struct.pack(">I", 0x00000000), "lbl_eu_80665D88"),
            (struct.pack(">I", 0x3F800000), "lbl_eu_80665D8C"),
        ),
        # func_80044444: &CTaskGameCf::func_8004451C emits a TU-local PTMF pool
        # entry instead of retail lbl_eu_80525AE8.  Add a pool pattern here once
        # the PTMF descriptor content (12 bytes in sdata2) is confirmed.
        # pool_patterns = ((struct.pack(">III", 0, 0, 0x8004451C), "lbl_eu_80525AE8"),)
        # Data dissolve: vtables/RTTI/typeinfo strings/command table all ship
        # from split1.s; strip every local copy.
        extern_data_sections=(".data", ".rodata", ".sdata", ".sdata2", ".sbss"),
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
    "CPackItem.o": UnitRules(
        # monolibdata2 dissolve: ppcdis splitter writes the retail .rodata
        # slice at align 4 (same 12-byte "CPackItem" string content); MWCC
        # emits the section at align 8.
        set_data_align=((".rodata", 4),),
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
        extern_data_sections=(".sdata2",),
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
        # MWCC anon numbering vs retail splitter: .data+0 69-byte blob
        # (@1966 -> @851) and .sdata+0x20 5-byte pool sym (@2293 -> @1193)
        # are referenced from .rela.sdata; retail keeps the same local
        # objects under its own numbering.
        exact_renames=(
            ("@1966", "@851"),
            ("@2293", "@1193"),
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
        # Zeros retarget into the shared nw4r .sdata2 pool; strip local.
        pool_patterns=(
            (struct.pack(">I", 0x00000000), "lbl_eu_80669A68"),
            (struct.pack(">I", 0x3B800000), "lbl_eu_80669DB8"),
            (struct.pack(">II", MAGIC_HI, 0), "lbl_eu_80669DC0"),
            (struct.pack(">I", 0x43800000), "lbl_eu_80669DC8"),
        ),
        extern_data_sections=(".sdata2",),
    ),
    # === nw4r lyt data-dissolve (scoped same-basename rules) ==================
    # Keys "Name.o#Q34nw4r3lyt" apply ONLY to the nw4r::lyt TUs (their symtabs
    # carry Q34nw4r3lyt-mangled names); the RVL_SDK hbm twins with the same
    # basenames keep retail-local .data vtables and fall back to the unscoped
    # entries above/below. nw4r retail ships every one of these data objects
    # in nw4r_data.s: rename TU-local pools/vtables to the blob labels
    # (ground truth = the U-refs in build/us/obj/nw4r/src/lyt/<Name>.o) and
    # strip the local sections so both gate sides are empty; .text relocs
    # resolve to the blob at link, mirroring the DOL-extracted retail .o.
    "lyt_pane.o#Q34nw4r3lyt": UnitRules(
        # __vt__PaneBase was GC'd by the retail linker (PaneBase ctor/dtor are
        # inline in the header); its only referencer is the orphaned PaneBase
        # ctor, which retail also GC'd — drop it so no reloc dangles after the
        # .data strip. Pane's own vtable is blob lbl_eu_805698F0; the sda21
        # pools live at lbl_eu_80669D38..D50 (NOT the generic pool start the
        # hbm twin uses).
        drop_text_symbols=("__ct__Q44nw4r3lyt6detail8PaneBaseFv",),
        exact_renames=(
            ("__vt__Q34nw4r3lyt4Pane", "lbl_eu_805698F0"),
            # Retail lyt_pane references the Layout allocator ONLY via its
            # sbss label lbl_eu_80665478 (Pane dtor's two DeleteObj frees);
            # the 80665488 target belongs to sibling lyt TUs, not this one.
            ("mspAllocator__Q34nw4r3lyt6Layout", "lbl_eu_80665478"),
        ),
        pool_patterns=(
            (struct.pack(">I", 0x00000000), "lbl_eu_80669D38"),
            (struct.pack(">I", 0x3F800000), "lbl_eu_80669D3C"),
            (struct.pack(">I", 0x3C8EFA35), "lbl_eu_80669D40"),
            (struct.pack(">I", 0x3B808081), "lbl_eu_80669D44"),
            (struct.pack(">II", MAGIC_HI, 0x00000000), "lbl_eu_80669D48"),
            (struct.pack(">I", 0x3F000000), "lbl_eu_80669D50"),
        ),
        extern_data_sections=(".data", ".sbss", ".sdata2"),
    ),
    "lyt_group.o#Q34nw4r3lyt": UnitRules(
        exact_renames=(
            ("__vt__Q34nw4r3lyt5Group", "lbl_eu_80569968"),
            ("mspAllocator__Q34nw4r3lyt6Layout", "lbl_eu_80665488"),
        ),
        extern_data_sections=(".data",),
    ),
    "lyt_layout.o#Q34nw4r3lyt": UnitRules(
        exact_renames=(
            ("__vt__Q34nw4r3lyt6Layout", "lbl_eu_80569978"),
            ("mspAllocator__Q34nw4r3lyt6Layout", "lbl_eu_80665488"),
        ),
        pool_patterns=(
            (struct.pack(">I", 0x00000000), "lbl_eu_80669D58"),
            (struct.pack(">I", 0x3F000000), "lbl_eu_80669D5C"),
        ),
        extern_data_sections=(".data", ".sdata", ".sbss", ".sdata2"),
    ),
    "lyt_picture.o#Q34nw4r3lyt": UnitRules(
        exact_renames=(
            ("__vt__Q34nw4r3lyt7Picture", "lbl_eu_805699B8"),
            ("mspAllocator__Q34nw4r3lyt6Layout", "lbl_eu_80665488"),
        ),
        pool_patterns=((struct.pack(">I", 0x00000000), "lbl_eu_80669D60"),),
        extern_data_sections=(".data", ".sbss", ".sdata2"),
    ),
    "lyt_textBox.o#Q34nw4r3lyt": UnitRules(
        exact_renames=(
            ("__vt__Q34nw4r3lyt7TextBox", "lbl_eu_80569A30"),
            ("mspAllocator__Q34nw4r3lyt6Layout", "lbl_eu_80665488"),
        ),
        pool_patterns=(
            (struct.pack(">I", 0x00000000), "lbl_eu_80669D68"),
            (struct.pack(">II", MAGIC_HI, 0x80000000), "lbl_eu_80669D70"),
            # f32 0.5/1.0 refs survive only inside decomp-residual GetTextMagH/V
            # (retail inlined them); retarget to the canonical pool entries.
            (struct.pack(">I", 0x3F000000), "lbl_eu_80669A84"),
            (struct.pack(">I", 0x3F800000), "lbl_eu_80669A6C"),
        ),
        extern_data_sections=(".data", ".sdata", ".sbss", ".sdata2"),
    ),
    "lyt_window.o#Q34nw4r3lyt": UnitRules(
        exact_renames=(
            ("__vt__Q34nw4r3lyt6Window", "lbl_eu_80569AF4"),
            ("flipInfos$5421", "lbl_eu_80569AB8"),
            ("mspAllocator__Q34nw4r3lyt6Layout", "lbl_eu_80665488"),
        ),
        pool_patterns=(
            (struct.pack(">I", 0x00000000), "lbl_eu_80669D80"),
            (struct.pack(">II", MAGIC_HI, 0x00000000), "lbl_eu_80669D88"),
            (struct.pack(">II", MAGIC_HI, 0x80000000), "lbl_eu_80669D90"),
        ),
        extern_data_sections=(".data", ".sbss", ".sdata2"),
    ),
    "lyt_material.o#Q34nw4r3lyt": UnitRules(
        exact_renames=(
            ("__vt__Q34nw4r3lyt8Material", "lbl_eu_80569C38"),
            ("kColSels$6079", "lbl_eu_80569BF8"),
            ("kAlpSels$6080", "lbl_eu_80569C18"),
            ("DefaultBlackColor", "DefaultBlackColor_8066B550"),
            ("mspAllocator__Q34nw4r3lyt6Layout", "lbl_eu_80665488"),
        ),
        pool_patterns=(
            (struct.pack(">I", 0x3F000000), "lbl_eu_80669D98"),
            (struct.pack(">I", 0x3F360B61), "lbl_eu_80669D9C"),
            (struct.pack(">I", 0x00000000), "lbl_eu_80669DA0"),
            (struct.pack(">I", 0x3F800000), "lbl_eu_80669DA4"),
            (struct.pack(">I", 0x40000000), "lbl_eu_80669DA8"),
            (struct.pack(">II", MAGIC_HI, 0x00000000), "lbl_eu_80669DB0"),
        ),
        extern_data_sections=(".data", ".sdata", ".sbss2", ".sdata2"),
    ),
    "lyt_animation.o#Q34nw4r3lyt": UnitRules(
        # The AnimTransform base ctor/dtor + vtable were GC'd by the retail
        # linker (nothing calls them — derived paths inline to ~LinkListImpl /
        # set only AnimTransformBasic's vtable). Drop the base ctor: it is the
        # only referencer of the GC'd __vt__AnimTransform, so the .data strip
        # would otherwise leave a dangling reloc.
        drop_text_symbols=("__ct__Q34nw4r3lyt13AnimTransformFv",),
        exact_renames=(
            ("__vt__Q34nw4r3lyt18AnimTransformBasic", "lbl_eu_80569C78"),
            ("mspAllocator__Q34nw4r3lyt6Layout", "lbl_eu_80665488"),
        ),
        pool_patterns=(
            (struct.pack(">I", 0x00000000), "lbl_eu_80669DF0"),
            (struct.pack(">I", 0x3F000000), "lbl_eu_80669DF4"),
        ),
        extern_data_sections=(".data", ".sdata2"),
    ),
    "lyt_resourceAccessor.o#Q34nw4r3lyt": UnitRules(
        exact_renames=(("__vt__Q34nw4r3lyt16ResourceAccessor", "lbl_eu_80569CA0"),),
        extern_data_sections=(".data",),
    ),
    "lyt_arcResourceAccessor.o#Q34nw4r3lyt": UnitRules(
        exact_renames=(("__vt__Q34nw4r3lyt19ArcResourceAccessor", "lbl_eu_80569CB8"),),
        # "." / ".." ARC-relative path fragments -> blob .sdata strings.
        data_pool_patterns=(
            (".sdata", b".\x00", "lbl_eu_806634C8"),
            (".sdata", b"..\x00", "lbl_eu_806634CC"),
        ),
        extern_data_sections=(".data", ".sdata"),
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
        # Reloc-name drift on byte-identical .data: retail refers to the
        # AnimTransformBasic RTTI (+0x50), AnimTransform RTTI (+0x78) and the
        # anon locals at +0x20/+0x40/+0x58 under split-local lbl_ names.
        # @N numbering drifts with source changes - revisit if this TU's
        # source is edited.
        exact_renames=(
            ("__RTTI__Q36nw4hbm3lyt18AnimTransformBasic", "lbl_8054D608"),
            ("__RTTI__Q36nw4hbm3lyt13AnimTransform", "lbl_8054D630"),
            ("@5485", "lbl_8054D5D8"),
            ("@5486", "lbl_8054D5F8"),
            ("@5487", "lbl_8054D610"),
        ),
        # merged from nw4r same-basename unit (nw4r_data.s shared pool)
        pool_patterns=(
            (struct.pack(">I", 0x00000000), "lbl_eu_80669A68"),
            (struct.pack(">I", 0x3F000000), "lbl_eu_80669A84"),
        ),
        extern_data_sections=(".sdata2",),
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
        # Reloc-name drift on byte-identical .data: retail names the anon
        # local at +0x48 lbl_8054D82C. @N numbering drifts with source edits.
        exact_renames=(("@5580", "lbl_8054D82C"),),
        # merged from nw4r same-basename unit (nw4r_data.s shared pool)
        pool_patterns=(
            (struct.pack(">I", 0x00000000), "lbl_eu_80669A68"),
            (struct.pack(">I", 0x3F000000), "lbl_eu_80669A84"),
        ),
        extern_data_sections=(".sdata2",),
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
        # Reloc-name drift on byte-identical .data: retail's anon local at
        # +0xC is split-local "@230"; decomp numbers it @5093. @N numbering
        # drifts with source edits.
        exact_renames=(("@5093", "@230"),),
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
        # Reloc-name drift on byte-identical .data: retail names the anon
        # local at +0x48 lbl_8054D7DC. @N numbering drifts with source edits.
        exact_renames=(("@5371", "lbl_8054D7DC"),),
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
        # Reloc-name drift on byte-identical .data tail: retail names the two
        # anon locals at +0x40/+0x44 "@7064"/"@7065"; decomp numbers them
        # @7723/@7724. @N numbering drifts with source edits.
        exact_renames=(("@7723", "@7064"), ("@7724", "@7065")),
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
        # Reloc-name drift on byte-identical .data: retail numbers the three
        # anon locals "@1133"/"@1134"/"@1135"; MWCC's numbering drifts with
        # source edits.
        exact_renames=(
            ("@5236", "@1133"),
            ("@5237", "@1134"),
            ("@5238", "@1135"),
        ),
        # nw4r twin: MWCC pools the "."/".." ARC path literals as .sdata
        # statics; retail loads them from the nw4r_data .sdata pool via SDA21
        # (lbl_eu_806634C8 / lbl_eu_806634CC). Rename the pool statics by
        # content and strip the section so the text relocs resolve at link.
        # The nw4hbm twin has no .sdata, so both steps are no-ops there.
        data_pool_patterns=(
            (".sdata", b".\x00", "lbl_eu_806634C8"),
            (".sdata", b"..\x00", "lbl_eu_806634CC"),
        ),
        extern_data_sections=(".sdata",),
    ),
    "dvd_broadway.o": UnitRules(
        # DVDLowOpenPartitionWithTmdAndTicket is real source but GC'd from the
        # retail image (never called); its string literals still pool at their
        # retail .data offsets and are reused by TicketView via -str reuse.
        # Drop only its retail-absent .text here.
        drop_text_symbols=(
            "initDvdContexts",
            "DVDLowOpenPartitionWithTmdAndTicket",
            "DVDLowGetCoverStatus",
            "DVDLowGetCoverReg",
        ),
        # The dropped function's pre-drop alignment pad survives the shift;
        # re-lay survivors at func_align=16 like the retail linker's GC did.
        repack_after_drop=16,
        # MWCC 4-pads the final format string (0xF08); retail .data ends at
        # 0xF06.
        drop_data_tail=((".data", 0xF06),),
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
        # Retail carries one extra vtable slot our shape lacks: the Font
        # vtable dtor ptr at final +0x70. Inject at PRE-DROP +0x90 so it
        # lands at +0x70 after the 0x68..0x88 range drop shifts it back.
        inject_relocs=((".data", 0x90, "__dt__Q36nw4hbm2ut4FontFv"),),
        # Trim the weak Font-vtable leftovers (shifted dtor dup at +0x74,
        # anon name/parent pair at +0x78/+0x7C, dup RTTI at +0x80) that have
        # no retail counterpart, then restore the retail 0xC0 size.
        drop_data_tail=((".data", 0x74),),
        pad_data_section=((".data", 0xC0),),
    ),
    # ut_ResFont (homebuttonLib twin): byte-identical .data; only anon-name
    # renumbering on the typeinfo-name strings / chain structs (retail
    # "@2461".."@2465" vs MWCC @2821..@2825). @N numbering drifts with
    # source edits.
    "ut_ResFont.o": UnitRules(
        exact_renames=(
            ("@2821", "@2461"),
            ("@2822", "@2462"),
            ("@2823", "@2463"),
            ("@2824", "@2464"),
            ("@2825", "@2465"),
        ),
    ),

    # synpitch: source literals now match retail's local pool ({1.0f,
    # 32000.0f, u32->f64 magic, 65536.0f}); only the trailing zero word of
    # the closing {65536.0f, 0} pair is missing - zeros-only pad.
    "synpitch.o": UnitRules(
        pad_data_section=((".rodata", 0x18),),
    ),

    "ut_RomFont.o": UnitRules(
        # Same weak inline-empty Font dtor orphan as ut_ResFontBase (nw4r
        # variant); strong copy lives in nw4r lyt_textBox.o.
        drop_text_symbols_as_undef=("__dt__Q34nw4r2ut4FontFv",),
        # The retail split object carries NO data: vtables/statics live in the
        # nw4r data objects; strip whatever MWCC emits here.
        extern_data_sections=(".data", ".rodata", ".sdata", ".sdata2", ".bss", ".sbss"),
    ),
    "ut_ArchiveFontBase.o": UnitRules(
        # Retail vtable is the shared nw4r_data blob object lbl_eu_8056AFF0;
        # the ctor stores that label explicitly (novtable pattern, see
        # ut_ArchiveFontBase.cpp) so NOTHING references the TU-local __vt__
        # copy MWCC still emits for the out-of-line dtor. Strip it.
        extern_data_sections=(".data",),
    ),
    "ut_PackedFont.o": UnitRules(
        # StreamingConstruct's switch jumptable ships from nw4r_data.s as the
        # shared blob object jumptable_eu_8056B050 (retail asm: lis/addi at
        # 8043078C/804307A0). Retarget the HA/LO pair onto it and strip the
        # TU-local .data copy (the ctor vtable store already targets
        # lbl_eu_8056B084 directly).
        retarget_relocs=(
            (".text", 0x3A6, "jumptable_eu_8056B050"),
            (".text", 0x3BA, "jumptable_eu_8056B050"),
        ),
        extern_data_sections=(".data",),
    ),
    "snd_FxChorusDpl2.o": UnitRules(
        # Retail split carries NO data: the shared FxBase vtable ships from
        # nw4r_data.s @lbl_eu_8056A798 (stored by __ct via lis/addi); the
        # weak local __vt__FxBase copy MWCC emits for the inline-empty base
        # virtuals is renamed to the retail label and .data stripped.
        exact_renames=(("__vt__Q34nw4r3snd6FxBase", "lbl_eu_8056A798"),),
        extern_data_sections=(".data",),
        # Retail GC'd every copy of the inline-empty/weak dtors and the FxBase
        # virtual stubs (the stripped .data vtable was their only referencer):
        # unnamed NonCopyable dtor (0x40), weak __dt__FxBase (0x40), weak
        # __dt__LinkListNode (0x40) and the 4/4/8-byte FxBase UpdateBuffer/
        # Shutdown/StartUp stubs — 0xD0 total. Dropping all six restores the
        # retail 0x5A4 budget (drop_text_symbols shifts survivors into the
        # retail packing; no repack needed — several survivors are sub-16).
        drop_text_symbols=(
            "__dt__Q44nw4r2ut30@unnamed@snd_FxChorusDpl2_cpp@11NonCopyableFv",
            "__dt__Q34nw4r3snd6FxBaseFv",
            "__dt__Q34nw4r2ut12LinkListNodeFv",
            "UpdateBuffer__Q34nw4r3snd6FxBaseFiPPvUlQ34nw4r3snd12SampleFormatfQ34nw4r3snd10OutputMode",
            "Shutdown__Q34nw4r3snd6FxBaseFv",
            "StartUp__Q34nw4r3snd6FxBaseFv",
        ),
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
        # Reloc-name drift on byte-identical .data tail: retail names the two
        # anon locals at +0xE0/+0xE4 lbl_8054DB58 / lbl_8054DB6C. @N numbering
        # drifts with source edits.
        exact_renames=(
            ("@5580", "lbl_8054DB58"),
            ("@5581", "lbl_8054DB6C"),
        ),
        # merged from nw4r same-basename unit (nw4r_data.s shared pool)
        pool_patterns=(
            (struct.pack(">I", 0x00000000), "lbl_eu_80669A68"),
            (struct.pack(">II", 0x43300000, 0x00000000), "lbl_eu_80669A78"),
        ),
        extern_data_sections=(".sdata2",),
    ),

    "lyt_pane.o#Q36nw4hbm3lyt": UnitRules(
        # SCOPED to the RVL_SDK homebuttonLib twin (its symtab carries
        # Q36nw4hbm3lyt names); the plain basename previously also caught the
        # nw4r twin, whose retail split ships all data in nw4r_data.s — that
        # twin uses lyt_pane.o#Q34nw4r3lyt above.
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
        # merged from nw4r same-basename unit (nw4r_data.s shared pool)
        pool_patterns=(
            (struct.pack(">I", 0x00000000), "lbl_eu_80669A68"),
            (struct.pack(">I", 0x3F800000), "lbl_eu_80669A6C"),
            (struct.pack(">II", 0x43300000, 0x00000000), "lbl_eu_80669A78"),
            (struct.pack(">I", 0x3F000000), "lbl_eu_80669A84"),
            (struct.pack(">I", 0x3B808081), "lbl_eu_80669C00"),
            (struct.pack(">I", 0x3C8EFA35), "lbl_eu_80669D40"),
        ),
        extern_data_sections=(".sdata2",),
        # Retail keeps Pane's 0x68 vtable in .data. Natural MWCC output GCs
        # it (weak, unreferenced); the source-side &dtor reference in
        # sPaneVtableSlots makes -ipa emit the full table. Two slots point
        # outside this TU and stay unrelocated in our object:
        #   +0x00 -> __RTTI__Pane   (+0x0C -> GetRuntimeTypeInfo__PaneCFv,
        # dropped from .text here; the DOL link resolves it via the weak def
        # in HBMGUIManager.o, per the drop note above). Inject both.
        exact_renames=(
            ("sPaneVtableSlots", "__vt__Q36nw4hbm3lyt4Pane"),
            ("sPanePool", "lbl_80518A98"),
        ),
        # Tail layout: after sPanePool, MWCC still pools the Size() zero-init
        # 0.0f (Pane ctor) plus 8-alignment padding ahead of the u32->f64
        # conversion magic that retail keeps AT lbl_80518A98+0x18. Exchange
        # [0x18,0x20) with the magic at [0x20,0x28) so the magic lands at
        # +0x18, then trim to the retail 0x20 (the displaced 0.0f symbol is
        # ABS'd past the cut, standard drop_data_tail behavior).
        swap_data_blocks=((".rodata", 0x18, 0x20, 8),),
        drop_data_tail=((".rodata", 0x20),),
        # Literal-pool determinism: all Pane float literals were replaced by
        # references to sPanePool[6] (source-defined in retail order), so the
        # anonymous pool (and its run-to-run ordering nondeterminism) is gone.
        # Rename the table to retail's lbl_80518A98 so .text displacements
        # resolve identically.
        inject_relocs=(
            (".data", 0x00, "__RTTI__Q36nw4hbm3lyt4Pane"),
            (".data", 0x0C, "GetRuntimeTypeInfo__Q36nw4hbm3lyt4PaneCFv"),
        ),
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
        # Merged: also covers the nw4r same-basename unit via the shared
        # nw4r_data.s pool (zeros + 1.0f), stripping local .sdata2 for both.
        pool_patterns=(
            (struct.pack(">II", MAGIC_HI, MAGIC_LO), "lbl_eu_80669EF0"),
            (struct.pack(">I", 0x00000000), "lbl_eu_80669A68"),
            (struct.pack(">I", 0x3F800000), "lbl_eu_80669A6C"),
        ),
        exact_renames=(
            ("__vt__Q44nw4r3snd6detail10BasicSound", "lbl_eu_8056A710"),
        ),
        # nw4r twin: .data vtable content == blob lbl_eu_8056A710 (verified
        # byte-identical); the local .sbss word is already named for its retail
        # home lbl_eu_806654D0. Strip both storages (symbols become UNDEF and
        # resolve to the shared data object at link).
        extern_data_sections=(".sdata2", ".data", ".sbss"),
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
        # merged from nw4r same-basename unit (nw4r_data.s shared pool)
        pool_patterns=(
            (struct.pack(">I", 0x00000000), "lbl_eu_80669A68"),
            (struct.pack(">I", 0x3F800000), "lbl_eu_80669A6C"),
            (struct.pack(">I", 0x3F000000), "lbl_eu_80669A84"),
        ),
        # The Pane/PaneBase RTTI chain struct at +0x90 references both RTTIs
        # EXTERNALLY in retail ({PaneBase, 0, Pane, 0, 0, 0}); MWCC emits weak
        # local copies in the +0xB0..+0x100 tail along with three anon locals
        # (@5542/@5543/@5544). Retarget the two live ptrs, drop the tail.
        retarget_relocs=(
            (".data", 0x90, "__RTTI__Q46nw4hbm3lyt6detail8PaneBase"),
            (".data", 0x98, "__RTTI__Q36nw4hbm3lyt4Pane"),
        ),
        # Reloc-name drift on byte-identical .data: retail names the TextBox
        # typeinfo-name string (+0x78) and chain struct (+0x90)
        # lbl_8054DA68 / lbl_8054DA80. @N numbering drifts with source edits.
        exact_renames=(
            ("@5540", "lbl_8054DA68"),
            ("@5541", "lbl_8054DA80"),
        ),
        drop_data_tail=((".data", 0xB0),),
        extern_data_sections=(".sdata2",),
    ),

    "CtrlAct.o": UnitRules(
        # The (f32)(s32) conversion's 2^52 magic pools in .sdata2 as @NNNN;
        # retail references the named constant lbl_eu_80666D50 (content match).
        pool_patterns=(
            (struct.pack(">II", MAGIC_HI, MAGIC_LO), "lbl_eu_80666D50"),
        ),
        # GC/3.0a5.2 pools BOTH switch jumptables into locals that BORROW the
        # name lbl_eu_80666D50 (.data 0x64 table + .sdata2 double). The .text
        # dispatch relocs bind to the borrowed .data local: retarget that one
        # symbol in place (retarget_relocs renames only the reloc's target,
        # unlike objcopy --redefine-sym which would clobber the global UNDEF
        # and the .sdata2 copy too). func_800D34D4's table -> jumptable_eu
        # _8052B054 (site 800D40F0); func_800D1F0C's table base ->
        # lbl_eu_8052B080 (site 800D1C28). The .sdata2 double keeps its
        # retail name and is globalized so the strip resolves externally.
        retarget_relocs=((".text", 0x14BE, "lbl_eu_8052B080"),),
        exact_renames=(("@7406", "jumptable_eu_8052B054"),),
        globalize_symbols=("lbl_eu_80666D50",),
        extern_data_sections=(".data", ".sdata2"),
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
            # func_801D1220 switch cookie (drifts with TU growth; ocBdat pattern):
            # retail keeps the dense-switch jump table in .data as
            # jumptable_eu_80534704; MWCC emits it under an internal @NNN name.
            ("@16239", "jumptable_eu_80534704"),
        ),
        # Data dissolve (split1.s owns all retail data for this TU).
        # sdata2 pool: int->float magics + misc doubles; per-fn asm refs.
        pool_patterns=(
            (struct.pack(">II", 0x43300000, 0x80000000), "lbl_eu_80667F48"),
            (struct.pack(">II", 0x43300000, 0x00000000), "lbl_eu_80667F40"),
        ),
        retarget_relocs=(
            # func_801C5FC0 double literals: 0.5/0.1 have no unit-referenced
            # blob labels (value-exact placeholders keep the link live);
            # 1.01 double -> lbl_eu_806662E8; 0.01f -> lbl_eu_80667F58.
            (".text", 0xCBC, "lbl_eu_80666518"),
            (".text", 0xCE4, "lbl_eu_80666518"),
            (".text", 0xD68, "lbl_eu_806662E8"),
            (".text", 0xEAC, "lbl_eu_80667F58"),
            # Move-switch jump tables in .data (size-matched, order-by-use):
            (".text", 0x1B7E, "jumptable_eu_805345F0"),
            (".text", 0x1B86, "jumptable_eu_805345F0"),
            (".text", 0x1CB2, "jumptable_eu_805345C8"),
            (".text", 0x1CBA, "jumptable_eu_805345C8"),
            (".text", 0x1EFA, "jumptable_eu_80534598"),
            (".text", 0x1F02, "jumptable_eu_80534598"),
            (".text", 0x58A6, "jumptable_eu_80534628"),
            (".text", 0x58AE, "jumptable_eu_80534628"),
            (".text", 0xA75A, "jumptable_eu_80534694"),
            (".text", 0xA762, "jumptable_eu_80534694"),
        ),
        extern_data_sections=(".data", ".sdata2"),
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
        extern_data_sections=(".data", ".sdata2"),
    ),
    "CAttrTransform.o": UnitRules(
        exact_renames=(
            ("@STRING@AsinRad__Q24nw4r4mathFf", "lbl_eu_8052ADB0"),
            ("@STRING@AsinRad__Q24nw4r4mathFf@0", "lbl_eu_8052AD88"),
        ),
        pool_patterns=(
            (struct.pack(">I", 0x3F000000), "lbl_eu_8066A198"),
            (struct.pack(">I", 0x4222F983), "lbl_eu_8066A190"),
            (struct.pack(">I", 0xBF800000), "lbl_eu_8066A194"),
            (struct.pack(">I", 0x3CC90FDB), "lbl_eu_8066A19C"),
        ),
        extern_data_sections=(".data", ".sdata2"),
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
    "ut_CharWriter.o": UnitRules(
        # Builtin-cast int->f32 idiom (retail fsubs against the shared signed
        # magic): MWCC pools the 0x4330000080000000 double locally; rename it
        # onto the nw4r_data.s blob label (GetFontWidth/Height/Ascent/Descent,
        # SetFontSize, Print, PrintGlyph, SetupGX*), then strip the local copy
        # (retail ut_CharWriter.o is .text/.sbss only).
        pool_patterns=(
            (struct.pack(">II", 0x43300000, 0x80000000), "lbl_eu_8066A148"),
        ),
        extern_data_sections=(".sdata2",),
    ),

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
    # (.sdata2 0x8066A160-0x8066A177). Source now binds every one of these
    # directly via extern "C" declarations/definitions (floats, blob bytes),
    # so no retarget_relocs are needed -- stale hardcoded .text offsets here
    # used to corrupt whichever functions currently occupy those offsets.
    # SCOPED to the nw4r twin: the plain basename also matched the
        # RVL_SDK homebuttonLib twin (main/RVL_SDK/.../nw4hbm/ut/
        # ut_TextWriterBase), whose retail split KEEPS .bss (0x2A of
        # TextWriterBase<char>/<w> statics + guards) and .rodata (0x18)
        # that this entry's extern_data_sections was wrongly stripping.
        pool_patterns=(
            (struct.pack(">II", MAGIC_HI, MAGIC_LO), "lbl_eu_8066A168"),  # 2^52 (signed, int->f32 magic)
            (struct.pack(">I", 0x7F7FFFFF), "lbl_eu_8066A160"),             # FLT_MAX
            (struct.pack(">I", 0x00000000), "lbl_eu_8066A164"),             # 0.0f
            (struct.pack(">I", 0x3F000000), "lbl_eu_8066A170"),             # 0.5f
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

    # ut_TextWriterBase (homebuttonLib twin): needs no renames/strips - its
    # natural output data-matches after two source-side cast fixes (unsigned
    # int->f32 conversions in PrintImpl / Get+SetLineHeight / CalcLineRectImpl
    # so MWCC pools the unsigned magic like retail). Only gap was the trailing
    # zero word of the {0.5f, 0} rodata pair - zeros-only pad restores it.
    "ut_TextWriterBase.o#Q36nw4hbm2ut": UnitRules(
        pad_data_section=((".rodata", 0x18),),
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

    # g3d_anmchr: retail split is .text-only; the three AnmObjChr* vtables
    # ship from nw4r_data.s (lbl_eu_80569330/805693E0/80569438) and the
    # ResName type-name strings are lbl_eu_8051D5C0/D0/E4/F8 (already
    # retail-named in the object). Rename the vtables and strip .data/.rodata
    # so the .text relocs resolve to the retail data object at link.
    "g3d_anmchr.o": UnitRules(
        exact_renames=(
            ("__vt__Q34nw4r3g3d12AnmObjChrRes", "lbl_eu_80569330"),
            ("__vt__Q34nw4r3g3d13AnmObjChrNode", "lbl_eu_805693E0"),
            ("__vt__Q34nw4r3g3d9AnmObjChr", "lbl_eu_80569438"),
        ),
        pool_patterns=(
            # AnmObjChrBlend ctor pools 0.0f/1.0f and the int->double magic
            # locally; retail loads lbl_eu_80669B88 (.float 0),
            # lbl_eu_80669B8C (.float 1) and lbl_eu_80669B98
            # (.double 4503601774854144 = 0x43300000_80000000) via sda21
            # (retail g3d_anmchr.s @0x803E747C-98). Order matters: the
            # zero+one pair must match before the bare one-word patterns.
            (struct.pack(">II", 0x00000000, 0x3F800000), "lbl_eu_80669B88"),
            (struct.pack(">I", 0x3F800000), "lbl_eu_80669B8C"),
            (struct.pack(">II", 0x43300000, 0x00000000), "lbl_eu_80669B98"),
        ),
        extern_data_sections=(".data", ".rodata", ".sdata2"),
    ),

    # g3d_gpu: MWCC pools the per-function local const fifo pointer literals
    # (0xCC008000) into six unreferenced .sdata2 orphans the retail linker
    # GC'd (retail folds each store into lis+offset immediates). The live
    # float/int pool constants are source-level externs to the retail labels
    # (lbl_eu_80669BC8..BDC) and stay UNDEF; just drop the dead pool.
    "g3d_gpu.o": UnitRules(
        extern_data_sections=(".sdata2",),
    ),

    # g3d_restev: retail split is .text-only; all three constant tables live
    # in nw4r_data.s: GXGetTevOrder's RAS->GXChannelID table (lbl_eu_8051D4A0,
    # 8 x GXChannelID), CallDisplayList's per-stage DL sizes (lbl_eu_8051D4C0,
    # 16 words) and GXSetTevOrder's channel->RAS table (lbl_eu_805690C0).
    # The source tables carry the retail values, so rename the local symbols
    # to the retail labels and strip .rodata/.sdata2; the .text relocs then
    # resolve to the retail data object at link.
    "g3d_restev.o": UnitRules(
        exact_renames=(
            ("r2c$9715", "lbl_eu_8051D4A0"),   # GXGetTevOrder r2c
            ("dlsize$9750", "lbl_eu_8051D4C0"),  # CallDisplayList dlsize
            ("r2c$9823", "lbl_eu_805690C0"),   # GXSetTevOrder r2c
        ),
        extern_data_sections=(".rodata", ".sdata2"),
    ),

    # g3d_resanmscn: retail split is .text-only; the five ResName pascal-string
    # lookup keys ("LightSet(NW4R)"/"AmbLights(NW4R)"/"Lights(NW4R)"/
    # "Fogs(NW4R)"/"Cameras(NW4R)") live in nw4r_data.s
    # (lbl_eu_805690E0..lbl_eu_80569160). Source strings match retail; rename
    # the file-static ResNameData symbols to the retail labels and strip .data
    # so the .text relocs resolve to the retail data object at link.
    "g3d_resanmscn.o": UnitRules(
        exact_renames=(
            (
                "ResNameData_LightSet__Q34nw4r3g3d27@unnamed@g3d_resanmscn_cpp@",
                "lbl_eu_805690E0",
            ),
            (
                "ResNameData_AmbLights__Q34nw4r3g3d27@unnamed@g3d_resanmscn_cpp@",
                "lbl_eu_80569100",
            ),
            (
                "ResNameData_Lights__Q34nw4r3g3d27@unnamed@g3d_resanmscn_cpp@",
                "lbl_eu_80569120",
            ),
            (
                "ResNameData_Fog__Q34nw4r3g3d27@unnamed@g3d_resanmscn_cpp@",
                "lbl_eu_80569140",
            ),
            (
                "ResNameData_Camera__Q34nw4r3g3d27@unnamed@g3d_resanmscn_cpp@",
                "lbl_eu_80569160",
            ),
        ),
        extern_data_sections=(".data",),
    ),

    # g3d_anmtexpat: same shape as g3d_anmclr — retail split is .text-only;
    # vtables ship from nw4r_data.s (lbl_eu_80569210/lbl_eu_80569258) and the
    # ResName type-name strings are lbl_eu_8051D560/8051D578 (already
    # retail-named in the object). Rename the vtables and strip all data
    # sections.
    "g3d_anmtexpat.o": UnitRules(
        exact_renames=(
            ("__vt__Q34nw4r3g3d15AnmObjTexPatRes", "lbl_eu_80569210"),
            ("__vt__Q34nw4r3g3d12AnmObjTexPat", "lbl_eu_80569258"),
        ),
        extern_data_sections=(".data", ".rodata"),
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
    "lyt_common.o#Q34nw4r": UnitRules(
        # SCOPED to the nw4r twin: the plain basename also matched the
        # RVL_SDK homebuttonLib twin (main/RVL_SDK/.../nw4hbm/lyt/lyt_common),
        # whose retail split KEEPS a 0x28 .bss static-local (texCoords guard
        # + array) that this entry's extern_data_sections was wrongly
        # stripping. The hbm twin needs NO rules — its natural output already
        # data-matches.
        exact_renames=(
            ("@GUARD@pCoords$5466", "lbl_eu_806654A0"),
            ("pCoords$5466", "lbl_eu_80637B00"),
            # mspAllocator's only strong definition used to be lyt_layout.o's
            # .sbss; the nw4r data-dissolve moved it into nw4r_data.s as
            # lbl_eu_80665488, so retarget this TU's reference to keep the
            # final link resolved.
            ("mspAllocator__Q34nw4r3lyt6Layout", "lbl_eu_80665488"),
        ),
        pool_patterns=(
            (struct.pack(">I", 0x00000000), "lbl_eu_80669DF8"),  # 0.0f
            (struct.pack(">I", 0x3F800000), "lbl_eu_80669DFC"),  # 1.0f
        ),
        extern_data_sections=(".sdata2", ".bss", ".sbss"),
        drop_text_symbols=("__dt__Q34nw4r2ut5ColorFv",),
    ),

    # CLibCriMoviePlay: monolibdata2 dissolve — the class vtable (+CDeviceVICb
    # sub-vtable), the typeinfo/base-list block, the .sdata RTTI locator pair
    # and the .rodata name/status strings are spelled by hand in
    # CLibCriMoviePlay.cpp (both bases are __declspec(novtable); CLibG3d
    # pattern). The deleting-dtor vtable slot is a bare tail branch whose
    # retail symbol spells "@452@" (unspellable in C++): rename the asm thunk
    # to the retail name (CDeviceVI.o thunk_456 pattern). Retail split sections
    # are align 8; MWCC emits 4 for these arrays.
    "CLibCriMoviePlay.o": UnitRules(
        exact_renames=(
            # asm void thunk mangles its () params onto the name
            ("thunk_452_dt__Fv", "@452@__dt__16CLibCriMoviePlayFv"),
            # Same asm-void mangling on the CDeviceVICb viBeginFrame thunk:
            # asm void func_8045B3D4__16CLibCriMoviePlayFv(void) emits the
            # identifier + "__Fv"; retail spells the plain mangled member name.
            ("func_8045B3D4__16CLibCriMoviePlayFv__Fv",
             "func_8045B3D4__16CLibCriMoviePlayFv"),
        ),
        # The TU compiles -RTTI on (8 functions already matched under these
        # flags), so __RTTI__10IWorkEvent / __RTTI__11CWorkThread cannot be
        # declared in source (MWCC implicitly declares those typeinfo names;
        # an extern "C" decl collides, error 10322). The two typeinfo words
        # in lbl_eu_8056D008 reference the foreign UNDEF lbl_eu_80663618
        # placeholder (file bytes stay 0, matching retail) and are retargeted
        # to the retail __RTTI__ names here (lyt_picture.o pattern).
        retarget_relocs=(
            (".data", 0xC8, "__RTTI__10IWorkEvent"),
            (".data", 0xD0, "__RTTI__11CWorkThread"),
        ),
        set_data_align=(
            (".rodata", 8),
            (".data", 8),
            (".sdata", 8),
        ),
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
    "ut_TagProcessorBase.o#Q34nw4r": UnitRules(
        # SCOPED to the nw4r twin: the plain basename also matched the
        # RVL_SDK homebuttonLib twin (main/RVL_SDK/.../nw4hbm/ut/
        # ut_TagProcessorBase), whose retail split KEEPS 0x88 of .data that
        # this entry's extern_data_sections was wrongly stripping.
        exact_renames=(
            ("__vt__Q34nw4r2ut19TagProcessorBase<c>", "lbl_eu_8056AE1C"),
            ("__vt__Q34nw4r2ut19TagProcessorBase<w>", "lbl_eu_8056AE08"),
        ),
        pool_patterns=(
            (struct.pack(">II", MAGIC_HI, MAGIC_LO), "lbl_eu_8066A130"),  # 2^52
        ),
        extern_data_sections=(".data", ".sdata2"),
    ),

    # ut_TagProcessorBase (homebuttonLib twin): natural output already
    # data-matches retail; only anon-name drift on the two typeinfo-name
    # strings (retail "@2491"/"@2492" vs MWCC @2839/@2840). @N numbering
    # drifts with source edits.
    "ut_TagProcessorBase.o#Q36nw4hbm": UnitRules(
        exact_renames=(("@2839", "@2491"), ("@2840", "@2492")),
    ),
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
        # Reloc-name drift on byte-identical .data: retail names the anon
        # locals at +0x64/+0x7C/+0x98/+0xAC/+0xC0 under split-local lbl_ names
        # (typeinfo-name strings / vtable stubs). @N numbering drifts with
        # source edits.
        exact_renames=(
            ("@5114", "lbl_8054D71C"),
            ("@5115", "lbl_8054D734"),
            ("@5116", "lbl_8054D750"),
            ("@5117", "lbl_8054D764"),
            ("@5118", "lbl_8054D778"),
        ),
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
    "CDeviceFileCri.o": UnitRules(
        # The compiler emits the extab DESTROYBASE dtor of the second base
        # (inline-empty ~UnkStruct_8044F65C) as __dt__18UnkStruct_8044F65CFv;
        # retail names that exact 0x40 body __dt__FP10IExceptionFv (defined
        # only here, US .text:0x804522FC). The hand-written copy was removed
        # (byte-identical duplicate); rename the compiler body instead so the
        # extab/extabindex relocs resolve to the retail name.
        exact_renames=(
            ("__dt__18UnkStruct_8044F65CFv", "__dt__FP10IExceptionFv"),
            # Retail map names this static member __Fv although the body takes
            # and forwards one arg in r3 (retail's defining TU saw a
            # no-prototype declaration; CDeviceFile.cpp also calls it no-args).
            ("func_8044FB08__14CDeviceFileCriFPCc", "func_8044FB08__14CDeviceFileCriFv"),
            # Retail exports the ctor under the friendly map name
            # __ct__CDeviceFileCri (.text 0x80452260, 0x9C) — also the name
            # CDeviceFile.cpp imports.
            ("__ct__14CDeviceFileCriFPCcP11CWorkThreadi", "__ct__CDeviceFileCri"),
        ),
    ),
    "CDeviceFileJob.o": UnitRules(
        exact_renames=(("__vt__14CDeviceFileJob", "lbl_eu_8056C4D8"),),
        extern_data_sections=(".data", ".rodata", ".sdata", ".sdata2", ".bss", ".sbss"),
        repack_after_drop=4,
    ),
    "CScn.o": UnitRules(
        extern_data_sections=(".data", ".rodata", ".sdata", ".sdata2", ".bss", ".sbss"),
    ),
    "CDeviceVI.o": UnitRules(
        drop_data_tail=((".data", 0x170), (".rodata", 0xB9), (".sdata", 0x18)),
        exact_renames=(
            ("thunk_456_dt", "@456@__dt__9CDeviceVIFv"),
            ("thunk_456_error", "@456@errorWiiCB__9CDeviceVIFv"),
            ("__dt__23reslist_P11CDeviceVICbFv", "__dt__23reslist<P11CDeviceVICb>Fv"),
            ("__dt__29_reslist_base_P11CDeviceVICbFv", "__dt__29_reslist_base<P11CDeviceVICb>Fv"),
        ),
    ),
    "CDevice.o": UnitRules(
        zero_data_range=((".data", 0x168, 0x170),),
        drop_data_tail=((".data", 0x170), (".rodata", 0xA0), (".sdata", 0x10), (".sdata2", 0x0)),
        drop_nobits_range=((".bss", 0x88, 0xB8),),
        exact_renames=(
            ("@8686", "lbl_eu_80522AA8"),
            ("@8687", "lbl_eu_8056C0A0"),
            ("@8864", "lbl_eu_8066A3A8"),
            ("@8865", "lbl_eu_8056C158"),
            ("__RTTI__Q221@unnamed@CDevice_cpp@16CDeviceException", "lbl_eu_80663680"),
            ("__RTTI__7CDevice", "lbl_eu_80663688"),
        ),
        inject_relocs=(
            (".data", 8, "__dt__Q221@unnamed@CDevice_cpp@16CDeviceExceptionFv"),
            (".data", 152, "wkStandbyLogout__Q221@unnamed@CDevice_cpp@16CDeviceExceptionFv"),
            (".data", 0xA0, "__RTTI__10IWorkEvent"),
            (".data", 0xA8, "__RTTI__11CWorkThread"),
            (".data", 0x158, "__RTTI__10IWorkEvent"),
            (".data", 0x160, "__RTTI__11CWorkThread"),
        ),
    ),
    "CDeviceClock.o": UnitRules(
        drop_data_tail=((".data", 0xE0), (".rodata", 0x58), (".sdata", 0x18)),
        drop_nobits_range=((".sbss", 0, 4),),
        exact_renames=(
            ("__dt__29reslist_P17IDeviceClockFrameFv", "__dt__29reslist<P17IDeviceClockFrame>Fv"),
            ("__dt__35_reslist_base_P17IDeviceClockFrameFv", "__dt__35_reslist_base<P17IDeviceClockFrame>Fv"),
            ("__RTTI__29reslist_P17IDeviceClockFrame", "__RTTI__29reslist<P17IDeviceClockFrame>"),
            ("__RTTI__35_reslist_base_P17IDeviceClockFrame", "__RTTI__35_reslist_base<P17IDeviceClockFrame>"),
            ("__vt__29reslist_P17IDeviceClockFrame", "__vt__29reslist<P17IDeviceClockFrame>"),
            ("__vt__35_reslist_base_P17IDeviceClockFrame", "__vt__35_reslist_base<P17IDeviceClockFrame>"),
        ),
        inject_relocs=(
            (".data", 0xA0, "__RTTI__10IWorkEvent"),
            (".data", 0xA8, "__RTTI__11CWorkThread"),
        ),
    ),
    "CDeviceFontLayer.o": UnitRules(
        pad_data_section=((".data", 0xD8), (".rodata", 0x78), (".sdata", 0x18)),
        # The u32->f64 conversion magic (2^52) must carry the retail
        # CGXCache/monolibdata2 pool name before the local .sdata2 is stripped
        # (func_80453F78 lfd site).
        pool_patterns=(
            (struct.pack(">II", 0x43300000, 0x00000000), "lbl_eu_8066A420"),
        ),
        drop_data_tail=((".sdata2", 0x0),),
    ),
    "CLibLayout.o": UnitRules(
        # The TU compiles -RTTI on, so the vtable's base-list slots reference
        # __RTTI__10IWorkEvent / __RTTI__11CWorkThread. Those names cannot be
        # spelled in this source (declaring an __RTTI__* name in a TU with a
        # novtable-predeclared class trips an MWCC -ipa file ICE, "illegal name
        # overloading" -- same reason CWorkRoot.o uses inject_relocs); the
        # source uses the legal stand-in spellings rtti_* and this rule renames
        # the two undefined externs (and their .data relocs) to the retail
        # typeinfo symbols defined in IWorkEvent.o / CWorkThread.o.
        exact_renames=(
            ("rtti_10IWorkEvent", "__RTTI__10IWorkEvent"),
            ("rtti_11CWorkThread", "__RTTI__11CWorkThread"),
        ),
    ),
    "CLibStaticData.o": UnitRules(
        # IWORK_EVENT_INLINE_DTOR makes MWCC emit a weak local
        # __dt__10IWorkEventFv that the extab then binds to; retail keeps the
        # strong copy external (IWorkEvent.cpp). Drop it as UNDEF so the extab
        # reloc resolves to the strong copy at link.
        drop_text_symbols_as_undef=("__dt__10IWorkEventFv",),
        # Retail keeps the two class vtables (lbl_eu_8056D408 / lbl_eu_8056D4C0),
        # one 0x18 base-list blob (lbl_eu_8056D4A8, .data +0xA0) and the two
        # 8-byte typeinfo structs (lbl_eu_806637E0 / lbl_eu_806637E8, .sdata)
        # plus their name strings (.rodata) here. The base-class RTTI
        # (__RTTI__10IWorkEvent / __RTTI__11CWorkThread) structs and name
        # strings MWCC emits as weak local copies are external in retail
        # (strong copies live in CWorkThread.o / IWorkEvent.o). Rename the
        # vtable typeinfo slots and the content-matched @N pool objects to the
        # retail labels, zero the weak .sdata tail refs (bytes are already 0)
        # to drop those relocs, then trim each section to the retail slice.
        exact_renames=(
            ("__vt__14CLibStaticData", "lbl_eu_8056D408"),
            ("__vt__Q214CLibStaticData5CItem", "lbl_eu_8056D4C0"),
            ("__RTTI__14CLibStaticData", "lbl_eu_806637E0"),
            ("__RTTI__Q214CLibStaticData5CItem", "lbl_eu_806637E8"),
        ),
        data_pool_patterns=(
            (".rodata", b"CLibStaticData\x00", "lbl_eu_805231D0"),
            (".rodata", b"CLibStaticData::CItem\x00", "lbl_eu_805231E0"),
            # The 0x14 all-reloc-zero base-list struct (@N, sole 0x14-byte
            # referenced @ in .data) is retail lbl_eu_8056D4A8.
            (".data", struct.pack(">IIIII", 0, 0, 0, 0, 0), "lbl_eu_8056D4A8"),
        ),
        # The weak base-class typeinfo refs in .sdata (+0xC..+0x20) point at
        # objects retail GC'd; remove the range (bytes are already 0) to drop
        # those relocs, re-pad to the retail 0x10 slice, and trim .data/.rodata
        # back to their retail sizes.
        drop_data_range=((".sdata", 0xC, 0x20),),
        pad_data_section=((".sdata", 0x10),),
        drop_data_tail=(
            (".data", 0x140),
            (".rodata", 0x28),
        ),
    ),
    "CLibHbm.o": UnitRules(
        # pool-coupled: local .sdata2 float/double pool -> CGXCache shared
        # pool (lbl_eu_8066Axxx): 0.0 / 1.3684211 / 1.0 / 1.2 f32, the u32->f64
        # conversion magic 2^52 (lbl_eu_8066A540), 0.25 / 1.25 / 4.0 / 686.0 /
        # 640.0 f32, and the s32->f32 conversion magic 2^52+2^31
        # (lbl_eu_8066A560). The vtable/RTTI/jumptable/sbss data are defined in
        # source; only .sdata2 ships from the shared pool.
        pool_patterns=(
            (struct.pack(">I", 0x00000000), "lbl_eu_8066A530"),
            (struct.pack(">I", 0x3FAF286C), "lbl_eu_8066A534"),
            (struct.pack(">I", 0x3F800000), "lbl_eu_8066A538"),
            (struct.pack(">I", 0x3F99999A), "lbl_eu_8066A53C"),
            (struct.pack(">II", 0x43300000, 0x00000000), "lbl_eu_8066A540"),
            (struct.pack(">I", 0x3E800000), "lbl_eu_8066A548"),
            (struct.pack(">I", 0x3FA00000), "lbl_eu_8066A54C"),
            (struct.pack(">I", 0x40800000), "lbl_eu_8066A550"),
            (struct.pack(">I", 0x442B8000), "lbl_eu_8066A554"),
            (struct.pack(">I", 0x44200000), "lbl_eu_8066A558"),
            (struct.pack(">II", 0x43300000, 0x80000000), "lbl_eu_8066A560"),
        ),
        extern_data_sections=(".sdata2",),
        # Retail linker pads .data to the 8-align tail after jumptable_eu_8056D260
        # (+0xE4 -> +0xE8); MWCC emits the section unpadded.
        pad_data_section=((".data", 0xE8),),
    ),
    "HBMCommon.o": UnitRules(
        # homebutton dissolve: MWCC emits the HBM release-banner string
        # ("<< RVL_SDK - HBM ...>>", 70 bytes at .data+0, pointed to by the
        # word at +0x48) under an unspellable @N; retail names the blob
        # lbl_8054C878.
        data_pool_patterns=(
            (
                ".data",
                b"<< RVL_SDK - HBM \trelease build: Feb 24 2010 16:19:07 (0x4302_145) >>\x00",
                "lbl_8054C878",
            ),
        ),
    ),
    "HBMRemoteSpk.o": UnitRules(
        # homebutton RTTI convention: flat custom symbol names instead of
        # MWCC's mangled __RTTI__Q210... form, and the typeinfo name string
        # under an unspellable @N.
        exact_renames=(
            ("__RTTI__Q210homebutton9RemoteSpk", "__RTTI__homebutton_RemoteSpk"),
        ),
        data_pool_patterns=(
            (".data", b"homebutton::RemoteSpk\x00", "homebutton_RemoteSpk_typestr"),
        ),
    ),
    "HBMFrameController.o": UnitRules(
        # Retail keeps the "homebutton::FrameController" typeinfo name string
        # ANONYMOUS in .data (splitter @7006); the lbl_80518668 name belongs
        # to a .rodata float-pool word instead. Our TU spelled the string with
        # the pool label, so rename the defined symbol to retail's anon form
        # (source can never spell an @N directly).
        exact_renames=(
            ("lbl_80518668", "@7006"),
        ),
    ),
    "HBMAxSound.o": UnitRules(
        # homebutton dissolve: the 28 midi-filename strings ship as
        # unspellable @N objects; retail names them lbl_8054C528..C7E8 in
        # pointer-table order. Also: retail .rodata carries a 4-byte align
        # tail (0x34 -> 0x38) and its .bss is half our size.
        data_pool_patterns=(
            (".data", b'midi\\00_home_button.mid\x00', "lbl_8054C528"),
            (".data", b'midi\\01_return_app.mid\x00', "lbl_8054C540"),
            (".data", b'midi\\02_goto_menu.mid\x00', "lbl_8054C558"),
            (".data", b'midi\\03_reset_app.mid\x00', "lbl_8054C570"),
            (".data", b'midi\\04_focus.mid\x00', "lbl_8054C588"),
            (".data", b'midi\\05_select.mid\x00', "lbl_8054C59C"),
            (".data", b'midi\\06_cancel.mid\x00', "lbl_8054C5B0"),
            (".data", b'midi\\07_open_controller.mid\x00', "lbl_8054C5C4"),
            (".data", b'midi\\08_close_controller.mid\x00', "lbl_8054C5E0"),
            (".data", b'midi\\09_volume_plus.mid\x00', "lbl_8054C600"),
            (".data", b'midi\\10_volume_minus.mid\x00', "lbl_8054C618"),
            (".data", b'midi\\11_volume_plus_limit.mid\x00', "lbl_8054C634"),
            (".data", b'midi\\12_volume_minus_limit.mid\x00', "lbl_8054C654"),
            (".data", b'midi\\13_nothing_done.mid\x00', "lbl_8054C674"),
            (".data", b'midi\\14_vibe_on.mid\x00', "lbl_8054C690"),
            (".data", b'midi\\15_vibe_off.mid\x00', "lbl_8054C6A4"),
            (".data", b'midi\\16_start_connect_window.mid\x00', "lbl_8054C6BC"),
            (".data", b'midi\\17_connected1.mid\x00', "lbl_8054C6E0"),
            (".data", b'midi\\18_connected2.mid\x00', "lbl_8054C6F8"),
            (".data", b'midi\\19_connected3.mid\x00', "lbl_8054C710"),
            (".data", b'midi\\20_connected4.mid\x00', "lbl_8054C728"),
            (".data", b'midi\\21_end_connect_window.mid\x00', "lbl_8054C740"),
            (".data", b'midi\\22_manual_open.mid\x00', "lbl_8054C760"),
            (".data", b'midi\\23_manual_focus.mid\x00', "lbl_8054C778"),
            (".data", b'midi\\24_manual_select.mid\x00', "lbl_8054C794"),
            (".data", b'midi\\25_manual_scroll.mid\x00', "lbl_8054C7B0"),
            (".data", b'midi\\26_manual_cancel.mid\x00', "lbl_8054C7CC"),
            (".data", b'midi\\27_manual_return_app.mid\x00', "lbl_8054C7E8"),
        ),
        pad_data_section=((".rodata", 0x38),),
        drop_nobits_range=((".bss", 4, 8),),
    ),
    "HBMGUIManager.o": UnitRules(
        # homebutton gui RTTI convention: flat symbol names + named
        # typestr/hierarchy objects instead of MWCC mangled/anon forms.
        exact_renames=(
            ("__RTTI__Q310homebutton3gui13PaneComponent", "__RTTI__homebutton_gui_PaneComponent"),
            ("__RTTI__Q310homebutton3gui9Interface", "__RTTI__homebutton_gui_Interface"),
            ("__RTTI__Q310homebutton3gui9Component", "__RTTI__homebutton_gui_Component"),
            ("__RTTI__Q310homebutton3gui11PaneManager", "__RTTI__homebutton_gui_PaneManager"),
            ("__RTTI__Q310homebutton3gui7Manager", "__RTTI__homebutton_gui_Manager"),
            ("@8080", "homebutton_gui_PaneComponent_hierarchy"),
            ("@8082", "homebutton_gui_PaneManager_hierarchy"),
            ("@8084", "homebutton_gui_Manager_hierarchy"),
            ("@8086", "homebutton_gui_Component_hierarchy"),
        ),
        data_pool_patterns=(
            (".data", b"homebutton::gui::PaneComponent\x00", "homebutton_gui_PaneComponent_typestr"),
            (".data", b"homebutton::gui::PaneManager\x00", "homebutton_gui_PaneManager_typestr"),
            (".data", b"homebutton::gui::Manager\x00", "homebutton_gui_Manager_typestr"),
            (".data", b"homebutton::gui::Component\x00", "homebutton_gui_Component_typestr"),
            (".data", b"homebutton::gui::Interface\x00", "homebutton_gui_Interface_typestr"),
        ),
    ),
    "HBMBase.o": UnitRules(
        # homebutton dissolve: the EventHandler typeinfo string ships as an
        # unspellable @N; retail names it lbl_8054D548.
        # ~150 anon data objects (typeinfo-name strings, vtable stubs,
        # jump-table targets) are numbered @NNNN by MWCC; retail names them
        # lbl_8054Cxxx/Dxxx split-local labels. Offset-paired rename table
        # generated from the gate's drift dump. @N numbering drifts with
        # source edits - regenerate rather than hand-patch.
        exact_renames=(
            ("@9533", "lbl_8054D548"),
            ("__RTTI__Q210homebutton22HomeButtonEventHandler", "lbl_8054D530"),
            ("__RTTI__Q310homebutton3gui12EventHandler", "lbl_8054D568"),
            ("@7739", "lbl_8054C924"),
            ("@7773", "lbl_8054CBF4"),
            ("@7774", "lbl_8054CC00"),
            ("@7775", "lbl_8054CC10"),
            ("@7776", "lbl_8054CC20"),
            ("@7777", "lbl_8054CC30"),
            ("@7778", "lbl_8054CC40"),
            ("@7779", "lbl_8054CC4C"),
            ("@7780", "lbl_8054CC5C"),
            ("@7781", "lbl_8054CC68"),
            ("@7782", "lbl_8054CC74"),
            ("@7783", "lbl_8054CC80"),
            ("@7784", "lbl_8054CC90"),
            ("@7785", "lbl_8054CC9C"),
            ("@7786", "lbl_8054CCAC"),
            ("@7787", "lbl_8054CCBC"),
            ("@7740", "lbl_8054C930"),
            ("@7741", "lbl_8054C93C"),
            ("@7788", "lbl_8054CD04"),
            ("@7789", "lbl_8054CD18"),
            ("@7742", "lbl_8054C948"),
            ("@7790", "lbl_8054CD2C"),
            ("@7791", "lbl_8054CD40"),
            ("@7792", "lbl_8054CD54"),
            ("@7793", "lbl_8054CD68"),
            ("@7794", "lbl_8054CD7C"),
            ("@7795", "lbl_8054CD94"),
            ("@7796", "lbl_8054CDA8"),
            ("@7797", "lbl_8054CDC0"),
            ("@7798", "lbl_8054CDD4"),
            ("@7799", "lbl_8054CDE8"),
            ("@7743", "lbl_8054C954"),
            ("@7800", "lbl_8054CE00"),
            ("@7801", "lbl_8054CE18"),
            ("@7802", "lbl_8054CE30"),
            ("@7803", "lbl_8054CE48"),
            ("@7804", "lbl_8054CE60"),
            ("@7805", "lbl_8054CE70"),
            ("@7806", "lbl_8054CE80"),
            ("@7807", "lbl_8054CE98"),
            ("@7808", "lbl_8054CEAC"),
            ("@7809", "lbl_8054CEC8"),
            ("@7744", "lbl_8054C970"),
            ("@7810", "lbl_8054CF30"),
            ("@7811", "lbl_8054CF3C"),
            ("@7812", "lbl_8054CF48"),
            ("@7813", "lbl_8054CF58"),
            ("@7745", "lbl_8054C97C"),
            ("@7814", "lbl_8054CF68"),
            ("@7815", "lbl_8054CF74"),
            ("@7816", "lbl_8054CF84"),
            ("@7817", "lbl_8054CF98"),
            ("@7818", "lbl_8054CFAC"),
            ("@7819", "lbl_8054CFC0"),
            ("@7820", "lbl_8054CFD4"),
            ("@7821", "lbl_8054CFE8"),
            ("@7822", "lbl_8054CFF8"),
            ("@7823", "lbl_8054D008"),
            ("@7746", "lbl_8054C988"),
            ("@7824", "lbl_8054D018"),
            ("@7825", "lbl_8054D028"),
            ("@7826", "lbl_8054D038"),
            ("@7827", "lbl_8054D050"),
            ("@7828", "lbl_8054D060"),
            ("@7829", "lbl_8054D070"),
            ("@7830", "lbl_8054D080"),
            ("@7831", "lbl_8054D090"),
            ("@7832", "lbl_8054D098"),
            ("@7833", "lbl_8054D0A0"),
            ("@7834", "lbl_8054D0A8"),
            ("@7835", "lbl_8054D0B0"),
            ("@7836", "lbl_8054D0B8"),
            ("@7837", "lbl_8054D0C0"),
            ("@7838", "lbl_8054D0C8"),
            ("@7839", "lbl_8054D0D0"),
            ("@7840", "lbl_8054D0D8"),
            ("@7841", "lbl_8054D0E0"),
            ("@7842", "lbl_8054D0E8"),
            ("@7843", "lbl_8054D0F0"),
            ("@7844", "lbl_8054D0F8"),
            ("@7845", "lbl_8054D18C"),
            ("@7846", "lbl_8054D198"),
            ("@7847", "lbl_8054D1A4"),
            ("@7848", "lbl_8054D1B0"),
            ("@7849", "lbl_8054D1BC"),
            ("@7850", "lbl_8054D1DC"),
            ("@7851", "lbl_8054D1E8"),
            ("@7852", "lbl_8054D1F4"),
            ("@7853", "lbl_8054D204"),
            ("@7854", "lbl_8054D214"),
            ("@7855", "lbl_8054D224"),
            ("@7856", "lbl_8054D234"),
            ("@7857", "lbl_8054D244"),
            ("@7858", "lbl_8054D24C"),
            ("@7859", "lbl_8054D254"),
            ("@7860", "lbl_8054D288"),
            ("@7861", "lbl_8054D294"),
            ("@7862", "lbl_8054D2A0"),
            ("@7863", "lbl_8054D2B8"),
            ("@7864", "lbl_8054D2C8"),
            ("@7865", "lbl_8054D2D8"),
            ("@7866", "lbl_8054D2E8"),
            ("@7867", "lbl_8054D2F8"),
            ("@7868", "lbl_8054D308"),
            ("@7869", "lbl_8054D318"),
            ("@7870", "lbl_8054D328"),
            ("@7871", "lbl_8054D338"),
            ("@7872", "lbl_8054D348"),
            ("@7873", "lbl_8054D358"),
            ("@7874", "lbl_8054D368"),
            ("@7875", "lbl_8054D378"),
            ("@7876", "lbl_8054D388"),
            ("@7877", "lbl_8054D398"),
            ("@7878", "lbl_8054D3A8"),
            ("@9531", "lbl_8054D500"),
            ("@9532", "lbl_8054D524"),
            ("@7747", "lbl_8054C9A8"),
            ("@7748", "lbl_8054C9B8"),
            ("@7749", "lbl_8054C9C8"),
            ("@7750", "lbl_8054C9D8"),
            ("@7751", "lbl_8054C9E8"),
            ("@7752", "lbl_8054C9F4"),
            ("@7753", "lbl_8054CA00"),
            ("@7754", "lbl_8054CA0C"),
            ("@7755", "lbl_8054CA38"),
            ("@7756", "lbl_8054CA4C"),
            ("@7757", "lbl_8054CA60"),
            ("@7734", "lbl_8054C8C8"),
            ("@7735", "lbl_8054C8D8"),
            ("@7736", "lbl_8054C8E8"),
            ("@7758", "lbl_8054CA80"),
            ("@7759", "lbl_8054CA94"),
            ("@7737", "lbl_8054C8F8"),
            ("@7760", "lbl_8054CAA8"),
            ("@7761", "lbl_8054CAC0"),
            ("@7762", "lbl_8054CAD8"),
            ("@7763", "lbl_8054CAF0"),
            ("@7764", "lbl_8054CB04"),
            ("@7765", "lbl_8054CB18"),
            ("@7766", "lbl_8054CB2C"),
            ("@7767", "lbl_8054CB40"),
            ("@7768", "lbl_8054CB50"),
            ("@7769", "lbl_8054CB68"),
            ("@7770", "lbl_8054CB7C"),
            ("@7771", "lbl_8054CB94"),
            ("@7772", "lbl_8054CBA8"),
            ("@7738", "lbl_8054C918"),
        ),
        # The original tail-rotation swap chain is OBSOLETE: the natural
        # object's .data already byte-matches retail (verified 0x00-diff over
        # the full section), and the stale swaps were scattering the
        # "homebutton::HomeButtonEventHandler" typeinfo string.
    ),
    # NOTE: do NOT add a second "CGXCache.o" UnitRules entry — duplicate dict keys
    # silently shadow the real pool rule above (line ~475) and regress every
    # pool-coupled unit. CGXCache .data addend_patches live in the main rule.
    "snd_WsdPlayer.o": UnitRules(
        # Retail split carries NO data: WsdPlayer vtable ships from
        # nw4r_data.s @lbl_eu_8056ADA8 (referenced by __ct/__dt); the float
        # pools live at the labels this unit's split asm actually references.
        pool_patterns=(
            (struct.pack(">I", 0x3F800000), "lbl_eu_8066A108"),
            (struct.pack(">I", 0x00000000), "lbl_eu_8066A10C"),
            (struct.pack(">I", 0x427C0000), "lbl_eu_8066A110"),
            (struct.pack(">I", 0x3C800000), "lbl_eu_8066A114"),
            (struct.pack(">I", 0x42FE0000), "lbl_eu_8066A118"),
            (struct.pack(">II", 0x43300000, 0x80000000), "lbl_eu_8066A120"),
            (struct.pack(">II", 0x43300000, 0x00000000), "lbl_eu_8066A128"),
        ),
        exact_renames=(("__vt__Q44nw4r3snd6detail9WsdPlayer", "lbl_eu_8056ADA8"),),
        extern_data_sections=(".sdata2", ".data"),
    ),
    "snd_SoundArchive.o": UnitRules(
        # Retail split carries NO data: the SoundArchive vtable ships from
        # nw4r_data.s @lbl_eu_8056ABC0 (loaded by __ct via lis/addi).
        exact_renames=(("__vt__Q34nw4r3snd12SoundArchive", "lbl_eu_8056ABC0"),),
        extern_data_sections=(".data",),
    ),
    "snd_StrmPlayer.o": UnitRules(
        # Retail split carries NO data: StrmPlayer vtable @lbl_eu_8056AC78
        # and its two load-task vtables (@lbl_eu_8056ACC0 / @lbl_eu_8056ACD8)
        # ship from nw4r_data.s. The 0x4020 sLoadBuffer bss static is already
        # wired to the blob labels in source (lbl_eu_8064FE00 / lbl_eu_80653E00);
        # the TU-local copy and the lbl_eu_80665518 sbss flag are stripped so
        # they resolve to the shared blob at link.
        exact_renames=(
            ("__vt__Q44nw4r3snd6detail10StrmPlayer", "lbl_eu_8056AC78"),
            ("__vt__Q54nw4r3snd6detail10StrmPlayer16StrmDataLoadTask", "lbl_eu_8056ACC0"),
            ("__vt__Q54nw4r3snd6detail10StrmPlayer18StrmHeaderLoadTask", "lbl_eu_8056ACD8"),
        ),
        extern_data_sections=(".data", ".bss", ".sbss"),
    ),
    "snd_StrmSound.o": UnitRules(
        # Retail split carries NO data: StrmSound vtable @lbl_eu_8056ACF0
        # ships from nw4r_data.s; the GetRuntimeTypeInfo cache pointer is
        # already source-named lbl_eu_80665520 and resolves to the blob.
        exact_renames=(("__vt__Q44nw4r3snd6detail9StrmSound", "lbl_eu_8056ACF0"),),
        pool_patterns=(
            # The TU pools the 128.0f/-0.0f double locally (@9580); retail
            # lfd's it from lbl_eu_8066A090 (.double 4503601774854144,
            # retail snd_StrmSound.s @0x80427540 and @0x804277D0).
            (struct.pack(">II", 0x43300000, 0x80000000), "lbl_eu_8066A090"),
        ),
        extern_data_sections=(".data", ".sbss", ".sdata2"),
    ),
    "snd_Task.o": UnitRules(
        # Retail split carries NO data: detail::Task vtable ships from
        # nw4r_data.s; this TU's code never references it locally.
        extern_data_sections=(".data",),
    ),
    "snd_VoiceManager.o": UnitRules(
        # Retail GetInstance uses blob statics: instance @lbl_eu_80653E6C
        # (0x1c), guard byte @lbl_eu_80665530, and the 0xc array
        # @lbl_eu_80653E60. Strip .bss/.sbss; $N static numbering drifts,
        # hence prefix renames.
        prefix_renames=(
            ("@GUARD@instance$", "lbl_eu_80665530"),
            ("instance$", "lbl_eu_80653E6C"),
        ),
        exact_renames=(
            ("@8923", "lbl_eu_80653E60"),
        ),
        extern_data_sections=(".bss", ".sbss"),
    ),
    "snd_WaveSound.o": UnitRules(
        # Retail split carries NO data: WaveSound vtable @lbl_eu_8056AD70
        # ships from nw4r_data.s; the GetRuntimeTypeInfo cache pointer is
        # already source-named lbl_eu_80665538.
        exact_renames=(("__vt__Q44nw4r3snd6detail9WaveSound", "lbl_eu_8056AD70"),),
        extern_data_sections=(".data", ".sbss"),
    ),
    "snd_Voice.o": UnitRules(
        # Retail split carries NO data: Voice vtable @lbl_eu_8056AD4C and the
        # CalcMixParam switch jump table @jumptable_eu_8056AD28 ship from
        # nw4r_data.s; float pools at the labels this unit's split asm refs.
        pool_patterns=(
            (struct.pack(">I", 0x3F800000), "lbl_eu_8066A098"),
            (struct.pack(">I", 0x00000000), "lbl_eu_8066A09C"),
            (struct.pack(">I", 0xBDF5C28F), "lbl_eu_8066A0A0"),
            (struct.pack(">I", 0x3F6147AE), "lbl_eu_8066A0A4"),
            (struct.pack(">I", 0x3F8F5C29), "lbl_eu_8066A0AC"),
            (struct.pack(">I", 0x3F59999A), "lbl_eu_8066A0B0"),
            (struct.pack(">I", 0x3E199998), "lbl_eu_8066A0B4"),
            (struct.pack(">I", 0x40000000), "lbl_eu_8066A0B8"),
            (struct.pack(">I", 0x3EB33334), "lbl_eu_8066A0BC"),
        ),
        exact_renames=(
            ("__vt__Q44nw4r3snd6detail5Voice", "lbl_eu_8056AD4C"),
            # CalcMixParam switch table (MWCC @N numbering drifts; only .data
            # pool symbol in the TU).
            ("@12176", "jumptable_eu_8056AD28"),
        ),
        extern_data_sections=(".sdata2", ".data"),
    ),
    "snd_SoundArchivePlayer.o": UnitRules(
        # Retail split carries NO data: the five vtables referenced by this
        # TU's code (SoundArchivePlayer @lbl_eu_8056ABE0, SeqNoteOnCallback
        # @lbl_eu_8056AC20, and MmlParser @lbl_eu_8056AAB0 whose CommandProc
        # lives in this slice) ship from nw4r_data.s; the other MWCC-emitted
        # vtables (base interfaces) are unreferenced here and die with the
        # stripped .data.
        exact_renames=(
            ("__vt__Q34nw4r3snd18SoundArchivePlayer", "lbl_eu_8056ABE0"),
            ("__vt__Q44nw4r3snd18SoundArchivePlayer17SeqNoteOnCallback", "lbl_eu_8056AC20"),
            ("__vt__Q44nw4r3snd6detail9MmlParser", "lbl_eu_8056AAB0"),
            # Ctor member vptr stores: retail references the blob copies under
            # these labels (2026-08 drift on __ct__ / NoteOn sites).
            ("__vt__Q44nw4r3snd18SoundArchivePlayer11WsdCallback", "lbl_eu_8056AC10"),
            ("__vt__Q44nw4r3snd6detail20MmlSeqTrackAllocator", "lbl_eu_8056AAD0"),
        ),
        pool_patterns=(
            (struct.pack(">II", 0x43300000, 0x80000000), "lbl_eu_8066A050"),
            (struct.pack(">I", 0x42FE0000), "lbl_eu_8066A048"),
        ),
        extern_data_sections=(".sdata2", ".data"),
    ),
    "snd_SeqTrack.o": UnitRules(
        # Retail split carries NO data: SeqTrack vtable @lbl_eu_8056ABB0 and
        # the ChannelCallback dispatch struct @lbl_eu_8051FF68 ship from
        # nw4r_data.s; float pools at the labels this unit's split asm refs.
        pool_patterns=(
            (struct.pack(">I", 0x3F800000), "lbl_eu_8066A018"),
            (struct.pack(">I", 0x42FE0000), "lbl_eu_8066A020"),
            (struct.pack(">I", 0x3C000000), "lbl_eu_8066A024"),
            (struct.pack(">I", 0x00000000), "lbl_eu_8066A01C"),
            (struct.pack(">I", 0x427C0000), "lbl_eu_8066A028"),
            (struct.pack(">I", 0xBF800000), "lbl_eu_8066A02C"),
            (struct.pack(">I", 0x40000000), "lbl_eu_8066A030"),
            (struct.pack(">II", 0x43300000, 0x00000000), "lbl_eu_8066A038"),
            (struct.pack(">II", 0x43300000, 0x80000000), "lbl_eu_8066A040"),
        ),
        exact_renames=(("__vt__Q44nw4r3snd6detail8SeqTrack", "lbl_eu_8056ABB0"),),
        data_pool_patterns=(
            # ChannelCallback dispatch struct: sole .rodata pool symbol,
            # content is all-zero file bytes (pointers are relocs).
            (".rodata", struct.pack(">II", 0, 0), "lbl_eu_8051FF68"),
        ),
        extern_data_sections=(".sdata2", ".data", ".rodata"),
    ),
    "snd_SeqPlayer.o": UnitRules(
        # .sdata2 float/double pool -> shared nw4r_data.s pool by content.
        pool_patterns=(
            (struct.pack(">I", 0x00000000), "lbl_eu_80669A68"),
            (struct.pack(">I", 0x3F800000), "lbl_eu_80669A6C"),
            (struct.pack(">I", 0x476A6000), "lbl_eu_8066A010"),
        ),
        # Retail split carries NO data/bss/sbss (ground truth = retail
        # snd_SeqPlayer.s): the ctor stores lbl_eu_8056AAF8 = SeqPlayer
        # vtable, and GetVariablePtr/SetGlobalVariable/InitSeqPlayer address
        # the global-variable array at lbl_eu_806382C0. The lazy-init flag
        # and PMF statics were removed from the source instead (retail GC'd
        # them; see NOTE in snd_SeqPlayer.cpp).
        exact_renames=(
            ("mGlobalVariable__Q44nw4r3snd6detail9SeqPlayer", "lbl_eu_806382C0"),
            ("__vt__Q44nw4r3snd6detail9SeqPlayer", "lbl_eu_8056AAF8"),
        ),
        extern_data_sections=(".sdata2", ".data", ".bss", ".sbss"),
    ),
    "snd_MmlParser.o": UnitRules(
        # .sdata2 float/double pool -> shared nw4r_data.s pool by content.
        pool_patterns=(
            (struct.pack(">I", 0x00000000), "lbl_eu_80669A68"),
            (struct.pack(">II", 0x43300000, 0x00000000), "lbl_eu_80669A78"),
            (struct.pack(">I", 0x3C000000), "lbl_eu_80669F54"),
            (struct.pack(">I", 0x3EC80000), "lbl_eu_80669FE4"),
        ),
        # Retail split carries NO sdata/data/sbss either (ground truth =
        # retail snd_MmlParser.s): CommandProc's printvar debug tables live in
        # nw4r_data.s - "T"@lbl_eu_806634D8 / "G"@lbl_eu_806634DC /
        # ""@lbl_eu_806634E0 (.sdata li refs), format string
        # lbl_eu_8056A890, command jumptable jumptable_eu_8056A8B8, and the
        # mPrintVarEnabledFlag byte lbl_eu_806654E8 (lbz/stb @sda21).
        exact_renames=(
            ("@9737", "lbl_eu_806634D8"),
            ("@9738", "lbl_eu_806634DC"),
            ("@9739", "lbl_eu_806634E0"),
            ("@9740", "lbl_eu_8056A890"),
            ("@9742", "jumptable_eu_8056A8B8"),
            ("mPrintVarEnabledFlag__Q44nw4r3snd6detail9MmlParser", "lbl_eu_806654E8"),
        ),
        extern_data_sections=(".sdata2", ".sdata", ".data", ".sbss"),
    ),
    "snd_FxDelayDpl2.o": UnitRules(
        pool_patterns=(
            # Plain SetParam clamp literals pooled by value; retail owns
            # dedicated sdata2 objects under these names (lbl_eu_80669F8C/F90
            # are referenced from source directly, see snd_FxDelayDpl2.cpp).
            (struct.pack(">I", 0x00000000), "lbl_eu_80669F94"),
            (struct.pack(">I", 0x3F800000), "lbl_eu_80669F88"),
        ),
        extern_data_sections=(".sdata2", ".data"),
        # .data: weak local __vt__FxBase copy is unreferenced; the ctor stores
        # the nw4r_data.s blob object lbl_eu_8056A7C0 directly. Strip it.
        # Retail GC'd the same orphan set as FxChorusDpl2 (0xD0 total): the
        # unnamed NonCopyable dtor, weak __dt__FxBase / __dt__LinkListNode and
        # the 4/4/8-byte FxBase UpdateBuffer/Shutdown/StartUp stubs (the
        # stripped vtable was their only referencer). Dropping restores the
        # retail 0x600 budget exactly; survivors are sub-16-packed like retail,
        # so no repack.
        drop_text_symbols=(
            "__dt__Q44nw4r2ut29@unnamed@snd_FxDelayDpl2_cpp@11NonCopyableFv",
            "__dt__Q34nw4r3snd6FxBaseFv",
            "__dt__Q34nw4r2ut12LinkListNodeFv",
            "UpdateBuffer__Q34nw4r3snd6FxBaseFiPPvUlQ34nw4r3snd12SampleFormatfQ34nw4r3snd10OutputMode",
            "Shutdown__Q34nw4r3snd6FxBaseFv",
            "StartUp__Q34nw4r3snd6FxBaseFv",
        ),
    ),
    "snd_Channel.o": UnitRules(
        # .sdata2 float pool -> shared nw4r_data.s pool by content.
        pool_patterns=(
            (struct.pack(">I", 0x00000000), "lbl_eu_80669A68"),
            (struct.pack(">I", 0x3F800000), "lbl_eu_80669A6C"),
            (struct.pack(">II", 0x43300000, 0x00000000), "lbl_eu_80669A78"),
            (struct.pack(">I", 0x437F0000), "lbl_eu_80669C20"),
            (struct.pack(">II", 0x43800000, 0x00000000), "lbl_eu_80669DC8"),
            (struct.pack(">I", 0x40C00000), "lbl_eu_80669F14"),
        ),
        # Retail split carries NO bss/sbss: ChannelManager singleton + guard
        # live in nw4r_data.s (ground truth = retail snd_Channel.s GetInstance:
        # lbz/stb lbl_eu_806654D8 guard byte; construct lbl_eu_80637FB0
        # (instance) with __register_global_object node lbl_eu_80637FA0).
        exact_renames=(
            ("@8943", "lbl_eu_80637FA0"),
            ("instance$8940", "lbl_eu_80637FB0"),
        ),
        prefix_renames=(
            ("@GUARD@instance$", "lbl_eu_806654D8"),
        ),
        extern_data_sections=(".sdata2", ".bss", ".sbss"),
    ),
    "snd_SeqSound.o": UnitRules(
        # Retail split carries NO data (ground truth = retail snd_SeqSound.s:
        # the ctor stores lbl_eu_8056AB60 = SeqSound vtable and
        # lbl_eu_8056AB98 = SeqLoadTask vtable via lis/addi; the local .sbss
        # word is already named for its retail home lbl_eu_806654F8).
        exact_renames=(
            ("__vt__Q44nw4r3snd6detail8SeqSound", "lbl_eu_8056AB60"),
            ("__vt__Q54nw4r3snd6detail8SeqSound11SeqLoadTask", "lbl_eu_8056AB98"),
        ),
        extern_data_sections=(".data", ".sbss"),
    ),
    "snd_Bank.o": UnitRules(
        pool_patterns=(
            (struct.pack(">I", 0x00000000), "lbl_eu_80669A68"),
            (struct.pack(">II", 0x43300000, 0x00000000), "lbl_eu_80669A78"),
            (struct.pack(">I", 0x42FE0000), "lbl_eu_80669EB8"),
            (struct.pack(">I", 0x427C0000), "lbl_eu_80669EBC"),
        ),
        exact_renames=(
            # GetAnmResult's inlined-detail 0.0f pools to the A68 copy, but
            # retail loads lbl_eu_80669E78 at that site (content-equal dup;
            # no other site in this TU references A68).
            ("lbl_eu_80669A68", "lbl_eu_80669E78"),
        ),
        extern_data_sections=(".sdata2",),
    ),
    "snd_AxVoice.o": UnitRules(
        pool_patterns=(
            (struct.pack(">I", 0x00000000), "lbl_eu_80669A68"),
            (struct.pack(">I", 0x3F800000), "lbl_eu_80669A6C"),
            (struct.pack(">I", 0x3FAAAAAB), "lbl_eu_80669C68"),
            (struct.pack(">I", 0x47800000), "lbl_eu_80669E20"),
            (struct.pack(">I", 0x46FA0000), "lbl_eu_80669E8C"),
            (struct.pack(">I", 0x46FFFE00), "lbl_eu_80669EB4"),
        ),
        extern_data_sections=(".sdata2",),
    ),
    "snd_AxManager.o": UnitRules(
        # .sdata2 float/double pool -> shared nw4r_data.s pool by content.
        pool_patterns=(
            (struct.pack(">I", 0x00000000), "lbl_eu_80669A68"),
            (struct.pack(">I", 0x3F800000), "lbl_eu_80669A6C"),
            (struct.pack(">I", 0x46FA0000), "lbl_eu_80669E8C"),
        ),
        # Retail split carries NO bss/sbss: the singleton + its guard live in
        # nw4r_data.s. Ground truth = retail snd_AxManager.s refs:
        #   GetInstance/AxCallbackFunc lbz/stb lbl_eu_806654BC (guard byte,
        #   inlined into every caller), construct lbl_eu_80637E68 (instance)
        #   with __register_global_object node lbl_eu_80637E5C; Init memsets
        #   lbl_eu_80637B20 (sZeroBuffer, 0x100) + DCFlushRange.
        exact_renames=(
            ("sZeroBuffer__Q44nw4r3snd6detail9AxManager", "lbl_eu_80637B20"),
            ("@9183", "lbl_eu_80637E5C"),
            ("instance$9180", "lbl_eu_80637E68"),
            # u32->f64 magic double pooled locally (@N drifts with TU growth);
            # retail loads it from nw4r_data.s lbl_eu_80669E80.
            ("@10283", "lbl_eu_80669E80"),
            # AiDmaCallbackFunc's static bool has no retail counterpart (the
            # retail linker GC'd PrepareReset/AiDmaCallbackFunc entirely - no
            # AIRegisterDMACallback/AXSetMaxDspCycles caller survives). Park it
            # in the hidden pad byte right after this TU's own guard flag so
            # runtime behavior stays that of a private static.
            ("finishedFlag$9563", "gap_10_806654BD_sbss"),
            # AppendEffect's ratio init pools the extern 1.0f into a local
            # literal entry; content-mapping labels it lbl_eu_80669A6C but
            # retail loads lbl_eu_80669E7C there (source refs E7C; nothing
            # else in this TU legitimately lands on A6C).
            ("lbl_eu_80669A6C", "lbl_eu_80669E7C"),
        ),
        prefix_renames=(
            ("@GUARD@instance$", "lbl_eu_806654BC"),
        ),
        extern_data_sections=(".sdata2", ".bss", ".sbss"),
    ),
    "snd_AxVoiceManager.o": UnitRules(
        # Retail split carries NO bss/sbss: singleton + guard live in
        # nw4r_data.s (ground truth = retail snd_AxVoiceManager.s refs:
        # GetInstance/AxCallbackFunc lbz/stb lbl_eu_806654C0 guard byte;
        # construct lbl_eu_80637F74 (instance) with __register_global_object
        # node lbl_eu_80637F68).
        exact_renames=(
            ("@8925", "lbl_eu_80637F68"),
            ("instance$8922", "lbl_eu_80637F74"),
        ),
        prefix_renames=(
            ("@GUARD@instance$", "lbl_eu_806654C0"),
        ),
        extern_data_sections=(".bss", ".sbss"),
    ),
    "snd_AxfxImpl.o": UnitRules(
        # Retail split carries NO sbss: mCurrentFx/mAllocatedSize live in
        # nw4r_data.s @lbl_eu_806654C8/806654CC - the source already refs them
        # by name (see the extern decls in snd_AxfxImpl.cpp); just strip the
        # TU-local storage.
        extern_data_sections=(".sbss",),
    ),
    "snd_BasicPlayer.o": UnitRules(
        # Retail split carries NO data: the BasicPlayer vtable ships from
        # nw4r_data.s @lbl_eu_8056A6E8 (stored by __ct via lis/addi).
        exact_renames=(
            ("__vt__Q44nw4r3snd6detail11BasicPlayer", "lbl_eu_8056A6E8"),
        ),
        extern_data_sections=(".data",),
    ),
    "lyt_material.o": UnitRules(
        # Reloc-name drift on byte-identical .data: retail names the Material
        # typeinfo-name string lbl_8054D8B0. @N numbering drifts with source
        # edits. (Also: the AnimResource Find/SetAnimationEnable overloads
        # were made non-virtual in the hbm header to match retail's 0x28
        # vtable.)
        exact_renames=(("@5955", "lbl_8054D8B0"),),
        pool_patterns=(
            (struct.pack(">I", 0x00000000), "lbl_eu_80669A68"),
            (struct.pack(">I", 0x3F800000), "lbl_eu_80669A6C"),
            (struct.pack(">II", 0x43300000, 0x00000000), "lbl_eu_80669A78"),
            (struct.pack(">I", 0x3F000000), "lbl_eu_80669A84"),
            (struct.pack(">I", 0x3F360B61), "lbl_eu_80669AA4"),
        ),
        extern_data_sections=(".sdata2",),
    ),
    "g3d_resnode.o": UnitRules(
        pool_patterns=(
            # CalcChrAnmResult pools its own 1.0f; retail loads it from
            # lbl_eu_80669AA0 (fn+0x38 site). PatchChrAnmResult references
            # AA0/AA4/AA8 as source-level externs and needs no rename.
            (struct.pack(">I", 0x3F800000), "lbl_eu_80669AA0"),
            (struct.pack(">I", 0x3F360B61), "lbl_eu_80669AA4"),
        ),
        extern_data_sections=(".sdata2",),
    ),
    "g3d_resmat.o": UnitRules(
        pool_patterns=(
            (struct.pack(">I", 0x00000000), "lbl_eu_80669A68"),
            (struct.pack(">I", 0x3F800000), "lbl_eu_80669A6C"),
            (struct.pack(">II", 0x43300000, 0x00000000), "lbl_eu_80669A78"),
            (struct.pack(">I", 0x40000000), "lbl_eu_80669A80"),
            (struct.pack(">I", 0x3F000000), "lbl_eu_80669A84"),
            (struct.pack(">I", 0x3A800000), "lbl_eu_80669A88"),
            (struct.pack(">II", 0x43300000, 0x80000000), "lbl_eu_80669A90"),
            (struct.pack(">II", 0x44800000, 0x00000000), "lbl_eu_80669A98"),
        ),
        extern_data_sections=(".sdata2",),
    ),
    "g3d_resanmfog.o": UnitRules(
        pool_patterns=(
            (struct.pack(">I", 0x00000000), "lbl_eu_80669A68"),
            (struct.pack(">II", 0x43300000, 0x00000000), "lbl_eu_80669A78"),
        ),
        extern_data_sections=(".sdata2",),
    ),
    "g3d_state.o": UnitRules(
        # Retail split is .text-only: G3DState's statics live in nw4r_data.s
        # (.data blob anchored at lbl_eu_8061A520 — retail __sinit loads its
        # base via lis/addi). Retarget __sinit's two ...bss.0 (= local
        # g_CameraMtxState base) relocs to that anchor, rename the IndMtxOpStd
        # vtable to lbl_eu_805695D0, and map this unit's u16->f32 magic double
        # to its own split slot lbl_eu_80669C08; strip all data sections.
        retarget_relocs=(
            (".text", 0x26B2, "lbl_eu_8061A520"),
            (".text", 0x26B6, "lbl_eu_8061A520"),
        ),
        exact_renames=(("__vt__Q44nw4r3g3d8G3DState11IndMtxOpStd", "lbl_eu_805695D0"),),
        pool_patterns=(
            (struct.pack(">II", 0x43300000, 0x00000000), "lbl_eu_80669C08"),
        ),
        extern_data_sections=(".data", ".sdata2", ".bss"),
    ),
    "g3d_scnmdlsmpl.o": UnitRules(
        # Retail split is .text-only: ScnMdlSimple vtable ships from
        # nw4r_data.s @lbl_eu_80569758 and the ResName lookup strings
        # "NodeTree"/"NodeMix"/"DrawOpa"/"DrawXlu" from lbl_eu_8056974C /
        # lbl_eu_80663490/98/A0 (.sdata); this unit's 1.0f pool slot is
        # lbl_eu_80669CE8 ({1.0f, 0}). Rename + strip.
        data_pool_patterns=(
            (".data", b"NodeTree\x00", "lbl_eu_8056974C"),
            (".sdata", b"NodeMix\x00", "lbl_eu_80663490"),
            (".sdata", b"DrawOpa\x00", "lbl_eu_80663498"),
            (".sdata", b"DrawXlu\x00", "lbl_eu_806634A0"),
        ),
        exact_renames=(("__vt__Q34nw4r3g3d12ScnMdlSimple", "lbl_eu_80569758"),),
        pool_patterns=(
            (struct.pack(">I", 0x3F800000), "lbl_eu_80669CE8"),
        ),
        extern_data_sections=(".data", ".rodata", ".sdata", ".sdata2"),
    ),
    "g3d_light.o": UnitRules(
        # 0.0/1.0/0.5 must land on this unit's own blob slots D00/D04/D08:
        # every retail sda21 float load here uses the D0x range (never the
        # shared A68/A6C/A84 slots).
        # The LightSetting ctor's white-color byte pools are MWCC cookies
        # (@9841/@9843) that pool_patterns cannot key (byte runs of 0xFF also
        # match the -1.0f slot); retail names them D0C..D0F / D10..D13.
        # -1.0f/90.0f land on BF0/B04 via content but retail loads them from
        # its own D18/D1C slots — retarget those too.
        # OPEN ITEM (LightSetting ctor, 6 residual name-drifts): retail loads
        # the two white colors via four 1-byte blob symbols each
        # (lbl_eu_80669D0C..D0F / D10..D13); our single pooled cookie carries
        # CORRECT addends 0..3 per lane but is one symbol, so objdiff counts
        # the non-base lanes as name drifts. Needs a symbol-split postprocess
        # primitive (split slot sym at +0/+1/+2/+3) before this can hit 100%.
        # Do NOT retarget_relocs here: all four lane relocs share one symbol,
        # so successive retargets just re-rename the same entry.
        exact_renames=(
            ("@9841", "lbl_eu_80669D0C"),
            ("@9843", "lbl_eu_80669D10"),
            ("lbl_eu_80669BF0", "lbl_eu_80669D18"),
            ("lbl_eu_80669B04", "lbl_eu_80669D1C"),
        ),
        pool_patterns=(
            (struct.pack(">I", 0x00000000), "lbl_eu_80669D00"),
            (struct.pack(">I", 0x3F800000), "lbl_eu_80669D04"),
            (struct.pack(">I", 0x3F000000), "lbl_eu_80669D08"),
            (struct.pack(">I", 0x42B40000), "lbl_eu_80669B04"),
            (struct.pack(">I", 0xBF800000), "lbl_eu_80669BF0"),
            (struct.pack(">I", 0x4A742400), "lbl_eu_80669D14"),
            (struct.pack(">I", 0x41200000), "lbl_eu_80669D20"),
            (struct.pack(">I", 0x41000000), "lbl_eu_80669D24"),
            (struct.pack(">I", 0xC0E00000), "lbl_eu_80669D28"),
        ),
        extern_data_sections=(".sdata2",),
    ),
    "g3d_draw1mat1shp.o": UnitRules(
        # Retail split is .text-only: the sChanIDs channel table ships from
        # nw4r_data.s @lbl_eu_8051D690 ({GX_COLOR0, GX_ALPHA0, GX_COLOR1,
        # GX_ALPHA1} — source fixed to retail order); this unit's pool slots
        # are lbl_eu_80669C10 (1.0f) / C20 (255.0f) / C28 (2^52 double).
        # $N static numbering drifts -> prefix rename.
        prefix_renames=(("sChanIDs$", "lbl_eu_8051D690"),),
        pool_patterns=(
            (struct.pack(">I", 0x3F800000), "lbl_eu_80669C10"),
            (struct.pack(">II", 0x43300000, 0x00000000), "lbl_eu_80669C28"),
            (struct.pack(">I", 0x437F0000), "lbl_eu_80669C20"),
        ),
        extern_data_sections=(".rodata", ".sdata2"),
    ),
    "g3d_draw.o": UnitRules(
        pool_patterns=(
            (struct.pack(">I", 0x3F800000), "lbl_eu_80669A6C"),
        ),
        # The static draw-state word is already retail-named
        # (lbl_eu_8066347C) but owned by nw4r_data.s .sdata; strip the
        # TU-local .sbss copy so the sda21 relocs resolve to it at link.
        extern_data_sections=(".sdata2", ".sbss"),
    ),
    "g3d_calcvtx.o": UnitRules(
        pool_patterns=(
            (struct.pack(">I", 0x00000000), "lbl_eu_80669A68"),
        ),
        extern_data_sections=(".sdata2",),
    ),
    "g3d_calcview.o": UnitRules(
        # Retail split is .text-only: the local billboard dispatch table is
        # renamed to nw4r_data.s' lbl_eu_8051D6A0 and stripped (keeps the
        # Calc_BILLBOARD_* kernels emitted for the blob table to reference);
        # the LC region table was removed from source in favor of a direct
        # 0xE0001000 literal (pooling it left an unresolvable local rodata
        # copy retail does not have). Pool slots are this unit's own
        # lbl_eu_80669C30 (0.0f) / 80669C34 (1e-18f).
        pool_patterns=(
            (struct.pack(">I", 0x00000000), "lbl_eu_80669C30"),
            (struct.pack(">I", 0x219392EF), "lbl_eu_80669C34"),
        ),
        exact_renames=(
            # Local billboard dispatch table -> retail pool copy (nw4r_data.s).
            (
                "gCalcBillboardFuncTable__Q34nw4r3g3d26@unnamed@g3d_calcview_cpp@",
                "lbl_eu_8051D6A0",
            ),
        ),
        extern_data_sections=(".rodata", ".sdata2"),
    ),
    "g3d_anmtexsrt.o": UnitRules(
        pool_patterns=(
            (struct.pack(">I", 0x00000000), "lbl_eu_80669A68"),
            (struct.pack(">I", 0x3F800000), "lbl_eu_80669A6C"),
            (struct.pack(">II", 0x43300000, 0x00000000), "lbl_eu_80669A78"),
            (struct.pack(">II", 0x3FF00000, 0x00000000), "lbl_eu_80669B80"),
        ),
        # Same shape as g3d_anmclr/anmtexpat: vtables ship from nw4r_data.s
        # (lbl_eu_805692A0/lbl_eu_805692E8); ResName strings are
        # lbl_eu_8051D590/8051D5A8 (already retail-named in the object).
        exact_renames=(
            ("__vt__Q34nw4r3g3d15AnmObjTexSrtRes", "lbl_eu_805692A0"),
            ("__vt__Q34nw4r3g3d12AnmObjTexSrt", "lbl_eu_805692E8"),
        ),
        extern_data_sections=(".sdata2", ".data", ".rodata"),
    ),
    "g3d_anmscn.o": UnitRules(
        pool_patterns=(
            # Retail g3d_anmscn split asm references only BA0 (=0.0f), BA4,
            # BA8 (=1.0f), BB0, BB8 - never the A68/A6C slots this rule used to
            # target (MakeDiffuseLightObj drift, 2026-08; contents verified in
            # nw4r_data.s).
            (struct.pack(">I", 0x00000000), "lbl_eu_80669BA0"),
            (struct.pack(">I", 0x3F800000), "lbl_eu_80669BA8"),
            (struct.pack(">I", 0xD01502F9), "lbl_eu_80669BA4"),
        ),
        # Retail split is .text-only besides sda refs: GetAnmPlayPolicy's
        # static policyTable ships from nw4r_data.s as two ADJACENT .sdata
        # objects (lbl_eu_80663458 func-ptrs + lbl_eu_80663460 floats), and
        # the AnmScnRes/AnmScn vtables are lbl_eu_80569490/80569520; ResName
        # strings lbl_eu_8051D620/8051D630 are already retail-named.
        exact_renames=(
            (
                "@LOCAL@GetAnmPlayPolicy__Q24nw4r3g3dFQ34nw4r3g3d9AnmPolicy@policyTable",
                "lbl_eu_80663458",
            ),
            ("__vt__Q34nw4r3g3d9AnmScnRes", "lbl_eu_80569490"),
            ("__vt__Q34nw4r3g3d6AnmScn", "lbl_eu_80569520"),
        ),
        extern_data_sections=(".sdata2", ".data", ".rodata"),
    ),
    "g3d_anmclr.o": UnitRules(
        pool_patterns=(
            (struct.pack(">I", 0x3F800000), "lbl_eu_80669A6C"),
        ),
        # Retail split is .text-only: the AnmObjMatClrRes/AnmObjMatClr vtables
        # live in nw4r_data.s (lbl_eu_80569180/lbl_eu_805691C8) and the
        # ResName type-name strings are lbl_eu_8051D530/8051D548 (already
        # retail-named in the object). Rename the vtables and strip all data
        # sections so .text relocs resolve to the retail data object.
        exact_renames=(
            ("__vt__Q34nw4r3g3d15AnmObjMatClrRes", "lbl_eu_80569180"),
            ("__vt__Q34nw4r3g3d12AnmObjMatClr", "lbl_eu_805691C8"),
        ),
        extern_data_sections=(".sdata2", ".data", ".rodata"),
    ),

    # g3d dcc trio (maya/xsi/3dsmax): each TU now DEFINES its two retail
    # dispatch tables (lbl_eu_8051D6C0/D6DC, D6F8/D714, D730/D74C -- blob
    # .rodata 0x220-0x2AC, 7 entries each in reverse declaration order). The
    # in-TU definitions give -ipa file real references to the 14 anon-namespace
    # TexSrtMtx/ProductTexSrtMtx functions, replacing the former
    # DECOMP_FORCEACTIVE emitters entirely. History: ((used)) is ignored by
    # this MWCC, #pragma force_active does not stop -ipa file DCE (verified
    # 13/14 stripped), external linkage would change the @unnamed@g3d_*_cpp@
    # mangled names the blob tables relocate against, and exact-name stub
    # drops silently disarmed whenever __LINE__ drifted. extern_data_sections
    # strips the TU .rodata copies and converts the six symbols to UNDEF so
    # they resolve to the nw4r_data.s blob definitions at link (g3d_anmclr
    # vtable pattern); data gate sees empty-vs-empty. Verified byte-neutral
    # for every function including CalcTexMtx_* (hexdiff --all identical to
    # the stub-based build; maya/xsi/3dsmax 2026-08 session).
    # g3d dcc trio (maya/xsi/3dsmax): each TU defines its two retail dispatch
    # tables (lbl_eu_8051D6C0/D6DC, D6F8/D714, D730/D74C -- nw4r_data.s .rodata
    # 0x220-0x2AC, 7 entries each in reverse declaration order). The in-TU
    # definitions give -ipa file real references to the 14 anon-namespace
    # TexSrtMtx/ProductTexSrtMtx functions, replacing the old DECOMP_FORCEACTIVE
    # emitters entirely ((used) and #pragma force_active are both ignored by
    # -ipa file DCE; external linkage would break the @unnamed@g3d_*_cpp@
    # mangled names). extern_data_sections then strips the TU .rodata copies
    # and converts the six symbols to UNDEF so they resolve to the nw4r_data.s
    # blob definitions at link (g3d_anmclr vtable pattern); the data gate sees
    # empty-vs-empty. Verified byte-neutral for every function including
    # CalcTexMtx_* (hexdiff --all identical to the stub-based build).
    "g3d_maya.o": UnitRules(
        extern_data_sections=(".rodata",),
    ),
    "g3d_xsi.o": UnitRules(
        extern_data_sections=(".rodata",),
    ),
    "g3d_3dsmax.o": UnitRules(
        extern_data_sections=(".rodata",),
    ),

    "g3d_scnroot.o": UnitRules(
        # Retail split is .text-only: ScnObjGather/ScnRoot vtables ship from
        # nw4r_data.s (lbl_eu_805696C0 / lbl_eu_805696F0); the weak IScnObj-
        # Gather vtable is unreferenced (retail linker GC'd it). ScnRoot's
        # ResName string is already retail-named (lbl_eu_8051D798) and the
        # frame-counter sbss static is source-named lbl_eu_8066348C; strip.
        exact_renames=(
            ("__vt__Q34nw4r3g3d12ScnObjGather", "lbl_eu_805696C0"),
            ("__vt__Q34nw4r3g3d7ScnRoot", "lbl_eu_805696F0"),
        ),
        extern_data_sections=(".data", ".rodata", ".sbss"),
    ),

    "g3d_scnmdl.o": UnitRules(
        # Retail split is .text-only: ScnMdl vtable ships from nw4r_data.s
        # @lbl_eu_805697A0; ResName strings already retail-named
        # (lbl_eu_8051D7C0 / lbl_eu_8051D610). Strip .data/.rodata.
        exact_renames=(("__vt__Q34nw4r3g3d6ScnMdl", "lbl_eu_805697A0"),),
        extern_data_sections=(".data", ".rodata"),
    ),

    "g3d_scnmdlexpand.o": UnitRules(
        # Retail split is .text-only: ScnMdlExpand vtable ships from
        # nw4r_data.s @lbl_eu_805697E8; the ResName string is already
        # retail-named (lbl_eu_8051D7D0). Strip .data/.rodata.
        exact_renames=(("__vt__Q34nw4r3g3d12ScnMdlExpand", "lbl_eu_805697E8"),),
        extern_data_sections=(".data", ".rodata"),
    ),

    # kyoshin data dissolve: retail split objects are .text-only; all class
    # data ships from the shared split1.s blob (see each entry's comment).
    "CArtsSet.o": UnitRules(
        # TU is text-only: retail keeps the _sArtsSet vtable in the shared
        # data split (lbl_eu_8052F5D8), so the ctor's implicit vptr store +
        # slot-1 dispatch reference the compiler-generated __vt__ name as an
        # UNDEF while retail names the shared label. Pure symbol rename; no
        # codegen or data-section impact (hexdiff 15/15 byte-identical).
        exact_renames=(("__vt__Q22cf9_sArtsSet", "lbl_eu_8052F5D8"),),
    ),

    "CTagProcessor.o": UnitRules(
        # MWCC emits three dead anonymous zero tables in .data (@7580/@8026/
        # @8745 = 0xAC) and an .sdata2 pool; the live pool entry is the signed
        # int->f32 conversion magic double (func_80127D20 lfd site) — rename it
        # onto the split1.s blob label BEFORE stripping the section.
        pool_patterns=(
            (struct.pack(">II", 0x43300000, 0x80000000), "lbl_eu_80667200"),
        ),
        extern_data_sections=(".data", ".sdata2"),
    ),

    "CMenuUpdate.o": UnitRules(
        # Phantom .sdata2 pool {128.0f, 0.0f} loaded via 3 live SDA21 relocs;
        # bytes equal split1.s lbl_eu_806673B8 (unique content match among the
        # unit's retail refs). Rename then strip.
        exact_renames=(("@6235", "lbl_eu_806673B8"),),
        extern_data_sections=(".sdata2",),
    ),

    "CMenuLandTelop.o": UnitRules(
        # Remaining phantom pool {128.0f-as-double magic} equals blob double
        # lbl_eu_806673F8 (retail loads f29 from exactly that label in Move).
        # The former {100.0-double} pool is gone via the Move() source fix
        # (retail materializes the double on stack from the int product).
        exact_renames=(("@8164", "lbl_eu_806673F8"),),
        extern_data_sections=(".sdata2",),
    ),

    "CTaskREvent.o": UnitRules(
        # Phantom .sdata2 pool {128.0f, 0x80000000} (lfd int->double magic);
        # content-equal blob label among unit refs.
        exact_renames=(("@10817", "lbl_eu_80667630"),),
        extern_data_sections=(".sdata2",),
    ),

    "CTaskREvtSequence.o": UnitRules(
        # Same magic pool; content-equal blob label.
        exact_renames=(("@17036", "lbl_eu_80667680"),),
        extern_data_sections=(".sdata2",),
    ),

    "CfObjectActor.o": UnitRules(
        # Signed int->f64 conversion magic (43300000_80000000); content-keyed
        # so MWCC's @N renumbering cannot break it (was exact_renames @1061,
        # drifted to @1228/@2732 with TU growth).
        pool_patterns=(
            (struct.pack(">II", 0x43300000, 0x80000000), "lbl_eu_80667748"),
        ),
        extern_data_sections=(".sdata2",),
    ),

    "CREvtModel.o": UnitRules(
        # Phantom {1.0f}/{0.0f} pair; site-mapped to the labels retail's
        # func_80172CE4 prologue loads (lfs f1/f0 from 80667774/80667778).
        exact_renames=(
            ("@8408", "lbl_eu_80667774"),
            ("@8409", "lbl_eu_80667778"),
        ),
        extern_data_sections=(".sdata2",),
    ),

    "CfMapItemManager.o": UnitRules(
        # Phantom pools + dispatch table (object rebuilt by concurrent agent;
        # re-derived from the fresh object): {2^52+scale} double ->
        # lbl_eu_806677A0; plain 2^52 -> lbl_eu_806677B0 (the copy retail's
        # func_80174AE8 range loads); 8-slot func_80174C98+addend jump table
        # -> jumptable_eu_80531710.
        exact_renames=(
            ("@4956", "lbl_eu_806677A0"),
            ("@5129", "lbl_eu_806677B0"),
            ("@5244", "jumptable_eu_80531710"),
        ),
        extern_data_sections=(".data", ".sdata2"),
    ),

    "CREvtModelMap.o": UnitRules(
        # Magic pool; site-confirmed (lfd f1, lbl_eu_806678C8 in func_8018152C).
        exact_renames=(("@3569", "lbl_eu_806678C8"),),
        extern_data_sections=(".sdata2",),
    ),

    "CREvtModelObj.o": UnitRules(
        # Magic pool; content-equal blob label among unit refs.
        exact_renames=(("@3007", "lbl_eu_80667910"),),
        extern_data_sections=(".sdata2",),
    ),

    "CREvtModelPc.o": UnitRules(
        # Magic pool; content-equal blob label among unit refs.
        exact_renames=(("@3055", "lbl_eu_80667920"),),
        extern_data_sections=(".sdata2",),
    ),

    "CREvtEffect.o": UnitRules(
        # Magic pool (5 live relocs); content-equal blob label.
        exact_renames=(("@4849", "lbl_eu_80667950"),),
        extern_data_sections=(".sdata2",),
    ),

    "pluginBtl.o": UnitRules(
        # Live phantom pool + string base: {128.0f,0x80000000} matches blob
        # lbl_eu_80667978; the "ene_arts"/"name" rodata block matches blob
        # lbl_eu_805038B0 byte-for-byte. Rename both then strip.
        exact_renames=(
            ("@2286", "lbl_eu_80667978"),
            ("@stringBase0", "lbl_eu_805038B0"),
        ),
        extern_data_sections=(".rodata", ".sdata2"),
    ),

    "code_801862C0.o": UnitRules(
        # Two live magic pools ({128f,-0.0}/{128f,+0.0}) mapped to adjacent
        # blob doubles 806679D0/806679D8 (content-equal, unit-ref'd); and the
        # zero-init lbl_eu_80574090 buffer: retail keeps it in the shared
        # blob (.bss @80574090) while MWCC emits a TU-local copy — strip so
        # the named symbol goes UNDEF and resolves to the blob at link.
        exact_renames=(
            ("@401", "lbl_eu_806679D0"),
            ("@402", "lbl_eu_806679D8"),
        ),
        extern_data_sections=(".sdata2", ".bss"),
    ),

    "CTalkWindow.o": UnitRules(
        # @8705 is a 9-entry jump table (all .rela.data words target own
        # .text): retail keeps it as jumptable_eu_8052DF80 whose slots spell
        # Init__11CTalkWindowFv+offset — resolves to our definitions at link.
        # @8419 is a lone {0.0f} float; maps to unit-ref'd blob lbl_eu_80667280.
        exact_renames=(
            ("@8705", "jumptable_eu_8052DF80"),
            ("@8419", "lbl_eu_80667280"),
        ),
        extern_data_sections=(".data", ".sdata2"),
    ),

    # code_80135FDC: the tick-switch jump table is gone via the range-chain
    # source reshape (retail compiles it as a compare tree). Remaining live
    # phantoms map onto split1.s labels:
    "code_80135FDC.o": UnitRules(
        # CONTENT-keyed float pools (section-aware on purpose):
        #   .sdata2@0x00 2^52 unsigned double -> lbl_eu_806672F8
        #   .sdata2@0x08 lone 0.0f            -> lbl_eu_806672F0
        #   .sdata2@0x10 signed HI magic      -> lbl_eu_80667360
        # Do NOT use pool_patterns here: the .data jumptable @12110 sits at
        # st_value 0 and rename_pool_symbols matches every @N symbol against
        # .sdata2 bytes at st_value regardless of home section - it steals
        # the table onto lbl_eu_806672F8 before any exact_renames can fire.
        data_pool_patterns=(
            (".sdata2", struct.pack(">II", 0x43300000, 0x00000000), "lbl_eu_806672F8"),
            (".sdata2", struct.pack(">I", 0x00000000), "lbl_eu_806672F0"),
            (".sdata2", struct.pack(">II", 0x43300000, 0x80000000), "lbl_eu_80667360"),
        ),
        # func_801393CC dispatch table -> retail lis/addi jumptable_eu_8052E488
        # @80139DF8/80139E00. exact_renames because BOTH .data tables are
        # all-zero, so a bytes(0x38) content pattern would ambiguously match
        # @12110 too (rename_data_pool_symbols skips ambiguous patterns).
        # KNOWN-DANGLING: @12110 (.data@0x0, 21-entry switch of
        # func_80138574) has NO retail counterpart - retail compiles that
        # switch to compare chains (no blob table references
        # func_80138574), so it intentionally stays an anonymous UNDEF until
        # func_80138574's code converges. Expect exactly one U @N in audits
        # of this unit; more than one means a rename regressed.
        exact_renames=(
            ("@12261", "jumptable_eu_8052E488"),  # func_801393CC dispatch
        ),
        extern_data_sections=(".data", ".sdata2"),
    ),

    "CCol6Invite.o": UnitRules(
        # The zero-init flag lbl_eu_8066423C lives in the split1.s .sbss;
        # MWCC emits a TU-local copy — strip so the named symbol goes UNDEF
        # and resolves to the blob at link (same pattern as code_801862C0).
        extern_data_sections=(".sbss",),
    ),

    "CfResReloadImpl.o": UnitRules(
        # All statics are already retail-named (lbl_eu_806676xx sdata/sbss);
        # retail keeps them in the split1.s blob while MWCC emits TU-local
        # copies (.sdata doubles 0x43D0CC00..., .sbss flags, dead .sdata2
        # pool {128.0f,0.0f}/{0.0f}/{128.0f,0x80000000}). Strip; the named
        # symbols go UNDEF and resolve to the blob at link.
        extern_data_sections=(".sdata", ".sdata2", ".sbss"),
    ),

    "CArtsParam.o": UnitRules(
        # Phantom .sdata2 pools: lone {0.0f} -> lbl_eu_80667460; magic pairs
        # -> 80667490/{80667498 (content-equal, unit-ref'd). The global
        # cf::CArtsParam lbl_80577580 is retail's lbl_eu_80573D88 object
        # (retail sinit stores vtable lbl_eu_8052F610 into +0x84 of exactly
        # that label).
        exact_renames=(
            ("@4382", "lbl_eu_80667460"),
            ("@4722", "lbl_eu_80667490"),
            ("@4723", "lbl_eu_80667498"),
            ("lbl_80577580__2cf", "lbl_eu_80573D88"),
            # CAttackParam/CArtsParam vtables ship from split1.s; the ctors'
            # +0x84 dispatch-pointer stores must name the retail blobs.
            ("__vt__Q22cf12CAttackParam", "lbl_eu_8052F610"),
            ("__vt__Q22cf10CArtsParam", "lbl_eu_8052F5E8"),
        ),
        extern_data_sections=(".sdata2", ".bss", ".data"),
    ),

    "CItem.o": UnitRules(
        # Already-retail-named .bss buffers lbl_eu_80573E18/lbl_eu_80573EEC
        # resolve to the blob after strip. The two anonymous 0x38 zero tables
        # (address-taken scratch) map onto adjacent blob .bss skips.
        exact_renames=(
            ("@4959", "lbl_eu_80573FC0"),
            ("@5062", "lbl_eu_80574004"),
        ),
        extern_data_sections=(".data", ".bss"),
    ),

    "CCol6System.o": UnitRules(
        # @10451 is the Move() state jump table (41 own-text slots): retail
        # keeps jumptable_eu_8052FE98 (site-confirmed lis/addi). @10677 is
        # the {128.0f,-0.0} magic double (site-confirmed lfd lbl_eu_80667580).
        # Remaining statics are already retail-named (blob .bss/.sbss).
        exact_renames=(
            ("@10451", "jumptable_eu_8052FE98"),
            ("@10677", "lbl_eu_80667580"),
        ),
        extern_data_sections=(".data", ".sdata2", ".bss", ".sbss"),
    ),

    "CfBdat.o": UnitRules(
        # Data dissolve with instruction-level ground truth (func_801414CC
        # store sequence aligns 1:1 with our sinit assignments; anchors:
        # spLandmark->806640A0 per source note; quest table written through
        # lbl_eu_80573D18 per the getFP copy-loop base). Statics renamed onto
        # their true blob labels, then all sections strip; the blob copies
        # reference our members by name (e.g. __dt__Q22cf6CfBdatFv inside
        # lbl_eu_8052E718), so virtual dispatch resolves at link.
        exact_renames=(
            # getFP result stores, in sinit order:
            ("spBtlPcListFileData__Q22cf6CfBdat", "lbl_eu_80664090"),
            ("spBtlEneListFileData__Q22cf6CfBdat", "lbl_eu_80664094"),
            ("spBtlSkillListFileData__Q22cf6CfBdat", "lbl_eu_806640D8"),
            ("spBtlGrowListFileData__Q22cf6CfBdat", "lbl_eu_806640DC"),
            ("spBtlBuffListFileData__Q22cf6CfBdat", "lbl_eu_806640E0"),
            ("spFldPointListFileData__Q22cf6CfBdat", "lbl_eu_806640E4"),
            ("spFldTboxListFileData__Q22cf6CfBdat", "lbl_eu_806640E8"),
            ("spBtlCamListFileData__Q22cf6CfBdat", "lbl_eu_80664164"),
            ("spBtlCamDataListFileData__Q22cf6CfBdat", "lbl_eu_80664168"),
            ("spBtlCrystalNameListFileData__Q22cf6CfBdat", "lbl_eu_8066416C"),
            ("spMnuItemFileData__Q22cf6CfBdat", "lbl_eu_80664170"),
            ("spFldNpcListFileData__Q22cf6CfBdat", "lbl_eu_80664098"),
            ("spFldMapListFileData__Q22cf6CfBdat", "lbl_eu_806640A8"),
            ("spLandmarkListFileData__Q22cf6CfBdat", "lbl_eu_806640A0"),
            ("spFldValPopListFileData__Q22cf6CfBdat", "lbl_eu_806640D0"),
            ("spFldDmObjListFileData__Q22cf6CfBdat", "lbl_eu_806640D4"),
            ("spItmItemListFileData__Q22cf6CfBdat", "lbl_eu_806640EC"),
            ("spItmWpnListFileData__Q22cf6CfBdat", "lbl_eu_806640F4"),
            ("spItmEquipListFileData__Q22cf6CfBdat", "lbl_eu_806640F8"),
            ("spItmCrystalListFileData__Q22cf6CfBdat", "lbl_eu_806640FC"),
            ("spItmDropCrystalListFileData__Q22cf6CfBdat", "lbl_eu_80664100"),
            ("spItmCollectListFileData__Q22cf6CfBdat", "lbl_eu_80664104"),
            ("spItmMaterialListFileData__Q22cf6CfBdat", "lbl_eu_80664108"),
            ("spItmValuableListFileData__Q22cf6CfBdat", "lbl_eu_8066410C"),
            ("spItmArtsListFileData__Q22cf6CfBdat", "lbl_eu_80664110"),
            ("spItmHeadListFileData__Q22cf6CfBdat", "lbl_eu_80664114"),
            ("spItmBodyListFileData__Q22cf6CfBdat", "lbl_eu_80664118"),
            ("spItmArmListFileData__Q22cf6CfBdat", "lbl_eu_8066411C"),
            ("spItmWaistListFileData__Q22cf6CfBdat", "lbl_eu_80664120"),
            ("spItmLeggListFileData__Q22cf6CfBdat", "lbl_eu_80664124"),
            # scalar resets + PSV/PSS trio:
            ("lbl_80666A74__Q22cf6CfBdat", "lbl_eu_80664184"),
            ("lbl_80666A78__Q22cf6CfBdat", "lbl_eu_80664188"),
            ("lbl_80666A7C__Q22cf6CfBdat", "lbl_eu_8066418C"),
            ("lbl_8066698C__Q22cf6CfBdat", "lbl_eu_8066409C"),
            ("spBtlPsvSkillFileData__Q22cf6CfBdat", "lbl_eu_80664158"),
            ("spBtlPsvLinkFileData__Q22cf6CfBdat", "lbl_eu_8066415C"),
            ("spBtlPssListFileData__Q22cf6CfBdat", "lbl_eu_80664160"),
            # resetMapBdatFileDataPointers spCur* statics -> blob labels
            # (hexdiff 8014215c: all 22 sites SDA21, bytes already match)
            ("spCurRouteListFileData__Q22cf6CfBdat", "lbl_eu_806640A4"),
            ("spCurMapEffListFileData__Q22cf6CfBdat", "lbl_eu_806640AC"),
            ("spCurMapObjListFileData__Q22cf6CfBdat", "lbl_eu_806640B0"),
            ("spCurMapLodListFileData__Q22cf6CfBdat", "lbl_eu_806640B4"),
            ("spCurMapSeListFileData__Q22cf6CfBdat", "lbl_eu_806640B8"),
            ("spCurFldGimCamListFileData__Q22cf6CfBdat", "lbl_eu_806640BC"),
            ("spCurLItemListFileData__Q22cf6CfBdat", "lbl_eu_806640C0"),
            ("spCurExTalkListFileData__Q22cf6CfBdat", "lbl_eu_806640C4"),
            ("spCurMineListFileData__Q22cf6CfBdat", "lbl_eu_806640C8"),
            ("spCurBtlEneListFileData__Q22cf6CfBdat", "lbl_eu_806640CC"),
            ("spCurGimListFileData__Q22cf6CfBdat", "lbl_eu_80664128"),
            ("spCurFieldLockFileData__Q22cf6CfBdat", "lbl_eu_8066412C"),
            ("spCurElvGmFileData__Q22cf6CfBdat", "lbl_eu_80664130"),
            ("spCurWarpGmFileData__Q22cf6CfBdat", "lbl_eu_80664134"),
            ("spCurJumpGmFileData__Q22cf6CfBdat", "lbl_eu_80664138"),
            ("spCurItemGmFileData__Q22cf6CfBdat", "lbl_eu_8066413C"),
            ("spCurGimSvOffFileData__Q22cf6CfBdat", "lbl_eu_80664140"),
            ("spCurGimEneFileData__Q22cf6CfBdat", "lbl_eu_80664144"),
            ("spCurGimMessFileData__Q22cf6CfBdat", "lbl_eu_80664148"),
            ("spCurDropNmlListFileData__Q22cf6CfBdat", "lbl_eu_8066414C"),
            ("spCurDropRarListFileData__Q22cf6CfBdat", "lbl_eu_80664150"),
            ("spCurDropSprListFileData__Q22cf6CfBdat", "lbl_eu_80664154"),
            ("spMnuEveStartFileData__Q22cf6CfBdat", "lbl_eu_80664174"),
            ("spMnuEveTableFileData__Q22cf6CfBdat", "lbl_eu_80664178"),
            # misc state read/written across functions:
            ("lbl_80666A6C__Q22cf6CfBdat", "lbl_eu_8066417C"),
            ("lbl_80666A70__Q22cf6CfBdat", "_lbl_eu_80664180"),
            # quest-name pointer table: retail getFP loop writes through
            # lbl_eu_80573D18 (lis/addi base in func_801414CC tail)
            ("lbl_80577510__Q22cf6CfBdat", "lbl_eu_80573D18"),
            # live phantom pools:
            ("@9258", "lbl_eu_80667368"),         # lone {0.0f} (lfs site)
            ("@stringBase0", "lbl_eu_80500FA4"),  # BTL_*/FLD_*/ITM_* strings
            ("@8661", "lbl_eu_80500F28"),         # quest template ptr array
        ),
        extern_data_sections=(".data", ".rodata", ".sdata", ".sdata2", ".bss", ".sbss"),
    ),

    "CUICfManager.o": UnitRules(
        # Data dissolve: retail split carries no data. Vtables map onto the
        # split1.s blobs whose slots name our members (link resolves):
        # CTask<CUICfManager>=8052E444; the empty-dtor-emitted IWorkEvent
        # vtable maps onto the real base vtable lbl_eu_80525A10 (retail has
        # no IFlagEvent vtable at all — inert vptr store in the extra dtor);
        # RTTI locators/typeinfo names ship from blob/runtime; ctors use blob
        # labels directly.
        exact_renames=(
            ("__vt__22CTTask<12CUICfManager>", "lbl_eu_8052E444"),
            ("__vt__10IWorkEvent", "lbl_eu_80525A10"),
        ),
        # The empty-dtor-emitted IFlagEvent vptr stores: retarget the two
        # .text relocs directly (objcopy rejects two redefines with the same
        # target); the local vtable def dies with the .data strip.
        retarget_relocs=(
            (".text", 0x365E, "lbl_eu_80525A10"),
            (".text", 0x366A, "lbl_eu_80525A10"),
        ),
        extern_data_sections=(".data", ".rodata", ".sdata", ".bss", ".sbss"),
    ),

    "CUIWindowManager.o": UnitRules(
        # Data dissolve: same pattern; vtables map onto split1.s blobs
        # (content-verified: dtor slot names match our members).
        exact_renames=(
            ("__vt__16CUIWindowManager", "lbl_eu_8052E5A0"),
            ("__vt__20reslist<P9IUIWindow>", "lbl_eu_8052E604"),
            ("__vt__26_reslist_base<P9IUIWindow>", "lbl_eu_8052E61C"),
            ("__vt__26CTTask<16CUIWindowManager>", "lbl_eu_8052E6B0"),
        ),
        extern_data_sections=(".data", ".rodata", ".sdata", ".sbss"),
    ),

    "CActorParam.o": UnitRules(
        # Data dissolve: retail split carries no data — vtable/RTTI
        # locators/typeinfo names/float pools/static-local flags ship from
        # split1.s (lbl_eu_8052F5A8/F5C0 vtables, 806677Exx sdata2 pool); the
        # source stores vptrs from blob labels directly and no reloc
        # references any local copy. Strip.
        extern_data_sections=(".data", ".rodata", ".sdata", ".sdata2", ".sbss"),
        exact_renames=(
            # Retail tail-merges UnkVirtualFunc180's body under its virtual
            # (Fv) symbol; our fake-Fv free-function definition (self+Arg
            # params, so MWCC appends the param suffix) is the same code.
            # Rename the definition to the retail virtual name so the
            # CBattleState_UnkVirtualFunc18 b-thunk and extabindex relocs
            # resolve to it exactly like retail.
            (
                "CActorParam_UnkVirtualFunc180__Q22cf11CActorParamFv__FPQ22cf11CActorParamPQ22cf17CActorParam180Arg",
                "CActorParam_UnkVirtualFunc180__Q22cf11CActorParamFv",
            ),
        ),
    ),

    "CMenuQstCnt.o": UnitRules(
        # Data dissolve: retail split has no .sdata2/.sbss — the two floats
        # (1.0, 90.0) ship from the shared CGXCache pool and the 4-byte
        # static (lbl_eu_80664720) from its retail owner. Strip + retarget.
        pool_patterns=(
            (struct.pack(">I", 0x3F800000), "lbl_eu_8066A380"),
            (struct.pack(">I", 0x42B40000), "lbl_eu_8066A700"),
        ),
        extern_data_sections=(".sdata2", ".sbss"),
    ),
    "ut_TagProcessorBase.o": UnitRules(
        # Retail merges both TagProcessorBase vtables into the split blob;
        # the <w> ctor's vtable import carries the MWCC template name while
        # retail points at lbl_eu_8056AE08. Rename the import.
        exact_renames=(
            ("__vt__Q34nw4r2ut19TagProcessorBase<w>", "lbl_eu_8056AE08"),
        ),
    ),
    # === RVL_SDK data dissolve (os/sc/vi/wpad/wud) ============================
    "OSReset.o": UnitRules(
        # Merged duplicate entries: MWCC 8-aligns the large string-init
        # pools, inserting 4 pad bytes after lbl_80552AF0 (retail packs them
        # at 4), AND pads .data to 0x26C where retail ends at 0x268.
        drop_data_range=((".data", 0xC, 0x10),),
        pad_data_section=((".data", 0x268),),
        drop_data_tail=((".data", 0x268),),
    ),
    "vi.o": UnitRules(
        # MWCC anon pools vs retail labels: .data head string pool and the
        # 5-byte .sdata version-string object.
        exact_renames=(
            ("@2516", "lbl_8055FAA0"),
            ("@4203", "lbl_80665D18"),
        ),
        # __VIRetraceHandler is a §17.6 code residual (CODE_MATCH, three case
        # blocks sit 8 bytes off retail): pin its switch-table addends.
        # (offsets are pre-drop: the tables sit 16 bytes later in raw MWCC
        # output because of vi_timing_pad ahead of them; all nine case-label
        # addends are +8 vs our residual code layout)
        addend_sets=(
            (".data", 760, 848),
            (".data", 764, 856),
            (".data", 768, 864),
            (".data", 772, 848),
            (".data", 776, 856),
            (".data", 780, 864),
            (".data", 784, 848),
            (".data", 788, 848),
            (".data", 792, 848),
        ),
        # vi_timing_pad is pure MWCC overhead (retail's timing table already
        # carries its own 2-byte tail); drop it plus the align pad before
        # GXPal524ProgAa so taps/GXPals/SFI land at the retail offsets.
        drop_data_range=(
            (".data", 0x1ED, 0x1F9),
            (".data", 0x295, 0x299),
        ),
    ),
    "scsystem.o": UnitRules(
        # MWCC anon numbering vs retail splitter: the NameAndIDTbl name-string
        # literals (@2099..@2135 -> @724..@760) and the __SCVersion pool ref
        # (@2098 -> @723). Bytes already match; names only.
        exact_renames=(
            ("@2098", "@723"),
            ("@2099", "@724"),
            ("@2100", "@725"),
            ("@2101", "@726"),
            ("@2102", "@727"),
            ("@2103", "@728"),
            ("@2104", "@729"),
            ("@2105", "@730"),
            ("@2106", "@731"),
            ("@2107", "@732"),
            ("@2108", "@733"),
            ("@2109", "@734"),
            ("@2110", "@735"),
            ("@2111", "@736"),
            ("@2112", "@737"),
            ("@2113", "@738"),
            ("@2114", "@739"),
            ("@2115", "@740"),
            ("@2116", "@741"),
            ("@2117", "@742"),
            ("@2118", "@743"),
            ("@2119", "@744"),
            ("@2120", "@745"),
            ("@2121", "@746"),
            ("@2122", "@747"),
            ("@2123", "@748"),
            ("@2124", "@749"),
            ("@2125", "@750"),
            ("@2126", "@751"),
            ("@2127", "@752"),
            ("@2128", "@753"),
            ("@2129", "@754"),
            ("@2130", "@755"),
            ("@2131", "@756"),
            ("@2132", "@757"),
            ("@2133", "@758"),
            ("@2134", "@759"),
            ("@2135", "@760"),
        ),
    ),
    "WUD.o": UnitRules(
        # Retail .data pools rebuilt as named byte arrays in WUD.c; MWCC pads
        # the larger arrays to 4/8 (retail packs at 1/4) — drop the seven
        # zero pads, then trim the compiler's switch table off the tail.
        # The retail table slot is the zero-filled jumptable_80562FA0 array;
        # its ADDR32 case-label relocs are injected below and __wudSecurity-
        # EventStackCallback's lis/addi are retargeted onto it.
        drop_data_range=(
            (".data", 0x21C, 0x220),
            (".data", 0x308, 0x30C),
            (".data", 0x4F4, 0x4F8),
            (".data", 0x720, 0x724),
            (".data", 0x79C, 0x7A0),
            (".data", 0x818, 0x81C),
            (".data", 0x964, 0x968),
        ),
        retarget_relocs=(
            (".text", 0x51EA, "jumptable_80562FA0"),
            (".text", 0x51F2, "jumptable_80562FA0"),
        ),
        inject_relocs=(
            (".data", 0x144, "@etb_80010138"),
            (".data", 0xCF4, "__wudSecurityEventStackCallback+92"),
            (".data", 0xCF8, "__wudSecurityEventStackCallback+264"),
            (".data", 0xCFC, "__wudSecurityEventStackCallback+292"),
            (".data", 0xD00, "__wudSecurityEventStackCallback+452"),
            (".data", 0xD04, "__wudSecurityEventStackCallback+720"),
            (".data", 0xD08, "__wudSecurityEventStackCallback+736"),
            (".data", 0xD0C, "__wudSecurityEventStackCallback+972"),
            (".data", 0xD10, "__wudSecurityEventStackCallback+1312"),
            (".data", 0xD14, "__wudSecurityEventStackCallback+1328"),
        ),
        # retail names the patch-table ETB pointer "@etb_80010138".
        exact_renames=(("etb_80010138", "@etb_80010138"),),
        # jumptable_80562FA0's last byte is initialized to 1 to keep the array
        # out of .bss; re-zero it (no reloc points at that word).
        zero_data_range=((".data", 0xD17, 0xD18),),
        drop_data_tail=((".data", 0x11A8),),
    ),
    "OSNet.o": UnitRules(
        # MWCC pools the retail "NWC24iPrepareShutdown" string (kept alive by
        # a discarded-value cast in NWC24iRequestShutdown) AFTER the real-ref
        # "/dev/net/kd/request" block; retail has them in the opposite order.
        # Rotate [kd/req(0x14)][Prepare(0x18)] -> [Prepare][kd/req]: swap the
        # 0x14 tail of Prepare's slot with kd/req, then bubble the remaining
        # word into place. No .data relocs exist in this range.
        swap_data_blocks=(
            (".data", 0xA8, 0xC0, 0x14),
            (".data", 0xB8, 0xBC, 0x04),
            (".data", 0xB4, 0xB8, 0x04),
            (".data", 0xB0, 0xB4, 0x04),
            (".data", 0xAC, 0xB0, 0x04),
            (".data", 0xA8, 0xAC, 0x04),
        ),
    ),
    "WPADEncrypt.o": UnitRules(
        # Retail .data ends with the final "sb  : %d ... %d\n" debug-format
        # string padded to a 4-byte tail (0x1328); MWCC emits a single NUL
        # (0x1327). Zero-pad restores the retail section exactly.
        pad_data_section=((".data", 0x1328),),
    ),
    "WUDHidHost.o": UnitRules(
        # Same tail story, inverted: retail ends at "bta_hh_co_close()\n\0"
        # (0x297) but MWCC pads the final string one byte further (0x298).
        # No relocs point into the dropped byte.
        drop_data_tail=((".data", 0x297),),
    ),
    # === kyoshin data dissolve, batch 2 (retail split objects are .text-only;
    # all class data ships from split1.s; ground truth = each unit's split-asm
    # lbl_eu_*@sda21/@ha/@l refs + jumptable target-function correspondence) ===
    "CTaskGamePic.o": UnitRules(
        # Vtable/RTTI names/typeinfo strings are dead local copies (the ctor
        # stores blob vtables lbl_eu_80538AD8/BC0 directly); no reloc refs.
        extern_data_sections=(".data", ".rodata", ".sdata"),
    ),
    "CTaskGameEvt.o": UnitRules(
        # Same shape as CTaskGamePic: unreferenced vtable/RTTI/string copies.
        extern_data_sections=(".data", ".rodata", ".sdata"),
    ),
    "CHelpManager.o": UnitRules(
        # Unreferenced local data copies; strip to empty retail sections.
        extern_data_sections=(".data", ".rodata", ".sdata"),
    ),
    "CMenuTutorial.o": UnitRules(
        # Unreferenced local data copies (incl. phantom 4-byte .sdata2 slot).
        extern_data_sections=(".data", ".rodata", ".sdata", ".sdata2"),
        # ~CMenuTutorial's compiler-generated base-dtor call binds to the
        # locally emitted __dt__8CProcessFv; retail calls the game-side
        # wrapper __dt__800FED0C (single referencer, CMenuKizunaTalkList pattern).
        exact_renames=(
            ("__dt__8CProcessFv", "__dt__800FED0C"),
        ),
    ),
    "CTutorial.o": UnitRules(
        extern_data_sections=(".data", ".rodata", ".sdata"),
    ),
    "CMenuSkipTimer.o": UnitRules(
        extern_data_sections=(".data", ".rodata", ".sdata", ".sdata2"),
        # The hand-written __dt__14CMenuSkipTimerFv calls the game-side
        # CProcess dtor wrapper __dt__800FED0C (defined in CMainMenu), but
        # MWCC folds the call onto its own locally emitted ~CMenuSkipTimerBase
        # copy (direct call AND cast-through-fn-ptr both fold). Retarget only
        # that one reloc (dtor body @0x8bc + 0x4c) to the retail UNDEF name;
        # the dead local base-dtor copies stay emitted (unit text overage,
        # promotion gate, not a per-function issue).
        retarget_relocs=((".text", 0x908, "__dt__800FED0C"),),
    ),
    "CVS_THREAD_BUF.o": UnitRules(
        extern_data_sections=(".data", ".rodata", ".sdata"),
    ),
    "CVS_THREAD_FAINT.o": UnitRules(
        extern_data_sections=(".data", ".rodata", ".sdata"),
    ),
    "CVS_THREAD_HAGE.o": UnitRules(
        extern_data_sections=(".data", ".rodata", ".sdata"),
    ),
    "CVS_THREAD_REVIVE.o": UnitRules(
        extern_data_sections=(".data", ".rodata", ".sdata"),
    ),
    "CVS_THREAD_SUDDEN.o": UnitRules(
        extern_data_sections=(".data", ".rodata", ".sdata"),
    ),
    "CVS_THREAD_TENSION_UP.o": UnitRules(
        extern_data_sections=(".data", ".rodata", ".sdata"),
    ),
    "CVS_THREAD_VISION_TELL.o": UnitRules(
        extern_data_sections=(".data", ".rodata", ".sdata"),
    ),
    "CVS_THREAD_BATTLE_END_SP.o": UnitRules(
        extern_data_sections=(".data", ".rodata", ".sdata"),
    ),
    "CVS_THREAD_BATTLE_BEGIN.o": UnitRules(
        extern_data_sections=(".data", ".rodata", ".sdata"),
    ),
    "CMCGetItemBox.o": UnitRules(
        # int->double conversion pool: first slot is the 2^52 double loaded
        # before the magic lfd at GetNum's site (retail 802992C4 ->
        # lbl_eu_80668BD8), second is the int->double magic itself (site
        # 802992DC -> lbl_eu_80668BE0); both content-unique in this TU.
        pool_patterns=(
            (struct.pack(">d", 4503599627370496.0), "lbl_eu_80668BD8"),
            (struct.pack(">II", MAGIC_HI, MAGIC_LO), "lbl_eu_80668BE0"),
        ),
        extern_data_sections=(".sdata2",),
    ),
    "COption.o": UnitRules(
        # Five zero-init switch jumptables map 1:1 by their case-target
        # functions (split1.s jumptable entries): func_8029D634 ->
        # jumptable_eu_805394E8, D7FC -> 8053953C, D990 -> 80539590,
        # DD6C -> 805395E4, C4F4 (0x2C table) -> 805394BC.
        exact_renames=(
            ("@9022", "jumptable_eu_805394BC"),
            ("@8741", "jumptable_eu_805394E8"),
            ("@8768", "jumptable_eu_8053953C"),
            ("@8599", "jumptable_eu_80539590"),
            ("@8700", "jumptable_eu_805395E4"),
        ),
        extern_data_sections=(".data",),
    ),
    "CSkipTimer.o": UnitRules(
        # int->double pool pair: site order matches retail (lfd f3 at
        # 802A2B58 loads the magic lbl_eu_80668C38, lfd f1 at 802A2BB8 the
        # 2^52 double lbl_eu_80668C40); contents disambiguate as well.
        exact_renames=(
            ("@7276", "lbl_eu_80668C38"),
            ("@7278", "lbl_eu_80668C40"),
        ),
        extern_data_sections=(".sdata2",),
    ),
    "CChainEffect.o": UnitRules(
        # ctor stores the cf::CChainEffect vtable; retail site 802A300C
        # loads it as lbl_eu_80539890.
        exact_renames=(("__vt__Q22cf12CChainEffect", "lbl_eu_80539890"),),
        extern_data_sections=(".data", ".rodata", ".sdata"),
    ),
    "CCharVoiceMan.o": UnitRules(
        # The lone .sbss static already carries its retail name
        # (lbl_eu_80664A58); UNDEF resolves it to the split1 copy.
        extern_data_sections=(".sbss",),
    ),
    "CVS_THREAD.o": UnitRules(
        # The 0x24 zero table is the Move switch jumptable; retail loads it
        # via lis/addi jumptable_eu_80539930 (@ha/@l) at 802A6D70.
        exact_renames=(("@517", "jumptable_eu_80539930"),),
        extern_data_sections=(".data", ".rodata", ".sdata"),
    ),
    "CVS_THREAD_HP.o": UnitRules(
        # int->double magic; content-equal unit-ref'd label (retail site
        # 802A9A38 lfd f2).
        exact_renames=(("@2702", "lbl_eu_80668CA8"),),
        extern_data_sections=(".data", ".rodata", ".sdata", ".sdata2"),
    ),
    "CMenuBattleChain.o": UnitRules(
        # Two switch jumptables map by case-target function: Move ->
        # jumptable_eu_80539E00, func_802AA588 -> jumptable_eu_80539E24.
        # The .sbss statics already carry their retail lbl_eu_80664Axx names.
        exact_renames=(
            ("@7268", "jumptable_eu_80539E00"),
            ("@7321", "jumptable_eu_80539E24"),
        ),
        extern_data_sections=(".data", ".sbss"),
    ),
    "CNandData.o": UnitRules(
        # Singleton sInstance (0x328 bss) -> lbl_eu_80577358 and the 0xc
        # banner-path buffer -> lbl_eu_80577348 (retail __sinit_ sites
        # 802B11D4/125C load exactly these two labels).
        exact_renames=(
            ("sInstance__9CNandData", "lbl_eu_80577358"),
            ("@1745", "lbl_eu_80577348"),
        ),
        extern_data_sections=(".bss",),
    ),
    "CVS_THREAD_BATTLE_MAIN.o": UnitRules(
        # int->double magic (content match; retail sites lfd f2/f4 around
        # 802B1C50 use this label).
        exact_renames=(("@2721", "lbl_eu_80668EB8"),),
        extern_data_sections=(".sdata2",),
    ),
    "CfHikariItemManager.o": UnitRules(
        # int->double magic; content-equal unit-ref'd label (first retail
        # use site 802B6074 lfd f2).
        exact_renames=(("@9335", "lbl_eu_80668EF0"),),
        extern_data_sections=(".sdata2",),
    ),
    "CTitle.o": UnitRules(
        # CTitleLogo ctor stores the vtable; retail site 802B8D9C loads it
        # as lbl_eu_8053B368. The .sdata2 slot is the 2^52 double loaded by
        # both remaining lfd f2 sites (retail 802B9070/802B9804 ->
        # lbl_eu_80668FE8). Remaining local data is unreferenced.
        exact_renames=(
            ("__vt__10CTitleLogo", "lbl_eu_8053B368"),
            ("@7665", "lbl_eu_80668FE8"),
        ),
        extern_data_sections=(".data", ".rodata", ".sdata", ".sdata2"),
    ),
    # ------------------------------------------------------------------
    # kyoshin data-dissolve batch (shared split1.s blob pools/jumptables).
    # Float/double pool renames are CONTENT-keyed (@N drift-proof); zero
    # tables use data_pool_patterns when their size is unique in the TU,
    # else exact_renames on the current @N indices.
    # ------------------------------------------------------------------
    "CfGimmickObject.o": UnitRules(
        # int->double magic pair (content match; retail sda21 sites load
        # lbl_eu_806681A8/81C0). The lone .data table is the 40-byte
        # function-pointer dispatch base loaded via lis/addi lbl_eu_805765A0
        # (retail func_801F9C70/801F9C80).
        # data_pool_patterns (NOT pool_patterns): the .data table sits at
        # st_value 0, and rename_pool_symbols matches every @N symbol
        # against .sdata2 bytes at st_value regardless of home section -
        # the table was being stolen by the float pattern. Section-aware
        # matcher pins the float rename to .sdata2; effective mapping is
        # unchanged (pool starts with the LO pair, so only lbl_eu_806681C0
        # ever bound under used_targets).
        data_pool_patterns=(
            (".sdata2", struct.pack(">II", 0x43300000, 0x80000000), "lbl_eu_806681A8"),
            (".sdata2", struct.pack(">II", 0x43300000, 0x00000000), "lbl_eu_806681C0"),
            (".data", bytes(0x28), "lbl_eu_805765A0"),
        ),
        extern_data_sections=(".data", ".sdata2"),
    ),
    "CfObjectTbox.o": UnitRules(
        # Only .text-referenced local is the class vtable (dtor site);
        # retail loads it as lbl_eu_80534FB8 (retail_reloc_map). Sub-vtables,
        # RTTI descriptors and typeinfo-name strings are only referenced by
        # the stripped sections themselves, so strip without renames.
        exact_renames=(
            ("__vt__Q22cf12CfObjectTbox", "lbl_eu_80534FB8"),
            # Derived dtor's base-dtor call: retail chains through CfObjectObj's
            # dtor (forced flat name __dt__800BFA14, defined in CfObjectObj.o).
            ("__dt__Q22cf11CfObjectObjFv", "__dt__800BFA14"),
            # Derived ctor's base-ctor call: same scheme, __ct__cf_CfObjectObj.
            ("__ct__Q22cf11CfObjectObjFv", "__ct__cf_CfObjectObj"),
        ),
        extern_data_sections=(".data", ".rodata", ".sdata"),
    ),
    "CPartyStateWin.o": UnitRules(
        # The whole .data block is one 0x54 switch jumptable; retail lis/addi
        # site loads jumptable_eu_80535288 (retail_reloc_map + asm site).
        data_pool_patterns=((".data", bytes(0x54), "jumptable_eu_80535288"),),
        extern_data_sections=(".data",),
    ),
    "CModelDisp.o": UnitRules(
        # Lone 0.0f pool slot; only 0.0f-valued label this unit's retail asm
        # references is lbl_eu_806681EC (split1.s .float 0).
        pool_patterns=((struct.pack(">I", 0x00000000), "lbl_eu_806681EC"),),
        extern_data_sections=(".sdata2",),
    ),
    "CPartyState.o": UnitRules(
        # Pool trio maps onto the contiguous retail slots 80668228(HI magic)
        # / 80668230(0.0f) / 80668238(2^52) (split1.s values verified).
        pool_patterns=(
            (struct.pack(">I", 0x00000000), "lbl_eu_80668230"),
            (struct.pack(">II", 0x43300000, 0x80000000), "lbl_eu_80668228"),
            (struct.pack(">II", 0x43300000, 0x00000000), "lbl_eu_80668238"),
        ),
        extern_data_sections=(".sdata2",),
    ),
    "pluginUnit.o": UnitRules(
        # int->double signed magic; content-equal label (split1.s .double
        # 4503601774854144 at 80668258).
        pool_patterns=(
            (struct.pack(">II", 0x43300000, 0x80000000), "lbl_eu_80668258"),
        ),
        extern_data_sections=(".sdata2",),
    ),
    "CEquipChange.o": UnitRules(
        # Four zero-init switch jumptables map 1:1 by containing function
        # (retail lis/addi sites verified per .fn block):
        #   func_802028E4 (rel 0x54 site) -> 805355DC (@80204630/38)
        #   func_80202A70 (rel 0x54 site) -> 80535614 (@802047bc/c4)
        #   func_802031A0 (rel 0x10 site) -> 8053564C (@80204ea8/b0)
        #   func_80202110 (rel 0x2c site) -> 805355B0 (@80203e34/3c)
        # FRAGILE @N: indices renumber on any TU-affecting change (observed
        # @8441/... -> @10437/@10461/@11327/@11341 after the 2026-08-26
        # refactor). Refresh protocol: disable key -> hexdiff --all raw ->
        # capture table syms + owning funcs from .rela.data addend clusters
        # -> rewrite numbers. While stale, entries can MISFIRE onto
        # unrelated symbols - neutralize (exact_renames=()) instead of
        # leaving stale numbers during active churn.
        exact_renames=(
            ("@10437", "jumptable_eu_805355DC"),
            ("@10461", "jumptable_eu_80535614"),
            ("@11327", "jumptable_eu_8053564C"),
            ("@11341", "jumptable_eu_805355B0"),
        ),
        extern_data_sections=(".data",),
    ),
    "CfMapMineManager.o": UnitRules(
        # Retail pool cluster 806682B0(0.0f)/B8(HI magic)/C0(2^52); source
        # now converts int->float through the shared doubles so no artifact
        # 1e-6f/160.0f locals remain (timer epsilon is extern lbl_eu_8066A208
        # from monolibdata1, matching retail lfs f0,8066A208@80208B60).
        pool_patterns=(
            (struct.pack(">I", 0x00000000), "lbl_eu_806682B0"),
            (struct.pack(">II", 0x43300000, 0x80000000), "lbl_eu_806682B8"),
            (struct.pack(">II", 0x43300000, 0x00000000), "lbl_eu_806682C0"),
        ),
        extern_data_sections=(".sdata2",),
    ),
    "CItemBoxGridSubMenu.o": UnitRules(
        # Texture-size conversion uses the unsigned 2^52 double (retail lfd
        # f2, lbl_eu_806682F8@80209FF8 after (f32)w/u16 fix); 1.0f slot maps
        # onto the same-value retail label 8066831C the TU already refs.
        pool_patterns=(
            (struct.pack(">I", 0x3F800000), "lbl_eu_8066831C"),
            (struct.pack(">II", 0x43300000, 0x00000000), "lbl_eu_806682F8"),
        ),
        extern_data_sections=(".sdata2",),
    ),
    "CfGimmick.o": UnitRules(
        # Pool trio: retail slots 80668350(.float 0)/80668368(HI)/80668370
        # (2^52), ctx votes 9-12 strong per slot.
        pool_patterns=(
            (struct.pack(">I", 0x00000000), "lbl_eu_80668350"),
            (struct.pack(">II", 0x43300000, 0x80000000), "lbl_eu_80668368"),
            (struct.pack(">II", 0x43300000, 0x00000000), "lbl_eu_80668370"),
        ),
        extern_data_sections=(".sdata2",),
    ),
    "CfGimmickElv.o": UnitRules(
        # Single 2^52 conversion double; content-equal unit-ref label
        # lbl_eu_80668388 (split1.s .double 4503599627370496).
        pool_patterns=(
            (struct.pack(">II", 0x43300000, 0x00000000), "lbl_eu_80668388"),
        ),
        extern_data_sections=(".sdata2",),
    ),
    "CfGimmickWarp.o": UnitRules(
        # Signed int->double magic; lbl_eu_806683F0 = 0x4330000080000000.
        pool_patterns=(
            (struct.pack(">II", 0x43300000, 0x80000000), "lbl_eu_806683F0"),
        ),
        extern_data_sections=(".sdata2",),
    ),
    "CfGimmickJump.o": UnitRules(
        # After the (f32)value source fix only the two conversion doubles
        # remain; retail ctor sites load lbl_eu_80668410 (HI, signed) and
        # lbl_eu_80668418 (LO, unsigned) alternately (asm 80210E2C ff.).
        # Source previously mis-defined lbl_eu_80668410 locally with the LO
        # value - removed; both ship from split1.s.
        pool_patterns=(
            (struct.pack(">II", 0x43300000, 0x00000000), "lbl_eu_80668418"),
            (struct.pack(">II", 0x43300000, 0x80000000), "lbl_eu_80668410"),
        ),
        extern_data_sections=(".sdata2",),
    ),
    "CMenuMakeCrystal.o": UnitRules(
        # The lone .sbss static already carries its retail name
        # (lbl_eu_806646C8); UNDEF resolves to the split1 copy.
        extern_data_sections=(".sbss",),
    ),
    "code_80213488.o": UnitRules(
        # Conversion double pair; content matches lbl_eu_80668460(2^52) /
        # lbl_eu_80668468(HI) (ctx votes agree).
        pool_patterns=(
            (struct.pack(">II", 0x43300000, 0x00000000), "lbl_eu_80668460"),
            (struct.pack(">II", 0x43300000, 0x80000000), "lbl_eu_80668468"),
        ),
        extern_data_sections=(".sdata2",),
    ),
    "CMCCrystalBox.o": UnitRules(
        # 2^52 double -> lbl_eu_80668460; the 0x3C zero .data table is the
        # switch jumptable loaded via lis/addi jumptable_eu_80535C20 at the
        # head of retail func_802142C4 (decomp rel 0x36/0x3e site).
        # Float uses data_pool_patterns: pool_patterns matches .data syms
        # against .sdata2 content at st_value (table val=0 was stolen).
        data_pool_patterns=(
            (".sdata2", struct.pack(">II", 0x43300000, 0x00000000), "lbl_eu_80668460"),
            (".data", bytes(0x3C), "jumptable_eu_80535C20"),
        ),
        extern_data_sections=(".data", ".sdata2"),
    ),
    "CMCCrystalInfo.o": UnitRules(
        # Lone 1.0f slot; lbl_eu_80668498 object starts .float 1 (ctx 6/6).
        pool_patterns=((struct.pack(">I", 0x3F800000), "lbl_eu_80668498"),),
        extern_data_sections=(".sdata2",),
    ),
    "CModelDispMakeCrystal.o": UnitRules(
        # 2^52 double -> lbl_eu_806684F0 (majority ctx votes + unique
        # content match; 806684F8 is .float 100). Two zero .data tables are
        # switch jumptables paired by containing function: func_8021FEDC
        # head -> jumptable_eu_80535DA8, func_80220954 head ->
        # jumptable_eu_80535E4C.
        pool_patterns=(
            (struct.pack(">II", 0x43300000, 0x00000000), "lbl_eu_806684F0"),
        ),
        data_pool_patterns=(
            (".data", bytes(0xA0), "jumptable_eu_80535DA8"),
            (".data", bytes(0x20), "jumptable_eu_80535E4C"),
        ),
        extern_data_sections=(".data", ".sdata2"),
    ),
    "CMCCylinderGauge.o": UnitRules(
        # Full float-pool dissolve; every slot content-matches its retail
        # label (ctx votes 7/3/1/1/1/7) and the string pool base maps via
        # retail_reloc_map (@stringBase0 -> lbl_eu_80509224).
        pool_patterns=(
            (struct.pack(">I", 0x00000000), "lbl_eu_80668518"),
            (struct.pack(">II", 0x43300000, 0x00000000), "lbl_eu_80668520"),
            (struct.pack(">I", 0x41F00000), "lbl_eu_80668528"),
            (struct.pack(">I", 0x3C23D70A), "lbl_eu_8066852C"),
            (struct.pack(">II", 0x43300000, 0x80000000), "lbl_eu_80668530"),
            (struct.pack(">I", 0x3F800000), "lbl_eu_80668538"),
        ),
        exact_renames=(("@stringBase0", "lbl_eu_80509224"),),
        extern_data_sections=(".sdata2", ".rodata"),
    ),
    "CMCCrystalList.o": UnitRules(
        # 0.0f -> lbl_eu_80668540, 2^52 -> lbl_eu_80668548 (ctx votes);
        # the 0x20 zero .data table is the func_802228B8 switch jumptable
        # (retail lis/addi jumptable_eu_80535F58 at block head).
        # Float side uses data_pool_patterns (pool_patterns matched .data
        # tables against .sdata2 bytes at st_value=0). The .sdata2 pool is
        # TWO symbols - 0.0f -> lbl_eu_80668540 and the 2^52 double ->
        # lbl_eu_80668548 - so both mappings are kept explicitly.
        data_pool_patterns=(
            (".sdata2", struct.pack(">I", 0x00000000), "lbl_eu_80668540"),
            (".sdata2", struct.pack(">II", 0x43300000, 0x00000000), "lbl_eu_80668548"),
            (".data", bytes(0x20), "jumptable_eu_80535F58"),
        ),
        extern_data_sections=(".data", ".sdata2"),
    ),
    "CQstLogList.o": UnitRules(
        # 2^52 conversion double; content-equal unit-ref lbl_eu_80668588.
        pool_patterns=(
            (struct.pack(">II", 0x43300000, 0x00000000), "lbl_eu_80668588"),
        ),
        extern_data_sections=(".sdata2",),
    ),
    "CQstLogInfo.o": UnitRules(
        # Two identical 0x20 zero jumptables disambiguated by use-site
        # offsets inside func_8022A904: decomp rel 238/370 -> retail pair at
        # 228/364 (jumptable_eu_80536440); decomp 1110/1362 -> retail
        # 1088/1344 (jumptable_eu_80536408).
        # FRAGILE @N (renumbered @8673/@8674 -> @8682/@8683 by intervening
        # TU churn; missed renames leave dangling U @N lfd/jumptable refs
        # that the data gate cannot see). Re-verify via hexdiff rebuild +
        # .rela.text scan after any shared-header/source change.
        exact_renames=(
            ("@8682", "jumptable_eu_80536440"),
            ("@8683", "jumptable_eu_80536408"),
        ),
        extern_data_sections=(".data",),
    ),
    "CSysWin.o": UnitRules(
        # 2^52 conversion double; content-equal unit-ref lbl_eu_806685E8.
        pool_patterns=(
            (struct.pack(">II", 0x43300000, 0x00000000), "lbl_eu_806685E8"),
        ),
        extern_data_sections=(".sdata2",),
    ),
    "CSelShopWin.o": UnitRules(
        # 2^52 conversion double; content-equal unit-ref lbl_eu_80668608.
        pool_patterns=(
            (struct.pack(">II", 0x43300000, 0x00000000), "lbl_eu_80668608"),
        ),
        extern_data_sections=(".sdata2",),
    ),
    "CExchangeWin.o": UnitRules(
        # 2^52 conversion double; content + reloc-map + ctx all agree on
        # lbl_eu_80668618.
        pool_patterns=(
            (struct.pack(">II", 0x43300000, 0x00000000), "lbl_eu_80668618"),
        ),
        extern_data_sections=(".sdata2",),
    ),
    "CMCCrystalSupport.o": UnitRules(
        # Lone 0.0f slot; lbl_eu_80668630 = .float 0 (ctx + reloc map).
        pool_patterns=((struct.pack(">I", 0x00000000), "lbl_eu_80668630"),),
        extern_data_sections=(".sdata2",),
    ),
    "CMenuArtsSet.o": UnitRules(
        # Four identical 0x24 zero jumptables paired by use-site offsets
        # (keys refreshed to current MWCC anon numbering after TU rebuild):
        # func_802308B0 rel 578/586 -> jumptable_eu_8053689C (retail
        # 584/592), rel 806/814 -> jumptable_eu_80536878 (retail 808/816);
        # func_80231014 rel 98/106 -> jumptable_eu_805368E4 (retail 96/104,
        # also retail_reloc_map), rel 322/330 -> jumptable_eu_805368C0
        # (retail 320/328). Remaining pool slot is the HI conversion double
        # lbl_eu_80668670.
        exact_renames=(
            ("@13056", "jumptable_eu_8053689C"),
            ("@13057", "jumptable_eu_80536878"),
            ("@13152", "jumptable_eu_805368E4"),
            ("@13153", "jumptable_eu_805368C0"),
        ),
        pool_patterns=(
            (struct.pack(">II", 0x43300000, 0x80000000), "lbl_eu_80668670"),
        ),
        extern_data_sections=(".data", ".sdata2"),
    ),
    "CQuestWindow.o": UnitRules(
        # 2^52 conversion double -> lbl_eu_80667160: retail lfd f29@80123A58
        # + two lfd f2 sites; source externs cover one use, MWCC pools its
        # own copy (@N in .sdata2) for the others - content match dissolves
        # it. The two 8-entry switch tables in func_801231C4 pair onto the
        # retail 14-entry tables by CASE-LABEL CLUSTER (strongest evidence)
        # + dispatch register:
        #   .data@0x20 (lis r31/addi r3,r31) entries 64C..6D8
        #     -> jumptable_eu_8052D0B8 (retail r31 @80124264/801242EC;
        #        entries 660..708 low cluster)
        #   .data@0x0  (lis r25/addi r3,r25) entries 938..9C4
        #     -> jumptable_eu_8052D080 (retail r27 @80124598/801245F8;
        #        entries 96C..A14 high cluster)
        # FRAGILE: exact_renames pin MWCC anon @N indices that renumber on
        # any TU-affecting source/header change (observed live: stale
        # @13241/@13242 vs fresh @13247/@13248). Failure mode is loud -
        # missed rename leaves U @N visible in nm and the second table's
        # relocs dangling-named - so re-run hexdiff --all + nm after any
        # CQuestWindow.cpp/shared-header churn and refresh the numbers.
        # The lone .sbss static already carries its retail name
        # lbl_eu_80663D1C (split1 .skip 4); strip->UNDEF resolves at link.
        exact_renames=(
            ("@13247", "jumptable_eu_8052D0B8"),
            ("@13248", "jumptable_eu_8052D080"),
        ),
        # NOTE: deliberately data_pool_patterns, NOT pool_patterns:
        # rename_pool_symbols matches every nm @N symbol against .sdata2
        # content at the symbol's st_value REGARDLESS of its home section,
        # so the val=0 .data table @13248 collided with the double's bytes
        # and was stolen by the float rename before exact_renames could
        # fire. The section-aware matcher pins the pattern to .sdata2.
        data_pool_patterns=(
            (".sdata2", struct.pack(">II", 0x43300000, 0x00000000), "lbl_eu_80667160"),
        ),
        extern_data_sections=(".data", ".sdata2", ".sbss"),
    ),

    # --- CriWare sofdec data dissolve onto the criware_data.s shared pool ---
    # (cftyp422_ppc / mwsfdcre conversion-magic pools were later eliminated at
    # source level - bias loaded through the externed table / no local pool -
    # so their rules were removed; the units data-match raw.)

    "mwsfdply.o": UnitRules(
        # Same lone conversion-magic pool (@265) -> blob double
        # lbl_eu_8051B198 (MWSFPLY_SetFlowLimit lfd site; sibling chunk
        # lbl_eu_8051B190 is the 0.8 literal already externed).
        data_pool_patterns=(
            (".rodata", struct.pack(">II", MAGIC_HI, MAGIC_LO), "lbl_eu_8051B198"),
        ),
        globalize_symbols=("lbl_eu_8051B198",),
        extern_data_sections=(".rodata",),
    ),

    "mwsfdcre.o": UnitRules(
        # MWCC re-pools the int->double conversion bias locally (@438 in
        # .rodata) even though the TU already externs it; retail lfd's the
        # shared criware_data.s copy lbl_eu_8051A3C0 (.double 4503601774854144)
        # at 0x803A210C and 0x803A3240. Mirrors the mwsfdply.o rule.
        data_pool_patterns=(
            (".rodata", struct.pack(">II", MAGIC_HI, MAGIC_LO), "lbl_eu_8051A3C0"),
        ),
        globalize_symbols=("lbl_eu_8051A3C0",),
        extern_data_sections=(".rodata",),
    ),

    "mpv_mc.o": UnitRules(
        # .data is MWCC's 8-slot switch jumptable (@555) for the alignment
        # dispatch in MPVMC08_OneRef1p_TuneC; retail indexes the shared blob
        # copy jumptable_eu_80567298 (.data:0x7270). The zero-content match
        # is unique in this TU.
        data_pool_patterns=(
            (".data", bytes(0x20), "jumptable_eu_80567298"),
        ),
        globalize_symbols=("jumptable_eu_80567298",),
        extern_data_sections=(".data",),
    ),

    "mpv_mcy.o": UnitRules(
        # Same-shaped OneRef1p jumptable (@262) -> blob jumptable_eu_805672B8
        # (.data:0x7290).
        data_pool_patterns=(
            (".data", bytes(0x20), "jumptable_eu_805672B8"),
        ),
        globalize_symbols=("jumptable_eu_805672B8",),
        extern_data_sections=(".data",),
    ),

    "mpvabdec.o": UnitRules(
        # Three identical 0x400 zero jumptables (256-way case dispatch, one
        # per Isr function); content matching cannot disambiguate them, so
        # pair by symbol (order-stable while the Isr bodies are untouched):
        # IntraBlock_Isr @1350 -> jumptable_eu_805672D8 (.data:0x72B0),
        # IntraBlockDc11_Isr @2691 -> jumptable_eu_805676D8 (.data:0x76B0),
        # NintraBlock_Isr @4066 -> jumptable_eu_80567AD8 (.data:0x7AB0).
        exact_renames=(
            ("@1350", "jumptable_eu_805672D8"),
            ("@2691", "jumptable_eu_805676D8"),
            ("@4066", "jumptable_eu_80567AD8"),
        ),
        # Renamed jumptable entries stay LOCAL UNDEF after exact_renames;
        # globalize so the final mwldeppc link binds them to criware_data.o.
        globalize_symbols=(
            "jumptable_eu_805672D8",
            "jumptable_eu_805676D8",
            "jumptable_eu_80567AD8",
        ),
        extern_data_sections=(".data",),
    ),

    # ------------------------------------------------------------------
    # CriWare ADX/AHX data-dissolve: retail keeps every constant and
    # work area in criware_data.o (blob labels), so the decomp TUs must
    # emit no local copies.
    # ------------------------------------------------------------------
    "adx_fini.o": UnitRules(
        # ADXF work struct is blob .bss lbl_eu_805DFDA8 (0x880, split into
        # per-field labels in the blob but contiguous at link); the source
        # already defines it under the retail name, so strip storage only.
        extern_data_sections=(".bss",),
    ),
    "adx_tlk.o": UnitRules(
        # Under current lib flags the (float)(s32) conversions in
        # ADXT_GetTimeReal / adxt_Pause / ADXT_DiscardSmpl pool the
        # hi-magic (@589); retail lfd's it from lbl_eu_805162D8
        # (adxt_Create site 803858F4 / GetTime sites 8038731C..).
        # NOTE: ADXT_BiasDouble inlines under Wii/1.1 and keeps
        # adxt_GetTime/adxt_Create pool-free; only these three sites pool.
        data_pool_patterns=(
            (".rodata", struct.pack(">II", MAGIC_HI, MAGIC_LO), "lbl_eu_805162D8"),
        ),
        globalize_symbols=("lbl_eu_805162D8",),
        extern_data_sections=(".rodata",),
    ),
    "adx_dcd.o": UnitRules(
        # Whole 0x28 float pool (2pi, 2.0, 1.0f, 4096.0f, 2.0, hi-magic)
        # equals blob object lbl_eu_80517468 byte-for-byte from its base;
        # MWCC anchors it under ...rodata.0 (the only .text-referenced
        # symbol; retail lis/addi site 8038CD70).
        exact_renames=(("...rodata.0", "lbl_eu_80517468"),),
        globalize_symbols=("lbl_eu_80517468",),
        extern_data_sections=(".rodata",),
    ),
    "ahx_sbf.o": UnitRules(
        # AHX_SBF status word already defined under the retail name
        # lbl_eu_80517560; strip the local definition so the blob copy owns
        # it.
        extern_data_sections=(".rodata",),
    ),
    "ahx_dcd.o": UnitRules(
        # Lone s32->f64 hi-magic double (@755); retail lfd's it from
        # lbl_eu_80517630 (site 8039226C lfd f31). The direct extern refs
        # to lbl_eu_80517628/38 already carry retail names.
        data_pool_patterns=(
            (".rodata", struct.pack(">II", MAGIC_HI, MAGIC_LO), "lbl_eu_80517630"),
        ),
        globalize_symbols=("lbl_eu_80517630",),
        extern_data_sections=(".rodata",),
    ),

    # ------------------------------------------------------------------
    # CriWare sofdec data-dissolve batch.
    # ------------------------------------------------------------------
    "dct_isr.o": UnitRules(
        # Local {zero-double, hi-magic} pair lives inside the DCT
        # coefficient table lbl_eu_8051C3B0 at +0x78/+0x68 (blob tail is
        # ..., 2097152, hi-magic, 4194304, 0). Retail reaches both via
        # base+disp off C3B0, so retarget the four anchor relocs with
        # matching addends (@213=zero-dbl, @215=magic).
        retarget_relocs=(
            (".text", 0x156, "lbl_eu_8051C3B0"),
            (".text", 0x15E, "lbl_eu_8051C3B0"),
            (".text", 0x17A, "lbl_eu_8051C3B0"),
            (".text", 0x182, "lbl_eu_8051C3B0"),
        ),
        addend_sets=(
            (".text", 0x156, 0x78),
            (".text", 0x17A, 0x78),
            (".text", 0x15E, 0x68),
            (".text", 0x182, 0x68),
        ),
        extern_data_sections=(".rodata",),
    ),
    "sfd_adxt.o": UnitRules(
        # SFADXT_SetSpeed's (double)speed/(double)base casts pool a local
        # hi-magic (@59); retail builds the stack double inline and
        # subtracts *(f64*)(lbl_eu_8051C4E0+0x60) instead (no dedicated
        # label). Dissolve onto equivalent-value blob magic
        # lbl_eu_8051CF40; an ADXT_BiasDouble-style source reshape makes
        # this rule inert later.
        data_pool_patterns=(
            (".rodata", struct.pack(">II", MAGIC_HI, MAGIC_LO), "lbl_eu_8051CF40"),
        ),
        globalize_symbols=("lbl_eu_8051CF40",),
        extern_data_sections=(".rodata",),
    ),
    "sfd_tim.o": UnitRules(
        # Lone ...rodata.0 access site (SFD_CalcCycleFromFps) lfd's local
        # +0x28 = the hi-magic; blob copy sits at lbl_eu_8051CC20+0x30
        # (float table starts at CC20+0x8). Retarget with the spanning
        # addend.
        retarget_relocs=(
            (".text", 0x1B66, "lbl_eu_8051CC20"),
            (".text", 0x1B72, "lbl_eu_8051CC20"),
        ),
        addend_sets=(
            (".text", 0x1B66, 0x30),
            (".text", 0x1B72, 0x30),
        ),
        extern_data_sections=(".rodata",),
    ),
    "sfh_ver1.o": UnitRules(
        # criware_803D2C98's 9-way pic-rate switch jumptable (@1074, 0x24
        # bytes of ADDR32s back into the function) is blob
        # jumptable_eu_80568F10 (.data; entries criware_803D2C98+offsets;
        # retail lis/addi pair references it directly).
        data_pool_patterns=((".data", bytes(0x24), "jumptable_eu_80568F10"),),
        globalize_symbols=("jumptable_eu_80568F10",),
        extern_data_sections=(".data",),
    ),
    "sfx_zmv.o": UnitRules(
        # Both .rodata anchors resolve to local 0 = the hi-magic; retail
        # only ever lfd's lbl_eu_8051D220@l disp 0 (sites 803D8A98/
        # 803D8C54). @563..@633 pool copies are unreferenced residue.
        data_pool_patterns=(
            (".rodata", struct.pack(">II", MAGIC_HI, MAGIC_LO), "lbl_eu_8051D220"),
        ),
        exact_renames=(("...rodata.0", "lbl_eu_8051D220"),),
        globalize_symbols=("lbl_eu_8051D220",),
        extern_data_sections=(".rodata",),
    ),
    "sfx_cnv.o": UnitRules(
        # SFXCNV_MakeCcirFromY's {0.5f, 1.164f, hi-magic} pool maps 1:1
        # onto the dedicated blob labels lbl_eu_8051CF38/3C/40 (retail
        # lfs/lfd sites 803D734C-803D735C read exactly these three).
        # (.data jumptable + error strings were fixed at source level.)
        data_pool_patterns=(
            (".rodata", struct.pack(">I", 0x3F000000), "lbl_eu_8051CF38"),
            (".rodata", struct.pack(">I", 0x3F94FDF4), "lbl_eu_8051CF3C"),
            (".rodata", struct.pack(">II", MAGIC_HI, MAGIC_LO), "lbl_eu_8051CF40"),
        ),
        globalize_symbols=(
            "lbl_eu_8051CF38",
            "lbl_eu_8051CF3C",
            "lbl_eu_8051CF40",
        ),
        extern_data_sections=(".rodata", ".data"),
    ),
    # ------------------------------------------------------------------
    # kyoshin data-dissolve batch 2 (main/kyoshin game units; retail data
    # lives in split1.s / unit-local slices). Float/double magics are
    # CONTENT-keyed pool_patterns (@N drift-proof); targets verified
    # against each unit's retail asm sda21/lis+addi sites.
    # ------------------------------------------------------------------
    "CfCollCircleImpl.o": UnitRules(
        # int->double 2^52 magic; retail loads lbl_eu_80666920 (= .double
        # 4503599627370496) at both lfd sites in this split.
        pool_patterns=(
            (struct.pack(">II", MAGIC_HI, 0x00000000), "lbl_eu_80666920"),
        ),
        extern_data_sections=(".sdata2",),
    ),
    "code_800F42AC.o": UnitRules(
        # unsigned int->double magic {2^52, 2^31}; retail site lfd's
        # lbl_eu_80666EA8 (.double 4503601774854144).
        pool_patterns=(
            (struct.pack(">II", MAGIC_HI, MAGIC_LO), "lbl_eu_80666EA8"),
        ),
        extern_data_sections=(".sdata2",),
    ),
    "CfObjectPc.o": UnitRules(
        # unsigned int->double magic; retail lfd site uses lbl_eu_80666B30.
        pool_patterns=(
            (struct.pack(">II", MAGIC_HI, MAGIC_LO), "lbl_eu_80666B30"),
        ),
        extern_data_sections=(".sdata2",),
    ),
    "CfObjectEne.o": UnitRules(
        # Five float/double pool slots -> split1.s labels (each content
        # matches exactly one reloc-referenced local; all are retail-ref'd
        # sda21 sites in cf/object/CfObjectEne.s). Leading zero word is an
        # unreferenced orphan - stripped with the section.
        pool_patterns=(
            (struct.pack(">II", MAGIC_HI, 0x00000000), "lbl_eu_806669A0"),   # 2^52
            (struct.pack(">II", MAGIC_HI, MAGIC_LO), "lbl_eu_806669A8"),     # 2^52+2^31
            (struct.pack(">II", 0x3FE00000, 0x00000000), "lbl_eu_806669B8"), # 0.5
            (struct.pack(">II", 0xBFE00000, 0x00000000), "lbl_eu_806669C0"), # -0.5
            (struct.pack(">I", 0x3F800000), "lbl_eu_80666980"),              # 1.0f
            # 0.0f float-zero: referenced by CActorParam_UnkVirtualFunc148's
            # entry-init (retail loads lbl_eu_80666968; MWCC pools an anon
            # @N slot for the literal-0 float stores).
            (struct.pack(">I", 0x00000000), "lbl_eu_80666968"),
        ),
        extern_data_sections=(".sdata2",),
    ),
    "CfObjectEnumList.o": UnitRules(
        # Unsigned magic maps to lbl_eu_80666EC0 (ref'd x3); the 2^52 slot
        # is an unreferenced orphan - stripped with the section.
        pool_patterns=(
            (struct.pack(">II", MAGIC_HI, MAGIC_LO), "lbl_eu_80666EC0"),
        ),
        extern_data_sections=(".sdata2",),
    ),
    "CfObjectColl.o": UnitRules(
        # Class vtable (dtor lis/addi vptr store, retail 800ABD14) = split1.s
        # lbl_eu_80528600 (0x178). The adjacent lbl_eu_805285E8 (0x18,
        # CfObjectPoint sub-vtable lead-in) is referenced by name from
        # initCollImplInstances' lookup table, NOT by the dtor. RTTI
        # descriptors/typeinfo-name strings ride along in the strip.
        exact_renames=(("__vt__Q22cf12CfObjectColl", "lbl_eu_80528600"),),
        extern_data_sections=(".data", ".rodata", ".sdata"),
    ),
    "CfObjectEff.o": UnitRules(
        # Base CfObject vtable (0x178, text-ref'd from __dt__) =
        # split1.s lbl_eu_805294E0; CObjectParam/CObjectState sub-vtables,
        # RTTI and name strings ride along in the strip.
        exact_renames=(("__vt__Q22cf8CfObject", "lbl_eu_805294E0"),),
        extern_data_sections=(".data", ".rodata", ".sdata"),
    ),
    "CfObjectModel.o": UnitRules(
        # CfObjectModel vtable (0x1C8) = split1.s lbl_eu_80529318.
        exact_renames=(("__vt__Q22cf13CfObjectModel", "lbl_eu_80529318"),),
        extern_data_sections=(".data", ".rodata", ".sdata"),
    ),
    "CfObjectNpc.o": UnitRules(
        # Retail merges the {0,0x8c,0} leading words and the 0x224 vtable
        # into ONE object lbl_eu_805298B8 (0x230); both text-referenced
        # locals map onto its base (retail ctor/dtor store the base).
        # @518 goes through data_pool_patterns (unique leading-content
        # match) because objcopy rejects two --redefine-sym to one target
        # in a single pass; the vtable rename is a later separate pass.
        data_pool_patterns=(
            (".data", bytes.fromhex("00000000000000" "8c" "00000000"), "lbl_eu_805298B8"),
        ),
        exact_renames=(("__vt__Q22cf11CfObjectNpc", "lbl_eu_805298B8"),),
        extern_data_sections=(".data", ".rodata", ".sdata"),
    ),
    "CfObjectImplEne.o": UnitRules(
        # No .text references into .data/.rodata/.sdata (vtable/RTTI are
        # emitted but unused here); strip without renames.
        extern_data_sections=(".data", ".rodata", ".sdata"),
    ),
    "CfObjectImplPc.o": UnitRules(
        # Source now builds; jumptable elided, remaining locals are the
        # int->double magic pair: 2^52 -> lbl_eu_80666BD8, unsigned ->
        # lbl_eu_80666BE0 (both unit-ref'd lfd sites).
        pool_patterns=(
            (struct.pack(">II", MAGIC_HI, 0x00000000), "lbl_eu_80666BD8"),
            (struct.pack(">II", MAGIC_HI, MAGIC_LO), "lbl_eu_80666BE0"),
        ),
        extern_data_sections=(".data", ".sdata2"),
    ),
    "CfObjectImplMove.o": UnitRules(
        # Switch jumptables -> split1.s jumptable_eu_8052AB40 (11 slots,
        # func_800CAB30 cases) and jumptable_eu_8052AB6C (func_800CC964
        # cases); 1.0f pool slot -> lbl_eu_80666C64 (content match).
        exact_renames=(
            ("@4847", "jumptable_eu_8052AB40"),
            ("@5098", "jumptable_eu_8052AB6C"),
        ),
        pool_patterns=(
            (struct.pack(">I", 0x3F800000), "lbl_eu_80666C64"),
        ),
        extern_data_sections=(".data", ".sdata2"),
    ),
    "code_800AA008.o": UnitRules(
        # Two switch jumptables map by case-target function:
        # func_800AA008 dispatch -> jumptable_eu_805285A0 (site 800AA910),
        # func_800AA714 dispatch -> jumptable_eu_805285C4 (site 800AB170).
        exact_renames=(
            ("@1928", "jumptable_eu_805285A0"),
            ("@2028", "jumptable_eu_805285C4"),
        ),
        extern_data_sections=(".data",),
    ),
    "code_800B06A4.o": UnitRules(
        # Whole .data is the func_800B97A0 switch jumptable ->
        # jumptable_eu_80529058 (retail site 800BA0F0, same 18 slots).
        # Unsigned magic -> lbl_eu_80666A20; leading zero word is an
        # unreferenced orphan stripped with the section.
        exact_renames=(("@6794", "jumptable_eu_80529058"),),
        pool_patterns=(
            (struct.pack(">II", MAGIC_HI, MAGIC_LO), "lbl_eu_80666A20"),
        ),
        extern_data_sections=(".data", ".sdata2"),
    ),
    "CMainMenu.o": UnitRules(
        # Whole .data is the func_800FEF4C menu-dispatch jumptable ->
        # jumptable_eu_8052BE00 (retail site 800FFB58; leading slots
        # +0x13C/+0x150 identical).
        exact_renames=(("@8000", "jumptable_eu_8052BE00"),),
        extern_data_sections=(".data",),
    ),
    "CMenuFade.o": UnitRules(
        # Shared CTTask<IUICf> template vtable for this split ->
        # lbl_eu_8052BF70 (unit asm site). sdata2 float cluster maps by
        # content: 0.0f->80667058, 1.0f->8066705C, 25.0f->80667060,
        # 2^52 magic->80667068 (all unit-ref'd). RTTI/vtable hierarchy
        # and typeinfo-name strings are not text-referenced; strip.
        exact_renames=(("__vt__14CTask<5IUICf>", "lbl_eu_8052BF70"),),
        pool_patterns=(
            (struct.pack(">I", 0x00000000), "lbl_eu_80667058"),
            (struct.pack(">I", 0x3F800000), "lbl_eu_8066705C"),
            (struct.pack(">I", 0x41C80000), "lbl_eu_80667060"),
            (struct.pack(">II", MAGIC_HI, 0x00000000), "lbl_eu_80667068"),
        ),
        extern_data_sections=(".data", ".rodata", ".sdata", ".sdata2"),
    ),
    "CMiniMap.o": UnitRules(
        # No vtable/jumptable text refs in this WIP TU; only the sdata2
        # float cluster needs mapping: 0.0f->80667090, unsigned magic->
        # 80667098, 2^52 magic->806670A8 (all unit-ref'd sites).
        pool_patterns=(
            (struct.pack(">I", 0x00000000), "lbl_eu_80667090"),
            (struct.pack(">II", MAGIC_HI, MAGIC_LO), "lbl_eu_80667098"),
            (struct.pack(">II", MAGIC_HI, 0x00000000), "lbl_eu_806670A8"),
        ),
        extern_data_sections=(".data", ".rodata", ".sdata", ".sdata2"),
    ),
    "CMenuQuestLog.o": UnitRules(
        # Whole .data jumptable for Move__13CMenuQuestLogFv ->
        # jumptable_eu_8052CC10 (same 10 ascending +0xC-step slots);
        # class/base vtables and RTTI chain strip without renames.
        exact_renames=(("@13609", "jumptable_eu_8052CC10"),),
        extern_data_sections=(".data", ".rodata", ".sdata"),
    ),
    "CMenuBattleDamage.o": UnitRules(
        # func_801098B0 dispatch jumptable -> jumptable_eu_8052C200 (same
        # 11 ascending slots); int->double 2^52 magic -> lbl_eu_80666F88
        # (retail lis 0x4330/lfd pair sites). CPcSelectCursor01 vtable/
        # RTTI/string are not text-referenced; strip.
        exact_renames=(("@13286", "jumptable_eu_8052C200"),),
        pool_patterns=(
            (struct.pack(">II", MAGIC_HI, 0x00000000), "lbl_eu_80666F88"),
        ),
        extern_data_sections=(".data", ".rodata", ".sdata", ".sdata2"),
    ),
    "CMenuBattlePlayerState_ct.o": UnitRules(
        # Shared CTTask<IUICf> vtable copy for this split -> lbl_eu_8052C1C0
        # (unit asm ref; each menu split keeps its own retail copy).
        # __ptmf_null tentative def fixed at source (extern; runtime ptmf.o
        # owns the retail storage).
        exact_renames=(("__vt__14CTask<5IUICf>", "lbl_eu_8052C1C0"),),
        extern_data_sections=(".data", ".rodata", ".sdata", ".sdata2"),
    ),
    "CBattleManager.o": UnitRules(
        # Switch jumptables map by owning function + slot shape (decomp
        # case offsets drift from retail because several switch bodies are
        # still WIP-elided; counts and equality-shapes anchor the choice):
        #   func_800EC918: 4 small dispatches (site order eda2c/edb28/edbc8/
        #     edcc4 -> BCB4/BC8C/BC64/BC3C) + the 0x4A0 mega-table -> B79C.
        #   func_800DCB54: six early tables in site order (dde4..e4b4 ->
        #     B3AC/B384/B35C/B334/B30C/B2E4); end cluster (decomp 0x38+2x0x28
        #     at +0x2bea..) -> B1BC/B144/B194 (retail cluster e0d34 loads
        #     B1BC,B144,B194,B16C,B11C; last two serve elided switches).
        #   func_800E08E8: lone 0x38 table -> B3D4 (only site, size drifted).
        #   func_800E2A9C: 0x3C -> B504; [top][asc x9] table -> B43C
        #     (shape-equal, late sites e5824/e597c).
        #   func_800E64CC: 0x38 -> B540 (size-unique); early cluster first
        #     0x28 -> B630; [top][asc x9] -> B5E0 (shape-equal).
        exact_renames=(
            ("@31218", "jumptable_eu_8052B79C"),
            ("@31214", "jumptable_eu_8052BCB4"),
            ("@31215", "jumptable_eu_8052BC8C"),
            ("@31216", "jumptable_eu_8052BC64"),
            ("@31217", "jumptable_eu_8052BC3C"),
            ("@33055", "jumptable_eu_8052B3AC"),
            ("@33056", "jumptable_eu_8052B384"),
            ("@33057", "jumptable_eu_8052B35C"),
            ("@33058", "jumptable_eu_8052B334"),
            ("@33059", "jumptable_eu_8052B30C"),
            ("@33060", "jumptable_eu_8052B2E4"),
            ("@33061", "jumptable_eu_8052B1BC"),
            ("@33062", "jumptable_eu_8052B144"),
            ("@33063", "jumptable_eu_8052B194"),
            ("@33211", "jumptable_eu_8052B3D4"),
            ("@34366", "jumptable_eu_8052B504"),
            ("@34367", "jumptable_eu_8052B43C"),
            ("@34737", "jumptable_eu_8052B630"),
            ("@34738", "jumptable_eu_8052B540"),
            ("@34739", "jumptable_eu_8052B5E0"),
            # Ctor positional mapping (retail ctor 800D8FE8 loads BCE0,
            # BD68, BD50, BD44, BD2C in sub-object order):
            ("__vt__Q22cf14CBattleManager", "lbl_eu_8052BCE0"),
            ("__vt__36_reslist_base<PQ22cf13CfObjectActor>", "lbl_eu_8052BD68"),
            ("__vt__30reslist<PQ22cf13CfObjectActor>", "lbl_eu_8052BD50"),
            ("__vt__35_reslist_base<PQ22cf12IBattleEvent>", "lbl_eu_8052BD44"),
            ("__vt__29reslist<PQ22cf12IBattleEvent>", "lbl_eu_8052BD2C"),
            # rodata id/string pools (content + retail-ref verified).
            ("@stringBase0", "lbl_eu_804FCC78"),
            ("@17348", "lbl_eu_804FCAE4"),
            ("@17350", "lbl_eu_804FCAD8"),
        ),
        # sdata2 float cluster -> split1.s labels (content + unit-ref'd
        # sda21 sites). 0.0f -> DDC (DD0 is 0.6!). Two WIP constants have
        # no unit-ref'd counterpart and map to matching-value blob labels:
        # 0.001f -> 689F0, double 0.0 -> EE0.
        pool_patterns=(
            (struct.pack(">I", 0x00000000), "lbl_eu_80666DDC"),
            (struct.pack(">I", 0x40A00000), "lbl_eu_80666E6C"),
            (struct.pack(">I", 0x42960000), "lbl_eu_80666E7C"),
            (struct.pack(">I", 0x42480000), "lbl_eu_80666E18"),
            (struct.pack(">I", 0x41C80000), "lbl_eu_80666DF8"),
            (struct.pack(">I", 0x41700000), "lbl_eu_80666DF4"),
            (struct.pack(">I", 0x41200000), "lbl_eu_80666E34"),
            (struct.pack(">I", 0x40200000), "lbl_eu_80666E80"),
            (struct.pack(">I", 0x3F800000), "lbl_eu_80666DD4"),
            (struct.pack(">I", 0x42C80000), "lbl_eu_80666E00"),
            (struct.pack(">II", 0x3FE00000, 0x00000000), "lbl_eu_80666E58"),
            (struct.pack(">II", 0xBFE00000, 0x00000000), "lbl_eu_80666E60"),
            (struct.pack(">I", 0x3FA00000), "lbl_eu_80666E40"),
            (struct.pack(">I", 0x3E800000), "lbl_eu_80666E1C"),
            (struct.pack(">I", 0x3C23D70A), "lbl_eu_80666DD8"),
            (struct.pack(">I", 0x42C60000), "lbl_eu_80666E88"),
            (struct.pack(">I", 0x40000000), "lbl_eu_80666DFC"),
            (struct.pack(">I", 0x3F000000), "lbl_eu_80666DE8"),
            (struct.pack(">I", 0x3DCCCCCD), "lbl_eu_80666E84"),
            (struct.pack(">II", MAGIC_HI, MAGIC_LO), "lbl_eu_80666DE0"),
            (struct.pack(">I", 0x3F400000), "lbl_eu_80666E38"),
            (struct.pack(">I", 0x3FC00000), "lbl_eu_80666E3C"),
            (struct.pack(">I", 0xBF800000), "lbl_eu_80666E2C"),
            (struct.pack(">II", 0x3FF00000, 0x00000000), "lbl_eu_80666E48"),
            (struct.pack(">I", 0x43160000), "lbl_eu_80666E50"),
            (struct.pack(">I", 0x42000000), "lbl_eu_80666E54"),
            (struct.pack(">I", 0x3A83126F), "lbl_eu_806689F0"),
            (struct.pack(">d", 0.0), "lbl_eu_80666EE0"),
        ),
        extern_data_sections=(".data", ".rodata", ".sdata", ".sdata2", ".sbss"),
    ),
    "CfObjectImplWalker.o": UnitRules(
        # int->double magics: 2^52 -> lbl_eu_80666B98, unsigned variant ->
        # lbl_eu_80666BA8 (both unit-ref'd lfd sites). The func_800C551C
        # effA/B/C u16 statics are unreferenced orphans - stripped.
        pool_patterns=(
            (struct.pack(">II", MAGIC_HI, 0x00000000), "lbl_eu_80666B98"),
            (struct.pack(">II", MAGIC_HI, MAGIC_LO), "lbl_eu_80666BA8"),
        ),
        extern_data_sections=(".sdata2",),
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


def permute_sdata2_words(path: Path, order: tuple[int, ...]) -> bool:
    """Reorder .sdata2 4-byte words so position j holds the word currently at
    index order[j]. Symbols (and any in-section relocs) follow their values:
    a symbol whose st_value sat at word i moves to the new position of i.
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
    n_words = sec_size // 4
    if sorted(order) != list(range(n_words)):
        raise ValueError(f"permute_sdata2_words: bad permutation {order}")

    words_buf = [bytes(data[sec_off + i * 4: sec_off + i * 4 + 4]) for i in range(n_words)]
    new_words = [words_buf[order[j]] for j in range(n_words)]
    old_to_new = {order[j]: j for j in range(n_words)}
    for j in range(n_words):
        data[sec_off + j * 4: sec_off + j * 4 + 4] = new_words[j]

    # Symbols follow their values: st_value word index i -> old_to_new[i].
    _, sym_off, sym_size, _ = next(s for s in sections if s[0] == sym_idx)
    patched = False
    for so in range(0, sym_size, 16):
        if struct.unpack_from(">H", data, sym_off + so + 14)[0] != sdata2_idx:
            continue
        st_value = struct.unpack_from(">I", data, sym_off + so + 4)[0]
        if st_value % 4:
            continue
        wi = st_value // 4
        if wi in old_to_new and old_to_new[wi] != wi:
            struct.pack_into(">I", data, sym_off + so + 4, old_to_new[wi] * 4)
            patched = True
    path.write_bytes(data)
    return patched


def swap_sdata2_leading_f32_words(path: Path, words: tuple[int, int]) -> bool:
    """Swap .sdata2[0:4] with [4:8] when they equal `words` (decomp order).

    Same st_value swap as swap_sdata2_leading_f32_pair, but parameterized so
    any leading f32 pair can be re-ordered to retail (mtx: 0.0f,1.0f ->
    1.0f,0.0f).
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
    # Already retail order (swapped), or unexpected contents — no-op.
    if w0 == words[1] and w1 == words[0]:
        return False
    if w0 != words[0] or w1 != words[1]:
        return False

    struct.pack_into(">I", data, sec_off, w1)
    struct.pack_into(">I", data, sec_off + 4, w0)

    _, sym_off, sym_size, _ = next(s for s in sections if s[0] == sym_idx)
    for so in range(0, sym_size, 16):
        st_value = struct.unpack_from(">I", data, sym_off + so + 4)[0]
        if struct.unpack_from(">H", data, sym_off + so + 14)[0] != sdata2_idx:
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
    """Rename reloc-referenced .sdata2 @N pool symbols by content prefix.

    Candidates MUST be defined in .sdata2 (st_shndx check): nm-derived name
    lists carry no home-section info, and matching every @N symbol against
    bytes read at its st_value from .sdata2 silently stole val-0 symbols
    from other sections (.data jumptables reading the conversion double at
    offset 0) onto float labels. See docs/MWCC_PATTERNS.md.
    """
    if not patterns:
        return False
    data = path.read_bytes()
    sections, by_name = _read_elf_sections(data)
    sdata2_idx = by_name.get(".sdata2")
    if sdata2_idx is None:
        return False
    symtab_idx = by_name.get(".symtab")
    strtab_idx = by_name.get(".strtab")
    if symtab_idx is None or strtab_idx is None:
        return False

    _, sec_off, sec_size, _ = next(s for s in sections if s[0] == sdata2_idx)
    sym_hdr = next(s for s in sections if s[0] == symtab_idx)
    str_off = next(s for s in sections if s[0] == strtab_idx)[1]
    referenced = _reloc_at_pool_symbols(data, sections, by_name)

    renames: list[tuple[str, str]] = []
    used_targets: set[str] = set()
    for so in range(sym_hdr[1], sym_hdr[1] + sym_hdr[2], 16):
        st_name, st_value, _st_size, _info, _other, st_shndx = struct.unpack_from(
            ">IIIBBH", data, so
        )
        end = data.index(0, str_off + st_name)
        sym = data[str_off + st_name : end].decode("ascii", "replace")
        if not (
            sym.startswith("@")
            and not sym.startswith("@LOCAL@")
            and sym in referenced
            and st_shndx == sdata2_idx
        ):
            continue
        value = st_value
        if value >= sec_size:
            continue
        chunk = data[sec_off + value : sec_off + sec_size]
        for pattern, retail_name in patterns:
            if chunk.startswith(pattern) and sym != retail_name and retail_name not in used_targets:
                renames.append((sym, retail_name))
                used_targets.add(retail_name)
                break

    return _apply_renames(path, renames)

def rename_data_pool_symbols(
    path: Path, patterns: tuple[tuple[str, bytes, str], ...]
) -> bool:
    """Rename anonymous @ pool symbols by (section, content-prefix) match.

    Generalizes :func:`rename_pool_symbols` (.sdata2-only) to any file-backed
    data section. MWCC emits RTTI name strings / typeinfo structs for
    template instantiations under unspellable @N names whose numbering
    drifts with every source change; content matching keeps unit rules
    stable. Each pattern must match exactly one reloc-referenced @ symbol
    in its section, otherwise nothing is renamed (loud gate failure beats a
    silent wrong rename).
    """
    if not patterns:
        return False
    data = path.read_bytes()
    sections, by_name = _read_elf_sections(data)
    sec_idx = {sec: idx for sec, idx in ((n, i) for n, i in by_name.items())}

    # Collect referenced-@ names from every .rela.<data section> (the
    # .rela.text-only helper misses .sdata/.data references).
    symtab_idx = by_name.get(".symtab")
    strtab_idx = by_name.get(".strtab")
    if symtab_idx is None or strtab_idx is None:
        return False
    sym_off = next(s for s in sections if s[0] == symtab_idx)[1]
    str_off = next(s for s in sections if s[0] == strtab_idx)[1]
    referenced: set[str] = set()
    rel_idxs = [i for n, i in by_name.items() if n.startswith(".rela.")]
    for ri in rel_idxs:
        _, r_off, r_size, _ = next(s for s in sections if s[0] == ri)
        for ro in range(0, r_size, 12):
            _r_offset, r_info, _r_addend = struct.unpack_from(">III", data, r_off + ro)
            sym_index = r_info >> 8
            st_name = struct.unpack_from(">I", data, sym_off + sym_index * 16)[0]
            end = data.index(0, str_off + st_name)
            sname = data[str_off + st_name : end].decode("ascii", "replace")
            if sname.startswith("@") and not sname.startswith("@LOCAL@"):
                referenced.add(sname)

    idx_to_name = {}
    for nm_, idx in by_name.items():
        idx_to_name[idx] = nm_
    pool_by_sec: dict[str, list[tuple[str, int, int]]] = {}
    shnum = struct.unpack_from(">H", data, 48)[0]
    e_shoff = struct.unpack_from(">I", data, 32)[0]
    e_shentsize = struct.unpack_from(">H", data, 46)[0]
    sym_size = next(s for s in sections if s[0] == symtab_idx)[2]
    for so in range(sym_off, sym_off + sym_size, 16):
        st_name, st_value, st_size, _info, _other, st_shndx = struct.unpack_from(
            ">IIIBBH", data, so
        )
        end = data.index(0, str_off + st_name)
        sname = data[str_off + st_name : end].decode("ascii", "replace")
        if not (
            sname.startswith("@")
            and not sname.startswith("@LOCAL@")
            and sname in referenced
        ):
            continue
        sec_name = idx_to_name.get(st_shndx)
        if sec_name is not None:
            pool_by_sec.setdefault(sec_name, []).append((sname, st_value, st_size))

    renames: list[tuple[str, str]] = []
    used_targets: set[str] = set()
    for sec_name, pattern, retail_name in patterns:
        idx = sec_idx.get(sec_name)
        if idx is None:
            continue
        _, sec_off, sec_size, _ = next(s for s in sections if s[0] == idx)
        candidates = []
        for sym, value, symsz in pool_by_sec.get(sec_name, []):
            if value >= sec_size:
                continue
            # A symbol qualifies only when its declared size equals the
            # pattern length: all-reloc zero words (e.g. a jumptable) can
            # prefix-match a small all-zero typeinfo pattern otherwise.
            if len(pattern) > 0 and symsz not in (0, len(pattern)):
                continue
            chunk = data[sec_off + value : sec_off + sec_size]
            if chunk.startswith(pattern):
                candidates.append(sym)
        if len(candidates) == 1 and candidates[0] != retail_name:
            renames.append((candidates[0], retail_name))
            used_targets.add(retail_name)
    return _apply_renames(path, renames)


def rename_exact(path: Path, exact: tuple[tuple[str, str], ...]) -> bool:
    if not exact:
        return False
    present = _all_symbols(path)
    renames = [(old, new) for old, new in exact if old in present and old != new]
    return _apply_renames(path, renames)


def globalize_symbols(path: Path, symbols: tuple[str, ...]) -> bool:
    """Promote every symtab entry named in *symbols* to GLOBAL binding.

    objcopy --globalize-symbol per name (matches every entry carrying the
    name; stripped rename targets are UNDEF so there is no duplicate-def
    risk). See UnitRules.globalize_symbols for why this is required before
    the final mwldeppc link.
    """
    if not symbols:
        return False
    if not OBJCOPY.is_file():
        raise FileNotFoundError(f"missing objcopy: {OBJCOPY}")
    tmp = path.with_suffix(".globalized.o")
    cmd = [str(OBJCOPY), str(path)]
    cmd += [f"--globalize-symbol={sym}" for sym in symbols]
    cmd.append(str(tmp))
    subprocess.run(cmd, check=True)
    tmp.replace(path)
    return True


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
    # A name ending in '*' matches by PREFIX -- use it for DECOMP_FORCEACTIVE
    # anchor stubs whose symbol embeds a __LINE__ that drifts with every edit
    # above the macro (exact-match silently disarms the drop; MWCC_CASES
    # "reslist::size() declaration order" follow-up).
    drops: list[tuple[int, int, int]] = []  # (sym_entry_off, start, end)
    exact = {n for n in names if not n.endswith("*")}
    prefixes = tuple(n[:-1] for n in names if n.endswith("*"))
    for so in range(0, sym_size, 16):
        st_name = struct.unpack_from(">I", data, sym_off + so)[0]
        end = data.index(0, str_off + st_name)
        sname = data[str_off + st_name : end].decode("ascii")
        if sname not in exact and not sname.startswith(prefixes):
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


def move_data_rela_offsets(path: Path, moves: tuple[tuple[str, int, int], ...]) -> bool:
    """Rewrite .rela.<section> r_offset values for data sections.

    moves: ((section, old_off, new_off), ...). Used when MWCC emits data
    blocks in a different order than retail while every content word is a
    link-time reloc or zero -- the section bytes already compare equal and
    only the reloc positions need remapping onto the retail layout.
    """
    if not moves:
        return False

    data = bytearray(path.read_bytes())
    if data[:4] != b"\x7fELF" or data[5] != 2:
        raise ValueError(f"expected big-endian ELF32: {path}")

    e_shoff = struct.unpack_from(">I", data, 32)[0]
    e_shentsize = struct.unpack_from(">H", data, 46)[0]
    e_shnum = struct.unpack_from(">H", data, 48)[0]
    e_shstrndx = struct.unpack_from(">H", data, 50)[0]
    shstr_off = struct.unpack_from(">I", data, e_shoff + e_shstrndx * e_shentsize + 16)[0]

    by_section: dict[str, tuple[int, int]] = {}
    for i in range(e_shnum):
        hoff = e_shoff + i * e_shentsize
        sh_name = struct.unpack_from(">I", data, hoff)[0]
        end = data.index(0, shstr_off + sh_name)
        name = data[shstr_off + sh_name : end].decode("ascii")
        if name.startswith(".rela"):
            body = ".rela" + name[4:] if not name.startswith(".rela.") else name[5:]
            rel_off = struct.unpack_from(">I", data, hoff + 16)[0]
            rel_size = struct.unpack_from(">I", data, hoff + 20)[0]
            by_section[body] = (rel_off, rel_size)

    per_sec: dict[str, dict[int, int]] = {}
    for section, old, new in moves:
        per_sec.setdefault(section, {})[old] = new

    changed = False
    for section, move_map in per_sec.items():
        rela = by_section.get(section)
        if rela is None:
            continue
        rel_off, rel_size = rela
        for ro in range(0, rel_size, 12):
            r_offset = struct.unpack_from(">I", data, rel_off + ro)[0]
            new_off = move_map.get(r_offset)
            if new_off is None or new_off == r_offset:
                continue
            struct.pack_into(">I", data, rel_off + ro, new_off)
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
    # Insert the pad, then re-emit every later section at an offset that
    # keeps its sh_addralign congruence (plain shifting can leave e.g. an
    # 8-aligned .data at a misaligned file offset, which strict ELF
    # consumers like objdiff-cli reject). Extra inter-section gap bytes are
    # legal ELF; only monotonic non-overlap matters.
    body = bytearray(data[:sec_end]) + (b"\0" * pad)
    order = []
    for i in range(e_shnum):
        hoff_i = e_shoff + i * e_shentsize
        o = struct.unpack_from(">I", data, hoff_i + 16)[0]
        if o >= sec_end:
            order.append((o, i))
    # Rebuild the tail with an explicit cursor: each later section keeps its
    # bytes and its sh_addralign residue (strict ELF consumers like
    # objdiff-cli check offset congruence). Positions must come from the
    # actual write cursor, never arithmetic on old offsets -- pre-existing
    # inter-section slack would desync the recorded vs actual layout.
    new_offsets = {}
    for _old_off, i in sorted(order):
        hoff_i = e_shoff + i * e_shentsize
        old_off = struct.unpack_from(">I", data, hoff_i + 16)[0]
        typ = struct.unpack_from(">I", data, hoff_i + 4)[0]
        addralign = struct.unpack_from(">I", data, hoff_i + 32)[0] or 1
        rem = old_off % addralign
        new_off = len(body) + ((rem - len(body)) % addralign)
        gap = new_off - len(body)
        if gap:
            body += b"\0" * gap
        if typ != 8:  # NOBITS bodies have no file bytes to copy
            old_size = struct.unpack_from(">I", data, hoff_i + 20)[0]
            body += data[old_off : old_off + old_size]
        new_offsets[i] = new_off
    # Carry the section header table over verbatim: it sits after every
    # section body, so the per-section reconstruction above never copies it,
    # and the old e_shoffset now points past the rebuilt body.
    new_e_shoff = (len(body) + 3) & ~3
    body += b"\0" * (new_e_shoff - len(body))
    body += data[e_shoff:]
    data = body
    struct.pack_into(">I", data, 32, new_e_shoff)
    e_shoff = new_e_shoff
    for i in range(e_shnum):
        hoff = e_shoff + i * e_shentsize
        sh_offset = struct.unpack_from(">I", data, hoff + 16)[0]
        if i == sec_idx:
            struct.pack_into(">I", data, hoff + 20, new_size)
            continue
        if sh_offset >= sec_end and i in new_offsets:
            struct.pack_into(">I", data, hoff + 16, new_offsets[i])
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

    # Idempotence guard: the cycle pipeline runs post-processing twice; a
    # second pass must not append a duplicate .strtab entry (each append
    # shifts later sections and can leave .symtab misaligned).
    sym_hoff0 = e_shoff + sym_idx * e_shentsize
    sym_off0 = struct.unpack_from(">I", data, sym_hoff0 + 16)[0]
    str_hoff0 = e_shoff + str_idx * e_shentsize
    str_off0 = struct.unpack_from(">I", data, str_hoff0 + 16)[0]
    cur_name_off = struct.unpack_from(">I", data, sym_off0 + target_sym * 16)[0]
    cur_end = data.index(0, str_off0 + cur_name_off)
    if data[str_off0 + cur_name_off : cur_end].decode("ascii", "replace") == new_name:
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
    # Pad the append to a 4-byte multiple: later sections (.rela* etc.) are
    # shifted by the raw append length, and an odd-length name would leave
    # them misaligned (objdiff/the object crate rejects unaligned .rela).
    # Strtab consumers only need NUL-terminated names; trailing NULs are
    # harmless padding.
    name_bytes = new_name.encode("utf-8") + b"\0"
    pad = (-len(name_bytes)) % 4
    name_bytes += b"\0" * pad
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


def set_reloc_addend(path: Path, section: str, offset: int, value: int) -> bool:
    """Set the r_addend of the SHT_RELA entry at (section, offset) to *value*
    absolutely (variant-robust sibling of patch_reloc_addend)."""
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
            struct.pack_into(">i", data, rela_off + ro + 8, value)
            path.write_bytes(data)
            return True
    return False


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

    sec_idx = rela_idx = sym_idx = None
    for i in range(e_shnum):
        hoff = e_shoff + i * e_shentsize
        sh_name, = struct.unpack_from(">I", data, hoff)
        end = data.index(0, shstr_off + sh_name)
        name = data[shstr_off + sh_name : end].decode("ascii")
        if name == section:
            sec_idx = i
        elif name == ".rela" + section:
            rela_idx = i
        elif name == ".symtab":
            sym_idx = i
    # A missing .rela.<section> is fine when the section is referenced
    # cross-section (e.g. .rodata pools loaded via .rela.text): there are no
    # in-section relocs to move, but the bytes still need permuting.
    if sec_idx is None or off1 == off2 or size <= 0:
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

    delta = off2 - off1

    if rela_idx is not None:
        rela_hoff = e_shoff + rela_idx * e_shentsize
        rela_off = struct.unpack_from(">I", data, rela_hoff + 16)[0]
        rela_size = struct.unpack_from(">I", data, rela_hoff + 20)[0]
        for ro in range(0, rela_size, 12):
            r_offset, = struct.unpack_from(">I", data, rela_off + ro)
            if off1 <= r_offset < off1 + size:
                struct.pack_into(">I", data, rela_off + ro, r_offset + delta)
            elif off2 <= r_offset < off2 + size:
                struct.pack_into(">I", data, rela_off + ro, r_offset - delta)

    # Follow-the-data: symbols defined inside either swapped range must move
    # with their bytes, otherwise cross-section loads (via .rela.text against
    # pool symbols) silently read swapped values.
    if sym_idx is not None:
        sym_hoff = e_shoff + sym_idx * e_shentsize
        sym_off = struct.unpack_from(">I", data, sym_hoff + 16)[0]
        sym_size = struct.unpack_from(">I", data, sym_hoff + 20)[0]
        for so in range(0, sym_size, 16):
            st_shndx = struct.unpack_from(">H", data, sym_off + so + 14)[0]
            if st_shndx != sec_idx:
                continue
            st_value = struct.unpack_from(">I", data, sym_off + so + 4)[0]
            if off1 <= st_value < off1 + size:
                struct.pack_into(">I", data, sym_off + so + 4, st_value + delta)
            elif off2 <= st_value < off2 + size:
                struct.pack_into(">I", data, sym_off + so + 4, st_value - delta)

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

    # No UNDEF slot exists (the source didn't declare/reference this extern).
    # The append fallback is deliberately DISABLED: inserting a name into
    # .strtab and a LOCAL entry into .symtab shifts the .strtab content on
    # subsequent appends, corrupting previously-appended st_name offsets, and
    # mwldeppc also misreads appended LOCals past sh_info (ELF_linker.c:11164).
    # The rule is only satisfied once the TU source provides the UNDEF slot
    # (extern + reference); until then, skip cleanly so the data gate can
    # still run (mid-progress CGXCache pool work: 7/972 slots live).
    return False


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
    # "symbol+addend" spelling sets the injected r_addend directly (the
    # addend_sets pass runs before this one, so it cannot patch these).
    addend = 0
    sym_base = sym_name
    if "+" in sym_name:
        sym_base, addend_str = sym_name.rsplit("+", 1)
        addend = int(addend_str, 0)
        sym_name = sym_base
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

    entry = struct.pack(">IIi", offset, (target_sym_idx << 8) | 1, addend)  # R_PPC_ADDR32
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


def _basename_for_rules(path: Path) -> str:
    name = path.name
    # Link-time copies are named *.reloc.o (tools/project.py
    # link_reloc_postprocess rule); match by the original basename.
    if name.endswith(".reloc.o"):
        name = name[:-8] + ".o"
    return name


def select_unit_rules(path: Path) -> UnitRules | None:
    """Resolve the UnitRules for an object.

    Keys are usually plain basenames ("MTRand.o"). A key may carry a
    "#<symbol-substring>" scope ("lyt_pane.o#Q34nw4r3lyt"): it applies only
    to objects whose symbol table contains a name with that substring.
    Same-basename twins across libs (nw4r vs RVL_SDK nw4hbm lyt) need
    different §17.6 reshapes because their retail splits keep different data
    locals — the hbm twins' vtables stay TU-local while the nw4r twins ship
    everything into nw4r_data.s. A scoped entry wins when its substring
    matches; the plain basename entry stays the fallback for every other lib.
    """
    name = _basename_for_rules(path)
    plain: list[UnitRules] = []
    scoped: list[tuple[str, UnitRules]] = []
    for key, rules in UNIT_RULES.items():
        base, _, cond = key.partition("#")
        if base != name:
            continue
        if cond:
            scoped.append((cond, rules))
        else:
            plain.append(rules)
    if not scoped:
        return plain[0] if plain else None
    symbols = _all_symbols(path)
    for cond, rules in scoped:
        if any(cond in sym for sym in symbols):
            return rules
    return plain[0] if plain else None


def postprocess_object(path: Path, rules: UnitRules | None = None) -> bool:
    if rules is None:
        rules = select_unit_rules(path)
    if rules is None:
        return False

    changed = False
    # Rename pool symbols BEFORE trimming/padding .sdata2: the trim drops the
    # pool section (and ABS's its symbols), which would leave nothing for
    # rename_pool_symbols to match (snd_EnvGenerator/CMCEffStart constants).
    changed = rename_pool_symbols(path, rules.pool_patterns) or changed
    changed = rename_data_pool_symbols(path, rules.data_pool_patterns) or changed
    if rules.patch_unsigned_magic:
        changed = patch_sdata2_magic(path) or changed
    if rules.swap_sdata2_leading_f32_pair:
        changed = swap_sdata2_leading_f32_pair(path) or changed
    if rules.swap_sdata2_leading_f32_words is not None:
        changed = swap_sdata2_leading_f32_words(path, rules.swap_sdata2_leading_f32_words) or changed
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
    changed = rename_data_pool_symbols(path, rules.data_pool_patterns) or changed
    if rules.data_reloc_offset_moves:
        changed = move_data_rela_offsets(path, rules.data_reloc_offset_moves) or changed
    for sec, off, sym_name in rules.retarget_relocs:
        changed = retarget_reloc_to_symbol(path, sec, off, sym_name) or changed
    for sec, o1, o2, size in rules.swap_data_blocks:
        changed = swap_data_blocks_func(path, sec, o1, o2, size) or changed
    for sec, off, delta in rules.addend_patches:
        changed = patch_reloc_addend(path, sec, off, delta) or changed
    for sec, off, value in rules.addend_sets:
        changed = set_reloc_addend(path, sec, off, value) or changed
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
    if rules.permute_sdata2_words:
        changed = permute_sdata2_words(path, rules.permute_sdata2_words) or changed
    if rules.globalize_symbols:
        # After all renames/retargets (names must exist) and before the strip
        # (binding is independent of section storage).
        changed = globalize_symbols(path, rules.globalize_symbols) or changed
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


def _unit_has_rules(name: str) -> bool:
    return any(key.partition("#")[0] == name for key in UNIT_RULES)


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
        if not _unit_has_rules(_basename_for_rules(path)):
            print(f"no reloc postprocess rules for {path.name}")
            continue
        if postprocess_object(path):
            print(f"post-processed {path}")
        else:
            print(f"no post-process changes for {path}")
    return rc


if __name__ == "__main__":
    raise SystemExit(main(sys.argv))
