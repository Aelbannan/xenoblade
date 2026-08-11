// Decompiled: monolib/src/nand/CNBanner
//
// CNBanner manages the per-save banner filename list. Layout:
//   +0x000 void*  mVtable   -- data vtable ptr (lbl_eu_80570378)
//   +0x004 void*  mAlloc0   -- dynamically allocated NANDBanner (0xF0A0, operator delete'd)
//   +0x008 u32    field_8
//   +0x00C u32    field_C
//   +0x010 FixStr<64> mTitle -- UTF-8 title string (mLength at 0x050)
//   +0x054 FixStr<64> mDesc  -- UTF-8 subtitle string (mLength at 0x094)
//   +0x098 FixStr<64> mPath      -- primary banner path string
//   +0x0DC FixStr<64> mFiles[8]  -- per-slot banner path strings
//   +0x2FC u32    mFileId[8]     -- per-slot file ids (0..mCount)
//   +0x31C u32    mCount         -- number of slots in use
//   +0x320 u32    mCountRef      -- pending-load reference counter
//   +0x324 u8     mBusy          -- files still registered with CDeviceFile
//   +0x325 u8     mFlag          -- "needs rebuild" / dirty flag
//   +0x326 u8     mKind          -- selector forwarded to func_eu_804521BC
//
// The retail symbol names (detached / non-standard) are kept as extern "C"
// stand-alone functions, matching the monolib NAND catalog-TU pattern
// (see CNand.cpp / CNReqtaskSaveBanner.cpp).

#include <types.h>
#include <string.h>
#include <monolib/util/FixStr.hpp>
#include <monolib/util/MemManager.hpp>
#include <monolib/work/IWorkEvent.hpp>
#include <monolib/device/CDeviceFile.hpp>
#include <monolib/device/CDeviceFileCri.hpp>
#include <revolution/NAND.h>
#include <revolution/ENC.h>

// Runtime / external symbols (retail linker names).
extern "C" {
void __dl__FPv(void*);

// Releases a registered file path from the CDeviceFile manager.
void func_8044F0E4__11CDeviceFileFPCc(const char* path);

// Locale/count selector latch.
void func_eu_804521BC(u8 val);
}

// Data objects referenced by this unit (global scope -> no C++ mangling).
extern u8 lbl_eu_80570378[];     // .data: vtable blob (installed at +0x000)
extern const char lbl_eu_805248BC[];  // .rodata: path format string ("" for empty slots)

// CEventFile as consumed by CNBanner::OnFileEvent: an event type word at +0x0
// and a file-name/data string pointer at +0xC.
struct CEventFile {
    int field_0;        // 0x0  event type
    u8 pad4[8];         // 0x4
    const char* field_C;  // 0xC  file path / name string
};

// Mirror of the NANDBanner block layout as consumed by func_804F53DC. The
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

class CNBanner {
public:
    CNBanner();
    ~CNBanner();
    void func_804F531C(const char** names, int* ids);
    bool OnFileEvent(CEventFile* event);
    void func_804F52F8(const char* str);
    void func_804F5304(const char* str);
    void func_804F5310(const char* str);

    void* mVtable;             // 0x000
    void* mAlloc0;             // 0x004
    u32 field_8;               // 0x008
    u32 field_C;               // 0x00C
    ml::FixStr<64> mTitle;     // 0x010 (title string, mLength at 0x050)
    ml::FixStr<64> mDesc;      // 0x054 (subtitle string, mLength at 0x094)
    ml::FixStr<64> mPath;      // 0x098
    ml::FixStr<64> mFiles[8];  // 0x0DC
    u32 mFileId[8];            // 0x2FC
    s32 mCount;                // 0x31C
    s32 mCountRef;             // 0x320
    u8 mBusy;                  // 0x324
    u8 mFlag;                  // 0x325
    u8 mKind;                  // 0x326
};

// --- constructor ---------------------------------------------------------

