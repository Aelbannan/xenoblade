#pragma once

#include "types.h"
#include "monolib/util.hpp"
#include "monolib/math.hpp"
#include "monolib/math/FloatUtils.hpp"  // H3 label-owner decl (ml::epsilon)

// C-linkage imports (retail symbol names - keep linkage/signatures verbatim)
// nw4r db Warning() file/message rodata strings (retail .data).
extern const char lbl_eu_80526324[];
extern const char lbl_eu_80526300[];
//Some type of view frustum?
struct CCullFrustum{
    enum Flags{
        FLAG_0 = 1 << 0,
        FLAG_1 = 1 << 1,
        FLAGS_01 = (FLAG_0 | FLAG_1) 
    };

    void init(const ml::CVec3& pos, const ml::CVec3& rot, const ml::CVec3& scale, u32 flags){
        mPos = pos;
        mRot = rot;
        mScale = ml::CVec3(scale.x, scale.y, 1.0f);
        mInFirstList = true;
        unkC0[0] = 1.0f;
        unkC0[1] = 1.0f;
        unkC0[2] = 1.0f;
        unkC0[3] = 1.0f;
        unk12C = 1;
        mFlags = flags;
    }

    ml::CVec3 mPos; //0x0
    ml::CVec3 mRot; //0xC
    ml::CVec3 mScale; //0x18
    //unsure
    ml::CVec3 mDir; //0x24
    ml::CMat34 mMat; //0x30
    ml::CMat34 mMatInv; //0x60
    ml::CVec3 unk90[4]; //0x90
    float unkC0[4]; //unused?
    ml::CPlane mPlane0; //0xD0
    ml::CPlane mPlane1; //0xE0
    ml::CPlane mPlane2; //0xF0
    ml::CPlane mPlane3; //0x100
    ml::CPlane mPlane4; //0x110
    bool mInFirstList; //0x120
    float unk124; //Near plane value?
    float unk128; //Far plane value?
    u32 unk12C;
    u32 mFlags; //0x130
};

class COccCulling{
public:
    COccCulling();
    virtual ~COccCulling();
    void createHeap(u32 r4, int size);
    DECOMP_DONT_INLINE void clear();
    int addFrustum(const ml::CVec3& r4, const ml::CVec3& r5, const ml::CVec3& r6, u32 flags);
    void setFrustum(CCullFrustum* r4);
    bool update(ml::CFrustum* r4);
    void func_801A1188(CCullFrustum* r4);
    bool isOccluded(const ml::CVec3& vec, float distance);
    bool isRayOccluded(const ml::CVec3& rayStartPos, const ml::CVec3& rayEndPos, UNKWORD r6);

    //0x0: vtable
    resvector<CCullFrustum*> mFrustumList1; //0x4
    resvector<CCullFrustum*> mFrustumList2; //0x14
    ml::CFrustum* unk24;
    mtl::ALLOC_HANDLE unk28;
    bool unk2C;
    u8 unk2D;
    u8 unk2E;

};
