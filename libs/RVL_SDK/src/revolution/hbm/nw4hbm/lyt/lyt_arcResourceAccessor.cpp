#include <nw4hbm/lyt.h>

#include <revolution/ARC.h>

#include <cstring>


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

namespace nw4hbm {
namespace lyt {

/******************************************************************************
 *
 * ArcResourceAccessor
 *
 ******************************************************************************/
ArcResourceAccessor::ArcResourceAccessor()
    : ResourceAccessor(), mArcBuf(NULL), mFontList() {}
// Residual (open-item): schedule, store-order, and size are byte-identical to
// retail (84B/84B); the only diff is a pure 3-temp Chaitin colour cycle -
// retail allocates [vt=r4, fml=&mNode=r5, zero=r0], decomp allocates
// [zero=r4, vt=r5, fml=r0] (the retail compiler keeps r0 for the literal zero;
// ours routes &mNode through r0 and the zero through r4 - a global allocator
// preference). All three temps are independently hoisted up-front
// (lis vt@ha / addi r31,0x28 / li 0) so they form an equal-lifetime
// interference clique; MWCC's tiebreak priority for assigning {r0,r4,r5} is
// flags/IR-internal. Verified invariant to every .cpp lever: member-init-list
// permutations, body-vs-list, implicit base/mFontList, placement-new in body
// (broke structure, OVER budget - ruled out), and the literal upstream
// nw4r source `: mArcBuf(NULL) {}` all yield the same rotation. Members are
// header-locked (offsets feed the 5 already-matched fns) and the TU .text
// budget is exact (0x3A0), so no register-pressure or size lever exists in the
// writable scope. Needs a unit-flag/-O4,s change or header-level reshuffle.
// Residual: schedule/store-order/size byte-identical; only a pure 3-cycle
// Chaitin color rotation differs (vt/r0-zero/this+0x28). Member-list,
// init-order, and body-vs-list perturbations all leave it fixed (empty body;
// members fixed by the locked header).

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
