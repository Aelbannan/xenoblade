#pragma once

#include <types.h>
#include <revolution/tpl/TPL.h>
#include <revolution/gx/GXTypes.h>

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
class AnimResource;
class AnimationLink;
class ArcResourceAccessor;
class DrawInfo;
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
// Pane-tree animation links (pane, its materials, then children recursively).
nw4r::lyt::AnimationLink* FindAnimLink(nw4r::lyt::Pane* pane, const nw4r::lyt::AnimResource* res);
void SetAnimLinkFrame(nw4r::lyt::Pane* pane, const nw4r::lyt::AnimResource* res, f32 frame);
// Advances the frame by one; returns 1 when the last frame was reached (or no
// link exists), 0 otherwise.
int StepAnimLinkFrame(nw4r::lyt::Pane* pane, const nw4r::lyt::AnimResource* res);
void AnimResetToFirst(nw4r::lyt::Pane* pane, const nw4r::lyt::AnimResource* res);
void AnimJumpToLast(nw4r::lyt::Pane* pane, const nw4r::lyt::AnimResource* res);

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

// --- material TEV colours (Material::mTevCols, GXColorS10; idx < 3) ---
// GXColorS10 (8 bytes) is returned in r3:r4. An absent pane/material gives
// {0, 0, 0, 0}.
GXColorS10 PaneMatGetS16QuadByName(nw4r::lyt::Layout* layout, const char* name, u32 idx);
GXColorS10 PaneMatGetS16Quad(nw4r::lyt::Pane* pane, u32 idx);
void PaneMatSetS16QuadByName(nw4r::lyt::Layout* layout, const char* name, const GXColorS10* color,
                             u32 idx);
void PaneMatSetS16Quad(nw4r::lyt::Pane* pane, const GXColorS10* color, u32 idx);
// Sets TEV colours 0 and 1 (the black/white interpolation pair).
void PaneMatSetTevColorsByName(nw4r::lyt::Layout* layout, const char* name, const GXColorS10* c0,
                               const GXColorS10* c1);
void PaneMatSetTevColors(nw4r::lyt::Pane* pane, const GXColorS10* c0, const GXColorS10* c1);

// --- text box colours (TextBox::mTextColors at +0xDC, packed RGBA) ---
void PaneSetColorFieldSingle(nw4r::lyt::Layout* layout, const char* name, u32 color);
void PaneSetColorFieldPair(nw4r::lyt::Layout* layout, const char* name, u32 top, u32 bottom);

} // extern "C"

// --- layout lifetime / drawing (retail C++-linkage symbols, e.g.
// drawLayout__FPQ34nw4r3lyt6LayoutPQ34nw4r3lyt8DrawInfoii) ---
void buildLayout(nw4r::lyt::Layout** ppLayout, nw4r::lyt::ArcResourceAccessor* accessor,
                 const char* name);
void bindLayoutAnimTransform(nw4r::lyt::Layout* layout, nw4r::lyt::AnimTransform** ppAnim,
                             nw4r::lyt::ArcResourceAccessor* accessor, char* name);
void drawLayout(nw4r::lyt::Layout* layout, nw4r::lyt::DrawInfo* drawInfo, int useProjection,
                int calcMtx);
void SetWideLocAdjust(nw4r::lyt::DrawInfo* drawInfo);
// Advances the frame by `delta`; returns 1 once the last frame is reached.
u32 advanceAnimTransform(nw4r::lyt::AnimTransform* anim, f32 delta);
void releaseArcResourceAccessor(nw4r::lyt::ArcResourceAccessor* accessor);
// `font` is a nw4r::ut::Font* passed as a word (retail symbol
// setLayoutTextBoxFont__FPQ34nw4r3lyt6LayoutPcUl).
void setLayoutTextBoxFont(nw4r::lyt::Layout* layout, char* name, u32 font);
void setLayoutTextBoxNumber(nw4r::lyt::Layout* layout, char* name, int value);

extern "C" void LayoutBindAnimResource(nw4r::lyt::Layout* layout, void** ppAnimRes,
                                       nw4r::lyt::ArcResourceAccessor* accessor, char* name);
