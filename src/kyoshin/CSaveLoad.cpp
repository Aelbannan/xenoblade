#define IWORK_EVENT_INLINE_DTOR

#include "kyoshin/harness_catalog.hpp"
#include "kyoshin/CScrollBar.hpp"
#include "kyoshin/CTaskGame.hpp"
#include "monolib/lib/CLibLayout.hpp"
extern "C" {
    void closeFileHandle__FPP11CFileHandle(void*);
    void func_80139124(void*);
    void func_80138078(int);
    void CtrlRemote_SetSharedBit(u32, u32);
}
#include "kyoshin/CSaveLoad.hpp"
#include "monolib/util/MemManager.hpp"
#include "monolib/work/CWorkSystem.hpp"
#include "monolib/lib/CLibHbm.hpp"
#include "monolib/lib/UnkClass_8045F564.hpp"
// code_80135FDC.hpp declares several nw4r::db warning strings as u8[] while
// CfObjectImplMove.hpp (via harness_catalog.hpp) already declared some of them
// const char[]. This TU never uses those strings, so alias them away for the
// duration of the include.
#define lbl_eu_80526324 lbl_eu_80526324_decl_shield
#define lbl_eu_80526300 lbl_eu_80526300_decl_shield
#define lbl_eu_805262F0 lbl_eu_805262F0_decl_shield
#define lbl_eu_805262C8 lbl_eu_805262C8_decl_shield
#define lbl_eu_8052CB40 lbl_eu_8052CB40_decl_shield
#define lbl_eu_8052CB1C lbl_eu_8052CB1C_decl_shield
#define lbl_eu_8052E4E4 lbl_eu_8052E4E4_decl_shield
#define lbl_eu_8052E4C0 lbl_eu_8052E4C0_decl_shield
#define lbl_eu_8052E558 lbl_eu_8052E558_decl_shield
#define lbl_eu_8052E530 lbl_eu_8052E530_decl_shield
#include "kyoshin/code_80135FDC.hpp"
#undef lbl_eu_80526324_decl_shield
#undef lbl_eu_80526300_decl_shield
#undef lbl_eu_805262F0_decl_shield
#undef lbl_eu_805262C8_decl_shield
#undef lbl_eu_8052CB40_decl_shield
#undef lbl_eu_8052CB1C_decl_shield
#undef lbl_eu_8052E4E4_decl_shield
#undef lbl_eu_8052E4C0_decl_shield
#undef lbl_eu_8052E558_decl_shield
#undef lbl_eu_8052E530_decl_shield
#include "monolib/work/CEventFile.hpp"
#include "monolib/device/CDeviceFont.hpp"
#include <string.h>
#include <revolution/os/OSCache.h>
#include "monolib/device/CDeviceSC.hpp"
#include "monolib/device/CDeviceFile.hpp"

// C-linkage imports - retail symbol names; extern "C" so calls bind to the
// literal (un)mangled identifiers. (Retail names are the true symbol names;
// do not rename.) Forward decls for structs whose full definitions live in
// the TU.

extern "C" void Cur_SetVisible(void*, u8);
extern "C" void* __dt__Q22cf7CfAwardFv(cf::CfAward*, int);
extern "C" u8 lbl_eu_80538858[];
extern "C" void CtrlRemote_ResetSlotArrayByIndex(void*);
extern "C" void* getHandleMEM2__Q23mtl10MemManagerFv();
extern "C" void* readFile__11CDeviceFileFUlPCcP10IWorkEventii(u32, const char*, void*, int, int);
extern "C" u8 lbl_eu_8050F7CC[];
extern "C" u32 lbl_eu_80662AD0;
extern "C" s32 lbl_eu_80662AC8;
extern "C" u32 lbl_eu_80662ACC;
extern "C" int CSysWin_isActive(void*);
extern "C" u32 CSysWin_isReady(void*);
extern "C" void requestLandTelop();
extern "C" void MenuUpdate_SignalGlobalField64();
extern "C" void CMenuGetItem_SetField54();
extern "C" void invalidateQstFlag();
extern "C" void CTaskGame_offerLoadCap();
extern "C" void resetBattlePresentation__Q22cf13CfGameManagerFv();
extern "C" u32 CtrlRemote_TouchBitByArg(u32);
extern "C" void* allocate_head__Q23mtl10MemManagerFUlUli(u32 handle, u32 size, int align);
extern "C" int CSysWin_getUnk34(void*);
extern "C" void sysWinDispatchPhase(void*);   // CSysWin per-frame update
extern "C" void Cur_Update(void*);   // CCur per-frame update
extern "C" void sysWinDrawLayout(void*, void*);
extern "C" void Cur_DrawLayout(void*, void*);
extern "C" void sysWinGetPaneScreenPos(void*, void*, u8);
extern "C" void awardCollectCount(int val);
extern "C" void awardQuestFlags(void);
extern "C" u32 chkAwardFlags99(void);
extern "C" u32 chkAwardFlags9B(void);
extern "C" void sysWinAdvancePhase3(void*);
extern "C" u32 CUICfManager_getPackedFontD8();
extern "C" void* CUICfManager_getPackedFont9C();
extern "C" void setPaneVisible(void* obj, u32 value);
extern "C" u8 bdatLookupRes(u16, u8);
extern "C" bool Attach__Q34nw4r3lyt19ArcResourceAccessorFPvPCc(nw4r::lyt::ArcResourceAccessor* self, void* data, const char* name);
extern "C" nw4r::lyt::ArcResourceAccessor* CUICfManager_getArcResourceAccessor();
extern "C" void sysWinSetKindContent(void*, u32, int);
extern "C" void sysWinSetTwoTextValues(void*, u32, u32);
extern "C" void sysWinApplyKind(void*, int);
extern "C" void sysWinOpenPhase1(void*);
extern "C" int isCollepediaId(int v);
extern "C" void NandRegHeapCb(void*, int, void (*)(int, int, u8));
extern "C" void DevFile_SetByteA9(int);
extern "C" void sysWinTermLayout(void*);
extern "C" void NandQueueChk860(int, void*);
extern "C" void NandQueueLoadCk(int, void*);
extern "C" void NandQueueSaveCk(int, void*, u8);
extern "C" void CtrlRemote_SetSharedBit(u32, u32);
extern "C" void clearGlobalState__Q22cf13CfGameManagerFv(int, int, int);
extern "C" void code80135FDC_thunk_BFE8C(u8*);
extern "C" void SysWinLog_QueueEvent(u32);
extern "C" u32 SysWinLog_BumpEventValue(u32, u32);
extern "C" int getTableValueByPair__Q22cf13CfGameManagerFv(int, int);
void bindLayoutAnimTransform(nw4r::lyt::Layout* layout, nw4r::lyt::AnimTransform** anim, nw4r::lyt::ArcResourceAccessor* accessor, char* name);
void buildLayout(nw4r::lyt::Layout** layout, nw4r::lyt::ArcResourceAccessor* accessor, const char* name);

// External function declarations
void playUISound(u32);
u32 advanceAnimTransform(nw4r::lyt::AnimTransform*, float);
// AnimRewindFrame declared in code_80135FDC.hpp
extern const float lbl_eu_80668B68;
extern const float lbl_eu_80668B6C;
extern const float lbl_eu_80668B70;
extern const float lbl_eu_80668B74;

// Forward declarations for functions called before their definitions
// extern "C" to match retail unmangled reloc names
extern cf::CfAward lbl_eu_806649F8;
extern u8 lbl_eu_80576CF8[];
extern void __register_global_object(void*, void*, void*);
extern void CtrlRemote_ResetSlotArrayObj(void*);

// CDeviceFile / MemManager / CSysWin / game system function declarations
extern u32 lbl_eu_80663E28;
// External function declarations needed by OnFileEvent
// Declared as void* by kyoshin/cf/CfGameManager.hpp (via CWorkSystem.hpp)
extern void* lbl_eu_806640A8;

extern CSLData* lbl_eu_806649F0;
extern CSaveLoad* lbl_eu_806649F4;
void drawLayout(nw4r::lyt::Layout*, nw4r::lyt::DrawInfo*, int, int);

#pragma push
#pragma dont_inline on
CSLData::CSLData() {
    mBuf[0] = nullptr;
    mBuf[1] = nullptr;
    mBuf[2] = nullptr;
    mRead[0] = 1;
    mRead[1] = 1;
    mRead[2] = 1;
    mValid[0] = 0;
    mValid[1] = 0;
    mValid[2] = 0;
}
#pragma pop

CSLData::~CSLData() {}

// Allocate the three 0xA000-byte slot header buffers from MEM2
__declspec(noinline) void CSLData::allocBufs() {
    lbl_eu_806649F0 = this;
    mBuf[0] = mtl::MemManager::allocate_head(mtl::MemManager::getHandleMEM2(), 0xA000, 0x40);
    mBuf[1] = mtl::MemManager::allocate_head(mtl::MemManager::getHandleMEM2(), 0xA000, 0x40);
    mBuf[2] = mtl::MemManager::allocate_head(mtl::MemManager::getHandleMEM2(), 0xA000, 0x40);
}

__declspec(noinline) void CSLData::freeBufs() {
    mtl::MemManager::deallocate(mBuf[0]);
    mtl::MemManager::deallocate(mBuf[1]);
    mtl::MemManager::deallocate(mBuf[2]);
    lbl_eu_806649F0 = nullptr;
}

__declspec(noinline) void CSLData::bindSlots() {
    NandRegHeapCb(mBuf[0], 1, onSlotRead);
    NandRegHeapCb(mBuf[1], 2, onSlotRead);
    NandRegHeapCb(mBuf[2], 3, onSlotRead);
    mRead[0] = 0;
    mRead[1] = 0;
    mRead[2] = 0;
    mValid[0] = 0;
    mValid[1] = 0;
    mValid[2] = 0;
}

__declspec(noinline) int CSLData::isReady() {
    int result = 0;
    if (mRead[0] != 0) {
        if (mRead[1] != 0) {
            if (mRead[2] != 0) {
                result = 1;
            }
        }
    }
    return result;
}

// Slot record (+0x20 into the header buffer) when the slot holds valid data.
__declspec(noinline) int CSLData::getSlot(u8 index) {
    if (index >= 3) return 0;
    void* ptr = mBuf[index];
    if (ptr == nullptr) return 0;
    if (mValid[index] != 0) {
        return (int)((u8*)ptr + 0x20);
    }
    return 0;
}

// NAND read callback: slot numbers are 1-based.
void CSLData::onSlotRead(int, int index, u8 valid) {
    if (lbl_eu_806649F0 == nullptr) return;
    (lbl_eu_806649F0->mRead - 1)[index] = 1;
    (lbl_eu_806649F0->mValid - 1)[index] = valid;
}

#pragma push
#pragma dont_inline on
CSLCur::CSLCur(nw4r::lyt::ArcResourceAccessor* accessor)
    : mAccessor(accessor), mLayout(nullptr), mAnimLoop(nullptr), mAnimPush(nullptr), mVisible(0),
      mState(0) {}
#pragma pop

CSLCur::~CSLCur() {}

__declspec(noinline) void CSLCur::createLayout() {
    buildLayout(&mLayout, mAccessor, (const char*)&lbl_eu_8050F7CC);
    bindLayoutAnimTransform(mLayout, &mAnimLoop, mAccessor, (char*)&lbl_eu_8050F7CC[0x18]);
    bindLayoutAnimTransform(mLayout, &mAnimPush, mAccessor, (char*)&lbl_eu_8050F7CC[0x35]);
    mLayout->UnbindAllAnimation();
    bindPane();
}

