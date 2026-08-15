#include <nw4r/g3d.h>

namespace nw4r {
namespace g3d {
namespace {

// The ResName pascal-string data for the file resource groups. Retail keeps
// the bytes in the shared nw4r_data.s slice (this TU's retail split carries no
// .data); the definitions stay HERE so MWCC can fold adjacent references into
// base+offset immediates (CheckRevision's ten iterations are `addi rX,base,ofs`
// on a single base), and the postprocess extern_data_sections rule strips the
// emitted .data/UNDEFs the symbols for the link (CfPadTask.o pattern).
NW4R_G3D_RESFILE_NAME_DEF(Models, "3DModels(NW4R)");
NW4R_G3D_RESFILE_NAME_DEF(Pltts, "Palettes(NW4R)");
NW4R_G3D_RESFILE_NAME_DEF(Textures, "Textures(NW4R)");
NW4R_G3D_RESFILE_NAME_DEF(AnmChr, "AnmChr(NW4R)");
NW4R_G3D_RESFILE_NAME_DEF(AnmVis, "AnmVis(NW4R)");
NW4R_G3D_RESFILE_NAME_DEF(AnmClr, "AnmClr(NW4R)");
NW4R_G3D_RESFILE_NAME_DEF(AnmTexPat, "AnmTexPat(NW4R)");
NW4R_G3D_RESFILE_NAME_DEF(AnmTexSrt, "AnmTexSrt(NW4R)");
NW4R_G3D_RESFILE_NAME_DEF(AnmShp, "AnmShp(NW4R)");
NW4R_G3D_RESFILE_NAME_DEF(AnmScn, "AnmScn(NW4R)");
NW4R_G3D_RESFILE_NAME_DEF(Ext, "External");

} // namespace

/******************************************************************************
 *
 * ResMdl
 *
 ******************************************************************************/
ResMdl ResFile::GetResMdl(int idx) const {
    void* pResMdlDicData = ResDic(const_cast<ResDicData*>(
        &ref().dict.topLevel))[ResName(&ResNameData_Models)];

    if (pResMdlDicData != NULL) {
        return ResMdl(ResDic(pResMdlDicData)[idx]);
    }

    return ResMdl(NULL);
}

u32 ResFile::GetResMdlNumEntries() const {
    void* pResMdlDicData = ResDic(const_cast<ResDicData*>(
        &ref().dict.topLevel))[ResName(&ResNameData_Models)];

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
        &ref().dict.topLevel))[ResName(&ResNameData_Pltts)];

    if (pResPlttDicData != NULL) {
        return ResPltt(ResDic(pResPlttDicData)[pName]);
    }

    return ResPltt(NULL);
}

ResPltt ResFile::GetResPltt(const ResName name) const {
    void* pResPlttDicData = ResDic(const_cast<ResDicData*>(
        &ref().dict.topLevel))[ResName(&ResNameData_Pltts)];

    if (pResPlttDicData != NULL) {
        return ResPltt(ResDic(pResPlttDicData)[name]);
    }

    return ResPltt(NULL);
}

ResPltt ResFile::GetResPltt(int idx) const {
    void* pResPlttDicData = ResDic(const_cast<ResDicData*>(
        &ref().dict.topLevel))[ResName(&ResNameData_Pltts)];

    if (pResPlttDicData != NULL) {
        return ResPltt(ResDic(pResPlttDicData)[idx]);
    }

    return ResPltt(NULL);
}

/******************************************************************************
 *
 * ResTex
 *
 ******************************************************************************/
ResTex ResFile::GetResTex(const char* pName) const {
    void* pResTexDicData = ResDic(const_cast<ResDicData*>(
        &ref().dict.topLevel))[ResName(&ResNameData_Textures)];

    if (pResTexDicData != NULL) {
        return ResTex(ResDic(pResTexDicData)[pName]);
    }

    return ResTex(NULL);
}

ResTex ResFile::GetResTex(const ResName name) const {
    void* pResTexDicData = ResDic(const_cast<ResDicData*>(
        &ref().dict.topLevel))[ResName(&ResNameData_Textures)];

    if (pResTexDicData != NULL) {
        return ResTex(ResDic(pResTexDicData)[name]);
    }

    return ResTex(NULL);
}

ResTex ResFile::GetResTex(int idx) const {
    void* pResTexDicData = ResDic(const_cast<ResDicData*>(
        &ref().dict.topLevel))[ResName(&ResNameData_Textures)];

    if (pResTexDicData != NULL) {
        return ResTex(ResDic(pResTexDicData)[idx]);
    }

    return ResTex(NULL);
}

/******************************************************************************
 *
 * ResAnmChr
 *
 ******************************************************************************/
