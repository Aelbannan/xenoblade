#pragma once

#include <types.h>

#include "monolib/util/reslist.hpp"

class CDeviceFontLayer;
class CException;

namespace nw4r {
namespace lyt {
class Layout;
}
} // namespace nw4r

// IDeviceFontInfo - font-info provider interface (impls: CDeviceFontInfoRom /
// CDeviceFontInfoExt). Retail vtable (e.g. lbl_eu_8056C740 for Ext) is the
// RTTI layout: RTTI ptr @0x0, 0 @0x4, virtual dtor @0x8, then the user
// virtuals in declaration order (0xC identity, ... 0x34 availability).
class IDeviceFontInfo {
public:
    virtual ~IDeviceFontInfo();  // 0x8
    virtual s32 getFlags(); // 0xC (identity/query)
    virtual u32 getState(); // 0x10
    virtual u32 getMode(); // 0x14
    virtual u32 getBufferSize(); // 0x18
    virtual u16 getLineHeight(); // 0x1C
    virtual void func_80453468();// 0x20
    virtual void* getFont(); // 0x24
    virtual void* getFontConst(); // 0x28
    virtual void advanceState();// 0x2C
    virtual void initState();// 0x30
    virtual u32 isStateNonZero(); // 0x34 (availability: returns v != 0)
    virtual u32 isStateReady();// 0x38
};

// reslist<IDeviceFontInfo> node layout (12 bytes): mNext@0, mPrev@4, item@8.
struct CDeviceFontInfoListNode {
    CDeviceFontInfoListNode* mNext;  // 0x0
    CDeviceFontInfoListNode* mPrev;  // 0x4
    IDeviceFontInfo* mItem;          // 0x8
};

// _reslist_base<IDeviceFontInfo> layout (0x20 bytes).
struct CDeviceFontInfoReslist {
    u32* m_vtable;                          // 0x0 (points at lbl_eu_8056C734)
    CDeviceFontInfoListNode* mStartNodePtr; // 0x4
    CDeviceFontInfoListNode mStartNode;     // 0x8 (embedded sentinel node)
    CDeviceFontInfoListNode* mList;         // 0x14
    int mCapacity;                          // 0x18
    u8 field_0x1C;                          // 0x1C (externally-owned flag)
};

// Font-info provider impls (typing only; the real layouts live in their own
// units). Both share the IDeviceFontInfo vtable layout.
class CDeviceFontInfoExt : public IDeviceFontInfo {
public:
    static CDeviceFontInfoExt* create();  // static factory (retail name)
};

class CDeviceFontInfoRom : public IDeviceFontInfo {
public:
    static CDeviceFontInfoRom* create();  // static factory (retail name)
};

// Minimal CWorkThread view (CDeviceFileDvd.hpp pattern): same class name and
// same-signature methods so the base ctor/dtor/login/logout calls resolve to
// the retail symbols. Declared non-virtual here so MWCC emits no vtable of
// its own, and the ctor can store the retail vtable manually.
class CWorkThread {
public:
    CWorkThread(const char* pName, CWorkThread* pParent, int capacity);
    ~CWorkThread();
    bool wkStandbyLogin();
    bool wkStandbyLogout();

    u8 field_0x0[0x54];                    // 0x0 (vtable + name + state/id/type)
    mtl::ALLOC_HANDLE mAllocHandle;        // 0x54
    CWorkThread* mParent;                  // 0x58
    reslist<CDeviceFontLayer*> mChildren;  // 0x5C (child work threads)
    u8 field_0x7C[0x1C4 - 0x7C];           // 0x7C (flags/msgqueue/base tail)
};

// Minimal singletons used by wkStandbyLogout (work/CLib systems).
class CWorkSystem {
public:
    static CWorkSystem* getInstance();
};

class CLib {
public:
    static CLib* getInstance();
};

// CDevice::isColdStartReady() (called from wkStandbyLogin).
class CDevice {
public:
    static bool isColdStartReady();
};

