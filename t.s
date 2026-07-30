Analyzing: Z:\var\folders\4q\rzjqpyrj41g23zz2f9jkgqfc0000gn\T\t.o
Header idenfication: 0x7f, ELF, 32 bit MSB v1
    type:           1, machine:         20
    version: 00000001, entry:     00000000, phoff:  00000000
    shoff:   00000258, flags:     80000000, ehsize:       52, phentsize:  0
    phnum:          0, shentsize:       40, shnum:         8, shstrndx:  6


==> .shstrtab
    [  1]	.text
    [  7]	.mwcats.text
    [ 20]	.rela.mwcats.text
    [ 38]	.symtab
    [ 46]	.strtab
    [ 54]	.shstrtab
    [ 64]	.comment

==> .text [1]: Section Header
    type:    00000001, flags:  00000006
    address: 00000000, offset: 00000040, size: 00000050
    link:    00000000, info:   00000000, alignment: 16, entry size: 0


==> .mwcats.text [2]: Section Header
    type:    ca2a82c2, flags:  00000000
    address: 00000000, offset: 00000090, size: 00000028
    link:    00000001, info:   00000000, alignment: 4, entry size: 1


==> .rela.mwcats.text [3]: Section Header
    type:    00000004, flags:  00000000
    address: 00000000, offset: 000000b8, size: 0000003c
    link:    00000004, info:   00000002, alignment: 4, entry size: 12


==> .symtab [4]: Section Header
    type:    00000002, flags:  00000000
    address: 00000000, offset: 000000f4, size: 00000090
    link:    00000005, info:   00000004, alignment: 4, entry size: 16


==> .strtab [5]: Section Header
    type:    00000003, flags:  00000000
    address: 00000000, offset: 00000184, size: 00000011
    link:    00000000, info:   00000000, alignment: 1, entry size: 1


==> .shstrtab [6]: Section Header
    type:    00000003, flags:  00000000
    address: 00000000, offset: 00000195, size: 00000049
    link:    00000000, info:   00000000, alignment: 1, entry size: 1


==> .comment [7]: Section Header
    type:    00000001, flags:  00000000
    address: 00000000, offset: 000001de, size: 00000074
    link:    00000000, info:   00000000, alignment: 1, entry size: 1


==> .strtab
    [  1]	t.cpp
    [  7]	a
    [  9]	b
    [ 11]	c
    [ 13]	d
    [ 15]	e

==> .symtab
    entry  spndx   value      size   bind   type  other    shndx         name
    [   0]     0   00000000      0   LOC    NULL      0    UND           
    [   1]     1   00000000      0   LOC    FILE      0    ABS           t.cpp
    [   2]     0   00000000      0   LOC    SECT      0    .text         .text
    [   3]     0   00000000      0   LOC    SECT      0    .mwcats.text  .mwcats.text
    [   4]     7   00000000     16   GLOB   FUNC      0    .text         a
    [   5]     9   00000010     16   GLOB   FUNC      0    .text         b
    [   6]    11   00000020     16   GLOB   FUNC      0    .text         c
    [   7]    13   00000030     16   GLOB   FUNC      0    .text         d
    [   8]    15   00000040     16   GLOB   FUNC      0    .text         e

==> .text

Hunk:	Kind=HUNK_GLOBAL_CODE     Name="a"  Size=16
00000000: A0030000  lhz      r0,0(r3)
00000004: 5400047E  clrlwi   r0,r0,17                ; rlwinm     r0,r0,0,17,31
00000008: B0030000  sth      r0,0(r3)
0000000C: 4E800020  blr

Hunk:	Kind=HUNK_GLOBAL_CODE     Name="b"  Size=16
00000010: A0030000  lhz      r0,0(r3)
00000014: 5400047E  clrlwi   r0,r0,17                ; rlwinm     r0,r0,0,17,31
00000018: B0030000  sth      r0,0(r3)
0000001C: 4E800020  blr

Hunk:	Kind=HUNK_GLOBAL_CODE     Name="c"  Size=16
00000020: A0030000  lhz      r0,0(r3)
00000024: 5400047E  clrlwi   r0,r0,17                ; rlwinm     r0,r0,0,17,31
00000028: B0030000  sth      r0,0(r3)
0000002C: 4E800020  blr

Hunk:	Kind=HUNK_GLOBAL_CODE     Name="d"  Size=16
00000030: A0030000  lhz      r0,0(r3)
00000034: 5400047E  clrlwi   r0,r0,17                ; rlwinm     r0,r0,0,17,31
00000038: B0030000  sth      r0,0(r3)
0000003C: 4E800020  blr

Hunk:	Kind=HUNK_GLOBAL_CODE     Name="e"  Size=16
00000040: A0030000  lhz      r0,0(r3)
00000044: 5400047E  clrlwi   r0,r0,17                ; rlwinm     r0,r0,0,17,31
00000048: B0030000  sth      r0,0(r3)
0000004C: 4E800020  blr

==> .rela.mwcats.text
    entry  offset   addend   type name(symbol id)
    [   0] 00000004 00000000    1 a(4)
    [   1] 0000000c 00000000    1 b(5)
    [   2] 00000014 00000000    1 c(6)
    [   3] 0000001c 00000000    1 d(7)
    [   4] 00000024 00000000    1 e(8)

==> .mwcats.text
    entry   offset    type size     addr     procexits
    [   0]  00000000     2 0010     00000000         0
    [   1]  00000008     2 0010     00000000         0
    [   2]  00000010     2 0010     00000000         0
    [   3]  00000018     2 0010     00000000         0
    [   4]  00000020     2 0010     00000000         0

==> .comment
MagicWord: CodeWarrior
Comment section version: 15
Compiler version: 4.0.0
CW Compiler generated
Pooled data: true
PPC EABI compliant
FP Support: Hardware
Processor: Gecko
Char Signedness: signed
