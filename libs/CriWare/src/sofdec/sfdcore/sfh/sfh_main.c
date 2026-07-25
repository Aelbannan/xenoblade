// Auto-scaffolded catalog TU for CriWare/src/sofdec/sfdcore/sfh/sfh_main
// Mangled extern stubs for llm-harness / coop selection.
// Replace stubs with high-level C/C++ during decomp.

#include <harness_catalog.h>

// LLM-HARNESS-BEGIN: us-803d2e40
extern u32 lbl_eu_80619BE8;
void SFH_Init(void) {
    lbl_eu_80619BE8++;
}
// LLM-HARNESS-END: us-803d2e40

// LLM-HARNESS-BEGIN: us-803d2e54
void SFH_Create() {}
// LLM-HARNESS-END: us-803d2e54

// LLM-HARNESS-BEGIN: us-803d2ee8
void SFH_Destroy(void) {}
// LLM-HARNESS-END: us-803d2ee8

// LLM-HARNESS-BEGIN: us-803d2ef4
void SFH_IsSfdHeader() {}
// LLM-HARNESS-END: us-803d2ef4

// LLM-HARNESS-BEGIN: us-803d30b8
u32 VER1_IsExistStmId(void* buf);
u32 VER2_IsExistStmId(void* buf);
u32 SFH_IsExistStmId(void* buf) {
    s32 ver = *(s32*)((u8*)buf + 0x10);
    if (ver < 0xC8) return VER1_IsExistStmId(buf);
    if (ver < 0x12C) return VER2_IsExistStmId(buf);
    return 0;
}
// LLM-HARNESS-END: us-803d30b8

// LLM-HARNESS-BEGIN: us-803d30dc
u32 VER1_IsEffFtrInf(void* buf);
u32 VER2_IsEffFtrInf(void* buf);
u32 SFH_IsEffFtrInf(void* buf) {
    s32 ver = *(s32*)((u8*)buf + 0x10);
    if (ver < 0xC8) return VER1_IsEffFtrInf(buf);
    if (ver < 0x12C) return VER2_IsEffFtrInf(buf);
    return 0;
}
// LLM-HARNESS-END: us-803d30dc

// LLM-HARNESS-BEGIN: us-803d3100
u32 VER1_AnlyHdrToolVer(void* buf);
u32 VER2_AnlyHdrToolVer(void* buf);
u32 SFH_AnlyHdrToolVer(void* buf) {
    s32 ver = *(s32*)((u8*)buf + 0x10);
    if (ver < 0xC8) return VER1_AnlyHdrToolVer(buf);
    if (ver < 0x12C) return VER2_AnlyHdrToolVer(buf);
    return 0;
}
// LLM-HARNESS-END: us-803d3100

// LLM-HARNESS-BEGIN: us-803d3124
u32 VER1_AnlyHdrSiz(void* buf);
u32 VER2_AnlyHdrSiz(void* buf);
u32 SFH_AnlyHdrSiz(void* buf) {
    s32 ver = *(s32*)((u8*)buf + 0x10);
    if (ver < 0xC8) return VER1_AnlyHdrSiz(buf);
    if (ver < 0x12C) return VER2_AnlyHdrSiz(buf);
    return 0;
}
// LLM-HARNESS-END: us-803d3124

// LLM-HARNESS-BEGIN: us-803d3148
u32 VER1_AnlyPackType(void* buf);
u32 VER2_AnlyPackType(void* buf);
u32 SFH_AnlyPackType(void* buf) {
    s32 ver = *(s32*)((u8*)buf + 0x10);
    if (ver < 0xC8) return VER1_AnlyPackType(buf);
    if (ver < 0x12C) return VER2_AnlyPackType(buf);
    return 0;
}
// LLM-HARNESS-END: us-803d3148

// LLM-HARNESS-BEGIN: us-803d316c
u32 VER1_AnlyPketSizLen(void* buf);
u32 VER2_AnlyPketSizLen(void* buf);
u32 SFH_AnlyPketSizLen(void* buf) {
    s32 ver = *(s32*)((u8*)buf + 0x10);
    if (ver < 0xC8) return VER1_AnlyPketSizLen(buf);
    if (ver < 0x12C) return VER2_AnlyPketSizLen(buf);
    return 0;
}
// LLM-HARNESS-END: us-803d316c

