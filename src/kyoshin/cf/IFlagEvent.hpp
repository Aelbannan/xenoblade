#pragma once

namespace cf{

    // Retail secondary IFlagEvent tables (CTaskREvent +0x58, CUIWindowManager
    // +0x54, CUICfManager +0x58, CHelp_LandMark +0x0C) carry exactly two
    // slots: the dtor and one flag-event handler. The dtor is inline and
    // trivial (no derived dtor calls it; mwld keeps one weak copy, placed in
    // CUICfManager).
    class IFlagEvent{
    public:
        virtual ~IFlagEvent() {}

        virtual void FlagEvent1(int arg1, int arg2, int arg3);
    };

} //namespace cf