__declspec(noinline) void CSLCur::update() {
    if (mLayout == nullptr) return;

    switch (mState) {
    case 0:
        advanceAnimTransform(mAnimLoop, lbl_eu_80668B68);
        break;
    case 1:
        tickAnim();
        break;
    }

    mLayout->Animate(0);
}

__declspec(noinline) void CSLCur::draw(nw4r::lyt::DrawInfo* drawInfo) {
    if (mLayout == nullptr) return;
    if (mVisible == 0) return;
    drawLayout(mLayout, drawInfo, 0, 1);
}

__declspec(noinline) void CSLCur::destroy() {
    mVisible = 0;
    mAnimLoop = nullptr;
    mAnimPush = nullptr;
    if (mLayout != nullptr) {
        delete mLayout;
        mLayout = nullptr;
    }
}

__declspec(noinline) void CSLCur::setPos(const nw4r::math::VEC3* pos) {
    mLayout->GetRootPane()->SetTranslate(*pos);
}

__declspec(noinline) void CSLCur::tickAnim() {
    if (advanceAnimTransform(mAnimPush, lbl_eu_80668B68) != 0) {
        mState = 0;
        bindPane();
    }
}

__declspec(noinline) void CSLCur::bindPane() {
    mLayout->UnbindAnimation(mAnimPush);
    mLayout->BindAnimation(mAnimLoop);
    mLayout->SetAnimationEnable(mAnimLoop, true);
    mAnimLoop->SetFrame(lbl_eu_80668B6C);
    mLayout->Animate(0);
}

#pragma push
#pragma dont_inline on
CSLSub::CSLSub(nw4r::lyt::ArcResourceAccessor* accessor)
    : mAccessor(accessor), mLayout(nullptr), mAnimTrans(nullptr), mState(0), mShown(0), mIdle(1) {}
#pragma pop

CSLSub::~CSLSub() {}

__declspec(noinline) void CSLSub::create() {
    buildLayout(&mLayout, mAccessor, (const char*)&lbl_eu_8050F7CC[0x50]);
    bindLayoutAnimTransform(mLayout, &mAnimTrans, mAccessor, (char*)&lbl_eu_8050F7CC[0x69]);
    ApplyFontToTxtPanes(mLayout->GetRootPane(),
                        (const nw4r::ut::Font*)static_cast<IDeviceFontInfo*>(
                            CDeviceFont::getFontInfo(1, mLayout))->getFont());
    bindPane();
}

__declspec(noinline) void CSLSub::update() {
    if (mLayout == nullptr) return;

    switch (mState) {
    case 1:
        tickIn();
        break;
    case 3:
        tickOut();
        break;
    }

    mLayout->Animate(0);
}

__declspec(noinline) void CSLSub::destroy() {
    if (mLayout == nullptr) return;
    delete mLayout;
    mLayout = nullptr;
}

__declspec(noinline) void CSLSub::setBusy() {
    mState = 1;
    mShown = 1;
    mIdle = 0;
}

__declspec(noinline) void CSLSub::tickIn() {
    if (advanceAnimTransform(mAnimTrans, lbl_eu_80668B68) != 0) {
        mState = 2;
        mIdle = 1;
    }
}

__declspec(noinline) void CSLSub::tickOut() {
    if (AnimRewindFrame(mAnimTrans, lbl_eu_80668B68) != 0) {
        mState = 0;
        mShown = 0;
        mIdle = 1;
    }
}

__declspec(noinline) void CSLSub::bindPane() {
    mLayout->SetAnimationEnable(mAnimTrans, true);
    mAnimTrans->SetFrame(lbl_eu_80668B6C);
    mLayout->Animate(0);
}

// Sub-objects are built in the init list; the scroll bar and both windows
// are then rebuilt from temporaries with their own construction arguments.
CSaveLoad::CSaveLoad(u8 load, u8 arg)
    : mFileHandle(nullptr), mArcAccessor(nullptr), mLayout(nullptr), mAnimTransA(nullptr),
      mAnimTransB(nullptr), mCur(nullptr), mCur40(nullptr), mScrollBar(0), mSysWin98(0),
      mSysWinD4(0), mSub(nullptr), mField120(0), mField121(0), mField122(0), mField123(1),
      mField124(0), mField126(0), mField128(1), mField129(load), mField12A(0), mField12B(arg),
      mField12C(0), mField12D(0), mField12E(0), mField12F(0) {
    mSlotTex[0] = nullptr;
    mSlotTex[1] = nullptr;
    mSlotTex[2] = nullptr;
    {
        CScrollBar bar(1);
        mScrollBar = bar;
    }
    {
        CSysWin win(2);
        mSysWin98 = win;
    }
    {
        CSysWin win(0);
        mSysWinD4 = win;
    }
}

CSaveLoad::~CSaveLoad() {}

void CSaveLoad::loadSaveData() {
    mFileHandle = CDeviceFile::readFile(mtl::MemManager::getHandleMEM2(),
                                        (const char*)&lbl_eu_8050F7CC[0x92], this, 0, 0);
    mScrollBar.loadLayoutArc();
    mSysWin98.loadSystemArc();
    mSysWinD4.loadSystemArc();
    mField122 = 0;
    lbl_eu_806649F4 = this;
    lbl_eu_80662AD0 = -1;
}

// Per-frame update: dispatch on mField121 state, then refresh layout,
// cursor, scrollbar, both syswins and the sub-cursor.
void CSaveLoad::update() {
    if (mField120 == 0) return;

    switch (mField121) {
    case 0:
        tickOpenA();
        break;
    case 1:
        tickOpenB();
        break;
    case 2:
        tickCloseB();
        break;
    case 3:
        tickCloseA();
        break;
    case 4:
        tickConf();
        break;
    case 5:
        execOp();
        break;
    case 6:
        // Second window finished: drop back to the selection state
        if (CSysWin_isActive(&mSysWinD4) != 0) {
            mField121 = 3;
        }
        break;
    case 7:
        finish();
        break;
    case 8:
        tickWait();
        break;
    case 9:
        tickWindow();
        break;
    case 10:
    case 11:
    case 12:
        tickSlot();
        break;
    }

    mLayout->Animate(0);
    mCur.update();
    mScrollBar.UpdateDispatch();
    sysWinDispatchPhase(&mSysWin98);
    Cur_Update(&mCur40);
    sysWinDispatchPhase(&mSysWinD4);
    mSub.update();
}

// Draw the save/load screen layout and all sub-windows.
void CSaveLoad::draw(nw4r::lyt::DrawInfo* drawInfo) {
    if (mField120 == 0) return;

    drawLayout(mLayout, drawInfo, 0, 1);
    mScrollBar.draw(drawInfo);

    // Cursor pane is only drawn while both windows are idle
    if (CSysWin_getUnk34(&mSysWin98) == 0 && CSysWin_getUnk34(&mSysWinD4) == 0 &&
        mSub.mShown == 0 && mField121 <= 5) {
        mCur.draw(drawInfo);
    }

    sysWinDrawLayout(&mSysWin98, drawInfo);
    Cur_DrawLayout(&mCur40, drawInfo);
    sysWinDrawLayout(&mSysWinD4, drawInfo);

    if (mSub.mLayout != nullptr) {
        drawLayout(mSub.mLayout, drawInfo, 0, 1);
    }
}

// Cleanup/reset function for CSaveLoad
void CSaveLoad::reset() {
    CWorkSystem::setSaveLoadInvalidReset(false);
    DevFile_SetByteA9(1);
    CLibHbm::setHbmStopFlag(false);
    closeFileHandle__FPP11CFileHandle(&mFileHandle);
    mField120 = 0;
    mData.freeBufs();
    
    if (mLayout != nullptr) {
        delete mLayout;
        mLayout = nullptr;
    }
    
    releaseArcResourceAccessor(mArcAccessor);
    
    if (mSlotTex[0] != nullptr) {
        mtl::MemManager::deallocate(mSlotTex[0]);
        mSlotTex[0] = nullptr;
    }
    if (mSlotTex[1] != nullptr) {
        mtl::MemManager::deallocate(mSlotTex[1]);
        mSlotTex[1] = nullptr;
    }
    if (mSlotTex[2] != nullptr) {
        mtl::MemManager::deallocate(mSlotTex[2]);
        mSlotTex[2] = nullptr;
    }
    
    mCur.destroy();
    mScrollBar.Teardown();
    sysWinTermLayout(&mSysWin98);
    mCur40.cleanup();
    sysWinTermLayout(&mSysWinD4);
    mSub.destroy();
    mMemRegion.deleteRegion();
    lbl_eu_806649F4 = nullptr;
}

int CSaveLoad::isReady() {
    if (!mScrollBar.isVisible()) return 0;
    if (!CSysWin_isReady(&mSysWin98)) return 0;
    if (!CSysWin_isReady(&mSysWinD4)) return 0;
    if (!mData.isReady()) return 0;
    if (mSub.mShown != 0) return 0;
    return mField122;
}

int CSaveLoad::getIdle() { return mField123; }

// Check save window state: returns 1 if window is active/accessible,
// or mSub.mShown if both windows are idle
int CSaveLoad::isBusy() {
    if (mField121 != 3) return 1;
    if (CSysWin_getUnk34(&mSysWin98) != 0) return 1;
    if (CSysWin_getUnk34(&mSysWinD4) != 0) return 1;
    return mSub.mShown;
}

void CSaveLoad::open() {
    if (mField121 != 0) return;

    mField121 = 1;
    mField123 = 0;
    enAnimA();
    bestSlot();
    refresh();

    // Set up float array for scrollbar init
    float vec[3];
    vec[0] = lbl_eu_80668B70;
    vec[1] = lbl_eu_80668B74;
    vec[2] = lbl_eu_80668B6C;
    mScrollBar.InitRootPane(reinterpret_cast<const nw4r::math::VEC3*>(vec));

    int val = (int)(u16)lbl_eu_80662AC8;
    mScrollBar.UpdateThumb(3, val);

    renderSlots();
}

void CSaveLoad::close() {
    if (mField121 != 3) return;
    mField121 = 4;
    mField123 = 0;
    enAnimB();
    mCur.mVisible = 0;
    mScrollBar.requestScrollOut();
    playUISound(6);
}

// Handle cursor up/left movement in save/load screen.
// Decrements cursor position with wrap-around logic for both rows and columns.
void CSaveLoad::curUp() {
    if (mField121 != 3) return;
    if (CSysWin_getUnk34(&mSysWinD4) != 0) return;
    if (mSub.mShown != 0) return;
    
    if (CSysWin_getUnk34(&mSysWin98) != 0) {
        if (CSysWin_isActive(&mSysWin98) == 0) return;
        
        // Decrement mField128 with wrap from 0 to 1
        u8 val = mField128 - 1;
        mField128 = val;
        if ((s8)val < 0) {
            mField128 = 1;
        }
        
        refresh();
        playUISound(1);
        return;
    }
    
    // Decrement mField124
    u8 f124 = mField124 - 1;
    mField124 = f124;
    if ((s8)f124 >= 0) goto normalPath;
    
    // Underflow: move to previous row
    // Signed predecrement of the field: MWCC loads with lha, decrements in
    // place, stores, and sign-extends into a scratch reg for the test
    // (subi/extsh./sth on one register), matching retail.
    mField124 = 0;
    if (--mField126 >= 0) goto normalPath;
    
    // Underflow again: wrap to last row
    {
        s32 maxRows = (s32)lbl_eu_80662AC8;
        if (maxRows >= 3) {
            mField124 = 2;
            mField126 = (u16)((s32)lbl_eu_80662AC8 - 3);
        } else {
            u8 prev = (u8)(maxRows - 1);
            mField124 = prev;
            mField126 = 0;
            if ((s8)prev < 0) {
                mField124 = 0;
            }
        }
    }
    
normalPath:
    refresh();
    mScrollBar.PlaceThumb(mField126);
    playUISound(1);
}

