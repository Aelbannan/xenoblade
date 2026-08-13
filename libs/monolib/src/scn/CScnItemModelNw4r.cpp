// Auto-scaffolded catalog TU for monolib/src/scn/CScnItemModelNw4r
// Replace stubs with high-level C/C++ during decomp.

#include <harness_catalog.h>
#include <string.h>

#include <nw4r/db/db_assert.h>
#include <nw4r/g3d/g3d_anmclr.h>
#include <nw4r/g3d/g3d_anmobj.h>
#include <nw4r/g3d/g3d_anmscn.h>
#include <nw4r/g3d/g3d_anmtexpat.h>
#include <nw4r/g3d/g3d_anmtexsrt.h>
#include <nw4r/g3d/g3d_fog.h>
#include <nw4r/g3d/g3d_scnmdl.h>
#include <nw4r/g3d/g3d_scnobj.h>
#include <nw4r/g3d/g3d_scnroot.h>
#include <nw4r/g3d/res/g3d_resfile.h>
#include <nw4r/g3d/res/g3d_resmdl.h>
#include <nw4r/g3d/res/g3d_resanmchr.h>

#include "monolib/util/MemManager.hpp"

#include "libs/monolib/src/scn/CScn.hpp"
#include "libs/monolib/src/scn/CScnItemModel.hpp"
#include "libs/monolib/src/scn/CScnItemModelNw4r.hpp"

// Multiple targets map to the same function - single definition
u32 func_804871A8(u8* self) { return *(u32*)((u8*)self + 0x4ac); }

u32 func_8048736C(u8* self) { return *(u32*)((u8*)self + 0x14c0); }

extern "C" void func_804876C0(u8* self) {
    *(u16*)((u8*)self + 0x17FA) |= 1;
    *(u16*)((u8*)self + 0x181A) |= 1;
}

void func_804876DC(u8* self) { func_804EB798((u8*)self + 0x17a0); }

void func_804EB7F8(u8* self);
void func_804876E4(u8* self) { ((void(*)(void*))func_804EB7F8)((char*)self + 0x17a0); }

void func_80487B84(){}

// func_80487C78: for every material in the model, rewrite the four GX
// tev-swap-mode tables so the R/G/B/A channels are reordered: identity
// (0,1,2,3) when `enable` is clear, or a red-mono mapping (0,0,0,3) when
// set. The +0x7A4 bit-4 flag mirrors `enable`; the reference chain is
// notified last (func_80484914).
void func_80487C78(CScnItemModelNw4r* self, u32 enable) {
    if (enable != 0) {
        self->field_0x7A4 |= 0x10;
    } else {
        self->field_0x7A4 &= ~0x10;
    }
    u32 c0, c1, c2, c3;
    if (enable != 0) {
        c0 = 0;
        c1 = 0;
        c2 = 0;
        c3 = 3;
    } else {
        c0 = 0;
        c1 = 1;
        c2 = 2;
        c3 = 3;
    }
    for (u32 i = 0;
         i < nw4r::g3d::ResMdl(self->field_0x146C).GetResMatNumEntries();
         i++) {
        nw4r::g3d::ScnMdl::CopiedMatAccess access(
            (nw4r::g3d::ScnMdl*)self->field_0x147C, i);
        nw4r::g3d::ResTev tev =
            GetResTevEx__Q44nw4r3g3d6ScnMdl15CopiedMatAccessFv(&access);
        for (u32 s = 0; s < 4; s++) {
            GXTevColorChan chan;
            if (tev.GXGetTevSwapModeTable((GXTevSwapSel)s, &chan, &chan,
                                          &chan, &chan)) {
                tev.GXSetTevSwapModeTable((GXTevSwapSel)s,
                                          (GXTevColorChan)c0,
                                          (GXTevColorChan)c1,
                                          (GXTevColorChan)c2,
                                          (GXTevColorChan)c3);
            }
        }
    }
    func_80484914((CScnItemModel*)self, enable);
}

void func_804E679C(u8* self);
void func_80487E40(u8* self) { ((void(*)(void*))func_804E679C)((char*)self + 0x1700); }

void func_804E6898(u8* self);
void func_80487E48(u8* self) { ((void(*)(void*))func_804E6898)((char*)self + 0x1700); }

u32 func_80487E50(u8* self) { return *(u32*)((u8*)self + 0x1708); }

// func_80487E58: forward `param` to the chain-last node's vtable-0x64
// handler (when linked), then to the +0x1770 eye-anim state machine.
void func_80487E58(CScnItemModelNw4r* self, u32 param) {
    CScnItemModel* node = self->field_0x7C4;
    if (node != 0) {
        node->vfunc64(param);
    }
    func_804E77BC(&self->field_0x1770, param);
}

// Virtual dispatch target: v_i at vtable offset 8+4*i (MWCC RTTI header).
struct VTarget {
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
    virtual void v13() = 0;
    virtual void v14() = 0;
    virtual void v15() = 0;
    virtual void v16() = 0;
    virtual void v17() = 0;
    virtual void v18() = 0;
    virtual void v19() = 0;
    virtual void v20() = 0;
    virtual void v21() = 0;
    virtual void v22() = 0;
    virtual void v23() = 0;
    virtual void* v24() = 0;
    virtual ~VTarget() {}
};

extern "C" void* func_80487EB8(u8* self) {
    void* obj = *(void**)((u8*)self + 0x7C4);
    if (obj != 0) return ((VTarget*)obj)->v24();
    return *(void**)((u8*)self + 0x179C);
}

// func_80487EE0: per-frame visibility/fade update. Copies the first
// frame-table entry (low u16) into the scene object's +0x122 slot, resets
// the +0x16C4 frame counter and refreshes the fade: the fade value comes
// from the owner's fade-control object (+0x84 sub, +8 float), combined with
// the fade-walk result (func_80484EB0, forced to 1.0 by the +0x7A8 bit-2
// flag) through the +0x16BC latch, then pushed to the +0x1484 (vtable-0x24)
// / +0x1488 / +0x148C (vtable-0x28) anim handles. The +0x7A4 bit-16 hidden
// flag is then re-evaluated from the owner's node-list distance checks, and
// the bit-4/bit-5 camera-hookup latches are advanced (func_8048F7A8 /
// func_8048F630 gated by the +0x1000 flag).
void func_80487EE0(CScnItemModelNw4r* self) {
    if (self->field_0x17D0 != 0) {
        ((CScnItemModelNw4rScnObj122*)self->field_0x147C)->field_0x122 =
            (u16)self->field_0x17CC[0];
    }
    self->field_0x16C4 = 0;
    func_80485CE8(self);
    if (self->field_0x7A4 & 0x1000) {
        func_804E77C4(&self->field_0x1770);
        func_804E6A28(&self->field_0x1700);
        f32 fade = self->field_04->field_0x84->value8;
        f32 f = func_80484EB0((CScnItemModel*)self);
        if (self->field_0x7A8 & 4) {
            f = lbl_eu_8066A8FC;
        }
        if (self->field_0x1484 != 0) {
            ((CScnItemModelNw4rAnimV24*)self->field_0x1484)->v09(fade);
        }
        if (fade == lbl_eu_8066A8E0 || fade != f) {
            if (self->field_0x16BC != lbl_eu_8066A8E0) {
                if (f != lbl_eu_8066A8E0) {
                    fade = self->field_0x16BC;
                }
                self->field_0x16BC = lbl_eu_8066A8E0;
            }
        } else {
            self->field_0x16BC = fade;
        }
        f = fade * f;
        if (f < lbl_eu_8066A900) {
            f = lbl_eu_8066A8E0;
        }
        if (f != lbl_eu_8066A8E0 && !(self->field_0x7A8 & 0x200)) {
            f = lbl_eu_8066A8FC;
        }
        if (self->field_0x1488 != 0) {
            ((CScnItemModelNw4rAnimV28*)self->field_0x1488)->v10(f);
        }
        if (self->field_0x148C != 0) {
            ((CScnItemModelNw4rAnimV28*)self->field_0x148C)->v10(f);
        }
    }
    if (!(self->field_0x7A4 & 0x20) && self->field_854 == 0) {
        self->field_0x147C->SetMtx(nw4r::g3d::ScnObj::MTX_LOCAL,
                                   self->field_0x240);
    }
    ((CScnItemModelNw4rScnScale*)self->field_0x147C)->mScale[0] =
        self->field_0x304;
    ((CScnItemModelNw4rScnScale*)self->field_0x147C)->mScale[1] =
        self->field_0x308;
    ((CScnItemModelNw4rScnScale*)self->field_0x147C)->mScale[2] =
        self->field_0x30C;
    if (self->field_0x7A4 & 0x1000) {
        func_804EBAE8(&self->field_0x17A0);
    }
    if (!(self->field_0x7A4 & 0x1000)) {
        if (((CScnItemModelNw4rV74*)self)->v27() == 0 &&
            (func_804844D0(self) == 0 || func_804842B0(self) == 0)) {
            u32 oldBit = (self->field_0x7A4 >> 15) & 1;
            if (self->field_0x7A8 & 0x80) {
                self->field_0x7A4 &= ~0x10000;
            } else {
                void* obj = func_80496264(self->field_04, -1);
                if (((CScnItemModelNw4rV20*)obj)
                        ->v06(&self->field_0x2DC[0]) == 0) {
                    self->field_0x7A4 |= 0x10000;
                } else {
                    self->field_0x7A4 &= ~0x10000;
                }
                if (!(self->field_0x7A4 & 0x400000) &&
                    !(self->field_0x7A4 & 0x10000)) {
                    CScnItemModelNw4rB4V10* b4 =
                        (CScnItemModelNw4rB4V10*)self->field_04->field_0xB4;
                    if (b4 != 0) {
                        if (b4->v02(&self->field_0x2DC[0],
                                    self->field_0x2E8) == 0) {
                            self->field_0x7A4 &= ~0x10000;
                        } else {
                            self->field_0x7A4 |= 0x10000;
                        }
                    }
                }
            }
            if (oldBit != ((self->field_0x7A4 >> 15) & 1)) {
                ((CScnItemModel*)self)->vfuncA4(1);
            }
            self->field_0x7A8 |= 0x400;
        }
    }
    if (self->field_0x7A4 & 0x08000000) {
        if (self->field_0x7A4 & 0x10000) {
            // [inferred elided retail block: node distance re-check, then:]
            if (func_8048F7A8(self->field_04->field_0x8C,
                              (CScnCamLayout*)self) == 0) {
                self->field_0x7A4 |= 0x1000;
            } else {
                self->field_0x7A4 &= ~0x1000;
            }
        } else {
            if (!(self->field_0x7A4 & 0x1000)) {
                if (func_8048F630(self->field_04->field_0x8C,
                                  (CScnCamLayout*)self) != 0) {
                    self->field_0x7A4 |= 0x1000;
                } else {
                    self->field_0x7A4 &= ~0x1000;
                }
            }
        }
    } else if (self->field_0x7A4 & 0x02000000) {
        func_80483134(self, 0);
        u32 b10 = (self->field_0x7A4 >> 21) & 1;
        u32 b6 = (self->field_0x7A4 >> 25) & 1;
        u32 b23 = (self->field_0x7A8 >> 8) & 1;
        u32 b5 = (self->field_0x7A4 >> 26) & 1;
        u32 comb = (b10 | b23) | b6;
        if ((b5 ^ comb) != 0) {
            if (comb != 0) {
                self->field_0x7A4 |= 0x80000000;
            } else {
                self->field_0x7A4 &= ~0x04000000;
            }
            if (self->field_0x7A4 & 0x04000000) {
                if (self->field_0x7A4 & 0x1000) {
                    if (func_8048F7A8(self->field_04->field_0x8C,
                                      (CScnCamLayout*)self) == 0) {
                        self->field_0x7A4 |= 0x1000;
                    } else {
                        self->field_0x7A4 &= ~0x1000;
                    }
                }
            } else {
                if (!(self->field_0x7A4 & 0x1000)) {
                    if (func_8048F630(self->field_04->field_0x8C,
                                      (CScnCamLayout*)self) != 0) {
                        self->field_0x7A4 |= 0x1000;
                    } else {
                        self->field_0x7A4 &= ~0x1000;
                    }
                }
            }
        }
    }
}

// func_804884F8: fade refresh (func_80485D64), then set the +0x310 scale
// triple: from the +0x14AC model object's floats when present, else
// bit-copy the act-data transform's previous position (mPrevPos x/y/z) into
// the +0x310/+0x314/+0x318 slots.
void func_804884F8(CScnItemModelNw4r* self) {
    func_80485D64(self);
    CScnItemModelNw4r14AC* src = self->field_0x14AC;
    if (src != 0) {
        self->field_0x310 = src->f0xC;
        self->field_0x314 = src->f0x1C;
        self->field_0x318 = src->f0x2C;
    } else {
        ml::CAttrTransform* t = (ml::CAttrTransform*)func_8048315C(self);
        // Two u32 copies with the +4 store first (retail's exact order and
        // register colors: x->r4, y->r0, then stores 0x314, 0x310).
        u32 lo = *(u32*)&t->mPrevPos.x;
        u32 hi = *(u32*)&t->mPrevPos.y;
        *(u32*)&self->field_0x314 = hi;
        *(u32*)&self->field_0x310 = lo;
        *(u32*)&self->field_0x318 = *(u32*)&t->mPrevPos.z;
    }
}

// func_8048856C: model transform refresh - update the act-data transform,
// push the +0x240 matrix into the g3d scene object (SetMtx), then copy the
// +0x304 scale triple into its ScnLeaf scale slot.
void func_8048856C(CScnItemModelNw4r* self) {
    ((ml::CAttrTransform*)func_8048315C(self))->update();
    self->field_0x147C->SetMtx(nw4r::g3d::ScnObj::MTX_LOCAL, self->field_0x240);
    CScnItemModelNw4rScnScale* scn = (CScnItemModelNw4rScnScale*)self->field_0x147C;
    scn->mScale[0] = self->field_0x304;
    scn->mScale[1] = self->field_0x308;
    scn->mScale[2] = self->field_0x30C;
}

extern "C" u32 func_804885C8(u8* self) {
    u32 r = 0;
    if (*(u32*)((u8*)self + 0x7C8) != 0 || *(u32*)((u8*)self + 0x854) != 0 || (*(u32*)((u8*)self + 0x7A8) & 0x800)) {
        r = 1;
    }
    return r;
}

