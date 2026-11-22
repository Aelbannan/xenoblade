// Inline-empty ~IWorkEvent: the ctor's cleanup action references the (link-deduped)
// weak copy, and ~CBattery elides the base call (retail shape).
#define IWORK_EVENT_INLINE_DTOR
#include "kyoshin/CBattery.hpp"
#include "kyoshin/code_80135FDC.hpp"
#include "monolib/device.hpp"
#include "monolib/lib.hpp"

#include <cstdio>
#include "kyoshin/code_80135FDC_State.hpp"


CBattery::CBattery(u8 batteryLevel) : mMemRegion() {
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
    mFileHandle = CDeviceFile::readFile(CWorkThreadSystem::getWorkMem(), "menu/jp/Battery.arc",
        this, 0, 0);
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

void CBattery::releaseLayout() {
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
void CBattery::updateBatteryImage() {
    if (mLayout != nullptr) {
        // Cap the battery level at 4
        if (mBatteryLevel > 4)
            mBatteryLevel = 4;

        char name[16];

        // Go through each image, and enable it if the index is <= battery level
        for (u8 num = 1; num <= 4; num++) {
            sprintf(name, "pic_%02d", num);
            nw4r::lyt::Pane* pane = mLayout->GetRootPane()->FindPaneByName(name, true);
            if (pane != nullptr) {
                pane->SetVisible(num <= mBatteryLevel);
            }
        }
    }
}

void CBattery::onLayoutReady() {
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
        mMemRegion.createRegion(CWorkThreadSystem::getWorkMem(), 0xC00, "CBattery", 0);
        Class_8045F858 sp8 = Class_8045F858(&mMemRegion);
        u8* data = static_cast<u8*>(mFileHandle->getData());
        mtl::MemManager::setMemInitFlag(0);
        mAccessor = CLibLayout::createArcResourceAccessor();
        mAccessor->Attach(data, "arc");
        buildLayout(&mLayout, mAccessor, "mf00_btry.brlyt");
        onLayoutReady();
        mFileHandle = nullptr;
        mMemRegion.validateHeap();
        return true;
    } else
        return false;
}

