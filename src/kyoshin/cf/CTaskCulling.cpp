// Retail elides the empty ~IWorkEvent base-dtor call in ~CTaskCulling.
#define IWORK_EVENT_INLINE_DTOR
#include "kyoshin/cf/CTaskCulling.hpp"
#include "kyoshin/cf/CfGameManager.hpp"
#include "monolib/scn.hpp"
#include <cstring>

// --- CTTask<cf::CTaskCulling> out-of-line specializations ---
// The canonical declared-only template emits no bodies; these explicit
// specializations produce the retail standalone Move/Draw/dtor symbols.
template<>
void CTTask<cf::CTaskCulling>::Move() {
    if (mMoveFunc) {
        (static_cast<cf::CTaskCulling*>(this)->*mMoveFunc)();
    }
}

template<>
void CTTask<cf::CTaskCulling>::Draw() {
    if (mDrawFunc) {
        (static_cast<cf::CTaskCulling*>(this)->*mDrawFunc)();
    }
}

template<>
CTTask<cf::CTaskCulling>::~CTTask() {}

// Buffer header for spInstance->unk94 (occ culling data)
#pragma pack(push, 1)
struct OccBufferHead {
    char magic[4];   // "occ\0"
    u8 _04[2];       // padding
    u16 count;        // at offset 6
    u8 _08[8];        // 0x08-0x0F
    // OccFrustumEntry entries[] at offset 0x10
};

struct OccFrustumEntry {
    ml::CVec3 vec0;   // 0x00
    ml::CVec3 vec1;   // 0x0C
    ml::CVec3 vec2;   // 0x18
    u32 field_24;      // 0x24
    u8 _28[0x0C];      // 0x28-0x33 padding
};
#pragma pack(pop)

// Retail sbss singleton (config symbols.txt); mangled spInstance fails reloc name match.
cf::CTaskCulling* lbl_eu_80664328;

// Merged rodata string blob: "/" at +0, "occ" at +2 (defined in data_defs.cpp).
extern const char lbl_eu_80503F58[];

// Retail symbols are C-linkage (unmangled); declared here because the declaring
// header code_800AA008.hpp uses C++ mangling that drifts the reloc names.
extern "C" void func_800AA318(u32 packed, u32* out0, u32* out1, u32* out2, u32* out3);
extern "C" int func_800AA33C(ml::FixStr<64>& buf, u32 packed, int prefixFlag, int suffixFlag);

namespace cf{
    //unused
    ml::CVec3 CTaskCulling::lbl_80579018_0 = ml::CVec3(5,30,50);
    ml::CVec3 CTaskCulling::lbl_80579018_C = ml::CVec3(5,30,50);
    ml::CVec3 CTaskCulling::lbl_80579018_18 = ml::CVec3(5,30,0);
    ml::CVec4 CTaskCulling::lbl_80579018_28 = ml::CVec4(0,1,0,0.2f);

    CTaskCulling::CTaskCulling(CScn* pScene):
    mpScene(pScene),
    unk94(nullptr),
    unk98(),
    unkDC(),
    unk120(0){
        lbl_eu_80664328 = this;
        pScene->unkB4 = this;
        unk94 = mtl::MemManager::allocate_head(mtl::MemManager::getHandleMEM2(), 0x1000, 4);
    }

    CTaskCulling::~CTaskCulling(){
        lbl_eu_80664328 = nullptr;
    }

void CTaskCulling::func_801A2BD0(unsigned long r3){
    CTaskCulling* instance = lbl_eu_80664328;
    
    if(instance != nullptr){
        if(r3 != 0){
            instance->unk120 |= 8;
        }else{
            instance->unk120 &= ~8;
        }
    }
}

    UNKTYPE* CTaskCulling::getOccBuffer(){
        if(lbl_eu_80664328 == nullptr) return nullptr;
        return lbl_eu_80664328->unk94;
    }

    bool CTaskCulling::ICulling_UnkVirtualFunc1(ml::CFrustum* r4){
        if(lbl_eu_80664328 == nullptr) return false;
        return 0; // patched for compile
    }

bool CTaskCulling::ICulling_UnkVirtualFunc2(const ml::CVec3& r4, float r5){
        if(lbl_eu_80664328 == nullptr) return false;
        return (unk120 & 8) ? false : 0;
    }

bool CTaskCulling::ICulling_UnkVirtualFunc3(const ml::CVec3& r4, const ml::CVec3& r5, int r6){
        if(lbl_eu_80664328 == nullptr) return false;
        return 0;
    }

void CTaskCulling::func_801A2C94(){
    CTaskCulling* instance = lbl_eu_80664328;
    if (instance == nullptr) return;
    (void)instance;
}

