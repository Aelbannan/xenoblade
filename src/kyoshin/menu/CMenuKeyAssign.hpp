#pragma once

#include <types.h>

#include "kyoshin/IUICf.hpp"
#include "monolib/work/IWorkEvent.hpp"
#include "monolib/scn/IScnRender.hpp"
#include "monolib/lib/UnkClass_8045F564.hpp"

namespace nw4r { namespace lyt { class Layout; } }
class CScn;

// Key-assign (button mapping) menu controller (singleton).
class CMenuKeyAssign : public IUICf, public IWorkEvent, public IScnRender {
public:
    CMenuKeyAssign(CScn* scene);
    virtual ~CMenuKeyAssign();

    virtual void Init();
    virtual void Term();
    virtual void Move();
    virtual void cbRenderBefore();
    void Draw();

    static CMenuKeyAssign* create(CProcess* parent, CScn* scene);

    static CMenuKeyAssign* sInstance;

    /* 0x60 */ CScn* mScn;                 // owning scene (render-callback target)
    /* 0x64 */ UnkClass_8045F564 mUnkClass; // scratch region
    /* 0x74 */ nw4r::lyt::Layout* mLayout; // layout drawn in cbRenderBefore
    /* 0x78 */ u32 mField_78;              // open state (openKeyAssignPlayer: 3 = already open)

    void hideKeyAssignPanes();
    void setKeyAssignLabels(char* str, int idx);
    void openKeyAssignPlayer();
    void openKeyAssignColumns(int state, int count, int mode);
    void openKeyAssignButtons(int a, int b, int c, int d, int e, int mode);
};
