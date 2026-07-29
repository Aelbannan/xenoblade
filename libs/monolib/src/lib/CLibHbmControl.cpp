#include "monolib/lib/CLibHbmControl.hpp"
#include "monolib/lib/CLibHbm.hpp"
#include "monolib/core.hpp"
#include "monolib/work.hpp"
#include "monolib/device.hpp"
#include <revolution/WPAD.h>
#include <revolution/mtx/mtx44.h>
#include <revolution/os/OSFastCast.h>
#include <cstring>

// Extern retail data symbols (sdata2 float constants)
extern float lbl_eu_8066A578;
extern float lbl_eu_8066A57C;
extern float lbl_eu_8066A588;
extern float lbl_eu_8066A58C;
extern float lbl_eu_8066A590;
extern float lbl_eu_8066A594;
extern float lbl_eu_8066A598;
extern float lbl_eu_8066A59C;
extern float lbl_eu_8066A5A0;

// Extern retail function stubs
void func_8043EA88__5CViewFRQ22ml5CRectP5CView(ml::CRect16* rect, CView* view);

CLibHbmControl::CLibHbmControl(const char* pName, CWorkThread* pParent) : CProc(pName, pParent, MAX_CHILD),
mHbmPhase(0),
mWaitTimer(0) {
    spInstance = this;
    mType = THREAD_CLIBHBMCONTROL;
    std::memset(&mHBMControllerData, 0, sizeof(HBMControllerData));
}

CLibHbmControl::~CLibHbmControl(){
    spInstance = nullptr;
}

CLibHbmControl* CLibHbmControl::getInstance(){
    return spInstance;
}

bool CLibHbmControl::isActive(){
    CLibHbmControl* hbmControl = spInstance;

    if(hbmControl == nullptr) return false;
    return hbmControl->mHbmPhase == 3;
}

// Advances the HBM (Home Button Menu) state machine each frame.
// Phase 0: load arc file   → 1
// Phase 1: wait for file I/O, countdown timer, then → 2 (or back to 0 on error)
// Phase 2: init HBM system  → 3
// Phase 3: active — polls WPAD controllers, processes HBM button selections
void CLibHbmControl::wkUpdate(){
    switch(mHbmPhase){
        case 0:
            CLibHbm::loadHbmArcFile();
            mWaitTimer = 0xD2;
            mHbmPhase++;
            break;
        case 1:
            if(!CDeviceFileCri::getInstance()->isException()){
                mWaitTimer--;
                if(mWaitTimer <= 0){
                    wkSetEvent(EVT_NONE);
                }

                break;
            }

            wkSetEvent(EVT_NONE);

            if(CLibHbm::checkFlag6()){
                mHbmPhase = 0;
            }else if(CLibHbm::isHbmMemPointerValid()){
                mHbmPhase++;
            }
            break;
        case 2:
            CLibHbm::initHbm();
            mHbmPhase++;
            break;
        case 3:
            for(int i = 0; i < WPAD_MAX_CONTROLLERS; i++){
                CWpadStatus* wpadStatus = CDeviceRemotePad::getWpadStatus(i);

            }
            break;
        default:
            break;
    }

    HBMUpdateSound();

    HBMSelectBtnNum selectBtnNum = HBMCalc(&mHBMControllerData);

    switch(selectBtnNum){
        case HBM_SELECT_HOMEBTN:
            wkSetEvent(EVT_NONE);
            break;
        case HBM_SELECT_BTN1:
            returnToWiiMenu(false);
            break;
        case HBM_SELECT_BTN2:
            resetGame(false);
            break;
        default:
            break;
    }
}

void CLibHbmControl::wkRender(){
    // Begin scene via CGXCache
    CDeviceGX::getCacheInstance()->func_8044BE38();

    // Set up CDrawGX for full-screen rect
    CDrawGX draw;
    draw.func_80456570(0);
    draw.func_8045657C(0);

    // Set white vertex color (r,g,b = 1.0, alpha from sdata2)
    ml::CCol4 col;
    col.r = lbl_eu_8066A578;
    col.g = lbl_eu_8066A578;
    col.b = lbl_eu_8066A578;
    col.a = lbl_eu_8066A588;
    draw.setCol(col);

    // Begin drawing quads
    draw.begin(9, 1);

    // Get current view's screen rectangle and add it to draw
    CView* view = CView::getCurrentView();
    ml::CRect16 rect;
    func_8043EA88__5CViewFRQ22ml5CRectP5CView(&rect, view);
    draw.add(rect);

    draw.end();

    // If HBM is active (phase 3), set up GX and render the HBM
    if (CLibHbmControl::isActive()) {
        GXClearVtxDesc();
        GXSetVtxAttrFmt(GX_VTXFMT4, GX_VA_POS, GX_POS_XY, GX_F32, 0);
        GXSetVtxAttrFmt(GX_VTXFMT4, GX_VA_CLR0, GX_CLR_RGBA, GX_RGBA8, 0);
        GXSetVtxDesc(GX_VA_POS, GX_DIRECT);
        GXSetVtxDesc(GX_VA_CLR0, GX_DIRECT);
        GXSetNumChans(1);
        GXSetNumTexGens(0);
        GXSetNumTevStages(1);
        GXSetTevOrder(GX_TEVSTAGE0, GX_TEXCOORD_NULL, GX_TEXMAP_NULL, GX_COLOR0A0);
        GXSetTevOp(GX_TEVSTAGE0, GX_PASSCLR);
        GXSetBlendMode(GX_BM_NONE, GX_BL_ZERO, GX_BL_ZERO, GX_LO_CLEAR);
        GXSetZMode(GX_TRUE, GX_LEQUAL, GX_TRUE);
        GXSetCurrentMtx(GX_PNMTX0);

        // Build orthographic projection matrix
        GXRenderModeObj* rmode = CDeviceVI::getRenderModeObj();
        Mtx44 mtx;

        if (CDeviceVI::isWideAspectRatio()) {
            float f7 = (float)rmode->efbHeight / lbl_eu_8066A59C;
            C_MTXOrtho(mtx,
                       lbl_eu_8066A590 * lbl_eu_8066A57C,
                       lbl_eu_8066A590 * (-lbl_eu_8066A57C),
                       lbl_eu_8066A5A0 * (-f7),
                       lbl_eu_8066A5A0 * f7,
                       lbl_eu_8066A578,
                       lbl_eu_8066A598);
        } else {
            float f7 = lbl_eu_8066A598 / lbl_eu_8066A58C;
            C_MTXOrtho(mtx,
                       lbl_eu_8066A590 * lbl_eu_8066A57C,
                       lbl_eu_8066A590 * (-lbl_eu_8066A57C),
                       lbl_eu_8066A594 * (-f7),
                       lbl_eu_8066A594 * f7,
                       lbl_eu_8066A578,
                       lbl_eu_8066A598);
        }

        GXSetProjection(mtx, GX_ORTHOGRAPHIC);

        // Initialize fast-cast GQRs for HBM's psq_* instructions
        OSInitFastCast();

        HBMDraw();
    }

    // End scene
    CDeviceGX::getCacheInstance()->func_8044BE38();
    CViewRoot::func_80442DA8();
}

