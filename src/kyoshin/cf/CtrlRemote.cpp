// Auto-scaffolded catalog TU for kyoshin/cf/CtrlRemote
// Replace stubs with high-level C/C++ during decomp.

#include "kyoshin/harness_catalog.hpp"

#include "kyoshin/cf/CtrlRemote.hpp"
// CtrlPc.hpp scaffolds __ct__CtrlRemote as `void`; this TU declares the real
// returning signature (CtrlRemote.hpp). Hide the scaffold decl so the two do
// not conflict (the hidden name is never referenced in this TU).
#define __ct__CtrlRemote __ct__CtrlRemote_scaffold
#include "kyoshin/cf/CtrlPc.hpp"
#undef __ct__CtrlRemote
#include "kyoshin/cf/CfGameManager.hpp"
#include "monolib/core/CPadManager.hpp"
#include "monolib/util/MemManager.hpp"

#include <string.h>

cf::CtrlRemote* __ct__CtrlRemote(cf::CtrlRemote* obj, void* posObj, int arg5)
{
    __ct__cf_CtrlPc(reinterpret_cast<cf::CtrlPc*>(obj), posObj, (void*)arg5);
    CtrlRemoteLayout* self = reinterpret_cast<CtrlRemoteLayout*>(obj);
    // novtable class: store the retail vtable manually.
    self->vtbl = (void*)lbl_eu_80527F88;
    self->mField25C = 0;
    self->mField260 = 0;
    for (int i = 0; i < 32; i++) {
        self->mFloats1DC[i] = lbl_eu_80666730;
    }
    CPadManager::updateLongHoldTimerThreshold(0xa);
    return obj;
}

void func_80098CB8(){}

void func_80098EF8(){}

float CActorParam_UnkVirtualFunc106__Q22cf11CActorParamFv(void* self) { return *(float*)((u8*)self + 0x1840); }

// Target us-8009abb4. Pad-handler gate: when the global "demo/idle" state
// word clears, run a battery of player-state probes; if none of them flag a
// blocking condition, mirror the current menu-state bitmask into self->mField4
// bit 0 (set when the arts/menu mask is active, clear otherwise).
void func_8009A1DC(cf::CtrlPc* self)
{
    bool flag = true;
    cf::CfGameManager::getInstance();
    if (func_8006EF04(0x400) != 0) {
        return;
    }
    if (func_80148778(&self->mField5C->mField8, 0xf) != 0 ||
        func_80148778(&self->mField5C->mField8, 0x6) != 0) {
        flag = false;
    }
    CtrlPlayerObj* player = self->mField5C;
    u32 v1 = *player->mField4->vf30();
    if (func_80174C98(player, &v1, 1) != 0) {
        flag = false;
    } else {
        CtrlPlayerObj* player2 = self->mField5C;
        u32 v2 = *player2->mField4->vf30();
        if (func_80174C98(player2, &v2, 2) != 0) {
            flag = false;
        }
    }
    if (self->mField5C->mSub3ED4->vf14(0x40000) != 0) {
        flag = false;
    }
    if (func_800FEDF8() != 0) {
        flag = false;
    }
    if (self->mField5C->mSub3ED4->vf14(0x400) != 0) {
        flag = false;
    }
    CtrlPlayerObj* player3 = self->mField5C;
    u32 v3 = *player3->mField4->vf30();
    if (func_80174C98(player3, &v3, 3) == 0) {
        CtrlPlayerObj* player4 = self->mField5C;
        u32 v4 = *player4->mField4->vf30();
        if (func_80174C98(player4, &v4, 4) == 0) {
            flag = false;
        }
    }
    if (func_800FE68C()->mField90E4 != 0) {
        flag = false;
    }
    if (flag) {
        u32 mask = func_80086F9C__Q22cf13CfGameManagerFv(-1)
                       ? lbl_eu_80527F10[2]
                       : lbl_eu_80527E98[2];
        if (self->vf37()->mField4 & mask) {
            if (func_80086F9C__Q22cf13CfGameManagerFv(-1) != 0 &&
                (self->vf37()->mField0 & 0x2000000)) {
                return;
            }
            u32 mask2 = func_80086F9C__Q22cf13CfGameManagerFv(-1)
                            ? lbl_eu_80527F10[2]
                            : lbl_eu_80527E98[2];
            if (self->vf37()->mField4 & mask2) {
                self->mField4 |= 1;
            } else {
                self->mField4 &= ~1;
            }
        }
    }
}

