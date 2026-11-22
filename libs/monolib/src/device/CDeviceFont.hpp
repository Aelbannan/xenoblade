#pragma once

#include <types.h>

#include "monolib/util/reslist.hpp"
#include "monolib/device/CDeviceFont.hpp"

class CDeviceFontLayer;
class CException;

namespace ml {
struct CCol4;
}

namespace nw4r {
namespace lyt {
class Layout;
}
} // namespace nw4r

typedef _reslist_node<IDeviceFontInfo*> CDeviceFontInfoListNode;

// Font-info provider impls (typing only; the real layouts live in their own
// units). Both share the IDeviceFontInfo vtable layout.
class CDeviceFontInfoExt : public CDeviceFontInfo {
public:
    static CDeviceFontInfoExt* create();  // static factory (retail name)
};

class CDeviceFontInfoRom : public CDeviceFontInfo {
public:
    static CDeviceFontInfoRom* create();  // static factory (retail name)
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
    void queueText();
    void setBackgroundColor(const ml::CCol4* col);
    void setFontScale();
    void setFontId(u32 id);

    static void allocFontHeap();  // font-layer unit helper (standby login)
    static void freeFontHeap();  // font-layer unit helper (standby logout)
};

// Retail singleton pointer (sdata2). Declared here so the catalog TU's
// global references emit the retail spDeviceFont reloc name.
extern CDeviceFont* spDeviceFont;

// Retail font-device config/state word (sdata2).
extern u32 sDeviceFontInitCount;

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

// Unmangled C-linkage import used by wkUpdate: the SC language query.
extern "C" {
bool func_eu_8044A600();
}
