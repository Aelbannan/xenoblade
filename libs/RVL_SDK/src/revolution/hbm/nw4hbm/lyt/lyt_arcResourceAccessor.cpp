#include <nw4hbm/lyt.h>

#include <revolution/ARC.h>

#include <cstring>


/******************************************************************************
 *
 * Utility functions
 *
 ******************************************************************************/
namespace {

// Recursively searches the archive rooted at pHandle's current directory for
// a file whose name matches pName (case-insensitive). Returns the entry
// number, or -1 if not found.
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

// Resolves a resource inside the archive. When type is RES_TYPE_NAME (0),
// the whole tree is searched by file name; otherwise pRootDir/<type>/pName
// is opened directly. Stores the resource length in *pSize when non-NULL.
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

namespace nw4hbm {
namespace lyt {

/******************************************************************************
 *
 * ArcResourceAccessor
 *
 ******************************************************************************/
// NOTE (us-8032ebc0, known residual): this ctor shows 9 pure reg-swaps vs
// retail: a fixed Chaitin coloring rotation on {vtable ptr, &mFontList.mNode,
// NULL} during mFontList construction. All source-shape levers were ruled out
// (init-list order, placement/value-form of mArcBuf, base-call form - all
// byte-identical); the remaining levers (member decl order in the header,
// ut_LinkList.h Initialize_ statement order) would change field offsets or
// perturb every LinkList TU. Scheduler-driven regalloc soft-cap; cf. walls #11.
ArcResourceAccessor::ArcResourceAccessor()
    : ResourceAccessor(), mArcBuf(NULL), mFontList() {}

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

void* ArcResourceAccessor::GetResource(u32 type, const char* pName,
                                       u32* pSize) {

    return GetResourceSub(&mArcHandle, mResRootDir, type, pName, pSize);
}

ut::Font* ArcResourceAccessor::GetFont(const char* pName) {
    NW4R_UT_LINKLIST_FOREACH (it, mFontList, {
        if (std::strcmp(pName, it->GetFontName()) == 0) {
            return it->GetFont();
        }
    })

    return NULL;
}

} // namespace lyt
} // namespace nw4hbm
