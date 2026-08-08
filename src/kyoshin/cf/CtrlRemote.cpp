// Auto-scaffolded catalog TU for kyoshin/cf/CtrlRemote
// Replace stubs with high-level C/C++ during decomp.

#include "kyoshin/harness_catalog.hpp"

#include "kyoshin/cf/CtrlRemote.hpp"

#include <string.h>

void __ct__CtrlRemote(){}

void func_80098CB8(){}

void func_80098EF8(){}

float CActorParam_UnkVirtualFunc106__Q22cf11CActorParamFv(void* self) { return *(float*)((u8*)self + 0x1840); }

void func_8009A1DC(){}

void func_8009A4AC(){}

void func_8009AE80(){}

void func_8009B788(){}

void func_8009BD14(){}

void func_8009C1BC(){}

void func_8009C6B4() {}

void func_8009C6B8() {}

void func_8009C6BC(){}

void func_8009C860(){}

void func_8009C87C(){}

void func_8009C8F4(){}

void func_8009C980(){}

void func_8009C9B8(){}

void func_8009CAAC(){}

void func_8009CB80(){}

void func_8009CE14(){}

void func_8009CE88(){}

void func_8009CF0C(){}

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

void func_8009D018(){}

void func_8009D0B4(){}

u32* func_8009D12C(u32* buffer, s32 index, s32* typeOut, s32* idxOut)
{
	if ((u32)index <= 0x1F) {
		*idxOut = 0;
		*typeOut = 4;
		return &buffer[index];
	}

	s32 r7 = index - 0x20;
	if ((u32)r7 <= 0x1FF) {
		*idxOut = r7;
		s32 r4 = r7 / 2;
		*typeOut = 3;
		return &buffer[r4 + 0x20];
	}

	r7 = index - 0x220;
	if ((u32)r7 <= 0x7FF) {
		*idxOut = r7;
		s32 r4 = (r7 + 3) / 4;
		*typeOut = 2;
		return &buffer[r4 + 0x120];
	}

	s32 r4 = index - 0xA20;
	if ((u32)r4 <= 0x2CAF) {
		*idxOut = r4;
		r4 = (r4 + 31) / 32;
		*typeOut = 1;
		return &buffer[r4 + 0x320];
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

void func_8009D2C8(){}

void func_8009D414(){}

void func_8009D514(){}

extern "C" bool func_8009D5FC() { return false; }
