#pragma once

// Forward declaration for adjusting thunks.
// Full class definition TBD as more functions are decompiled.
class CMenuPassiveSkill;

extern "C" void __ct__CMenuPassiveSkill();
extern "C" void __dt__17CMenuPassiveSkillFv(void* self);
extern "C" void Init__17CMenuPassiveSkillFv();
extern "C" void Term__17CMenuPassiveSkillFv();
extern "C" void Move__17CMenuPassiveSkillFv();
extern "C" void cbRenderBefore__17CMenuPassiveSkillFv(void* self);
extern "C" void func_802638D0();
extern "C" void func_80263944();
extern "C" void func_80263954();
extern "C" void func_802639E4();
extern "C" void func_80263A34();
extern "C" void func_80263D3C();
extern "C" void func_80263D8C();
extern "C" void func_80263DE8();
extern "C" void func_80263E4C();

// Adjusted-this thunks.
// These are called through a secondary base vtable entry at offset +0x58
// within CMenuPassiveSkill (IScnRender interface). They adjust this back
// to the full object before tail-calling the actual implementation.
extern "C" void func_80263EAC(CMenuPassiveSkill* self);
extern "C" void func_80263EB4(CMenuPassiveSkill* self);
