Analyzing: Z:\Users\ahmedelbannan\Ahmed\xenoblade\build\us\src\monolib\src\scn\code_804BF59C.o
Header idenfication: 0x7f, ELF, 32 bit MSB v1
    type:           1, machine:         20
    version: 00000001, entry:     00000000, phoff:  00000000
    shoff:   000008c0, flags:     80000000, ehsize:       52, phentsize:  0
    phnum:          0, shentsize:       40, shnum:        10, shstrndx:  8


==> .shstrtab
    [  1]	.text
    [  7]	extab
    [ 13]	extabindex
    [ 24]	.rela.text
    [ 35]	.relaextabindex
    [ 51]	.symtab
    [ 59]	.strtab
    [ 67]	.shstrtab
    [ 77]	.comment

==> .text [1]: Section Header
    type:    00000001, flags:  00000006
    address: 00000000, offset: 00000034, size: 00000290
    link:    00000000, info:   00000000, alignment: 4, entry size: 0


==> extab [2]: Section Header
    type:    00000001, flags:  00000002
    address: 00000000, offset: 000002c4, size: 00000010
    link:    00000000, info:   00000000, alignment: 4, entry size: 0


==> extabindex [3]: Section Header
    type:    00000001, flags:  00000002
    address: 00000000, offset: 000002d4, size: 00000018
    link:    00000000, info:   00000000, alignment: 4, entry size: 0


==> .rela.text [4]: Section Header
    type:    00000004, flags:  00000000
    address: 00000000, offset: 000002ec, size: 000000c0
    link:    00000006, info:   00000001, alignment: 4, entry size: 12


==> .relaextabindex [5]: Section Header
    type:    00000004, flags:  00000000
    address: 00000000, offset: 000003ac, size: 00000030
    link:    00000006, info:   00000003, alignment: 4, entry size: 12


==> .symtab [6]: Section Header
    type:    00000002, flags:  00000000
    address: 00000000, offset: 000003dc, size: 000001e0
    link:    00000007, info:   00000009, alignment: 4, entry size: 16


==> .strtab [7]: Section Header
    type:    00000003, flags:  00000000
    address: 00000000, offset: 000005bc, size: 0000018c
    link:    00000000, info:   00000000, alignment: 1, entry size: 1


==> .shstrtab [8]: Section Header
    type:    00000003, flags:  00000000
    address: 00000000, offset: 00000748, size: 00000056
    link:    00000000, info:   00000000, alignment: 1, entry size: 1


==> .comment [9]: Section Header
    type:    00000001, flags:  00000000
    address: 00000000, offset: 0000079e, size: 0000011c
    link:    00000000, info:   00000000, alignment: 1, entry size: 1


==> .strtab
    [  1]	code_804BF59C.cpp
    [ 19]	@6545
    [ 25]	@6546
    [ 31]	@6552
    [ 37]	@6553
    [ 43]	func_804BF59C
    [ 57]	__ct__804BF5A0__Fv
    [ 76]	__dt__804BF6F4
    [ 91]	__dt__Q34nw4r3g3d8LightObjFv
    [ 120]	__destroy_arr
    [ 134]	__dt__6CLightFv
    [ 150]	__dl__FPv
    [ 160]	func_804BF774__Fv
    [ 178]	func_804BF8A8
    [ 192]	Clear__Q34nw4r3g3d8LightObjFv
    [ 222]	lbl_eu_8066AF80
    [ 238]	func_804BF940
    [ 252]	func_804BF944__Fv
    [ 270]	func_804BFA70__Fv
    [ 288]	func_804BFEB4
    [ 302]	lbl_eu_8066AF88
    [ 318]	lbl_eu_8066AF9C
    [ 334]	lbl_eu_8066AFA0
    [ 350]	func_804BFFB8__Fv
    [ 368]	func_804C0228
    [ 382]	func_804C0254