// func_804885FC: per-frame model transform refresh. Clears the +0x16C2 u16
// flag; when the bit-26 busy flag is set, rebuilds the +0x2DC bounding vec
// (from the +0x14AC source object's translate when present, else from the
// summed +0x2EC/+0x2F8 box extents halved and scaled by the +0x304 scale
// triple, with +0x2E8 = +0x7AC radius * max(scale)), then rotates it by the
// act-data mLocalMat. While the +0x824 node table is populated and the
// +0x7A8 bit-10 flag is clear, re-evaluates the +0x7A4 bit-16 hidden flag
// via the owner's nodelist (vtable-0x20) and the +0xB4 distance check
// (vtable-0x10), notifying via the vtable-0xA4 virtual when it changes.
// Finally updates the +0x7A0/+0x17A0 sub-object state gated on the
// +0x7A4/+0x7A8 flag combos.
void func_804885FC(CScnItemModelNw4r* self) {
    self->field_0x16C2 = 0;
    if (self->field_0x7A4 & 0x04000000) {
        if (self->field_0x14AC != 0) {
            self->field_0x2DC[0] = self->field_0x14B4.x;
            self->field_0x2DC[1] = self->field_0x14B4.y;
            self->field_0x2DC[2] = self->field_0x14B4.z;
            ml::CAttrTransform* t = func_8048315C(self);
            PSMTXMultVec(t->mLocalMat.m, (const Vec*)&self->field_0x2DC[0],
                         (Vec*)&self->field_0x2DC[0]);
        } else {
            self->field_0x2DC[0] =
                self->field_0x304 * (lbl_eu_8066A904 * (self->field_0x2EC[3] + self->field_0x2EC[0]));
            self->field_0x2DC[1] =
                self->field_0x308 * (lbl_eu_8066A904 * (self->field_0x2EC[4] + self->field_0x2EC[1]));
            self->field_0x2DC[2] =
                self->field_0x30C * (lbl_eu_8066A904 * (self->field_0x2EC[5] + self->field_0x2EC[2]));
            f32 mx = self->field_0x308 > self->field_0x304 ? self->field_0x308
                                                            : self->field_0x304;
            if (self->field_0x30C > mx) {
                mx = self->field_0x30C;
            }
            self->field_0x2E8 = self->field_0x7AC * mx;
            ml::CAttrTransform* t = func_8048315C(self);
            PSMTXMultVec(t->mLocalMat.m, (const Vec*)&self->field_0x2DC[0],
                         (Vec*)&self->field_0x2DC[0]);
        }
    }
    if (self->member824.field_4 != 0) {
        if (!(self->field_0x7A8 & 0x400)) {
            u32 oldBit16 = (self->field_0x7A4 >> 16) & 1;
            if (self->field_0x7A8 & 0x80) {
                self->field_0x7A4 &= ~0x10000;
            } else {
                void* obj = func_80496264(self->field_04, -1);
                if (((CScnItemModelNw4rV20*)obj)->v06(&self->field_0x2DC[0]) == 0) {
                    self->field_0x7A4 |= 0x10000;
                } else {
                    self->field_0x7A4 &= ~0x10000;
                }
                u32 f3 = self->field_0x7A4;
                if (!(f3 & 0x400000) && !(f3 & 0x10000)) {
                    CScnItemModelNw4rB4V10* b4 =
                        (CScnItemModelNw4rB4V10*)self->field_04->field_0xB4;
                    if (b4 != 0) {
                        if (b4->v02(&self->field_0x2DC[0], self->field_0x2E8) == 0) {
                            self->field_0x7A4 &= ~0x10000;
                        } else {
                            self->field_0x7A4 |= 0x10000;
                        }
                    }
                }
            }
            if (oldBit16 != ((self->field_0x7A4 >> 16) & 1)) {
                ((CScnItemModel*)self)->vfuncA4(1);
            }
            self->field_0x7A8 |= 0x400;
        }
        self->field_0x7A8 &= ~0x400;
    }
    u32 f4 = self->field_0x7A4;
    if (!(f4 & 0x04000000) && (f4 & 2) && !(f4 & 0x8000) && !(f4 & 0x10000)) {
        // all-set: retail jumps straight to the epilogue
    } else {
        if ((f4 & 1) && (f4 & 0x01000000) && !(f4 & 0x20)) {
            if (self->field_854 == 0) {
                self->field_0x7A0 &= ~2;
            }
        }
        if (!(self->field_0x7A8 & 0x1000)) {
            func_804EB798(self->field_0x17A0);
        }
    }
}

extern "C" void func_80488B50();

// func_804888B4: after linking `other` into this model's reference list
// (func_804858C8), append other's scene object to the +0x1474 ScnGroup
// (Insert at mNumScnObj), then forward to func_80488B50. The forward call
// goes through a cast pointer so the empty stub is not auto-inlined away.
u32 func_804888B4(CScnItemModelNw4r* self, CScnItemModelNw4r* other) {
    if (func_804858C8(self, other)) {
        nw4r::g3d::ScnGroup* group = self->field_0x1474;
        u32 r = group->Insert(group->Size(), other->field_0x147C);
        ((void (*)(CScnItemModelNw4r*, u32, CScnItemModelNw4r*))func_80488B50)(
            self, 0, other);
        return r;
    }
    return 0;
}

// func_80488938: buffer space check for the +0x860 pool. Returns whether
// `amount` more bytes fit in the 0xC00-byte region starting at +0x864.
// Unsigned `>` selects MWCC's branchless cntlzw compare idiom (xor/cntlzw/
// slw/srwi 31 - the high differing bit of (free ^ amount) is tested in free).
u32 func_80488938(CScnItemModelNw4r* self, u32 amount) {
    u32 free = 0xC00 - self->field_0x860;
    return free > amount;
}

// Buffer sub-allocation from the +0x860 counter: check amount < 0xC00 - used,
// return a pointer into the +0x864 region, bump the counter.
struct CScnItemModelNw4rAlloc {
    u8 _pad[0x860];
    volatile u32 used;    // 0x860
    u8 data[0x864];       // buffer base 0x864 (1-byte member to anchor offset)
};

#pragma push
#pragma optimization_level 1
extern "C" void* func_80488954(void* self, u32 amount) {
    CScnItemModelNw4rAlloc* buf = (CScnItemModelNw4rAlloc*)self;
    // The +0x860 counter is read again inside the success path (retail
    // reloads it: the check and the update are separate volatile reads).
    if (amount < 0xC00u - buf->used) goto success;
    return 0;
success:
    u32 used = buf->used;
    u8* p = (u8*)buf + used;
    buf->used = used + amount;
    return p + 0x864;
}
#pragma pop

// func_80488984: (re)build the +0x1494 node table. Sets the model's
// working param (func_804857DC), then sizes the table from the resource's
// node count (8 bytes per node). When it fits in the +0x864 static pool,
// carve it out of the +0x860 counter and flag it (0x85C bit 2); otherwise
// allocate from the owner's heap. The +0x860 reads are volatile (retail
// reloads the counter for the check and for the carve).
void func_80488984(CScnItemModelNw4r* self, u32 param) {
    func_804857DC(self, param);
    u32 size = nw4r::g3d::ResMdl(self->field_0x146C).GetResNodeNumEntries() * 8;
    CScnItemModelNw4rAlloc* pool = (CScnItemModelNw4rAlloc*)self;
    if (size < 0xC00u - pool->used) {
        u32 used = pool->used;
        u8* p = pool->data + used;
        pool->used = used + size;
        self->field_0x1494 = p;
        self->field_0x85C |= 4;
    } else {
        self->field_0x1494 = mtl::MemManager::allocate_head(
            func_80496018(self->field_04), size, 4);
    }
}

// func_80488A28: link `other` into this model's reference list
// (func_80485994), append other's scene object to the +0x1478 ScnGroup
// (Insert at its Size), then forward to func_80488B50. Returns the Insert
// result; 0 when the link was rejected.
u32 func_80488A28(CScnItemModelNw4r* self, CScnItemModelNw4r* other) {
    if (func_80485994(self, other) == 0) {
        return 0;
    }
    nw4r::g3d::ScnGroup* group = self->field_0x1478;
    u32 r = group->Insert(group->Size(), other->field_0x147C);
    ((void (*)(CScnItemModelNw4r*, u32, CScnItemModelNw4r*))func_80488B50)(
        self, 0, other);
    return r;
}

// func_80488AAC: like func_80488984, but for the +0x1498 node table
// (func_804857F0 setup, 0x85C bit 3 flag).
void func_80488AAC(CScnItemModelNw4r* self, u32 param) {
    func_804857F0(self, param);
    u32 size = nw4r::g3d::ResMdl(self->field_0x146C).GetResNodeNumEntries() * 8;
    CScnItemModelNw4rAlloc* pool = (CScnItemModelNw4rAlloc*)self;
    if (size < 0xC00u - pool->used) {
        u32 used = pool->used;
        u8* p = pool->data + used;
        pool->used = used + size;
        self->field_0x1498 = p;
        self->field_0x85C |= 8;
    } else {
        self->field_0x1498 = mtl::MemManager::allocate_head(
            func_80496018(self->field_04), size, 4);
    }
}

// func_8048776C: unlink the +0x7C4 chain model. Notify the material chain
// (func_80488B50), drop the chain model's scene object from the +0x1478
// group, release the chain model (func_80495E60), clear the link, free the
// +0x1498 node table unless the +0x85C bit-3 pool flag is set, and clear the
// +0x7A4 bit-0x80 visibility flag. The func_80488B50 call goes through a cast
// pointer (C-linkage forward decl above) so the reloc keeps the unmangled
// name, matching the other call sites.
void func_8048776C(CScnItemModelNw4r* self) {
    if (self->field_0x7C4 != 0) {
        ((void (*)(CScnItemModelNw4r*, u32, CScnItemModelNw4r*))func_80488B50)(
            self, 1, (CScnItemModelNw4r*)self->field_0x7C4);
        self->field_0x1478->Remove(
            ((CScnItemModelNw4r*)self->field_0x7C4)->field_0x147C);
        func_80495E60(self->field_0x7C4);
        self->field_0x7C4 = 0;
        if ((self->field_0x85C & 8) == 0) {
            if (self->field_0x1498 != 0) {
                mtl::MemManager::deallocate(self->field_0x1498);
                self->field_0x1498 = 0;
            }
        } else {
            self->field_0x1498 = 0;
        }
        self->field_0x7A4 &= ~0x100;
        self->field_0x149E = 0;
    }
}

// func_80487818: teardown of a CScnItemModelNw4r: deregister from the owner's
// pool, release the +0x824 node-table buffer holder and the +0x17CC frame
// table (guarded by their 0xFFFFFFFF sentinels), destroy the +0x16C8
// sub-object, release the +0x1494 node table (unless carved from the static
// pool, flag bit 2), unlink and release all four +0x7B4 reference slots, then
// release the g3d scene objects: the +0x1490/+0x1484/+0x1488/+0x148C anim
// objects (removing the latter two from the ScnMdl first, and the anm scene
// from the scene root when it is registered), the CMdlLook handle (vtable-0x8
// dtor), the +0x17A0 CMdlDynamics, the camera hookup (when the flag combo is
// clear), and the +0x1470 group with its +0x1474/+0x1478/+0x1480 children.
// Placed before func_80488B50's definition so the call below resolves to the
// C-linkage alias (line 266) and keeps the unmangled retail reloc.
void func_80487818(CScnItemModelNw4r* self) {
    func_80496D74(&self->field_0xC);
    func_804830AC((CScnItemModel*)self);
    self->member824.field_4 = 0;
    if (self->member824.field_C != 0xFFFFFFFF) {
        if (self->member824.field_0 != 0) {
            mtl::MemManager::deallocate(self->member824.field_0);
            self->member824.field_0 = 0;
        }
    }
    self->member824.field_0 = 0;
    self->member824.field_8 = 0;
    self->member824.field_C = -1;
    self->field_0x17D0 = 0;
    if (self->field_0x17D8 != 0xFFFFFFFF) {
        if (self->field_0x17CC != 0) {
            mtl::MemManager::deallocate(self->field_0x17CC);
            self->field_0x17CC = 0;
        }
    }
    self->field_0x17CC = 0;
    self->field_0x17D4 = 0;
    self->field_0x17D8 = -1;
    __dt__804E5DE0(&self->field_0x16C8);
    if (!(self->field_0x85C & 4)) {
        if (self->field_0x1494 != 0) {
            mtl::MemManager::deallocate(self->field_0x1494);
            self->field_0x1494 = 0;
        }
    } else {
        self->field_0x1494 = 0;
    }
    for (u32 i = 0; i < 4; i++) {
        CScnItemModelNw4r* m = self->slots7B4[i];
        if (m != 0) {
            ((void (*)(CScnItemModelNw4r*, u32,
                       CScnItemModelNw4r*))func_80488B50)(self, 1, m);
            self->field_0x1474->Remove(m->field_0x147C);
            func_80495E60((CScnItemModel*)m);
            self->slots7B4[i] = 0;
        }
    }
    ((CScnItemModelNw4rVtbl*)self)->v44();
    if (self->field_0x1490 != 0) {
        ((nw4r::g3d::G3dObj*)self->field_0x1490)->Destroy();
        self->field_0x1490 = 0;
    }
    if (self->field_0x1488 != 0) {
        ((nw4r::g3d::ScnMdl*)self->field_0x147C)->RemoveAnmObj(
            (nw4r::g3d::AnmObj*)self->field_0x1488);
        ((nw4r::g3d::G3dObj*)self->field_0x1488)->Destroy();
    }
    if (self->field_0x148C != 0) {
        ((nw4r::g3d::ScnMdl*)self->field_0x147C)->RemoveAnmObj(
            (nw4r::g3d::AnmObj*)self->field_0x148C);
        ((nw4r::g3d::G3dObj*)self->field_0x148C)->Destroy();
    }
    if (self->field_0x1484 != 0) {
        CScnItemModelNw4rRoot2888* root =
            (CScnItemModelNw4rRoot2888*)func_8048ECD8(self->field_04);
        if (root->field_0x2888 == (u32)self->field_0x1484) {
            RemoveAnmScn__Q34nw4r3g3d7ScnRootFv(func_8048ECD8(self->field_04));
        }
        ((nw4r::g3d::G3dObj*)self->field_0x1484)->Destroy();
    }
    if (!(self->field_0x85C & 2)) {
        if (self->field_0x17C8 != 0) {
            if (self->field_0x17C8 != 0) {  // retail keeps the dead second check
                ((CScnItemModelNw4rLook*)self->field_0x17C8)->v00(1);
            }
            self->field_0x17C8 = 0;
        }
    }
    func_804EB8A0(self->field_0x17A0);
    u32 f = self->field_0x7A4;
    if (!(f & 0x20) && !(f & 0x100) && !(f & 0x04000000)) {
        func_8048F7A8(self->field_04->field_0x8C, (CScnCamLayout*)self);
    }
    if (self->field_0x1470 != 0) {
        self->field_0x1470->Remove((nw4r::g3d::ScnObj*)self->field_0x1474);
        self->field_0x1470->Remove((nw4r::g3d::ScnObj*)self->field_0x1478);
        self->field_0x1470->Remove((nw4r::g3d::ScnObj*)self->field_0x1480);
        ((nw4r::g3d::G3dObj*)self->field_0x1480)->Destroy();
        self->field_0x1480 = 0;
        ((nw4r::g3d::G3dObj*)self->field_0x1474)->Destroy();
        self->field_0x1474 = 0;
        ((nw4r::g3d::G3dObj*)self->field_0x1478)->Destroy();
        self->field_0x1478 = 0;
        ((nw4r::g3d::G3dObj*)self->field_0x1470)->Destroy();
        self->field_0x1470 = 0;
    }
}

