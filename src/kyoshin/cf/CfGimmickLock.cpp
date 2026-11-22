// Auto-scaffolded catalog TU for kyoshin/cf/CfGimmickLock
// Replace stubs with high-level C/C++ during decomp.

#include "kyoshin/harness_catalog.hpp"
#include "monolib/coli/code_804B2FF0.hpp"
#include "monolib/scn/CScn.hpp"
#include "kyoshin/cf/CfGimmickLock.hpp"
#include <nw4r/math.h>
#include "kyoshin/cf/CfGameManagerData.hpp"  // H3 label-owner decl (gmScene; gmGameFlags)
#include "kyoshin/cf/CfGameManager.hpp"  // real static manager accessors
#include "kyoshin/cf/object/CfObjectMove.hpp"  // real battle-actor object type

namespace cf {}
using namespace cf;

__attribute__((used)) const void* sGimmickLockIdColumns[2] = { "popID1", nullptr };

// Constructor: __ct__cf_CfGimmickLock (0x224 bytes).  The retail symbol is a
// C-linkage name (no C++ mangling markers), so it is emitted as an extern "C"
// free function like the sibling gimmick ctors.
extern "C" void* __ct__cf_CfGimmickLock(cf::CfGimmickLock* self, u16 row) {
    __ct__cf_CfGimmick(self);
    *(void**)self = (void*)lbl_eu_80535900;
    ColiNodeInit(&self->subA);
    ColiNodeInit(&self->subB);
    self->typeId = 2;

    void* bdat = Bdat_GetTable_AA34();
    void* holder = gLockBdatTable;
    self->rowId = row;

    CfGimmick_LoadBdatAreaPos(self, &self->position, bdat, &holder);
    CfGimmick_LoadBdatAreaExtents(self, &self->scale, bdat, &holder);
    CfGimmick_LoadBdatAreaRotation(self, &self->rotation, bdat, &holder);

    // Bdat columns: min/max unlock bounds and the resource id (the cells are
    // string pointers; only the low 16 bits are used).
    // Byte offsets into the pointer table (cast through char* -- the label is
    // typed void*[], so bare +0x34 would scale by 4 -> lwz 0xD0).
    u8* colTab = (u8*)gGimmickBdatColumns;
    u32 vMin = getBdatStringColumnValue(holder, *(const char**)(colTab + 0x34), row);
    self->minVal = *(u16*)&vMin;
    u32 vMax = getBdatStringColumnValue(holder, *(const char**)(colTab + 0x38), row);
    self->maxVal = *(u16*)&vMax;
    u32 vRes = getBdatStringColumnValue(holder, *(const char**)(colTab + 0x30), row);
    self->resourceId = *(u16*)&vRes;

    // Four lock-id columns; the shared column-name buffer is patched in place
    // with the per-slot character (index + 0x31).
    for (int i = 0; i < 4; ++i) {
        u8* nameBuf = (u8*)sGimmickLockIdColumns[0];
        nameBuf[5] = (u8)(i + 0x31);
        u32 v = getBdatStringColumnValue(holder, (const char*)nameBuf, row);
        // Base+displacement store (offset last) so MWCC keeps the retail
        // scaled-offset induction instead of a strength-reduced pointer walk.
        *(u16*)((u8*)self + (u32)i * 2 + 0x84) = (u8)(*(u16*)&v);
    }

    u32 vCfg = getBdatStringColumnValue(holder, "rockTYPE", row);
    self->configFlags = *(u8*)&vCfg;

    // Derived vtable init hook (slot 0x20).
    self->vinit();

    // Unbounded locks rebuild their geometry immediately.
    if (self->stateIndex != 0 && self->maxVal == 0 && self->minVal == 0) {
        CfGimmickLock_Refresh(self);
    }

    // Any populated lock id marks a key lock.
    if (self->lockId[0] != 0 || self->lockId[1] != 0 || self->lockId[2] != 0 || self->lockId[3] != 0) {
        self->configFlags |= 0x8000;
        self->flags |= 0x10;
    }

    self->flags |= 0x8;
    self->field_1F8 = 0x3c;
    self->field_1F4 = 2.0f;
    return self;
}

