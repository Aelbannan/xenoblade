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
#include <new>
namespace nw4r { namespace db { void Panic(const char* file, int line, const char* fmt, ...); } }

// Message ring header embedded in CWorkThread at +0x84 (the getMessageQueueSlot
// helper indexes array/front/capacity at +0x120/+0x124/+0x12C from here).
struct CWorkThreadMsgQueue {
    u8 pad[0x120];            // +0x84..0x1A4
    CMsgParamEntry* array;    // +0x1A4
    u32 front;                // +0x1A8
    u32 size;                 // +0x1AC
    u32 capacity;             // +0x1B0
    u32 lastIndex;            // +0x1B4
};

extern "C" {
    // C++ delete helper for the extern "C" deleting dtor.

    // CProc constructor
    void __ct__5CProcFPCcP11CWorkThreads(CProc* self, const char* pName, CWorkThread* pParent, s16 capacity);
    
    extern u32 sExceptionDrawCount;
    
    // Shared exception strings/data and external helpers.
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

    void getFontId__11CDeviceFontFv();
    void* findFontByDevice__11CDeviceFontFv(u32 index);
    void* getFontInfo__11CDeviceFontFUlPQ34nw4r3lyt6Layout(u32 index, nw4r::lyt::Layout* layout);
    ml::CCol4* CTaskGame_setVec4(ml::CCol4* out, f32 r, f32 g, f32 b, f32 a);
    void __ct__7CDrawGXFv(CDrawGX* self);
    void __dt__7CDrawGXFv(CDrawGX* self, int shouldDelete);
    void setCol__7CDrawGXFRCQ22ml5CCol4(CDrawGX* self, const ml::CCol4* col);
    void renderRect__7CDrawGXFRCQ22ml7CRect16(CDrawGX* self, const ml::CRect16* rect);
    void SetTextColor__Q34nw4r2ut10CharWriterFQ34nw4r2ut5Color(void* writer, u32 color);
    void SetupGX__Q34nw4r2ut10CharWriterFv(void* writer);
    void Print__Q34nw4r2ut17TextWriterBaseFPCwi(void* writer, const wchar_t* text, int len);
    void TagWriterSetScale(void* writer, f32 a, f32 b);
    void TagWriterSetCharSpace(void* writer, f32 a);
    void* getMessageQueueSlot__10CExceptionFv(CMsgParamEntry* entries, u32 value);
    void drawExceptionText__10CExceptionFv(const void* message);
    void setWriterCursor__10CExceptionFv(u8* writer, f32 x, f32 y, f32 z);
    void printExceptionMessage__10CExceptionFv(u8* writer, const wchar_t* text);

    // Device exception is an unnamed class in CDevice.cpp.
    void* CDeviceException_getInstance();

    // C-style allocation helpers used by the retail factory.
    mtl::ALLOC_HANDLE getWorkMem__17CWorkThreadSystemFv();
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
    bool wkStandbyLogout__5CProcFv(CProc* self);
    bool wkStandbyLogin__5CProcFv(CProc* self);
    void __dt__5CProcFv(CProc* self, int flag);
    CException* getException__9CWorkRootFv();
    void updateCmd2Color__8CGXCacheFv(CGXCache* cache, const ml::CCol4* color, int flag);
    extern CGXCache* cacheInstance__9CDeviceGX;

    // Output rectangle/ring data helpers.
    void setRect__5CViewFRCQ22ml7CRect16(CView* self, const ml::CRect16* rect);
    CView* pssCreateView__5CProcFPCcP11CWorkThreadi(CProc* self, const char* name,
                                                     CWorkThread* thread, int arg);
    void* isHbmStopPending__7CLibHbmFv();
    bool isHbmActive__7CLibHbmFv();
    bool isHbmControlInitialized__7CLibHbmFv();
}

