#pragma once

// Thin owner-DATA header for kyoshin/cf/CfGameManager.cpp. The definitions
// of these .sdata globals live in CfGameManager.cpp; TUs whose include
// closure carries divergent provisional copies of the same label include
// this instead of re-declaring it locally.

class CScn;

// Shared scene object (@sdata21); set by cf::CfGameManager.
extern "C" CScn* lbl_eu_80663E14;

// Event/presentation flag word (.sbss); canonical decl for the whole tree.
// Reads that must stay separate loads use explicit volatile casts
// (e.g. *(volatile u32*)&lbl_eu_80663E24).
extern "C" u32 lbl_eu_80663E24;
