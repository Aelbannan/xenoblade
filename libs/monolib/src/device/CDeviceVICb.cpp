#include "monolib/device.hpp"

CDeviceVICb::CDeviceVICb(){
    *(void**)this = (void*)lbl_eu_8056BF68;
    CDeviceVI::entryCb(this);
}

CDeviceVICb::~CDeviceVICb(){
    *(void**)this = (void*)lbl_eu_8056BF68;
    CDeviceVI::removeCb(this);
}

// ===== Dissolved monolibdata2 (blob surgery) data owned by this TU =====
namespace VCBlob {
extern "C" void __dt__11CDeviceVICbFv();
extern "C" void viBeforeDrawDone__11CDeviceVICbFv();
extern "C" void viAfterDrawDone__11CDeviceVICbFv();
extern "C" void viBeginFrame__11CDeviceVICbFv();
}
extern "C" u32 lbl_eu_80663618[2]; // this unit's sdata

// [.data] 0x8056BF68-0x8056BF80 (24B): CDeviceVICb vtable
extern "C" void* lbl_eu_8056BF68[6] = {
    (void*)&lbl_eu_80663618, (void*)0,
    (void*)&VCBlob::__dt__11CDeviceVICbFv,
    (void*)&VCBlob::viBeforeDrawDone__11CDeviceVICbFv,
    (void*)&VCBlob::viAfterDrawDone__11CDeviceVICbFv,
    (void*)&VCBlob::viBeginFrame__11CDeviceVICbFv,
};

// [.rodata] 0x80522A50-0x80522A60 (16B)
extern "C" __declspec(align(4)) const char lbl_eu_80522A50[0x10] = {
    0x43,0x44,0x65,0x76,0x69,0x63,0x65,0x56,0x49,0x43,0x62,0x00,0x00,0x00,0x00,0x00,
};

// [.sdata] 0x80663618-0x80663620 (8B)
extern "C" u32 lbl_eu_80663618[2] = { (u32)&lbl_eu_80522A50, 0x00000000 };