// Shared .sdata2 float constants (retail pool; owned by another TU's range).
extern const f32 lbl_eu_8066A480;  // 0.0f
extern const f32 lbl_eu_8066A484;  // 0.8f

CException::CException(const char* pName, CWorkThread* pParent)
    : CProc(pName, pParent, 8) {
    mExceptionCode = -1;
    mMessage = nullptr;
    mAlphaStep = lbl_eu_8066A480;
    mAlpha = lbl_eu_8066A480;
    mAnimState = 0;
    mException = nullptr;
    unk204 = 0;
    unk208 = 0;
    mFrameCounter = 0;
    mFlag210 = 0;
    mMaxExceptions = 5;
    mType = CWorkThread::THREAD_CEXCEPTION;
    spExceptionInstance = this;
}

CException::~CException() {
    if (spExceptionInstance == this) {
        spExceptionInstance = 0;
    }
}

// Type check function
bool CException::isFadedIn() {
    extern const f32 lbl_eu_8066A484;
    return lbl_eu_8066A484 == mAlpha;
}

// Static factory function
CException* CException::createException(CWorkThread* pThread, const wchar_t* message,
                                      u32 value) {
    CException* exception;
    if (isHbmActive__7CLibHbmFv()) {
        return nullptr;
    }

    exception = spExceptionInstance;
    if (exception != nullptr) {
        if (exception->mMaxExceptions <= value) {
            return nullptr;
        }
        exception->mFlag210 = 1;
        wkSetEvent__11CWorkThreadFQ211CWorkThread3EVT(exception, CWorkThread::EVT_NONE);
    }

    CException* result;
    if (getException__8CDesktopFv() != nullptr) {
        CView* view = getView__8CDesktopFv();
        CWorkThread* parent = getException__8CDesktopFv();
        mtl::ALLOC_HANDLE handle = getWorkMem__17CWorkThreadSystemFv();
        result = (CException*)mtl::MemManager::allocate(0x218, handle);
        if (result != nullptr) {
            new (result) CException("CException\0\0CLibHbm\0CLibG3d\0CLibLayout\0CLibVM\0CLibStaticData\0\203\215\203\117\203\101\203\105\203\147\202\311\216\270\224\163\202\265\202\334\202\265\202\275\0CLib\0", parent);
        }
        entryWork__9CWorkUtilFP11CWorkThreadP11CWorkThreadb(result, parent, false);
        result->unk1E4 = view->mWorkID;
        exception = result->validateExceptionType();
    } else {
        CWorkThread* parent = (CWorkThread*)CDeviceException_getInstance();
        mtl::ALLOC_HANDLE handle = getWorkMem__17CWorkThreadSystemFv();
        result = (CException*)mtl::MemManager::allocate(0x218, handle);
        if (result != nullptr) {
            new (result) CException("CException\0\0CLibHbm\0CLibG3d\0CLibLayout\0CLibVM\0CLibStaticData\0\203\215\203\117\203\101\203\105\203\147\202\311\216\270\224\163\202\265\202\334\202\265\202\275\0CLib\0", parent);
        }
        entryWork__9CWorkUtilFP11CWorkThreadP11CWorkThreadb(result, parent, false);
        exception = result->validateExceptionType();
        setException__9CWorkRootFP10CException(exception);
    }

    if (exception == nullptr) {
        return nullptr;
    }

    exception->mMaxExceptions = value;
    exception->mExceptionCode = pThread->mWorkID;
    exception->mMessage = message;

    // Push an EVT_EXCEPTION copy of the newest queue entry onto the thread's
    // message ring, then tag the new tail and link it to the exception.
    CWorkThreadMsgQueue* queue =
        reinterpret_cast<CWorkThreadMsgQueue*>((u8*)pThread + 0x84);
    u32 sum = queue->front + queue->size;
    u32 idx = sum % queue->capacity;
    CMsgParamEntry* slot =
        reinterpret_cast<CMsgParamEntry*>((u8*)queue->array + idx * 0x24);
    CMsgParamEntry msg;
    *slot = msg;
    slot->command = CWorkThread::EVT_EXCEPTION;
    slot->unk23 = 0;
    queue->size += 1;
    queue->lastIndex = queue->size - 1;

    CMsgParamEntry* tail1 = (CMsgParamEntry*)getMessageQueueSlot__10CExceptionFv(
        (CMsgParamEntry*)queue, queue->lastIndex);
    tail1->unk23 = 3;
    CMsgParamEntry* tail2 = (CMsgParamEntry*)getMessageQueueSlot__10CExceptionFv(
        (CMsgParamEntry*)queue, queue->lastIndex);
    tail2->wid = exception->mWorkID;
    return exception;
}

