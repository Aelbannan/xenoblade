#include "monolib/core/CException.hpp"
#include "monolib/core/CDesktop.hpp"
#include "monolib/core/CView.hpp"
#include "monolib/core/CDrawGX.hpp"
#include "monolib/device/CDeviceVI.hpp"
#include "monolib/device/CDeviceFont.hpp"
#include "monolib/device/CDeviceGX.hpp"
#include "monolib/device/CGXCache.hpp"
#include "monolib/lib/CLibHbm.hpp"
#include "monolib/lib/CLibCri.hpp"
#include "monolib/work/CWorkRoot.hpp"
#include "monolib/work/CWorkSystem.hpp"
#include "monolib/util/MemManager.hpp"
#include "monolib/math.hpp"
#include <nw4r/ut/ut_TextWriterBase.h>
#include <revolution/enc/encunicode.h>
#include <cstring>

struct CExceptionMsgParamView {
    void* vtable;
    CMsgParamEntry entries[8];
    CMsgParamEntry* array;
    u32 front;
    u32 size;
    u32 capacity;
    u32 field6;
    u32 field7;
};

static CExceptionMsgParamView* msgParam(CWorkThread* thread) {
    return (CExceptionMsgParamView*)((u8*)thread + 0x80);
}

static CMsgParamEntry* msgLast(CWorkThread* thread) {
    CExceptionMsgParamView* queue = msgParam(thread);
    return &queue->array[(queue->front + queue->field6) % queue->capacity];
}