void func_8009A4AC(){}

void func_8009AE80(){}

void func_8009B788(){}

void func_8009BD14(){}

void func_8009C1BC(){}

void func_8009C6B4() {}

void func_8009C6B8() {}

void func_8009C6BC(){}

// Tail-call wrapper: report whether the [0x800,0x1000) flag range is set on
// the player actor's +0x3ED4 flag object (virtual slot 0x40).
u32 func_8009C860(cf::CtrlRemote* self)
{
    cf::CtrlPc* pc = reinterpret_cast<cf::CtrlPc*>(self);
    return pc->mField5C->mSub3ED4->vf14(0x800);
}

// Returns 1 when the 0x800 flag range is set, else 1 when the 0x1000 range is
// clear (inverse of the second query).
u32 func_8009C87C(cf::CtrlRemote* self)
{
    cf::CtrlPc* pc = reinterpret_cast<cf::CtrlPc*>(self);
    if (pc->mField5C->mSub3ED4->vf14(0x800) != 0) {
        return 1;
    }
    return !pc->mField5C->mSub3ED4->vf14(0x1000);
}

// Allocate a*b bytes (4-aligned) from the default heap, zero the first b
// bytes, and fall back to the MEM2 heap if the first allocation fails.
// `self` is unused - the retail function still takes it in r3.
void* func_8009C8F4(cf::CtrlRemote* self, u32 a, u32 b)
{
    u32 total = a * b;
    void* p = mtl::MemManager::allocate_head(func_80061FE8(), total, 4);
    if (p != NULL && b != 0) {
        memset(p, 0, b);
    }
    if (p == NULL) {
        p = mtl::MemManager::allocate_head(mtl::MemManager::getHandleMEM2(), total, 4);
    }
    return p;
}

// Free a heap buffer when non-null.
void func_8009C980(cf::CtrlRemote* self, u8* ptr)
{
    if (ptr != NULL) {
        mtl::MemManager::deallocate(ptr);
    }
}

// Type-2 control-data parser: build a 0x38-byte UnkClass_80460C34Ctx around
// the payload, initialize it via the retail stream ctor, then validate the
// stream state (v==0/1 accepted) before returning the payload size.
u32 func_8009CAAC(cf::CtrlRemote* self, CtrlRemoteBuf* buf, u32 arg)
{
    UnkClass_80460C34Ctx ctx;
    u32 size = buf->mSize;
    ctx.mSize = size;
    ctx.mArg = arg;
    ctx.mSelf = self;
    ctx.mAlloc = (void*)func_8009C8F4;
    ctx.mFree = (void*)func_8009C980;
    ctx.mZero = 0;
    ctx.mPayload = (u8*)buf + 8;
    if (func_80460DCC__17UnkClass_80460C34Fv(&ctx, lbl_eu_804FBC20, 0x38) != 0) {
        return 0;
    }
    s32 v = func_80460F58__17UnkClass_80460C34Fv(&ctx, 4);
    if (v != 1) {
        func_80461FE0__17UnkClass_80460C34Fv(&ctx);
        if (v != 0) {
            return 0;
        }
    }
    if (func_80461FE0__17UnkClass_80460C34Fv(&ctx) != 0) {
        return 0;
    }
    return size;
}