// Handle cursor down/right movement in save/load screen.
// Increments cursor position with wrap-around logic.
void CSaveLoad::curDown() {
    if (mField121 != 3) return;
    if (CSysWin_getUnk34(&mSysWinD4) != 0) return;
    if (mSub.mShown != 0) return;
    
    if (CSysWin_getUnk34(&mSysWin98) != 0) {
        if (CSysWin_isActive(&mSysWin98) == 0) return;
        
        u8 val = mField128 + 1;
        mField128 = val;
        if ((s8)val > 1) {
            mField128 = 0;
        }
        
        refresh();
        playUISound(1);
        return;
    }
    
    // Main scrollbar path
    if (lbl_eu_80662AC8 >= 3) {
        u8 f124;
        f124 = mField124 + 1;
        mField124 = f124;
        if ((s8)f124 >= 3) {
            s16 row;
            mField124 = 2;
            row = (s16)mField126 + 1;
            mField126 = (u16)row;
            if (row > (lbl_eu_80662AC8 - 3)) {
                mField124 = 0;
                mField126 = 0;
            }
            goto updatePath;
        }
    } else {
        u8 f124;
        f124 = mField124 + 1;
        mField124 = f124;
        if ((s8)f124 >= lbl_eu_80662AC8) {
            mField124 = 0;
            mField126 = 0;
        }
    }

updatePath:
    refresh();
    mScrollBar.PlaceThumb(mField126);
    playUISound(1);
}

// Handle cursor movement: if global count >= 3, move cursor back by 3 positions
// with underflow handling. Otherwise reset cursor to 0.
void CSaveLoad::pageUp() {
    if (mField121 != 3) return;
    if (CSysWin_getUnk34(&mSysWin98) != 0) return;
    if (CSysWin_getUnk34(&mSysWinD4) != 0) return;
    if (mSub.mShown != 0) return;

    if ((s32)lbl_eu_80662AC8 >= 3) {
        s16 sval = (s16)mField126;
        s16 newVal = sval - 3;
        mField126 = (u16)newVal;
        if (newVal < 0) {
            int adj = (int)newVal + 2;
            mField124 = (u8)adj;
            mField126 = 0;
            if ((s8)(u8)adj < 0) {
                mField124 = 0;
            }
        }
    } else {
        mField124 = 0;
        mField126 = 0;
    }

    refresh();
    mScrollBar.PlaceThumb(mField126);
    playUISound(1);
}

// Page-down / advance the save-slot cursor by one entry (wraps every 3 rows).
void CSaveLoad::pageDown() {
    if (mField121 != 3) return;
    if (CSysWin_getUnk34(&mSysWin98) != 0) return;
    if (CSysWin_getUnk34(&mSysWinD4) != 0) return;
    if (mSub.mShown != 0) return;

    if ((s32)lbl_eu_80662AC8 >= 3) {
        // Advance row cursor by 3; clamp/wrap when past the last page
        s16 newVal = (s16)mField126 + 3;
        mField126 = (u16)newVal;
        if ((int)newVal > lbl_eu_80662AC8 - 3) {
            // Wrapped past the final row: derive column and clamp to 2
            u8 diff = (u8)(newVal - (lbl_eu_80662AC8 - 3));
            mField124 = diff;
            mField126 = (u16)(lbl_eu_80662AC8 - 3);
            if ((s8)diff >= 3) {
                mField124 = 2;
            }
        }
    } else {
        // Decrement last column; wrap to previous row on underflow
        int prev = lbl_eu_80662AC8 - 1;
        u8 prevU8 = prev;
        mField124 = prev;
        mField126 = 0;
        if ((s8)prevU8 < 0) {
            mField124 = 0;
        }
    }

    refresh();
    mScrollBar.PlaceThumb(mField126);
    playUISound(1);
}

// Handle window state transitions for save/load screen.
// Checks syswin at 0x98 and 0xD4, sets up cursor strings and state.
void CSaveLoad::confirm() {
    char* s1;
    char* s2;
    char* s3;

    if (CSysWin_getUnk34(&mSysWin98) != 0) {
        if (CSysWin_isActive(&mSysWin98) != 0) {
            if ((s8)mField128 == 0) {
                playUISound(3);
            } else {
                playUISound(6);
                mField12C = 0;
            }
            sysWinAdvancePhase3(&mSysWin98);
            Cur_SetVisible(&mCur40, 0);
            mField121 = 7;
        }
        return;
    }

    if (CSysWin_getUnk34(&mSysWinD4) != 0) {
        if (CSysWin_isActive(&mSysWinD4) != 0) {
            sysWinAdvancePhase3(&mSysWinD4);
            mField121 = 9;
            if (mField12C != 0) {
                mField12C = 0;
            }
            playUISound(3);
        }
        return;
    }

    if (mField121 != 3) return;

    if (mData.getSlot(mField124) == 0) goto noCursor;

    if (mField129 != 0) {
        s1 = (char*)BdatTouchStringCell((char*)&lbl_eu_8050F7CC[0xa7], (char*)&lbl_eu_8050F7CC[0xb2], 0x43);
        s2 = (char*)BdatTouchStringCell((char*)&lbl_eu_8050F7CC[0xa7], (char*)&lbl_eu_8050F7CC[0xb2], 0x44);
        s3 = (char*)BdatTouchStringCell((char*)&lbl_eu_8050F7CC[0xa7], (char*)&lbl_eu_8050F7CC[0xb2], 0x45);
        goto common;
    }

    s1 = (char*)BdatTouchStringCell((char*)&lbl_eu_8050F7CC[0xa7], (char*)&lbl_eu_8050F7CC[0xb2], 0x33);
    s2 = (char*)BdatTouchStringCell((char*)&lbl_eu_8050F7CC[0xa7], (char*)&lbl_eu_8050F7CC[0xb2], 0x34);
    s3 = (char*)BdatTouchStringCell((char*)&lbl_eu_8050F7CC[0xa7], (char*)&lbl_eu_8050F7CC[0xb2], 0x35);
    goto common;

noCursor:
    if (mField129 != 0) return;
    s1 = (char*)BdatTouchStringCell((char*)&lbl_eu_8050F7CC[0xa7], (char*)&lbl_eu_8050F7CC[0xb2], 0x38);
    s2 = (char*)BdatTouchStringCell((char*)&lbl_eu_8050F7CC[0xa7], (char*)&lbl_eu_8050F7CC[0xb2], 0x39);
    s3 = (char*)BdatTouchStringCell((char*)&lbl_eu_8050F7CC[0xa7], (char*)&lbl_eu_8050F7CC[0xb2], 0x3a);

common:
    sysWinSetKindContent(&mSysWin98, (u32)s1, 0);
    sysWinSetTwoTextValues(&mSysWin98, (u32)s2, (u32)s3);
    sysWinApplyKind(&mSysWin98, 0);
    sysWinOpenPhase1(&mSysWin98);
    mField128 = 1;
    mField121 = 6;
    playUISound(3);
}

// Handle window close event: if the first syswin is active and ready,
// close it, reset the sub-object state, and update status fields
void CSaveLoad::cancel() {
    if (CSysWin_getUnk34(&mSysWin98) == 0) return;
    if (CSysWin_isActive(&mSysWin98) == 0) return;

    sysWinAdvancePhase3(&mSysWin98);
    Cur_SetVisible(&mCur40, 0);

    mField121 = 7;
    mField128 = 1;
    mField12C = 0;
}

int CSaveLoad::getDone() { return mField12A; }

// Open the delete-confirm window for the selected slot.
void CSaveLoad::openDel() {
    if (mField121 != 3) return;
    if (CSysWin_getUnk34(&mSysWin98) != 0) return;
    if (CSysWin_getUnk34(&mSysWinD4) != 0) return;
    if (mSub.mShown != 0) return;
    if (mData.getSlot(mField124) == 0) return;

    char* s1 = BdatTouchStringCell((char*)&lbl_eu_8050F7CC[0xa7], (char*)&lbl_eu_8050F7CC[0xb2], 0x3b);
    char* s2 = BdatTouchStringCell((char*)&lbl_eu_8050F7CC[0xa7], (char*)&lbl_eu_8050F7CC[0xb2], 0x3c);
    char* s3 = BdatTouchStringCell((char*)&lbl_eu_8050F7CC[0xa7], (char*)&lbl_eu_8050F7CC[0xb2], 0x3d);
    sysWinSetKindContent(&mSysWin98, (u32)s1, 0);
    sysWinSetTwoTextValues(&mSysWin98, (u32)s2, (u32)s3);
    sysWinApplyKind(&mSysWin98, 0);
    sysWinOpenPhase1(&mSysWin98);
    mField128 = 1;
    mField12C = 1;
    mField121 = 6;
    playUISound(3);
}

// Check save/load state and return a status code:
// 0 = not ready (state mismatch, window active, or sub-window active)
// 0x69 = ready (no cursor) - mField129 == 0, CSLCur_getSlot returns 0
// 0x6A = ready (has cursor) - mField129 == 0, CSLCur_getSlot returns > 0
// 0x6B = ready (has cursor, mField129 != 0) - CSLCur_getSlot returns > 0
u8 CSaveLoad::getHelpKind() {
    if (mField121 != 3) return 0;
    if (CSysWin_getUnk34(&mSysWin98) != 0) return 0;
    if (CSysWin_getUnk34(&mSysWinD4) != 0) return 0;
    if (mSub.mShown != 0) return 0;

    if (mField129 != 0) {
        // Cursor active: return 0x6B if valid, 0 otherwise
        int rv = mData.getSlot(mField124);
        return rv > 0 ? 0x6B : 0;
    } else {
        // Cursor inactive: return 0x6A if valid, 0x69 otherwise
        int rv = mData.getSlot(mField124);
        return rv > 0 ? 0x6A : 0x69;
    }
}

__declspec(noinline) void CSaveLoad::tickOpenA() {
    if (advanceAnimTransform(mAnimTransA, lbl_eu_80668B68) != 0) {
        mField121 = 2;
        enAnimB();
    }
}

__declspec(noinline) void CSaveLoad::tickOpenB() {
    if (advanceAnimTransform(mAnimTransB, lbl_eu_80668B68) != 0) {
        mField121 = 3;
        mCur.mVisible = 1;
        refresh();
        mField123 = 1;
    }
}

__declspec(noinline) void CSaveLoad::tickCloseB() {
    if (AnimRewindFrame(mAnimTransB, lbl_eu_80668B68) != 0) {
        mField121 = 5;
        enAnimA();
    }
}

__declspec(noinline) void CSaveLoad::tickCloseA() {
    if (AnimRewindFrame(mAnimTransA, lbl_eu_80668B68) != 0) {
        mField121 = 0;
        mField123 = 1;
    }
}

__declspec(noinline) void CSaveLoad::tickConf() {
    if (CSysWin_isActive(&mSysWin98) != 0) {
        mField121 = 3;
        Cur_SetVisible(&mCur40, 1);
        refresh();
    }
}