// CWorkRoot::setException/getException (called from wkStandbyLogin/wkUpdate).
class CWorkRoot {
public:
    static void setException(CException* pException);
    static CException* getException();
};

// CDeviceBase - intermediate base (CDeviceFile.cpp pattern). The inline
// ctor forwards to CWorkThread and clears mFlags (0x1C4); the inline empty
// dtor, when inlined into the derived dtor, contributes the redundant
// null-check before the CWorkThread dtor call that retail keeps.
class CDeviceBase : public CWorkThread {
public:
    CDeviceBase(const char* pName, CWorkThread* pParent, int capacity)
        : CWorkThread(pName, pParent, capacity) {
        mFlags = 0;
    }
    ~CDeviceBase() {}

    u32 mFlags; // 0x1C4
};

// Minimal CDeviceFile view: wkUpdate only needs the singleton and its init
// state. (The full header cannot be included here - it would clash with this
// TU's minimal CWorkThread/CDeviceBase views.)
class CDeviceFile {
public:
    static CDeviceFile* getInstance(); // getInstance__11CDeviceFileFv
    static bool isInitialized();       // isInitialized__11CDeviceFileFv
};

// The font-file loader work thread is opaque here: the full struct layout
// lives in the loader unit's own .cpp; this TU only allocates/constructs it
// and hands it a font path.
class CDeviceFontLoader;

// Retail font-path pointers (sdata2) selected by the SC language in
// wkUpdate: lbl_eu_806636F8 -> "font/font_eu5.brfna",
// lbl_eu_806636FC -> "font/MenuFont.brfna".
extern const char* lbl_eu_806636F8;
extern const char* lbl_eu_806636FC;

// Minimal layout view of CDeviceFont for the catalog TU
// (libs/monolib/src/device/CDeviceFont.cpp).
//
// The retail class derives from CDeviceBase (CWorkThread, 0x0-0x1C4) and
// carries its own state at 0x1C4-0x1F0: the font-info list (0x1C8) and the
// current font layer/id (0x1E8). The retail singleton lives at
// lbl_eu_80665678 (sdata2).
class CDeviceFont : public CDeviceBase {
public:
    CDeviceFont(const char* pName, CWorkThread* pParent);
    ~CDeviceFont();

    static CDeviceFont* getInstance();

    u32 getFontId();
    u32 func_804525F0();
    IDeviceFontInfo* getFontInfo(u32 fontId, nw4r::lyt::Layout* layout);
    IDeviceFontInfo* func_eu_804558F4();
    u32 func_80452D80();
    void wkUpdate();
    void wkRender();
    bool wkStandbyLogin();
    bool wkStandbyLogout();

    // 0x0-0x1C8: CWorkThread + CDeviceBase base
    CDeviceFontInfoReslist mInfoList;      // 0x1C8 (font-info list)
    u32 mFontId;                           // 0x1E8 (current font layer/id)
    s32 field_0x1EC;                       // 0x1EC (signed: retail switch emits cmpi, not cmpli)
};

// Minimal view of a child font-layer work thread (CDeviceFontLayer). This TU
// only searches the children list for a layer and tail-calls one of its
// methods, so only mType (0x50), the font-id field (0x1F8) and the called
// methods are declared here; the full layout lives in the layer's own unit.
class CDeviceFontLayer {
public:
    // CWorkThread::ThreadType value for font layers
    enum { TYPE_FONT_LAYER = 0x3F };

    u8 field_0x0[0x50];           // 0x0 (vtable + name/state/id)
    s32 mType;                    // 0x50 (CWorkThread::ThreadType)
    u8 field_0x54[0x1F8 - 0x54];  // 0x54
    u32 field_0x1F8;              // 0x1F8 (bound key: font id or device)
    u8 field_0x1FC[0x2F0 - 0x1FC]; // 0x1FC
    u8 mFlag2F0;                  // 0x2F0 (set by notifyLayerDestroy)

    // The retail symbol-map entries carry decompiler-guessed Fv suffixes even
    // though the bodies consume arguments; the wrappers in CDeviceFont.cpp
    // reference them under the literal retail names.
    void func_80453BB4();
    void setBackgroundColor();
    void setFontScale();
    void setFontId();