==> .symtab
    entry  spndx   value      size   bind   type  other    shndx         name
    [   0]     0   00000000      0   LOC    NULL      0    UND           
    [   1]     1   00000000      0   LOC    FILE      0    ABS           code_804BF59C.cpp
    [   2]     0   00000000      0   LOC    SECT      0    .text         .text
    [   3]     0   00000000      0   LOC    SECT      0    extab         extab
    [   4]     0   00000000      0   LOC    SECT      0    extabindex    extabindex
    [   5]    19   00000000      8   LOC    OBJ       2    extab         @6545
    [   6]    25   00000000     12   LOC    OBJ       2    extabindex    @6546
    [   7]    31   00000008      8   LOC    OBJ       2    extab         @6552
    [   8]    37   0000000c     12   LOC    OBJ       2    extabindex    @6553
    [   9]    43   00000000      4   GLOB   FUNC      0    .text         func_804BF59C
    [  10]    57   00000004      4   GLOB   FUNC      0    .text         __ct__804BF5A0__Fv
    [  11]    76   00000008    128   GLOB   FUNC      0    .text         __dt__804BF6F4
    [  12]    91   00000000      0   GLOB   NULL      0    UND           __dt__Q34nw4r3g3d8LightObjFv
    [  13]   120   00000000      0   GLOB   NULL      0    UND           __destroy_arr
    [  14]   134   00000000      0   GLOB   NULL      0    UND           __dt__6CLightFv
    [  15]   150   00000000      0   GLOB   NULL      0    UND           __dl__FPv
    [  16]   160   00000088      4   GLOB   FUNC      0    .text         func_804BF774__Fv
    [  17]   178   0000008c    152   GLOB   FUNC      0    .text         func_804BF8A8
    [  18]   192   00000000      0   GLOB   NULL      0    UND           Clear__Q34nw4r3g3d8LightObjFv
    [  19]   222   00000000      0   GLOB   NULL      0    UND           lbl_eu_8066AF80
    [  20]   238   00000124      4   GLOB   FUNC      0    .text         func_804BF940
    [  21]   252   00000128      4   GLOB   FUNC      0    .text         func_804BF944__Fv
    [  22]   270   0000012c      4   GLOB   FUNC      0    .text         func_804BFA70__Fv
    [  23]   288   00000130    260   GLOB   FUNC      0    .text         func_804BFEB4
    [  24]   302   00000000      0   GLOB   NULL      0    UND           lbl_eu_8066AF88
    [  25]   318   00000000      0   GLOB   NULL      0    UND           lbl_eu_8066AF9C
    [  26]   334   00000000      0   GLOB   NULL      0    UND           lbl_eu_8066AFA0
    [  27]   350   00000234      4   GLOB   FUNC      0    .text         func_804BFFB8__Fv
    [  28]   368   00000238     44   GLOB   FUNC      0    .text         func_804C0228
    [  29]   382   00000264     44   GLOB   FUNC      0    .text         func_804C0254

==> .rela.text
    entry  offset   addend   type name(symbol id)
    [   0] 0000002e 00000000    6 __dt__Q34nw4r3g3d8LightObjFv(12)
    [   1] 00000036 00000000    4 __dt__Q34nw4r3g3d8LightObjFv(12)
    [   2] 00000040 00000000   10 __destroy_arr(13)
    [   3] 00000046 00000000    6 __dt__6CLightFv(14)
    [   4] 0000004e 00000000    4 __dt__6CLightFv(14)
    [   5] 00000058 00000000   10 __destroy_arr(13)
    [   6] 00000068 00000000   10 __dl__FPv(15)
    [   7] 000000bc 00000000   10 Clear__Q34nw4r3g3d8LightObjFv(18)
    [   8] 000000e0 00000000  109 lbl_eu_8066AF80(19)
    [   9] 00000140 00000000  109 lbl_eu_8066AF88(24)
    [  10] 00000158 00000000  109 lbl_eu_8066AF9C(25)
    [  11] 000001bc 00000000  109 lbl_eu_8066AF88(24)
    [  12] 000001d0 00000000  109 lbl_eu_8066AF88(24)
    [  13] 000001e4 00000000  109 lbl_eu_8066AFA0(26)
    [  14] 000001e8 00000000  109 lbl_eu_8066AF80(19)
    [  15] 0000021c 00000000  109 lbl_eu_8066AF80(19)