// Type validation
CException* CException::validateExceptionType() {
    if (this == nullptr) {
        return nullptr;
    }
    if (mType == THREAD_CEXCEPTION) {
        return this;
    }
    return nullptr;
}

// Ring buffer index calculation
extern "C" void* getMessageQueueSlot__10CExceptionFv(CMsgParamEntry* entries, u32 value) {
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

    // The CFontLayer subobject of the current view (implicit base upcast).
    CFontLayer* view = CView::getCurrentView();
    int width = 0;
    int height = 0;
    GXRenderModeObj* mode = CDeviceVI::getRenderModeObj();
    width = mode->fbWidth;
    mode = CDeviceVI::getRenderModeObj();
    height = mode->efbHeight;
    if (view == nullptr) {
        getFontId__11CDeviceFontFv();
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
    draw.setCol(*CTaskGame_setVec4(&color, lbl_eu_8066A480, lbl_eu_8066A480, lbl_eu_8066A480, mAlpha));
    ml::CRect16 rect;
    rect.mPos.x = 0;
    rect.mPos.y = 0;
    rect.mSize.x = width;
    rect.mSize.y = height;
    draw.renderRect(rect);

    if (mAnimState >= 1 && (mFlags & 1) != 0) {
        return;
    }
    drawExceptionText__10CExceptionFv(mMessage);
}

// Main exception text renderer.
extern "C" void drawExceptionText__10CExceptionFv(const void* message) {
    u8* writer = (u8*)findFontByDevice__11CDeviceFontFv(0);
    SetupGX__Q34nw4r2ut10CharWriterFv(writer);
    setWriterCursor__10CExceptionFv(writer, lbl_eu_8066A480, lbl_eu_8066A480, lbl_eu_8066A480);
    printExceptionMessage__10CExceptionFv(writer, (const wchar_t*)message);
}

// Set the nw4r TextWriter font pointer.
extern "C" void setWriterFont__10CExceptionFv(u8* writer, void* font) {
    // Validate the writer pointer against the Wii memory map (MEM1, MEM2,
    // hardware IO registers); panic if it falls outside every known range.
    bool writerRegs2 = true;
    bool writerRegs = true;
    bool writerIo2 = true;
    bool writerIo = true;
    bool writerMem2 = true;
    bool writerMem1 = true;
    u32 writerTop = (u32)writer & 0xFF000000;

    if (writerTop != 0x80000000 || ((u32)writer & 0xFF800000) != 0x81000000) {
        writerMem1 = false;
    }
    if (!writerMem1 && ((u32)writer & 0xF8000000) != 0x90000000) {
        writerMem2 = false;
    }
    if (!writerMem2 && writerTop != 0xC0000000) {
        writerIo = false;
    }
    if (!writerIo && ((u32)writer & 0xFF800000) != 0xC1000000) {
        writerIo2 = false;
    }
    if (!writerIo2 && ((u32)writer & 0xF8000000) != 0xD0000000) {
        writerRegs = false;
    }
    if (!writerRegs && ((u32)writer & 0xFFFFC000) != 0xE0000000) {
        writerRegs2 = false;
    }
    if (!writerRegs2) {
        nw4r::db::Panic(lbl_eu_8053785C, 0x41, lbl_eu_80537828, writer);
    }

    // Same range validation for the font pointer.
    bool fontRegs2 = true;
    bool fontRegs = true;
    bool fontIo2 = true;
    bool fontIo = true;
    bool fontMem2 = true;
    bool fontMem1 = true;
    u32 fontTop = (u32)font & 0xFF000000;
    if (fontTop != 0x80000000 || ((u32)font & 0xFF800000) != 0x81000000) {
        fontMem1 = false;
    }
    if (!fontMem1 && ((u32)font & 0xF8000000) != 0x90000000) {
        fontMem2 = false;
    }
    if (!fontMem2 && fontTop != 0xC0000000) {
        fontIo = false;
    }
    if (!fontIo && ((u32)font & 0xFF800000) != 0xC1000000) {
        fontIo2 = false;
    }
    if (!fontIo2 && ((u32)font & 0xF8000000) != 0xD0000000) {
        fontRegs = false;
    }
    if (!fontRegs && ((u32)font & 0xFFFFC000) != 0xE0000000) {
        fontRegs2 = false;
    }
    if (!fontRegs2) {
        nw4r::db::Panic(lbl_eu_80537818, 0x42, lbl_eu_805377E0, font);
    }
    *(void**)((u8*)writer + 0x48) = font;
}

// RGBA setter
extern "C" void setExceptionColor__10CExceptionFv(u8* buffer, u8 r, u8 g, u8 b, u8 a) {
    buffer[0] = r;
    buffer[1] = g;
    buffer[2] = b;
    buffer[3] = a;
}

// Set a TextWriter cursor position.
extern "C" void setWriterCursor__10CExceptionFv(u8* writer, f32 x, f32 y, f32 z) {
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
        nw4r::db::Panic(lbl_eu_805378A0, 0x102, lbl_eu_8053786C, writer);
    }
    *(f32*)((u8*)writer + 0x2C) = x;
    *(f32*)((u8*)writer + 0x30) = y;
    *(f32*)((u8*)writer + 0x34) = z;
}