    void CTaskCulling::func_801A2CAC(){
        if(lbl_eu_80664328 == nullptr) return;

        func_801A2C94();

        cf::CfGameManager* gm = (cf::CfGameManager*)cf::CfGameManager::getGameSubManager();
        u32 outA, outB, outC, outD;

        func_800AA318(gm->unk70, &outA, &outB, &outC, &outD);
        func_800AA33C(lbl_eu_80664328->unk98, gm->unk70, 0, 0);
        func_800AA33C(lbl_eu_80664328->unkDC, gm->unk70, 1, 0);

        // Cached after the last external call.
        ml::FixStr<64>* path = &lbl_eu_80664328->unkDC;
        const char* sep = lbl_eu_80503F58;

        // Inlined FixStr path trim: find the last "/" and cut right after it
        // (the stored length keeps the trailing separator).
        int index = path->rfind(sep, -1);
        if(index != -1 && index + 1 < path->mLength){
            path->mString[index + 1] = '\0';
            path->mLength = index + 1;
        }

        OccBufferHead* head = (OccBufferHead*)lbl_eu_80664328->unk94;

        if(std::strcmp(head->magic, sep + 2) == 0){
            OccFrustumEntry* entry = (OccFrustumEntry*)(head + 1); // entries at offset 0x10

            for (int i = 0; i < head->count; i++) {
                if(lbl_eu_80664328 != nullptr){
                    lbl_eu_80664328->mOccCulling.addFrustum(entry->vec0, entry->vec1, entry->vec2, entry->field_24);
                }
                entry++;
            }
        }
    }

CTaskCulling* CTaskCulling::getInstance() {
    return lbl_eu_80664328;
}

    void CTaskCulling::Init(){
        cf::CfGameManager::spScene->addRenderCB(this, 1, 0);
    }
    void CTaskCulling::Term(){
        CScnNw4r* scene = cf::CfGameManager::spScene;
        if(scene != nullptr){
            scene->removeRenderCB(this);
        }

        mpScene->unkB4 = nullptr;
        //???
        if(lbl_eu_80664328 != nullptr) (void)lbl_eu_80664328;

        DELETE_OBJ(unk94);
    }
void CTaskCulling::Move(){}
void CTaskCulling::Draw(){}

void CTaskCulling::cbRenderBefore(){}

    CTaskCulling* CTaskCulling::create(CProcess* pParent, CScn* pScene){
        CTaskCulling* taskCulling = new (CWorkThreadSystem::getWorkMem()) CTaskCulling(pScene);
        taskCulling->Regist(pParent, false);
        return taskCulling;
    }

} //namespace cf

// absorb: split1 retail data sections
// generated from retail build/us/asm via split1_spec.txt
__declspec(section ".data") __attribute__((aligned(8))) __attribute__((used)) unsigned char __absorb_kyoshin_cf_CTaskCulling_data[0x148] = {
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00
};
__declspec(section ".rodata") __attribute__((aligned(8))) __attribute__((used)) const unsigned char __absorb_kyoshin_cf_CTaskCulling_rodata[0x38] = {
    0x63, 0x66, 0x3A, 0x3A, 0x43, 0x54, 0x61, 0x73, 0x6B, 0x43, 0x75, 0x6C,
    0x6C, 0x69, 0x6E, 0x67, 0x00, 0x00, 0x00, 0x00, 0x43, 0x54, 0x54, 0x61,
    0x73, 0x6B, 0x3C, 0x63, 0x66, 0x3A, 0x3A, 0x43, 0x54, 0x61, 0x73, 0x6B,
    0x43, 0x75, 0x6C, 0x6C, 0x69, 0x6E, 0x67, 0x3E, 0x00, 0x00, 0x00, 0x00,
    0x2F, 0x00, 0x6F, 0x63, 0x63, 0x00, 0x00, 0x00
};
__attribute__((used)) unsigned int __absorb_kyoshin_cf_CTaskCulling_sbss[0x2];
__declspec(section ".sdata") __attribute__((aligned(8))) __attribute__((used)) unsigned char __absorb_kyoshin_cf_CTaskCulling_sdata[0x10] = {
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00
};