// func_80488B50: notify the +0x16C8 material sub-object of every live model
// in the +0x7B4 reference list and the +0x7C4 link (skipping `other`), then
// notify this model and `other` themselves. func_804E64B0 propagates the
// (param, model) pair to each sub-object.
void func_80488B50(CScnItemModelNw4r* self, u32 param, CScnItemModelNw4r* other) {
    for (u32 i = 0; i < 4; i++) {
        CScnItemModelNw4r* m = self->slots7B4[i];
        if (m != 0 && m != other) {
            func_804E64B0(&m->field_0x16C8, param, other);
            func_804E64B0(&other->field_0x16C8, param, m);
        }
    }
    CScnItemModelNw4r* link = (CScnItemModelNw4r*)self->field_0x7C4;
    if (link != 0 && link != other) {
        func_804E64B0(&link->field_0x16C8, param, other);
        func_804E64B0(&other->field_0x16C8, param, link);
    }
    func_804E64B0(&self->field_0x16C8, param, other);
    func_804E64B0(&other->field_0x16C8, param, self);
}

void func_804E5E38(u8* self);
void func_80488C20(u8* self) { ((void(*)(void*))func_804E5E38)((char*)self + 0x16c8); }

// func_80488C28: like func_80488D14, but the chain notify carries an argument.
void func_80488C28(CScnItemModelNw4r* self, u32 param) {
    if (func_804E5FD4(&self->field_0x16C8)) {
        func_804849E4(self, param);
    }
}

// func_80488C78: when the material sub-object reports activity, OR its
// status with the chain-wide 2-arg query and return whether either is
// non-zero (bool-ized via retail's neg/or/srwi setnz).
int func_80488C78(CScnItemModelNw4r* self, u32 a, u32 b) {
    int v = func_804E6158(&self->field_0x16C8);
    if (v != 0) {
        v = (v | func_80484AB4(self, a, b)) != 0;
    }
    return v;
}

// func_80488CF8: copy the 12-byte block at src onto self+0x76C, then
// propagate to linked models (tail call). The pair is loaded as two words
// and stored reversed (w1 at +4, w0 at +0); `#pragma scheduling off` is
// required to stop the -O4 scheduler hoisting the tail load above the pair
// stores (retail keeps lwz r5,0; lwz r0,4; stw; stw; lwz r0,8; stw).
#pragma push
#pragma scheduling off
void func_80488CF8(CScnItemModelNw4r* self, const CScnItemModelNw4rBlock76C* src) {
    const u32* words = reinterpret_cast<const u32*>(src);
    u32 w0 = words[0];
    u32 w1 = words[1];
    self->field_0x770 = w1;
    self->field_0x76C = w0;
    self->field_0x774 = words[2];
    func_80484BB4(self);
}
#pragma pop

// func_80488D14: when the material sub-object reports activity
// (func_804E6358), notify the whole model chain (func_80484C84 walks the
// reference lists calling the vtable-0x98 virtual).
void func_80488D14(CScnItemModelNw4r* self) {
    if (func_804E6358(&self->field_0x16C8)) {
        func_80484C84(self);
    }
}

// func_80488D54: when the material sub-object reports activity
// (func_804E6158 with a zero color vector), reset the anim helpers and
// update the +0x78C..0x798 color quadruple from the 12-float table at
// lbl_eu_8056DE50 (4 groups of 3), selecting the row by the +0x7A8 mode
// bits 0x10/0x20/0x40 (falling back to `arg2`). The vtable-0x9C chain
// notify (func_80484D3C) is always issued at the end.
void func_80488D54(CScnItemModelNw4r* self, u32 arg2, u32 arg3) {
    const float* tbl = lbl_eu_8056DE50;
    u32 mode = arg2;
    ml::CVec3 vec;
    vec.x = lbl_eu_8066A8FC;
    vec.y = lbl_eu_8066A8FC;
    vec.z = lbl_eu_8066A8FC;
    if (func_804E6158(&self->field_0x16C8, &vec, 0) != 0) {
        if (arg3 == 0) {
            func_80482918(self, 1);
            func_804829E8(self, 1);
            self->field_0x7A0 |= 8;
        }
        u32 flags = self->field_0x7A8;
        if (flags & 0x10) {
            mode = 0;
        } else if (flags & 0x20) {
            mode = 1;
        } else if (flags & 0x40) {
            mode = 2;
        }
        switch (mode) {
        case 3:
            self->field_0x7A0 |= 0x10;
            // fall through
        case 0:
            self->field_0x790 = tbl[0];
            self->field_0x78C = tbl[6];
            self->field_0x798 = tbl[3];
            self->field_0x794 = tbl[9];
            break;
        case 1:
            self->field_0x790 = tbl[1];
            self->field_0x78C = tbl[7];
            self->field_0x798 = tbl[4];
            self->field_0x794 = tbl[10];
            break;
        case 2:
            self->field_0x790 = tbl[2];
            self->field_0x78C = tbl[8];
            self->field_0x798 = tbl[5];
            self->field_0x794 = tbl[11];
            break;
        }
    }
    func_80484D3C(self, mode, 1);
}

// func_80488EF4: reset both material/anim helpers to 0, then invoke the
// no-arg vtable-0x98 notify (CScnItemModel::vfunc98) on the model.
void func_80488EF4(CScnItemModelNw4r* self) {
    func_80482918(self, 0);
    func_804829E8(self, 0);
    ((CScnItemModel*)self)->vfunc98();
}

// func_80488F44: add `value` to the +0x17CC u32 frame table when absent
// (linear scan), then bubble-sort the table ascending (adjacent swaps,
// repeat while a pass changed anything).
void func_80488F44(CScnItemModelNw4r* self, u32 value) {
    u32 count = self->field_0x17D0;
    u32* p = self->field_0x17CC;
    u32* end = p + count;
    while (p != end && *p != value) {
        p++;
    }
    if (p != end) {
        return;
    }
    u32 c = self->field_0x17D0;
    self->field_0x17CC[c] = value;
    self->field_0x17D0 = c + 1;

    u32 swapped;
    do {
        swapped = 0;
        u32* q = self->field_0x17CC;
        while (q != self->field_0x17CC + self->field_0x17D0) {
            if (q + 1 == self->field_0x17CC + self->field_0x17D0) break;
            u32 a = q[0];
            u32 b = q[1];
            if (a > b) {
                q[1] = a;
                q[0] = b;
                swapped = 1;
            }
            q++;
        }
    } while (swapped);
}

void func_80488FEC(CScnItemModelNw4r* self) {
    s32 v = self->field_0x17C8;
    if (v == 0) return;
    func_804E8220(v);
}

void func_80489000(CScnItemModelNw4r* self) {
    s32 v = self->field_0x17C8;
    if (v == 0) return;
    func_804E8284(v);
}

// __dt__17CScnItemModelNw4rFv: CScnItemModelNw4r destructor, fragment-function
// form (like __dt__13CScnItemModelFv). Installs the vtable label at +0 and
// the two secondary vtable pointers at +0x1464/+0x1468 (vtable+0xDC / +0xF4),
// destroys the g3d scene object, releases the +0x17CC frame-table buffer
// (the inlined cleanup keeps retail's dead `addic. this+0x17cc` null test),
// then destroys the sub-objects in reverse member order (CMdlDynamics down
// to CScnEffectActNw4r), the CScnItemModel base, and the object itself when
// the deletion flag is positive.
CScnItemModelNw4r* __dt__17CScnItemModelNw4rFv(CScnItemModelNw4r* self,
                                               int deleting) {
    if (self != nullptr) {
        u8* vt = (u8*)lbl_eu_8056DE80;
        *(void**)self = (void*)vt;
        *(void**)((u8*)self + 0x1464) = vt + 0xDC;
        *(void**)((u8*)self + 0x1468) = vt + 0xF4;
        self->field_0x147C->Destroy();
        if (&self->field_0x17CC != 0) {
            self->field_0x17D0 = 0;
            if (self->field_0x17D8 != 0xFFFFFFFF) {
                if (self->field_0x17CC != 0) {
                    mtl::MemManager::deallocate(self->field_0x17CC);
                    self->field_0x17CC = 0;
                }
            }
            self->field_0x17CC = 0;
            self->field_0x17D4 = 0;
            self->field_0x17D8 = -1;
        }
        __dt__12CMdlDynamicsFv((u8*)self + 0x17A0, -1);
        __dt__10CMdlAnmEyeFv((u8*)self + 0x1770, -1);
        __dt__9CMdlAnmUVFv((u8*)self + 0x1730, -1);
        __dt__9CMdlMouthFv((u8*)self + 0x1700, -1);
        __dt__12CMdlMaterialFv((u8*)self + 0x16C8, -1);
        __dt__4CScnFvMaruShadowNw4r((u8*)self + 0x1650, -1);
        __dt__17CScnEffectActNw4rFv((u8*)self + 0x14C4, -1);
        __dt__13CScnItemModelFv((CScnItemModel*)self, 0);
        if (deleting > 0) {
            operator delete(self);
        }
    }
    return self;
}

// func_80489014: per-frame model pose hook - resolve the node selected by
// `frame`'s +6 id against the +0x14A0 node index; when they match, refresh
// the two +0x7CC/+0x7D8 translate outputs from the anim sub-object at +0xC
// (func_804979A4), rotate them by the act transform's local matrix, fold
// them into the transform's position, then (unless the bit-5 busy flag or a
// link id is set) refresh the transform and push it into the scene object.
// Otherwise fall back to the UV/mouth anim helpers (func_804E72D0 /
// func_804E68A0) gated on the vtable-0x74 handled query.
void func_80489014(CScnItemModelNw4r* self, nw4r::g3d::ChrAnmResult* out,
                   u32 unused, CScnItemModelNw4rFrameOut* frame) {
    if (self->field_0x14A0 == frame->field_6) {
        if (((CScnItemModelNw4rAnimC*)self->field_0xC)->field_0x0 != 0) {
            nw4r::math::VEC3 v;
            func_804979A4((void*)self->field_0xC, (ml::CVec3*)&self->vec7CC,
                          (ml::CVec3*)&self->vec7D8, (ml::CVec3*)&v,
                          self->field_0x14A0);
            out->SetTranslate(&v);
            {
                const nw4r::math::MTX34* mtx =
                    (const nw4r::math::MTX34*)&func_8048315C(self)->mLocalMat;
                nw4r::math::VEC3TransformNormal(&self->vec7CC, mtx,
                                                &self->vec7CC);
            }
            {
                const nw4r::math::MTX34* mtx =
                    (const nw4r::math::MTX34*)&func_8048315C(self)->mLocalMat;
                nw4r::math::VEC3TransformNormal(&self->vec7D8, mtx,
                                                &self->vec7D8);
            }
            {
                ml::CAttrTransform* t = func_8048315C(self);
                nw4r::math::VEC3Add((nw4r::math::VEC3*)&t->mPos,
                                    (nw4r::math::VEC3*)&t->mPos,
                                    &self->vec7CC);
            }
        }
        if ((self->field_0x7A4 & 0x20) == 0 && self->field_854 == 0) {
            ml::CAttrTransform* t = (ml::CAttrTransform*)func_8048315C(self);
            t->update();
            t = (ml::CAttrTransform*)func_8048315C(self);
            self->field_0x147C->SetMtx(
                nw4r::g3d::ScnObj::MTX_LOCAL,
                *(const nw4r::math::MTX34*)&t->mLocalMat);
        }
        out->GetTranslate(&self->field_0x14B4);
        return;
    }
    if (func_804E72D0(&self->field_0x1730, frame->field_6, out) == 0) {
        if (((CScnItemModelNw4rV74*)self)->v27() == 0) {
            func_804E68A0(&self->field_0x1700, frame->field_6, out);
        }
    }
}

// func_8048917C: feed the CMdlLook handle (+0x17C8) with the current frame
// value (retail keeps the u16 slot at out+6), then advance the +0x16C4 u16
// frame counter; while it stays under the +0x17D0 table count, write the
// table entry (+0x17CC, u32) into the caller's u16 slot at +6.
void func_8048917C(CScnItemModelNw4r* self, u32 param2, u32 param3,
                   CScnItemModelNw4rFrameOut* out) {
    if (self->field_0x17C8 != 0) {
        func_804E8290(self->field_0x17C8, self, out->field_6, param2);
    }
    u32 counter = self->field_0x16C4;
    u32 next = counter + 1;
    u32 masked = (u16)next;
    self->field_0x16C4 = (u16)next;
    if (masked < self->field_0x17D0) {
        out->field_6 = self->field_0x17CC[(u16)next];
    }
}

void func_80489200(){}