==> .text

Hunk:	Kind=HUNK_GLOBAL_CODE     Name="func_804BF59C"  Size=4
00000000: 4E800020  blr

Hunk:	Kind=HUNK_GLOBAL_CODE     Name="__ct__804BF5A0__Fv"  Size=4
00000004: 4E800020  blr

Hunk:	Kind=HUNK_GLOBAL_CODE     Name="__dt__804BF6F4"  Size=128
00000008: 9421FFF0  stwu     rsp,-16(rsp)
0000000C: 7C0802A6  mflr     r0
00000010: 2C030000  cmpwi    r3,0x0000
00000014: 90010014  stw      r0,20(rsp)
00000018: 93E1000C  stw      r31,12(rsp)
0000001C: 7C9F2378  mr       r31,r4
00000020: 93C10008  stw      r30,8(rsp)
00000024: 7C7E1B78  mr       r30,r3
00000028: 41820044  beq      *+68                    ; 0x0000006C
0000002C: 3C800000  lis      r4,__dt__Q34nw4r3g3d8LightObjFv@ha
00000030: 38A00044  li       r5,68
00000034: 38840000  addi     r4,r4,__dt__Q34nw4r3g3d8LightObjFv@l
00000038: 38C00008  li       r6,8
0000003C: 38630200  addi     r3,r3,512
00000040: 48000001  bl       __destroy_arr
00000044: 3C800000  lis      r4,__dt__6CLightFv@ha
00000048: 7FC3F378  mr       r3,r30
0000004C: 38840000  addi     r4,r4,__dt__6CLightFv@l
00000050: 38A00040  li       r5,64
00000054: 38C00008  li       r6,8
00000058: 48000001  bl       __destroy_arr
0000005C: 2C1F0000  cmpwi    r31,0x0000
00000060: 4081000C  ble      *+12                    ; 0x0000006C
00000064: 7FC3F378  mr       r3,r30
00000068: 48000001  bl       __dl__FPv
0000006C: 7FC3F378  mr       r3,r30
00000070: 83E1000C  lwz      r31,12(rsp)
00000074: 83C10008  lwz      r30,8(rsp)
00000078: 80010014  lwz      r0,20(rsp)
0000007C: 7C0803A6  mtlr     r0
00000080: 38210010  addi     rsp,rsp,16
00000084: 4E800020  blr

Hunk:	Kind=HUNK_GLOBAL_CODE     Name="func_804BF774__Fv"  Size=4
00000088: 4E800020  blr