// Complete-object destructor: MWCC auto-generates the this!=0 guard and the
// delete-flag wrapper around this body.  Order matches retail: restore the
// vtable, detach the linked game-manager object, release both region
// sub-objects from the global registry (flag-gated), then destroy the
// members and the CfGimmick base.
cf::CfGimmickLock::~CfGimmickLock() {
    *(void**)this = (void*)lbl_eu_80535900;
    CfGimmick_ClearManagerBinding(this);

    // Inner region (0x8C) was registered with the global manager.
    if (this->flags & 1) {
        if (sEneColiProbe != 0) {
            Coli_ListRemoveNode(sEneColiProbe, &this->subA);
            this->flags &= ~0x801;
        }
    }

    // Outer region (0x140) was registered; reset its radius timer.
    if (this->flags & 0x1000) {
        if (sEneColiProbe != 0) {
            Coli_ListRemoveNode(sEneColiProbe, &this->subB);
        }
        this->flags &= ~0x1000;
        this->field_1F4 = 2.0f;
    }

    __dt__804B095C(&this->subB, -1);
    __dt__804B095C(&this->subA, -1);
    __dt__Q22cf9CfGimmickFv(this, 0);
}

// Per-frame lock update: eligibility gate, key-lock bookkeeping, geometry
// refresh, and the timed player probe that opens the lock.
extern "C" void CfGimmickLock_Update(cf::CfGimmickLock* self) {
    int flag = 1;
    if ((gGimmickPartyFlags & 0x4) != 0 && (gmGameFlags & 0x10000000) == 0) {
        flag = 0;
    } else if (self->maxVal != 0) {
        u32 count = cf::CfGameManager::getQueuedFileEventCount();
        if (self->minVal > count || count > self->maxVal) {
            flag = 0;
        } else {
            flag = 1;
        }
    }

    if (flag != 0) {
        // Key-lock presence bookkeeping: clear/set the per-lock bits
        // according to which key ids are still held by the party.
        if ((self->flags & 0x10) != 0) {
            if ((self->configFlags & 0x10) != 0) {
                if ((self->flags & 0x200) != 0) {
                    if (self->lockId[1] != 0) {
                        flag = 1;
                        if ((self->flags & 0x400) != 0) {
                            if (CfGimmick_CheckFightListPlayerAlive() == 0) {
                                self->flags &= ~0x600;
                                flag = 0;
                            }
                        } else if (CfGimmick_CheckFightListPlayerAlive() == 0) {
                            self->flags |= 0x400;
                        }
                    }
                } else if (self->lockId[0] != 0 && CfGimmick_CheckFightListPlayerAlive() == 0) {
                    self->flags = (self->flags & ~0x400) | 0x200;
                    flag = 1;
                }
            } else {
                flag = 0;
                for (int i = 0; i < 4; ++i) {
                    if (self->lockId[i] != 0 && CfGimmick_CheckFightListPlayerAlive() == 0) {
                        flag = 1;
                        break;
                    }
                }
            }
        }

        if (flag == 0) {
            goto cleanup;
        }

        CfGimmickLock_Refresh(self);
        if ((self->configFlags & 1) != 0) {
            int hit = jumptable_eu_80535830[self->stateIndex](
                (cf::CfGimmick*)&self->scale, &gGimmickAnchorPoint, &self->position);
            if (hit != 0) {
                CfGimmickLock_Activate(self);
                if ((self->flags & 0xF00) != 0) {
                    CfGimmickLock_MarkSlots(self, 0);
                    self->flags &= ~0x1E0;
                }
            } else {
                if ((self->flags & 2) != 0) {
                    self->flags &= ~0x803;
                    CfGimmick_ClearManagerBinding(self);
                    if ((self->flags & 0x1000) != 0) {
                        if (sEneColiProbe != 0) {
                            Coli_ListRemoveNode(sEneColiProbe, &self->subB);
                        }
                        self->flags &= ~0x1000;
                        self->field_1F4 = 2.0f;
                    }
                }
                if ((self->flags & 0x10) != 0 &&
                    ((self->flags & 0x20) == 0 || (self->flags & 0x40) == 0 ||
                     (self->flags & 0x80) == 0 || (self->flags & 0x100) == 0)) {
                    CfGimmickLock_MarkSlots(self, 1);
                }
            }
        }

        // Timed proximity probe: when the player stands inside the region long
        // enough, consume the key resource and open the lock.
        if ((self->configFlags & 0x4) == 0 && self->resourceId != 0) {
            cf::CfGimmickLockPlayer* player =
                (cf::CfGimmickLockPlayer*)getPlayer__Q22cf13CfGameManagerFi(0);
            if (player != 0) {
                if ((self->flags & 0x4) != 0) {
                    if (CfGimmick_IsMessageSystemBusy() == 0) {
                        self->flags &= ~0x4;
                    }
                } else if (self->field_1F8 <= 0) {
                    self->field_1F8 = 0;
                    player = (cf::CfGimmickLockPlayer*)getPlayer__Q22cf13CfGameManagerFi(0);
                    f32 f31 = player->field_63C;
                    CfGimmickLockGetTargetFn getTarget =
                        *(CfGimmickLockGetTargetFn*)(*(u32*)player + 0xac);
                    void* target = getTarget(player);
                    if (ColiCheckMoveRadius(&self->subA, target, 1, 0, 0.01f + f31) != 0) {
                        if ((self->flags & 0x8) == 0) {
                            CfGimmick_PostGimmickName(self->resourceId);
                            self->field_1F8 = 0x3c;
                            self->flags |= 0xc;
                        }
                    } else {
                        self->flags &= ~0x8;
                    }
                } else {
                    self->field_1F8 -= 1;
                }
            }
        }
        return;
    }

cleanup:
    // Deactivation path: release the registered regions.
    if ((self->flags & 1) != 0) {
        if (sEneColiProbe != 0) {
            Coli_ListRemoveNode(sEneColiProbe, &self->subA);
        }
        self->flags &= ~0x802;
    }
    if ((self->flags & 0x1000) != 0) {
        if (sEneColiProbe != 0) {
            Coli_ListRemoveNode(sEneColiProbe, &self->subB);
        }
        self->flags &= ~0x1000;
        self->field_1F4 = 2.0f;
    }
    if ((self->flags & 2) != 0) {
        self->flags &= ~0x803;
        CfGimmick_ClearManagerBinding(self);
        if ((self->flags & 0x1000) != 0) {
            if (sEneColiProbe != 0) {
                Coli_ListRemoveNode(sEneColiProbe, &self->subB);
            }
            self->flags &= ~0x1000;
            self->field_1F4 = 2.0f;
        }
    }
    if ((self->flags & 0xF00) != 0) {
        CfGimmickLock_MarkSlots(self, 0);
        self->flags &= ~0x1E0;
    }
}

