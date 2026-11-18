#pragma once

#include <types.h>
#include "monolib/work/CTTask.hpp"

namespace nw4r {
namespace lyt {
class Layout;
}
} // namespace nw4r

/*
 * Shared base of every UI window (menus, system windows, talk windows).
 *
 * Layout (retail sizeof 0x6C):
 *   0x00-0x53  CTTask<IUIWindow> (CProcess header + the Move/Draw ptmf hooks
 *              at 0x3C / 0x48)
 *   0x54       mLayout      - the window's layout, built in Init
 *   0x58       mField58
 *   0x5C       mField5C     - timer/dispatch object (CUIWindowManager::Move)
 *   0x60       mField60     - timer word, initialised to -1
 *   0x64       mField64     - pending-removal flag (CUIWindowManager)
 *   0x65       mField65     - pending mark
 *   0x66       mField66     - removal mark
 *   0x67       mField67     - pending flag, initialised to 1
 *   0x68       mField68     - window id
 *
 * The vtable is __vt__9IUIWindow (9 words: the CProcess slots) and is owned by
 * the TU that defines the key function ~IUIWindow (retail: CQuestWindow.cpp,
 * next to the CTTask<IUIWindow> destructor/Move/Draw specialisations). The
 * constructor is defined inline here: retail has an out-of-line weak
 * __ct__IUIWindow copy in the CMenuUpdate unit (which derives from it) and an
 * inlined body in CQuestWindow's own constructor.
 *
 * Init/Term stay pure (inherited from CProcess), which is why the retail
 * IUIWindow vtable has null entries in slots 2 and 3.
 */
class IUIWindow : public CTTask<IUIWindow> {
public:
    IUIWindow()
        : mLayout(NULL), mField58(0), mField5C(0), mField60(-1), mField64(0), mField65(0),
          mField66(0), mField67(1), mField68(0) {}

#ifdef IUIWINDOW_INLINE_DTOR
    // Some TUs compile the window base dtor inline: their retail derived
    // destructor calls `__dt__8CProcessFv` directly, with the double null
    // check MWCC emits when the IUIWindow/CTTask<IUIWindow> dtor bodies are
    // inlined (CSysWinBuff/CSysWinSave/CSysWinScenarioLog/CSysWinSelect/
    // CSystemWindow/CTalkWindow/CSimpleEveTalkWin). The TUs that call the
    // out-of-line copy (CQuestWindow/CMenuUpdate/CMenuItemExchange, where
    // CQuestWindow.cpp owns the key function) leave this undefined.
    virtual ~IUIWindow() {}
#else
    virtual ~IUIWindow();
#endif

    nw4r::lyt::Layout* mLayout; // 0x54
    void* mField58;             // 0x58
    void* mField5C;             // 0x5C
    s32 mField60;               // 0x60
    u8 mField64;                // 0x64
    u8 mField65;                // 0x65
    u8 mField66;                // 0x66
    u8 mField67;                // 0x67
    u32 mField68;               // 0x68
}; // size: 0x6C