// func_80489584: per-frame transform/bounding refresh for the Nw4r model,
// invoked with mode == 4. When the +0x7A4 bit-30 flag is set, normalize the
// 3x3 rotation columns of each +0x844 matrix (nw4r FSqrt-style guarded
// inverse-length with the inlined Warning; zero-length columns skipped),
// then run every +0x7F0 hook with this model as the arg. The +0x2DC
// bounding vec is refreshed from the +0x14AC source (translation column +
// radius * |row0|, plus a copy of the source matrix into the act-data mMtx1
// with the FLAG_0 raise and update when linked) or from the halved
// bounding-box sums scaled by the +0x304/+0x308/+0x30C scale triple (max
// scale * radius), rotated by mLocalMat. Finally the light-env is updated
// through the owner's virtual light (func_80493C30), propagated
// (func_80485804) and refreshed (func_804BFFB8) when the visibility flags
// allow.
void func_80489584(CScnItemModelNw4r* self, int param) {
    if (param != 4) {
        return;
    }
    if (self->field_0x7A4 & 0x40000000) {
        nw4r::math::VEC3 v;                 // single stack slot reused by all 12 columns
        nw4r::math::VEC3* vp = &v;          // keep &v in a saved reg across the calls (retail r26)
        for (int i = 0; i < 4; i++) {
            nw4r::math::MTX34* mtx = (nw4r::math::MTX34*)self->field_844[i];
            if (mtx == 0) continue;
            for (int j = 0; j < 3; j++) {
                v.x = mtx->m[0][j];
                v.y = mtx->m[1][j];
                v.z = mtx->m[2][j];
                f32 len2 = nw4r::math::VEC3LenSq(vp);
                if (len2 == lbl_eu_8066A8E0) continue;
                // inlined nw4r FSqrt domain check (retail file/line labels)
                if (!(len2 >= lbl_eu_8066A8E0)) {
                    nw4r::db::Warning(lbl_eu_80526324, 0x273, lbl_eu_80526300);
                }
                f32 len = len2 <= lbl_eu_8066A8E0
                              ? lbl_eu_8066A8E0
                              : len2 * nw4r::math::FrSqrt(len2);
                f32 scale = lbl_eu_8066A8FC / len;
                nw4r::math::VEC3Scale(vp, vp, scale);
                mtx->m[0][j] = v.x;
                mtx->m[1][j] = v.y;
                mtx->m[2][j] = v.z;
            }
        }
    }
    for (int i = 0; i < self->field_0x820; i++) {
        ((CScnItemModelNw4rHookV0C*)self->field_0x7F0[i])->v02(self);
    }
    CScnItemModelNw4r14AC* src = self->field_0x14AC;
    if (src != 0) {
        self->field_0x2DC[0] = src->f0xC;
        self->field_0x2DC[1] = src->f0x1C;
        self->field_0x2DC[2] = src->f0x2C;
        f32 mag = PSVECMag((const Vec*)&src->vec0);
        self->field_0x2E8 = self->field_0x7AC * mag;
        if (self->field_854 != 0) {
            ml::CAttrTransform* t = func_8048315C(self);
            t->mMtx1 = *(const ml::CMat34*)src;
            t->mFlags |= ml::CAttrTransform::FLAG_0;
            func_8048315C(self)->update();
        }
    } else {
        self->field_0x2DC[0] =
            self->field_0x304 *
            (lbl_eu_8066A904 * (self->field_0x2EC[3] + self->field_0x2EC[0]));
        self->field_0x2DC[1] =
            self->field_0x308 *
            (lbl_eu_8066A904 * (self->field_0x2EC[4] + self->field_0x2EC[1]));
        self->field_0x2DC[2] =
            self->field_0x30C *
            (lbl_eu_8066A904 * (self->field_0x2EC[5] + self->field_0x2EC[2]));
        f32 mx = self->field_0x308 > self->field_0x304 ? self->field_0x308
                                                       : self->field_0x304;
        if (self->field_0x30C > mx) {
            mx = self->field_0x30C;
        }
        self->field_0x2E8 = self->field_0x7AC * mx;
        ml::CAttrTransform* t = func_8048315C(self);
        PSMTXMultVec(t->mLocalMat.m, (const Vec*)&self->field_0x2DC[0],
                     (Vec*)&self->field_0x2DC[0]);
    }
    self->field_0x7A4 |= 0x80000000;
    if ((self->field_0x7A4 & 1) && (self->field_0x7A4 & 0x01000000) &&
        !(self->field_0x7A4 & 0x20) && self->field_854 == 0) {
        int mode = ((self->field_0x7A4 & 2) && !(self->field_0x7A4 & 0x10000))
                       ? 1
                       : 0;
        ml::CVec3 pos;
        pos.x = self->field_0x2DC[0];
        pos.y = self->field_0x2DC[1];
        pos.z = self->field_0x2DC[2];
        func_80493C30(self->field_04->field_0x5C,
                      (CScnEnvLgtData*)&self->field_0x31C, &pos, mode,
                      self->field_0x2E8);
        func_80485804((CScnItemModel*)self, 0);
        func_804BFFB8((CScnEnvLgtData*)&self->field_0x31C);
    }
}

void func_80489924(){}

// func_804899F4: light-env mode switch - on mode 1, init the env light from
// the +0x31C sub-object when the +0x7A4 bit-0 flag is set, else clear it;
// then reset the +0x7A4 0x80/0x200 bits (mask ~0x280).
void func_804899F4(CScnItemModelNw4r* self, s32 mode) {
    if (mode == 1) {
        if (self->field_0x7A4 & 1) {
            func_804C1720(self->field_04->field_7C, &self->field_0x31C);
        } else {
            func_804C172C(self->field_04->field_7C);
        }
    }
    self->field_0x7A4 &= ~0x280;
}

// func_80489A60: build a new CScnItemModelNw4r from the `index`-th model of
// the given ResFile (must be 32-byte aligned) and register it in this
// model's scene-item pool. When the owner's memory region cannot hold a
// 0x181C-byte block, fall back to a tail allocation from the MEM2 region
// (align 0x20). The +0x7A4 0x800000 flag mirrors the ResFile bind result.
// Returns the new model, or 0 when busy / out of pool space / the file is
// invalid.
CScnItemModelNw4r* func_80489A60(CScnItemModelNw4r* self,
                                 nw4r::g3d::ResFileData* resFileData,
                                 u32 param, u32 enable, int index, u32 flags) {
    if (self->field_0x31C.field_0x3E4 != 0) {
        return 0;
    }
    if (func_8048C5B8(self->field_0x60, 1) == 0) {
        return 0;
    }
    nw4r::g3d::ResFile resFile(resFileData);
    if ((u32)resFile.ptr() & 0x1F) {
        nw4r::db::Panic(lbl_eu_8052637C, 0x3C, lbl_eu_80526354);
    }
    if (resFile.ptr() == 0) {
        return 0;
    }
    if (!resFile.CheckRevision()) {
        return 0;
    }
    if (resFile.GetResMdlNumEntries() == 0) {
        return 0;
    }
    resFile.Init();
    u32 bound = resFile.Bind(resFile);
    if (resFile.GetResMdl(index).ptr() == 0) {
        return 0;
    }
    if (enable != 0) {
        flags |= 1;
    }
    mtl::ALLOC_HANDLE handle = func_80495FF0(self);
    CScnItemModelNw4r* model;
    if (mtl::MemManager::getMaxAllocSize(handle) < 0x3038) {
        model = (CScnItemModelNw4r*)mtl::MemManager::allocate_ex(
            0x181C, mtl::MemManager::getHandleMEM2(), -0x20);
        if (model != 0) {
            nw4r::g3d::ResMdl resMdl = resFile.GetResMdl(index);
            model = __ct__CScnItemModelNw4r(model, self, &resMdl,
                                            resFileData, param, flags);
        }
    } else {
        model = (CScnItemModelNw4r*)mtl::MemManager::allocate(0x181C, handle);
        if (model != 0) {
            nw4r::g3d::ResMdl resMdl = resFile.GetResMdl(index);
            model = __ct__CScnItemModelNw4r(model, self, &resMdl,
                                            resFileData, param, flags);
        }
    }
    if (model == 0) {
        return 0;
    }
    u8* pool = self->field_0x60;  // retail loads the pool before the vtable call
    u32 listId = ((CScnItemModelNw4rVtbl*)model)->v04();
    func_8048C630(pool, model, listId);
    if (!bound) {
        model->field_0x7A4 |= 0x800000;
    } else {
        model->field_0x7A4 &= ~0x800000;
    }
    return model;
}

// func_80489C94: build a new CScnItemModelNw4r from the first model of the
// given ResFile (must be 32-byte aligned) and register it in this model's
// scene-item pool. The +0x7A4 0x800000 flag mirrors the ResFile bind result;
// +0x7A8 bit 9 is raised on the new model. Returns the new model, or 0 when
// busy / out of pool space / the file is invalid.
CScnItemModelNw4r* func_80489C94(CScnItemModelNw4r* self,
                                 nw4r::g3d::ResFileData* resFileData,
                                 u32 param) {
    if (self->field_0x31C.field_0x3E4 != 0) {
        return 0;
    }
    if (func_8048C5B8(self->field_0x60, 1) == 0) {
        return 0;
    }
    nw4r::g3d::ResFile resFile(resFileData);
    if ((u32)resFileData & 0x1F) {
        nw4r::db::Panic(lbl_eu_8052637C, 0x3C, lbl_eu_80526354);
    }
    if (resFile.ptr() == 0) {
        return 0;
    }
    if (resFile.GetResMdlNumEntries() == 0) {
        return 0;
    }
    if (!resFile.CheckRevision()) {
        return 0;
    }
    resFile.Init();
    int bound = resFile.Bind(resFile);
    if (resFile.GetResMdl(0).ptr() == 0) {
        return 0;
    }
    CScnItemModelNw4r* model;  // declared before handle (retail: model=r30, handle=r29)
    mtl::ALLOC_HANDLE handle = func_80495FF0(self);
    if (mtl::MemManager::getMaxAllocSize(handle) < 0x3038) {
        return 0;
    }
    model = (CScnItemModelNw4r*)mtl::MemManager::allocate(0x181C, handle);
    if (model != 0) {
        nw4r::g3d::ResMdl resMdl = resFile.GetResMdl(0);
        model = __ct__CScnItemModelNw4r(model, self, &resMdl, resFileData,
                                        param, 0x11);
    }
    if (model == 0) {
        return 0;
    }
    u8* pool = self->field_0x60;  // retail loads the pool before the vtable call
    u32 listId = ((CScnItemModelNw4rVtbl*)model)->v04();
    func_8048C630(pool, model, listId);
    if (!bound) {
        model->field_0x7A4 |= 0x800000;
    } else {
        model->field_0x7A4 &= ~0x800000;
    }
    model->field_0x7A8 |= 0x200;
    ((CScnItemModelNw4rVtbl*)model)->v40();
    return model;
}

// func_80489E80: link `other` into this model's second reference list
// (slots834) and push its scene object into the +0x1480 ScnMdlExpand group
// at `priority`. When `mtx` is present, its matrix is copied into the
// linked model's act-data transform (mMtx1) and the transform's FLAG_0 is
// raised; otherwise the transform is cleared. Returns the PushBack result.
bool func_80489E80(CScnItemModelNw4r* self, CScnItemModelNw4r* other,
                   u32 priority, const ml::CMat34* mtx) {
    if (self->field_04->field_0x3E4 != 0) {
        return false;
    }
    if (self->field_0x1480 == 0) {
        return false;
    }
    nw4r::g3d::ScnObj* scnObj = other->field_0x147C;
    ((CScnItemModelNw4rVtbl*)self)->v17();
    ((CScnItemModelNw4rVtbl*)other)->v16();
    bool pushed = PushBack__Q34nw4r3g3d12ScnMdlExpandFPQ34nw4r3g3d6ScnObjUl(
        (nw4r::g3d::ScnMdlExpand*)self->field_0x1480, scnObj, priority);
    if (pushed) {
        if (mtx != 0) {
            ((ml::CAttrTransform*)((u8*)other + 0x1F8))->mMtx1 = *mtx;
            ((ml::CAttrTransform*)((u8*)other + 0x1F8))->mFlags |=
                ml::CAttrTransform::FLAG_0;
        } else {
            ((ml::CAttrTransform*)((u8*)other + 0x1F8))->clear();
        }
        for (u32 i = 0; i < 4; i++) {
            if (self->slots834[i] == 0) {
                self->slots834[i] = other;
                other->field_854 = (u32)self;
                other->field_0x14A8 = priority;
                u32 linkId = ((CScnItemModelNw4rVtbl*)self)->v14(priority);
                other->field_0x14B0 = linkId;
                self->field_844[i] = linkId;
                break;
            }
        }
    }
    return pushed;
}

void func_80489FDC(){}

// func_8048A0B4: unlink `other` from this model. Returns false while the
// owner's busy byte (+0x3E4) is set or the +0x1480 group is absent;
// otherwise remove other's scene object from the group and, when the removal
// succeeds, clear the matching +0x834/+0x844 slot pair and other's
// +0x854/+0x14A8 link fields. The removal result is returned.
bool func_8048A0B4(CScnItemModelNw4r* self, CScnItemModelNw4r* other) {
    if (self->field_04->field_0x3E4 != 0) {
        return false;
    }
    nw4r::g3d::ScnGroup* group = self->field_0x1480;
    if (group == 0) {
        return false;
    }
    bool removed = group->Remove(other->field_0x147C);
    if (!removed) {
        return false;
    }
    for (u32 i = 0; i < 4; i++) {
        if (self->slots834[i] == other) {
            self->slots834[i] = 0;
            self->field_844[i] = 0;
            other->field_854 = 0;
            other->field_0x14A8 = -1;
            break;
        }
    }
    return removed;
}

// ===========================================================================
// nw4r g3d ResUserData accessor reconstruction (g3d_resuser_ac.h semantics)
// ===========================================================================
// Mirrors CMdlMaterial.cpp's reconstruction; the last-arg panic strings are
// this call site's own (.sdata) labels. func_8048A17C iterates the +0x824
// node table and scans each node's ResUserData for a named item.

// One named item: name string, value type, and relative offsets.
struct Nw4rUserDataItem {
    u32 field_0x00;      // +0x00
    s32 dataOffset;      // +0x04: relative offset to the value (0 = null)
    u32 field_0x08;      // +0x08
    u32 valueType;       // +0x0C: 0 = S32 (asserted)
    s32 nameOffset;      // +0x10: relative offset to the name string
};

// A single 0x10-byte reference slot in the dic index array.
struct Nw4rUserDataRef {
    s32 dataOffset;      // +0x00: relative offset (from the dic base) to item
    u32 field_0x04;
    u32 field_0x08;
    u32 field_0x0C;
};

// Dic accessor base: sits 4 bytes into the user-data block (ud + 4).
// numData at +0x04, index array (0x10-byte refs) at +0x24.
struct Nw4rUserDataDic {
    u32 field_0x00;
    u32 numData;             // +0x04: number of reference slots
    u32 field_0x08;
    u32 field_0x0C;
    u32 field_0x10;
    u32 field_0x14;
    u32 field_0x18;
    u32 field_0x1C;
    u32 field_0x20;
    Nw4rUserDataRef refs[1];  // +0x24
};

// GetNumData(): null + alignment asserts, then the item count.
static inline u32 ResUserDataNumData(const void* ud, Nw4rUserDataDic* dic) {
    if (ud == NULL) {
        nw4r::db::Panic(lbl_eu_80530D18, 0x57, lbl_eu_80530CFC,
                        lbl_eu_80530CF0, &lbl_eu_8066391C);
    }
    if (reinterpret_cast<u32>(dic) & 3) {
        nw4r::db::Panic(lbl_eu_80530F08, 0x54, lbl_eu_80530EE0);
    }
    return (dic != NULL) ? dic->numData : 0;
}

// operator[](int): null + bounds asserts, then resolve the item pointer.
static inline Nw4rUserDataItem* ResUserDataItemAt(const void* ud,
                                                  Nw4rUserDataDic* dic,
                                                  s32 idx) {
    if (ud == NULL) {
        nw4r::db::Panic(lbl_eu_80530D18, 0x57, lbl_eu_80530CFC,
                        lbl_eu_80530CF0, &lbl_eu_8066391C);
    }
    if (reinterpret_cast<u32>(dic) & 3) {
        nw4r::db::Panic(lbl_eu_80530F08, 0x54, lbl_eu_80530EE0);
    }

    Nw4rUserDataItem* item = NULL;

    if (dic != NULL) {
        if (idx >= 0 && idx <= static_cast<s32>(dic->numData) - 1) {
            item = reinterpret_cast<Nw4rUserDataItem*>(1);  // valid marker
        }
        if (item == NULL) {
            if (dic == NULL) {
                nw4r::db::Panic(lbl_eu_8056E43C, 0x54, lbl_eu_8056E420,
                                &lbl_eu_80663928, &lbl_eu_80663930);
            }
            nw4r::db::Panic(lbl_eu_8056E3D0, 0x2A, lbl_eu_8056E398, idx, 0,
                            static_cast<s32>(dic->numData) - 1);
        }
        if (dic == NULL) {
            nw4r::db::Panic(lbl_eu_8056E43C, 0x54, lbl_eu_8056E420,
                            &lbl_eu_80663928, &lbl_eu_80663930);
        }
        s32 refOff = dic->refs[idx].dataOffset;
        if (refOff != 0) {
            item = reinterpret_cast<Nw4rUserDataItem*>(
                reinterpret_cast<u8*>(dic) + refOff);
        } else {
            item = NULL;
        }
    }

    return item;
}

