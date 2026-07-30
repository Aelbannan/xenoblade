Analyzing: Z:\var\folders\4q\rzjqpyrj41g23zz2f9jkgqfc0000gn\T\t2.o
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
    address: 00000000, offset: 00000184, size: 00000017
    link:    00000000, info:   00000000, alignment: 1, entry size: 1


==> .shstrtab [6]: Section Header
    type:    00000003, flags:  00000000
    address: 00000000, offset: 0000019b, size: 00000049
    link:    00000000, info:   00000000, alignment: 1, entry size: 1


==> .comment [7]: Section Header
    type:    00000001, flags:  00000000
    address: 00000000, offset: 000001e4, size: 00000074
    link:    00000000, info:   00000000, alignment: 1, entry size: 1


==> .strtab
    [  1]	t2.cpp
    [  8]	sa
    [ 11]	sc
    [ 14]	se
    [ 17]	sf
    [ 20]	sg

==> .symtab
    entry  spndx   value      size   bind   type  other    shndx         name
    [   0]     0   00000000      0   LOC    NULL      0    UND           
    [   1]     1   00000000      0   LOC    FILE      0    ABS           t2.cpp
    [   2]     0   00000000      0   LOC    SECT      0    .text         .text
    [   3]     0   00000000      0   LOC    SECT      0    .mwcats.text  .mwcats.text
    [   4]     8   00000000     16   GLOB   FUNC      0    .text         sa
    [   5]    11   00000010     12   GLOB   FUNC      0    .text         sc
    [   6]    14   00000020     16   GLOB   FUNC      0    .text         se
    [   7]    17   00000030     16   GLOB   FUNC      0    .text         sf
    [   8]    20   00000040     16   GLOB   FUNC      0    .text         sg

==> .text

Hunk:	Kind=HUNK_GLOBAL_CODE     Name="sa"  Size=16
00000000: A8030000  lha      r0,0(r3)
00000004: 5400047E  clrlwi   r0,r0,17                ; rlwinm     r0,r0,0,17,31
00000008: B0030000  sth      r0,0(r3)
0000000C: 4E800020  blr

Hunk:	Kind=HUNK_GLOBAL_CODE     Name="sc"  Size=12
00000010: A0030000  lhz      r0,0(r3)
00000014: 5403047E  clrlwi   r3,r0,17                ; rlwinm     r3,r0,0,17,31
00000018: 4E800020  blr

Hunk:	Kind=HUNK_GLOBAL_CODE     Name="se"  Size=16
00000020: A0030000  lhz      r0,0(r3)
00000024: 5400047E  clrlwi   r0,r0,17                ; rlwinm     r0,r0,0,17,31
00000028: B0030000  sth      r0,0(r3)
0000002C: 4E800020  blr

Hunk:	Kind=HUNK_GLOBAL_CODE     Name="sf"  Size=16
00000030: A0030000  lhz      r0,0(r3)
00000034: 5400047E  clrlwi   r0,r0,17                ; rlwinm     r0,r0,0,17,31
00000038: B0030000  sth      r0,0(r3)
0000003C: 4E800020  blr

Hunk:	Kind=HUNK_GLOBAL_CODE     Name="sg"  Size=16
00000040: A0030000  lhz      r0,0(r3)
00000044: 5400047E  clrlwi   r0,r0,17                ; rlwinm     r0,r0,0,17,31
00000048: B0030000  sth      r0,0(r3)
0000004C: 4E800020  blr

==> .rela.mwcats.text
    entry  offset   addend   type name(symbol id)
    [   0] 00000004 00000000    1 sa(4)
    [   1] 0000000c 00000000    1 sc(5)
    [   2] 00000014 00000000    1 se(6)
    [   3] 0000001c 00000000    1 sf(7)
    [   4] 00000024 00000000    1 sg(8)

==> .mwcats.text
    entry   offset    type size     addr     procexits
    [   0]  00000000     2 0010     00000000         0
    [   1]  00000008     2 000c     00000000         0
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
