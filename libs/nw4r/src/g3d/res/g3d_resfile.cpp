#include <nw4r/g3d.h>


nw4r::g3d::ResNameData27 lbl_eu_80568F60 ALIGN(8) __attribute__((used)) = {14, "3DModels(NW4R)"};
nw4r::g3d::ResNameData27 lbl_eu_80568F80 ALIGN(8) __attribute__((used)) = {14, "Palettes(NW4R)"};
nw4r::g3d::ResNameData27 lbl_eu_80568FA0 ALIGN(8) __attribute__((used)) = {14, "Textures(NW4R)"};
nw4r::g3d::ResNameData27 lbl_eu_80568FC0 ALIGN(8) __attribute__((used)) = {12, "AnmChr(NW4R)"};
nw4r::g3d::ResNameData27 lbl_eu_80569000 ALIGN(8) __attribute__((used)) = {12, "AnmVis(NW4R)"};
nw4r::g3d::ResNameData27 lbl_eu_80569020 ALIGN(8) __attribute__((used)) = {12, "AnmClr(NW4R)"};
nw4r::g3d::ResNameData27 lbl_eu_80569040 ALIGN(8) __attribute__((used)) = {15, "AnmTexPat(NW4R)"};
nw4r::g3d::ResNameData27 lbl_eu_80569060 ALIGN(8) __attribute__((used)) = {15, "AnmTexSrt(NW4R)"};
nw4r::g3d::ResNameData27 lbl_eu_80569080 ALIGN(8) __attribute__((used)) = {12, "AnmShp(NW4R)"};
nw4r::g3d::ResNameData27 lbl_eu_805690A0 ALIGN(8) __attribute__((used)) = {12, "AnmScn(NW4R)"};
nw4r::g3d::ResNameData27 lbl_eu_805690C0 ALIGN(8) __attribute__((used)) = {8, "External"};

