Analyzing: Z:\var\folders\4q\rzjqpyrj41g23zz2f9jkgqfc0000gn\T\tco_test.o
Header idenfication: 0x7f, ELF, 32 bit MSB v1
    type:           1, machine:         20
    version: 00000001, entry:     00000000, phoff:  00000000
    shoff:   00000288, flags:     80000000, ehsize:       52, phentsize:  0
    phnum:          0, shentsize:       40, shnum:        12, shstrndx:  10


==> .shstrtab
    [  1]	.text
    [  7]	extab
    [ 13]	extabindex
    [ 24]	.mwcats.text
    [ 37]	.rela.text
    [ 48]	.relaextabindex
    [ 64]	.rela.mwcats.text
    [ 82]	.symtab
    [ 90]	.strtab
    [ 98]	.shstrtab
    [ 108]	.comment

==> .text [1]: Section Header
    type:    00000001, flags:  00000006
    address: 00000000, offset: 00000040, size: 00000040
    link:    00000000, info:   00000000, alignment: 16, entry size: 0


==> extab [2]: Section Header
    type:    00000001, flags:  00000002
    address: 00000000, offset: 00000080, size: 00000008
    link:    00000000, info:   00000000, alignment: 4, entry size: 0


==> extabindex [3]: Section Header
    type:    00000001, flags:  00000002
    address: 00000000, offset: 00000088, size: 0000000c
    link:    00000000, info:   00000000, alignment: 4, entry size: 0


==> .mwcats.text [4]: Section Header
    type:    ca2a82c2, flags:  00000000
    address: 00000000, offset: 00000094, size: 00000008
    link:    00000001, info:   00000000, alignment: 4, entry size: 1


==> .rela.text [5]: Section Header
    type:    00000004, flags:  00000000
    address: 00000000, offset: 0000009c, size: 0000000c
    link:    00000008, info:   00000001, alignment: 4, entry size: 12


==> .relaextabindex [6]: Section Header
    type:    00000004, flags:  00000000
    address: 00000000, offset: 000000a8, size: 00000018
    link:    00000008, info:   00000003, alignment: 4, entry size: 12


==> .rela.mwcats.text [7]: Section Header
    type:    00000004, flags:  00000000
    address: 00000000, offset: 000000c0, size: 0000000c
    link:    00000008, info:   00000004, alignment: 4, entry size: 12


==> .symtab [8]: Section Header
    type:    00000002, flags:  00000000
    address: 00000000, offset: 000000cc, size: 000000a0
    link:    00000009, info:   00000009, alignment: 4, entry size: 16


==> .strtab [9]: Section Header
    type:    00000003, flags:  00000000
    address: 00000000, offset: 0000016c, size: 00000028
    link:    00000000, info:   00000000, alignment: 1, entry size: 1


==> .shstrtab [10]: Section Header
    type:    00000003, flags:  00000000
    address: 00000000, offset: 00000194, size: 00000075
    link:    00000000, info:   00000000, alignment: 1, entry size: 1


==> .comment [11]: Section Header
    type:    00000001, flags:  00000000
    address: 00000000, offset: 00000209, size: 0000007c
    link:    00000000, info:   00000000, alignment: 1, entry size: 1


==> .strtab
    [  1]	tco_test.c
    [ 12]	Run
    [ 16]	@7
    [ 19]	@8
    [ 22]	ICFlashInvalidate

==> .symtab
    entry  spndx   value      size   bind   type  other    shndx         name
    [   0]     0   00000000      0   LOC    NULL      0    UND           
    [   1]     1   00000000      0   LOC    FILE      0    ABS           tco_test.c
    [   2]     0   00000000      0   LOC    SECT      0    .text         .text
    [   3]     0   00000000      0   LOC    SECT      0    extab         extab
    [   4]     0   00000000      0   LOC    SECT      0    extabindex    extabindex
    [   5]     0   00000000      0   LOC    SECT      0    .mwcats.text  .mwcats.text
    [   6]    12   00000000     64   LOC    FUNC      0    .text         Run
    [   7]    16   00000000      8   LOC    OBJ       2    extab         @7
    [   8]    19   00000000     12   LOC    OBJ       2    extabindex    @8
    [   9]    22   00000000      0   GLOB   NULL      0    UND           ICFlashInvalidate

==> .rela.text
    entry  offset   addend   type name(symbol id)
    [   0] 00000014 00000000   10 ICFlashInvalidate(9)

==> .text

Hunk:	Kind=HUNK_LOCAL_CODE     Name="Run"  Size=64
00000000: 9421FFF0  stwu     rsp,-16(rsp)
00000004: 7C0802A6  mflr     r0
00000008: 90010014  stw      r0,20(rsp)
0000000C: 93E1000C  stw      r31,12(rsp)
00000010: 7C7F1B78  mr       r31,r3
00000014: 48000001  bl       ICFlashInvalidate
00000018: 7C0004AC  sync
0000001C: 4C00012C  isync
00000020: 7FECFB78  mr       r12,r31
00000024: 7D8903A6  mtctr    r12
00000028: 4E800421  bctrl
0000002C: 80010014  lwz      r0,20(rsp)
00000030: 83E1000C  lwz      r31,12(rsp)
00000034: 7C0803A6  mtlr     r0
00000038: 38210010  addi     rsp,rsp,16
0000003C: 4E800020  blr

==> .rela.mwcats.text
    entry  offset   addend   type name(symbol id)
    [   0] 00000004 00000000    1 Run(6)

==> .mwcats.text
    entry   offset    type size     addr     procexits
    [   0]  00000000     2 0040     00000000         0

==> .comment
MagicWord: CodeWarrior
Comment section version: 15
Compiler version: 4.0.0
CW Compiler generated
Pooled data: true
PPC EABI compliant
FP Support: Software
Processor: Gecko
Char Signedness: signed