// Handle save/load operation execution (state 5).
// Checks window state, cursor status, and dispatches to delete/load/save logic.
__declspec(noinline) void CSaveLoad::execOp() {
    if (CSysWin_isActive(&mSysWin98) == 0) return;

    mField121 = 3;
    if ((s8)mField128 != 0) return;

    mField121 = 10;

    if (mField12C != 0) {
        // Delete/overwrite path
        CWorkSystem::setSaveLoadInvalidReset(true);
        DevFile_SetByteA9(0);
        CLibHbm::setHbmStopFlag(true);

        char* msgText = (char*)BdatTouchStringCell((char*)&lbl_eu_8050F7CC[0xa7], (char*)&lbl_eu_8050F7CC[0xb2], 0x52);
        LayoutSetTextBoxFmtValue(mSub.mLayout, (char*)&lbl_eu_8050F7CC[0x85], msgText, 0u);

        mSub.setBusy();

        NandQueueChk860((s8)mField124 + 1, (void*)onFile);

        lbl_eu_80662AD0 = 0;
    } else if (mField129 != 0) {
        // Load path
        char* msgText = (char*)BdatTouchStringCell((char*)&lbl_eu_8050F7CC[0xa7], (char*)&lbl_eu_8050F7CC[0xb2], 0x46);
        LayoutSetTextBoxFmtValue(mSub.mLayout, (char*)&lbl_eu_8050F7CC[0x85], msgText, 0u);

        mSub.setBusy();

        NandQueueLoadCk((s8)mField124 + 1, (void*)onFile);

        lbl_eu_80662AD0 = 2;

        // Retail runs the slot update only when the presentation bit is CLEAR
        // (rlwinm./bne skips over it otherwise).
        if ((lbl_eu_80663E28 & 0x01000000u) == 0) {
            int result = mData.getSlot(mField124);
            clearGlobalState__Q22cf13CfGameManagerFv(*(u16*)(result + 0x0E), *(u8*)(result + 0x11), 1);
        }
    } else {
        // Save path
        if (mField12B != 0) {
            CtrlRemote_SetSharedBit(0x3213, 1);
        }

        CWorkSystem::setSaveLoadInvalidReset(true);
        DevFile_SetByteA9(0);
        CLibHbm::setHbmStopFlag(true);

        char* msgText = (char*)BdatTouchStringCell((char*)&lbl_eu_8050F7CC[0xa7], (char*)&lbl_eu_8050F7CC[0xb2], 0x36);
        LayoutSetTextBoxFmtValue(mSub.mLayout, (char*)&lbl_eu_8050F7CC[0x85], msgText, 0u);

        mSub.setBusy();

        NandQueueSaveCk((s8)mField124 + 1, (void*)onFile, mField12B);

        lbl_eu_80662AD0 = 3;
    }

    playUISound(0x80);
}

__declspec(noinline) void CSaveLoad::finish() {
    if (!CSysWin_isActive(&mSysWinD4)) return;

    if (mField129 == 0) goto setError3;
    if (mField12D == 0) goto setError3;

    mField12A = 1;
    requestLandTelop();
    MenuUpdate_SignalGlobalField64();
    CMenuGetItem_SetField54();
    invalidateQstFlag();

    // Load global after function calls to prevent reordering
    {
        u32 flags = lbl_eu_80663E28;
        if ((flags & 0x01000000u) != 0) {
            CTaskGame::getInstance();
            CTaskGame_offerLoadCap();
        } else {
            resetBattlePresentation__Q22cf13CfGameManagerFv();
        }
    }
    return;

setError3:
    mField121 = 3;
}

__declspec(noinline) void CSaveLoad::tickWait() {
    if (mSub.mIdle != 0) {
        mField121 = 3;
    }
}

// Handle save/load window state machine.
// Checks mSub.mIdle, mField12E, mField12C, mField129, mField12B
// and sets up the appropriate syswin with strings.
__declspec(noinline) void CSaveLoad::tickWindow() {
    char* s1;
    char* s2;
    char* s3;

    if (mSub.mIdle == 0) return;

    mField121 = 8;

    if (mField12E != 0) {
        int val = mField12F;
        // Forward-goto structure mirrors retail layout:
        // dispatch, body1, body2, 3-string block, then mField12E=0
        if ((u32)(val - 3) <= 1u) goto threeString;
        if (val == 1) goto win54;
        if (val == 2) goto win55;
        goto clear12E;
win54:
        s1 = (char*)BdatTouchStringCell((char*)&lbl_eu_8050F7CC[0xa7], (char*)&lbl_eu_8050F7CC[0xb2], 0x54);
        sysWinSetKindContent(&mSysWinD4, (u32)s1, 0);
        sysWinApplyKind(&mSysWinD4, 1);
        sysWinOpenPhase1(&mSysWinD4);
        goto clear12E;
win55:
        s1 = (char*)BdatTouchStringCell((char*)&lbl_eu_8050F7CC[0xa7], (char*)&lbl_eu_8050F7CC[0xb2], 0x55);
        sysWinSetKindContent(&mSysWinD4, (u32)s1, 0);
        sysWinApplyKind(&mSysWinD4, 1);
        sysWinOpenPhase1(&mSysWinD4);
        goto clear12E;
threeString:
        // 3-string path: update syswin 0x98 with strings
        s1 = (char*)BdatTouchStringCell((char*)&lbl_eu_8050F7CC[0xa7], (char*)&lbl_eu_8050F7CC[0xb2], 0x56);
        s2 = (char*)BdatTouchStringCell((char*)&lbl_eu_8050F7CC[0xa7], (char*)&lbl_eu_8050F7CC[0xb2], 0x59);
        // Statement-form selection keeps MWCC branchy (cmpi/li/beq/li), matching retail
        int msgIdx;
        if (mField129 != 0)
            msgIdx = 0x58;
        else
            msgIdx = 0x57;
        // Single call site
        s3 = (char*)BdatTouchStringCell((char*)&lbl_eu_8050F7CC[0xa7], (char*)&lbl_eu_8050F7CC[0xb2],
                                  msgIdx);
        sysWinSetKindContent(&mSysWin98, (u32)s1, 0);
        sysWinSetTwoTextValues(&mSysWin98, (u32)s2, (u32)s3);
        sysWinApplyKind(&mSysWin98, 0);
        sysWinOpenPhase1(&mSysWin98);
        mField128 = 1;
        mField12C = 1;
        mField121 = 6;
clear12E:
        mField12E = 0;
        return;
    }

    if (mField12C != 0) {
        CWorkSystem::setSaveLoadInvalidReset(false);
        DevFile_SetByteA9(1);
        CLibHbm::setHbmStopFlag(false);
        s1 = (char*)BdatTouchStringCell((char*)&lbl_eu_8050F7CC[0xa7], (char*)&lbl_eu_8050F7CC[0xb2], 0x3E);
        sysWinSetKindContent(&mSysWinD4, (u32)s1, 0);
        sysWinApplyKind(&mSysWinD4, 1);
        sysWinOpenPhase1(&mSysWinD4);
        playUISound(0x7A);
        return;
    }

    if (mField129 != 0) {
        s1 = (char*)BdatTouchStringCell((char*)&lbl_eu_8050F7CC[0xa7], (char*)&lbl_eu_8050F7CC[0xb2], 0x47);
        sysWinSetKindContent(&mSysWinD4, (u32)s1, 0);
        sysWinApplyKind(&mSysWinD4, 1);
        sysWinOpenPhase1(&mSysWinD4);
        lbl_eu_80662ACC = (u32)(s8)mField124;
        mField12D = 1;
        playUISound(0x28);
        return;
    }

    if (mField12B != 0) {
        CtrlRemote_SetSharedBit(0x3213, 0);
    }

    CWorkSystem::setSaveLoadInvalidReset(false);
    DevFile_SetByteA9(1);
    CLibHbm::setHbmStopFlag(false);
    s1 = (char*)BdatTouchStringCell((char*)&lbl_eu_8050F7CC[0xa7], (char*)&lbl_eu_8050F7CC[0xb2], 0x37);
    sysWinSetKindContent(&mSysWinD4, (u32)s1, 0);
    sysWinApplyKind(&mSysWinD4, 1);
    sysWinOpenPhase1(&mSysWinD4);
    lbl_eu_80662ACC = (u32)(s8)mField124;
    playUISound(0x28);
}

__declspec(noinline) void CSaveLoad::tickSlot() {
    if (mData.isReady() != 0) {
        mField121 = 0xb;
        mSub.mState = 3;
        mSub.mIdle = 0;
        renderSlots();
    }
}

__declspec(noinline) void CSaveLoad::enAnimA() {
    // Enable animation B, disable animation A
    mLayout->SetAnimationEnable(mAnimTransB, false);
    mLayout->SetAnimationEnable(mAnimTransA, true);
}

__declspec(noinline) void CSaveLoad::enAnimB() {
    // Enable animation A, disable animation B
    mLayout->SetAnimationEnable(mAnimTransA, false);
    mLayout->SetAnimationEnable(mAnimTransB, true);
}

// Refresh cursor/button display state for the save/load screen.
__declspec(noinline) void CSaveLoad::refresh() {
    if (CSysWin_getUnk34(&mSysWin98) != 0) {
        // Window open: refresh the L/R button highlight on the cursor object
        char btnBuf[0xC];
        sysWinGetPaneScreenPos(btnBuf, &mSysWin98, mField128);
        mCur40.setRootPaneTranslate(reinterpret_cast<nw4r::math::VEC3*>(btnBuf));
    } else {
        char* strBase = (char*)&lbl_eu_8050F7CC;
        char numBuf[0x18];
        char paneBuf[0xC];

        // Format the slot number text and set it on the root panes
        sprintf(numBuf, strBase + 0xb7, (s8)mField124 + 1);
        void* paneA = mLayout->GetRootPane()->FindPaneByName(numBuf, 1);
        void* paneB = mLayout->GetRootPane()->FindPaneByName(
            strBase + 0xc4, 1);
        ScalePaneWorldPos((nw4r::math::VEC3*)paneBuf, (nw4r::lyt::Pane*)paneA, (nw4r::lyt::Pane*)paneB, mLayout->GetRootPane());
        mCur.setPos(reinterpret_cast<nw4r::math::VEC3*>(paneBuf));
    }
}

