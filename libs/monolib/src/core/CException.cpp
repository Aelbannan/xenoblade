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
#include <nw4r/lyt/lyt_layout.h>
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
    // C++ delete helper for the extern "C" deleting dtor.
    void* __dl__FPv(void* p);

    // CProc constructor
    void __ct__5CProcFPCcP11CWorkThreads(CProc* self, const char* pName, CWorkThread* pParent, s16 capacity);
    
    // CException vtable (defined in the data block below)
    extern u32 lbl_eu_8056CCE0[40];
    
    // Global exception instance pointer
    extern CException* lbl_eu_806656C0;
    
    // Global exception array and counter
    extern u32 lbl_eu_806656C4;
    extern CException* lbl_eu_80657B50[16];
    
    // Global state variables
    extern u32 lbl_eu_806656C8;
    extern u32 lbl_eu_806656CC;
    
    // Shared exception strings/data and external helpers.
    extern const char lbl_eu_80522F7C[];
    extern char lbl_eu_8053785C[];
    extern char lbl_eu_80537828[];
    extern char lbl_eu_805378A0[];
    extern char lbl_eu_8053786C[];
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
    void* func_80452C10__11CDeviceFontFUlPQ34nw4r3lyt6Layout(u32 index, nw4r::lyt::Layout* layout);
    void Panic__Q24nw4r2dbFPCciPCce(const char* file, int line, const char* fmt, ...);
    ml::CCol4* func_800407C8(ml::CCol4* out, f32 r, f32 g, f32 b, f32 a);
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
    void func_80458B78__10CExceptionFv(u8* writer, f32 x, f32 y, f32 z);
    void func_80458CBC__10CExceptionFv(u8* writer, const wchar_t* text);

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
    bool func_8045DE00__7CLibHbmFv();
    bool isHbmControlInitialized__7CLibHbmFv();
}

// Shared .sdata2 float constants (retail pool; owned by another TU's range).
extern const f32 lbl_eu_8066A480;  // 0.0f
extern const f32 lbl_eu_8066A484;  // 0.8f
extern const f32 lbl_eu_8066A488;  // 1.0f