namespace nw4r {
namespace g3d {
namespace {

} // namespace

/******************************************************************************
 *
 * ResMdl
 *
 ******************************************************************************/
ResMdl ResFile::GetResMdl(int idx) const {
    void* pResMdlDicData = ResDic(const_cast<ResDicData*>(
        &ref().dict.topLevel))[ResName(&lbl_eu_80568F60)];

    if (pResMdlDicData != NULL) {
        return ResMdl(ResDic(pResMdlDicData)[idx]);
    }

    return ResMdl(NULL);
}

ResMdl ResFile::GetResMdl(u32 idx) const {
    return GetResMdl(static_cast<int>(idx));
}

u32 ResFile::GetResMdlNumEntries() const {
    void* pResMdlDicData = ResDic(const_cast<ResDicData*>(
        &ref().dict.topLevel))[ResName(&lbl_eu_80568F60)];

    if (pResMdlDicData != NULL) {
        return ResDic(pResMdlDicData).GetNumData();
    }

    return 0;
}

/******************************************************************************
 *
 * ResPltt
 *
 ******************************************************************************/
ResPltt ResFile::GetResPltt(const char* pName) const {
    void* pResPlttDicData = ResDic(const_cast<ResDicData*>(
        &ref().dict.topLevel))[ResName(&lbl_eu_80568F80)];

    if (pResPlttDicData != NULL) {
        return ResPltt(ResDic(pResPlttDicData)[pName]);
    }

    return ResPltt(NULL);
}

ResPltt ResFile::GetResPltt(const ResName name) const {
    void* pResPlttDicData = ResDic(const_cast<ResDicData*>(
        &ref().dict.topLevel))[ResName(&lbl_eu_80568F80)];

    if (pResPlttDicData != NULL) {
        return ResPltt(ResDic(pResPlttDicData)[name]);
    }

    return ResPltt(NULL);
}

ResPltt ResFile::GetResPltt(int idx) const {
    void* pResPlttDicData = ResDic(const_cast<ResDicData*>(
        &ref().dict.topLevel))[ResName(&lbl_eu_80568F80)];

    if (pResPlttDicData != NULL) {
        return ResPltt(ResDic(pResPlttDicData)[idx]);
    }

    return ResPltt(NULL);
}

ResPltt ResFile::GetResPltt(u32 idx) const {
    return GetResPltt(static_cast<int>(idx));
}

u32 ResFile::GetResPlttNumEntries() const {
    void* pResPlttDicData = ResDic(const_cast<ResDicData*>(
        &ref().dict.topLevel))[ResName(&lbl_eu_80568F80)];

    if (pResPlttDicData != NULL) {
        return ResDic(pResPlttDicData).GetNumData();
    }

    return 0;
}

/******************************************************************************
 *
 * ResTex
 *
 ******************************************************************************/
ResTex ResFile::GetResTex(const char* pName) const {
    void* pResTexDicData = ResDic(const_cast<ResDicData*>(
        &ref().dict.topLevel))[ResName(&lbl_eu_80568FA0)];

    if (pResTexDicData != NULL) {
        return ResTex(ResDic(pResTexDicData)[pName]);
    }

    return ResTex(NULL);
}

ResTex ResFile::GetResTex(const ResName name) const {
    void* pResTexDicData = ResDic(const_cast<ResDicData*>(
        &ref().dict.topLevel))[ResName(&lbl_eu_80568FA0)];

    if (pResTexDicData != NULL) {
        return ResTex(ResDic(pResTexDicData)[name]);
    }

    return ResTex(NULL);
}

ResTex ResFile::GetResTex(int idx) const {
    void* pResTexDicData = ResDic(const_cast<ResDicData*>(
        &ref().dict.topLevel))[ResName(&lbl_eu_80568FA0)];

    if (pResTexDicData != NULL) {
        return ResTex(ResDic(pResTexDicData)[idx]);
    }

    return ResTex(NULL);
}

ResTex ResFile::GetResTex(u32 idx) const {
    return GetResTex(static_cast<int>(idx));
}

u32 ResFile::GetResTexNumEntries() const {
    void* pResTexDicData = ResDic(const_cast<ResDicData*>(
        &ref().dict.topLevel))[ResName(&lbl_eu_80568FA0)];

    if (pResTexDicData != NULL) {
        return ResDic(pResTexDicData).GetNumData();
    }

    return 0;
}

/******************************************************************************
 *
 * ResAnmChr
 *
 ******************************************************************************/
ResAnmChr ResFile::GetResAnmChr(int idx) const {
    void* pResAnmChrDicData = ResDic(const_cast<ResDicData*>(
        &ref().dict.topLevel))[ResName(&lbl_eu_80568FC0)];

    if (pResAnmChrDicData != NULL) {
        return ResAnmChr(ResDic(pResAnmChrDicData)[idx]);
    }

    return ResAnmChr(NULL);
}

ResAnmChr ResFile::GetResAnmChr(u32 idx) const {
    return GetResAnmChr(static_cast<int>(idx));
}

u32 ResFile::GetResAnmChrNumEntries() const {
    void* pResAnmChrDicData = ResDic(const_cast<ResDicData*>(
        &ref().dict.topLevel))[ResName(&lbl_eu_80568FC0)];

    if (pResAnmChrDicData != NULL) {
        return ResDic(pResAnmChrDicData).GetNumData();
    }

    return 0;
}

/******************************************************************************
 *
 * ResAnmVis
 *
 ******************************************************************************/
// Retail emits NO GetResAnmVis* symbols: the only callers are the AnmVis
// iteration inside CheckRevision, where -ipa file inlines these bodies. The
// standalone copies are dropped by the postprocess drop_text_symbols rule.
ResAnmVis ResFile::GetResAnmVis(int idx) const {
    void* pResAnmVisDicData = ResDic(const_cast<ResDicData*>(
        &ref().dict.topLevel))[ResName(&lbl_eu_80569000)];

    if (pResAnmVisDicData != NULL) {
        return ResAnmVis(ResDic(pResAnmVisDicData)[idx]);
    }

    return ResAnmVis(NULL);
}

ResAnmVis ResFile::GetResAnmVis(u32 idx) const {
    return GetResAnmVis(static_cast<int>(idx));
}

u32 ResFile::GetResAnmVisNumEntries() const {
    void* pResAnmVisDicData = ResDic(const_cast<ResDicData*>(
        &ref().dict.topLevel))[ResName(&lbl_eu_80569000)];

    if (pResAnmVisDicData != NULL) {
        return ResDic(pResAnmVisDicData).GetNumData();
    }

    return 0;
}

/******************************************************************************
 *
 * ResAnmClr
 *
 ******************************************************************************/
ResAnmClr ResFile::GetResAnmClr(int idx) const {
    void* pResAnmClrDicData = ResDic(const_cast<ResDicData*>(
        &ref().dict.topLevel))[ResName(&lbl_eu_80569020)];

    if (pResAnmClrDicData != NULL) {
        return ResAnmClr(ResDic(pResAnmClrDicData)[idx]);
    }

    return ResAnmClr(NULL);
}

ResAnmClr ResFile::GetResAnmClr(u32 idx) const {
    return GetResAnmClr(static_cast<int>(idx));
}

u32 ResFile::GetResAnmClrNumEntries() const {
    void* pResAnmClrDicData = ResDic(const_cast<ResDicData*>(
        &ref().dict.topLevel))[ResName(&lbl_eu_80569020)];

    if (pResAnmClrDicData != NULL) {
        return ResDic(pResAnmClrDicData).GetNumData();
    }

    return 0;
}

/******************************************************************************
 *
 * ResAnmTexPat
 *
 ******************************************************************************/
ResAnmTexPat ResFile::GetResAnmTexPat(int idx) const {
    void* pResAnmTexPatDicData = ResDic(const_cast<ResDicData*>(
        &ref().dict.topLevel))[ResName(&lbl_eu_80569040)];

    if (pResAnmTexPatDicData != NULL) {
        return ResAnmTexPat(ResDic(pResAnmTexPatDicData)[idx]);
    }

    return ResAnmTexPat(NULL);
}

ResAnmTexPat ResFile::GetResAnmTexPat(u32 idx) const {
    return GetResAnmTexPat(static_cast<int>(idx));
}

u32 ResFile::GetResAnmTexPatNumEntries() const {
    void* pResAnmTexPatDicData = ResDic(const_cast<ResDicData*>(
        &ref().dict.topLevel))[ResName(&lbl_eu_80569040)];

    if (pResAnmTexPatDicData != NULL) {
        return ResDic(pResAnmTexPatDicData).GetNumData();
    }

    return 0;
}

/******************************************************************************
 *
 * ResAnmTexSrt
 *
 ******************************************************************************/
ResAnmTexSrt ResFile::GetResAnmTexSrt(int idx) const {
    void* pResAnmTexSrtDicData = ResDic(const_cast<ResDicData*>(
        &ref().dict.topLevel))[ResName(&lbl_eu_80569060)];

    if (pResAnmTexSrtDicData != NULL) {
        return ResAnmTexSrt(ResDic(pResAnmTexSrtDicData)[idx]);
    }

    return ResAnmTexSrt(NULL);
}

ResAnmTexSrt ResFile::GetResAnmTexSrt(u32 idx) const {
    return GetResAnmTexSrt(static_cast<int>(idx));
}

u32 ResFile::GetResAnmTexSrtNumEntries() const {
    void* pResAnmTexSrtDicData = ResDic(const_cast<ResDicData*>(
        &ref().dict.topLevel))[ResName(&lbl_eu_80569060)];

    if (pResAnmTexSrtDicData != NULL) {
        return ResDic(pResAnmTexSrtDicData).GetNumData();
    }

    return 0;
}

/******************************************************************************
 *
 * ResAnmShp
 *
 ******************************************************************************/
// Retail emits NO GetResAnmShp* symbols: the only callers are the AnmShp
// iteration inside CheckRevision, where -ipa file inlines these bodies. The
// standalone copies are dropped by the postprocess drop_text_symbols rule.
ResAnmShp ResFile::GetResAnmShp(int idx) const {
    void* pResAnmShpDicData = ResDic(const_cast<ResDicData*>(
        &ref().dict.topLevel))[ResName(&lbl_eu_80569080)];

    if (pResAnmShpDicData != NULL) {
        return ResAnmShp(ResDic(pResAnmShpDicData)[idx]);
    }

    return ResAnmShp(NULL);
}

ResAnmShp ResFile::GetResAnmShp(u32 idx) const {
    return GetResAnmShp(static_cast<int>(idx));
}

u32 ResFile::GetResAnmShpNumEntries() const {
    void* pResAnmShpDicData = ResDic(const_cast<ResDicData*>(
        &ref().dict.topLevel))[ResName(&lbl_eu_80569080)];

    if (pResAnmShpDicData != NULL) {
        return ResDic(pResAnmShpDicData).GetNumData();
    }

    return 0;
}

/******************************************************************************
 *
 * ResAnmScn
 *
 ******************************************************************************/
ResAnmScn ResFile::GetResAnmScn(int idx) const {
    void* pResAnmScnDicData = ResDic(const_cast<ResDicData*>(
        &ref().dict.topLevel))[ResName(&lbl_eu_805690A0)];

    if (pResAnmScnDicData != NULL) {
        return ResAnmScn(ResDic(pResAnmScnDicData)[idx]);
    }

    return ResAnmScn(NULL);
}

ResAnmScn ResFile::GetResAnmScn(u32 idx) const {
    return GetResAnmScn(static_cast<int>(idx));
}

u32 ResFile::GetResAnmScnNumEntries() const {
    void* pResAnmScnDicData = ResDic(const_cast<ResDicData*>(
        &ref().dict.topLevel))[ResName(&lbl_eu_805690A0)];

    if (pResAnmScnDicData != NULL) {
        return ResDic(pResAnmScnDicData).GetNumData();
    }

    return 0;
}

/******************************************************************************
 *
 * Miscellaneous
 *
 ******************************************************************************/

bool ResFile::HasResTex() const {
    return ResDic(const_cast<ResDicData*>(
        &ref().dict.topLevel))[ResName(&lbl_eu_80568FA0)] != NULL;
}

bool ResFile::HasResAnmChr() const {
    return ResDic(const_cast<ResDicData*>(
        &ref().dict.topLevel))[ResName(&lbl_eu_80568FC0)] != NULL;
}

bool ResFile::HasResAnmClr() const {
    return ResDic(const_cast<ResDicData*>(
        &ref().dict.topLevel))[ResName(&lbl_eu_80569020)] != NULL;
}

bool ResFile::HasResAnmTexPat() const {
    return ResDic(const_cast<ResDicData*>(
        &ref().dict.topLevel))[ResName(&lbl_eu_80569040)] != NULL;
}

bool ResFile::HasResAnmTexSrt() const {
    return ResDic(const_cast<ResDicData*>(
        &ref().dict.topLevel))[ResName(&lbl_eu_80569060)] != NULL;
}

bool ResFile::HasResAnmShp() const {
    return ResDic(const_cast<ResDicData*>(
        &ref().dict.topLevel))[ResName(&lbl_eu_80569080)] != NULL;
}

bool ResFile::HasResAnmScn() const {
    return ResDic(const_cast<ResDicData*>(
        &ref().dict.topLevel))[ResName(&lbl_eu_805690A0)] != NULL;
}

bool ResFile::Bind(const ResFile file) {
    u32 i;
    bool success = true;

    u32 mdlNum = GetResMdlNumEntries();
    for (i = 0; i < mdlNum; i++) {
        success = GetResMdl(i).Bind(file) && success;
    }

    u32 anmTexPatNum = GetResAnmTexPatNumEntries();
    for (i = 0; i < anmTexPatNum; i++) {
        // @bug Success value clobbered by most recent result
        success = GetResAnmTexPat(i).Bind(file);
    }

    return success;
}

void ResFile::Init() {
    u32 i;

    u32 mdlNum = GetResMdlNumEntries();
    for (i = 0; i < mdlNum; i++) {
        GetResMdl(i).Init();
    }

    u32 texNum = GetResTexNumEntries();
    for (i = 0; i < texNum; i++) {
        GetResTex(i).Init();
    }

    u32 plttNum = GetResPlttNumEntries();
    for (i = 0; i < plttNum; i++) {
        GetResPltt(i).Init();
    }
}

void ResFile::Terminate() {
    u32 i;

    u32 mdlNum = GetResMdlNumEntries();
    for (i = 0; i < mdlNum; i++) {
        GetResMdl(i).Terminate();
    }
}

bool ResFile::CheckRevision() const {
    u32 i;

    u32 mdlNum = GetResMdlNumEntries();
    for (i = 0; i < mdlNum; i++) {
        if (!GetResMdl(i).CheckRevision()) {
            return false;
        }
    }

    u32 texNum = GetResTexNumEntries();
    for (i = 0; i < texNum; i++) {
        if (!GetResTex(i).CheckRevision()) {
            return false;
        }
    }

    u32 plttNum = GetResPlttNumEntries();
    for (i = 0; i < plttNum; i++) {
        if (!GetResPltt(i).CheckRevision()) {
            return false;
        }
    }

    u32 anmChrNum = GetResAnmChrNumEntries();
    for (i = 0; i < anmChrNum; i++) {
        if (!GetResAnmChr(i).CheckRevision()) {
            return false;
        }
    }

    u32 anmVisNum = GetResAnmVisNumEntries();
    for (i = 0; i < anmVisNum; i++) {
        if (!GetResAnmVis(i).CheckRevision()) {
            return false;
        }
    }

    u32 anmClrNum = GetResAnmClrNumEntries();
    for (i = 0; i < anmClrNum; i++) {
        if (!GetResAnmClr(i).CheckRevision()) {
            return false;
        }
    }

    u32 anmTexPatNum = GetResAnmTexPatNumEntries();
    for (i = 0; i < anmTexPatNum; i++) {
        if (!GetResAnmTexPat(i).CheckRevision()) {
            return false;
        }
    }

    u32 anmTexSrtNum = GetResAnmTexSrtNumEntries();
    for (i = 0; i < anmTexSrtNum; i++) {
        if (!GetResAnmTexSrt(i).CheckRevision()) {
            return false;
        }
    }

    u32 anmShpNum = GetResAnmShpNumEntries();
    for (i = 0; i < anmShpNum; i++) {
        if (!GetResAnmShp(i).CheckRevision()) {
            return false;
        }
    }

    u32 anmScnNum = GetResAnmScnNumEntries();
    for (i = 0; i < anmScnNum; i++) {
        if (!GetResAnmScn(i).CheckRevision()) {
            return false;
        }
    }

    return true;
}

} // namespace g3d
} // namespace nw4r

extern "C" void KeepResFileExternal() { (void)lbl_eu_805690C0; }
