// Auto-scaffolded catalog TU for CriWare/src/sofdec/sfdcore/sfh/sfh_ver2
// Mangled extern stubs for llm-harness / coop selection.
// Replace stubs with high-level C/C++ during decomp.

#include <harness_catalog.h>

extern int SFHLOCAL_GetSizeofMember(int, int);
extern int SFHLOCAL_GetNbyteB();

// LLM-HARNESS-BEGIN: us-803d53cc
void VER2_IsSfdHeader() {}
// LLM-HARNESS-END: us-803d53cc

// LLM-HARNESS-BEGIN: us-803d560c
void VER2_IsExistStmId() {}
// LLM-HARNESS-END: us-803d560c

// LLM-HARNESS-BEGIN: us-803d5664
void VER2_IsEffFtrInf() {}
// LLM-HARNESS-END: us-803d5664

// LLM-HARNESS-BEGIN: us-803d5730
void searchStmId() {}
// LLM-HARNESS-END: us-803d5730

// LLM-HARNESS-BEGIN: us-803d595c
void VER2_AnlyHdrToolVer() {}
// LLM-HARNESS-END: us-803d595c

// LLM-HARNESS-BEGIN: us-803d5ad0
void VER2_AnlyHdrSfhVer() {}
// LLM-HARNESS-END: us-803d5ad0

// LLM-HARNESS-BEGIN: us-803d5b60
void VER2_AnlyHdrModuleVer() {}
// LLM-HARNESS-END: us-803d5b60

// LLM-HARNESS-BEGIN: us-803d5bf0

int VER2_AnlyHdrSiz(const unsigned int *hdr, int *out_size) {
    int sz;
    *out_size = 0;
    sz = SFHLOCAL_GetSizeofMember(0x12, 0x14);
    *out_size = SFHLOCAL_GetNbyteB(hdr[1] + 0x12, sz);
    return 1;
}
// LLM-HARNESS-END: us-803d5bf0

// LLM-HARNESS-BEGIN: us-803d5c50
u32 VER2_AnlyPackType(void* self, u32* out) {
    *out = 0;
    return 1;
}
// LLM-HARNESS-END: us-803d5c50

// LLM-HARNESS-BEGIN: us-803d5c60
u32 VER2_AnlyPketSizLen(void* self, u32* out) {
    *out = 2;
    return 1;
}
// LLM-HARNESS-END: us-803d5c60

// LLM-HARNESS-BEGIN: us-803d5c70
void VER2_AnlyPackSiz() {}
// LLM-HARNESS-END: us-803d5c70

// LLM-HARNESS-BEGIN: us-803d5cd0

int VER2_AnlyNumElemTot(char *work, int *out) {
    *out = 0;
    int size = SFHLOCAL_GetSizeofMember(0xc0, 0xc1);
    *out = SFHLOCAL_GetNbyteB(*(char **)(work + 4) + 0xc0, size);
    return 1;
}
// LLM-HARNESS-END: us-803d5cd0

// LLM-HARNESS-BEGIN: us-803d5d30
void VER2_AnlyNumElemAud() {}
// LLM-HARNESS-END: us-803d5d30

// LLM-HARNESS-BEGIN: us-803d5d90

int VER2_AnlyNumElemVid(unsigned char **ctx, int *out) {
    *out = 0;
    int size = SFHLOCAL_GetSizeofMember(0xc2, 0xc3);
    *out = SFHLOCAL_GetNbyteB(ctx[1] + 0xc2, size);
    return 1;
}
// LLM-HARNESS-END: us-803d5d90

// LLM-HARNESS-BEGIN: us-803d5df0
void VER2_AnlyNumElemPrv() {}
// LLM-HARNESS-END: us-803d5df0

// LLM-HARNESS-BEGIN: us-803d5e50
int VER2_AnlyByteRate(unsigned char **work, int *out) {
    *out = 0;
    *out = SFHLOCAL_GetNbyteB(work[1] + 0xc4, SFHLOCAL_GetSizeofMember(0xc4, 0xc8));
    return 1;
}
// LLM-HARNESS-END: us-803d5e50

