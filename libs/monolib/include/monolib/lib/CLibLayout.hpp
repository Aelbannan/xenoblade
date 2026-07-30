#pragma once

#include <types.h>
#include "monolib/work/CWorkThread.hpp"
#include <nw4r/lyt/lyt_arcResourceAccessor.h>

class CLibLayout : public CWorkThread {
public:
    CLibLayout(const char* pName, CWorkThread* pParent);
    virtual ~CLibLayout();

    DECL_WORKTHREAD_CREATE(CLibLayout);

    static bool isInitialized();
    static CLibLayout* getInstance();
    static nw4r::lyt::ArcResourceAccessor* createArcResourceAccessor();
    void getAllocHandle();
    void createLayout();
    void createPicture();
    void createTextbox();
    void deleteTextboxOrPicture();
    void func_8045F438();
    void func_8045F4E4();

    virtual void wkUpdate() override;  //0x88
    virtual bool wkStandbyLogin();
    virtual bool wkStandbyLogout();

    //0x0: vtable
    //0x0-1c4: CWorkThread
    u32 unk1C4;                       // 0x1C4: unknown field
    u8 unk1C8[0x2C0 - 0x1C8];        // 0x1C8-2C0: unknown trailing data
};
