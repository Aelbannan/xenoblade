#pragma once

#include <types.h>
#include <monolib/work/CProcess.hpp>
#include <monolib/scn/IScnRender.hpp>
#include "kyoshin/IUICf.hpp"
#include "kyoshin/CBgTex.hpp"
#include "kyoshin/CTitleAHelp.hpp"
#include "kyoshin/CKizunaTalkList.hpp"

class CPad;


/*
 * Kizuna (Friendship) Talk List screen process.
 *
 * Layout:
 *   0x00: IUICf base        -- CTTask task base + mField54/mField55 (0x58 bytes)
 *   0x58: IScnRender        -- render-callback interface (vptr only)
 *   0x5C: CProcess*         -- parent process reference
 *   0x60: CBgTex            -- background layout widget (0x20 bytes)
 *   0x80: CTitleAHelp       -- title/help bar (0x38 bytes)
 *   0xB8: CKizunaTalkList   -- core talk-list data/logic (0x1494 bytes)
 *  0x154C: u8               -- phase/state byte
 */
class CMenuKizunaTalkList : public IUICf, public IScnRender {
public:
    CMenuKizunaTalkList(CProcess* parent);
    virtual ~CMenuKizunaTalkList();

    virtual void Init();
    virtual void Term();
    virtual void Move();
    virtual void cbRenderBefore();

    static CMenuKizunaTalkList* create(CProcess* self, CProcess* parent);

    static CMenuKizunaTalkList* sInstance;

    CProcess* mParentRef;            // 0x5C
    CBgTex mBgTex;                   // 0x60
    CTitleAHelp mTitleAHelp;         // 0x80
    CKizunaTalkList mKizunaTalkList; // 0xB8
    u8 mState;                       // 0x154C
};
