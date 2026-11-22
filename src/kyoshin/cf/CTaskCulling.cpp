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

// The singleton is the class's real static member; MWCC emits its mangled name.
cf::CTaskCulling* cf::CTaskCulling::spInstance;

// Merged rodata string blob: "/" at +0, "occ" at +2 (retail .rodata label, owning TU or extern).
extern const char lbl_eu_80503F58[];

// Retail symbols are C-linkage (unmangled); declared here because the declaring
// header code_800AA008.hpp uses C++ mangling that drifts the reloc names.
#include "kyoshin/code_800AA008_Tok.hpp"
extern "C" int Tok_FormatStr(ml::FixStr<64>& buf, u32 packed, int prefixFlag, int suffixFlag);

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
        spInstance = this;
        pScene->unkB4 = this;
        unk94 = mtl::MemManager::allocate_head(mtl::MemManager::getHandleMEM2(), 0x1000, 4);
    }

    CTaskCulling::~CTaskCulling(){
        spInstance = nullptr;
    }

void CTaskCulling::setOccDisabled(unsigned long disable){
    CTaskCulling* instance = spInstance;
    
    if(instance != nullptr){
        if(disable != 0){
            instance->unk120 |= 8;
        }else{
            instance->unk120 &= ~8;
        }
    }
}

    UNKTYPE* CTaskCulling::getOccBuffer(){
        if(spInstance == nullptr) return nullptr;
        return spInstance->unk94;
    }

    bool CTaskCulling::update(ml::CFrustum* frustum){
        if(spInstance == nullptr) return false;
        return mOccCulling.update(frustum);
    }

bool CTaskCulling::isOccluded(const ml::CVec3& intersectPoint, float distance){
        if(spInstance == nullptr) return false;
        if(unk120 & 8) return false;
        return mOccCulling.isOccluded(intersectPoint, distance);
    }

bool CTaskCulling::isRayOccluded(const ml::CVec3& rayStartPos, const ml::CVec3& rayEndPos, int flags){
        if(spInstance == nullptr) return false;
        return mOccCulling.isRayOccluded(rayStartPos, rayEndPos, flags);
    }

void CTaskCulling::clearOccFrustums(){
    CTaskCulling* instance = spInstance;
    if (instance == nullptr) return;
    instance->mOccCulling.clear();
}

    void CTaskCulling::reloadOccFrustums(){
        if(spInstance == nullptr) return;

        clearOccFrustums();

        cf::CfGameManager* gm = (cf::CfGameManager*)cf::CfGameManager::getGameSubManager();
        u32 outA, outB, outC, outD;

        Tok_Unpack(gm->unk70, &outA, &outB, &outC, &outD);
        Tok_FormatStr(spInstance->unk98, gm->unk70, 0, 0);
        Tok_FormatStr(spInstance->unkDC, gm->unk70, 1, 0);

        // Cached after the last external call.
        ml::FixStr<64>* path = &spInstance->unkDC;
        const char* sep = lbl_eu_80503F58;

        // Inlined FixStr path trim: find the last "/" and cut right after it
        // (the stored length keeps the trailing separator).
        int index = path->rfind(sep, -1);
        if(index != -1 && index + 1 < path->mLength){
            path->mString[index + 1] = '\0';
            path->mLength = index + 1;
        }

        OccBufferHead* head = (OccBufferHead*)spInstance->unk94;

        if(std::strcmp(head->magic, sep + 2) == 0){
            OccFrustumEntry* entry = (OccFrustumEntry*)(head + 1); // entries at offset 0x10

            for (int i = 0; i < head->count; i++) {
                if(spInstance != nullptr){
                    spInstance->mOccCulling.addFrustum(entry->vec0, entry->vec1, entry->vec2, entry->field_24);
                }
                entry++;
            }
        }
    }

CTaskCulling* CTaskCulling::getInstance() {
    return spInstance;
}

    void CTaskCulling::Init(){
        gmScene->addRenderCB(this, 1, 0);
    }
    void CTaskCulling::Term(){
        CScn* scene = gmScene;
        if(scene != nullptr){
            scene->removeRenderCB(this);
        }

        mpScene->unkB4 = nullptr;
        //???
        if(spInstance != nullptr) (void)spInstance;

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