// Parse a serialized control buffer into this: type 1/2/3 dispatch to the
// typed parsers, otherwise the payload is memcpy'd and its size returned.
u32 func_8009CE14(cf::CtrlRemote* self, CtrlRemoteBuf* src, s32 type, u32 arg)
{
    if (type == 1) {
        return func_8009C9B8(self, src, arg);
    }
    if (type == 2) {
        return func_8009CAAC(self, src, arg);
    }
    if (type == 3) {
        return func_8009CB80(self, src, arg);
    }
    memcpy(self, src->mData, src->mSize);
    return src->mSize;
}

// Clear the shared control-data buffer (initializing it first if needed).
void func_8009CE88()
{
    s32 flag = (s8)lbl_eu_80663E80;
    if (flag == 0) {
        memset(lbl_eu_80571848, 0, 0x1214);
        memset(&lbl_eu_80571848[0x1234 / 4], 0, 0x20);
        memset(&lbl_eu_80571848[0x1214 / 4], 0, 0x20);
        lbl_eu_80663E80 = 1;
    }
    memset(lbl_eu_80571848, 0, 0x1214);
}

// Return a pointer to the shared control-data buffer, initializing it once.
u32* func_8009CF0C()
{
    s32 flag = (s8)lbl_eu_80663E80;
    if (flag == 0) {
        memset(lbl_eu_80571848, 0, 0x1214);
        memset(&lbl_eu_80571848[0x1234 / 4], 0, 0x20);
        memset(&lbl_eu_80571848[0x1214 / 4], 0, 0x20);
        lbl_eu_80663E80 = 1;
    }
    return lbl_eu_80571848;
}

int func_8009CF84(void* self) { return 4628; }

u32* func_8009D12C(u32* buffer, s32 index, s32* typeOut, s32* idxOut);

extern "C" void func_8009CF8C(void* arg)
{
	s32 flag = (s8)lbl_eu_80663E80;
	if (flag == 0) {
		memset(lbl_eu_80571848, 0, 0x1214);
		memset(&lbl_eu_80571848[0x1234 / 4], 0, 0x20);
		memset(&lbl_eu_80571848[0x1214 / 4], 0, 0x20);
		lbl_eu_80663E80 = 1;
	}
	func_8009D1F8(lbl_eu_80571848, (s32)arg);
}

// Set a control-data bit: initialize the shared buffer if needed, then write
// `value` at index `destination` via the retail bit-setter.
void func_8009D018(u32 destination, u32 value)
{
    s32 flag = (s8)lbl_eu_80663E80;
    if (flag == 0) {
        memset(lbl_eu_80571848, 0, 0x1214);
        memset(&lbl_eu_80571848[0x1234 / 4], 0, 0x20);
        memset(&lbl_eu_80571848[0x1214 / 4], 0, 0x20);
        lbl_eu_80663E80 = 1;
    }
    func_8009D2C8(lbl_eu_80571848, destination, value);
}

// Return a pointer to the shared control-data buffer, initializing it once.
u32* func_8009D0B4()
{
    s32 flag = (s8)lbl_eu_80663E80;
    if (flag == 0) {
        memset(lbl_eu_80571848, 0, 0x1214);
        memset(&lbl_eu_80571848[0x1234 / 4], 0, 0x20);
        memset(&lbl_eu_80571848[0x1214 / 4], 0, 0x20);
        lbl_eu_80663E80 = 1;
    }
    return lbl_eu_80571848;
}

u32* func_8009D12C(u32* buffer, s32 index, s32* typeOut, s32* idxOut)
{
	if ((u32)index <= 0x1F) {
		*idxOut = 0;
		*typeOut = 4;
		return &buffer[index];
	}

	s32 v = index - 0x20;
	if ((u32)v <= 0x1FF) {
		*idxOut = v;
		*typeOut = 3;
		return &buffer[v / 2 + 0x20];
	}

	v = index - 0x220;
	if ((u32)v <= 0x7FF) {
		*idxOut = v;
		*typeOut = 2;
		return &buffer[v / 4 + 0x120];
	}

	v = index - 0xA20;
	if ((u32)v <= 0x2CAF) {
		*idxOut = v;
		*typeOut = 1;
		return &buffer[v / 32 + 0x320];
	}

	*idxOut = 0;
	*typeOut = 0;
	return NULL;
}