Hunk:	Kind=HUNK_GLOBAL_CODE     Name="func_804BF8A8"  Size=152
0000008C: 9421FFE0  stwu     rsp,-32(rsp)
00000090: 7C0802A6  mflr     r0
00000094: 90010024  stw      r0,36(rsp)
00000098: 93E1001C  stw      r31,28(rsp)
0000009C: 3BE30200  addi     r31,r3,512
000000A0: 93C10018  stw      r30,24(rsp)
000000A4: 93A10014  stw      r29,20(rsp)
000000A8: 3BA00000  li       r29,0
000000AC: 93810010  stw      r28,16(rsp)
000000B0: 7C7C1B78  mr       r28,r3
000000B4: 7F9EE378  mr       r30,r28
000000B8: 7FE3FB78  mr       r3,r31
000000BC: 48000001  bl       Clear__Q34nw4r3g3d8LightObjFv
000000C0: 801E0200  lwz      r0,512(r30)
000000C4: 3BBD0001  addi     r29,r29,1
000000C8: 281D0008  cmplwi   r29,0x0008
000000CC: 3BFF0044  addi     r31,r31,68
000000D0: 540007B8  rlwinm   r0,r0,0,30,28
000000D4: 901E0200  stw      r0,512(r30)
000000D8: 3BDE0044  addi     r30,r30,68
000000DC: 4180FFDC  blt      *-36                    ; 0x000000B8
000000E0: C0000000  lfs      fp0,lbl_eu_8066AF80@sda21
000000E4: 38000000  li       r0,0
000000E8: D01C0450  stfs     fp0,1104(r28)
000000EC: D01C0454  stfs     fp0,1108(r28)
000000F0: D01C0458  stfs     fp0,1112(r28)
000000F4: D01C0438  stfs     fp0,1080(r28)
000000F8: D01C043C  stfs     fp0,1084(r28)
000000FC: D01C0440  stfs     fp0,1088(r28)
00000100: 901C0480  stw      r0,1152(r28)
00000104: 83E1001C  lwz      r31,28(rsp)
00000108: 83C10018  lwz      r30,24(rsp)
0000010C: 83A10014  lwz      r29,20(rsp)
00000110: 83810010  lwz      r28,16(rsp)
00000114: 80010024  lwz      r0,36(rsp)
00000118: 7C0803A6  mtlr     r0
0000011C: 38210020  addi     rsp,rsp,32
00000120: 4E800020  blr

Hunk:	Kind=HUNK_GLOBAL_CODE     Name="func_804BF940"  Size=4
00000124: 4E800020  blr

Hunk:	Kind=HUNK_GLOBAL_CODE     Name="func_804BF944__Fv"  Size=4
00000128: 4E800020  blr

Hunk:	Kind=HUNK_GLOBAL_CODE     Name="func_804BFA70__Fv"  Size=4
0000012C: 4E800020  blr