// GetName(): alignment + null asserts, then the name string.
static inline const char* Nw4rItemGetName(Nw4rUserDataItem* item) {
    if (reinterpret_cast<u32>(item) & 3) {
        nw4r::db::Panic(lbl_eu_80530D54, 0x26, lbl_eu_80530D2C);
    }
    if (item == NULL) {
        nw4r::db::Panic(lbl_eu_80530DC4, 0x26, lbl_eu_80530DA8,
                        lbl_eu_80530D68, &lbl_eu_80663924);
    }
    return (item->nameOffset != 0) ? (const char*)item + item->nameOffset
                                   : NULL;
}

// GetData(): null + valueType + null asserts, then the data pointer.
static inline void* Nw4rItemGetData(Nw4rUserDataItem* item) {
    if (item == NULL) {
        nw4r::db::Panic(lbl_eu_80530DC4, 0x26, lbl_eu_80530DA8,
                        lbl_eu_80530D68, &lbl_eu_80663924);
    }
    if (item->valueType != 0) {
        nw4r::db::Panic(lbl_eu_80530E1C, 0x36, lbl_eu_80530DD8);
    }
    if (item == NULL) {
        nw4r::db::Panic(lbl_eu_80530D94, 0x26, lbl_eu_80530D78,
                        lbl_eu_80530D68, &lbl_eu_80663920);
    }
    return (item->dataOffset != 0) ? reinterpret_cast<u8*>(item) +
                                        item->dataOffset
                                   : NULL;
}

// func_8048A17C: scan the +0x824 node table (walked by 4-byte entries against
// a reloaded base+count*4 end, retail keeps the reloads). For every node, open
// its nw4r ResUserData and scan the named items: an item whose name matches
// the global lbl_eu_806638D0 target raises the entry's +2 flag bit 1; when its
// data slot (item+dataOffset) holds a non-zero first word, the +0x7A4 bit-2
// flag is raised on the model.
void func_8048A17C(CScnItemModelNw4r* self) {
    for (CScnItemModel824Entry* entry =
             (CScnItemModel824Entry*)self->member824.field_0;
         entry != (CScnItemModel824Entry*)((u8*)self->member824.field_0 +
                                           self->member824.field_4 * 4);
         entry++) {
        nw4r::g3d::ResMdl mdl(self->field_0x146C);
        nw4r::g3d::ResNode node = mdl.GetResNode((u32)entry->field_0);
        void* ud = node.GetResUserData();
        if (ud == NULL) continue;

        Nw4rUserDataDic* dic =
            reinterpret_cast<Nw4rUserDataDic*>(reinterpret_cast<u8*>(ud) + 4);
        for (u32 i = 0; i < ResUserDataNumData(ud, dic); i++) {
            Nw4rUserDataItem* item = ResUserDataItemAt(ud, dic, (s32)i);
            const char* name = Nw4rItemGetName(item);
            if (strcmp(name, lbl_eu_806638D0) == 0) {
                if (Nw4rItemGetData(item) != NULL) {
                    entry->flags |= 2;
                } else {
                    entry->flags &= ~2;
                }
                if (*(const u32*)Nw4rItemGetData(item) != 0) {
                    self->field_0x7A4 |= 4;
                }
            }
        }
    }
}

// func_8048A588: material fade/colour refresh for the Nw4r model, called with
// a fade `param`. When field_0x858 already equals param nothing happens.
// param == 0: when the +0x7A4 bit-10 flag is set, reset every material's
// blend/alpha state from the copied resource (GX blend + alpha compare), then
// clear the +0x7A4 bits 0x300000/0x10000 and notify via vtable-0xA4.
// param >= 1: set the +0x7A4 0x8000 flag and notify. 0 < param < 1: run the
// material fade loop - per material, re-copy the GX state and write a tev
// color (or a k-color when the per-material count exceeds 1) computed from
// param; the +0x7A4 0xC00 bits are raised and bit 0x10000 cleared before the
// notify. The model's material chain is refreshed with param at the end
// (func_80484838).
void func_8048A588(CScnItemModelNw4r* self, f32 param) {
    if (self->field_0x858 == param) {
        return;
    }
    if (param != lbl_eu_8066A8E0) {
        if (param >= lbl_eu_8066A8FC) {
            self->field_0x7A4 |= 0x8000;
            ((CScnItemModel*)self)->vfuncA4(1);
        } else {
            u32 numMats = nw4r::g3d::ResMdl(self->field_0x146C)
                              .GetResMatNumEntries();
            for (u32 i = 0; i < numMats; i++) {
                nw4r::g3d::ScnMdl::CopiedMatAccess access(
                    (nw4r::g3d::ScnMdl*)self->field_0x147C, i);
                nw4r::g3d::ResMat mat =
                    nw4r::g3d::ResMdl(self->field_0x146C).GetResMat(i);
                // [elided retail middle: GetResTev + GXSetNumTevStages +
                // blend/alpha re-copy per material, and the per-material
                // count at the stack slot compared against 1]
                nw4r::g3d::ResMatTevColor tevColor =
                    GetResMatTexCoordGenEx__Q44nw4r3g3d6ScnMdl15CopiedMatAccessFv(
                        &access);
                if (numMats <= 1) {
                    f32 alpha = lbl_eu_8066A8E4 * (lbl_eu_8066A8FC - param);
                    GXColor color;
                    color.r = 0xFF;
                    color.g = 0xFF;
                    color.b = 0xFF;
                    color.a = (u8)alpha;
                    tevColor.GXSetTevColor((GXTevRegID)1, color);
                    tevColor.DCStore(false);
                } else {
                    f32 k0 = lbl_eu_8066A8E4 * lbl_eu_8066A8E0;
                    f32 k1 = lbl_eu_8066A8E4 * (lbl_eu_8066A8FC - param);
                    GXColor color;
                    color.r = (u8)k0;
                    color.g = (u8)k0;
                    color.b = (u8)k0;
                    color.a = (u8)k1;
                    tevColor.GXSetTevKColor((GXTevKColorID)3, color);
                    tevColor.DCStore(false);
                }
            }
            self->field_0x7A4 |= 0xC00;
            self->field_0x7A4 &= ~0x10000;
            ((CScnItemModel*)self)->vfuncA4(1);
        }
    } else {
        if (self->field_0x7A4 & 0x400) {
            u32 numMats = nw4r::g3d::ResMdl(self->field_0x146C)
                              .GetResMatNumEntries();
            for (u32 i = 0; i < numMats; i++) {
                nw4r::g3d::ScnMdl::CopiedMatAccess access(
                    (nw4r::g3d::ScnMdl*)self->field_0x147C, i);
                nw4r::g3d::ResMat mat =
                    nw4r::g3d::ResMdl(self->field_0x146C).GetResMat(i);
                nw4r::g3d::ResTev tev =
                    nw4r::g3d::ResMdl(self->field_0x146C).GetResTev(i);
                nw4r::g3d::ResGenMode genMode = access.GetResGenMode(false);
                if (tev.ptr() == 0) {
                    nw4r::db::Panic(lbl_eu_8056E0D0, 0x25, lbl_eu_8056E0B0,
                                    &lbl_eu_806638F4, &lbl_eu_806638FC);
                }
                genMode.GXSetNumTevStages(((u8*)tev.ptr())[0xC]);
                nw4r::g3d::ResMatPix pix =
                    GetResMatIndMtxAndScaleEx__Q44nw4r3g3d6ScnMdl15CopiedMatAccessFv(
                        &access);
                if (mat.ptr() == 0) {
                    nw4r::db::Panic(lbl_eu_8056E068, 0x26D, lbl_eu_8056E04C,
                                    &lbl_eu_806638E8, &lbl_eu_806638F0);
                }
                nw4r::g3d::ResMatPix resPix =
                    (mat.ptr() != 0)
                        ? *(nw4r::g3d::ResMatPix*)((u8*)mat.ptr() +
                                                  *(s32*)((u8*)mat.ptr() + 0x3C))
                        : nw4r::g3d::ResMatPix(0);
                if (((u32)resPix.ptr() & 0x1F) != 0) {
                    nw4r::db::Panic(lbl_eu_8056E0A0, 0x154, lbl_eu_8056E078);
                }
                _GXBlendMode blendMode;
                _GXBlendFactor srcFac, dstFac;
                _GXLogicOp logicOp;
                resPix.GXGetBlendMode(&blendMode, &srcFac, &dstFac, &logicOp);
                _GXCompare comp0;
                u8 ref0;
                _GXAlphaOp alphaOp;
                _GXCompare comp1;
                u8 ref1;
                resPix.GXGetAlphaCompare(&comp0, &ref0, &alphaOp, &comp1,
                                         &ref1);
                pix.GXSetBlendMode(blendMode, srcFac, dstFac, logicOp);
                pix.GXSetAlphaCompare(comp0, ref0, alphaOp, comp1, ref1);
                pix.DCStore(false);
            }
            self->field_0x7A4 &= ~0x300000;
        }
        self->field_0x7A4 &= ~0x10000;
        ((CScnItemModel*)self)->vfuncA4(1);
    }
    func_80484838(self, param);
}

extern "C" void func_8048AB0C(u8* self, u32 mode) {
    if ((s32)mode == 1) {
        *(void**)((u8*)self + 0x7EC) = (u8*)self + 0x1650;
    } else {
        *(void**)((u8*)self + 0x7EC) = 0;
    }
}

// func_8048AB2C: per-frame shadow-node world-matrix update. Gated on the
// +0x7A4 bit-3 flag; computes a fade scale (func_80484EB0 result forced to
// 1.0 by the +0x7A8 bit-2 flag, times the owner scale func_80496288, capped
// at 1.0). For each of the two +0x17DC shadow nodes: resolve the resource
// node by id and its world-matrix slot (base + mtxID*0x30) in the caller's
// matrix array; when the entry's +0x1E flag bit 0 is set, reset the offset
// vec (+0xC triple) to zero and copy the matrix translation into the +0x0
// position; otherwise compute the shadow position from the matrix
// translation minus the node's stored offset (normalized when short, with
// the Y component clamped to a unit step), rotate it via a Y-angle matrix
// built from the +0x18 u16 angle slot, and store the concatenated result
// back into the world matrix and the node.
void func_8048AB2C(CScnItemModelNw4r* self, nw4r::math::MTX34* worldMtxBase) {
    if (!(self->field_0x7A4 & 0x10000000)) {
        return;
    }
    f32 fade = func_80484EB0((CScnItemModel*)self);
    if (self->field_0x7A8 & 4) {
        fade = lbl_eu_8066A8FC;
    }
    f32 scale = fade * func_80496288(self->field_04);
    if (scale > lbl_eu_8066A8FC) {
        scale = lbl_eu_8066A8FC;
    }
    for (int i = 0; i < 2; i++) {
        CScnItemModelNw4rShadowNode* node = &self->shadowNodes[i];
        if (node->id == -1) {
            continue;
        }
        nw4r::g3d::ResNode rnode =
            nw4r::g3d::ResMdl(self->field_0x146C).GetResNode((u32)node->id);
        if (rnode.ptr() == 0) {
            nw4r::db::Panic(lbl_eu_80529678, 0x53, lbl_eu_80529658);
        }
        nw4r::math::MTX34* worldMtx =
            worldMtxBase + (rnode.ptr() != 0 ? rnode.GetMtxID() : 0);
        if (node->field_0x1E & 1) {
            node->vecB = ml::CVec3::zero;
            node->vecA.x = worldMtx->m[0][3];
            node->vecA.y = worldMtx->m[1][3];
            node->vecA.z = worldMtx->m[2][3];
            node->field_0x1E &= ~1;
            continue;
        }
        // Shadow position: translation minus the stored offset, with the
        // offset renormalized when the delta is short.
        nw4r::math::VEC3 tgt;
        tgt.x = worldMtx->m[0][3];
        tgt.y = worldMtx->m[1][3];
        tgt.z = worldMtx->m[2][3];
        nw4r::math::VEC3 delta;
        nw4r::math::VEC3Sub(&delta, &tgt, (nw4r::math::VEC3*)&node->vecA);
        f32 len2 = delta.x * delta.x + delta.y * delta.y + delta.z * delta.z;
        if (len2 <= lbl_eu_8066A908) {
            f32 inv = (lbl_eu_8066A8FC / sqrt(len2)) * lbl_eu_8066A90C;
            delta.x *= inv;
            delta.z *= inv;
            node->vecA.x = tgt.x - delta.x;
            node->vecA.z = tgt.z - delta.z;
        }
        if (ml::math::abs(delta.y) > lbl_eu_8066A8FC) {
            f32 dir = delta.y >= lbl_eu_8066A8E0 ? lbl_eu_8066A8FC
                                                 : lbl_eu_8066A910;
            node->vecA.y = tgt.y - dir;
        }
        // [inferred elided retail middle: angle = field_0x18 converted via
        // the 0x4330 double magic, scaled by 1/65536 * fade; a Y-rotation
        // matrix is built at the stack slot (PSMTXIdentity fallback), then:]
        nw4r::math::MTX34 rot;
        PSMTXIdentity(rot.m);
        nw4r::math::MTX34 out;
        PSMTXConcat(rot.m, worldMtx->m, out.m);
        *worldMtx = out;
        worldMtx->m[0][3] = tgt.x;
        worldMtx->m[1][3] = tgt.y;
        worldMtx->m[2][3] = tgt.z;
    }
}

// __dt__804871B0: destructor of the 0x10-byte buffer-holder sub-object at
// CScnItemModelNw4r+0x824 (mirror of __dt__8048268C in CScnItemModel.cpp).
// Releases the owned buffer (field_C != 0xFFFFFFFF) via
// mtl::MemManager::deallocate, resets the fields, and frees the object when
// the deletion flag is positive. Fragment-function form: the class name is
// an address fragment and MWCC emits `__dt__` names verbatim.
CScnItemModel824* __dt__804871B0(CScnItemModel824* self, int deleting) {
    if (self != nullptr) {
        self->field_4 = 0;
        if (self->field_C != 0xFFFFFFFF) {
            if (self->field_0 != nullptr) {
                mtl::MemManager::deallocate(self->field_0);
                self->field_0 = nullptr;
            }
        }
        self->field_0 = nullptr;
        self->field_8 = nullptr;
        self->field_C = -1;
        if (deleting > 0) {
            operator delete(self);
        }
    }
    return self;
}

void func_8048B30C(){}

