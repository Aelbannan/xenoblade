#include "kyoshin/CBattery.hpp"
#include "kyoshin/code_80135FDC.hpp"
#include "monolib/device.hpp"
#include "monolib/lib.hpp"

#include <cstdio>

extern void func_801390E0(CFileHandle**);

// Shared string pool in split1 .rodata (US/EU):
// +0x00 "/menu/jp/Battery.arc"
// +0x14 "pic_%02d"
// +0x1D "CBattery"
// +0x26 "arc"
// +0x2A "mf00_btry.brlyt"
extern "C" char lbl_eu_8051399C[];

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

void CBattery::func_802B92A4() {
    mFileHandle = CDeviceFile::readFile(CWorkThreadSystem::getWorkMem(), lbl_eu_8051399C,
        reinterpret_cast<IWorkEvent*>(this), 0, 0);
    CDeviceFile::func_8044F154(mFileHandle, 3);
    CDeviceFile::setHandleFlag2(mFileHandle);
}

void CBattery::func_802B92FC() {
    if (mDrawn == false || mLayoutReady == false)
        return;
    mLayout->Animate(0);
}

// r4 inherits from DrawInfo
extern "C" void func_80137038__FPQ34nw4r3lyt6LayoutPQ34nw4r3lyt8DrawInfoii(nw4r::lyt::Layout*, nw4r::lyt::DrawInfo*, int, int);

void CBattery::func_802B9334(void* param) {
    if (mDrawn == 0 || mLayoutReady == 0)
        return;
    func_80137038__FPQ34nw4r3lyt6LayoutPQ34nw4r3lyt8DrawInfoii(mLayout, static_cast<nw4r::lyt::DrawInfo*>(param), 0, 1);
}

void CBattery::func_802B9364() {
    CDeviceVI::waitForDrawDone();
    func_801390E0(&mFileHandle);
    nw4r::lyt::Layout* layout = mLayout;
    mLayoutReady = false;
    if (layout != nullptr) {
        delete layout;
        mLayout = nullptr;
    }
    func_80139124(mAccessor);
    mAccessor = nullptr;
    mMemRegion.func_8045F778();
}

void CBattery::setBatteryLevel(u8 level) {
    mBatteryLevel = level;
    updateBatteryImage();
}

/* Updates the current battery images by going through the individual images
for each bar, and only showing the ones for the current battery level. */
void CBattery::updateBatteryImage() {
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

void CBattery::func_802B94B0() {
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
            func_802B9364();
            return true;
        }

        // Create a region for layout related stuff
        mMemRegion.createRegion(CWorkThreadSystem::getWorkMem(), 0xC00, lbl_eu_8051399C + 0x1D, 0);
        Class_8045F858 sp8 = Class_8045F858(&mMemRegion);
        void* data = mFileHandle->getData();
        mtl::MemManager::func_80434A4C(0);
        mAccessor = CLibLayout::createArcResourceAccessor();
        mAccessor->Attach(data, lbl_eu_8051399C + 0x26);
        func_80136E84(&mLayout, mAccessor, lbl_eu_8051399C + 0x2A);
        func_802B94B0();
        mFileHandle = nullptr;
        mMemRegion.func_8045F810();
        return true;
    } else
        return false;
}
