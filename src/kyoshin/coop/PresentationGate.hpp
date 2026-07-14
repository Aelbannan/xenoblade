#pragma once

#include <types.h>

// Fork-only presentation gating for single-instance split-screen (PLAN.md /
// DISCOVERY.md §10). Not a retail TU — do not expect MWCC FULL_MATCH.

namespace coop {

// lbl_eu_80663E24 — primary event / presentation bitfield (IBM bit 0 = MSB).
static const u32 kEventVision = 0x04000000u;      // IBM bit 5 — isVisionEvent
static const u32 kEventREvent = 0x02000000u;      // IBM bit 6 — realtime / CTaskREvent
static const u32 kEventTalk = 0x01000000u;        // IBM bit 7 — isTalkEvent
static const u32 kEventIsEvent = 0x00400000u;     // IBM bit 9 — isEvent

// lbl_eu_80663E28 — secondary mode bitfield.
static const u32 kModeRealtimeBusy = 0x00000001u; // bit 0 — realtimeEventStart
static const u32 kModeFade = 0x00000002u;         // IBM bit 30 — pluginEve fade
static const u32 kModeKizunaTalk = 0x00001000u;   // IBM bit 19
static const u32 kModeFieldVision = 0x20000000u;  // IBM bit 2 — setFieldVision

// Wide UI suppress mask used by MainMenu / ArtsSelect (andis 0xAFA4).
static const u32 kEventUiSuppressComposite = 0xAFA40000u;

// Synthetic mask bits (not SDA) for pause/HBM and modal UI.
static const u32 kSynthPauseOrHbm = 0x80000000u;
static const u32 kSynthModalMenu = 0x40000000u;

struct PresentationSnapshot {
    u32 eventFlags;  // lbl_eu_80663E24
    u32 modeFlags;   // lbl_eu_80663E28
    bool pauseOrHbm; // CGame::unk228 > 0 or HBM active
    bool modalMenu;  // CUIWindowManager queues or CUICfManager::mFlags
};

// Bits that caused suppress (0 => allow split). Useful for TRACE / debug.
inline u32 PresentationSuppressMask(const PresentationSnapshot& snap) {
    u32 mask = 0;

    if (snap.pauseOrHbm) {
        mask |= kSynthPauseOrHbm;
    }
    if (snap.modalMenu) {
        mask |= kSynthModalMenu;
    }

    if (snap.eventFlags & kEventVision) {
        mask |= kEventVision;
    }
    if (snap.eventFlags & kEventREvent) {
        mask |= kEventREvent;
    }
    if (snap.eventFlags & kEventTalk) {
        mask |= kEventTalk;
    }
    if (snap.eventFlags & kEventIsEvent) {
        mask |= kEventIsEvent;
    }
    if (snap.eventFlags & kEventUiSuppressComposite) {
        mask |= (snap.eventFlags & kEventUiSuppressComposite);
    }

    if (snap.modeFlags & kModeRealtimeBusy) {
        mask |= kModeRealtimeBusy;
    }
    if (snap.modeFlags & kModeFade) {
        mask |= kModeFade;
    }
    if (snap.modeFlags & kModeKizunaTalk) {
        mask |= kModeKizunaTalk;
    }
    if (snap.modeFlags & kModeFieldVision) {
        mask |= kModeFieldVision;
    }

    return mask;
}

inline bool ShouldRenderSplitScreenFrom(const PresentationSnapshot& snap) {
    return PresentationSuppressMask(snap) == 0;
}

// Live game: reads SDA + managers. Returns true when split views should draw.
bool ShouldRenderSplitScreen();

// Placeholder for full-screen restore (view/camera/HUD/culling/GX).
// Call before rendering a full-screen presentation when leaving split.
void RestoreFullScreenPresentation();

} // namespace coop