extern "C" {
    // CProc constructor
    void __ct__5CProcFPCcP11CWorkThreads(CProc* self, const char* pName, CWorkThread* pParent, s16 capacity);
    
    // CException vtable
    extern void* lbl_eu_8056CCE0[];
    
    // Global exception instance pointer
    CException* lbl_eu_806656C0;
    
    // Global exception array and counter
    u32 lbl_eu_806656C4;
    CException* lbl_eu_80657B50[16];
    
    // Global state variables
    u32 lbl_eu_806656C8;
    u32 lbl_eu_806656CC;
    
    // Shared exception strings/data and external helpers.
    extern char lbl_eu_80522F7C[];
    extern char lbl_eu_8053785C[];
    extern char lbl_eu_80537828[];
    extern char lbl_eu_80537818[];
    extern char lbl_eu_805377E0[];
    extern char lbl_eu_80537734[];
    extern char lbl_eu_80537700[];
    extern char lbl_eu_805376EC[];
    extern char lbl_eu_805376B8[];
    extern char lbl_eu_8052DC70[];
    extern char lbl_eu_8052DC3C[];

    void func_804525D4__11CDeviceFontFv();
    void* func_eu_804558F4__11CDeviceFontFv(u32 index);
    void Panic__Q24nw4r2dbFPCciPCce(const char* file, int line, const char* fmt, ...);
    void func_800407C8(ml::CCol4* out, f32 r, f32 g, f32 b, f32 a);
    void __ct__7CDrawGXFv(CDrawGX* self);
    void __dt__7CDrawGXFv(CDrawGX* self, int shouldDelete);
    void setCol__7CDrawGXFRCQ22ml5CCol4(CDrawGX* self, const ml::CCol4* col);
    void renderRect__7CDrawGXFRCQ22ml7CRect16(CDrawGX* self, const ml::CRect16* rect);
    void SetTextColor__Q34nw4r2ut10CharWriterFQ34nw4r2ut5Color(void* writer, u32 color);
    void SetupGX__Q34nw4r2ut10CharWriterFv(void* writer);
    void Print__Q34nw4r2ut17TextWriterBaseFPCwi(void* writer, const wchar_t* text, int len);
    void func_80129F3C(void* writer, f32 a, f32 b);
    void func_8012B204(void* writer, f32 a);
    void* func_80457ED4__10CExceptionFv(CMsgParamEntry* entries, u32 value);
    void func_80458084__10CExceptionFv(const void* message);
    void func_80458B78__10CExceptionFv(void* writer, f32 x, f32 y, f32 z);
    void func_80458CBC__10CExceptionFv(void* writer, const wchar_t* text);

    // Device exception is an unnamed class in CDevice.cpp.
    void* CDeviceException_getInstance();

    // C-style allocation helpers used by the retail factory.
    mtl::ALLOC_HANDLE getWorkMem__17CWorkThreadSystemFv();
    void* allocate__Q23mtl10MemManagerFUlUl(u32 size, mtl::ALLOC_HANDLE handle);
    void entryWork__9CWorkUtilFP11CWorkThreadP11CWorkThreadb(CWorkThread* child,
                                                               CWorkThread* parent,
                                                               bool prepend);
    void wkSetEvent__11CWorkThreadFQ211CWorkThread3EVT(CWorkThread* self,
                                                        CWorkThread::EVT event);
    void setException__9CWorkRootFP10CException(CException* exception);
    CDesktop* getInstance__8CDesktopFv();
    CDesktop* getException__8CDesktopFv();
    CView* getView__8CDesktopFv();
    CView* getCurrentView__5CViewFv();
    GXRenderModeObj* getRenderModeObj__9CDeviceVIFv();
    void setFlag0__9CDeviceVIFb(bool state);
    void setFlag4__9CDeviceVIFb(bool state);
    void endFrame__9CDeviceVIFv();
    void beginFrame__9CDeviceVIFv();
    bool isOff__11CWorkSystemFv();
    void setAppException__8CDesktopFi(int state);
    CLibCri* getInstance__7CLibCriFv();
    void wkStandbyLogout__5CProcFv(CProc* self);
    bool wkStandbyLogin__5CProcFv(CProc* self);
    CException* getException__9CWorkRootFv();
    void func_8044A578__8CGXCacheFv(CGXCache* cache, const ml::CCol4* color, int flag);
    extern CGXCache* cacheInstance__9CDeviceGX;

    // Output rectangle/ring data helpers.
    void setRect__5CViewFRCQ22ml7CRect16(CView* self, const ml::CRect16* rect);
    CView* pssCreateView__5CProcFPCcP11CWorkThreadi(CProc* self, const char* name,
                                                     CWorkThread* thread, int arg);
    void* func_8045D478__7CLibHbmFv();
    bool isHbmControlInitialized__7CLibHbmFv();
}

// Constructor - extern "C" to match retail symbol name
extern "C" CException* __ct__CException(CException* self, const char* pName, CWorkThread* pParent) {
    __ct__5CProcFPCcP11CWorkThreads(self, pName, pParent, 8);
    *(void**)self = lbl_eu_8056CCE0;
    self->mExceptionCode = -1;
    self->mMessage = nullptr;
    self->mAlphaStep = 0.0f;
    self->mAlpha = 0.0f;
    self->mAnimState = 0;
    self->mException = nullptr;
    self->unk204 = 0;
    self->unk208 = 0;
    self->mFrameCounter = 0;
    self->mFlag210 = 0;
    self->mMaxExceptions = 5;
    self->mType = CWorkThread::THREAD_CEXCEPTION;
    lbl_eu_806656C0 = self;
    return self;
}

// Destructor
CException::~CException() {
    if (lbl_eu_806656C0 == this) {
        lbl_eu_806656C0 = nullptr;
    }
}

// Type check function
bool CException::func_80457C8C() {
    return mAlpha == 0.8f;
}