static const char sCLibHbmControlName[] = "CLibHbmControl";

CLibHbmControl* CLibHbmControl::create() {
    CLibHbm* parent = CLibHbm::getInstance();
    const char* name = sCLibHbmControlName;
    WORK_ID id = CWorkThreadSystem::getWorkMem();
    CLibHbmControl* hbmControl = (CLibHbmControl*)mtl::MemManager::allocate(sizeof(CLibHbmControl), id);

    new (hbmControl) CLibHbmControl(name, parent);

    CWorkUtil::entryWork(hbmControl, parent, false);
    hbmControl->unk1E4 = CDesktop::getView()->mWorkID;
    return hbmControl;
}

bool CLibHbmControl::wkStandbyLogin() {
    // Locals corresponding to callee-saved registers used in the context ring push.
    // Declared early so the compiler assigns them to r25-r31 and emits _savegpr_25.
    s32 saved_r25, saved_r26, saved_r27, saved_r28;
    s32 saved_r29, saved_r30, saved_r31;

    CView* view = CDesktop::getView();
    CView* subView = pssCreateView(mName.c_str(), view, 0);

    // Context ring push: compute entry position
    s32 sum = subView->unk3F0 + subView->mContextRingWriteIndex;
    s32 product = (sum / (s32)subView->mContextRingCapacity) * (s32)subView->mContextRingCapacity;
    s32 remainder = sum - product;
    s32 offset = remainder * (s32)sizeof(CViewContextRingEntry);

    // Write context ring entry with saved register state
    CViewContextRingEntry* entry = (CViewContextRingEntry*)((u8*)subView->mContextRingBase + offset);
    entry->tag = 4;
    ((u32*)entry->payload)[0] = saved_r25;
    ((u32*)entry->payload)[1] = saved_r26;
    ((u32*)entry->payload)[2] = saved_r27;
    ((u32*)entry->payload)[3] = saved_r28;
    ((u32*)entry->payload)[4] = saved_r29;
    ((u32*)entry->payload)[5] = saved_r30;
    ((u32*)entry->payload)[6] = saved_r31;
    entry->unk54 = 0;
    entry->unk56Hi = 0;
    entry->pad = 0;

    subView->mContextRingWriteIndex++;
    subView->unk3FC = subView->mContextRingWriteIndex - 1;
    subView->unk278 |= 8;
    subView->unk460 = 2;

    // Copy this->mName to subView's own mName (CView::mName at 0x400)
    subView->mName.mLength = strlen(mName.c_str());
    strcpy(subView->mName.mString, mName.c_str());

    // If the inherited CWorkThread::mName is empty, copy to it too
    if (subView->CWorkThread::mName.mLength == 0) {
        subView->CWorkThread::mName.mLength = strlen(mName.c_str());
        strcpy(subView->CWorkThread::mName.mString, mName.c_str());
    }

    // Set view rect to full screen
    GXRenderModeObj* rmode = CDeviceVI::getRenderModeObj();
    s16 fbWidth = rmode->fbWidth;
    s16 efbHeight = rmode->efbHeight;

    ml::CRect16 rect;
    rect.mPos.x = 0;
    rect.mPos.y = 0;
    rect.mSize.x = fbWidth;
    rect.mSize.y = efbHeight;
    subView->setRect(rect);

    // Set position to zero (four floats loaded from sdata)
    float zero = 0.0f;
    subView->unk444.x = zero;
    subView->unk444.y = zero;
    subView->unk444.z = zero;
    subView->unk444.w = zero;

    CWorkControl::pause(true);
    return CProc::wkStandbyLogin();
}


bool CLibHbmControl::wkStandbyLogout() {
    if (mChildren.empty()) {
        CLibHbm::deleteHbm();
        CWorkControl::pause(false);
        return CProc::wkStandbyLogout();
    }
    return false;
}