// LLM-HARNESS-BEGIN: us-803d3190
u32 VER1_AnlyPackSiz(void* buf);
u32 VER2_AnlyPackSiz(void* buf);
u32 SFH_AnlyPackSiz(void* buf) {
    s32 ver = *(s32*)((u8*)buf + 0x10);
    if (ver < 0xC8) return VER1_AnlyPackSiz(buf);
    if (ver < 0x12C) return VER2_AnlyPackSiz(buf);
    return 0;
}
// LLM-HARNESS-END: us-803d3190

// LLM-HARNESS-BEGIN: us-803d31b4
u32 VER1_AnlyNumElemTot(void* buf);
u32 VER2_AnlyNumElemTot(void* buf);
u32 SFH_AnlyNumElemTot(void* buf) {
    s32 ver = *(s32*)((u8*)buf + 0x10);
    if (ver < 0xC8) return VER1_AnlyNumElemTot(buf);
    if (ver < 0x12C) return VER2_AnlyNumElemTot(buf);
    return 0;
}
// LLM-HARNESS-END: us-803d31b4

// LLM-HARNESS-BEGIN: us-803d31d8
u32 VER1_AnlyNumElemAud(void* buf);
u32 VER2_AnlyNumElemAud(void* buf);
u32 SFH_AnlyNumElemAud(void* buf) {
    s32 ver = *(s32*)((u8*)buf + 0x10);
    if (ver < 0xC8) return VER1_AnlyNumElemAud(buf);
    if (ver < 0x12C) return VER2_AnlyNumElemAud(buf);
    return 0;
}
// LLM-HARNESS-END: us-803d31d8

// LLM-HARNESS-BEGIN: us-803d31fc
u32 VER1_AnlyNumElemVid(void* buf);
u32 VER2_AnlyNumElemVid(void* buf);
u32 SFH_AnlyNumElemVid(void* buf) {
    s32 ver = *(s32*)((u8*)buf + 0x10);
    if (ver < 0xC8) return VER1_AnlyNumElemVid(buf);
    if (ver < 0x12C) return VER2_AnlyNumElemVid(buf);
    return 0;
}
// LLM-HARNESS-END: us-803d31fc

// LLM-HARNESS-BEGIN: us-803d3220
u32 VER1_AnlyNumElemPrv(void* buf);
u32 VER2_AnlyNumElemPrv(void* buf);
u32 SFH_AnlyNumElemPrv(void* buf) {
    s32 ver = *(s32*)((u8*)buf + 0x10);
    if (ver < 0xC8) return VER1_AnlyNumElemPrv(buf);
    if (ver < 0x12C) return VER2_AnlyNumElemPrv(buf);
    return 0;
}
// LLM-HARNESS-END: us-803d3220

// LLM-HARNESS-BEGIN: us-803d3244
u32 VER1_AnlyByteRate(void* buf);
u32 VER2_AnlyByteRate(void* buf);
u32 SFH_AnlyByteRate(void* buf) {
    s32 ver = *(s32*)((u8*)buf + 0x10);
    if (ver < 0xC8) return VER1_AnlyByteRate(buf);
    if (ver < 0x12C) return VER2_AnlyByteRate(buf);
    return 0;
}
// LLM-HARNESS-END: us-803d3244

// LLM-HARNESS-BEGIN: us-803d3268
u32 VER1_AnlyMaxPlyLenAud(void* buf);
u32 VER2_AnlyMaxPlyLenAud(void* buf);
u32 SFH_AnlyMaxPlyLenAud(void* buf) {
    s32 ver = *(s32*)((u8*)buf + 0x10);
    if (ver < 0xC8) return VER1_AnlyMaxPlyLenAud(buf);
    if (ver < 0x12C) return VER2_AnlyMaxPlyLenAud(buf);
    return 0;
}
// LLM-HARNESS-END: us-803d3268

