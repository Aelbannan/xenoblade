#include "kyoshin/coop/PresentationGate.hpp"

#include "kyoshin/CGame.hpp"
#include "kyoshin/CUICfManager.hpp"
#include "kyoshin/CUIWindowManager.hpp"
#include "monolib/lib/CLibHbm.hpp"

// Retail SDA presentation / mode words (DISCOVERY.md §10).
extern "C" {
u32 lbl_eu_80663E24;
u32 lbl_eu_80663E28;
}

namespace coop {

static bool IsModalMenuActive() {
    CUIWindowManager* windows = CUIWindowManager::getInstance();
    if (windows != NULL && windows->hasOpenWindows()) {
        return true;
    }

    CUICfManager* ui = CUICfManager::getInstance();
    if (ui != NULL && ui->getFlags() != 0) {
        return true;
    }

    return false;
}

static bool IsPauseOrHbmActive() {
    if (CLibHbm::func_8045DE00()) {
        return true;
    }

    CGame* game = CGame::getInstance();
    if (game != NULL && game->unk228 > 0) {
        return true;
    }

    return false;
}

bool ShouldRenderSplitScreen() {
    PresentationSnapshot snap;
    snap.eventFlags = lbl_eu_80663E24;
    snap.modeFlags = lbl_eu_80663E28;
    snap.pauseOrHbm = IsPauseOrHbmActive();
    snap.modalMenu = IsModalMenuActive();
    return ShouldRenderSplitScreenFrom(snap);
}

void RestoreFullScreenPresentation() {
    // TODO (split renderer): restore view rects, current view, camera,
    // HUD bindings, culling frustums, and GX viewport/scissor/projection
    // before a full-screen pass. Predicate-only for now (TASKS should-render-split).
}

} // namespace coop