    static void allocFontHeap();  // font-layer unit helper (standby login)
    static void freeFontHeap();  // font-layer unit helper (standby logout)
};

class CDeviceFontInfo {
public:
    ~CDeviceFontInfo();
};

// Retail singleton pointer (sdata2). Declared here so the catalog TU's
// global references emit the retail lbl_eu_80665678 reloc name.
extern CDeviceFont* lbl_eu_80665678;

// Retail font-info reslist vtables (.data). The base/derived dtors store
// them into the reslist's vtable slot; declared here so the store emits the
// retail lbl reloc names.
extern u32 lbl_eu_8056C734[];
extern u32 lbl_eu_8056C71C[];

// Retail CDeviceFont vtable (.data) - stored manually in the ctor (the
// class is non-virtual, so MWCC emits no vtable of its own).
extern u32 lbl_eu_8056C660[];

// Retail rodata string passed as the font-layer work-thread name.
extern const char lbl_eu_80522DDC[];

// Retail font-device config/state word (sdata2).
extern u32 lbl_eu_8066567C;

// Minimal CDeviceVI view (the full header would drag in CDeviceBase and
// clash with this TU's minimal CWorkThread views).
#include <revolution/GX.h>
class CDeviceVI {
public:
    static void setFlag0(bool state);
    static GXRenderModeObj* getRenderModeObj();
};

// nw4r assertion hook used by the wkRender pointer-validation macros.
namespace nw4r {
namespace db {
void Panic(const char* file, int line, const char* message, ...);
} // namespace db
} // namespace nw4r

// sdata2 constants used by wkRender (fade ramp + projection/cursor math).
extern f64 lbl_eu_8066A3B8;
extern f64 lbl_eu_8066A3C0;
extern const f32 lbl_eu_8066A3C8;
extern const f32 lbl_eu_8066A3CC;
extern const f32 lbl_eu_8066A3D0;
extern const f32 lbl_eu_8066A3D4;
extern const f32 lbl_eu_8066A3D8;
extern const f32 lbl_eu_8066A3DC;
extern const f32 lbl_eu_8066A3E0;
extern const f32 lbl_eu_8066A3E4;
extern const f32 lbl_eu_8066A3E8;
extern const f32 lbl_eu_8066A3EC;
extern const f32 lbl_eu_8066A3F0;
extern const f32 lbl_eu_8066A3F4;
extern const f32 lbl_eu_8066A3F8;
extern const f32 lbl_eu_8066A3FC;

// UTF-16 render buffer (wchar_t[0x200]) filled by wkRender.
extern wchar_t lbl_eu_80657750[512];

// rodata file/message strings passed to nw4r::db::Panic by wkRender.
extern char lbl_eu_8052DC70[];
extern char lbl_eu_8052DC3C[];
extern char lbl_eu_8052DD84[];
extern char lbl_eu_8052DD50[];
extern char lbl_eu_8052DC28[];
extern char lbl_eu_8052DBF4[];
extern char lbl_eu_8053785C[];
extern char lbl_eu_80537828[];
extern char lbl_eu_80537818[];
extern char lbl_eu_805377E0[];
extern char lbl_eu_8052DCFC[];
extern char lbl_eu_8052DCC8[];
extern char lbl_eu_805378A0[];
extern char lbl_eu_8053786C[];
extern char lbl_eu_80537734[];
extern char lbl_eu_80537700[];
extern char lbl_eu_805376EC[];
extern char lbl_eu_805376B8[];

// Unmangled C-linkage imports used by wkUpdate (retail emits these names
// literally, so they cannot be expressed as C++ members): the font-loader
// ctor, its set-path helper, and the SC language query.
extern "C" {
void* __ct__CDeviceFontLoader(CDeviceFontLoader* self, const char* name,
                              CWorkThread* parent);
void setFontPath__17CDeviceFontLoaderFv(CDeviceFontLoader* self, void* arg1,
                                          const char* pPath);
bool func_eu_8044A600();
}