// Render the save/load screen for all 3 slots.
// Gets localized strings, sets up text elements, buttons, and scrollbar
// for each save slot based on slot data (or empty if no data).
__declspec(noinline) void CSaveLoad::renderSlots() {
    char* strBase = (char*)lbl_eu_8050F7CC;

    char* str4a = (char*)BdatTouchStringCell(&strBase[0xa7], &strBase[0xb2], 0x4a);
    char* str4b = (char*)BdatTouchStringCell((char*)&strBase[0xa7], (char*)&strBase[0xb2], 0x4b);
    char* str4c = (char*)BdatTouchStringCell((char*)&strBase[0xa7], (char*)&strBase[0xb2], 0x4c);
    char* str4d = (char*)BdatTouchStringCell((char*)&strBase[0xa7], (char*)&strBase[0xb2], 0x4d);

    // Retail calls getLanguage separately for each flag.
    u32 isJapanese = (CDeviceSC::getLanguage() == 0);
    u32 isGerman = (CDeviceSC::getLanguage() == 2);

    char slotNameBuf[0x20];
    char textBuf[0x20];
    char emptyBtnBuf[0x20];
    char dataBtnBuf[0x20];

    // Loop through 3 save slots
    for (u8 slot = 0; slot < 3; slot++) {
        u8 slotNum = slot + 1;

        // Format slot name string
        sprintf(slotNameBuf, (const char*)&strBase[0xd3], slotNum);

        // Set up default background text for the slot
        LayoutSetTextBoxFmtValue(mLayout, slotNameBuf, str4c, 0u);

        // Check if the slot has save data
        u8* slotData = (u8*)mData.getSlot(slot);
        if (slotData == NULL) {
            // Empty slot
            sprintf(textBuf, (const char*)&strBase[0xde], str4d, str4d);
            sprintf(slotNameBuf, (const char*)&strBase[0xe3], slotNum);
            LayoutSetTextBoxText(mLayout, slotNameBuf, textBuf, 0u);

            sprintf(slotNameBuf, (const char*)&strBase[0xf2], slotNum);
            LayoutSetTextBoxFmtValue(mLayout, slotNameBuf, (char*)&strBase[0xfd], 0u);

            sprintf(textBuf, (const char*)&strBase[0xfe], str4d, str4d, str4a, str4d, str4d);
            sprintf(slotNameBuf, (const char*)&strBase[0x109], slotNum);
            LayoutSetTextBoxText(mLayout, slotNameBuf, textBuf, 0u);

            // Language-specific formatting
            if (isJapanese != 0) {
                sprintf(textBuf, (const char*)&strBase[0x117],
                        str4d, str4d, str4d, str4d, str4b, str4d, str4d, str4b, str4d);
            } else if (isGerman != 0) {
                sprintf(textBuf, (const char*)&strBase[0x12c],
                        str4d, str4d, str4d, str4d, str4d, str4d, str4d, str4d, str4d);
            } else {
                sprintf(textBuf, (const char*)&strBase[0x117],
                        str4d, str4d, str4b, str4d, str4d, str4b, str4d, str4d, str4d);
            }

            sprintf(slotNameBuf, (const char*)&strBase[0x13f], slotNum);
            LayoutSetTextBoxText(mLayout, slotNameBuf, textBuf, 0u);

            // Format empty slot detail text
            sprintf(textBuf, (const char*)&strBase[0xfe], str4d, str4d, str4a, str4d, str4d);
            sprintf(slotNameBuf, (const char*)&strBase[0x14c], slotNum);
            LayoutSetTextBoxText(mLayout, slotNameBuf, textBuf, 0u);

            // Hide all 7 per-slot buttons
            for (u8 btn = 0; btn < 7; btn++) {
                sprintf(emptyBtnBuf, (const char*)&strBase[0x15c], btn, btn + 1);
                void* pane =
                    mLayout->GetRootPane()->FindPaneByName(emptyBtnBuf, 1);
                if (pane != 0) {
                    setPaneVisible(pane, 0u);
                }
            }

            // Set up empty slot indicator
            sprintf(slotNameBuf, (const char*)&strBase[0x170], slotNum);
            LayoutSetTextBoxFmtValue(mLayout, slotNameBuf, (char*)&strBase[0xfd], 0u);

            // Check for award data
            void* awardData = mArcAccessor->GetResource(0x74696D67, (const char*)&strBase[0x17c], 0);
            if (awardData != 0) {
                sprintf(slotNameBuf, (const char*)&strBase[0x18f], slotNum);
                PaneSetTexPaletteByName(mLayout, (const char*)slotNameBuf, (TPLPalette*)awardData);
            }

            // Hide the main slot button
            sprintf(slotNameBuf, (const char*)&strBase[0x19d], slotNum);
            void* pane =
                mLayout->GetRootPane()->FindPaneByName(slotNameBuf, 1);
            setPaneVisible(pane, 0u);
        } else {
            // Slot has save data
            // Format slot data strings
            sprintf(textBuf, (const char*)&strBase[0x1aa], *(u16*)(slotData + 0x64));
            sprintf(slotNameBuf, (const char*)&strBase[0xe3], slotNum);
            LayoutSetTextBoxText(mLayout, slotNameBuf, textBuf, 0u);

            sprintf(slotNameBuf, (const char*)&strBase[0xf2], slotNum);
            LayoutSetTextBoxText(mLayout, slotNameBuf, (char*)(slotData + 0x44), 0u);

            // Format time played string (minutes clamped to 59 when over an hour cap)
            u16 playTime = *(u16*)(slotData + 0xa);
            u16 minutes = *(u16*)(slotData + 0xc);
            if (playTime > 0x63) {
                if (playTime > 0x3e7 || minutes > 0x3b) minutes = 0x3b;
                u16 hours = (playTime > 0x3e7) ? 0x3e7 : playTime;
                sprintf(textBuf, (const char*)&strBase[0x1af], hours, str4a, minutes);
            } else {
                if (minutes > 0x3b) minutes = 0x3b;
                u16 hours = (playTime > 0x63) ? 0x63 : playTime;
                sprintf(textBuf, (const char*)&strBase[0x1ba], hours, str4a, minutes);
            }

            sprintf(slotNameBuf, (const char*)&strBase[0x109], slotNum);
            LayoutSetTextBoxText(mLayout, slotNameBuf, textBuf, 0u);

            // Format slot details based on language and save data
            if (isJapanese != 0) {
                sprintf(textBuf, (const char*)&strBase[0x1c5],
                        *(u16*)(slotData + 0x4), str4b,
                        *(u16*)(slotData + 0x6), str4b,
                        *(u8*)(slotData + 0x9));
            } else if (isGerman != 0) {
                sprintf(textBuf, (const char*)&strBase[0x1d6],
                        *(u8*)(slotData + 0x9),
                        *(u16*)(slotData + 0x6),
                        *(u16*)(slotData + 0x4));
            } else {
                sprintf(textBuf, (const char*)&strBase[0x1e5],
                        *(u8*)(slotData + 0x9), str4b,
                        *(u16*)(slotData + 0x6), str4b,
                        *(u16*)(slotData + 0x4));
            }

            sprintf(slotNameBuf, (const char*)&strBase[0x13f], slotNum);
            LayoutSetTextBoxText(mLayout, slotNameBuf, textBuf, 0u);

            sprintf(textBuf, (const char*)&strBase[0x1ba],
                    *(u8*)(slotData + 0x8), str4a, *(u8*)(slotData + 0x2));
            sprintf(slotNameBuf, (const char*)&strBase[0x14c], slotNum);
            LayoutSetTextBoxText(mLayout, slotNameBuf, textBuf, 0u);

            // Set up the 7 award buttons
            for (u8 btn = 0; btn < 7; btn++) {
                sprintf(dataBtnBuf, (const char*)&strBase[0x15c], btn, btn + 1);
                void* pane =
                    mLayout->GetRootPane()->FindPaneByName(dataBtnBuf, 1);

                // Record presence flag lives in the low byte of a word entry
                u32 rec = *(u32*)(slotData + btn * 4 + 0x14);
                if ((u8)rec != 0) {
                    if (pane != 0) {
                        setPaneVisible(pane, 1u);
                    }
                    // Look up the award icon texture and attach it
                    char* iconName = (char*)MakeTplNameSysFile(
                        (u16)BdatGetU16Direct(lbl_eu_80664090, (const char*)&strBase[0x1f6], (u8)rec));
                    void* awardRes =
                        ((nw4r::lyt::ArcResourceAccessor*)CUICfManager_getArcResourceAccessor())
                            ->GetResource(0x74696D67, iconName, 0);
                    if (awardRes != 0) {
                        sprintf(textBuf, (const char*)&strBase[0x1ff], slotNum, btn + 1);
                        mLayout->GetRootPane()->FindPaneByName(textBuf, 1);
                        PaneSetTexPaletteByName(mLayout, (const char*)textBuf, (TPLPalette*)awardRes);
                    }
                } else {
                    if (pane != 0) {
                        setPaneVisible(pane, 0u);
                    }
                }
            }

            // Resolve the character-class indicator string
            u8 charResult = bdatLookupRes(*(u16*)(slotData + 0xe), *(u8*)(slotData + 0x11));
            char* slotIndicator;
            // NOTE: retail reloads the +0x66 class byte for each test (no cached local).
            if (*(u8*)(slotData + 0x66) != 0) {
                slotIndicator = (char*)BdatTouchStringCell((char*)&strBase[0xa7], (char*)&strBase[0xb2], 0x53);
            } else {
                slotIndicator = BdatGetPtrDirect(lbl_eu_806640A8, &strBase[0xb2], (u16)charResult);
            }

            if (*(u8*)(slotData + 0x67) != 0 && charResult == 1) {
                slotIndicator = (char*)BdatTouchStringCell((char*)&strBase[0xa7], (char*)&strBase[0xb2], 0x30);
            }

            sprintf(slotNameBuf, (const char*)&strBase[0x170], slotNum);
            LayoutSetTextBoxFmtValue(mLayout, slotNameBuf, slotIndicator, 0u);

            // Copy slot data to the heap buffer and flush it for GX
            memcpy(mSlotTex[slot], (const void*)(slotData + 0x80), 0x9C00);
            DCStoreRange(mSlotTex[slot], 0x9C00);

            // Handle class-specific data
            void* classData;
            if (*(u8*)(slotData + 0x66) != 0) {
                classData = mArcAccessor->GetResource(0x74696D67, (const char*)&strBase[0x211], 0);
            } else {
                classData = mSlotTex[slot];
            }

            if (classData != 0) {
                sprintf(slotNameBuf, (const char*)&strBase[0x18f], slotNum);
                PaneSetTexPaletteByName(mLayout, (const char*)slotNameBuf, (TPLPalette*)classData);
            }

            // Show/hide the final button depending on the character's sex flag
            sprintf(slotNameBuf, (const char*)&strBase[0x19d], slotNum);
            void* pane =
                mLayout->GetRootPane()->FindPaneByName(slotNameBuf, 1);
            setPaneVisible(pane, (*(u8*)(slotData + 0x67) != 0) ? 1u : 0u);
        }
    }
}

// Find the best save slot by comparing fields in priority order.
// If global value >= 0, use it directly. Otherwise iterate slots 0-2
// and find the one with the highest field values (lexicographic comparison).
// TU-local view of the CSLCur_getSlot record (+0x20 into the slot object):
// priority fields at +2/+3 (bytes), +4/+6 (halfwords), +8/+9 (bytes).
struct SaveSlotView { u8 b00; u8 b01; u8 b02; u8 b03; u16 w04; u16 w06; u8 b08; u8 b09; };
__declspec(noinline) void CSaveLoad::bestSlot() {
    s32 globalVal = (s32)lbl_eu_80662ACC;
    if (globalVal >= 0) {
        mField124 = (u8)globalVal;
        return;
    }

    s32 best = -1;
    u32 i;
    for (i = 0; i < 3; i++) {
        SaveSlotView* slot = (SaveSlotView*)mData.getSlot((u8)i);
        if (slot == nullptr) continue;

        // Compare fields in priority order: +4, +6, +9, +8, +2, +3
        if (best >= 0) {
            SaveSlotView* bestSlot = (SaveSlotView*)mData.getSlot((u8)best);
            if (bestSlot->w04 < slot->w04) {
                best = i & 0xFF;
            } else if (bestSlot->w04 == slot->w04) {
                if (bestSlot->w06 < slot->w06) {
                    best = i & 0xFF;
                } else if (bestSlot->w06 == slot->w06) {
                    if (bestSlot->b09 < slot->b09) {
                        best = i & 0xFF;
                    } else if (bestSlot->b09 == slot->b09) {
                        if (bestSlot->b08 < slot->b08) {
                            best = i & 0xFF;
                        } else if (bestSlot->b08 == slot->b08) {
                            if (bestSlot->b02 < slot->b02) {
                                best = i & 0xFF;
                                continue;
                            }
                            // Retail reuses the cr0 result here (bge) rather than
                            // testing equality, leaving the +3 check unreachable.
                            if (bestSlot->b02 >= slot->b02) continue;
                            if (bestSlot->b03 < slot->b03) {
                                best = i & 0xFF;
                            }
                        }
                    }
                }
            }
        } else {
            best = i & 0xFF;
        }
    }

    if (best != -1) {
        mField124 = (u8)best;
    }
}