// us-804f96c0: initialise the banner manager. The vtable is installed, the
// path/FixStr members are cleared, the per-slot id arrays are zeroed and the
// count/flag/latch fields are reset (mKind armed to 1).
CNBanner::CNBanner() {
    this->mVtable = (void*)lbl_eu_80570378;
    this->mTitle.clear();
    this->mDesc.clear();
    this->mPath.clear();
    this->mFiles[0].clear();
    // Pointer-walk clears the remaining 7 slots; MWCC keeps it as an mtctr/
    // bdnz counted loop with a runtime (end - start + 0x43) / 0x44 trip count.
    for (ml::FixStr<64>* f = &this->mFiles[1]; f < &this->mFiles[8]; f++) {
        f->clear();
    }
    this->mAlloc0 = 0;
    this->field_8 = 0;
    this->field_C = 0;
    memset(this->mFileId, 0, sizeof(this->mFileId));
    this->mCount = 0;
    this->mCountRef = 0;
    this->mBusy = 0;
    this->mFlag = 0;
    this->mKind = 1;
}

// --- destructor ----------------------------------------------------------

// us-804f9794: full object destructor (deleting destructor). Frees the dynamic
// buffer and, when the hidden deleting flag is set, releases the object. MWCC
// emits the deleting-dtor scaffold (null check, r4 flag test + operator delete
// of the object, `return this`) automatically for the out-of-line member
// destructor.
CNBanner::~CNBanner() {
    this->mVtable = (void*)lbl_eu_80570378;
    if (this->mBusy != 0) {
        func_8044F0E4__11CDeviceFileFPCc(this->mPath.c_str());
        int i = 0;
        while (i < this->mCount) {
            // One shared address per element: retail computes r3 =
            // &mFiles[i] once (add + lbzu) and reuses it for the check
            // and the release call.
            const char* p = this->mFiles[i].mString;
            if ((s8)p[0] != 0) {
                func_8044F0E4__11CDeviceFileFPCc(p);
            }
            i++;
        }
        this->mBusy = 0;
        func_eu_804521BC(this->mKind);
    }
    if (this->mAlloc0 != 0) {
        __dl__FPv(this->mAlloc0);
        this->mAlloc0 = 0;
    }
}

// --- func_804F531C -------------------------------------------------------

// us-804f9894: (re)build the banner slot list from a NULL-terminated array of
// path strings. Copies each name into a FixStr slot, records its id, then
// fills any remaining slots up to 8 with the empty format string.
void CNBanner::func_804F531C(const char** names, int* ids) {
    int i = 0;
    while (names[i] != 0) {
        this->mFiles[i].format(names[i]);
        this->mFileId[i] = ids[i];
        i++;
    }
    this->mCount = i;
    while (i < 8) {
        this->mFiles[i].format(lbl_eu_805248BC);
        this->mFileId[i] = 0;
        i++;
    }
}

// --- task-block destructor ------------------------------------------------

// us-804f9cc4: destroys the banner data block (the object held at +0x000 of a
// CNReqtaskSaveBannerData). Same teardown as the member destructor but without
// the object-delete scaffold: it always frees the dynamic buffer alone.
void __dt__804F5738(CNBanner* self) {
    if (self->mBusy != 0) {
        func_8044F0E4__11CDeviceFileFPCc(self->mPath.c_str());
        for (int i = 0; i < self->mCount; i++) {
            if ((s8)self->mFiles[i].mString[0] != 0) {
                func_8044F0E4__11CDeviceFileFPCc(self->mFiles[i].c_str());
            }
        }
        self->mBusy = 0;
        func_eu_804521BC(self->mKind);
    }
    if (self->mAlloc0 != 0) {
        __dl__FPv(self->mAlloc0);
        self->mAlloc0 = 0;
    }
}

// --- OnFileEvent ----------------------------------------------------------

