#include "kyoshin/CBattery.hpp"
#include "kyoshin/code_80135FDC.hpp"
#include "monolib/device.hpp"
#include "monolib/lib.hpp"

#include <cstdio>

void closeFileHandle(CFileHandle**);

// Shared string pool in split1 .rodata (US/EU), relative to lbl_eu_8051399C:
// +0x00 "/menu/jp/Battery.arc"
// +0x14 "pic_%02d"
// +0x1D "CBattery"
// +0x26 "arc"
// +0x2A "mf00_btry.brlyt"
// (lbl_eu_80513990 "CBattery" heads the section at -0x0C.)

CBattery::CBattery(u8 batteryLevel) : CBatteryVtblBase(), mMemRegion() {
    mFileHandle = nullptr;
    mAccessor = nullptr;
    mLayout = nullptr;
    mLayoutReady = false;
    unk21 = 1;
    mDrawn = false;
    mBatteryLevel = batteryLevel;
}

CBattery::~CBattery() {
}

void CBattery::loadBatteryArchive() {
    mFileHandle = CDeviceFile::readFile(CWorkThreadSystem::getWorkMem(), lbl_eu_8051399C,
        reinterpret_cast<IWorkEvent*>(this), 0, 0);
    CDeviceFile::tryUpdateJobPriority(mFileHandle, 3);
    CDeviceFile::setHandleFlag2(mFileHandle);
}

void CBattery::updateLayout() {
    if (mDrawn == false || mLayoutReady == false)
        return;
    mLayout->Animate(0);
}

// r4 inherits from DrawInfo
void CBattery::drawBattery(void* param) {
    if (mDrawn == 0 || mLayoutReady == 0)
        return;
    drawLayout(mLayout, static_cast<nw4r::lyt::DrawInfo*>(param), 0, 1);
}

__declspec(noinline) void CBattery::releaseLayout() {
    CDeviceVI::waitForDrawDone();
    closeFileHandle(&mFileHandle);
    nw4r::lyt::Layout* layout = mLayout;
    mLayoutReady = false;
    if (layout != nullptr) {
        delete layout;
        mLayout = nullptr;
    }
    releaseArcResourceAccessor(mAccessor);
    mAccessor = nullptr;
    mMemRegion.deleteRegion();
}

void CBattery::setBatteryLevel(u8 level) {
    mBatteryLevel = level;
    updateBatteryImage();
}

/* Updates the current battery images by going through the individual images
for each bar, and only showing the ones for the current battery level. */
void __declspec(noinline) CBattery::updateBatteryImage() {
    if (mLayout != nullptr) {
        // Cap the battery level at 4
        if (mBatteryLevel > 4)
            mBatteryLevel = 4;

        char name[16];

        // Go through each image, and enable it if the index is <= battery level
        for (u8 num = 1; num <= 4; num++) {
            sprintf(name, (const char*)(u32)(lbl_eu_8051399C + 0x14), num);
            nw4r::lyt::Pane* pane = mLayout->GetRootPane()->FindPaneByName(name, true);
            if (pane != nullptr) {
                pane->SetVisible(num <= mBatteryLevel);
            }
        }
    }
}

__declspec(noinline) void CBattery::onLayoutReady() {
    if (mLayout != nullptr) {
        mDrawn = true;
        mLayoutReady = true;
        updateBatteryImage();
    }
}

bool CBattery::OnFileEvent(CEventFile* pEventFile) {
    // Only run the event if the file handle in the event struct matches the one in this class
    if (mFileHandle == pEventFile->mFileHandle) {
        if (pEventFile->unk0 != 1) {
            releaseLayout();
            return true;
        }

        // Create a region for layout related stuff
        mMemRegion.createRegion(CWorkThreadSystem::getWorkMem(), 0xC00, lbl_eu_8051399C + 0x1D, 0);
        Class_8045F858 sp8 = Class_8045F858(&mMemRegion);
        u8* data = static_cast<u8*>(mFileHandle->getData());
        mtl::MemManager::setMemInitFlag(0);
        mAccessor = CLibLayout::createArcResourceAccessor();
        mAccessor->Attach(data, lbl_eu_8051399C + 0x26);
        buildLayout(&mLayout, mAccessor, lbl_eu_8051399C + 0x2A);
        onLayoutReady();
        mFileHandle = nullptr;
        mMemRegion.validateHeap();
        return true;
    } else
        return false;
}

// Retail tables owned by this unit.
// .rodata (0x48): "CBattery" head (lbl_eu_80513990, read by the sdata locator
// below) then the shared string pool (lbl_eu_8051399C: offsets +0x14/+0x1D /
// +0x26/+0x2A used above). The head is [0x0C] (splitter says 0x9; the 3 pad
// bytes ride inside) so the pool starts at its retail +0x0C with no attribute
// needed - and none wanted: __attribute__((aligned(4))) on the pool flips
// updateBatteryImage's SetVisible lowering to a branch (measured 63%, +0x14
// .text). Plain const arrays keep .text 10/10.
extern const char lbl_eu_80513990[0x0C] = "CBattery";
extern const char lbl_eu_8051399C[0x3C] =
    "menu/jp/Battery.arc\0" "pic_%02d\0" "CBattery\0" "arc\0" "mf00_btry.brlyt\0\0";
// .sdata (0x8): locator pair {class-name, vtable-trailer}. The names match
// the retail reloc spellings exactly.
// .data (0x98): IWorkEvent vtable for CBattery (34 slots: locator, reserved,
// dtor, 31 WorkEvent slots with CBattery::OnFileEvent overriding slot 1),
// then the 16-byte lbl_eu_8053B918 trailer. Slot targets come from the retail
// rela.data (+0x04 reserved is a literal zero). Only the locator slot carries
// a real reference: every other slot's label is already declared in this TU
// (IWorkEvent/CBattery members via IWorkEvent.hpp/CBattery.hpp), so naming it
// again - even through an extern "C" object decl - is MWCC error 10322
// (measured for the dtor, OnFileEvent and all 31 WorkEvents). Those slots
// stay zero; file bytes are still retail-identical (reloc sites read as
// zeros) and only the linked addresses differ - see residual.
extern u32 lbl_eu_8053B918[4];
const void* lbl_eu_80662F28[2] = { lbl_eu_80513990, lbl_eu_8053B918 };
u32 lbl_eu_8053B890[34] = {
    (u32)lbl_eu_80662F28, 0, 0,
    0, 0,
    0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
};
// The RTTI word cannot name __RTTI__10IWorkEvent (MWCC auto-emits that symbol
// for the in-scope IWorkEvent class under -RTTI, so any declaration of it is
// error 10322; typeid is unavailable - MSL has no std::type_info). The slot
// stays zero with the all-zero-aggregate pragma idiom (cf. code_800A75FC.cpp)
// so the section keeps its retail size; the file bytes are retail-identical
// (reloc sites read as zeros) - only the linked address differs (residual).
#pragma section data_type ".data" ".data"
__declspec(section ".data") u32 lbl_eu_8053B918[4] = { 0, 0, 0, 0 };

