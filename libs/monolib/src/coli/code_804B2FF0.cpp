// Auto-scaffolded catalog TU for monolib/src/coli/code_804B2FF0
// Replace stubs with high-level C/C++ during decomp.

#include <harness_catalog.h>
#include "monolib/coli/CTaskColiManager.hpp"

void func_804B2FF0(){}

void func_804B30CC(){}

void func_804B31EC(){}

void func_804B33C8(){}

void func_804B34F4(){}

void func_804B3658(){}

void func_804B37E4(){}

void func_804B3970(){}

void func_804B3B18(){}

void func_804B3D1C(){}

void func_804B3EA8(){}

void func_804B4020(){}

void func_804B41FC(){}

// Global axis-aligned bounding box shared by the coli object-space
// bounds-expansion helpers. Layout: max x/y/z at +0x0/+0x4/+0x8,
// min x/y/z at +0xC/+0x10/+0x14.
struct CColiBounds {
    f32 max[3]; //0x0 max x/y/z
    f32 min[3]; //0xC min x/y/z
};

extern "C" CColiBounds* lbl_eu_80665944;

// Position (x/y/z at +0x4/+0x8/+0xC) and radius at +0x10 used for
// sphere-like bounds expansion.
struct CColiSphere {
    f32 pad0;     //0x0
    f32 x;        //0x4
    f32 y;        //0x8
    f32 z;        //0xC
    f32 radius;   //0x10
};

void func_804B43B4(CColiSphere* p) {
    // Expand the box's max corner by the sphere's max extent and the
    // min corner by its min extent, per axis.
    if (lbl_eu_80665944->max[0] < p->x + p->radius) {
        lbl_eu_80665944->max[0] = p->x + p->radius;
    }
    if (lbl_eu_80665944->min[0] > p->x - p->radius) {
        lbl_eu_80665944->min[0] = p->x - p->radius;
    }
    if (lbl_eu_80665944->max[1] < p->y + p->radius) {
        lbl_eu_80665944->max[1] = p->y + p->radius;
    }
    if (lbl_eu_80665944->min[1] > p->y - p->radius) {
        lbl_eu_80665944->min[1] = p->y - p->radius;
    }
    if (lbl_eu_80665944->max[2] < p->z + p->radius) {
        lbl_eu_80665944->max[2] = p->z + p->radius;
    }
    if (lbl_eu_80665944->min[2] > p->z - p->radius) {
        lbl_eu_80665944->min[2] = p->z - p->radius;
    }
}

// Variant with separate horizontal (x/z) and vertical (y) radii.
// Position x/y/z at +0x4/+0x8/+0xC, radiusXZ at +0x10, radiusY at +0x14.
struct CColiEllipsoid {
    f32 pad0;      //0x0
    f32 x;         //0x4
    f32 y;         //0x8
    f32 z;         //0xC
    f32 radiusXZ;  //0x10
    f32 radiusY;   //0x14
};

void func_804B4478(CColiEllipsoid* p) {
    if (lbl_eu_80665944->max[0] < p->x + p->radiusXZ) {
        lbl_eu_80665944->max[0] = p->x + p->radiusXZ;
    }
    if (lbl_eu_80665944->min[0] > p->x - p->radiusXZ) {
        lbl_eu_80665944->min[0] = p->x - p->radiusXZ;
    }
    if (lbl_eu_80665944->max[2] < p->z + p->radiusXZ) {
        lbl_eu_80665944->max[2] = p->z + p->radiusXZ;
    }
    if (lbl_eu_80665944->min[2] > p->z - p->radiusXZ) {
        lbl_eu_80665944->min[2] = p->z - p->radiusXZ;
    }
    if (lbl_eu_80665944->max[1] < p->y + p->radiusY) {
        lbl_eu_80665944->max[1] = p->y + p->radiusY;
    }
    if (lbl_eu_80665944->min[1] > p->y - p->radiusY) {
        lbl_eu_80665944->min[1] = p->y - p->radiusY;
    }
}

// Capsule (segment swept by a radius) bounds expansion: two endpoints
// (+0x4..+0xC and +0x10..+0x18) and a uniform radius at +0x28, expanded
// into the global box one axis at a time.
struct CColiCapsule {
    f32 field_0x0;         //0x0
    f32 endPos[3];         //0x4  (x/y/z of first end)
    f32 endPosB[3];        //0x10 (x/y/z of second end)
    f32 pad_0x1C;          //0x1C
    f32 field_0x20;        //0x20
    f32 field_0x24;        //0x24
    f32 radius;            //0x28
};

