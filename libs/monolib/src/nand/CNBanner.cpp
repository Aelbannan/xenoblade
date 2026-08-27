// Decompiled: monolib/src/nand/CNBanner
//
// CNBanner manages the per-save banner filename list, deriving from IWorkEvent
// so the compiler emits its vtable/RTTI (retail blobs lbl_eu_80570378 etc.).
// Layout (after the base vtable pointer at +0x000):
//   +0x004 void*  mAlloc0   -- dynamically allocated NANDBanner (0xF0A0)
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
// OPEN ITEM (split-size): all 9 functions byte-match, but decomp .text is
// +0x50 over budget. The overflow is the weak out-of-line copy of the
// inline-empty ~IWorkEvent (~0x48) that MWCC emits into every TU which can
// see that body (IWORK_EVENT_INLINE_DTOR). Without it, ~CNBanner emits a
// __dt__10IWorkEventFv call (+0xC structural) instead. Closing this needs a
// header/tooling change (e.g. a declaration-only-with-visible-empty-strong
// variant of IWorkEvent.hpp) and is out of scope for this TU's writable files.

#include <string.h>
#include <decomp.h>
// Inline-empty base dtor: retail's derived dtor does not emit a
// __dt__10IWorkEventFv call (see IWorkEvent.hpp note).
#define IWORK_EVENT_INLINE_DTOR 1
#include <monolib/util/FixStr.hpp>
#include <monolib/util/MemManager.hpp>
#include <monolib/work/IWorkEvent.hpp>
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
void func_eu_804521BC(u8 val);
}

// Data objects referenced by this unit (global scope -> no C++ mangling).
extern const char lbl_eu_805248BC[];  // .rodata: path format string ("" for empty slots)

