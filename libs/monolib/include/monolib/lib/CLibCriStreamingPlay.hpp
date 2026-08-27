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
    bool isStreamActive(int id);
    void stopStream(int id);
    void stopAllStreams();
    void setStreamPause(int id, bool pause);
    int getStreamPosition(int id);

    // Volume/fade control
    float getStreamVolume(int id);
    void setStreamVolume(int id, float volume);
    void setStreamVolumeScale(int id, float volume);
    void fadeStreamVolume(int id, float volume, float fadeTime, int action);
    void setStreamPanVolume(int id, float param2, float param3, float param4);

    // Cleanup/state
    void clearAllStreams();
    int calcStreamBufferSize(int param);
    bool isStreamPaused(int id);

    // Virtual thunks (FULL_MATCH - adjust this from CDeviceVICb subobject)
    void onViBeginFrame();
    void onViDestroy();

    // CWorkThread virtual overrides
    void wkUpdate();
    bool wkStandbyLogin();
    bool wkStandbyLogout();
    void OnPauseTrigger(bool paused);
};
