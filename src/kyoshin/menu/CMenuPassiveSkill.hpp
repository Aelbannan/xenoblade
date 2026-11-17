#pragma once

#include <types.h>

#include "kyoshin/IUICf.hpp"
#include "monolib/scn/IScnRender.hpp"
#include "kyoshin/CBgTex.hpp"
#include "kyoshin/CTitleAHelp.hpp"
#include <nw4r/lyt.h>

class CScn;
class CMenuPassiveSkill;


/*
 * Passive skill menu screen. IUICf is the common menu task base (0x00..0x57);
 * the IScnRender subobject sits at 0x58 and is registered with the scene.
 */
class CMenuPassiveSkill : public IUICf, public IScnRender {
public:
    CMenuPassiveSkill(CScn* scene);
    virtual ~CMenuPassiveSkill();

    virtual void Init();
    virtual void Term();
    virtual void Move();
    virtual void cbRenderBefore();

    static CMenuPassiveSkill* create(CProcess* parent, CScn* scene);
    static CMenuPassiveSkill* sInstance;

    /* 0x5C */ CScn* mScnRef;            // owning scene (addRenderCB / removeRenderCB target)
    /* 0x60 */ CBgTex mBgTex;
    /* 0x80 */ CTitleAHelp mTitleAHelp;
    /* UI::CPassiveSkill sub-object at 0xB8 (opaque here; its full layout lives
     * in kyoshin/CPassiveSkill.hpp whose tail padding would swallow 0x2AC). */
    u8 mPassiveSkill[0x1F4];  // 0xB8..0x2AB
    u8 field_2AC;             // 0x2AC - widgets-ready flag set by pollPassiveSkillReady
    u8 _pad2AD[3];            // 0x2AD..0x2AF
    f32 field_2B0;            // 0x2B0 - idle timer, seeded from lbl_eu_806688F0
};