// Static factory function
CException* CException::func_80457CA4(CWorkThread* pThread, const wchar_t* message, u32 value) {
    CException* exception;
    CDesktop* desktop = getException__8CDesktopFv();

    if (isHbmControlInitialized__7CLibHbmFv()) {
        return nullptr;
    }

    exception = lbl_eu_806656C0;
    if (exception != nullptr) {
        if (exception->mMaxExceptions <= value) {
            return nullptr;
        }
        exception->mFlag210 = 1;
        wkSetEvent__11CWorkThreadFQ211CWorkThread3EVT(exception, CWorkThread::EVT_NONE);
    }

    if (desktop != nullptr) {
        CView* view = getView__8CDesktopFv();
        CWorkThread* parent = getException__8CDesktopFv();
        mtl::ALLOC_HANDLE handle = getWorkMem__17CWorkThreadSystemFv();
        CException* result = (CException*)allocate__Q23mtl10MemManagerFUlUl(0x218, handle);
        if (result != nullptr) {
            __ct__CException(result, lbl_eu_80522F7C, parent);
        }
        exception = result;
        if (exception != nullptr) {
            entryWork__9CWorkUtilFP11CWorkThreadP11CWorkThreadb(exception, parent, false);
            exception->unk1E4 = view->mWorkID;
            exception = exception->func_80457EB0();
        }
    } else {
        CWorkThread* parent = (CWorkThread*)CDeviceException_getInstance();
        mtl::ALLOC_HANDLE handle = getWorkMem__17CWorkThreadSystemFv();
        CException* result = (CException*)allocate__Q23mtl10MemManagerFUlUl(0x218, handle);
        if (result != nullptr) {
            __ct__CException(result, lbl_eu_80522F7C, parent);
        }
        exception = result;
        if (exception != nullptr) {
            entryWork__9CWorkUtilFP11CWorkThreadP11CWorkThreadb(exception, parent, false);
            exception = exception->func_80457EB0();
            setException__9CWorkRootFP10CException(exception);
        }
    }

    if (exception == nullptr) {
        return nullptr;
    }

    exception->mMaxExceptions = value;
    exception->mExceptionCode = pThread->mWorkID;
    exception->mMessage = message;

    CExceptionMsgParamView* queue = msgParam(pThread);
    CMsgParamEntry* source = msgLast(pThread);
    CMsgParamEntry* entry = (CMsgParamEntry*)func_80457ED4__10CExceptionFv(queue->entries, queue->field6);
    entry->command = CWorkThread::EVT_EXCEPTION;
    entry->wid = source->wid;
    entry->unk8 = source->unk8;
    entry->unkC = source->unkC;
    entry->unk10 = source->unk10;
    entry->unk14 = source->unk14;
    entry->unk18 = source->unk18;
    entry->unk1C = source->unk1C;
    entry->unk20 = source->unk20;
    entry->unk22 = source->unk22;
    entry->unk23 = 0;
    queue->field6++;
    pThread->unk1BC = queue->field6 - 1;

    entry = (CMsgParamEntry*)func_80457ED4__10CExceptionFv(queue->entries, pThread->unk1BC);
    entry->unk23 = 3;
    entry = (CMsgParamEntry*)func_80457ED4__10CExceptionFv(queue->entries, queue->field6 - 1);
    entry->wid = exception->mWorkID;
    return exception;
}

// Type validation
CException* CException::func_80457EB0() {
    if (this == nullptr) {
        return nullptr;
    }
    if (mType == THREAD_CEXCEPTION) {
        return this;
    }
    return nullptr;
}

// Ring buffer index calculation
extern "C" void* func_80457ED4__10CExceptionFv(CMsgParamEntry* entries, u32 value) {
    u32 offset = *(u32*)((u8*)entries + 0x124);
    u32 capacity = *(u32*)((u8*)entries + 0x12C);
    u32 index = offset + value;
    u32 remainder = index / capacity;
    remainder = index - remainder * capacity;
    return (u8*)*(u32*)((u8*)entries + 0x120) + remainder * 0x24;
}

