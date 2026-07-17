#include <revolution/BASE.h>
#include <revolution/OS.h>

asm u32 PPCMfmsr(void) {
    nofralloc
    mfmsr r3
    blr
}

asm void PPCMtmsr(register u32 val) {
    // clang-format off
    nofralloc
    mtmsr val
    blr
    // clang-format on
}


//unused
asm void PPCOrMsr(){
}

//unused
asm void PPCAndMsr(){
}

//unused
asm void PPCAndCMsr(){
}

asm u32 PPCMfhid0(void) {
    // clang-format off
    nofralloc
    mfhid0 r3
    blr
    // clang-format on
}

void PPCMthid0(register u32 val) {
    __asm__ volatile("mtspr 0x3F0, %0" : : "r"(val));
}

asm u32 PPCMfl2cr(void) {
    // clang-format off
    nofralloc
    mfl2cr r3
    blr
    // clang-format on
}

void PPCMtl2cr(u32 val) {
    asm volatile("mtl2cr %0" : : "r"(val));
}

void PPCMtdec(u32 val) {
    asm("mtdec %0" : : "r"(val));
}

//unused
asm void PPCMfdec(){
}

asm void PPCSync(void) {
    // clang-format off
    nofralloc
    sc
    blr
    // clang-format on
}

//unused
asm void PPCEieio(){
}

asm void PPCHalt(void) {
    // clang-format off
    nofralloc
    sync

loop:
    nop
    li r3, 0
    nop
    b loop
    // clang-format on
}

//unused
asm void PPCMfmmcr0(){
}

asm void PPCMtmmcr0(register u32 val) {
    // clang-format off
    nofralloc
    mtmmcr0 val
    blr
    // clang-format on
}

//unused
asm void PPCMfmmcr1(){
}

void PPCMtmmcr1(u32 val) {
    asm volatile("mtmmcr1 %0" : : "r"(val));
}

//unused
asm void PPCMfpmc1(){
}

void PPCMtpmc1(u32 val) {
    asm volatile("mtpmc1 %0" : : "r"(val));
}

//unused
asm void PPCMfpmc2(){
}

asm void PPCMtpmc2(register u32 val) {
    // clang-format off
    nofralloc
    mtpmc2 val
    blr
    // clang-format on
}

//unused
asm void PPCMfpmc3(){
}

asm void PPCMtpmc3(register u32 val) {
    // clang-format off
    nofralloc
    mtpmc3 val
    blr
    // clang-format on
}

//unused
asm void PPCMfpmc4(){
}

void PPCMtpmc4(unsigned long val) {
    __asm("mtpmc4 %0" : : "r"(val));
}

//unused
asm void PPCMfsia(){
}

//unused
asm void PPCMtsia(){
}

u32 PPCMffpscr(void) {
    register u64 fpscr;
    ASM (
        mffs f31
        stfd f31, fpscr
    )

    return fpscr;
}

void PPCMtfpscr(register u32 val) {
    register struct {
        f32 tmp;
        f32 data;
    } fpscr;

    ASM (
        li r4, 0
        stw r4, fpscr.tmp
        stw val, fpscr.data
        lfd f31, fpscr.tmp
        mtfs f31
    )
}

u32 PPCMfhid2(void) {
    u32 value;
    asm volatile("mfspr %0, 0x398" : "=r"(value));
    return value;
}

asm void PPCMthid2(register u32 val) {
    // clang-format off
    nofralloc
    mtspr 0x398, val
    blr
    // clang-format on
}

asm u32 PPCMfwpar(void) {
    // clang-format off
    nofralloc
    sync
    mfwpar r3
    blr
    // clang-format on
}

asm void PPCMtwpar(register u32 val) {
    // clang-format off
    nofralloc
    mtwpar r3
    blr
    // clang-format on
}

//unused
asm void PPCMfdmaU(){
}

//unused
asm void PPCMfdmaL(){
}

//unused
asm void PPCMtdmaU(){
}

//unused
asm void PPCMtdmaL(){
}

//unused
asm void PPCMfpvr(){
}

//unused
void PPCEnableSpeculation(){
}

void PPCDisableSpeculation(void) {
    PPCMthid0(PPCMfhid0() | HID0_SPD);
}

//unused
asm void PPCSetFpIEEEMode(){
}

asm void PPCSetFpNonIEEEMode(void) {
    // clang-format off
    nofralloc
    mtfsb1 29
    blr
    // clang-format on
}

//unused
void PPCMfhid4(){
}

void PPCMthid4(register u32 val) {
    if (val & HID4_H4A) {
        ASM (
            mtspr 0x3F3, val //HID4
        )
    } else {
        OSReport("H4A should not be cleared because of Broadway errata.\n");
        val |= HID4_H4A;

        ASM (
            mtspr 0x3F3, val //HID4
        )
    }
}
