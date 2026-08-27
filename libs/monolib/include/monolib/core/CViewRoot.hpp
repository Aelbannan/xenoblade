#pragma once

#include <revolution/GX.h>
#include "monolib/monolib_types.hpp"
#include "monolib/util/reslist.hpp"
#include "monolib/work/CWorkThread.hpp"

class CProc;
class CView;
class CDesktop;
class CViewFrame;

struct CViewRootPool {
    _reslist_node<CWorkThread*> mSentinel;
    u8 pad0[0x100 - sizeof(_reslist_node<CWorkThread*>)];
    _reslist_node<CWorkThread*>* mStartNodePtr;
    _reslist_node<CWorkThread*>* mList;
    u32 mUsed;
    s32 mCapacity;
};

class __declspec(novtable) CViewRoot : public CWorkThread {
public:
    CViewRoot(const char* pName, CWorkThread* pParent);
    virtual ~CViewRoot();

    static CViewRoot* create(CWorkThread* pParent);
    static CViewRoot* getInstance();
    static CView* getCurrent();
    static bool isCurrent(const CView* view);
    static bool isCurrentChild(const CView* view, const CView* current);
    static bool isInitialized();
    static void destroyProc(CProc* pProc);
    static void setCurrent(CView* view);
    static void invalidCurrent(CView* view);
    static void updateCache();
    static void renderView();
    static CView* getFullScreenView();
    static CView* getView(WORK_ID id);
    void func_80442B54(void* a, void* b, void* c);
    void func_80442C68();

    virtual bool wkStandbyLogin();
    virtual bool wkStandbyLogout();

    CViewRootPool mPool0; //0x1C4
    CViewRootPool mPool1; //0x2D4
    CViewRootPool mPool2; //0x3E4
    reslist<WORK_ID> mViewHistory; //0x4F4
    CView* mCurrentView; //0x514
    CProc* mAttachedProc0; //0x518
    CProc* mAttachedProc1; //0x51C
};

// C-linkage imports (retail symbol names - keep linkage/signatures verbatim)
extern "C" {
extern CViewRoot* lbl_eu_806655D0;
extern u8 lbl_eu_806655D4;
extern const char lbl_eu_8052266C[];
extern u32 lbl_eu_8056B710[0xA0 / 4];
extern char lbl_eu_8056B280[];
extern char lbl_eu_8056B298[];
mtl::ALLOC_HANDLE getWorkMem__17CWorkThreadSystemFv();
void* allocate__Q23mtl10MemManagerFUlUl(u32 size, mtl::ALLOC_HANDLE handle);
void* allocate_array__Q23mtl10MemManagerFUlUl(u32 size, mtl::ALLOC_HANDLE handle);
void __ct__11CWorkThreadFPCcP11CWorkThreadi(CWorkThread* ths, const char* name, CWorkThread* parent, int cap);
void entryWork__9CWorkUtilFP11CWorkThreadP11CWorkThreadb(CWorkThread* ths, CWorkThread* parent, bool prepend);
CWorkThread* getWorkThread__9CWorkUtilFUl(WORK_ID wid);
CProc* pssGetRoot__5CProcFP5CProc(CProc* proc);
CView* getView__8CDesktopFv();
CDesktop* getInstance__8CDesktopFv();
GXRenderModeObj* getRenderModeObj__9CDeviceVIFv();
void getFrame2ViewOffset__10CViewFrameFR7CRect16PC10CViewFrame(ml::CRect16* rect, const CViewFrame* frame);
void renderView__5CViewFv(CView* view);
}
extern "C" void func_8044B298__8CGXCacheFv(void* self, void* a, void* b, void* c);
