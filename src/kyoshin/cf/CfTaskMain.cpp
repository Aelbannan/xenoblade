#include "kyoshin/cf/CfTaskMain.hpp"
#include "monolib/work/CProcess.hpp"
#include "monolib/work/CWorkThreadSystem.hpp"
#include "monolib/util/MemManager.hpp"

// Raw constructor symbols used to construct base classes in-place
extern "C" void __ct__8CProcessFv(void* self);

namespace cf {

// Complete object destructor.
// MWCC generates: null check, CTTask::~CTTask() → CProcess::~CProcess(),
// conditional delete. IWorkEvent is not a C++ base so no ~IWorkEvent() call.
CfTaskMain::~CfTaskMain() {}

void CfTaskMain::Init() {}

void CfTaskMain::Term() {}

void CfTaskMain::Move() {
    extern void func_8007F9C4__Q22cf13CfGameManagerFv(void);
    extern unsigned long lbl_eu_80663E28;
    if ((lbl_eu_80663E28 & 0x4000) != 0) return;
    func_8007F9C4__Q22cf13CfGameManagerFv();
}

void CfTaskMain::Draw() {
    extern void func_8007FBFC__Q22cf13CfGameManagerFv(void);
    func_8007FBFC__Q22cf13CfGameManagerFv();
}

void CfTaskMain::Tail() {
    extern void func_8007FC2C__Q22cf13CfGameManagerFv(void);
    func_8007FC2C__Q22cf13CfGameManagerFv();
}

} // namespace cf

// Factory function: allocates, constructs, and registers a CfTaskMain.
//   r3 = parent CProcess* (for Regist)
//   r4 = u32 value stored at field_0x58
// Returns new CfTaskMain* (null if allocation failed).
// 
// Construction is done manually (without placement new) to avoid the extra
// null-check that MWCC's placement new inserts.
void* __ct__800697E8(CProcess* parent, u32 field58val) {
    // Extern data symbols used during construction
    extern const u8 lbl_eu_80526F58[];   // CTTask<CfTaskMain> vtable
    extern const u8 lbl_eu_80526E80[];   // CfTaskMain vtable (primary + IWorkEvent)
    extern const u8 __ptmf_null[12];     // null pointer-to-member-function

    void* mem = mtl::MemManager::allocate(sizeof(cf::CfTaskMain), CWorkThreadSystem::getWorkMem());
    cf::CfTaskMain* obj = static_cast<cf::CfTaskMain*>(mem);
    if (obj) {
        // Construct CProcess base
        __ct__8CProcessFv(obj);
        
        // CTTask<CfTaskMain> constructor (inlined):
        // Set CTTask vtable
        reinterpret_cast<void**>(obj)[4] = const_cast<u8*>(lbl_eu_80526F58); // 0x10 / 4 = 4
        
        // Copy __ptmf_null to mMoveFunc (0x3C) and mDrawFunc (0x48)
        for (int i = 0; i < 12; i++) {
            reinterpret_cast<u8*>(obj)[0x3C + i] = __ptmf_null[i];
            reinterpret_cast<u8*>(obj)[0x48 + i] = __ptmf_null[i];
        }
        
        // CfTaskMain constructor (inlined):
        // Overwrite vtable with CfTaskMain vtable
        reinterpret_cast<void**>(obj)[4] = const_cast<u8*>(lbl_eu_80526E80); // 0x10
        // Set IWorkEvent vtable (at offset 0x24 in CfTaskMain vtable)
        obj->mIWorkEventVtable = const_cast<u8*>(lbl_eu_80526E80 + 0x24); // 0x54
        // Store field
        obj->field_0x58 = field58val;
    }
    // Regist is called even when obj is null (retail behavior)
    obj->Regist(parent, false);
    return obj;
}

// IWorkEvent dtor adjusting thunk.
// Subtracts 0x54 from IWorkEvent* to recover CfTaskMain*, then calls the dtor.
void func_80069944(void* self) {
    reinterpret_cast<cf::CfTaskMain*>(reinterpret_cast<u8*>(self) - 0x54)->~CfTaskMain();
}
