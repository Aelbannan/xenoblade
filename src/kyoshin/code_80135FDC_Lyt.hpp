#pragma once

#include <types.h>
#include <revolution/tpl/TPL.h>

namespace nw4r {
namespace ut {
class Font;
}
namespace math {
struct VEC3;
}
namespace lyt {
class Layout;
class Pane;
class AnimTransform;
} // namespace lyt
} // namespace nw4r

// Lean public header for kyoshin/code_80135FDC: the layout / pane / vector
// helpers of that unit. Single canonical declaration of each helper -- do NOT
// re-declare these anywhere else (C linkage: a second declaration with
// different parameter types is a hard "illegal function overloading" error).
// Retail names are unmangled (C linkage).
//
//   layout = nw4r::lyt::Layout* (the "ByName" helpers look the pane up from
//            layout->GetRootPane(), i.e. the Pane* at Layout+0x10)
//   name   = pane name
//   color  = packed RGBA (nw4r::ut::Color bit pattern, passed in a GPR)
//   tagProc = CTagProcessorBase* (or 0); passed through as an integer
extern "C" {

// --- text boxes ---
void ApplyFontToTxtPanes(nw4r::lyt::Pane* pane, const nw4r::ut::Font* font);
void LayoutSetTextBoxText(nw4r::lyt::Layout* layout, const char* name, const char* text,
                          u32 tagProc);
// Formats `text` through the "%s" table string (retail never reads a format
// argument) and forwards to LayoutSetTextBoxText.
void LayoutSetTextBoxFmtValue(nw4r::lyt::Layout* layout, const char* name, const char* text,
                              u32 tagProc);
void LayoutSetTextBoxInt(nw4r::lyt::Layout* layout, u32 value);
void SetLayoutSelfText(nw4r::lyt::Layout* layout, const char* text, u32 tagProc);

// --- animation ---
u32 AnimRewindFrame(nw4r::lyt::AnimTransform* anim, f32 delta);

// --- pane world positions / vectors ---
void AccumPaneWorldPos(nw4r::math::VEC3* out, nw4r::lyt::Pane* pane);
void AccumPaneWorldPosB(nw4r::math::VEC3* out, nw4r::lyt::Pane* pane);
void ScalePaneWorldPos(nw4r::math::VEC3* out, nw4r::lyt::Pane* pane, nw4r::lyt::Pane* scale,
                       nw4r::lyt::Pane* offset);
// Returns `out` (r3 untouched); callers chain it into the next call.
nw4r::math::VEC3* code80135FDC_setVec3(nw4r::math::VEC3* out, f32 x, f32 y, f32 z);
void Vec3AddInto(nw4r::math::VEC3* out, const nw4r::math::VEC3* value);
int CheckViewAngle60(const nw4r::math::VEC3* a, const nw4r::math::VEC3* b,
                     const nw4r::math::VEC3* c);

// --- pane vertex colours / textures ---
void PaneSetVtxColorByName(nw4r::lyt::Layout* layout, const char* name, u32 color);
void PaneSetVtxColorAll(nw4r::lyt::Pane* pane, u32 color);
void PaneSetVtxColorPairsByName(nw4r::lyt::Layout* layout, const char* name, u32 colorA,
                                u32 colorB);
void PaneSetVtxColorPairs(nw4r::lyt::Pane* pane, u32 colorA, u32 colorB);
void PaneSetTexPaletteByName(nw4r::lyt::Layout* layout, const char* name, TPLPalette* palette);
void PaneSetTexPalette(nw4r::lyt::Pane* pane, TPLPalette* palette);

} // extern "C"
