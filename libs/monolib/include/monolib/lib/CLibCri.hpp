#pragma once

#include "monolib/device/CDeviceVICb.hpp"
#include "monolib/work/CWorkUtil.hpp"
#include "monolib/work/CWorkThread.hpp"
#include "monolib/util/CErrorWii.hpp"

//size: 0x1d0
// IErrorWii is an interface base, inherited privately: MWCC lists only public
// bases in the class's RTTI base list, and retail's list has no IErrorWii entry.
class CLibCri : public CWorkThread, public CDeviceVICb, IErrorWii {
public:
    CLibCri(const char* pName, CWorkThread* pParent);
    virtual ~CLibCri();

    // CWorkThread virtual overrides
    virtual void wkUpdate();
    virtual bool wkStandbyLogin();
    virtual bool wkStandbyLogout();

    // CDeviceVICb virtual override
    virtual void viBeforeDrawDone();

    // IErrorWii virtual override
    virtual void errorWiiCB();

    static CLibCri* spInstance;
    static CLibCri* getInstance();

    static void handleAxCallback();
    static int dispatchFilePlayback(const char* filename, u32 allocHandle, int fileHandle);
    void isStreamActive();
    void stopStream();
    static void stopAllStreams();
    void setStreamPause();
    void fadeStreamVolume();
    void getStreamVolume();
    void setStreamVolume();
    void setStreamVolumeScale();
    void getStreamPosition();
    void setStreamPanVolume();
    void calcStreamBufferSize();
    void isStreamPaused();
    void startMovie();
    void stopMovie();
    // Defined in CLibCri.cpp as the retail flat symbol (the retail body swaps
    // its two incoming register args, which a no-arg member cannot express).
    void setMoviePause();
    void clearMoviePause();
    void isMovieGlobalPaused();
    void isMoviePlaying();
    void renderMovie();
    void getMovieWorkSize();

    static inline CLibCri* create(const char* pName, CWorkThread* pParent){
        CLibCri* lib = new (CWorkThreadSystem::getWorkMem()) CLibCri(pName, pParent);
        CWorkUtil::entryWork(lib, pParent, false);
        return lib;
    }

    //0x0: vtable
    //0x0-1c4: CWorkThread
    //0x1c4-1c8: CDeviceVICb
    //0x1c8-1cc: IErrorWii
    u32 unk1CC; //0x1cc
};

// C-linkage imports (retail symbol names - keep linkage/signatures verbatim)
// CLibCriStreamingPlay factory (static, 8 args; member-flavoured retail name
// on a free function).
extern "C" int startStream__20CLibCriStreamingPlayFv(
    const char* filename,
    int allocHandle,
    int loopFlag,
    int afsId,
    int afsSubId,
    bool forceAhx
);
