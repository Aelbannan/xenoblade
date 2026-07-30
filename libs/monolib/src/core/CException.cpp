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
#include "monolib/math.hpp"
#include <cstring>

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
    
    // UTF16 buffer for text conversion
    wchar_t lbl_eu_80657B90[0x200];
    
    // External functions
    int ENCConvertStringUtf8ToUtf16(wchar_t* dst, const char* src, int maxLen, int flags);
    void* func_eu_804558F4__11CDeviceFontFv(int r3);
    void func_804525D4__11CDeviceFontFv();
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
    self->mGameException = nullptr;
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
CException* CException::func_80457CA4(CWorkThread* pThread, const wchar_t* message, u32 r5) {
    return nullptr; // stub
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
void* CException::func_80457ED4(u32 r4) {
    u32 base = *(u32*)((u8*)this + 0x120);
    u32 offset = *(u32*)((u8*)this + 0x124);
    u32 divisor = *(u32*)((u8*)this + 0x12C);
    u32 idx = (offset + r4) % divisor;
    return (void*)(base + idx * 0x24);
}

// Render function
void CException::wkRender() {
    // stub
}

// Large text rendering function
void CException::func_80458084(const wchar_t* message) {
    // stub
}

// Layout setter
void CException::func_eu_8045C964(void* pLayout) {
    // stub
}

// RGBA setter
void CException::func_80458B64(u8* buffer, u8 r, u8 g, u8 b, u8 a) {
    buffer[0] = r;
    buffer[1] = g;
    buffer[2] = b;
    buffer[3] = a;
}

// Position setter
void CException::func_80458B78(float x, float y, float z) {
    // stub
}

// Text printer
void CException::func_80458CBC(const wchar_t* text) {
    // stub
}

// Login setup
bool CException::wkStandbyLogin() {
    return CProc::wkStandbyLogin();
}

// Logout teardown
bool CException::wkStandbyLogout() {
    return CProc::wkStandbyLogout();
}

// Infinite render loop
void CException::func_80459118(const char* message) {
    // stub
}

// Add to global array
void CException::func_804591BC(IException* pException) {
    u32 idx = lbl_eu_806656C4;
    lbl_eu_80657B50[idx] = this;
    lbl_eu_806656C4 = idx + 1;
}

// Remove from global array
void CException::func_804591DC(IException* pException) {
    u32 count = lbl_eu_806656C4;
    CException** arr = lbl_eu_80657B50;
    u32 i = 0;
    u32 offset = 0;
    
    while (i < count) {
        if (*(CException**)((u8*)arr + offset) == this) {
            u32 last = count - 1;
            u32 numShifts = last - i;
            u32 shiftOffset = i * 4;
            for (u32 j = 0; j < numShifts; j++) {
                *(u32*)((u8*)arr + shiftOffset) = *(u32*)((u8*)arr + shiftOffset + 4);
                shiftOffset += 4;
            }
            lbl_eu_806656C4 = count - 1;
            return;
        }
        i++;
        offset += 4;
    }
}

// Store this to global
void CException::func_8045925C() {
    if (lbl_eu_806656C0 != nullptr) {
        *(CException**)((u8*)lbl_eu_806656C0 + 0x1F0) = this;
    }
}