Hunk:	Kind=HUNK_GLOBAL_CODE     Name="func_804BFEB4"  Size=260
00000130: 80030484  lwz      r0,1156(r3)
00000134: 54000085  rlwinm.  r0,r0,0,2,2
00000138: 418200D8  beq      *+216                   ; 0x00000210
0000013C: C0A30468  lfs      fp5,1128(r3)
00000140: C0800000  lfs      fp4,lbl_eu_8066AF88@sda21
00000144: C0630438  lfs      fp3,1080(r3)
00000148: ECC42828  fsubs    fp6,fp4,fp5
0000014C: C043043C  lfs      fp2,1084(r3)
00000150: C0230440  lfs      fp1,1088(r3)
00000154: C0E30444  lfs      fp7,1092(r3)
00000158: C0000000  lfs      fp0,lbl_eu_8066AF9C@sda21
0000015C: EC6301B2  fmuls    fp3,fp3,fp6
00000160: C103045C  lfs      fp8,1116(r3)
00000164: EC4201B2  fmuls    fp2,fp2,fp6
00000168: EC2101B2  fmuls    fp1,fp1,fp6
0000016C: C0C30460  lfs      fp6,1120(r3)
00000170: EC68197A  fmadds   fp3,fp8,fp5,fp3
00000174: ECE001F2  fmuls    fp7,fp0,fp7
00000178: C1030464  lfs      fp8,1124(r3)
0000017C: C1230448  lfs      fp9,1096(r3)
00000180: EC46117A  fmadds   fp2,fp6,fp5,fp2
00000184: C143044C  lfs      fp10,1100(r3)
00000188: ECC00272  fmuls    fp6,fp0,fp9
0000018C: EC6700F2  fmuls    fp3,fp7,fp3
00000190: EC28097A  fmadds   fp1,fp8,fp5,fp1
00000194: EC0002B2  fmuls    fp0,fp0,fp10
00000198: D0630450  stfs     fp3,1104(r3)
0000019C: EC4600B2  fmuls    fp2,fp6,fp2
000001A0: FC032040  fcmpo    cr0,fp3,fp4
000001A4: EC000072  fmuls    fp0,fp0,fp1
000001A8: D0430454  stfs     fp2,1108(r3)
000001AC: D0030458  stfs     fp0,1112(r3)
000001B0: 40810008  ble      *+8                     ; 0x000001B8
000001B4: D0830450  stfs     fp4,1104(r3)
000001B8: C0230454  lfs      fp1,1108(r3)
000001BC: C0000000  lfs      fp0,lbl_eu_8066AF88@sda21
000001C0: FC010040  fcmpo    cr0,fp1,fp0
000001C4: 40810008  ble      *+8                     ; 0x000001CC
000001C8: D0030454  stfs     fp0,1108(r3)
000001CC: C0230458  lfs      fp1,1112(r3)
000001D0: C0000000  lfs      fp0,lbl_eu_8066AF88@sda21
000001D4: FC010040  fcmpo    cr0,fp1,fp0
000001D8: 40810008  ble      *+8                     ; 0x000001E0
000001DC: D0030458  stfs     fp0,1112(r3)
000001E0: C0430468  lfs      fp2,1128(r3)
000001E4: C0200000  lfs      fp1,lbl_eu_8066AFA0@sda21
000001E8: C0000000  lfs      fp0,lbl_eu_8066AF80@sda21
000001EC: EC220828  fsubs    fp1,fp2,fp1
000001F0: D0230468  stfs     fp1,1128(r3)
000001F4: FC010040  fcmpo    cr0,fp1,fp0
000001F8: 40800008  bge      *+8                     ; 0x00000200
000001FC: D0030468  stfs     fp0,1128(r3)
00000200: 80030484  lwz      r0,1156(r3)
00000204: 54000004  clrrwi   r0,r0,29                ; rlwinm     r0,r0,0,0,2
00000208: 90030484  stw      r0,1156(r3)
0000020C: 4E800020  blr
00000210: C0630438  lfs      fp3,1080(r3)
00000214: C043043C  lfs      fp2,1084(r3)
00000218: C0230440  lfs      fp1,1088(r3)
0000021C: C0000000  lfs      fp0,lbl_eu_8066AF80@sda21
00000220: D0630450  stfs     fp3,1104(r3)
00000224: D0430454  stfs     fp2,1108(r3)
00000228: D0230458  stfs     fp1,1112(r3)
0000022C: D003046C  stfs     fp0,1132(r3)
00000230: 4E800020  blr

Hunk:	Kind=HUNK_GLOBAL_CODE     Name="func_804BFFB8__Fv"  Size=4
00000234: 4E800020  blr

Hunk:	Kind=HUNK_GLOBAL_CODE     Name="func_804C0228"  Size=44
00000238: 2C040000  cmpwi    r4,0x0000
0000023C: 41820018  beq      *+24                    ; 0x00000254
00000240: 80030484  lwz      r0,1156(r3)
00000244: 5400062C  rlwinm   r0,r0,0,24,22
00000248: 60000080  ori      r0,r0,0x0080
0000024C: 90030484  stw      r0,1156(r3)
00000250: 4E800020  blr
00000254: 80030484  lwz      r0,1156(r3)
00000258: 5400066E  rlwinm   r0,r0,0,25,23
0000025C: 90030484  stw      r0,1156(r3)
00000260: 4E800020  blr

Hunk:	Kind=HUNK_GLOBAL_CODE     Name="func_804C0254"  Size=44
00000264: 2C040000  cmpwi    r4,0x0000
00000268: 41820018  beq      *+24                    ; 0x00000280
0000026C: 80030484  lwz      r0,1156(r3)
00000270: 5400066E  rlwinm   r0,r0,0,25,23
00000274: 60000100  ori      r0,r0,0x0100
00000278: 90030484  stw      r0,1156(r3)
0000027C: 4E800020  blr
00000280: 80030484  lwz      r0,1156(r3)
00000284: 5400062C  rlwinm   r0,r0,0,24,22
00000288: 90030484  stw      r0,1156(r3)
0000028C: 4E800020  blr

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
