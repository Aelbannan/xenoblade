#include "kyoshin/CBattery.hpp"
#include "kyoshin/code_80135FDC.hpp"
#include "monolib/device.hpp"
#include "monolib/lib.hpp"

#include <cstdio>

// Shared string pool in split1 .rodata (US/EU):
// +0x00 "/menu/jp/Battery.arc"
// +0x14 "pic_%02d"
// +0x1D "CBattery"
// +0x26 "arc"
// +0x2A "mf00_btry.brlyt"

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
    CDeviceFile::func_8044F154(mFileHandle, 3);
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
    func_801390E0(&mFileHandle);
    nw4r::lyt::Layout* layout = mLayout;
    mLayoutReady = false;
    if (layout != nullptr) {
        delete layout;
        mLayout = nullptr;
    }
    releaseArcResourceAccessor(mAccessor);
    mAccessor = nullptr;
    mMemRegion.func_8045F778();
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
            sprintf(name, lbl_eu_8051399C + 0x14, num);
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
        void* data = mFileHandle->getData();
        mtl::MemManager::setMemInitFlag(0);
        mAccessor = CLibLayout::createArcResourceAccessor();
        mAccessor->Attach(data, lbl_eu_8051399C + 0x26);
        buildLayout(&mLayout, mAccessor, lbl_eu_8051399C + 0x2A);
        onLayoutReady();
        mFileHandle = nullptr;
        mMemRegion.func_8045F810();
        return true;
    } else
        return false;
}
