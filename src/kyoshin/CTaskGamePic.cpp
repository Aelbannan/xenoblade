// Auto-scaffolded catalog TU for kyoshin/CTaskGamePic
// Replace stubs with high-level C/C++ during decomp.

#include "kyoshin/harness_catalog.hpp"

void Draw__12CTaskGamePicFv() {}









void func_8029539C(void* self){}

void CTaskGamePic::create() {}


void OnFileEvent__12CTaskGamePicFP10CEventFile(void* self) { ((void(*)(void*))func_8029539C)((char*)self - 0x54); }

void func_8029554C(void* self) { ((void(*)(void*))__dt__12CTaskGamePicFv)((char*)self - 0x54); }

void func_80295554(void* self) { ((void(*)(void*))cbRenderBefore__12CTaskGamePicFv)((char*)self - 0x58); }

void func_8029555C(void* self) { ((void(*)(void*))__dt__12CTaskGamePicFv)((char*)self - 0x58); }

// --- hard-symbol stubs (scaffold_hard_symbols) ---
// Local CTTask (out-of-line Move/Draw/dtor) for harness stubs.
// Do not include monolib/work/CTTask.hpp here — its inline methods collide.
template <typename T>
class CTTask {
public:
    CTTask();
    virtual ~CTTask();
    virtual void Move();
    virtual void Draw();
};

class CTaskGamePic;
template<> void CTTask<CTaskGamePic>::Move() {}
template<> void CTTask<CTaskGamePic>::Draw() {}
s16 func_80295388(void* self){
    struct UnkPart {
        u8 _pad[2];
        s16 field_2;
        u16 _pad2;
        s16 field_6;
    };
    const UnkPart* p = static_cast<const UnkPart*>(self);
    return p->field_2 + p->field_6;
}