// func_8048B3F0: scan the +0x824 node table (count at +0x828) for an entry
// whose resolved node name matches `name`. Always returns 1. The redundant
// null assert after the null-continue is retail's dead second check reusing
// the first compare's CR flags (see func_8048B68C).
int func_8048B3F0(CScnItemModelNw4r* self, const char* name) {
    CScnItemModel824Entry* entry =
        (CScnItemModel824Entry*)self->member824.field_0;
    while (entry != (CScnItemModel824Entry*)self->member824.field_0 +
                        self->member824.field_4) {
        nw4r::g3d::ResNode node = nw4r::g3d::ResMdl(self->field_0x146C)
                                      .GetResNode(entry->field_0);
        if (node.ptr() != 0) {
            if (node.ptr() == 0) {
                nw4r::db::Panic(lbl_eu_8056E194, 0x2C, lbl_eu_8056E178,
                                &lbl_eu_80663910, &lbl_eu_80663918);
            }
            nw4r::g3d::ResNodeData* data = node.ptr();
            const char* nodeName;
            if (data->name != 0) {
                nodeName = (const char*)data + data->name;
            } else {
                nodeName = 0;
            }
            if (strcmp(nodeName, name) == 0) {
                return 1;
            }
        }
        entry++;
    }
    return 1;
}

u32 func_8048B4BC(u8* self) { return *(u32*)((u8*)self + 0x828); }

// func_8048B4C4: resolve the node named by the +0x824 entry table and
// return its name string (node data + self-relative +8 offset), or 0 when
// the node or its name offset is absent. A missing node panics first.
const char* func_8048B4C4(CScnItemModelNw4r* self, u32 index) {
    nw4r::g3d::ResNode node = nw4r::g3d::ResMdl(self->field_0x146C).GetResNode(
        ((CScnItemModel824Entry*)self->member824.field_0)[index].field_0);
    if (node.ptr() == 0) {
        nw4r::db::Panic(lbl_eu_8056E194, 0x2C, lbl_eu_8056E178,
                        &lbl_eu_80663910, &lbl_eu_80663918);
    }
    nw4r::g3d::ResNodeData* data = node.ptr();
    s32 name = data->name;
    if (name == 0) {
        return 0;
    }
    return (const char*)data + name;
}

// func_8048B54C: set the visibility bit (entry flags bit 3) for the node at
// `index`. Propagates (index, value) to the chain first (func_80485B98);
// when the new value differs from the current flag (xor test), rewrite the
// flag and notify via the vtable-0xA4 virtual.
void func_8048B54C(CScnItemModelNw4r* self, u32 index, u32 value) {
    func_80485B98((CScnItemModel*)self, index, value);
    CScnItemModel824Entry* entries =
        (CScnItemModel824Entry*)self->member824.field_0;
    nw4r::g3d::ResNode node = nw4r::g3d::ResMdl(self->field_0x146C)
                                  .GetResNode(entries[index].field_0);
    if (node.ptr() != 0) {
        CScnItemModel824Entry* entry =
            &((CScnItemModel824Entry*)self->member824.field_0)[index];
        volatile u16* flagsPtr = &entry->flags;
        if (value ^ ((*flagsPtr >> 3) & 1)) {
            if (value != 0) {
                *flagsPtr = (u16)(*flagsPtr | 0x8);
            } else {
                *flagsPtr = (u16)(*flagsPtr & 0xFFF7);
            }
            ((CScnItemModel*)self)->vfuncA4(1);
        }
    }
}

// func_8048B608: if any model in the reference chain reports handled for
// the query (func_80485C28), report "busy" (1). Otherwise resolve the node
// named by the +0x824 entry table and return its hidden flag (entry flags
// bit 3); 0 when the node is absent.
u32 func_8048B608(CScnItemModelNw4r* self, u32 index) {
    if (func_80485C28(self, index) != 0) {
        return 1;
    }
    nw4r::g3d::ResNode node = nw4r::g3d::ResMdl(self->field_0x146C).GetResNode(
        ((CScnItemModel824Entry*)self->member824.field_0)[index].field_0);
    if (node.ptr() == 0) {
        return 0;
    }
    return (((CScnItemModel824Entry*)self->member824.field_0)[index].flags >> 3) &
           1;
}

// func_8048B68C: like func_8048B728, but the node is resolved by name via
// func_80490AF4 (the model resource's ResMdl handle). Null name or absent
// node -> 0; the second null check is retail's redundant assert reusing
// the first compare's CR flags.
nw4r::math::MTX34* func_8048B68C(CScnItemModelNw4r* self, const char* name) {
    if (name == 0) {
        return 0;
    }
    nw4r::g3d::ResNode node = func_80490AF4(self, name);
    if (node.ptr() == 0) {
        return 0;
    }
    if (node.ptr() == 0) {
        nw4r::db::Panic(lbl_eu_80529678, 0x53, lbl_eu_80529658);
    }
    nw4r::math::MTX34* base =
        ((CScnItemModelNw4rScnMdlView*)self->field_0x147C)->mpWorldMtxArray;
    return base + node.GetMtxID();
}

// func_8048B728: resolve the node at `index` in the model resource and
// return a pointer to its world-matrix slot in the scene object's
// per-node world-matrix array (indexed by the node's mtxID, 0x30 bytes
// each). Absent node -> 0; the second null check is retail's redundant
// assert reusing the first compare's CR flags.
nw4r::math::MTX34* func_8048B728(CScnItemModelNw4r* self, u32 index) {
    nw4r::g3d::ResNode node =
        nw4r::g3d::ResMdl(self->field_0x146C).GetResNode(index);
    if (node.ptr() == 0) {
        return 0;
    }
    if (node.ptr() == 0) {
        nw4r::db::Panic(lbl_eu_80529678, 0x53, lbl_eu_80529658);
    }
    nw4r::math::MTX34* base =
        ((CScnItemModelNw4rScnMdlView*)self->field_0x147C)->mpWorldMtxArray;
    return base + node.GetMtxID();
}

void func_8048B7C0(CScnItemModelNw4r* self) {}

// func_8048BA58: resolve a named node in the model resource and return its
// id. Absent node -> -1; then a redundant validity assert (retail keeps the
// second check, reusing the first compare's CR flags) before GetID.
int func_8048BA58(CScnItemModelNw4r* self, const char* name) {
    nw4r::g3d::ResNode node = nw4r::g3d::ResMdl(self->field_0x146C).GetResNode(name);
    if (node.ptr() == 0) {
        return -1;
    }
    if (node.ptr() == 0) {
        nw4r::db::Panic(lbl_eu_8056E1C8, 0x38, lbl_eu_8056E1A8);
    }
    return node.GetID();
}

void func_80496FC4(u8* self);
void func_8048BAD4(u8* self) { ((void(*)(void*))func_80496FC4)((char*)self + 0xc); }

// func_8048BADC: model-visible flag sync. Syncs the +0x7A4 bit-21 flag to
// `param` (func_804830BC), then - when the "action" combination (bit 21 |
// 0x7A8 bit 8 | bit 25) no longer matches the bit-26 latch - updates the
// latch (bit 26 when any action is set, else clears bit 5) and pokes the
// camera/effect helpers func_8048F7A8 / func_8048F630 guarded by the
// bit-5/bit-19 state.
void func_8048BADC(CScnItemModelNw4r* self, u32 param) {
    u32 bit21 = (self->field_0x7A4 >> 21) & 1;
    if (param == bit21) {
        return;
    }
    func_804830BC(self, param);
    volatile u32* f7a4 = (volatile u32*)&self->field_0x7A4;
    u32 flags = *f7a4;
    u32 flags8 = self->field_0x7A8;
    u32 b21 = (flags & 0x200000) >> 21;
    u32 b25 = (flags & 0x02000000) >> 25;
    u32 b8 = (flags8 & 0x100) >> 8;
    u32 b26 = (flags & 0x04000000) >> 26;
    u32 comb = (b21 | b8) | b25;
    if ((b26 ^ comb) == 0) {
        return;
    }
    if (comb != 0) {
        *f7a4 |= 0x04000000;
    } else {
        *f7a4 &= ~0x04000000;
    }
    u32 f = *f7a4;
    if (f & 0x04000000) {
        if (!(f & 0x1000)) return;
        u32 cleared = (func_8048F7A8(self->field_04->field_0x8C,
                                     (CScnCamLayout*)self) == 0);
        if (cleared) {
            *f7a4 |= 0x1000;
        } else {
            *f7a4 &= ~0x1000;
        }
    } else {
        if (f & 0x1000) return;
        if (func_8048F630(self->field_04->field_0x8C, (CScnCamLayout*)self) != 0) {
            *f7a4 |= 0x1000;
        } else {
            *f7a4 &= ~0x1000;
        }
    }
}

// func_8048BBF0: like func_8048BADC, but syncs the +0x7A8 bit-8 flag to
// `param` (func_804830E4) before the same combination/latch logic.
void func_8048BBF0(CScnItemModelNw4r* self, u32 param) {
    u32 bit8 = (self->field_0x7A8 >> 8) & 1;
    if (param == bit8) {
        return;
    }
    func_804830E4(self, param);
    volatile u32* f7a4 = (volatile u32*)&self->field_0x7A4;
    u32 flags = *f7a4;
    u32 flags8 = self->field_0x7A8;
    u32 b21 = (flags & 0x200000) >> 21;
    u32 b25 = (flags & 0x02000000) >> 25;
    u32 b8 = (flags8 & 0x100) >> 8;
    u32 b26 = (flags & 0x04000000) >> 26;
    u32 comb = (b21 | b8) | b25;
    if ((b26 ^ comb) == 0) {
        return;
    }
    if (comb != 0) {
        *f7a4 |= 0x04000000;
    } else {
        *f7a4 &= ~0x04000000;
    }
    u32 f = *f7a4;
    if (f & 0x04000000) {
        if (!(f & 0x1000)) return;
        u32 cleared = (func_8048F7A8(self->field_04->field_0x8C,
                                     (CScnCamLayout*)self) == 0);
        if (cleared) {
            *f7a4 |= 0x1000;
        } else {
            *f7a4 &= ~0x1000;
        }
    } else {
        if (f & 0x1000) return;
        if (func_8048F630(self->field_04->field_0x8C, (CScnCamLayout*)self) != 0) {
            *f7a4 |= 0x1000;
        } else {
            *f7a4 &= ~0x1000;
        }
    }
}

// Helper: resolve the node name string (ResNodeData self-relative +8 name
// offset), or null when the offset is absent. Returns a 4-byte name handle
// (mirrors the nw4r ResName wrapper) so MWCC materialises the retail's
// branchy select (cmpwi/beq/add/b/li 0) instead of if-converting to a
// preloaded default.
static inline CScnItemModelNw4rName resolveNodeName(
    nw4r::g3d::ResNodeData* data) {
    if (data->name != 0) {
        CScnItemModelNw4rName n = {(const char*)data + data->name};
        return n;
    }
    CScnItemModelNw4rName n = {0};
    return n;
}

// func_8048B1F4: set/clear the per-node "hidden" flag (entry flags bit 3)
// for the node whose name matches `name` in the +0x824 table. Notifies the
// reference chain (func_80485A48) first, then scans the table resolving each
// entry's node (the redundant null assert after the null-continue is retail's
// dead second check reusing the first compare's CR flags); on a name match,
// syncs flag bit 3 to `flag` and notifies via the vtable-0xA4 virtual.
void func_8048B1F4(CScnItemModelNw4r* self, const char* name, u32 flag) {
    func_80485A48((CScnItemModel*)self, (u32)name, flag);
    CScnItemModel824Entry* entry =
        (CScnItemModel824Entry*)self->member824.field_0;
    while (entry != (CScnItemModel824Entry*)self->member824.field_0 +
                        self->member824.field_4) {
        nw4r::g3d::ResNode node = nw4r::g3d::ResMdl(self->field_0x146C)
                                      .GetResNode(entry->field_0);
        if (node.ptr() != 0) {
            if (node.ptr() == 0) {
                nw4r::db::Panic(lbl_eu_8056E194, 0x2C, lbl_eu_8056E178,
                                &lbl_eu_80663910, &lbl_eu_80663918);
            }
            nw4r::g3d::ResNodeData* data = node.ptr();
            if (strcmp(resolveNodeName(data).p, name) == 0) {
                volatile u16* flags = &entry->flags;
                if (flag ^ ((*flags >> 3) & 1)) {
                    if (flag != 0) {
                        *flags = (u16)(*flags | 0x8);
                    } else {
                        *flags = (u16)(*flags & 0xFFF7);
                    }
                    ((CScnItemModel*)self)->vfuncA4(1);
                }
                break;
            }
        }
        entry++;
    }
}

void func_8048BD04(CScnItemModelNw4r* self, u32 enable) {
    u32 v = (self->field_0x7A4 >> 27) & 1;
    if (enable == v) return;
    func_8048310C(self, enable);
}

void* func_8048BD1C(u8* self, unsigned int size) {
    void* buffer = *(void**)((char*)self + 8);
    unsigned int current = *(unsigned int*)((char*)buffer + 0x860);
    if (size >= 0xC00 - current) return 0;
    unsigned int newOffset = current + size;
    *(unsigned int*)((char*)buffer + 0x860) = newOffset;
    return (char*)buffer + current + 0x864;
}

void func_8048BD50(void) {}