// Deactivate the lock: clear the active/linked flags, detach the manager,
// release the outer region if it was registered, and reset the radius.
void gimmickLockClearBinding(cf::CfGimmickLock* self) {
    if (self->flags & 2) {
        // volatile re-read forces the retail lwz (MWCC would otherwise CSE
        // the two adjacent flag reads into one register).
        *(volatile u32*)&self->flags &= ~0x802;
        CfGimmick_ClearManagerBinding(self);
        if (self->flags & 0x1000) {
            if (sEneColiProbe != 0) {
                Coli_ListRemoveNode(sEneColiProbe, &self->subB);
            }
            self->flags &= ~0x1000;
            self->field_1F4 = 2.0f;
        }
    }
}

// (Re)build the lock geometry: recompute the placement matrix, then place the
// inner region according to the placement kind (1/2 = door, 3/4 = lift shape)
// and restart the unlock timer/state.
void CfGimmickLock_Rebuild(cf::CfGimmickLock* self) {
    CfGimmick_BuildColliderMtx(&self->scale, &self->position.x, &self->rotation);

    // Branchy bool so MWCC keeps the retail li/beq shape (not a bit extract).
    int flag = 0;
    if (self->configFlags & 0x4) {
        flag = 1;
    }

    if (self->stateIndex - 3 <= 1) {
        f32 vec[3];
        vec[1] = (self->extent1 - self->extent2) * 0.5f;
        vec[0] = self->extent0;
        vec[2] = self->extent3;
        ColiSetMoveVecFlagged(&self->subA, vec);
    } else if (self->stateIndex - 1 <= 1) {
        ColiSetMoveVec2(&self->subA, flag, self->extent0, self->extent1 - self->extent2);
    }

    if (self->stateIndex - 3 <= 1) {
        f32 vec[3];
        vec[1] = ((self->position.y + self->extent1) + (self->position.y + self->extent2)) * 0.5f;
        vec[0] = self->position.x;
        vec[2] = self->position.z;
        ColiSetAxisBlockRotInverse(&self->subA, vec, &self->rotation);
    } else if (self->stateIndex - 1 <= 1) {
        f32 vec[3];
        vec[0] = self->position.x;
        vec[1] = self->position.y + self->extent2;
        vec[2] = self->position.z;
        ColiSetAxisBlockInverse(&self->subA, vec);
    }

    self->subA.timer = 0x2710;
    // Branchy select matching the retail li/beq/li + word store.
    u32 initState = 1;
    if (self->configFlags & 1) {
        initState = 6;
    }
    self->subA.state = initState;
}