// CEventFile as consumed by CNBanner::OnFileEvent: an event type word at +0x0
// and a file-name/data string pointer at +0xC.
struct CEventFile {
    int field_0;          // 0x0  event type
    u8 pad4[8];           // 0x4
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

class CNBanner : public IWorkEvent {
public:
    CNBanner();
    ~CNBanner();
    // IWorkEvent override: only OnFileEvent is ever specialised here.
    virtual bool OnFileEvent(CEventFile* event);
    void setFileList(const char** names, int* ids);
    void setTitle(const char* str);
    void setSubtitle(const char* str);
    void setBannerPath(const char* str);

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

// us-804f96c0: initialise the banner manager. The compiler stores the vtable,
// default-constructs the FixStr members (inline stb/stw clears plus a counted
// loop over slots 1..7), then this body zeroes the id array and resets the
// count/flag fields (mKind armed to 1).
CNBanner::CNBanner() {
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
// buffer and releases registered paths. MWCC emits the deleting-dtor scaffold
// (null check, r4 flag test + operator delete of the object, `return this`)
// automatically for the virtual out-of-line member destructor.
CNBanner::~CNBanner() {
    if (this->mBusy != 0) {
        cancelJobsForPath__11CDeviceFileFPCc(this->mPath.c_str());
        // Byte offset kept alongside the index: retail walks the slot array
        // with a base+offset pair (add r3,r28,r30 / lbzu 220(r3)) and reuses
        // the computed element address for both the empty-check and the
        // release call.
        int i = 0;
        int off = 0;
        while (i < this->mCount) {
            const char* p = (const char*)this->mFiles + off;
            if ((s8)p[0] != 0) {
                cancelJobsForPath__11CDeviceFileFPCc(p);
            }
            off += (int)sizeof(ml::FixStr<64>);
            i++;
        }
        this->mBusy = 0;
        func_eu_804521BC(this->mKind);
    }
    if (this->mAlloc0 != 0) {
        ::operator delete(this->mAlloc0);
        this->mAlloc0 = 0;
    }
}

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
        this->mFiles[i].format(lbl_eu_805248BC);
        this->mFileId[i] = 0;
        i++;
    }
}

// --- task-block destructor ------------------------------------------------

// us-804f9cc4: destroys the banner data block (the object held at +0x000 of a
// CNReqtaskSaveBannerData). Same teardown as the member destructor but without
// the object-delete scaffold: it always frees the dynamic buffer alone.
// Retail keeps this under the address-named linker symbol __dt__804F5738;
// the exact_renames rule for CNBanner.o maps our C++-mangled name onto it.
void CNBanner_destroyBlock(CNBanner* banner) {
    if (banner->mBusy != 0) {
        cancelJobsForPath__11CDeviceFileFPCc(banner->mPath.c_str());
        for (int i = 0; i < banner->mCount; i++) {
            if ((s8)banner->mFiles[i].mString[0] != 0) {
                cancelJobsForPath__11CDeviceFileFPCc(banner->mFiles[i].c_str());
            }
        }
        banner->mBusy = 0;
        func_eu_804521BC(banner->mKind);
    }
    if (banner->mAlloc0 != 0) {
        ::operator delete(banner->mAlloc0);
        banner->mAlloc0 = 0;
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

void CNBanner::setTitle(const char* str) { mTitle.format(str); }

void CNBanner::setSubtitle(const char* str) { mDesc.format(str); }

void CNBanner::setBannerPath(const char* str) { mPath.format(str); }

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
        self->mPath.c_str(), self, 0x40, 0x6000);
    self->mCountRef++;

    for (int j = 0; j < self->mCount; j++) {
        CDeviceFile::readCommonArchiveFile(
            (mtl::ALLOC_HANDLE)((CNandBannerBlock*)self->mAlloc0)->iconTexture[j],
            self->mFiles[j].c_str(), self, 0x40, 0x1200);
        self->mCountRef++;
    }

    self->mKind = (u8)func_eu_804521C4();
    func_eu_804521BC(1);
    self->field_8 = self->mCount * 0x1200 + 0x60A0;
    return 1;
}


// ===== Dissolved monolibdata2 (blob surgery) data owned by this TU =====
// The CNBanner vtable / RTTI chain is compiler-emitted: the class derives
// IWorkEvent and overrides OnFileEvent, so MWCC's own __vt__8CNBanner,
// typeinfo and base list carry the retail bytes. Hand-spelling those slots
// as extern "C" collides with the member manglings under -ipa file
// (MWCC 10322; MWCC_CASES "CDeviceGX data gate"). Only the FOREIGN-class
// blobs of the dissolved monolithdata2 range stay hand-written here.
//
// Do NOT spell IWorkEvent slot names (__dt__8CNBannerFv, WorkEventN__...,
// __RTTI__10IWorkEvent) in this TU - see note above.

extern "C" u32 lbl_eu_80663B70;  // foreign .sdata typeinfo locator (CNRequest)

// [.rodata] RTTI name strings.
extern "C" __declspec(section ".rodata") const char lbl_eu_805248B0[0x10] = "CNBanner";
extern "C" __declspec(section ".rodata") const char lbl_eu_805248C0[0x13] = "CNReqtaskCreatedir";

// Foreign free functions referenced from the probe-vtable blob.
extern "C" void func_eu_804F9EE0();
extern "C" void func_804DA4CC();
// forward decls for cross-referencing blobs below
extern "C" u32 lbl_eu_80663CE8[2];
extern "C" u32 lbl_eu_80570410[4];
extern "C" u32 lbl_eu_80570420[3];
extern "C" u32 lbl_eu_80663CE0[2];
extern "C" u32 lbl_eu_80570400[4];

// [.sdata] RTTI name/hierarchy pairs (0x80663CE0-0x80663CF0). The CNBanner
// pair points at the compiler-emitted base list, renamed onto lbl_eu_80570400
// by the CNBanner.o UNIT_RULES. __declspec(section) is required: this TU is
// compiled with -sdata 0, so small aggregates would otherwise fall into .data.
extern "C" __declspec(section ".sdata") u32 lbl_eu_80663CE0[2] = { (u32)&lbl_eu_805248B0, (u32)&lbl_eu_80570400 };
extern "C" __declspec(section ".sdata") u32 lbl_eu_80663CE8[2] = { (u32)&lbl_eu_805248C0, (u32)&lbl_eu_80570420 };

// [.sbss] 0x80665A98: module-global string pointer installed by
// sinit_eu_804F9FA4 (monolib_eu_804F9E98).
char* lbl_eu_80665A98;  // kept alive by refs from monolib_eu_804F9E98.cpp
                        // (func_eu_804F9E98 / sinit_eu_804F9FA4)

// The probe-vtable + tail blobs are defined at EOF so their .data placement
// follows the compiler-emitted vtable/base list (retail order), see bottom.

// [.data] probe vtable + tail of the foreign class (defined here so their
// .data placement follows the compiler-emitted vtable / base list).
extern "C" u32 lbl_eu_80570410[4] = { (u32)&lbl_eu_80663CE8, 0, (u32)&func_eu_804F9EE0, (u32)&func_804DA4CC };
extern "C" u32 lbl_eu_80570420[3] = { (u32)&lbl_eu_80663B70, 0, 0 };