// ===========================================================================
// __ct__CScnItemModelNw4r: CScnItemModelNw4r constructor (0x8048A0DC).
// Fragment-function form (the retail symbol is a bare fragment name, like the
// base __ct__CScnItemModel). Builds the base model, installs the three vtable
// anchors (+0x0 / +0x1464 / +0x1468), constructs the seven sub-objects
// (effect-act, maru-shadow, material, mouth, UV-anim, eye-anim, dynamics),
// then builds the g3d scene tree: the MEMAllocator adapter at +0x16A4, the
// ScnMdl and its groups, the frame table carved from the +0x864 pool, the anm
// scene (policy + fog), the per-material tev rewrite, the node tables and the
// CMdlLook handle.
// ===========================================================================
extern "C" CScnItemModelNw4r* __ct__CScnItemModelNw4r(
    void* pMem, CScnItemModelNw4r* pSrc, nw4r::g3d::ResMdl* pResMdl,
    void* resFileData, u32 param, u32 flags) {
    CScnItemModelNw4r* self = (CScnItemModelNw4r*)pMem;

    __ct__CScnItemModel((CScnItemModel*)self, (CScnItemModelOwner*)pSrc,
                        (u32)resFileData);

    // Vtable anchors + model resource pointer.
    u8* vt = (u8*)lbl_eu_8056DE80;
    *(void**)self = (void*)vt;
    *(void**)((u8*)self + 0x1464) = vt + 0xDC;
    *(void**)((u8*)self + 0x1468) = vt + 0xF4;
    self->field_0x146C = pResMdl->ptr();

    // Clear the g3d scene handle slots and the frame-table fields.
    self->field_0x1470 = 0;
    self->field_0x1474 = 0;
    self->field_0x1478 = 0;
    self->field_0x147C = 0;
    self->field_0x1480 = 0;
    self->field_0x1484 = 0;
    self->field_0x1488 = 0;
    self->field_0x148C = 0;
    self->field_0x1490 = 0;
    self->field_0x1494 = 0;
    self->field_0x1498 = 0;
    self->field_0x149C = 0;
    self->field_0x149E = 0;
    self->field_0x14A0 = -1;
    self->field_0x14A8 = -1;
    self->field_0x14AC = 0;
    self->field_0x14B0 = 0;
    self->field_0x14B4.x = lbl_eu_8066A8E0;
    self->field_0x14B4.y = lbl_eu_8066A8E0;
    self->field_0x14B4.z = lbl_eu_8066A8E0;
    self->field_0x14C0 = 0;

    // Sub-objects.
    __ct__CScnEffectActNw4r(&self->field_0x14C4);
    __ct__CScnMaruShadowNw4r(&self->field_0x1650);
    self->field_0x16BC = lbl_eu_8066A8E0;
    self->field_0x16C0 = (s16)param;
    self->field_0x16C4 = 0;
    __ct__CMdlMaterial(&self->field_0x16C8);
    __ct__CMdlMouth(&self->field_0x1700);
    __ct__CMdlAnmUV(&self->field_0x1730);
    __ct__CMdlAnmEye(&self->field_0x1770);
    __ct__CMdlDynamics(&self->field_0x17A0);
    self->field_0x17C8 = 0;
    self->field_0x17CC = 0;
    self->field_0x17D0 = 0;
    self->field_0x17D8 = -1;
    if (self->field_0x16C0 == -1) {
        self->field_0x16C0 = 7;
    }

    // ResFile wrapper (alignment-panics on the raw data pointer).
    nw4r::g3d::ResFile resFile(resFileData);
    if ((u32)resFile.ptr() & 0x1F) {
        nw4r::db::Panic(lbl_eu_8052637C, 0x3C, lbl_eu_80526354);
    }

    // MEMAllocator adapter at +0x16A4: an 8-byte funcs table (alloc/free)
    // followed by the nw4r MEMAllocator whose funcs point back at the table
    // and whose heapParam1 carries the model (func_8048BD1C reads
    // allocator+8 == this, then this->field_0x860).
    self->field_0x16A4 = (void*)&func_8048BD1C;
    self->field_0x16A8 = (void*)&func_8048BD50;
    self->allocator.funcs = (const MEMAllocatorFuncs*)&self->field_0x16A4;
    self->allocator.heap = 0;
    self->allocator.heapParam1 = (u32)self;
    self->allocator.heapParam2 = 0;

    // ScnMdl buffer-option bits from the anm resources present in the file.
    u32 bufferOption = 0;
    if (resFile.HasResAnmTexPat()) bufferOption |= 0x3;
    if (resFile.HasResAnmTexSrt()) bufferOption |= 0x204;
    if (resFile.HasResAnmClr()) bufferOption |= 0x108;
    if (resFile.HasResAnmShp()) bufferOption |= 0x7000;
    bufferOption |= 0x9DC;

    // Scene model: the first model built in the session uses the
    // func_8048ED04 allocator path, later ones func_8048ECE4 (the .sbss byte
    // is a one-shot switch cleared right after the read).
    u32 scnMdlSize;
    if (lbl_eu_806658D8 != 0) {
        nw4r::g3d::ResMdl resMdl(self->field_0x146C);
        self->field_0x147C = nw4r::g3d::ScnMdl::Construct(
            (MEMAllocator*)func_8048ED04((CScn*)pSrc), &scnMdlSize, resMdl,
            bufferOption, 2);
    } else {
        nw4r::g3d::ResMdl resMdl(self->field_0x146C);
        self->field_0x147C = nw4r::g3d::ScnMdl::Construct(
            (MEMAllocator*)func_8048ECE4((CScn*)pSrc), &scnMdlSize, resMdl,
            bufferOption, 2);
    }
    lbl_eu_806658D8 = 0;

    // Optional group tree (bit-0) and the camera hookup (func_8048F630).
    if (flags & 1) {
        if (!(flags & 0x10)) {
            self->field_0x1470 = nw4r::g3d::ScnGroup::Construct(
                &self->allocator, 0, 3);
            self->field_0x1480 =
                Construct__Q34nw4r3g3d12ScnMdlExpandFP12MEMAllocatorPUlUlPQ34nw4r3g3d12ScnMdlSimple(
                    &self->allocator, 0, 4,
                    (nw4r::g3d::ScnMdlSimple*)self->field_0x147C);
            self->field_0x1478 = nw4r::g3d::ScnGroup::Construct(
                &self->allocator, 0, 1);
            self->field_0x1470->Insert(self->field_0x1470->Size(),
                                       self->field_0x1478);
            self->field_0x1470->Insert(self->field_0x1470->Size(),
                                       self->field_0x1480);
            self->field_0x1474 = nw4r::g3d::ScnGroup::Construct(
                &self->allocator, 0, 4);
            self->field_0x1470->Insert(self->field_0x1470->Size(),
                                       self->field_0x1474);
        }
        if (func_8048F630(
                (CScnRootNw4r*)(uintptr_t)func_8048ECD0((CScn*)pSrc),
                (CScnCamLayout*)self) != 0) {
            self->field_0x7A4 |= 0x1000;
        } else {
            self->field_0x7A4 &= ~0x1000;
        }
    }

    // Scene-object callbacks: the +0x1468 anchor (or this when null) is the
    // callback object; timing/exec-op masks are raised.
    void* cbExec = (u8*)self + ((self != 0) ? 0x1468 : 0);
    ((CScnItemModelNw4rScnObjD4*)self->field_0x147C)->field_0xD4 = cbExec;
    ((nw4r::g3d::ScnObj*)self->field_0x147C)->EnableScnObjCallbackTiming(
        nw4r::g3d::ScnObj::CALLBACK_TIMING_A);
    ((nw4r::g3d::ScnObj*)self->field_0x147C)->EnableScnObjCallbackTiming(
        (nw4r::g3d::ScnObj::Timing)4);
    ((nw4r::g3d::ScnObj*)self->field_0x147C)->EnableScnObjCallbackExecOp(
        nw4r::g3d::ScnObj::EXECOP_DRAW_OPA);
    ((nw4r::g3d::ScnObj*)self->field_0x147C)->EnableScnObjCallbackExecOp(
        nw4r::g3d::ScnObj::EXECOP_DRAW_XLU);
    ((nw4r::g3d::ScnObj*)self->field_0x147C)->EnableScnObjCallbackExecOp(
        nw4r::g3d::ScnObj::EXECOP_CALC_WORLD);

    // Frame table carved from the +0x864 pool (0x20 bytes), then the model
    // callback anchor (+0x1464 or this) and its timing/options.
    self->field_0x17CC = (u32*)((u8*)self + self->field_0x860 + 0x864);
    self->field_0x17D0 = 0;
    self->field_0x17D4 = 8;
    self->field_0x860 += 0x20;
    void* mdlCb = (u8*)self + ((self != 0) ? 0x1464 : 0);
    ((CScnItemModelNw4rScnMdl11C*)self->field_0x147C)->field_0x11C = mdlCb;
    ((nw4r::g3d::ScnMdlSimple*)self->field_0x147C)->EnableScnMdlCallbackTiming(
        (nw4r::g3d::ScnObj::Timing)1);
    ((nw4r::g3d::ScnMdlSimple*)self->field_0x147C)->EnableScnMdlCallbackTiming(
        (nw4r::g3d::ScnObj::Timing)2);
    ((nw4r::g3d::ScnMdlSimple*)self->field_0x147C)->EnableScnMdlCallbackTiming(
        (nw4r::g3d::ScnObj::Timing)4);
    ((nw4r::g3d::ScnMdl*)self->field_0x147C)->SetScnObjOption(
        nw4r::g3d::ScnMdl::OPTID_VISBUFFER_REFRESH_NEEDED, 0);
    ((nw4r::g3d::ScnMdl*)self->field_0x147C)->SetScnObjOption(
        nw4r::g3d::ScnObj::OPTID_ENABLE_CULLING, 0);

    // Sub-object init hooks gated on the flag bits; both shadow-node ids
    // start free (-1).
    if (flags & 0x2) {
        func_804E95E0(&self->field_0x17A0, self);
    }
    self->shadowNodes[0].id = -1;
    self->shadowNodes[1].id = -1;
    if (flags & 0x20) {
        func_804E54B8(&self->field_0x16C8, self);
    }
    if (flags & 0x40) {
        func_80496F14(&self->field_0xC, (CScn*)pSrc, self, 2);
    }

    // Anm-scene: bind the first scene animation, construct the AnmScnRes,
    // force the loop play policy (address-range checked), attach it to the
    // scene root and push its fog into the fog manager.
    if (resFile.HasResAnmScn()) {
        nw4r::g3d::ResAnmScn resAnmScn = resFile.GetResAnmScn(0);
        nw4r::g3d::ResAnmScn bound = resAnmScn;
        bound.Bind(resAnmScn);
        u32 anmSize;
        nw4r::g3d::AnmScnRes* anmScnRes = nw4r::g3d::AnmScnRes::Construct(
            (MEMAllocator*)func_8048ECE4((CScn*)pSrc), &anmSize, bound, 0);
        self->field_0x1484 = anmScnRes;
        u32 policyAddr = (u32)(void*)&nw4r::g3d::PlayPolicy_Loop;
        if (!((policyAddr & 0xFF000000) == 0x80000000 ||
              (policyAddr & 0xFF800000) == 0x81000000 ||
              (policyAddr & 0xF8000000) == 0x90000000 ||
              (policyAddr & 0xFF000000) == 0x40000000 ||
              (policyAddr & 0xFF800000) == 0x41000000 ||
              (policyAddr & 0xF8000000) == 0x50000000 ||
              (policyAddr & 0xFFFFC000) == 0x60000000)) {
            nw4r::db::Panic(lbl_eu_8056E03C, 0x5F, lbl_eu_8056E008,
                            &nw4r::g3d::PlayPolicy_Loop);
        }
        ((nw4r::g3d::FrameCtrl*)((u8*)anmScnRes + 0xC))
            ->SetPlayPolicy(nw4r::g3d::PlayPolicy_Loop);
        nw4r::g3d::ScnRoot* root =
            (nw4r::g3d::ScnRoot*)(uintptr_t)func_8048ECD8(pSrc);
        if (((CScnItemModelNw4rRoot2888*)root)->field_0x2888 != 0) {
            RemoveAnmScn__Q34nw4r3g3d7ScnRootFv(root);
        }
        SetAnmScn__Q34nw4r3g3d7ScnRootFPQ34nw4r3g3d6AnmScn(
            root, (nw4r::g3d::AnmScn*)self->field_0x1484);
        if (anmScnRes->GetNumFog() != 0) {
            nw4r::g3d::FogData fogData;
            nw4r::g3d::Fog fog(&fogData);
            anmScnRes->GetFog(fog, 0);
            GXFogType fogType;
            f32 startz, endz, nearz, farz;
            GXColor fogColor;
            fog.GetFog(&fogType, &startz, &endz, &nearz, &farz, &fogColor);
            if (fogType != 0) {
                // RGBA bytes / 255 via the 0x4330 double magic, stored in
                // reverse member order (b, g, r, a).
                f32 vec[4];
                vec[3] = (f32)((f64)fogColor.b) / lbl_eu_8066A8E4;
                vec[2] = (f32)((f64)fogColor.g) / lbl_eu_8066A8E4;
                vec[1] = (f32)((f64)fogColor.r) / lbl_eu_8066A8E4;
                vec[0] = (f32)((f64)fogColor.a) / lbl_eu_8066A8E4;
                func_8049DE74(self->field_04->field_0x78, (u32)fogType, vec,
                              startz, endz, nearz, farz);
            }
        }
    }

    // Per-animation-type construction: material colour, tex-srt, tex-pattern.
    // Each binds into the ScnMdl (SetAnmObj with the not-specified type).
    if (resFile.HasResAnmClr()) {
        nw4r::g3d::ResMdl resMdl(pResMdl->ptr());
        nw4r::g3d::ResAnmClr resClr = resFile.GetResAnmClr(0);
        u32 clrSize;
        nw4r::g3d::AnmObjMatClrRes* obj = nw4r::g3d::AnmObjMatClrRes::Construct(
            (MEMAllocator*)func_8048ECF4((CScn*)pSrc), &clrSize, resClr,
            resMdl, 0);
        self->field_0x1490 = obj;
        nw4r::g3d::ResMdl bindMdl(pResMdl->ptr());
        obj->Bind(bindMdl);
        ((nw4r::g3d::ScnMdl*)self->field_0x147C)->SetAnmObj(
            (nw4r::g3d::AnmObj*)obj,
            (nw4r::g3d::ScnMdlSimple::AnmObjType)6);
    }
    if (resFile.HasResAnmTexSrt()) {
        nw4r::g3d::ResMdl resMdl(pResMdl->ptr());
        nw4r::g3d::ResAnmTexSrt resSrt = resFile.GetResAnmTexSrt(0);
        u32 srtSize;
        nw4r::g3d::AnmObjTexSrtRes* obj = nw4r::g3d::AnmObjTexSrtRes::Construct(
            (MEMAllocator*)func_8048ECF4((CScn*)pSrc), &srtSize, resSrt,
            resMdl, 0);
        self->field_0x1488 = obj;
        if (obj != 0) {
            nw4r::g3d::ResMdl bindMdl(pResMdl->ptr());
            obj->Bind(bindMdl);
            ((nw4r::g3d::ScnMdl*)self->field_0x147C)->SetAnmObj(
                (nw4r::g3d::AnmObj*)obj,
                (nw4r::g3d::ScnMdlSimple::AnmObjType)6);
        }
    }
    if (resFile.HasResAnmTexPat()) {
        nw4r::g3d::ResMdl resMdl(pResMdl->ptr());
        nw4r::g3d::ResAnmTexPat resPat = resFile.GetResAnmTexPat(0);
        u32 patSize;
        nw4r::g3d::AnmObjTexPatRes* obj = nw4r::g3d::AnmObjTexPatRes::Construct(
            (MEMAllocator*)func_8048ECE4((CScn*)pSrc), &patSize, resPat,
            resMdl, 0);
        self->field_0x148C = obj;
        if (obj != 0) {
            nw4r::g3d::ResMdl bindMdl(pResMdl->ptr());
            obj->Bind(bindMdl);
            ((nw4r::g3d::ScnMdl*)self->field_0x147C)->SetAnmObj(
                (nw4r::g3d::AnmObj*)obj,
                (nw4r::g3d::ScnMdlSimple::AnmObjType)6);
        }
    }

    // Node table: count the non-degenerate nodes under the root node, then
    // store their ids as (id, 0xD) pairs. Small tables are carved from the
    // +0x864 pool; larger ones come from the owner's heap.
    {
        nw4r::g3d::ResMdl resMdl(self->field_0x146C);
        nw4r::g3d::ResNode node = resMdl.GetResNode(0);
        int nodeCount = func_80491158(&node);
        if (nodeCount != 0) {
            u32 bytes = (u32)nodeCount * 4;
            if (bytes < 0xC00u - self->field_0x860) {
                self->member824.field_4 = 0;
                self->member824.field_0 =
                    (u8*)self + self->field_0x860 + 0x864;
                self->member824.field_C = (u32)nodeCount;
                self->field_0x860 += bytes;
            } else {
                u32 handle =
                    func_80496018((CScnItemModelNw4rOwner*)pSrc);
                self->member824.field_8 = (u8*)(uintptr_t)handle;
                self->member824.field_0 = (u8*)mtl::MemManager::allocate_head(
                    handle, bytes, 4);
                self->member824.field_4 = 0;
                self->member824.field_C = (u32)nodeCount;
            }
            // Collect the node ids into a temp list, mirroring each entry to
            // a local pair while it is written.
            CScnItemModelNw4rNodeIdList list;
            list.array = 0;
            list.count = 0;
            u32 handle =
                func_80496018((CScnItemModelNw4rOwner*)pSrc);
            list.array = (u32*)mtl::MemManager::allocate_head(handle, bytes, 4);
            list.count = 0;
            u32 nodeId = (u32)nodeCount;
            {
                nw4r::g3d::ResMdl resMdl2(self->field_0x146C);
                nw4r::g3d::ResNode node2 = resMdl2.GetResNode(0);
                func_80491764(&node2, &list);
            }
            // Mirror pair kept alive (retail stores it to the +0x38 stack
            // slots every iteration; volatile prevents the DCE that would
            // otherwise shrink the loop body below the unroller's threshold).
            volatile u16 curId = 0;
            volatile u16 curFlags = 0;
            for (u32 i = 0; i < (u32)nodeCount; i++) {
                u32 entryId = list.array[i];
                curId = (u16)entryId;
                curFlags = 0xD;
                u32 idx = self->member824.field_4;
                self->member824.field_4 = idx + 1;
                CScnItemModel824Entry* e =
                    &((CScnItemModel824Entry*)self->member824.field_0)[idx];
                e->field_0 = (u16)entryId;
                e->flags = 0xD;
            }
            list.count = 0;
            if (handle != 0xFFFFFFFF && list.array != 0) {
                mtl::MemManager::deallocate(list.array);
                list.array = 0;
            }
            list.array = 0;
            nodeId = 0;
            handle = 0xFFFFFFFF;
        }
    }

    // Node-name lookup: resolve the frame-table node, register its id and
    // stash its world-matrix slot for the material sub-object.
    {
        nw4r::g3d::ResMdl resMdl(self->field_0x146C);
        nw4r::g3d::ResNode node = resMdl.GetResNode(lbl_eu_806638D4);
        if (node.ptr() != 0) {
            if (node.ptr() == 0) {
                nw4r::db::Panic(lbl_eu_8056E1C8, 0x38, lbl_eu_8056E1A8);
            }
            self->field_0x14A0 = node.GetID();
            func_80488F44(self, self->field_0x14A0);
            if (node.ptr() == 0) {
                nw4r::db::Panic(lbl_eu_80529678, 0x53, lbl_eu_80529658);
            }
            nw4r::math::MTX34* worldMtx =
                ((CScnItemModelNw4rScnMdlView*)self->field_0x147C)
                    ->mpWorldMtxArray +
                node.GetMtxID();
            self->field_0x14AC = (CScnItemModelNw4r14AC*)worldMtx;
            nw4r::g3d::ResNode nodeCopy = node;
            func_804E5990(&self->field_0x16C8, &nodeCopy, worldMtx);
        }
    }

    func_804E65CC(&self->field_0x1700, self);

    // CMdlLook: carve 0x74 bytes from the +0x864 pool when it fits, else
    // allocate from the scene root's MemManager handle. The placement-new
    // carries retail's exception frame (catch-all __throw).
    if (flags & 0x4) {
        void* buf = 0;
        if (0xC00u - self->field_0x860 > 0x74) {
            buf = (u8*)self + self->field_0x860 + 0x864;
            self->field_0x860 += 0x74;
        }
        if (buf != 0) {
            try {
                __ct__CMdlLook(buf);
            } catch (...) {
                __throw(0, 0, 0);
            }
            self->field_0x17C8 = (s32)(uintptr_t)buf;
            self->field_0x85C |= 0x2;
        } else {
            u32 handle = ((CScnItemModelNw4rRootHandle*)(uintptr_t)
                              func_8048ECD0((CScn*)pSrc))
                              ->v09();
            void* p = mtl::MemManager::allocate(0x74, handle);
            if (p != 0) {
                __ct__CMdlLook(p);
            }
            self->field_0x17C8 = (s32)(uintptr_t)p;
        }
        func_804E7B38((void*)(uintptr_t)self->field_0x17C8, self);
    }

    func_804E6C80(&self->field_0x1730, self);
    func_804E75B8(&self->field_0x1770, self);
    func_8048A17C(self);

    // Per-material tev rewrite: materials with a subtract/logic blend get the
    // k-color path, the others the plain path; both force stage 0 config.
    {
        nw4r::g3d::ResMdl resMdl(pResMdl->ptr());
        u32 numMats = resMdl.GetResMatNumEntries();
        for (u32 i = 0; i < numMats; i++) {
            nw4r::g3d::ScnMdl::CopiedMatAccess access(
                (nw4r::g3d::ScnMdl*)self->field_0x147C, i);
            nw4r::g3d::ResMat mat = resMdl.GetResMat(i);
            nw4r::g3d::ResTev tev = resMdl.GetResTev(i);
            bool kColorPath = true;
            if (mat.ptr() == 0) {
                nw4r::db::Panic(lbl_eu_8056E068, 0x26D, lbl_eu_8056E04C,
                                &lbl_eu_806638E8, &lbl_eu_806638F0);
            }
            nw4r::g3d::ResMatPix pix = mat.GetResMatPix();
            if (((u32)pix.ptr() & 0x1F) != 0) {
                nw4r::db::Panic(lbl_eu_8056E0A0, 0x154, lbl_eu_8056E078);
            }
            _GXBlendMode blendMode;
            _GXBlendFactor srcFac, dstFac;
            _GXLogicOp logicOp;
            pix.GXGetBlendMode(&blendMode, &srcFac, &dstFac, &logicOp);
            if (blendMode == (_GXBlendMode)2 || blendMode == (_GXBlendMode)3) {
                kColorPath = false;
            }
            nw4r::g3d::ResTev tevEx =
                GetResTevEx__Q44nw4r3g3d6ScnMdl15CopiedMatAccessFv(&access);
            if (tev.ptr() == 0) {
                nw4r::db::Panic(lbl_eu_8056E0D0, 0x25, lbl_eu_8056E0B0,
                                &lbl_eu_806638F4, &lbl_eu_806638FC);
            }
            nw4r::g3d::ResTev tevExCopy = tevEx;
            u8 numStages = ((const u8*)tev.ptr())[0xC];
            tevExCopy.SetNumTevStages(numStages + 1);
            if (kColorPath) {
                tevExCopy.GXSetTevColorOp((GXTevStageID)0, (GXTevOp)0,
                                          (GXTevBias)0, (GXTevScale)0, 1,
                                          (GXTevRegID)0);
                tevExCopy.GXSetTevColorIn((GXTevStageID)0,
                                          (GXTevColorArg)0xF,
                                          (GXTevColorArg)0xF,
                                          (GXTevColorArg)0xF,
                                          (GXTevColorArg)0);
                tevExCopy.GXSetTevAlphaOp((GXTevStageID)0, (GXTevOp)0,
                                          (GXTevBias)0, (GXTevScale)0, 1,
                                          (GXTevRegID)0);
                tevExCopy.GXSetTevAlphaIn((GXTevStageID)0,
                                          (GXTevAlphaArg)7,
                                          (GXTevAlphaArg)0,
                                          (GXTevAlphaArg)1,
                                          (GXTevAlphaArg)7);
            } else {
                tevExCopy.GXSetTevColorOp((GXTevStageID)0, (GXTevOp)0,
                                          (GXTevBias)0, (GXTevScale)0, 1,
                                          (GXTevRegID)0);
                tevExCopy.GXSetTevColorIn((GXTevStageID)0,
                                          (GXTevColorArg)0xF,
                                          (GXTevColorArg)0xE,
                                          (GXTevColorArg)0,
                                          (GXTevColorArg)0xF);
                tevExCopy.GXSetTevAlphaOp((GXTevStageID)0, (GXTevOp)0,
                                          (GXTevBias)0, (GXTevScale)0, 1,
                                          (GXTevRegID)0);
                tevExCopy.GXSetTevAlphaIn((GXTevStageID)0,
                                          (GXTevAlphaArg)7,
                                          (GXTevAlphaArg)6,
                                          (GXTevAlphaArg)0,
                                          (GXTevAlphaArg)7);
                tevExCopy.GXSetTevKColorSel((GXTevStageID)0,
                                            (GXTevKColorSel)0x1F);
                tevExCopy.GXSetTevKAlphaSel((GXTevStageID)0,
                                            (GXTevKAlphaSel)0x1F);
            }
            tevExCopy.DCStore(false);
        }
    }

    // Effect-act manager registration + per-frame flags.
    self->field_0x14C0 = (u32)&self->field_0x14C4;
    func_8049B9EC(&self->field_0x14C4, self);
    func_804838DC((CScnItemModel*)self, 1);
    func_8048B7C0(self);
    if (self->field_0x7AC >= lbl_eu_8066A8E8) {
        self->field_0x7A4 |= 0x40000000;
    }
    u32 shadowOut;
    func_8048E69C(&self->field_0x1650, &shadowOut, self);
    if (shadowOut != 0) {
        if (shadowOut & 0x4) {
            func_804C0228((CScnEnvLgtData*)&self->field_0x31C, 1);
        } else if (shadowOut & 0x8) {
            func_804C0254((CScnEnvLgtData*)&self->field_0x31C, 1);
        }
        if (shadowOut & 0x10) {
            self->field_0x7A8 |= 0x10;
        } else if (shadowOut & 0x20) {
            self->field_0x7A8 |= 0x20;
        } else if (shadowOut & 0x40) {
            self->field_0x7A8 |= 0x40;
        }
    }
    return self;
}