// Rebuild the lock geometry: register the inner region and place it per the
// placement kind, then run the timed unlock state machine on the outer region.
extern "C" void CfGimmickLock_Refresh(cf::CfGimmickLock* self) {
    if ((self->flags & 1) == 0) {
        if (sEneColiProbe != 0) {
            ColiListAppend(sEneColiProbe, &self->subA);
        }
        self->flags |= 1;
        if (self->stateIndex - 3 <= 1) {
            f32 t = (self->position.y + self->extent1) + (self->position.y + self->extent2);
            f32 vec[3];
            vec[0] = self->position.x;
            vec[2] = self->position.z;
            vec[1] = t * 0.5f;
            ColiSetAxisBlockRotInverse(&self->subA, vec, &self->rotation);
        } else if (self->stateIndex - 1 <= 1) {
            f32 vec[3];
            vec[0] = self->position.x;
            vec[1] = self->position.y + self->extent2;
            vec[2] = self->position.z;
            ColiSetAxisBlockInverse(&self->subA, vec);
        }
    }

    if ((self->configFlags & 1) == 0) {
        return;
    }

    if ((self->flags & 0x800) != 0) {
        // Closed state: pause the inner timer.
        self->subA.state = 2;
        self->subA.timer = 0x2710;
        return;
    }

    self->subA.state = 6;
    if ((self->flags & 2) == 0) {
        self->subA.timer = 1;
        return;
    }
    self->subA.timer = 0x2710;

    // Probe the lock area; on success mark it open and release the outer
    // region if it was registered.
    void* scene = Scn_HasCamItem(gmScene, gmCurrentView);
    int hit = jumptable_eu_80535830[self->stateIndex](
        (cf::CfGimmick*)&self->scale, (const ml::CVec3*)((u8*)scene + 0x10c), &self->position);
    if (hit != 0) {
        self->flags |= 0x800;
        if ((self->flags & 0x1000) != 0) {
            if (sEneColiProbe != 0) {
                Coli_ListRemoveNode(sEneColiProbe, &self->subB);
            }
            self->flags &= ~0x1000;
            self->field_1F4 = 2.0f;
        }
        return;
    }

    // Open countdown: shrink the radius, then place the outer region.
    f32 f = self->field_1F4 - 0.05f * Scn_GetFrameDelta(gmScene);
    self->field_1F4 = f;
    if (f < 1.0f) {
        self->field_1F4 = 1.0f;
    }

    int flag = (self->configFlags & 4) != 0 ? 1 : 0;
    if (self->stateIndex - 3 <= 1) {
        f32 vec[3];
        vec[0] = self->extent0;
        vec[1] = (self->extent1 - self->extent2) * 0.5f;
        vec[2] = self->extent3;
        VEC3Scale((nw4r::math::VEC3*)vec, (const nw4r::math::VEC3*)vec, self->field_1F4);
        ColiSetMoveVecFlagged(&self->subB, vec);

        f32 vec2[3];
        vec2[0] = self->position.x;
        vec2[1] = ((self->position.y + self->extent1) + (self->position.y + self->extent2)) * 0.5f;
        vec2[2] = self->position.z;
        ColiSetAxisBlockRotInverse(&self->subB, vec2, &self->rotation);
    } else if (self->stateIndex - 1 <= 1) {
        ColiSetMoveVec2(&self->subB, flag, self->extent0 * self->field_1F4,
                      self->field_1F4 * (self->extent1 - self->extent2));

        f32 vec[3];
        vec[0] = self->position.x;
        vec[1] = self->position.y + self->extent2;
        vec[2] = self->position.z;
        ColiSetAxisBlockInverse(&self->subB, vec);
    }

    self->subB.timer = 1;
    self->subB.state = 2;
    if ((self->flags & 0x1000) == 0) {
        self->flags |= 0x1000;
        if (sEneColiProbe != 0) {
            ColiListAppend(sEneColiProbe, &self->subB);
        }
    }
}