// LLM-HARNESS-BEGIN: us-803d328c
u32 VER1_AnlyMaxPlyLenVid(void* buf);
u32 VER2_AnlyMaxPlyLenVid(void* buf);
u32 SFH_AnlyMaxPlyLenVid(void* buf) {
    s32 ver = *(s32*)((u8*)buf + 0x10);
    if (ver < 0xC8) return VER1_AnlyMaxPlyLenVid(buf);
    if (ver < 0x12C) return VER2_AnlyMaxPlyLenVid(buf);
    return 0;
}
// LLM-HARNESS-END: us-803d328c

// LLM-HARNESS-BEGIN: us-803d32b0
u32 VER1_AnlyMaxFrmNum(void* buf);
u32 VER2_AnlyMaxFrmNum(void* buf);
u32 SFH_AnlyMaxFrmNum(void* buf) {
    s32 ver = *(s32*)((u8*)buf + 0x10);
    if (ver < 0xC8) return VER1_AnlyMaxFrmNum(buf);
    if (ver < 0x12C) return VER2_AnlyMaxFrmNum(buf);
    return 0;
}
// LLM-HARNESS-END: us-803d32b0

// LLM-HARNESS-BEGIN: us-803d32d4
u32 VER1_AnlyMaxPicSiz(void* buf);
u32 VER2_AnlyMaxPicSiz(void* buf);
u32 SFH_AnlyMaxPicSiz(void* buf) {
    s32 ver = *(s32*)((u8*)buf + 0x10);
    if (ver < 0xC8) return VER1_AnlyMaxPicSiz(buf);
    if (ver < 0x12C) return VER2_AnlyMaxPicSiz(buf);
    return 0;
}
// LLM-HARNESS-END: us-803d32d4

// LLM-HARNESS-BEGIN: us-803d32f8
u32 VER1_AnlyDiffTime(void* buf);
u32 VER2_AnlyDiffTime(void* buf);
u32 SFH_AnlyDiffTime(void* buf) {
    s32 ver = *(s32*)((u8*)buf + 0x10);
    if (ver < 0xC8) return VER1_AnlyDiffTime(buf);
    if (ver < 0x12C) return VER2_AnlyDiffTime(buf);
    return 0;
}
// LLM-HARNESS-END: us-803d32f8

// LLM-HARNESS-BEGIN: us-803d3320
u32 VER1_AnlyElemCodecAud(void* buf);
u32 VER2_AnlyElemCodecAud(void* buf);
u32 SFH_AnlyElemCodecAud(void* buf) {
    s32 ver = *(s32*)((u8*)buf + 0x10);
    if (ver < 0xC8) return VER1_AnlyElemCodecAud(buf);
    if (ver < 0x12C) return VER2_AnlyElemCodecAud(buf);
    return 0;
}
// LLM-HARNESS-END: us-803d3320

// LLM-HARNESS-BEGIN: us-803d3344
u32 VER1_AnlyElemLayer(void* buf);
u32 VER2_AnlyElemLayer(void* buf);
u32 SFH_AnlyElemLayer(void* buf) {
    s32 ver = *(s32*)((u8*)buf + 0x10);
    if (ver < 0xC8) return VER1_AnlyElemLayer(buf);
    if (ver < 0x12C) return VER2_AnlyElemLayer(buf);
    return 0;
}
// LLM-HARNESS-END: us-803d3344

// LLM-HARNESS-BEGIN: us-803d3368
u32 VER1_AnlyElemChNum(void* buf);
u32 VER2_AnlyElemChNum(void* buf);
u32 SFH_AnlyElemChNum(void* buf) {
    s32 ver = *(s32*)((u8*)buf + 0x10);
    if (ver < 0xC8) return VER1_AnlyElemChNum(buf);
    if (ver < 0x12C) return VER2_AnlyElemChNum(buf);
    return 0;
}
// LLM-HARNESS-END: us-803d3368