extern "C" u32 func_8009D1F8(u32* buffer, s32 index)
{
	s32 shift;
	s32 type;
	s32 idx;
	u32 mask;
	u32* ptr;

	shift = 0;
	ptr = func_8009D12C(buffer, index, &type, &idx);
	if (ptr == NULL) {
		return 0;
	}

	switch (type) {
	case 4:
		mask = 0xFFFFFFFF;
		shift = 0;
		break;
	case 3:
		mask = lbl_eu_80661C70[idx & 1];
		shift = (idx & 1) << 4;
		break;
	case 2:
		mask = lbl_eu_80528048[idx & 3];
		shift = (idx & 3) << 3;
		break;
	case 1:
		mask = lbl_eu_80528058[idx & 0x1F];
		shift = idx & 0x1F;
		break;
	}

	return (*ptr & mask) >> shift;
}

// Register a control callback object into one of the 8 shared control-data
// slots (buf + 0x1234 + 4*i). The shared buffer is lazily zero-initialized
// once. NOTE: guard1 reads the global directly (not the local) - MWCC hoists
// that loop-invariant load into the preheader and coalesces it with the
// `flag` local, so the retail's pre-loop lbz + per-iteration extsb. + the
// `li r0,1` after the first init all come out naturally. Removing the local
// or reading it in the guard regresses the bytes.
void func_8009D414(void* obj)
{
    s8 flag = lbl_eu_80663E80;
    for (s32 i = 0; i < 8; i++) {
        if ((s8)lbl_eu_80663E80 == 0) {
            memset(lbl_eu_80571848, 0, 0x1214);
            memset(&lbl_eu_80571848[0x1234 / 4], 0, 0x20);
            memset(&lbl_eu_80571848[0x1214 / 4], 0, 0x20);
            lbl_eu_80663E80 = 1;
            flag = 1;
        }
        if (lbl_eu_80571848[0x1234 / 4 + i] == 0) {
            if ((s8)lbl_eu_80663E80 == 0) {
                memset(lbl_eu_80571848, 0, 0x1214);
                memset(&lbl_eu_80571848[0x1234 / 4], 0, 0x20);
                memset(&lbl_eu_80571848[0x1214 / 4], 0, 0x20);
                lbl_eu_80663E80 = 1;
                flag = 1;
            }
            lbl_eu_80571848[0x1234 / 4 + i] = (u32)obj;
            break;
        }
    }
}

// Clear a control-data slot: ensure the shared buffer is initialized once
// (the retail keeps the flag in a local across the whole loop, so both init
// guards survive), then find the slot holding `index` and clear it.
// Clear a control-data slot: ensure the shared buffer is initialized once
// (the retail keeps the flag in a local across the whole loop, so both init
// guards survive), then find the slot holding `index` and clear it.
void func_8009D514(u32 index)
{
    s32 flag = (s8)lbl_eu_80663E80;
    u32* buf = lbl_eu_80571848;
    for (s32 i = 0; i < 8; i++) {
        if (flag == 0) {
            memset(lbl_eu_80571848, 0, 0x1214);
            memset(&buf[0x1234 / 4], 0, 0x20);
            memset(&buf[0x1214 / 4], 0, 0x20);
            lbl_eu_80663E80 = 1;
            flag = 1;
        }
        if (buf[0x1234 / 4 + i] == index) {
            if (flag == 0) {
                memset(lbl_eu_80571848, 0, 0x1214);
                memset(&lbl_eu_80571848[0x1234 / 4], 0, 0x20);
                memset(&lbl_eu_80571848[0x1214 / 4], 0, 0x20);
                lbl_eu_80663E80 = 1;
                flag = 1;
            }
            buf[0x1234 / 4 + i] = 0;
        }
    }
}

// Read-only accessor for the global flag word at lbl_eu_80663E88.
u32 func_8009D5FC() { return lbl_eu_80663E88; }