// Print a wide string through a TextWriter.
extern "C" void printExceptionMessage__10CExceptionFv(u8* writer, const wchar_t* text) {
    bool writerRegs2 = true;
    bool writerRegs = true;
    bool writerIo2 = true;
    bool writerIo = true;
    bool writerMem2 = true;
    bool writerMem1 = true;
    u32 writerTop = (u32)writer & 0xFF000000;
    if (writerTop != 0x80000000 && ((u32)writer & 0xFF800000) != 0x81000000) {
        writerMem1 = false;
    }
    if (!writerMem1 && ((u32)writer & 0xF8000000) != 0x90000000) {
        writerMem2 = false;
    }
    if (!writerMem2 && writerTop != 0xC0000000) {
        writerIo = false;
    }
    if (!writerIo && ((u32)writer & 0xFF800000) != 0xC1000000) {
        writerIo2 = false;
    }
    if (!writerIo2 && ((u32)writer & 0xF8000000) != 0xD0000000) {
        writerRegs = false;
    }
    if (!writerRegs && ((u32)writer & 0xFFFFC000) != 0xE0000000) {
        writerRegs2 = false;
    }
    if (!writerRegs2) {
        nw4r::db::Panic(lbl_eu_80537734, 0x100, lbl_eu_80537700, writer);
    }
    bool textRegs2 = true;
    bool textRegs = true;
    bool textIo2 = true;
    bool textIo = true;
    bool textMem2 = true;
    bool textMem1 = true;
    u32 textTop = (u32)text & 0xFF000000;
    if (textTop != 0x80000000 && ((u32)text & 0xFF800000) != 0x81000000) {
        textMem1 = false;
    }
    if (!textMem1 && ((u32)text & 0xF8000000) != 0x90000000) {
        textMem2 = false;
    }
    if (!textMem2 && textTop != 0xC0000000) {
        textIo = false;
    }
    if (!textIo && ((u32)text & 0xFF800000) != 0xC1000000) {
        textIo2 = false;
    }
    if (!textIo2 && ((u32)text & 0xF8000000) != 0xD0000000) {
        textRegs = false;
    }
    if (!textRegs && ((u32)text & 0xFFFFC000) != 0xE0000000) {
        textRegs2 = false;
    }
    if (!textRegs2) {
        nw4r::db::Panic(lbl_eu_805376EC, 0x101, lbl_eu_805376B8, text);
    }
    ((nw4r::ut::TextWriterBase<wchar_t>*)writer)->Print(text, (int)wcslen(text));
}