// LLM-HARNESS-BEGIN: us-803d338c
u32 VER1_AnlyElemSmpHz(void* buf);
u32 VER2_AnlyElemSmpHz(void* buf);
u32 SFH_AnlyElemSmpHz(void* buf) {
    s32 ver = *(s32*)((u8*)buf + 0x10);
    if (ver < 0xC8) return VER1_AnlyElemSmpHz(buf);
    if (ver < 0x12C) return VER2_AnlyElemSmpHz(buf);
    return 0;
}
// LLM-HARNESS-END: us-803d338c

// LLM-HARNESS-BEGIN: us-803d33b0
u32 VER1_AnlyElemCodecVid(void* buf);
u32 VER2_AnlyElemCodecVid(void* buf);
u32 SFH_AnlyElemCodecVid(void* buf) {
    s32 ver = *(s32*)((u8*)buf + 0x10);
    if (ver < 0xC8) return VER1_AnlyElemCodecVid(buf);
    if (ver < 0x12C) return VER2_AnlyElemCodecVid(buf);
    return 0;
}
// LLM-HARNESS-END: us-803d33b0

// LLM-HARNESS-BEGIN: us-803d33d4
u32 VER1_AnlyElemBitRate(void* buf);
u32 VER2_AnlyElemAvrBitRate(void* buf);
u32 SFH_AnlyElemBitRate(void* buf) {
    s32 ver = *(s32*)((u8*)buf + 0x10);
    if (ver < 0xC8) return VER1_AnlyElemBitRate(buf);
    if (ver < 0x12C) return VER2_AnlyElemAvrBitRate(buf);
    return 0;
}
// LLM-HARNESS-END: us-803d33d4

// LLM-HARNESS-BEGIN: us-803d33f8
u32 VER1_AnlyElemPicSz(void* buf);
u32 VER2_AnlyElemPicSz(void* buf);
u32 SFH_AnlyElemPicSz(void* buf) {
    s32 ver = *(s32*)((u8*)buf + 0x10);
    if (ver < 0xC8) return VER1_AnlyElemPicSz(buf);
    if (ver < 0x12C) return VER2_AnlyElemPicSz(buf);
    return 0;
}
// LLM-HARNESS-END: us-803d33f8

// LLM-HARNESS-BEGIN: us-803d341c
u32 VER1_AnlyElemPicRate(void* buf);
u32 VER2_AnlyElemPicRate(void* buf);
u32 SFH_AnlyElemPicRate(void* buf) {
    s32 ver = *(s32*)((u8*)buf + 0x10);
    if (ver < 0xC8) return VER1_AnlyElemPicRate(buf);
    if (ver < 0x12C) return VER2_AnlyElemPicRate(buf);
    return 0;
}
// LLM-HARNESS-END: us-803d341c

// LLM-HARNESS-BEGIN: us-803d3440
u32 VER1_AnlyFtrColType(void* buf);
u32 VER2_AnlyFtrColType(void* buf);
u32 SFH_AnlyFtrColType(void* buf) {
    s32 ver = *(s32*)((u8*)buf + 0x10);
    if (ver < 0xC8) return VER1_AnlyFtrColType(buf);
    if (ver < 0x12C) return VER2_AnlyFtrColType(buf);
    return 0;
}
// LLM-HARNESS-END: us-803d3440

// LLM-HARNESS-BEGIN: us-803d3464
u32 VER1_AnlyFtrPicType(void* buf);
u32 VER2_AnlyFtrPicType(void* buf);
u32 SFH_AnlyFtrPicType(void* buf) {
    s32 ver = *(s32*)((u8*)buf + 0x10);
    if (ver < 0xC8) return VER1_AnlyFtrPicType(buf);
    if (ver < 0x12C) return VER2_AnlyFtrPicType(buf);
    return 0;
}
// LLM-HARNESS-END: us-803d3464

// LLM-HARNESS-BEGIN: us-803d3488
u32 VER1_AnlyFtrFixFlg(void* buf);
u32 VER2_AnlyFtrFixFlg(void* buf);
u32 SFH_AnlyFtrFixFlg(void* buf) {
    s32 ver = *(s32*)((u8*)buf + 0x10);
    if (ver < 0xC8) return VER1_AnlyFtrFixFlg(buf);
    if (ver < 0x12C) return VER2_AnlyFtrFixFlg(buf);
    return 0;
}
// LLM-HARNESS-END: us-803d3488