// Render function
void CException::wkRender() {
    u32 state = mAnimState;
    if (state == 0) {
        mAlpha += mAlphaStep;
        if (mAlpha >= 0.8f) {
            mAlpha = 0.8f;
            mAnimState = state + 1;
        }
    } else if (state == 1 && (mFlags & 1) != 0) {
        mAlpha -= mAlphaStep;
        if (mAlpha <= 0.0f) {
            mAlpha = 0.0f;
            mAnimState = state + 1;
        }
    }

    CView* currentView = getCurrentView__5CViewFv();
    ml::CRect16 rect;
    if (currentView == nullptr) {
        GXRenderModeObj* mode = getRenderModeObj__9CDeviceVIFv();
        func_804525D4__11CDeviceFontFv();
        setFlag0__9CDeviceVIFb(false);
        setFlag4__9CDeviceVIFb(false);
        rect.set(0, 0, mode->fbWidth, mode->efbHeight);
    } else {
        rect.set(0, 0, currentView->mRectData.mViewSize.x,
                 currentView->mRectData.mViewSize.y);
    }

    CDrawGX draw;
    ml::CCol4 color;
    func_800407C8(&color, 0.0f, 0.0f, 0.0f, mAlpha);
    setCol__7CDrawGXFRCQ22ml5CCol4(&draw, &color);
    renderRect__7CDrawGXFRCQ22ml7CRect16(&draw, &rect);

    if (mAnimState < 1 || (mFlags & 1) == 0) {
        func_80458084__10CExceptionFv(mMessage);
    }
}

// Main exception text renderer.
extern "C" void func_80458084__10CExceptionFv(const void* message) {
    void* writer = (void*)func_eu_804558F4__11CDeviceFontFv(0);
    SetupGX__Q34nw4r2ut10CharWriterFv(writer);
    func_80458B78__10CExceptionFv(writer, 0.0f, 0.0f, 0.0f);
    func_80458CBC__10CExceptionFv(writer, (const wchar_t*)message);
}

// Set the nw4r TextWriter font pointer.
extern "C" void func_eu_8045C964__10CExceptionFv(void* writer, void* font) {
    *(void**)((u8*)writer + 0x48) = font;
}

// RGBA setter
extern "C" void func_80458B64__10CExceptionFv(u8* buffer, u8 r, u8 g, u8 b, u8 a) {
    buffer[0] = r;
    buffer[1] = g;
    buffer[2] = b;
    buffer[3] = a;
}

// Set a TextWriter cursor position.
extern "C" void func_80458B78__10CExceptionFv(void* writer, f32 x, f32 y, f32 z) {
    *(f32*)((u8*)writer + 0x2C) = x;
    *(f32*)((u8*)writer + 0x30) = y;
    *(f32*)((u8*)writer + 0x34) = z;
}

// Print a wide string through a TextWriter.
extern "C" void func_80458CBC__10CExceptionFv(void* writer, const wchar_t* text) {
    Print__Q34nw4r2ut17TextWriterBaseFPCwi(writer, text, (int)wcslen(text));
}

