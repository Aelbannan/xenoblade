#pragma once
#include <types.h>
#include "monolib/work/IWorkEvent.hpp"
struct CEventFile;
namespace LOD {
// Retail RTTI identifies this 0x78-byte callback object at owner +0xA44.
class LODMemMan : public IWorkEvent {
public:
    LODMemMan() {}
    virtual ~LODMemMan();
    virtual bool OnFileEvent(CEventFile* evt);
    void initElementBuffers();
    void driveArchiveLoadState();
    u8* getOrCreatePoolData(int id);
    static void freePersistentBuffer();
    void resetManagerState();
    void cancelFileAndRelease();
    void clearFlagAndResetName();
    void delegateMergeToSub();
    void setArchiveReadyFlag();
    void mergeElementList();
private:
    u8 mState[0x74];
};
}
