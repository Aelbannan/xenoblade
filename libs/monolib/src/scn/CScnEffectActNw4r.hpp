#pragma once

#include <types.h>
#include "monolib/math/CVec3.hpp"

// Virtual dispatch target: v_i sits at vtable offset 8+4*i (MWCC RTTI header).
// Object identities are not yet recovered; only the virtuals actually
// dispatched from this TU carry real signatures, the rest are placeholders.

// Effect actor: returned by the lookups below. Carries a CVec3 triplet at
// 0x0c/0x1c/0x2c that callers copy out into a CVec3.
struct CScnEffectAct {
    virtual void v0() = 0;
    virtual void v1() = 0;
    virtual void v2() = 0;
    virtual void v3() = 0;
    virtual void v4() = 0;
    virtual void v5() = 0;
    virtual void v6() = 0;
    virtual void v7() = 0;
    virtual void v8() = 0;
    virtual void v9() = 0;
    virtual void v10() = 0;
    virtual void v11() = 0;
    virtual void v12() = 0;
    virtual CScnEffectAct* v13(u32 idx) = 0;
    virtual void v14() = 0;
    virtual CScnEffectAct* v15(u32 idx) = 0;
    virtual u32 v16(u32 idx, ml::CVec3* out) = 0;
    virtual void v17() = 0;
    virtual CScnEffectAct* v18(u32 idx) = 0;
    virtual void v19() = 0;
    virtual void v20() = 0;
    virtual void v21() = 0;
    virtual void v22() = 0;
    virtual void v23() = 0;
    virtual void v24() = 0;
    virtual void v25() = 0;
    virtual void v26() = 0;
    virtual void v27() = 0;
    virtual void v28() = 0;
    virtual void v29() = 0;
    virtual void v30() = 0;
    virtual void v31() = 0;
    virtual void v32() = 0;
    virtual void v33() = 0;
    virtual void v34() = 0;
    virtual void v35() = 0;
    virtual void v36() = 0;
    virtual void v37() = 0;
    virtual void v38() = 0;
    virtual void v39() = 0;
    virtual CScnEffectAct* v40() = 0;
    virtual void v41() = 0;
    virtual void v42() = 0;
    virtual void v43() = 0;
    virtual ~CScnEffectAct() {}

    /* 0x04 */ u8 pad_0x4[8];
    /* 0x0c */ ml::CVec3 field_0xc;
    /* 0x18 */ u8 pad_0x18[4];
    /* 0x1c */ ml::CVec3 field_0x1c;
    /* 0x28 */ u8 pad_0x28[4];
    /* 0x2c */ ml::CVec3 field_0x2c;
};

// Scene manager shared with the other CScn* classes; only the fields this TU
// touches are declared (flag word at 0x7a4, effect-act chain at 0x7c4).
struct CScnEffectActMgr {
    virtual void v0() = 0;
    virtual void v1() = 0;
    virtual void v2() = 0;
    virtual void v3() = 0;
    virtual void v4() = 0;
    virtual void v5() = 0;
    virtual void v6() = 0;
    virtual void v7() = 0;
    virtual void v8() = 0;
    virtual void v9() = 0;
    virtual void v10() = 0;
    virtual void v11() = 0;
    virtual void v12() = 0;
    virtual CScnEffectAct* v13(u32 idx) = 0;
    virtual ~CScnEffectActMgr() {}

    /* 0x04 */ u8 pad_0x4[0x7a0];
    /* 0x7a4 */ u32 field_0x7a4;
    /* 0x7a8 */ u8 pad_0x7a8[0x1c];
    /* 0x7c4 */ CScnEffectAct* field_0x7c4;
};

class CScnEffectActNw4r {
public:
    CScnEffectActNw4r();
    virtual ~CScnEffectActNw4r();

    // 0x00: vptr
    /* 0x04 */ CScnEffectActMgr* mpMgr;
    /* 0x08 */ CScnEffectAct* mActs[64];
    /* 0x108 */ u16 mIds[64];
    /* 0x188 */ u32 mCount;
};

// Import from monolib/src/scn/CScnTexWorkMan.cpp (target us-80494b68).
// Returns nonzero when the manager has a ready act for the given id.
// extern "C": the retail call-site reloc name is the plain func_80490AF4.
extern "C" int func_80490AF4(CScnEffectActMgr* mgr, u32 idx);