// Load global CSaveLoad* from lbl_eu_806649F4. Takes 4 parameters (r3-r6)
// but only uses r5 (flag: non-zero = handle mField12C/mField129 branches + play sound)
// and r6 (byte value stored into mField12F when r5 == 0).
// r3 and r4 are unused params - they exist so r5/r6 are in the correct register slots.
// No local variable for the global pointer - access lbl_eu_806649F4 directly
// to force MWCC to reload after each function call, matching retail's repeated @sda21 loads.
void CSaveLoad::onFile(int, int, int completed, int msgKind) {
    if (lbl_eu_806649F4 == nullptr) return;

    if (completed != 0) {
        if (lbl_eu_806649F4->mField12C != 0) {
            lbl_eu_806649F4->mField121 = 0xc;
            lbl_eu_806649F4->mData.bindSlots();
        } else if (lbl_eu_806649F4->mField129 != 0) {
            lbl_eu_806649F4->mField121 = 0xb;
            // Local pointer so MWCC reuses it for both stores (retail keeps
            // the reloaded pointer in one register for mSub.mState/mSub.mIdle).
            CSaveLoad* q = lbl_eu_806649F4;
            q->mSub.mState = 3;
            q->mSub.mIdle = 0;
        } else {
            lbl_eu_806649F4->mField121 = 0xc;
            lbl_eu_806649F4->mData.bindSlots();
        }
        code80135FDC_thunk_BFE8C((u8*)0x80);
    } else {
        lbl_eu_806649F4->mField121 = 0xb;
        // Local pointer so MWCC reuses it for the mSub.mState/mSub.mIdle pair
        // (retail keeps one register for both stores).
        CSaveLoad* q = lbl_eu_806649F4;
        q->mSub.mState = 3;
        q->mSub.mIdle = 0;
        lbl_eu_806649F4->mField12E = 1;
        lbl_eu_806649F4->mField12F = (u8)msgKind;
    }
}

// Builds the layout from the loaded archive, then rebuilds the cursor,
// sub-window and slot-data members from temporaries bound to the accessor.
bool CSaveLoad::OnFileEvent(CEventFile* pEventFile) {
    if (mFileHandle == pEventFile->mFileHandle) {
    mtl::ALLOC_HANDLE handle = mtl::MemManager::getHandleMEM2();
    u8* strBase = lbl_eu_8050F7CC;
    mMemRegion.createRegion(handle, 0x10000, (const char*)&strBase[0x224], 0);
    Class_8045F858 regionGuard(&mMemRegion);

    void* fileData = mFileHandle->getData();
    mtl::MemManager::setMemInitFlag(false);

    nw4r::lyt::ArcResourceAccessor* accessor = CLibLayout::createArcResourceAccessor();
    mArcAccessor = accessor;
    accessor->Attach(fileData, (const char*)&strBase[0x22e]);

    buildLayout(&mLayout, mArcAccessor, (const char*)&strBase[0x232]);
    bindLayoutAnimTransform(mLayout, &mAnimTransA, mArcAccessor, (char*)&strBase[0x248]);
    bindLayoutAnimTransform(mLayout, &mAnimTransB, mArcAccessor, (char*)&strBase[0x261]);

    ApplyFontToTxtPanes(mLayout->GetRootPane(),
                        (const nw4r::ut::Font*)static_cast<IDeviceFontInfo*>(
                            CDeviceFont::getFontInfo(1, mLayout))->getFont());

    // Japanese-only caption strings applied to fixed pane names
    char* capJp = (char*)CUICfManager_getPackedFontD8();
    if (capJp != nullptr) {
        setLayoutTextBoxFont__FPQ34nw4r3lyt6LayoutPcUl(mLayout, (char*)&strBase[0x27f], (u32)capJp);
        setLayoutTextBoxFont__FPQ34nw4r3lyt6LayoutPcUl(mLayout, (char*)&strBase[0x288], (u32)capJp);
        setLayoutTextBoxFont__FPQ34nw4r3lyt6LayoutPcUl(mLayout, (char*)&strBase[0x295], (u32)capJp);
        setLayoutTextBoxFont__FPQ34nw4r3lyt6LayoutPcUl(mLayout, (char*)&strBase[0x29e], (u32)capJp);
        setLayoutTextBoxFont__FPQ34nw4r3lyt6LayoutPcUl(mLayout, (char*)&strBase[0x2ab], (u32)capJp);
        setLayoutTextBoxFont__FPQ34nw4r3lyt6LayoutPcUl(mLayout, (char*)&strBase[0x2b4], (u32)capJp);
    }

    // Localized caption strings applied to fifteen fixed pane names
    char* capLoc = (char*)CUICfManager_getPackedFont9C();
    if (capLoc != nullptr) {
        setLayoutTextBoxFont__FPQ34nw4r3lyt6LayoutPcUl(mLayout, (char*)&strBase[0x2c1], (u32)capLoc);
        setLayoutTextBoxFont__FPQ34nw4r3lyt6LayoutPcUl(mLayout, (char*)&strBase[0x2cd], (u32)capLoc);
        setLayoutTextBoxFont__FPQ34nw4r3lyt6LayoutPcUl(mLayout, (char*)&strBase[0x2d9], (u32)capLoc);
        setLayoutTextBoxFont__FPQ34nw4r3lyt6LayoutPcUl(mLayout, (char*)&strBase[0x2e5], (u32)capLoc);
        setLayoutTextBoxFont__FPQ34nw4r3lyt6LayoutPcUl(mLayout, (char*)&strBase[0x2f0], (u32)capLoc);
        setLayoutTextBoxFont__FPQ34nw4r3lyt6LayoutPcUl(mLayout, (char*)&strBase[0x2fe], (u32)capLoc);
        setLayoutTextBoxFont__FPQ34nw4r3lyt6LayoutPcUl(mLayout, (char*)&strBase[0x30a], (u32)capLoc);
        setLayoutTextBoxFont__FPQ34nw4r3lyt6LayoutPcUl(mLayout, (char*)&strBase[0x316], (u32)capLoc);
        setLayoutTextBoxFont__FPQ34nw4r3lyt6LayoutPcUl(mLayout, (char*)&strBase[0x322], (u32)capLoc);
        setLayoutTextBoxFont__FPQ34nw4r3lyt6LayoutPcUl(mLayout, (char*)&strBase[0x32d], (u32)capLoc);
        setLayoutTextBoxFont__FPQ34nw4r3lyt6LayoutPcUl(mLayout, (char*)&strBase[0x33b], (u32)capLoc);
        setLayoutTextBoxFont__FPQ34nw4r3lyt6LayoutPcUl(mLayout, (char*)&strBase[0x347], (u32)capLoc);
        setLayoutTextBoxFont__FPQ34nw4r3lyt6LayoutPcUl(mLayout, (char*)&strBase[0x353], (u32)capLoc);
        setLayoutTextBoxFont__FPQ34nw4r3lyt6LayoutPcUl(mLayout, (char*)&strBase[0x35f], (u32)capLoc);
        setLayoutTextBoxFont__FPQ34nw4r3lyt6LayoutPcUl(mLayout, (char*)&strBase[0x36a], (u32)capLoc);
    }

    enAnimA();
    mLayout->Animate(0);

    // L/R button labels for all three rows
    char* str31 = (char*)BdatTouchStringCell((char*)&strBase[0xa7], (char*)&strBase[0xb2], 0x31);
    char* str32 = (char*)BdatTouchStringCell((char*)&strBase[0xa7], (char*)&strBase[0xb2], 0x32);
    LayoutSetTextBoxFmtValue(mLayout, (char*)&strBase[0x378], str32, 0u);
    LayoutSetTextBoxFmtValue(mLayout, (char*)&strBase[0x389], str31, 0u);
    LayoutSetTextBoxFmtValue(mLayout, (char*)&strBase[0x39a], str32, 0u);
    LayoutSetTextBoxFmtValue(mLayout, (char*)&strBase[0x3ab], str31, 0u);
    LayoutSetTextBoxFmtValue(mLayout, (char*)&strBase[0x3bc], str32, 0u);
    LayoutSetTextBoxFmtValue(mLayout, (char*)&strBase[0x3cd], str31, 0u);

    setPaneVisible(mLayout->GetRootPane()->FindPaneByName((const char*)&strBase[0x3de], true), 0);
    setPaneVisible(mLayout->GetRootPane()->FindPaneByName((const char*)&strBase[0x3ea], true), 0);
    setPaneVisible(mLayout->GetRootPane()->FindPaneByName((const char*)&strBase[0x3f6], true), 0);

    mCur = CSLCur(mArcAccessor);
    mCur.createLayout();

    mCur40 = CCur18(CUICfManager_getArcResourceAccessor());
    mCur40.initLayout();

    mSub = CSLSub(mArcAccessor);
    mSub.create();

    mData = CSLData();
    mData.allocBufs();
    mData.bindSlots();

    mField122 = 1;
    mField120 = 1;
    mFileHandle = nullptr;
    mMemRegion.validateHeap();

    mSlotTex[0] = mtl::MemManager::allocate_head(mtl::MemManager::getHandleMEM2(), 0x9C00, 0x20);
    mSlotTex[1] = mtl::MemManager::allocate_head(mtl::MemManager::getHandleMEM2(), 0x9C00, 0x20);
    mSlotTex[2] = mtl::MemManager::allocate_head(mtl::MemManager::getHandleMEM2(), 0x9C00, 0x20);
    return true;
    }
    return false;
}

u32 getSaveLoadOp(void) {
    extern u32 lbl_eu_80662AD0;
    return lbl_eu_80662AD0;
}

// The cf::CfAward code from here to the end of the split is speed-optimised in
// retail (-O4,p shapes), unlike the -O4,s CSaveLoad code above; it is most
// likely a separate source file sharing this split. push/pop restores -O4,s
// at the end of the TU, which is where MWCC applies the -O4,s inlining rule
// (MWCC_CASES "two rules", rule A): with the state left "off" the two
// CSysWin copies in the ctor are inlined instead of calling the weak
// __as__7CSysWinFRC7CSysWin.
#pragma push
#pragma optimize_for_size off
cf::CfAward::~CfAward() {
    // Restore vtable to the complete object vtable before destruction
    void* base = (void*)&lbl_eu_80538858;
    mSecondBase = base;
    mVtbl = (char*)base + 8;

    // Call the cleanup function (subobject destructor)
    CtrlRemote_ResetSlotArrayByIndex(this);
}

// Count valid entries (0-299) and award achievements based on thresholds.
// Returns immediately if the initial parameter check fails.
void awardCollectCount(int val) {
    int i;
    int count;

    // Check if the input parameter is valid first
    if (((int (*)(int))isCollepediaId)(val) == 0) return;

    count = 0;
    i = 0;
    while (i < 300) {
        if (((int (*)(int))isCollepediaId)(i) != 0) {
            if (((u32 (*)(u32))CtrlRemote_TouchBitByArg)(i + 0x1d44) != 0) {
                count++;
            }
        }
        i++;
    }

    // Award achievements based on number of completed entries
    if ((u32)count >= 1) {
        SysWinLog_QueueEvent(8);
    }
    if ((u32)count >= 10) {
        SysWinLog_QueueEvent(9);
    }
    if ((u32)count >= 100) {
        SysWinLog_QueueEvent(10);
    }
}

extern "C" void awardCount8F() {
    u32 result = SysWinLog_BumpEventValue(0x8f, 1);
    if (result >= 0x64) {
        SysWinLog_QueueEvent(0x8f);
    }
    if (result >= 0x3e8) {
        SysWinLog_QueueEvent(0x90);
    }
}

extern "C" void awardCount91() {
    u32 result = SysWinLog_BumpEventValue(0x91, 1);
    if (result >= 1) {
        SysWinLog_QueueEvent(0x91);
    }
    if (result >= 0x32) {
        SysWinLog_QueueEvent(0x92);
    }
    if (result >= 0x12c) {
        SysWinLog_QueueEvent(0x93);
    }
}

