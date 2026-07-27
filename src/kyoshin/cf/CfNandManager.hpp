#pragma once

#include "monolib/scn.hpp"
#include "monolib/work.hpp"

namespace cf{
    class CfNandManager : public CTTask<CfNandManager>, public IWorkEvent, public IScnRender{
    public:
        static u32 func_8024005C();

    private:
        //0x000-0x054 CTTask
        //0x054-0x058 IWorkEvent
        //0x058-0x05C IScnRender
        char unk05C[0x194 - 0x05C]; //0x05C
    virtual ~CfNandManager();
    void Term();
    void Move();
    void func_8023E4D4();
    void create();
    void cbRenderBefore();
    void Init();
    void OnFileEvent();
    void func_802405FC();
    void func_80240604();
    void func_8024060C();
    }; //size = 0x194
} //namespace cf