// us-804f9d7c: match a completed file-load event against the primary path and
// then the per-slot paths. On a match, decrement the pending counter; the last
// one out (counter reaches 0) re-arms the count latch. Returns whether the
// event belonged to this banner.
bool CNBanner::OnFileEvent(CEventFile* event) {
    const char* name = event->field_C;
    u32 type = (u32)event->field_0;
    u8 notPrimary = (type != 1);

    if (strcmp(name, this->mPath.c_str()) == 0) {
        s32 c = this->mCountRef - 1;
        this->mCountRef = c;
        s32 busy = (c > 0);
        this->mBusy = (u8)busy;
        this->mFlag = (u8)((this->mFlag | notPrimary) != 0);
        if (busy == 0) {
            func_eu_804521BC(this->mKind);
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
                func_eu_804521BC(this->mKind);
            }
            return true;
        }
    }
    return false;
}

// --- non-target scaffolding (retained from the catalog stub) --------------

void CNBanner::func_804F52F8(const char* str) { mTitle.format(str); }

void CNBanner::func_804F5304(const char* str) { mDesc.format(str); }

void CNBanner::func_804F5310(const char* str) { mPath.format(str); }

// --- func_804F53DC --------------------------------------------------------

// us-804f9954: func_804F53DC
// Builds and starts loading the NAND banner block. Allocates a NANDBanner
// (0xF0A0) from MEM2, renders the UTF-8 title/subtitle fields to UTF-16 locals,
// zeroes the block and initialises its header with NANDInitBanner, packs the
// per-slot file ids into the u16 icon-speed bitset (2 bits per slot), then
// issues async archive reads of the banner texture and each icon's texture
// through CDeviceFile. Returns 1 on success, or 0 if the allocation or either
// encoding conversion failed.
extern "C" s32 func_804F53DC(CNBanner* self) {
    self->mFlag = 0;

    // Allocate the NAND banner data block from the MEM2 heap.
    void* alloc = mtl::MemManager::allocate_ex(
        0xF0A0, mtl::MemManager::getHandleMEM2(), 0x20);
    self->mAlloc0 = alloc;
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

    str = self->mTitle.c_str();
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

    str = self->mDesc.c_str();
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

    memset((CNandBannerBlock*)self->mAlloc0, 0, 0xF0A0);
    NANDInitBanner((NANDBanner*)self->mAlloc0, self->field_C,
                   reinterpret_cast<const wchar_t*>(title),
                   reinterpret_cast<const wchar_t*>(subtitle));

    // Pack each live slot's file id into the u16 icon-speed bitset (2 bits per
    // slot); the second loop clears the remaining idle slots.
    int i = 0;
    for (; i < self->mCount; i++) {
        u16 v = ((CNandBannerBlock*)self->mAlloc0)->iconSpeed;
        ((CNandBannerBlock*)self->mAlloc0)->iconSpeed =
            (u16)((v & ~(3 << (2 * i))) | (self->mFileId[i] << (2 * i)));
    }
    for (; i < 8; i++) {
        u16 v = ((CNandBannerBlock*)self->mAlloc0)->iconSpeed;
        ((CNandBannerBlock*)self->mAlloc0)->iconSpeed =
            (u16)(v & ~(3 << (2 * i)));
    }

    // Register the primary banner texture for async load, then each icon.
    self->mBusy = 1;
    self->mCountRef = 0;
    CDeviceFile::readCommonArchiveFile(
        (mtl::ALLOC_HANDLE)((CNandBannerBlock*)self->mAlloc0)->bannerTexture,
        self->mPath.c_str(), reinterpret_cast<IWorkEvent*>(self), 0x40, 0x6000);
    self->mCountRef++;

    for (int j = 0; j < self->mCount; j++) {
        CDeviceFile::readCommonArchiveFile(
            (mtl::ALLOC_HANDLE)((CNandBannerBlock*)self->mAlloc0)->iconTexture[j],
            self->mFiles[j].c_str(), reinterpret_cast<IWorkEvent*>(self),
            0x40, 0x1200);
        self->mCountRef++;
    }

    self->mKind = (u8)func_eu_804521C4();
    func_eu_804521BC(1);
    self->field_8 = self->mCount * 0x1200 + 0x60A0;
    return 1;
}