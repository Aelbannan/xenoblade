#pragma once

// Thin owner-DATA header for kyoshin/cf/CfGameManager.cpp. The definitions
// of these .sdata globals live in CfGameManager.cpp; TUs whose include
// closure carries divergent provisional copies of the same label include
// this instead of re-declaring it locally.

class CScn;

// Shared scene object (@sdata21) and event/presentation flag word (.sbss):
// canonical declarations for the whole tree; definitions in CfGameManager.cpp.
// Reads that must stay separate loads use explicit volatile casts
// (e.g. *(volatile u32*)&gmGameFlags).
extern CScn* gmScene;
extern u32 gmGameFlags;

// CfGameManager mode-flag word (.sbss; sda21 access).
extern u32 gmPauseFlags;

// Presentation flag word and aim-point vector owned by this module (.sbss/.bss).
extern u32 gmPresentationFlag2C;
extern float gmAimPoint[3];