// LLM-HARNESS-BEGIN: us-803d5eb0
void VER2_AnlyMaxPlyLenAud() {}
// LLM-HARNESS-END: us-803d5eb0

// LLM-HARNESS-BEGIN: us-803d5f10
void VER2_AnlyMaxPlyLenVid() {}
// LLM-HARNESS-END: us-803d5f10

// LLM-HARNESS-BEGIN: us-803d5f70
void VER2_AnlyMaxFrmNum() {}
// LLM-HARNESS-END: us-803d5f70

// LLM-HARNESS-BEGIN: us-803d5fd0
void VER2_AnlyMaxPicSiz() {}
// LLM-HARNESS-END: us-803d5fd0

// LLM-HARNESS-BEGIN: us-803d6030
void VER2_AnlyDiffTime() {}
// LLM-HARNESS-END: us-803d6030

// LLM-HARNESS-BEGIN: us-803d60ac
void VER2_AnlyElemCodecAud() {}
// LLM-HARNESS-END: us-803d60ac

// LLM-HARNESS-BEGIN: us-803d61b4
void VER2_AnlyElemLayer() {}
// LLM-HARNESS-END: us-803d61b4

// LLM-HARNESS-BEGIN: us-803d6280
void VER2_AnlyElemChNum() {}
// LLM-HARNESS-END: us-803d6280

// LLM-HARNESS-BEGIN: us-803d62f4
void VER2_AnlyElemSmpHz() {}
// LLM-HARNESS-END: us-803d62f4

// LLM-HARNESS-BEGIN: us-803d6368
void VER2_AnlyElemCodecVid() {}
// LLM-HARNESS-END: us-803d6368

// LLM-HARNESS-BEGIN: us-803d6470
void VER2_AnlyElemAvrBitRate() {}
// LLM-HARNESS-END: us-803d6470

// LLM-HARNESS-BEGIN: us-803d64f0
void VER2_AnlyElemPicSz() {}
// LLM-HARNESS-END: us-803d64f0

// LLM-HARNESS-BEGIN: us-803d658c
void VER2_AnlyElemPicRate() {}
// LLM-HARNESS-END: us-803d658c

// LLM-HARNESS-BEGIN: us-803d65fc
void VER2_AnlyFtrColType() {}
// LLM-HARNESS-END: us-803d65fc

// LLM-HARNESS-BEGIN: us-803d666c
void VER2_AnlyFtrPicType() {}
// LLM-HARNESS-END: us-803d666c

// LLM-HARNESS-BEGIN: us-803d66dc
void VER2_AnlyFtrFixFlg() {}
// LLM-HARNESS-END: us-803d66dc

// LLM-HARNESS-BEGIN: us-803d6750
void VER2_AnlyFtrShcFixFlg() {}
// LLM-HARNESS-END: us-803d6750

// LLM-HARNESS-BEGIN: us-803d67c4
void VER2_AnlyFtrExpand() {}
// LLM-HARNESS-END: us-803d67c4

// LLM-HARNESS-BEGIN: us-803d6834
void VER2_AnlyFtrGopN() {}
// LLM-HARNESS-END: us-803d6834

// LLM-HARNESS-BEGIN: us-803d68b0
void VER2_AnlyFtrGopM() {}
// LLM-HARNESS-END: us-803d68b0

// LLM-HARNESS-BEGIN: us-803d692c
void VER2_AnlyFtrFxType() {}
// LLM-HARNESS-END: us-803d692c

// LLM-HARNESS-BEGIN: us-803d699c
void VER2_AnlyFtrNetWidth() {}
// LLM-HARNESS-END: us-803d699c

// LLM-HARNESS-BEGIN: us-803d6a20
void VER2_AnlyFtrNetHeight() {}
// LLM-HARNESS-END: us-803d6a20