void func_8048BD54(u8* self) { ((void(*)(void*))func_80489200)((char*)self - 0x1464); }

void func_8048BD5C(u8* self) { ((void(*)(void*))func_8048917C)((char*)self - 0x1464); }

void func_8048BD64(u8* self){ ((void(*)(void*))func_80489014)((char*)self - 0x1464); }

void func_8048BD6C(u8* self) {
    // this-adjusting thunk: the CScnItemModelNw4r subobject sits at +0x1464;
    // direct tail call to its destructor (retail sets no deleting flag).
    ((void(*)(void*))__dt__17CScnItemModelNw4rFv)((char*)self - 0x1464);
}

extern "C" void func_8048BD74(u8* self) { ((void(*)(void*))func_804899F4)((char*)self - 0x1468); }

extern "C" void func_8048BD7C(u8* self) { ((void(*)(void*))func_80489924)((char*)self - 0x1468); }

extern "C" void func_8048BD84(u8* self) { ((void(*)(void*))func_80489584)((char*)self - 0x1468); }

extern "C" void func_8048BD8C(u8* self) { ((void(*)(void*))__dt__17CScnItemModelNw4rFv)((char*)self - 0x1468); }

// func_80487374: adopt `other`'s model resource as this model's parent
// (GetParent on other's ResMdl, then Bind it into this model's ResMdl).
// Bind failure sets the +0x7A4 0x800000 flag; success clears it.
void func_80487374(CScnItemModelNw4r* self, CScnItemModelNw4r* other) {
    nw4r::g3d::ResFile parent = nw4r::g3d::ResMdl(other->field_0x146C).GetParent();
    bool failed = nw4r::g3d::ResMdl(self->field_0x146C).Bind(parent) == 0;
    if (failed) {
        self->field_0x7A4 |= 0x800000;
    } else {
        self->field_0x7A4 &= ~0x800000;
    }
}
// func_804873EC: set/clear the shadow-node entry matching the resource node
// named by `name` (node id from ResNodeData.id). Setting fills the first
// free (id == -1) entry with the id, two zeroed vectors and the 0xA/0x5A
// pair; clearing resets the matching entry's id. The +0x7A4 0x10000000 flag
// reflects whether any entry is active (the set path re-checks `enable`,
// matching retail's kept redundant test).
void func_804873EC(CScnItemModelNw4r* self, const char* name, u32 enable) {
    nw4r::g3d::ResNode node =
        nw4r::g3d::ResMdl(self->field_0x146C).GetResNode(name);
    if (node.ptr() == 0) {
        return;
    }
    if (node.ptr() == 0) {
        nw4r::db::Panic(lbl_eu_8056E1C8, 0x38, lbl_eu_8056E1A8);
    }
    u32 nodeId = node.GetID();
    if (enable != 0) {
        for (u32 i = 0; i < 2; i++) {
            if (self->shadowNodes[i].id == -1) {
                self->shadowNodes[i].id = (s16)nodeId;
                self->shadowNodes[i].field_0x1E = 0;
                self->shadowNodes[i].vecA = ml::CVec3::zero;
                self->shadowNodes[i].vecB = ml::CVec3::zero;
                self->shadowNodes[i].field_0x18 = 0xA;
                self->shadowNodes[i].field_0x1A = 0x5A;
                break;
            }
        }
        if (enable != 0) {
            self->field_0x7A4 |= 0x10000000;
        } else {
            self->field_0x7A4 &= ~0x10000000;
        }
    } else {
        for (u32 i = 0; i < 2; i++) {
            if (self->shadowNodes[i].id == (s16)nodeId) {
                self->shadowNodes[i].id = -1;
                break;
            }
        }
        if (self->shadowNodes[0].id != -1 || self->shadowNodes[1].id != -1) {
            self->field_0x7A4 |= 0x10000000;
        } else {
            self->field_0x7A4 &= ~0x10000000;
        }
    }
}
extern "C" void func_804875B8() {}
// func_804876EC: release this model's nw4r resource, re-fetch the parent
// file, and re-bind it. Bind failure sets the +0x7A4 0x800000 flag.
void func_804876EC(CScnItemModelNw4r* self) {
    nw4r::g3d::ResMdl mdl(self->field_0x146C);
    mdl.Release();
    nw4r::g3d::ResFile parent = nw4r::g3d::ResMdl(self->field_0x146C).GetParent();
    nw4r::g3d::ResFile file(parent);
    if (file.Bind(parent) == 0) {
        self->field_0x7A4 |= 0x800000;
    } else {
        self->field_0x7A4 &= ~0x800000;
    }
}
// func_80487B18: return the model resource's name string (ResMdlData plus
// its self-relative +0x48 name offset), or 0 when the resource data or its
// name offset is null.
const char* func_80487B18(CScnItemModelNw4r* self) {
    nw4r::g3d::ResMdlData* data = self->field_0x146C;
    if (data == 0) {
        nw4r::db::Panic(lbl_eu_8056E130, 0x78, lbl_eu_8056E110,
                        &lbl_eu_80663900, &lbl_eu_8066390C);
    }
    s32 name = data->name;
    if (name != 0) {
        return (const char*)data + name;
    }
    return 0;
}
