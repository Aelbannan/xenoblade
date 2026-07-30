// Auto-scaffolded catalog TU for monolib/src/device/CDeviceFont
// Replace stubs with high-level C/C++ during decomp.

#include <harness_catalog.h>

struct CDeviceFont {
    CDeviceFont(const char* pName, void* pParent);
    ~CDeviceFont();
    void func_804525D4();
    void func_804525F0();
    void func_80452690();
    void func_8045271C();
    void func_804527A4();
    void func_8045283C();
    void func_804528C4();
    void func_8045294C();
    void func_804529D4();
    void func_80452B78();
    void func_80452C10();
    void func_eu_804558F4();
    void func_80452CF8() const;
    void func_80452D80();
    void wkUpdate();
    void wkRender();
    void wkStandbyLogin();
    void wkStandbyLogout();
};

struct CDeviceFontInfo {
    ~CDeviceFontInfo();
};


CDeviceFont::CDeviceFont(const char* pName, void* pParent) {}

void __dt___reslist_base_IDeviceFontInfo(){}

void __dt__reslist_IDeviceFontInfo(){}

CDeviceFont::~CDeviceFont() {}

extern "C" CDeviceFont* getInstance__11CDeviceFontFv() {
    extern CDeviceFont* lbl_eu_80665678; // sdata2 singleton pointer
    return lbl_eu_80665678;
}

void CDeviceFont::func_804525D4() {}

void CDeviceFont::func_804525F0() {}

void CDeviceFont::func_80452690() {}

void CDeviceFont::func_8045271C() {}

void CDeviceFont::func_804527A4() {}

void CDeviceFont::func_8045283C() {}

void CDeviceFont::func_804528C4() {}

void CDeviceFont::func_8045294C() {}

void CDeviceFont::func_804529D4() {}

void CDeviceFont::func_80452B78() {}

void CDeviceFont::func_80452C10() {}

void CDeviceFont::func_eu_804558F4() {}

void CDeviceFont::func_80452CF8() const {}

void CDeviceFont::func_80452D80() {}

void CDeviceFont::wkUpdate() {}

CDeviceFontInfo::~CDeviceFontInfo() {}

void CDeviceFont::wkRender() {}

void CDeviceFont::wkStandbyLogin() {}

void CDeviceFont::wkStandbyLogout() {}

extern "C" void func_eu_80457318(u32 val) {
    extern u32 lbl_eu_80665680; // sdata2: font device config/state value
    lbl_eu_80665680 = val;
}