// Login setup
bool CException::wkStandbyLogin() {
    CView* desktopView = getView__8CDesktopFv();
    if (desktopView != nullptr) {
        CView* view = pssCreateView__5CProcFPCcP11CWorkThreadi(this, mName.c_str(), desktopView, 0);
        ml::CCol4 color;
        func_800407C8(&color, 0.0f, 0.0f, 0.0f, 1.0f);
        *(ml::CCol4*)((u8*)view + 0x444) = color;

        CExceptionMsgParamView* queue = msgParam(this);
        CMsgParamEntry* entry = (CMsgParamEntry*)func_80457ED4__10CExceptionFv(queue->entries, queue->field6);
        entry->command = CWorkThread::EVT_4;
        CMsgParamEntry* source = msgLast(this);
        entry->wid = source->wid;
        entry->unk8 = source->unk8;
        entry->unkC = source->unkC;
        entry->unk10 = source->unk10;
        entry->unk14 = source->unk14;
        entry->unk18 = source->unk18;
        entry->unk1C = source->unk1C;
        entry->unk20 = source->unk20;
        entry->unk22 = source->unk22;
        entry->unk23 = 0;
        queue->field6++;
        queue->field6--;
        view->mContextRingWriteIndex++;
        view->unk3FC = view->mContextRingWriteIndex - 1;
        GXRenderModeObj* mode = getRenderModeObj__9CDeviceVIFv();
        ml::CRect16 rect(0, 0, mode->fbWidth, mode->efbHeight);
        setRect__5CViewFRCQ22ml7CRect16(view, &rect);
        view->unk460 = 2;
    }

    mAlphaStep = 0.0f;
    mAlpha = 0.0f;
    mAnimState = 0;
    lbl_eu_806656C8 = unk208;
    setAppException__8CDesktopFi(1);
    CLibCri* cri = getInstance__7CLibCriFv();
    if (cri != nullptr) {
        wkSetEvent__11CWorkThreadFQ211CWorkThread3EVT(cri, CWorkThread::EVT_APPEXCEPTION_ON);
    }
    return CProc::wkStandbyLogin();
}

// Logout teardown
bool CException::wkStandbyLogout() {
    if (!mChildren.empty()) {
        return false;
    }
    if (mAlpha != 0.0f && !isOff__11CWorkSystemFv() && func_8045D478__7CLibHbmFv() == nullptr && mFlag210 == 0) {
        return false;
    }

    setAppException__8CDesktopFi(0);
    CLibCri* cri = getInstance__7CLibCriFv();
    if (cri != nullptr) {
        wkSetEvent__11CWorkThreadFQ211CWorkThread3EVT(cri, CWorkThread::EVT_APPEXCEPTION_OFF);
    }
    if (getException__9CWorkRootFv() == this) {
        setException__9CWorkRootFP10CException(nullptr);
    }
    return CProc::wkStandbyLogout();
}

// Infinite render loop
extern "C" void func_80459118__10CExceptionFv(const char* message) {
    CException** entries = lbl_eu_80657B50;
    u32 count = lbl_eu_806656C4;
    u32 index = 0;
    while (index < count) {
        CException* exception = entries[index];
        ((void (*)(CException*))(*(u32*)exception + 0xC))(exception);
        index++;
    }
    if (message == nullptr || message[0] == 0 || CDeviceVI::getInstance() == nullptr) {
        for (;;) {}
    }
    func_8044A578__8CGXCacheFv(cacheInstance__9CDeviceGX, &ml::CCol4::black, 1);
    lbl_eu_806656C8 = 0;
    for (;;) {
        endFrame__9CDeviceVIFv();
        beginFrame__9CDeviceVIFv();
        func_80458084__10CExceptionFv(message);
    }
}

// Add to global array
void CException::func_804591BC(IException* pException) {
    u32 idx = lbl_eu_806656C4;
    lbl_eu_80657B50[idx] = this;
    lbl_eu_806656C4 = idx + 1;
}

// Remove from global array
extern "C" void func_804591DC__10CExceptionFP10IException(CException* self) {
    u32 count = lbl_eu_806656C4;
    CException** entries = lbl_eu_80657B50;
    u32 index = 0;
    u32 byteOffset = 0;

    while (index < count) {
        if (entries[index] == self) {
            u32 last = count - 1;
            u32 shifts = last - index;
            u32 destination = index * 4;
            while (index < last) {
                u8* current = (u8*)entries + destination;
                *(u32*)current = *(u32*)(current + 4);
                destination += 4;
                index++;
            }
            lbl_eu_806656C4 = count - 1;
            return;
        }
        index++;
        byteOffset += 4;
    }
}

// Store this to global
void CException::func_8045925C() {
    if (lbl_eu_806656C0 != nullptr) {
        *(CException**)((u8*)lbl_eu_806656C0 + 0x1F0) = this;
    }
}