// Forward declarations for retail functions not yet decompiled in this TU
// Check if a set of specific flags are all set.
// Returns 1 if all flags are valid (non-zero), 0 otherwise.
// Uses bool conversion to match MWCC's neg+or+srwi. pattern.
extern "C" u32 chkAwardFlags98() {
    bool b;
    b = CtrlRemote_TouchBitByArg(0xa26);
    if (!b) return 0;
    b = CtrlRemote_TouchBitByArg(0xa2c);
    if (!b) return 0;
    b = CtrlRemote_TouchBitByArg(0xa2d);
    if (!b) return 0;
    b = CtrlRemote_TouchBitByArg(0xa2e);
    if (!b) return 0;
    b = CtrlRemote_TouchBitByArg(0xa2f);
    if (!b) return 0;
    b = CtrlRemote_TouchBitByArg(0xa30);
    if (!b) return 0;
    b = CtrlRemote_TouchBitByArg(0xa32);
    if (!b) return 0;
    b = CtrlRemote_TouchBitByArg(0xa34);
    if (!b) return 0;
    b = CtrlRemote_TouchBitByArg(0xa4b);
    if (b) goto ret1;
    b = CtrlRemote_TouchBitByArg(0xac5);
    if (b) goto ret1;
    return 0;

ret1:
    return 1;
}

// Check if flags 0xad6/0xb48, 0xad7/0xb47, 0xb38-0xb41, 0xb44-0xb46 are all set.
// Returns 1 if all pass, 0 otherwise.
extern "C" u32 chkAwardFlags9D() {
    bool b = CtrlRemote_TouchBitByArg(0xad6);
    if (b) {} else {
        b = CtrlRemote_TouchBitByArg(0xb48);
        if (b) {} else return 0;
    }
    b = CtrlRemote_TouchBitByArg(0xad7);
    if (b) {} else {
        b = CtrlRemote_TouchBitByArg(0xb47);
        if (b) {} else return 0;
    }
    b = CtrlRemote_TouchBitByArg(0xb38);
    if (b) {} else return 0;
    b = CtrlRemote_TouchBitByArg(0xb39);
    if (b) {} else return 0;
    b = CtrlRemote_TouchBitByArg(0xb3a);
    if (b) {} else return 0;
    b = CtrlRemote_TouchBitByArg(0xb3b);
    if (b) {} else return 0;
    b = CtrlRemote_TouchBitByArg(0xb3c);
    if (b) {} else return 0;
    b = CtrlRemote_TouchBitByArg(0xb3d);
    if (b) {} else return 0;
    b = CtrlRemote_TouchBitByArg(0xb3e);
    if (b) {} else return 0;
    b = CtrlRemote_TouchBitByArg(0xb3f);
    if (b) {} else return 0;
    b = CtrlRemote_TouchBitByArg(0xb40);
    if (b) {} else return 0;
    b = CtrlRemote_TouchBitByArg(0xb41);
    if (b) {} else return 0;
    b = CtrlRemote_TouchBitByArg(0xb44);
    if (b) {} else return 0;
    b = CtrlRemote_TouchBitByArg(0xb45);
    if (b) {} else return 0;
    b = CtrlRemote_TouchBitByArg(0xb46);
    return b;
}

// Check if a comprehensive set of flags (0xa85-0xab1) are all set.
// Some checks have fallback flags if the primary is not set.
// Returns 1 if all pass, 0 otherwise.
u32 chkAwardFlags9B() {
    bool b = CtrlRemote_TouchBitByArg(0xa85);
    if (b) {} else return 0;
    b = CtrlRemote_TouchBitByArg(0xa86);
    if (b) {} else return 0;
    b = CtrlRemote_TouchBitByArg(0xa87);
    if (b) {} else return 0;
    b = CtrlRemote_TouchBitByArg(0xa88);
    if (b) {} else {
        b = CtrlRemote_TouchBitByArg(0xad8);
        if (b) {} else return 0;
    }
    b = CtrlRemote_TouchBitByArg(0xa89);
    if (b) {} else {
        b = CtrlRemote_TouchBitByArg(0xac9);
        if (b) {} else return 0;
    }
    b = CtrlRemote_TouchBitByArg(0xa8a);
    if (b) {} else return 0;
    b = CtrlRemote_TouchBitByArg(0xa8b);
    if (b) {} else return 0;
    b = CtrlRemote_TouchBitByArg(0xa8c);
    if (b) {} else return 0;
    b = CtrlRemote_TouchBitByArg(0xa8d);
    if (b) {} else return 0;
    b = CtrlRemote_TouchBitByArg(0xa8e);
    if (b) {} else return 0;
    b = CtrlRemote_TouchBitByArg(0xa8f);
    if (b) {} else {
        b = CtrlRemote_TouchBitByArg(0xa7e);
        if (b) {} else return 0;
    }
    b = CtrlRemote_TouchBitByArg(0xa90);
    if (b) {} else return 0;
    b = CtrlRemote_TouchBitByArg(0xa91);
    if (b) {} else return 0;
    b = CtrlRemote_TouchBitByArg(0xa92);
    if (b) {} else return 0;
    b = CtrlRemote_TouchBitByArg(0xa93);
    if (b) {} else {
        b = CtrlRemote_TouchBitByArg(0xacc);
        if (b) {} else return 0;
    }
    b = CtrlRemote_TouchBitByArg(0xa94);
    if (b) {} else return 0;
    b = CtrlRemote_TouchBitByArg(0xa95);
    if (b) {} else return 0;
    b = CtrlRemote_TouchBitByArg(0xa96);
    if (b) {} else return 0;
    b = CtrlRemote_TouchBitByArg(0xa97);
    if (b) {} else return 0;
    b = CtrlRemote_TouchBitByArg(0xa98);
    if (b) {} else return 0;
    b = CtrlRemote_TouchBitByArg(0xa99);
    if (b) {} else return 0;
    b = CtrlRemote_TouchBitByArg(0xa9a);
    if (b) {} else {
        b = CtrlRemote_TouchBitByArg(0xace);
        if (b) {} else return 0;
    }
    b = CtrlRemote_TouchBitByArg(0xa9b);
    if (b) {} else return 0;
    b = CtrlRemote_TouchBitByArg(0xa9c);
    if (b) {} else return 0;
    b = CtrlRemote_TouchBitByArg(0xa9d);
    if (b) {} else return 0;
    b = CtrlRemote_TouchBitByArg(0xa9e);
    if (b) {} else return 0;
    b = CtrlRemote_TouchBitByArg(0xa9f);
    if (b) {} else return 0;
    b = CtrlRemote_TouchBitByArg(0xaa0);
    if (b) {} else return 0;
    b = CtrlRemote_TouchBitByArg(0xaa1);
    if (b) {} else {
        b = CtrlRemote_TouchBitByArg(0xacd);
        if (b) {} else return 0;
    }
    b = CtrlRemote_TouchBitByArg(0xaa2);
    if (b) {} else return 0;
    b = CtrlRemote_TouchBitByArg(0xaa3);
    if (b) {} else return 0;
    b = CtrlRemote_TouchBitByArg(0xaa4);
    if (b) {} else {
        b = CtrlRemote_TouchBitByArg(0xa80);
        if (b) {} else {
            b = CtrlRemote_TouchBitByArg(0xa81);
            if (b) {} else return 0;
        }
    }
    b = CtrlRemote_TouchBitByArg(0xaa5);
    if (b) {} else return 0;
    b = CtrlRemote_TouchBitByArg(0xaa6);
    if (b) {} else return 0;
    b = CtrlRemote_TouchBitByArg(0xaa7);
    if (b) {} else {
        b = CtrlRemote_TouchBitByArg(0xaca);
        if (b) {} else return 0;
    }
    b = CtrlRemote_TouchBitByArg(0xaa8);
    if (b) {} else {
        b = CtrlRemote_TouchBitByArg(0xac8);
        if (b) {} else return 0;
    }
    b = CtrlRemote_TouchBitByArg(0xab1);
    if (b) {} else return 0;
    return 1;
}

// Check if flags 0xa71-0xa79, 0xa70, 0xac1-0xac5, 0xac6/0xac7, 0xac8-0xacc,
// 0xacd/0xace, 0xacf-0xad3, 0xad4/0xad5, 0xad6, 0xad7 are all set.
// Returns 1 if all pass, 0 otherwise.
extern "C" u32 chkAwardFlags9A() {
    bool b = CtrlRemote_TouchBitByArg(0xa71);
    if (b) {} else return 0;
    b = CtrlRemote_TouchBitByArg(0xa72);
    if (b) {} else return 0;
    b = CtrlRemote_TouchBitByArg(0xa73);
    if (b) {} else return 0;
    b = CtrlRemote_TouchBitByArg(0xa74);
    if (b) {} else return 0;
    b = CtrlRemote_TouchBitByArg(0xa75);
    if (b) {} else return 0;
    b = CtrlRemote_TouchBitByArg(0xa76);
    if (b) {} else return 0;
    b = CtrlRemote_TouchBitByArg(0xa77);
    if (b) {} else return 0;
    b = CtrlRemote_TouchBitByArg(0xa78);
    if (b) {} else return 0;
    b = CtrlRemote_TouchBitByArg(0xa79);
    if (b) {} else return 0;
    b = CtrlRemote_TouchBitByArg(0xa70);
    if (b) {} else return 0;
    b = CtrlRemote_TouchBitByArg(0xac1);
    if (b) {} else return 0;
    b = CtrlRemote_TouchBitByArg(0xac2);
    if (b) {} else return 0;
    b = CtrlRemote_TouchBitByArg(0xac3);
    if (b) {} else return 0;
    b = CtrlRemote_TouchBitByArg(0xac4);
    if (b) {} else return 0;
    b = CtrlRemote_TouchBitByArg(0xac5);
    if (b) {} else return 0;
    b = CtrlRemote_TouchBitByArg(0xac6);
    if (b) {} else {
        b = CtrlRemote_TouchBitByArg(0xac7);
        if (b) {} else return 0;
    }
    b = CtrlRemote_TouchBitByArg(0xac8);
    if (b) {} else return 0;
    b = CtrlRemote_TouchBitByArg(0xac9);
    if (b) {} else return 0;
    b = CtrlRemote_TouchBitByArg(0xaca);
    if (b) {} else return 0;
    b = CtrlRemote_TouchBitByArg(0xacb);
    if (b) {} else return 0;
    b = CtrlRemote_TouchBitByArg(0xacc);
    if (b) {} else return 0;
    b = CtrlRemote_TouchBitByArg(0xacd);
    if (b) {} else {
        b = CtrlRemote_TouchBitByArg(0xace);
        if (b) {} else return 0;
    }
    b = CtrlRemote_TouchBitByArg(0xacf);
    if (b) {} else return 0;
    b = CtrlRemote_TouchBitByArg(0xad0);
    if (b) {} else return 0;
    b = CtrlRemote_TouchBitByArg(0xad1);
    if (b) {} else return 0;
    b = CtrlRemote_TouchBitByArg(0xad2);
    if (b) {} else return 0;
    b = CtrlRemote_TouchBitByArg(0xad3);
    if (b) {} else return 0;
    b = CtrlRemote_TouchBitByArg(0xad4);
    if (b) {} else {
        b = CtrlRemote_TouchBitByArg(0xad5);
        if (b) {} else return 0;
    }
    b = CtrlRemote_TouchBitByArg(0xad6);
    if (b) {} else return 0;
    b = CtrlRemote_TouchBitByArg(0xad7);
    return b;
}