// Activate (or deactivate) the lock's linked game-manager object and its
// geometry.  The factory id depends on the placement kind.
extern "C" void CfGimmickLock_Activate(cf::CfGimmickLock* self) {
    if ((self->flags & 2) == 0) {
        if (self->stateIndex - 3 <= 1) {
            self->field_78 = static_cast<cf::CfObjectMove*>(createBattleActor__Q22cf13CfGameManagerFv(0x4d, 0));
        } else if (self->stateIndex - 1 <= 1) {
            self->field_78 = static_cast<cf::CfObjectMove*>(createBattleActor__Q22cf13CfGameManagerFv(0x4c, 0));
        } else {
            return;
        }
        self->flags |= 2;
        // field_78 is re-read around every opaque call so MWCC keeps the
        // retail reload sequence instead of caching the pointer.
        if (self->field_78 != 0) {
            self->field_78->mSubObjB0 = self;
            self->field_78->CfObject_setMoveHeadAngle(self->rotation.y);
            if (self->stateIndex - 3 <= 1) {
                f32 diff = self->extent1 - self->extent2;
                ml::CVec3 vec;
                vec.z = self->extent3 / 30.0f;
                vec.y = diff * 0.5f / 30.0f;
                vec.x = self->extent0 / 30.0f;
                self->field_78->CfObject_setMoveTargetVec(&vec);
                setChild34Sc_(self->field_78, &vec);
            } else {
                self->field_78->CfObject_setMoveTargetVec(&self->position);
                self->field_78->CfObject_setObjScale(self->extent0 / 30.0f);
            }
            // Touch the target of every fight-list entry.
            CfGimmickList* list = getReslistB48();
            CfGimmickListNode* node = list->head->next;
            while (node != list->head) {
                cf::CfObjectMove* entry = static_cast<cf::CfObjectMove*>(node->object);
                void* target = entry->CfObject_getCurrentTarget();
                if (target != 0) {
                    CtrlMoveEne_MarkActiveSteady((u8*)target + 0x84);
                }
                node = node->next;
            }
        }
    } else {
        cf::CfObjectMove* mgr = self->field_78;
        if (mgr != 0) {
            if (self->stateIndex - 3 <= 1) {
                f32 diff = self->extent1 - self->extent2;
                f32 vec[3];
                vec[2] = self->extent3 / 30.0f;
                vec[1] = diff * 0.5f / 30.0f;
                vec[0] = self->extent0 / 30.0f;
                setChild34Sc_(mgr, vec);
            } else if (self->stateIndex - 1 <= 1) {
                mgr->CfObject_setObjScale(self->extent0 / 30.0f);
            } else {
                // Out-of-range placement: skip the tail update entirely.
                return;
            }
        }
    }
    CfGimmick_SetGlobalFlag8008();
}

// Mark the lock's four lock-id slots: unused slots get their presence bit in
// the flags word, then every fight-list player whose id matches a slot is
// flagged "locked" (or unflagged when the lock is released).
extern "C" void CfGimmickLock_MarkSlots(cf::CfGimmickLock* self, int flag) {
    CfGimmickList* list;
    CfGimmickLockPlayer* base;
    u32 id;
    u32 mask = 0x20;
    CfGimmickListNode* node;

    // Tag every unused lock slot with its presence bit (retail unrolls this
    // constant-trip loop, keeping 0x20 << i as a shift).
    if (flag != 0) {
        for (int i = 0; i < 4; ++i) {
            if (self->lockId[i] == 0) {
                self->flags |= 0x20 << i;
            }
        }
    }

    list = getReslistB48();
    node = list->head->next;
    while (node != list->head) {
        base = (CfGimmickLockPlayer*)node->object;
        if (node->object != 0) {
            base = (CfGimmickLockPlayer*)((char*)node->object - 0x3E9C);
        }
        id = (u32)((s32)base->id456C >> 4);
        for (int i = 0; i < 4; ++i) {
            if (self->lockId[i] != 0 && id == self->lockId[i]) {
                if (flag != 0) {
                    base->flags3374 |= 0x800000;
                    self->flags |= mask << i;
                } else {
                    base->flags3374 &= ~0x800000;
                }
                break;
            }
        }
        node = node->next;
    }
}

// Active-lock probe: tail-call the placement checker for the lock's kind,
// passing the scale matrix, the caller's target and the position vector.
int gimmickLockDispatchState(cf::CfGimmickLock* self, void* target) {
    if (self->flags & 2) {
        return jumptable_eu_80535830[self->stateIndex]((cf::CfGimmick*)&self->scale, (const ml::CVec3*)target, &self->position);
    }
    return 0;
}

// typed data sections -- source shape for retail bytes
// .rodata 0x20: RTTI name string followed by the BDAT column-name string.
extern "C" {
extern char lbl_eu_80508668[];
extern char lbl_eu_805358EC[];
}
__attribute__((used)) const char lbl_eu_805086D8[0x14] = "cf::CfGimmickLock";

__attribute__((used)) const void* __RTTI__Q22cf12CfGimmickElv[2] = { lbl_eu_80508668, lbl_eu_805358EC };
