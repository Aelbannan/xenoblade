#include <cstdio>
#include <cstdlib>

#include "kyoshin/coop/PresentationGate.hpp"

using coop::PresentationSnapshot;
using coop::ShouldRenderSplitScreenFrom;
using coop::PresentationSuppressMask;

static PresentationSnapshot snap(u32 e24, u32 e28, bool pause, bool modal) {
    PresentationSnapshot s{};
    s.eventFlags = e24;
    s.modeFlags = e28;
    s.pauseOrHbm = pause;
    s.modalMenu = modal;
    return s;
}

static void run_scenario(const char* name, u32 e24, u32 e28, bool pause, bool modal, bool expectAllow) {
    PresentationSnapshot s = snap(e24, e28, pause, modal);
    bool allow = ShouldRenderSplitScreenFrom(s);
    u32 mask = PresentationSuppressMask(s);
    if (allow != expectAllow || (expectAllow && mask != 0) || (!expectAllow && mask == 0)) {
        std::printf("FAIL %s allow=%d expect=%d mask=0x%08lX e24=0x%08lX e28=0x%08lX pause=%d modal=%d\n", name,
                    (int)allow, (int)expectAllow, (unsigned long)mask, (unsigned long)e24, (unsigned long)e28,
                    (int)pause, (int)modal);
        std::exit(1);
    }
    std::printf("PASS %s\n", name);
}

int main() {
    // Allow split — clean field.
    run_scenario("field_clean", 0, 0, false, false, true);
    run_scenario("field_other_e24", 0x00000008u, 0, false, false, true);
    run_scenario("field_other_e28", 0, 0x00000010u, false, false, true);

    // Pause / HBM / modal.
    run_scenario("pause", 0, 0, true, false, false);
    run_scenario("modal", 0, 0, false, true, false);
    run_scenario("pause_and_modal", 0, 0, true, true, false);

    // Named E24 presentation bits.
    run_scenario("vision", coop::kEventVision, 0, false, false, false);
    run_scenario("revent", coop::kEventREvent, 0, false, false, false);
    run_scenario("talk", coop::kEventTalk, 0, false, false, false);
    run_scenario("is_event", coop::kEventIsEvent, 0, false, false, false);

    // Named E28 mode bits.
    run_scenario("rt_busy", 0, coop::kModeRealtimeBusy, false, false, false);
    run_scenario("fade", 0, coop::kModeFade, false, false, false);
    run_scenario("kizuna", 0, coop::kModeKizunaTalk, false, false, false);
    run_scenario("field_vision", 0, coop::kModeFieldVision, false, false, false);

    // Composite UI suppress (MainMenu / ArtsSelect andis 0xAFA4).
    run_scenario("ui_composite_a", 0x80000000u, 0, false, false, false); // bit in AFA4
    run_scenario("ui_composite_f", 0x0F000000u, 0, false, false, false);
    run_scenario("ui_composite_4n", 0x00040000u, 0, false, false, false); // AFA4 low nibble
    run_scenario("ui_composite_partial", 0xA0000000u, 0, false, false, false);

    // Combinations still suppress.
    run_scenario("vision_plus_fade", coop::kEventVision, coop::kModeFade, false, false, false);
    run_scenario("talk_while_pause", coop::kEventTalk, 0, true, false, false);
    run_scenario("rt_with_modal", 0, coop::kModeRealtimeBusy, false, true, false);

    // PC_CTRL / unrelated high bit that is NOT suppress — 0x00002000 alone.
    run_scenario("pc_ctrl_alone", 0x00002000u, 0, false, false, true);

    std::printf("host presentation_gate: all scenarios passed\n");
    return 0;
}