// Login setup
bool CException::wkStandbyLogin() {
    if (getView__8CDesktopFv() != nullptr) {
        CView* view = pssCreateView__5CProcFPCcP11CWorkThreadi(this, mName.c_str(),
                                                               getView__8CDesktopFv(), 0);
        // CTaskGame_setVec4 fills a color plus a template message entry (the
        // retail callee writes 0x34 bytes); the entry bytes seed the ring push.
        struct {
            ml::CCol4 color;
            CMsgParamEntry entry;
        } local;
        CTaskGame_setVec4(reinterpret_cast<ml::CCol4*>(&local), lbl_eu_8066A480,
                      lbl_eu_8066A480, lbl_eu_8066A480, 1.0f);
        view->unk444 = *(ml::CVec4*)&local.color;

        // Enqueue an EVT_4 message into the view's context ring.
        u32 sum = view->mContextMsg.mRing.mFront + view->mContextMsg.mRing.mSize;
        u32 idx = sum % view->mContextMsg.mRing.mCapacity;
        CMsgParamEntry* slot = reinterpret_cast<CMsgParamEntry*>(
            (u8*)view->mContextMsg.mRing.mArrayPtr + idx * 0x24);
        *slot = local.entry;
        slot->command = CWorkThread::EVT_4;
        slot->unk23 = 0;
        view->mContextMsg.mRing.mSize += 1;
        view->mContextMsg.mRing.field6 = view->mContextMsg.mRing.mSize - 1;

        GXRenderModeObj* mode = getRenderModeObj__9CDeviceVIFv();
        s16 height = mode->efbHeight;
        mode = getRenderModeObj__9CDeviceVIFv();
        ml::CRect16 rect(0, 0, mode->fbWidth, height);
        setRect__5CViewFRCQ22ml7CRect16(view, &rect);
        view->unk460 = 2;
    }

    mAlphaStep = 0.04f;
    mAlpha = 0.04f;
    mAnimState = 0;
    sExceptionState = unk208;
    setAppException__8CDesktopFi(1);
    if (CLibCri::getInstance() != nullptr) {
        wkSetEvent__11CWorkThreadFQ211CWorkThread3EVT(CLibCri::getInstance(),
                                                      CWorkThread::EVT_APPEXCEPTION_ON);
    }
    return wkStandbyLogin__5CProcFv(this);
}

// Logout teardown
bool CException::wkStandbyLogout() {
    // Child-list header pointer at 0x60; empty when its back-link points at itself.
    // Proceed only when fully faded out or one of the override conditions holds.
    CDoubleListNode* header = *(CDoubleListNode**)((u8*)this + 0x60);
    if (header->GetPrev() == header &&
        (lbl_eu_8066A480 == mAlpha || isOff__11CWorkSystemFv() ||
         isHbmStopPending__7CLibHbmFv() != nullptr || mFlag210 != 0)) {
        setAppException__8CDesktopFi(0);
        if (CLibCri::getInstance() != nullptr) {
            wkSetEvent__11CWorkThreadFQ211CWorkThread3EVT(CLibCri::getInstance(),
                                                          CWorkThread::EVT_APPEXCEPTION_OFF);
        }
        if (getException__9CWorkRootFv() == this) {
            setException__9CWorkRootFP10CException(nullptr);
        }
        return wkStandbyLogout__5CProcFv(this);
    }
    return false;
}

