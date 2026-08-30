#include <nw4r/lyt.h>

#include <revolution/ARC.h>

#include <cstring>

// Retail vtable lbl_eu_80569CB8 (0x18) now owned by this TU (absorbed).
extern "C" void __dt__Q34nw4r3lyt19ArcResourceAccessorFv();
extern "C" void* GetResource__Q34nw4r3lyt19ArcResourceAccessorFUlPCcPUl();
extern "C" void* GetFont__Q34nw4r3lyt19ArcResourceAccessorFPCc();
extern "C" __declspec(section ".data") __attribute__((aligned(8))) const void* lbl_eu_80569CB8[6] = {
    nullptr, nullptr, (const void*)&__dt__Q34nw4r3lyt19ArcResourceAccessorFv,
    (const void*)&GetResource__Q34nw4r3lyt19ArcResourceAccessorFUlPCcPUl,
    (const void*)&GetFont__Q34nw4r3lyt19ArcResourceAccessorFPCc, nullptr
};
// .sdata
// code below and renamed to lbl_eu_806634C8/CC via data_pool_patterns; the
// section is kept (extern removed) so the retail 8B layout is preserved.

namespace {
// Retail schedules the derived vptr store inside the member-init phase
// (standard ABI order: base call -> vptr -> member ctors), not in the ctor
// body. Attaching it to the mArcBuf initializer via a comma expression puts
// it in that phase; the scheduler then weaves it between the FontRefLinkList
// member stores exactly as retail does.
inline void InitVptr(void* obj) { *(void**)obj = (void*)lbl_eu_80569CB8; }
} // namespace

/******************************************************************************
 *
 * Utility functions
 *
 ******************************************************************************/
namespace {

s32 FindNameResource(ARCHandle* pHandle, const char* pName) {
    s32 entrynum = -1;

    ARCDir dir;
    ARCOpenDir(pHandle, ".", &dir);

    ARCDirEntry ent;
    while (ARCReadDir(&dir, &ent)) {
        if (ent.isDir) {
            ARCChangeDir(pHandle, ent.name);
            entrynum = FindNameResource(pHandle, pName);
            ARCChangeDir(pHandle, "..");

            if (entrynum != -1) {
                break;
            }
        } else {
            if (std::stricmp(pName, ent.name) == 0) {
                entrynum = ent.entryNum;
                break;
            }
        }
    }

    ARCCloseDir(&dir);
    return entrynum;
}

void* GetResourceSub(ARCHandle* pHandle, const char* pRootDir, u32 type,
                     const char* pName, unsigned long* pSize) {

    s32 entrynum = -1;

    if (ARCConvertPathToEntrynum(pHandle, pRootDir) != -1) {
        if (ARCChangeDir(pHandle, pRootDir)) {
            if (type == 0) {
                entrynum = FindNameResource(pHandle, pName);
            } else {
                char resDir[4 + 1];
                resDir[0] = type >> 24;
                resDir[1] = type >> 16;
                resDir[2] = type >> 8;
                resDir[3] = type >> 0;
                resDir[4] = '\0';

                if (ARCConvertPathToEntrynum(pHandle, resDir) != -1 &&
                    ARCChangeDir(pHandle, resDir)) {

                    entrynum = ARCConvertPathToEntrynum(pHandle, pName);
                    ARCChangeDir(pHandle, "..");
                }
            }

            ARCChangeDir(pHandle, "..");
        }
    }

    if (entrynum != -1) {
        ARCFileInfo info;
        ARCFastOpen(pHandle, entrynum, &info);

        void* pResource = ARCGetStartAddrInMem(&info);

        if (pSize != NULL) {
            *pSize = ARCGetLength(&info);
        }

        ARCClose(&info);
        return pResource;
    }

    return NULL;
}

} // namespace

namespace nw4r {
namespace lyt {

/******************************************************************************
 *
 * Implementation details
 *
 ******************************************************************************/
namespace detail {

ut::Font* FindFont(FontRefLinkList* pFontList, const char* pName) {
    NW4R_UT_LINKLIST_FOREACH (it, *pFontList, {
        if (std::strcmp(pName, it->GetFontName()) == 0) {
            return it->GetFont();
        }
    })

    return NULL;
}

} // namespace detail

/******************************************************************************
 *
 * ArcResourceAccessor
 *
 ******************************************************************************/
ArcResourceAccessor::ArcResourceAccessor() : mArcBuf((InitVptr(this), (void*)NULL)) {}

bool ArcResourceAccessor::Attach(void* pArchive, const char* pRootDir) {
    BOOL success = ARCInitHandle(pArchive, &mArcHandle);
    if (!success) {
        return false;
    }

    mArcBuf = pArchive;

    std::strncpy(mResRootDir, pRootDir, ROOTPATH_MAX - 1);
    mResRootDir[ROOTPATH_MAX - 1] = '\0';

    return true;
}

void* ArcResourceAccessor::Detach() {
    void *old = mArcBuf;
    mArcBuf = NULL;
        
    return old;
}

void* ArcResourceAccessor::GetResource(u32 type, const char* pName,
                                       u32* pSize) {

    return GetResourceSub(&mArcHandle, mResRootDir, type, pName, pSize);
}

ut::Font* ArcResourceAccessor::GetFont(const char* pName) {
    return detail::FindFont(&mFontList, pName);
}

} // namespace lyt
} // namespace nw4r