// LLM-HARNESS-BEGIN: us-803d34ac
u32 VER1_AnlyFtrShcFixFlg(void* buf);
u32 VER2_AnlyFtrShcFixFlg(void* buf);
u32 SFH_AnlyFtrShcFixFlg(void* buf) {
    s32 ver = *(s32*)((u8*)buf + 0x10);
    if (ver < 0xC8) return VER1_AnlyFtrShcFixFlg(buf);
    if (ver < 0x12C) return VER2_AnlyFtrShcFixFlg(buf);
    return 0;
}
// LLM-HARNESS-END: us-803d34ac

// LLM-HARNESS-BEGIN: us-803d34d0
u32 VER1_AnlyFtrExpand(void* buf);
u32 VER2_AnlyFtrExpand(void* buf);
u32 SFH_AnlyFtrExpand(void* buf) {
    s32 ver = *(s32*)((u8*)buf + 0x10);
    if (ver < 0xC8) return VER1_AnlyFtrExpand(buf);
    if (ver < 0x12C) return VER2_AnlyFtrExpand(buf);
    return 0;
}
// LLM-HARNESS-END: us-803d34d0

// LLM-HARNESS-BEGIN: us-803d34f4
u32 VER1_AnlyFtrGopN(void* buf);
u32 VER2_AnlyFtrGopN(void* buf);
u32 SFH_AnlyFtrGopN(void* buf) {
    s32 ver = *(s32*)((u8*)buf + 0x10);
    if (ver < 0xC8) return VER1_AnlyFtrGopN(buf);
    if (ver < 0x12C) return VER2_AnlyFtrGopN(buf);
    return 0;
}
// LLM-HARNESS-END: us-803d34f4

// LLM-HARNESS-BEGIN: us-803d3518
u32 VER1_AnlyFtrGopM(void* buf);
u32 VER2_AnlyFtrGopM(void* buf);
u32 SFH_AnlyFtrGopM(void* buf) {
    s32 ver = *(s32*)((u8*)buf + 0x10);
    if (ver < 0xC8) return VER1_AnlyFtrGopM(buf);
    if (ver < 0x12C) return VER2_AnlyFtrGopM(buf);
    return 0;
}
// LLM-HARNESS-END: us-803d3518

// LLM-HARNESS-BEGIN: us-803d353c
u32 VER1_AnlyFtrFxType(void* buf);
u32 VER2_AnlyFtrFxType(void* buf);
u32 SFH_AnlyFtrFxType(void* buf) {
    s32 ver = *(s32*)((u8*)buf + 0x10);
    if (ver < 0xC8) return VER1_AnlyFtrFxType(buf);
    if (ver < 0x12C) return VER2_AnlyFtrFxType(buf);
    return 0;
}
// LLM-HARNESS-END: us-803d353c

// LLM-HARNESS-BEGIN: us-803d3560
u32 VER1_AnlyFtrNetWidth(void* buf);
u32 VER2_AnlyFtrNetWidth(void* buf);
u32 SFH_AnlyFtrNetWidth(void* buf) {
    s32 ver = *(s32*)((u8*)buf + 0x10);
    if (ver < 0xC8) return VER1_AnlyFtrNetWidth(buf);
    if (ver < 0x12C) return VER2_AnlyFtrNetWidth(buf);
    return 0;
}
// LLM-HARNESS-END: us-803d3560

// LLM-HARNESS-BEGIN: us-803d3584
u32 VER1_AnlyFtrNetHeight(void* buf);
u32 VER2_AnlyFtrNetHeight(void* buf);
u32 SFH_AnlyFtrNetHeight(void* buf) {
    s32 ver = *(s32*)((u8*)buf + 0x10);
    if (ver < 0xC8) return VER1_AnlyFtrNetHeight(buf);
    if (ver < 0x12C) return VER2_AnlyFtrNetHeight(buf);
    return 0;
}
// LLM-HARNESS-END: us-803d3584