// Constructor - extern "C" to match retail symbol name
extern "C" CException* __ct__CException(CException* self, const char* pName, CWorkThread* pParent) {
    extern const f32 lbl_eu_8066A480;
    __ct__5CProcFPCcP11CWorkThreads(self, pName, pParent, 8);
    *(void**)self = lbl_eu_8056CCE0;
    self->mExceptionCode = -1;
    self->mMessage = nullptr;
    self->mAlphaStep = lbl_eu_8066A480;
    self->mAlpha = lbl_eu_8066A480;
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

// Destructor - extern "C" free-function form (CFontLayer/CScnBlend pattern):
// the retail vtable references the deleting dtor symbol __dt__10CExceptionFv,
// and a plain C++ member dtor would auto-emit __vt__/__RTTI__ bloat here.
extern "C" void* __dt__10CExceptionFv(CException* self, int flag) {
    if (self != 0) {
        if (lbl_eu_806656C0 == self) {
            lbl_eu_806656C0 = 0;
        }
        if (flag > 0) {
            __dl__FPv(self);
        }
    }
    return self;
}

// Type check function
extern "C" bool func_80457C8C__10CExceptionFv(CException* self) {
    extern const f32 lbl_eu_8066A484;
    return lbl_eu_8066A484 == self->mAlpha;
}

// Static factory function
CException* CException::func_80457CA4(CWorkThread* pThread, const wchar_t* message, u32 value) {
    CException* exception;
    if (func_8045DE00__7CLibHbmFv()) {
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

    CDesktop* desktop = getException__8CDesktopFv();
    if (desktop != nullptr) {
        CView* view = getView__8CDesktopFv();
        CWorkThread* parent = getException__8CDesktopFv();
        mtl::ALLOC_HANDLE handle = getWorkMem__17CWorkThreadSystemFv();
        CException* result = (CException*)allocate__Q23mtl10MemManagerFUlUl(0x218, handle);
        if (result != nullptr) {
            __ct__CException(result, lbl_eu_80522F7C, parent);
        }
        exception = result;
        entryWork__9CWorkUtilFP11CWorkThreadP11CWorkThreadb(exception, parent, false);
        exception->unk1E4 = view->mWorkID;
        exception = exception->func_80457EB0();
    } else {
        CWorkThread* parent = (CWorkThread*)CDeviceException_getInstance();
        mtl::ALLOC_HANDLE handle = getWorkMem__17CWorkThreadSystemFv();
        CException* result = (CException*)allocate__Q23mtl10MemManagerFUlUl(0x218, handle);
        if (result != nullptr) {
            __ct__CException(result, lbl_eu_80522F7C, parent);
        }
        exception = result;
        entryWork__9CWorkUtilFP11CWorkThreadP11CWorkThreadb(exception, parent, false);
        exception = exception->func_80457EB0();
        setException__9CWorkRootFP10CException(exception);
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
    s32 state = mAnimState;
    if (state == 0) {
        mAlpha += mAlphaStep;
        if (mAlpha >= lbl_eu_8066A484) {
            mAlpha = lbl_eu_8066A484;
            mAnimState = state + 1;
        }
    } else if (state == 1) {
        if ((mFlags & 1) != 0) {
            mAlpha -= mAlphaStep;
            if (mAlpha <= lbl_eu_8066A480) {
                mAlpha = lbl_eu_8066A480;
                mAnimState = state + 1;
            }
        }
    }

    CView* currentView = CView::getCurrentView();
    CView* view = currentView;
    if (view != nullptr) {
        view = (CView*)((u8*)view + 0x1C4);
    }
    u16 width = 0;
    u16 height = 0;
    GXRenderModeObj* mode = CDeviceVI::getRenderModeObj();
    width = mode->fbWidth;
    mode = CDeviceVI::getRenderModeObj();
    height = mode->efbHeight;
    if (view == nullptr) {
        func_804525D4__11CDeviceFontFv();
        CDeviceVI::setFlag0(false);
        CDeviceVI::setFlag4(false);
    } else {
        CView* current = CView::getCurrentView();
        width = current->mRectData.mViewSize.x;
        current = CView::getCurrentView();
        height = current->mRectData.mViewSize.y;
    }

    CDrawGX draw;
    ml::CCol4 color;
    draw.setCol(*func_800407C8(&color, lbl_eu_8066A480, lbl_eu_8066A480, lbl_eu_8066A480, mAlpha));
    ml::CRect16 rect;
    rect.mPos.x = 0;
    rect.mPos.y = 0;
    rect.mSize.x = width;
    rect.mSize.y = height;
    draw.renderRect(rect);

    if (mAnimState >= 1 && (mFlags & 1) != 0) {
        return;
    }
    func_80458084__10CExceptionFv(mMessage);
}

// Main exception text renderer.
#pragma dont_inline on
extern "C" void func_80458084__10CExceptionFv(const void* message) {
    u8* writer = (u8*)func_eu_804558F4__11CDeviceFontFv(0);
    SetupGX__Q34nw4r2ut10CharWriterFv(writer);
    func_80458B78__10CExceptionFv(writer, lbl_eu_8066A480, lbl_eu_8066A480, lbl_eu_8066A480);
    func_80458CBC__10CExceptionFv(writer, (const wchar_t*)message);
}
#pragma dont_inline reset

// Set the nw4r TextWriter font pointer.
extern "C" void func_eu_8045C964__10CExceptionFv(u8* writer, void* font) {
    u32 writerAddress = (u32)writer;
    bool writerMem1 = true;
    bool writerMem2 = true;
    bool writerIo = true;
    bool writerIo2 = true;
    bool writerRegs = true;
    bool writerRegs2 = true;

    if ((writerAddress & 0xFF000000) != 0x80000000 &&
        (writerAddress & 0xFF800000) != 0x81000000) {
        writerMem1 = false;
    }
    if (!writerMem1 && (writerAddress & 0xF8000000) != 0x90000000) {
        writerMem2 = false;
    }
    if (!writerMem2 && (writerAddress & 0xFF000000) != 0xC0000000) {
        writerIo = false;
    }
    if (!writerIo && (writerAddress & 0xFF800000) != 0xC1000000) {
        writerIo2 = false;
    }
    if (!writerIo2 && (writerAddress & 0xF8000000) != 0xD0000000) {
        writerRegs = false;
    }
    if (!writerRegs && (writerAddress & 0xFFFFC000) != 0xE0000000) {
        writerRegs2 = false;
    }
    if (!writerRegs2) {
        Panic__Q24nw4r2dbFPCciPCce(lbl_eu_8053785C, 0x41, lbl_eu_80537828, writer);
    }

    u32 fontAddress = (u32)font;
    bool fontMem1 = true;
    bool fontMem2 = true;
    bool fontIo = true;
    bool fontIo2 = true;
    bool fontRegs = true;
    bool fontRegs2 = true;
    if ((fontAddress & 0xFF000000) != 0x80000000 &&
        (fontAddress & 0xFF800000) != 0x81000000) {
        fontMem1 = false;
    }
    if (!fontMem1 && (fontAddress & 0xF8000000) != 0x90000000) {
        fontMem2 = false;
    }
    if (!fontMem2 && (fontAddress & 0xFF000000) != 0xC0000000) {
        fontIo = false;
    }
    if (!fontIo && (fontAddress & 0xFF800000) != 0xC1000000) {
        fontIo2 = false;
    }
    if (!fontIo2 && (fontAddress & 0xF8000000) != 0xD0000000) {
        fontRegs = false;
    }
    if (!fontRegs && (fontAddress & 0xFFFFC000) != 0xE0000000) {
        fontRegs2 = false;
    }
    if (!fontRegs2) {
        Panic__Q24nw4r2dbFPCciPCce(lbl_eu_80537818, 0x42, lbl_eu_805377E0, font);
    }
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
extern "C" void func_80458B78__10CExceptionFv(u8* writer, f32 x, f32 y, f32 z) {
    u32 address = (u32)writer;
    bool validMem1 = true;
    bool validMem2 = true;
    bool validIo = true;
    bool validIo2 = true;
    bool validRegs = true;
    bool validRegs2 = true;
    if ((address & 0xFF000000) != 0x80000000 &&
        (address & 0xFF800000) != 0x81000000) {
        validMem1 = false;
    }
    if (!validMem1 && (address & 0xF8000000) != 0x90000000) {
        validMem2 = false;
    }
    if (!validMem2 && (address & 0xFF000000) != 0xC0000000) {
        validIo = false;
    }
    if (!validIo && (address & 0xFF800000) != 0xC1000000) {
        validIo2 = false;
    }
    if (!validIo2 && (address & 0xF8000000) != 0xD0000000) {
        validRegs = false;
    }
    if (!validRegs && (address & 0xFFFFC000) != 0xE0000000) {
        validRegs2 = false;
    }
    if (!validRegs2) {
        Panic__Q24nw4r2dbFPCciPCce(lbl_eu_805378A0, 0x102, lbl_eu_8053786C, writer);
    }
    *(f32*)((u8*)writer + 0x2C) = x;
    *(f32*)((u8*)writer + 0x30) = y;
    *(f32*)((u8*)writer + 0x34) = z;
}

// Print a wide string through a TextWriter.
extern "C" void func_80458CBC__10CExceptionFv(u8* writer, const wchar_t* text) {
    u32 writerAddress = (u32)writer;
    bool writerMem1 = true;
    bool writerMem2 = true;
    bool writerIo = true;
    bool writerIo2 = true;
    bool writerRegs = true;
    bool writerRegs2 = true;
    if ((writerAddress & 0xFF000000) != 0x80000000 &&
        (writerAddress & 0xFF800000) != 0x81000000) {
        writerMem1 = false;
    }
    if (!writerMem1 && (writerAddress & 0xF8000000) != 0x90000000) {
        writerMem2 = false;
    }
    if (!writerMem2 && (writerAddress & 0xFF000000) != 0xC0000000) {
        writerIo = false;
    }
    if (!writerIo && (writerAddress & 0xFF800000) != 0xC1000000) {
        writerIo2 = false;
    }
    if (!writerIo2 && (writerAddress & 0xF8000000) != 0xD0000000) {
        writerRegs = false;
    }
    if (!writerRegs && (writerAddress & 0xFFFFC000) != 0xE0000000) {
        writerRegs2 = false;
    }
    if (!writerRegs2) {
        Panic__Q24nw4r2dbFPCciPCce(lbl_eu_80537734, 0x100, lbl_eu_80537700, writer);
    }

    u32 textAddress = (u32)text;
    bool textMem1 = true;
    bool textMem2 = true;
    bool textIo = true;
    bool textIo2 = true;
    bool textRegs = true;
    bool textRegs2 = true;
    if ((textAddress & 0xFF000000) != 0x80000000 &&
        (textAddress & 0xFF800000) != 0x81000000) {
        textMem1 = false;
    }
    if (!textMem1 && (textAddress & 0xF8000000) != 0x90000000) {
        textMem2 = false;
    }
    if (!textMem2 && (textAddress & 0xFF000000) != 0xC0000000) {
        textIo = false;
    }
    if (!textIo && (textAddress & 0xFF800000) != 0xC1000000) {
        textIo2 = false;
    }
    if (!textIo2 && (textAddress & 0xF8000000) != 0xD0000000) {
        textRegs = false;
    }
    if (!textRegs && (textAddress & 0xFFFFC000) != 0xE0000000) {
        textRegs2 = false;
    }
    if (!textRegs2) {
        Panic__Q24nw4r2dbFPCciPCce(lbl_eu_805376EC, 0x101, lbl_eu_805376B8, text);
    }
    Print__Q34nw4r2ut17TextWriterBaseFPCwi(writer, text, (int)wcslen(text));
}

// Login setup
bool CException::wkStandbyLogin() {
    if (getView__8CDesktopFv() != nullptr) {
        CView* view = pssCreateView__5CProcFPCcP11CWorkThreadi(this, mName.c_str(),
                                                               getView__8CDesktopFv(), 0);
        ml::CCol4 color;
        func_800407C8(&color, lbl_eu_8066A480, lbl_eu_8066A480, lbl_eu_8066A480, lbl_eu_8066A488);
        *(ml::CCol4*)((u8*)view + 0x444) = color;

        CMsgParam<10>& messages =
            *reinterpret_cast<CMsgParam<10>*>(&view->mContextMsgVtable);
        messages.enqueue(CWorkThread::EVT_4);
        GXRenderModeObj* mode = getRenderModeObj__9CDeviceVIFv();
        ml::CRect16 rect(0, 0, mode->fbWidth, mode->efbHeight);
        setRect__5CViewFRCQ22ml7CRect16(view, &rect);
        view->unk460 = 2;
    }

    mAlphaStep = lbl_eu_8066A480;
    mAlpha = lbl_eu_8066A480;
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
    CDoubleListNode* sentinel = *(CDoubleListNode**)((u8*)this + 0x60);
    if (sentinel->mNext == sentinel) {
        extern const f32 lbl_eu_8066A480;
        f32 alpha = mAlpha;
        if (lbl_eu_8066A480 != alpha &&
            (isOff__11CWorkSystemFv() == false &&
             func_8045D478__7CLibHbmFv() == nullptr && mFlag210 == 0)) {
            return false;
        }
    } else {
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
    CException** exceptions = lbl_eu_80657B50;
    for (u32 index = 0; index < count; ++index) {
        if (exceptions[index] == self) {
            u32 last = count - 1;
            for (u32 shift = index; shift < last; ++shift) {
                exceptions[shift] = exceptions[shift + 1];
            }
            lbl_eu_806656C4 = last;
            return;
        }
    }
}

// Store this to global
void CException::func_8045925C() {
    if (lbl_eu_806656C0 != nullptr) {
        *(CException**)((u8*)lbl_eu_806656C0 + 0x1F0) = this;
    }
}

// ===== Dissolved monolibdata2 (blob surgery) data owned by this TU =====
// Retail ranges: .data 0x8056CCE0-0x8056CDA0, .rodata 0x80522F70-0x80522FD8,
// .sdata 0x80663780-0x80663788, .bss 0x80657B50-0x80657FD8,
// .sbss 0x806656C0-0x806656E0. All reloc names below are spellable, so no
// UNIT_RULES postprocess is required.

namespace CException_RTTI {
extern "C" void* __RTTI__10IWorkEvent;
extern "C" void* __RTTI__11CWorkThread;
extern "C" void* __RTTI__5CProc;
}

extern "C" int WorkEvent1__10IWorkEventFPvPCc(void*, const char*);
extern "C" int OnFileEvent__10IWorkEventFP10CEventFile(void*);
extern "C" int WorkEvent3__10IWorkEventFPv(void*);
extern "C" int WorkEvent4__10IWorkEventFv();
extern "C" void OnPauseTrigger__10IWorkEventFb(int);
extern "C" int WorkEvent6__10IWorkEventFv();
extern "C" int WorkEvent7__10IWorkEventFv();
extern "C" int WorkEvent8__10IWorkEventFv();
extern "C" int WorkEvent9__10IWorkEventFv();
extern "C" int WorkEvent10__10IWorkEventFv();
extern "C" int WorkEvent11__10IWorkEventFv();
extern "C" int WorkEvent12__10IWorkEventFv();
extern "C" int WorkEvent13__10IWorkEventFv();
extern "C" int WorkEvent14__10IWorkEventFv();
extern "C" int WorkEvent15__10IWorkEventFv();
extern "C" int WorkEvent16__10IWorkEventFv();
extern "C" int WorkEvent17__10IWorkEventFv();
extern "C" int WorkEvent18__10IWorkEventFv();
extern "C" int WorkEvent19__10IWorkEventFv();
extern "C" int WorkEvent20__10IWorkEventFv();
extern "C" int WorkEvent21__10IWorkEventFv();
extern "C" int WorkEvent22__10IWorkEventFv();
extern "C" int WorkEvent23__10IWorkEventFv();
extern "C" int WorkEvent24__10IWorkEventFv();
extern "C" int WorkEvent25__10IWorkEventFv();
extern "C" int WorkEvent26__10IWorkEventFv();
extern "C" int WorkEvent27__10IWorkEventFv();
extern "C" int WorkEvent28__10IWorkEventFv();
extern "C" int WorkEvent29__10IWorkEventFv();
extern "C" int WorkEvent30__10IWorkEventFv();
extern "C" void WorkEvent31__10IWorkEventFv();
extern "C" void wkUpdate__11CWorkThreadFv();
extern "C" void wkRender__10CExceptionFv();
extern "C" void wkRenderAfter__11CWorkThreadFv();
extern "C" void wkStandbyLogin__10CExceptionFv();
extern "C" void wkStandbyLogout__10CExceptionFv();
extern "C" void wkStandbyExceptionRetry__11CWorkThreadFUl(unsigned int);

// forward decls (defined in the data block below)
extern "C" const char lbl_eu_80522F70[];
extern "C" const char lbl_eu_80522F88[];
extern "C" u32 lbl_eu_8056CD80[];

// [.sdata] 0x80663780-0x80663788 (8B): RTTI locator {name "CException", class-info}.
extern "C" u32 lbl_eu_80663780[2] = {
    (u32)&lbl_eu_80522F70, (u32)&lbl_eu_8056CD80,
};

// [.data] 0x8056CCE0-0x8056CD80 (0xA0): CException primary vtable.
extern "C" u32 lbl_eu_8056CCE0[40] = {
    // IWorkEventVtbl
    (u32)&lbl_eu_80663780, 0, (u32)&__dt__10CExceptionFv,
    (u32)&WorkEvent1__10IWorkEventFPvPCc, (u32)&OnFileEvent__10IWorkEventFP10CEventFile,
    (u32)&WorkEvent3__10IWorkEventFPv, (u32)&WorkEvent4__10IWorkEventFv,
    (u32)&OnPauseTrigger__10IWorkEventFb,
    (u32)&WorkEvent6__10IWorkEventFv, (u32)&WorkEvent7__10IWorkEventFv,
    (u32)&WorkEvent8__10IWorkEventFv, (u32)&WorkEvent9__10IWorkEventFv,
    (u32)&WorkEvent10__10IWorkEventFv, (u32)&WorkEvent11__10IWorkEventFv,
    (u32)&WorkEvent12__10IWorkEventFv, (u32)&WorkEvent13__10IWorkEventFv,
    (u32)&WorkEvent14__10IWorkEventFv, (u32)&WorkEvent15__10IWorkEventFv,
    (u32)&WorkEvent16__10IWorkEventFv, (u32)&WorkEvent17__10IWorkEventFv,
    (u32)&WorkEvent18__10IWorkEventFv, (u32)&WorkEvent19__10IWorkEventFv,
    (u32)&WorkEvent20__10IWorkEventFv, (u32)&WorkEvent21__10IWorkEventFv,
    (u32)&WorkEvent22__10IWorkEventFv, (u32)&WorkEvent23__10IWorkEventFv,
    (u32)&WorkEvent24__10IWorkEventFv, (u32)&WorkEvent25__10IWorkEventFv,
    (u32)&WorkEvent26__10IWorkEventFv, (u32)&WorkEvent27__10IWorkEventFv,
    (u32)&WorkEvent28__10IWorkEventFv, (u32)&WorkEvent29__10IWorkEventFv,
    (u32)&WorkEvent30__10IWorkEventFv, (u32)&WorkEvent31__10IWorkEventFv,
    (u32)&wkUpdate__11CWorkThreadFv, (u32)&wkRender__10CExceptionFv,
    (u32)&wkRenderAfter__11CWorkThreadFv, (u32)&wkStandbyLogin__10CExceptionFv,
    (u32)&wkStandbyLogout__10CExceptionFv, (u32)&wkStandbyExceptionRetry__11CWorkThreadFUl,
};

// [.data] 0x8056CD80-0x8056CDA0 (0x20): RTTI class-info (IWorkEvent /
// CWorkThread / CProc base RTTI chain).
extern "C" u32 lbl_eu_8056CD80[8] = {
    (u32)&CException_RTTI::__RTTI__10IWorkEvent, 0,
    (u32)&CException_RTTI::__RTTI__11CWorkThread, 0,
    (u32)&CException_RTTI::__RTTI__5CProc, 0, 0, 0,
};

// [.rodata] 0x80522F70-0x80522FD8 (0x68): "CException" x2 + class-name / SJIS
// message pool + "CLib". Defined as byte arrays (with the retail inter-string
// padding folded into each so offsets stay exact).
extern "C" const char lbl_eu_80522F70[0xC] = {
    0x43,0x45,0x78,0x63,0x65,0x70,0x74,0x69,0x6F,0x6E,0x00,0x00,
};  // "CException\0" + 1B pad
extern "C" const char lbl_eu_80522F7C[0xC] = {
    0x43,0x45,0x78,0x63,0x65,0x70,0x74,0x69,0x6F,0x6E,0x00,0x00,
};  // "CException\0" + 1B pad
extern "C" const char lbl_eu_80522F88[0x50] = {
    0x43,0x4C,0x69,0x62,0x48,0x62,0x6D,0x00,  // "CLibHbm\0"
    0x43,0x4C,0x69,0x62,0x47,0x33,0x64,0x00,  // "CLibG3d\0"
    0x43,0x4C,0x69,0x62,0x4C,0x61,0x79,0x6F,0x75,0x74,0x00,  // "CLibLayout\0"
    0x43,0x4C,0x69,0x62,0x56,0x4D,0x00,  // "CLibVM\0"
    0x43,0x4C,0x69,0x62,0x53,0x74,0x61,0x74,0x69,0x63,0x44,0x61,0x74,0x61,0x00,  // "CLibStaticData\0"
    0x83,0x8D,0x83,0x4F,0x83,0x41,0x83,0x45,0x83,0x67,0x82,0xC9,0x8E,0xB8,0x94,0x73,
    0x82,0xB5,0x82,0xDC,0x82,0xB5,0x82,0xBD,0x00,  // \u30e1\u30e2\u30ea\u304c\u2026\u306a\u304f\u3057\u307e\u3057\u305f
    0x43,0x4C,0x69,0x62,0x00,0x00,  // "CLib\0" + 1B pad
};
DECOMP_FORCEACTIVE(CException_cpp, lbl_eu_80522F88);

// [.bss] 0x80657B50-0x80657FD8 (0x488, align 8): exception array + work buffer.
extern "C" {
CException* lbl_eu_80657B50[16];   // 0x40
u8   lbl_eu_80657B90[0x400];       // 0x400
u32  lbl_eu_80657F90[8];           // 0x20
u32  lbl_eu_80657FB0[3];           // 0xC
u32  lbl_eu_80657FBC[3];           // 0xC
u32  lbl_eu_80657FC8[4];           // 0x10
}

// [.sbss] 0x806656C0-0x806656E0 (0x20, align 8): small global state.
extern "C" {
CException* lbl_eu_806656C0;       // 0x0
u32  lbl_eu_806656C4;              // 0x4
u32  lbl_eu_806656C8;              // 0x8
u32  lbl_eu_806656CC;              // 0xC
u64  lbl_eu_806656D0;              // 0x10 (8B)
u32  lbl_eu_806656D8;              // 0x18
u32  lbl_eu_806656DC;              // 0x1C
}