void func_804B453C(CColiCapsule* p) {
    for (int i = 0; i < 3; i++) {
        if (lbl_eu_80665944->max[i] < p->endPos[i] + p->radius) {
            lbl_eu_80665944->max[i] = p->endPos[i] + p->radius;
        }
        if (lbl_eu_80665944->min[i] > p->endPos[i] - p->radius) {
            lbl_eu_80665944->min[i] = p->endPos[i] - p->radius;
        }
        if (lbl_eu_80665944->max[i] < p->endPosB[i] + p->radius) {
            lbl_eu_80665944->max[i] = p->endPosB[i] + p->radius;
        }
        if (lbl_eu_80665944->min[i] > p->endPosB[i] - p->radius) {
            lbl_eu_80665944->min[i] = p->endPosB[i] - p->radius;
        }
    }
}

// Another bounds-expansion source: position x/y/z at +0x10/+0x20/+0x30
// and a single uniform radius at +0x70.
struct CColiBoundsPoint {
    f32 field_0x00[4];  //0x0-0xF
    f32 x;              //0x10
    f32 field_0x14[3];  //0x14-0x1F
    f32 y;              //0x20
    f32 field_0x24[3];  //0x24-0x2F
    f32 z;              //0x30
    f32 field_0x34[15]; //0x34-0x6F
    f32 radius;         //0x70
};

void func_804B45E4(CColiBoundsPoint* p) {
    if (lbl_eu_80665944->max[0] < p->x + p->radius) {
        lbl_eu_80665944->max[0] = p->x + p->radius;
    }
    if (lbl_eu_80665944->min[0] > p->x - p->radius) {
        lbl_eu_80665944->min[0] = p->x - p->radius;
    }
    if (lbl_eu_80665944->max[1] < p->y + p->radius) {
        lbl_eu_80665944->max[1] = p->y + p->radius;
    }
    if (lbl_eu_80665944->min[1] > p->y - p->radius) {
        lbl_eu_80665944->min[1] = p->y - p->radius;
    }
    if (lbl_eu_80665944->max[2] < p->z + p->radius) {
        lbl_eu_80665944->max[2] = p->z + p->radius;
    }
    if (lbl_eu_80665944->min[2] > p->z - p->radius) {
        lbl_eu_80665944->min[2] = p->z - p->radius;
    }
}

void func_804B46A8(CColiBoundsPoint* p) {
    if (lbl_eu_80665944->max[0] < p->x + p->radius) {
        lbl_eu_80665944->max[0] = p->x + p->radius;
    }
    if (lbl_eu_80665944->min[0] > p->x - p->radius) {
        lbl_eu_80665944->min[0] = p->x - p->radius;
    }
    if (lbl_eu_80665944->max[1] < p->y + p->radius) {
        lbl_eu_80665944->max[1] = p->y + p->radius;
    }
    if (lbl_eu_80665944->min[1] > p->y - p->radius) {
        lbl_eu_80665944->min[1] = p->y - p->radius;
    }
    if (lbl_eu_80665944->max[2] < p->z + p->radius) {
        lbl_eu_80665944->max[2] = p->z + p->radius;
    }
    if (lbl_eu_80665944->min[2] > p->z - p->radius) {
        lbl_eu_80665944->min[2] = p->z - p->radius;
    }
}

void func_804B476C(){}

void func_804B4854(){}

void func_804B49B8(){}

void func_804B4BDC(){}

void func_804B4C7C(){}

// --- CTTask local declaration ---
// Local CTTask (out-of-line Move/Draw/dtor) for harness stubs.
// Do not include monolib/work/CTTask.hpp here — its inline methods collide
// with the explicit out-of-line instantiations below.
template <typename T>
class CTTask {
public:
    CTTask();
    virtual ~CTTask();
    virtual void Move();
    virtual void Draw();
};

// --- CTaskColiManager class definition ---
// Full class definition lives here (single-TU scope) because CTTask must
// be defined locally to avoid inline-method codegen from CTTask.hpp.
class CTaskColiManager : public CTTask<CTaskColiManager> {
public:
    // Overrides from CProcess (pure virtual)
    void Init() override;
    void Term() override;
    void Move() override;
    void Draw() override;

    virtual ~CTaskColiManager();
    static CTaskColiManager* create();
};

void CTaskColiManager::Init() {
    // Empty override — no initialization required.
}

void CTaskColiManager::Term() {
    // Empty override — no termination required.
}

void CTaskColiManager::Move() {}

void CTaskColiManager::Draw() {
    // Empty override — no draw-time work required.
}

void func_804B4E10(){}

void func_804B5088(){}

void func_804B526C(){}

void func_804B54D4(){}

void func_804B5658(){}

CTaskColiManager* CTaskColiManager::create() { return 0; }

// --- Explicit template specializations for CTTask<CTaskColiManager> ---
template<> CTTask<CTaskColiManager>::~CTTask() {}
template<> void CTTask<CTaskColiManager>::Move() {}
template<> void CTTask<CTaskColiManager>::Draw() {}
void sinit_804B598C(){}
