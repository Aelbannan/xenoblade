#include <string.h>
#include <decomp.h>
// Inline-empty base dtor: retail's derived dtor does not emit a
// __dt__10IWorkEventFv call (see IWorkEvent.hpp note).
#define IWORK_EVENT_INLINE_DTOR 1
#include <monolib/util/FixStr.hpp>
#include <monolib/util/MemManager.hpp>
#include <monolib/work/IWorkEvent.hpp>
#include <monolib/nand/CNBanner.hpp>
#include <monolib/work/CEventFile.hpp>
#include <monolib/device/CDeviceFile.hpp>
#include <monolib/device/CDeviceFileCri.hpp>
#include <revolution/NAND.h>
#include <revolution/ENC.h>

// Address-named retail imports (no C++ declarations exist for these yet).
extern "C" {
// Releases a registered file path from the CDeviceFile manager
// (CDeviceFile::func_8044F0E4; header edit out of scope for this session).
void cancelJobsForPath__11CDeviceFileFPCc(const char* path);
// Locale/count selector latch (defined in CDeviceFile.cpp).
void DevFile_SetByteA9(u8 val);
}

// Mirror of the NANDBanner block layout as consumed by CNBanner::load. The
// SDK header declares iconTexture[0x1200][8]; the actual per-icon stride is
// 0x1200 (8 icons fill the 0xF0A0 block), so this TU uses its own struct.
struct CNandBannerBlock {
    u32 magic;                  // 0x000
    u32 flags;                  // 0x004
    u16 iconSpeed;              // 0x008
    u8 padA[0x20 - 0xA];        // 0x00A
    wchar_t title[32];          // 0x020
    wchar_t subtitle[32];       // 0x060
    u8 bannerTexture[0x6000];   // 0x0A0
    u8 iconTexture[8][0x1200];  // 0x60A0
};


// --- constructor ---------------------------------------------------------

// us-804f96c0: initialise the banner manager. The compiler stores the vtable,
// default-constructs the FixStr members (inline stb/stw clears plus a counted
// loop over slots 1..7), then this body zeroes the id array and resets the
// count/flag fields (mKind armed to 1).
CNBanner::CNBanner() {
    this->mData = 0;
    this->mSize = 0;
    this->mFlags = 0;
    memset(this->mFileId, 0, sizeof(this->mFileId));
    this->mCount = 0;
    this->mCountRef = 0;
    this->mBusy = 0;
    this->mFlag = 0;
    this->mKind = 1;
}

// --- destructor ----------------------------------------------------------

CNBanner::~CNBanner() {
    unload();
}

void CNBanner::setTitle(const char* str) { mTitle.format(str); }

void CNBanner::setSubtitle(const char* str) { mDesc.format(str); }

void CNBanner::setBannerPath(const char* str) { mPath.format(str); }

// --- setFileList -------------------------------------------------------

// us-804f9894: (re)build the banner slot list from a NULL-terminated array of
// path strings. Copies each name into a FixStr slot, records its id, then
// fills any remaining slots up to 8 with the empty format string.
void CNBanner::setFileList(const char** names, int* ids) {
    int i = 0;
    while (names[i] != 0) {
        this->mFiles[i].format(names[i]);
        this->mFileId[i] = ids[i];
        i++;
    }
    this->mCount = i;
    while (i < 8) {
        this->mFiles[i].format("");
        this->mFileId[i] = 0;
        i++;
    }
}

// --- load ------------------------------------------------------------------