// Check if various flag groups (0xa66, 0xa67, 0xaa9/0xb05, 0xaaa/0xb07,
// 0xaab/0xad0/0xb1f, 0xacf/0xae7/0xb1b, 0xad1/0xae8/0xb1c, 0xad2/0xaea/0xb1e,
// 0xad3/0xae9/0xb1d, 0xad4/0xaeb/0xb20, 0xad5/0xaec/0xb21, 0xaed/0xb23,
// 0xadf/0xafe, 0xae0/0xb00, 0xae1/0xb04, 0xae2/0xb0d, 0xae3/0xb11,
// 0xae4/0xb13, 0xae5/0xb14, 0xae6/0xb18) are set.
// Returns 1 if all pass, 0 otherwise.
extern "C" u32 chkAwardFlags9C() {
    bool b = CtrlRemote_TouchBitByArg(0xa66);
    if (b) {} else return 0;
    b = CtrlRemote_TouchBitByArg(0xa67);
    if (b) {} else return 0;
    b = CtrlRemote_TouchBitByArg(0xaa9);
    if (b) {} else {
        b = CtrlRemote_TouchBitByArg(0xb05);
        if (b) {} else return 0;
    }
    b = CtrlRemote_TouchBitByArg(0xaaa);
    if (b) {} else {
        b = CtrlRemote_TouchBitByArg(0xb07);
        if (b) {} else return 0;
    }
    b = CtrlRemote_TouchBitByArg(0xaab);
    if (b) {} else {
        b = CtrlRemote_TouchBitByArg(0xad0);
        if (b) {} else {
            b = CtrlRemote_TouchBitByArg(0xb1f);
            if (b) {} else return 0;
        }
    }
    b = CtrlRemote_TouchBitByArg(0xacf);
    if (b) {} else {
        b = CtrlRemote_TouchBitByArg(0xae7);
        if (b) {} else {
            b = CtrlRemote_TouchBitByArg(0xb1b);
            if (b) {} else return 0;
        }
    }
    b = CtrlRemote_TouchBitByArg(0xad1);
    if (b) {} else {
        b = CtrlRemote_TouchBitByArg(0xae8);
        if (b) {} else {
            b = CtrlRemote_TouchBitByArg(0xb1c);
            if (b) {} else return 0;
        }
    }
    b = CtrlRemote_TouchBitByArg(0xad2);
    if (b) {} else {
        b = CtrlRemote_TouchBitByArg(0xaea);
        if (b) {} else {
            b = CtrlRemote_TouchBitByArg(0xb1e);
            if (b) {} else return 0;
        }
    }
    b = CtrlRemote_TouchBitByArg(0xad3);
    if (b) {} else {
        b = CtrlRemote_TouchBitByArg(0xae9);
        if (b) {} else {
            b = CtrlRemote_TouchBitByArg(0xb1d);
            if (b) {} else return 0;
        }
    }
    b = CtrlRemote_TouchBitByArg(0xad4);
    if (b) {} else {
        b = CtrlRemote_TouchBitByArg(0xaeb);
        if (b) {} else {
            b = CtrlRemote_TouchBitByArg(0xb20);
            if (b) {} else return 0;
        }
    }
    b = CtrlRemote_TouchBitByArg(0xad5);
    if (b) {} else {
        b = CtrlRemote_TouchBitByArg(0xaec);
        if (b) {} else {
            b = CtrlRemote_TouchBitByArg(0xb21);
            if (b) {} else return 0;
        }
    }
    b = CtrlRemote_TouchBitByArg(0xaed);
    if (b) {} else {
        b = CtrlRemote_TouchBitByArg(0xb23);
        if (b) {} else return 0;
    }
    b = CtrlRemote_TouchBitByArg(0xadf);
    if (b) {} else {
        b = CtrlRemote_TouchBitByArg(0xafe);
        if (b) {} else return 0;
    }
    b = CtrlRemote_TouchBitByArg(0xae0);
    if (b) {} else {
        b = CtrlRemote_TouchBitByArg(0xb00);
        if (b) {} else return 0;
    }
    b = CtrlRemote_TouchBitByArg(0xae1);
    if (b) {} else {
        b = CtrlRemote_TouchBitByArg(0xb04);
        if (b) {} else return 0;
    }
    b = CtrlRemote_TouchBitByArg(0xae2);
    if (b) {} else {
        b = CtrlRemote_TouchBitByArg(0xb0d);
        if (b) {} else return 0;
    }
    b = CtrlRemote_TouchBitByArg(0xae3);
    if (b) {} else {
        b = CtrlRemote_TouchBitByArg(0xb11);
        if (b) {} else return 0;
    }
    b = CtrlRemote_TouchBitByArg(0xae4);
    if (b) {} else {
        b = CtrlRemote_TouchBitByArg(0xb13);
        if (b) {} else return 0;
    }
    b = CtrlRemote_TouchBitByArg(0xae5);
    if (b) {} else {
        b = CtrlRemote_TouchBitByArg(0xb14);
        if (b) {} else return 0;
    }
    b = CtrlRemote_TouchBitByArg(0xae6);
    if (b) {} else {
        b = CtrlRemote_TouchBitByArg(0xb18);
        if (b) {} else return 0;
    }
    return 1;
}

u32 chkAwardFlags99();

// Count valid save entries (0-299) and award achievements based on thresholds.
// Also checks other game completion milestones.
void awardQuestFlags() {
    int i;
    u32 count = 0;
    for (i = 0; i < 300; i++) {
        bool b = CtrlRemote_TouchBitByArg(i + 0xa20);
        if (b) count++;
    }
    
    if (count >= 1) SysWinLog_QueueEvent(0x94);
    if (count >= 10) SysWinLog_QueueEvent(0x95);
    if (count >= 50) SysWinLog_QueueEvent(0x96);
    if (count >= 120) SysWinLog_QueueEvent(0x97);
    
    if (chkAwardFlags98()) SysWinLog_QueueEvent(0x98);
    if (chkAwardFlags99()) SysWinLog_QueueEvent(0x99);
    if (chkAwardFlags9A()) SysWinLog_QueueEvent(0x9a);
    if (chkAwardFlags9B()) SysWinLog_QueueEvent(0x9b);
    if (chkAwardFlags9C()) SysWinLog_QueueEvent(0x9c);
    if (chkAwardFlags9D()) SysWinLog_QueueEvent(0x9d);
}



// Despite Fv mangling, receives (self, val) with val in r4 -
// the value is checked against the [0x28, 0x3e) window.
// Check all pairs of values (1-7) and award achievements based on results.
// allFlag is set if all pairs have result >= 2000.
// allFlag2 is set if all pairs have result >= 5000.
extern "C" void awardAffinity() {
    int allFlag = 1;
    int allFlag2 = 1;
    int i = 1;
    int j;

    for (; i <= 7; i++) {
        for (j = i + 1; j <= 7; j++) {
            int result = getTableValueByPair__Q22cf13CfGameManagerFv(i, j);

            if (result >= 1000) {
                SysWinLog_QueueEvent(0xaf);
            }
            if (result >= 3000) {
                SysWinLog_QueueEvent(0xb0);
            }
            if (result >= 5000) {
                SysWinLog_QueueEvent(0xb1);
            }
            if (result < 2000) {
                allFlag = 0;
            }
            if (result < 5000) {
                allFlag2 = 0;
            }
        }
    }

    if (allFlag != 0) {
        SysWinLog_QueueEvent(0xb2);
    }
    if (allFlag2 != 0) {
        SysWinLog_QueueEvent(0xb3);
    }
}

// Despite Fv mangling, receives (self, val) with val in r4 -
// the value is checked against the [0x28, 0x3e) window.
extern "C" void processAward__Q22cf7CfAwardFv(cf::CfAward*, int val) {
    if (val < 0x28) return;
    if (val >= 0x3e) return;
    awardAffinity();
}


// Count valid items/quests: iterates IDs 0-299, checks validity via
// isCollepediaId and CtrlRemote_TouchBitByArg, returns the count of valid entries
// Retail symbol is unmangled; declared extern "C" in CfGameManager.hpp
extern "C" s32 countCollepedia() {
    int count;
    int i;
    count = 0;
    i = 0;
    while (i < 300) {
        if (((int (*)(int))isCollepediaId)(i) != 0) {
            if (((u32 (*)(u32))CtrlRemote_TouchBitByArg)(i + 0x1d44) != 0) {
                count++;
            }
        }
        i++;
    }
    return count;
}

// Check if a value is within the valid range.
// Returns 0 for specific values that are excluded, 1 otherwise.
// Binary-search dispatch tree; transcribed 1:1 from retail.
extern "C" int isCollepediaId(int v) {
    if (v == 0x8B)
        goto L_8029434C;
    if (v >= 0x8B)
        goto L_802942C4;
    if (v == 0x13)
        goto L_8029432C;
    if (v >= 0x13)
        goto L_802942A0;
    if (v == 0x10)
        goto L_80294314;
    if (v >= 0x10)
        goto L_80294294;
    if (v >= 0xF)
        goto L_8029430C;
    if (v >= 0x0)
        goto L_8029438C;
    goto L_80294304;

L_80294294:
    if (v >= 0x12)
        goto L_80294324;
    goto L_8029431C;

L_802942A0:
    if (v == 0x6E)
        goto L_8029433C;
    if (v >= 0x6E)
        goto L_802942B8;
    if (v >= 0x6D)
        goto L_80294334;
    goto L_8029438C;

L_802942B8:
    if (v >= 0x70)
        goto L_8029438C;
    goto L_80294344;

L_802942C4:
    if (v == 0xE5)
        goto L_80294374;
    if (v >= 0xE5)
        goto L_802942EC;
    if (v == 0xE3)
        goto L_80294364;
    if (v >= 0xE3)
        goto L_8029436C;
    if (v == 0x8D)
        goto L_8029435C;
    if (v >= 0x8D)
        goto L_8029438C;
    goto L_80294354;

L_802942EC:
    if (v == 0xE7)
        goto L_80294384;
    if (v < 0xE7)
        goto L_8029437C;
    if (v >= 0x12C)
        goto L_80294304;
    goto L_8029438C;

L_80294304:
    return 0;
L_8029430C:
    return 0;
L_80294314:
    return 0;
L_8029431C:
    return 0;
L_80294324:
    return 0;
L_8029432C:
    return 0;
L_80294334:
    return 0;
L_8029433C:
    return 0;
L_80294344:
    return 0;
L_8029434C:
    return 0;
L_80294354:
    return 0;
L_8029435C:
    return 0;
L_80294364:
    return 0;
L_8029436C:
    return 0;
L_80294374:
    return 0;
L_8029437C:
    return 0;
L_80294384:
    return 0;
L_8029438C:
    return 1;
}

// Check if all save/load related resources are available
u32 chkAwardFlags99() {
    bool b = CtrlRemote_TouchBitByArg(0xa61);
    if (b) {} else return 0;
    b = CtrlRemote_TouchBitByArg(0xa7a);
    if (b) {} else return 0;
    b = CtrlRemote_TouchBitByArg(0xa60);
    if (b) {} else return 0;
    b = CtrlRemote_TouchBitByArg(0xa62);
    if (b) {} else return 0;
    b = CtrlRemote_TouchBitByArg(0xa63);
    if (b) {} else return 0;
    b = CtrlRemote_TouchBitByArg(0xa64);
    if (b) {} else return 0;
    b = CtrlRemote_TouchBitByArg(0xa65);
    if (b) {} else return 0;
    b = CtrlRemote_TouchBitByArg(0xaee);
    return b;
}



// sinit: static initializer for the global CfAward instance at lbl_eu_806649F8.
// Sets the manually-managed vtable pointers, runs the constructor body, and
// registers the atexit destructor.
extern "C" void sinit_802930E0() {
    lbl_eu_806649F8.mSecondBase = (void*)&lbl_eu_80538858;
    lbl_eu_806649F8.mVtbl = (char*)&lbl_eu_80538858 + 8;
    CtrlRemote_ResetSlotArrayObj(&lbl_eu_806649F8);
    __register_global_object(&lbl_eu_806649F8, (void*)__dt__Q22cf7CfAwardFv, (void*)lbl_eu_80576CF8);
}
#pragma pop