ResAnmChr ResFile::GetResAnmChr(int idx) const {
    void* pResAnmChrDicData = ResDic(const_cast<ResDicData*>(
        &ref().dict.topLevel))[ResName(&ResNameData_AnmChr)];

    if (pResAnmChrDicData != NULL) {
        return ResAnmChr(ResDic(pResAnmChrDicData)[idx]);
    }

    return ResAnmChr(NULL);
}

u32 ResFile::GetResAnmChrNumEntries() const {
    void* pResAnmChrDicData = ResDic(const_cast<ResDicData*>(
        &ref().dict.topLevel))[ResName(&ResNameData_AnmChr)];

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
        &ref().dict.topLevel))[ResName(&ResNameData_AnmVis)];

    if (pResAnmVisDicData != NULL) {
        return ResAnmVis(ResDic(pResAnmVisDicData)[idx]);
    }

    return ResAnmVis(NULL);
}

u32 ResFile::GetResAnmVisNumEntries() const {
    void* pResAnmVisDicData = ResDic(const_cast<ResDicData*>(
        &ref().dict.topLevel))[ResName(&ResNameData_AnmVis)];

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
        &ref().dict.topLevel))[ResName(&ResNameData_AnmClr)];

    if (pResAnmClrDicData != NULL) {
        return ResAnmClr(ResDic(pResAnmClrDicData)[idx]);
    }

    return ResAnmClr(NULL);
}

u32 ResFile::GetResAnmClrNumEntries() const {
    void* pResAnmClrDicData = ResDic(const_cast<ResDicData*>(
        &ref().dict.topLevel))[ResName(&ResNameData_AnmClr)];

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
        &ref().dict.topLevel))[ResName(&ResNameData_AnmTexPat)];

    if (pResAnmTexPatDicData != NULL) {
        return ResAnmTexPat(ResDic(pResAnmTexPatDicData)[idx]);
    }

    return ResAnmTexPat(NULL);
}

u32 ResFile::GetResAnmTexPatNumEntries() const {
    void* pResAnmTexPatDicData = ResDic(const_cast<ResDicData*>(
        &ref().dict.topLevel))[ResName(&ResNameData_AnmTexPat)];

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
        &ref().dict.topLevel))[ResName(&ResNameData_AnmTexSrt)];

    if (pResAnmTexSrtDicData != NULL) {
        return ResAnmTexSrt(ResDic(pResAnmTexSrtDicData)[idx]);
    }

    return ResAnmTexSrt(NULL);
}

u32 ResFile::GetResAnmTexSrtNumEntries() const {
    void* pResAnmTexSrtDicData = ResDic(const_cast<ResDicData*>(
        &ref().dict.topLevel))[ResName(&ResNameData_AnmTexSrt)];

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
        &ref().dict.topLevel))[ResName(&ResNameData_AnmShp)];

    if (pResAnmShpDicData != NULL) {
        return ResAnmShp(ResDic(pResAnmShpDicData)[idx]);
    }

    return ResAnmShp(NULL);
}

u32 ResFile::GetResAnmShpNumEntries() const {
    void* pResAnmShpDicData = ResDic(const_cast<ResDicData*>(
        &ref().dict.topLevel))[ResName(&ResNameData_AnmShp)];

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
        &ref().dict.topLevel))[ResName(&ResNameData_AnmScn)];

    if (pResAnmScnDicData != NULL) {
        return ResAnmScn(ResDic(pResAnmScnDicData)[idx]);
    }

    return ResAnmScn(NULL);
}

u32 ResFile::GetResAnmScnNumEntries() const {
    void* pResAnmScnDicData = ResDic(const_cast<ResDicData*>(
        &ref().dict.topLevel))[ResName(&ResNameData_AnmScn)];

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
        &ref().dict.topLevel))[ResName(&ResNameData_Textures)] != NULL;
}

bool ResFile::HasResAnmChr() const {
    return ResDic(const_cast<ResDicData*>(
        &ref().dict.topLevel))[ResName(&ResNameData_AnmChr)] != NULL;
}

bool ResFile::HasResAnmClr() const {
    return ResDic(const_cast<ResDicData*>(
        &ref().dict.topLevel))[ResName(&ResNameData_AnmClr)] != NULL;
}

bool ResFile::HasResAnmTexPat() const {
    return ResDic(const_cast<ResDicData*>(
        &ref().dict.topLevel))[ResName(&ResNameData_AnmTexPat)] != NULL;
}

bool ResFile::HasResAnmTexSrt() const {
    return ResDic(const_cast<ResDicData*>(
        &ref().dict.topLevel))[ResName(&ResNameData_AnmTexSrt)] != NULL;
}

bool ResFile::HasResAnmShp() const {
    return ResDic(const_cast<ResDicData*>(
        &ref().dict.topLevel))[ResName(&ResNameData_AnmShp)] != NULL;
}

bool ResFile::HasResAnmScn() const {
    return ResDic(const_cast<ResDicData*>(
        &ref().dict.topLevel))[ResName(&ResNameData_AnmScn)] != NULL;
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