// Infinite render loop
void CException::logExceptionMessage(const wchar_t* message) {
    int index = 0;
    while (index < (int)sExceptionCount) {
        IException* exception = sRegisteredExceptions[index];
        exception->render();
        index++;
    }
    // Retail tests only the first byte of the wide string.
    if (message == nullptr || *reinterpret_cast<const char*>(message) == 0 ||
        CDeviceVI::getInstance() == nullptr) {
        for (;;) {}
    }
    updateCmd2Color__8CGXCacheFv(cacheInstance__9CDeviceGX, &ml::CCol4::black, 1);
    sExceptionState = 0;
    for (;;) {
        endFrame__9CDeviceVIFv();
        beginFrame__9CDeviceVIFv();
        drawExceptionText__10CExceptionFv(message);
    }
}

// Add to global array
void CException::AddToGlobalArray(IException* pException) {
    sRegisteredExceptions[sExceptionCount++] = reinterpret_cast<IException*>(this);
}

// Remove from global array
void CException::RemoveFromGlobalArray(IException* pException) {
    int count = sExceptionCount;
    for (int index = 0; index < count; ++index) {
        if (sRegisteredExceptions[index] == pException) {
            // Shift the tail entries down one slot.
            for (int shift = index; shift < count - 1; ++shift) {
                sRegisteredExceptions[shift] = sRegisteredExceptions[shift + 1];
            }
            sExceptionCount--;
            return;
        }
    }
}

// Store this to global
void CException::setGlobalMessage() {
    if (spExceptionInstance != nullptr) {
        *(CException**)((u8*)spExceptionInstance + 0x1F0) = this;
    }
}

// ===== Dissolved monolibdata2 data owned by this TU =====
// The primary vtable, RTTI locator and RTTI base list are emitted by MWCC
// from the real CException class hierarchy (CProc -> CWorkThread -> IWorkEvent).
// Retail ranges: .data 0x8056CCE0-0x8056CDA0, .rodata 0x80522F70-0x80522FD8,
// .sdata 0x80663780-0x80663788, .bss 0x80657B50-0x80657FD8,
// .sbss 0x806656C0-0x806656E0.

// [.rodata] 0x80522F88-0x80522FD8 (0x50): class-name / SJIS message pool
// + "CLib".  (0x80522F70 is the compiler-emitted RTTI class name.)
// [.bss] 0x80657B50-0x80657FD8 (0x488, align 8): exception array + work buffer.
extern "C" {
IException* sRegisteredExceptions[16];   // 0x40
u8   sExceptionMessageBuffer[0x400];  // name-guess: UTF-16 conversion destination in drawExceptionText (0x8045C1D8-0x8045C204)
u32  lbl_eu_80657F90[8];           // 0x20
u32  lbl_eu_80657FB0[3];           // 0xC
u32  lbl_eu_80657FBC[3];           // 0xC
u32  lbl_eu_80657FC8[4];           // 0x10
}

// [.sbss] 0x806656C0-0x806656E0 (0x20, align 8): small global state.
// 0x18 is CLibCri::spInstance (defined below; retail .sbss owner is this TU),
// 0x1C is CLibCri's AX callback pointer (imported by CLibCri.cpp).
extern "C" {
CException* spExceptionInstance;       // 0x0
u32  sExceptionCount;              // 0x4
u32  sExceptionState;              // 0x8
u32  sExceptionDrawCount;          // name-guess: incremented after message conversion (0x8045C218), bit 7 read at 0x8045C7A0
u64  lbl_eu_806656D0;              // 0x10 (8B)
u32  lbl_eu_806656DC;              // 0x1C
}
CLibCri* CLibCri::spInstance;      // 0x18
