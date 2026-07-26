// Auto-scaffolded catalog TU for CriWare/src/sofdec/sfdcore/sfh/sfh_ver2
// Replace stubs with high-level C/C++ during decomp.

#include <harness_catalog.h>

extern int SFHLOCAL_GetSizeofMember(int, int);
extern int SFHLOCAL_GetNbyteB();

void VER2_IsSfdHeader() {}

void VER2_IsExistStmId() {}

void VER2_IsEffFtrInf() {}

void searchStmId() {}

void VER2_AnlyHdrToolVer() {}

void VER2_AnlyHdrSfhVer() {}

void VER2_AnlyHdrModuleVer() {}


int VER2_AnlyHdrSiz(const unsigned int *hdr, int *out_size) {
    int sz;
    *out_size = 0;
    sz = SFHLOCAL_GetSizeofMember(0x12, 0x14);
    *out_size = SFHLOCAL_GetNbyteB(hdr[1] + 0x12, sz);
    return 1;
}

u32 VER2_AnlyPackType(void* self, u32* out) {
    *out = 0;
    return 1;
}

u32 VER2_AnlyPketSizLen(void* self, u32* out) {
    *out = 2;
    return 1;
}

void VER2_AnlyPackSiz() {}


int VER2_AnlyNumElemTot(char *work, int *out) {
    *out = 0;
    int size = SFHLOCAL_GetSizeofMember(0xc0, 0xc1);
    *out = SFHLOCAL_GetNbyteB(*(char **)(work + 4) + 0xc0, size);
    return 1;
}

void VER2_AnlyNumElemAud() {}


int VER2_AnlyNumElemVid(unsigned char **ctx, int *out) {
    *out = 0;
    int size = SFHLOCAL_GetSizeofMember(0xc2, 0xc3);
    *out = SFHLOCAL_GetNbyteB(ctx[1] + 0xc2, size);
    return 1;
}

void VER2_AnlyNumElemPrv() {}

int VER2_AnlyByteRate(unsigned char **work, int *out) {
    *out = 0;
    *out = SFHLOCAL_GetNbyteB(work[1] + 0xc4, SFHLOCAL_GetSizeofMember(0xc4, 0xc8));
    return 1;
}

void VER2_AnlyMaxPlyLenAud() {}

void VER2_AnlyMaxPlyLenVid() {}

void VER2_AnlyMaxFrmNum() {}

void VER2_AnlyMaxPicSiz() {}

void VER2_AnlyDiffTime() {}

void VER2_AnlyElemCodecAud() {}

void VER2_AnlyElemLayer() {}

void VER2_AnlyElemChNum() {}

void VER2_AnlyElemSmpHz() {}

void VER2_AnlyElemCodecVid() {}

void VER2_AnlyElemAvrBitRate() {}

void VER2_AnlyElemPicSz() {}

void VER2_AnlyElemPicRate() {}

void VER2_AnlyFtrColType() {}

void VER2_AnlyFtrPicType() {}

void VER2_AnlyFtrFixFlg() {}

void VER2_AnlyFtrShcFixFlg() {}

void VER2_AnlyFtrExpand() {}

void VER2_AnlyFtrGopN() {}

void VER2_AnlyFtrGopM() {}

void VER2_AnlyFtrFxType() {}

void VER2_AnlyFtrNetWidth() {}

void VER2_AnlyFtrNetHeight() {}
