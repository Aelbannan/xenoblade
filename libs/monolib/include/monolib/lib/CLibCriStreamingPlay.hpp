#pragma once

#include "monolib/lib/CLibCri.hpp"

class CLibCriStreamingPlay; // forward decl: the C-linkage alias below predates the class body

// C-linkage thunk target / deleting dtor (retail symbol name — keep verbatim)
extern "C" CLibCriStreamingPlay* __dt__20CLibCriStreamingPlayFv(CLibCriStreamingPlay* self, int flag);

//size: 0x540 (estimated based on CLibCri + streaming fields)
class CLibCriStreamingPlay : public CLibCri {
public:
    CLibCriStreamingPlay(const char* pName, CWorkThread* pParent);
    // (no virtual dtor declaration: the deleting dtor __dt__20CLibCriStreamingPlayFv
    // is provided by CLibCriStreamingPlay.cpp as an extern "C" retail-symbol function;
    // declaring it here would collide with that definition under MWCC)

    // CDeviceVICb virtual overrides (thunks adjust this from CDeviceVICb subobject at 0x1c4)
    virtual void viBeforeDrawDone();
    virtual void viAfterDrawDone();
    virtual void viBeginFrame();

    // Playback control
    int func_8045B5AC(const char* filename, int param2, bool loopFlag, int afsId, int afsSubId, bool forceAhx);
    bool func_8045B970(int id);
    void func_8045BAB0(int id);
    void func_8045BBA0();
    void func_8045BC4C(int id, bool pause);
    int func_8045BE48(int id);

    // Volume/fade control
    float func_8045C67C(int id);
    void func_8045C700(int id, float volume);
    void func_8045C8B0(int id, float volume);
    void func_8045CA4C(int id, float volume, float fadeTime, int action);
    void func_8045CCFC(int id, float param2, float param3, float param4);

    // Cleanup/state
    void func_8045CF30();
    int func_8045CFDC(int param);
    bool func_8045D03C(int id);

    // Virtual thunks (FULL_MATCH - adjust this from CDeviceVICb subobject)
    void func_8045D140();
    void func_8045D148();

    // CWorkThread virtual overrides
    void wkUpdate();
    bool wkStandbyLogin();
    bool wkStandbyLogout();
    void OnPauseTrigger(bool paused);
};