// us-804f9954: CNBanner::load
// Builds and starts loading the NAND banner block. Allocates a NANDBanner
// (0xF0A0) from MEM2, renders the UTF-8 title/subtitle fields to UTF-16 locals,
// zeroes the block and initialises its header with NANDInitBanner, packs the
// per-slot file ids into the u16 icon-speed bitset (2 bits per slot), then
// issues async archive reads of the banner texture and each icon's texture
// through CDeviceFile. Returns 1 on success, or 0 if the allocation or either
// encoding conversion failed.
bool CNBanner::load() {
    this->mFlag = 0;

    // Allocate the NAND banner data block from the MEM2 heap.
    void* alloc = mtl::MemManager::allocate_ex(
        0xF0A0, mtl::MemManager::getHandleMEM2(), 0x20);
    this->mData = alloc;
    if (alloc == 0) {
        return 0;
    }

    // UTF-16 render buffers and their length words. The string pointers are
    // kept in one reused local so MWCC holds them in a single callee-saved
    // register (retail reuses r29 for title/desc/files); the success flags
    // are separate booleans tested after each conversion (retail r4/r5).
    u16 title[0x20];
    u16 subtitle[0x20];
    u32 titleLen;
    u32 titleSrcLen;
    u32 subtitleLen;
    u32 subtitleSrcLen;
    const char* str;

    str = this->mTitle.c_str();
    titleLen = 0x20;
    titleSrcLen = strlen(str) * 2;
    s32 titleOk;
    if (ENCConvertStringUtf8ToUtf16(title, &titleLen,
            reinterpret_cast<const u8*>(str), &titleSrcLen) != 0) {
        titleOk = 0;
    } else {
        title[titleLen] = 0;
        titleOk = 1;
    }
    if (titleOk == 0) {
        return 0;
    }

    str = this->mDesc.c_str();
    subtitleLen = 0x20;
    subtitleSrcLen = strlen(str) * 2;
    s32 subtitleOk;
    if (ENCConvertStringUtf8ToUtf16(subtitle, &subtitleLen,
            reinterpret_cast<const u8*>(str), &subtitleSrcLen) != 0) {
        subtitleOk = 0;
    } else {
        subtitle[subtitleLen] = 0;
        subtitleOk = 1;
    }
    if (subtitleOk == 0) {
        return 0;
    }

    memset((CNandBannerBlock*)this->mData, 0, 0xF0A0);
    NANDInitBanner((NANDBanner*)this->mData, this->mFlags,
                   reinterpret_cast<const wchar_t*>(title),
                   reinterpret_cast<const wchar_t*>(subtitle));

    // Pack each live slot's file id into the u16 icon-speed bitset (2 bits per
    // slot); the second loop clears the remaining idle slots.
    int i = 0;
    for (; i < this->mCount; i++) {
        u16 v = ((CNandBannerBlock*)this->mData)->iconSpeed;
        ((CNandBannerBlock*)this->mData)->iconSpeed =
            (u16)((v & ~(3 << (2 * i))) | (this->mFileId[i] << (2 * i)));
    }
    for (; i < 8; i++) {
        u16 v = ((CNandBannerBlock*)this->mData)->iconSpeed;
        ((CNandBannerBlock*)this->mData)->iconSpeed =
            (u16)(v & ~(3 << (2 * i)));
    }

    // Register the primary banner texture for async load, then each icon.
    this->mBusy = 1;
    this->mCountRef = 0;
    CDeviceFile::readCommonArchiveFile(
        (mtl::ALLOC_HANDLE)((CNandBannerBlock*)this->mData)->bannerTexture,
        this->mPath.c_str(), this, 0x40, 0x6000);
    this->mCountRef++;

    for (int j = 0; j < this->mCount; j++) {
        CDeviceFile::readCommonArchiveFile(
            (mtl::ALLOC_HANDLE)((CNandBannerBlock*)this->mData)->iconTexture[j],
            this->mFiles[j].c_str(), this, 0x40, 0x1200);
        this->mCountRef++;
    }

    this->mKind = (u8)DevFile_GetByteA9();
    DevFile_SetByteA9(1);
    this->mSize = this->mCount * 0x1200 + 0x60A0;
    return 1;
}

// --- task-block destructor ------------------------------------------------

// us-804f9cc4: destroys the banner data block (the object held at +0x000 of a
// CNReqtaskSaveBannerData). Same teardown as the member destructor but without
// the object-delete scaffold: it always frees the dynamic buffer alone.
void CNBanner::unload() {
    if (this->mBusy != 0) {
        cancelJobsForPath__11CDeviceFileFPCc(this->mPath.c_str());
        for (int i = 0; i < this->mCount; i++) {
            if ((s8)this->mFiles[i].mString[0] != 0) {
                cancelJobsForPath__11CDeviceFileFPCc(this->mFiles[i].c_str());
            }
        }
        this->mBusy = 0;
        DevFile_SetByteA9(this->mKind);
    }
    if (this->mData != 0) {
        ::operator delete(this->mData);
        this->mData = 0;
    }
}

// --- OnFileEvent ----------------------------------------------------------

// us-804f9d7c: match a completed file-load event against the primary path and
// then the per-slot paths. On a match, decrement the pending counter; the last
// one out (counter reaches 0) re-arms the count latch. Returns whether the
// event belonged to this banner.
bool CNBanner::OnFileEvent(CEventFile* event) {
    const char* name = event->mName;
    u32 type = (u32)event->unk0;
    u8 notPrimary = (type != 1);

    if (strcmp(name, this->mPath.c_str()) == 0) {
        s32 c = this->mCountRef - 1;
        this->mCountRef = c;
        s32 busy = (c > 0);
        this->mBusy = (u8)busy;
        this->mFlag = (u8)((this->mFlag | notPrimary) != 0);
        if (busy == 0) {
            DevFile_SetByteA9(this->mKind);
        }
        return true;
    }

    for (int i = 0; i < this->mCount; i++) {
        if (strcmp(name, this->mFiles[i].c_str()) == 0) {
            s32 c = this->mCountRef - 1;
            this->mCountRef = c;
            s32 busy = (c > 0);
            this->mBusy = (u8)busy;
            this->mFlag = (u8)((this->mFlag | notPrimary) != 0);
            if (busy == 0) {
                DevFile_SetByteA9(this->mKind);
            }
            return true;
        }
    }
    return false;
}
