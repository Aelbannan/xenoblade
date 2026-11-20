#pragma once

// Defined in CMainMenu.cpp with C++ linkage (retail symbol CMainMenu_GetInstancePtr__Fv).
class CMainMenu;
CMainMenu* CMainMenu_GetInstancePtr();

#include <types.h>

#include "kyoshin/IUICf.hpp"
#include "kyoshin/CBaseCur.hpp"
#include "monolib/work/IWorkEvent.hpp"
#include "monolib/scn/IScnRender.hpp"
#include "monolib/scn/CScn.hpp"
#include "monolib/lib/UnkClass_8045F564.hpp"

class CFileHandle;
class CProcess;

namespace nw4r { namespace lyt {
class Layout;
class Pane;
class ArcResourceAccessor;
class AnimTransform;
class DrawInfo;
} }
namespace nw4r { namespace math { struct VEC3; } }

class CBaseCur;
class CSubCur;
class CEventFile;
class CFileHandle;

// The main cursor: a CBaseCur with its own layout and a "Param" pane that
// follows the cursor position.
class CMainCur : public CBaseCur {
public:
    CMainCur(nw4r::lyt::ArcResourceAccessor* arcResAcc = nullptr) : CBaseCur(arcResAcc) {}
    ~CMainCur();
    virtual void initLayout();
    virtual void setRootPaneTranslate(const nw4r::math::VEC3* pos);
};

class CMainMenu : public IUICf, public IWorkEvent, public IScnRender {
public:
    CMainMenu(CScn* scene);
    virtual ~CMainMenu();
    virtual void Init();
    virtual void Term();
    virtual void cbRenderBefore();
    virtual bool OnFileEvent(CEventFile* pEventFile);

    static CMainMenu* create(CProcess* parent, CScn* scene);
    static CMainMenu* sInstance;

    void moveWaitLoad();
    void moveMain();
    static int isAnyMenuOpen();

    UnkClass_8045F564 mMemRegion;              // 0x60
    CScn* field_0x70;                          // 0x70 - owning scene (from ctor param)
    CFileHandle* field_0x74;                   // 0x74 - file handle
    nw4r::lyt::ArcResourceAccessor* field_0x78; // 0x78 - arc resource accessor
    nw4r::lyt::Layout* field_0x7C;             // 0x7C - main layout
    nw4r::lyt::AnimTransform* field_0x80;      // 0x80 - anim0
    nw4r::lyt::AnimTransform* field_0x84;      // 0x84 - anim1
    nw4r::lyt::AnimTransform* field_0x88;      // 0x88 - anim2
    nw4r::lyt::AnimTransform* field_0x8C;      // 0x8C - anim3
    CMainCur mMainCur;                         // 0x90
    CSubCur mSubCur;                           // 0xA8
    s32 field_0xC0;                            // 0xC0 - cursor index
    s32 field_0xC4;                            // 0xC4 - sub-index
    u8 field_0xC8[8];                          // 0xC8-0xCF - cursor availability flags
    u8 field_0xD0;                             // 0xD0 - flag: hasSave
    u8 field_0xD1;
    u8 field_0xD2;
    u8 field_0xD3;
    u8 field_0xD4;
    u8 field_0xD5;
    u8 field_0xD6;
    u8 field_0xD7;
    u8 field_0xD8;
    u8 field_0xD9;
    u8 field_0xDA;
    u8 field_0xDB;
    u8 field_0xDC;
    s32 field_0xE0;                            // 0xE0 - state
};
