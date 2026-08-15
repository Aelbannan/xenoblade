#include "monolib/device.hpp"
#include "monolib/core.hpp"

// Local vtable-positioned view of the read-DVD job used by wkStandbyLogout:
// retail dispatches cancel(CFileHandle*) at vtable[0xA8], but MWCC lays the
// real class's inherited chain one slot longer (its cancel lands at 0xAC), so
// the call goes through this dummy-shaped interface instead (same trick as
// CDeviceFileJobVtbl in CDeviceFileDvd.cpp). The dummies only position the
// slot; declaring cancel as a real virtual makes MWCC emit the r12 dispatch
// shape (lwz r12,0(r3); lwz r12,0xa8(r12); mtctr; bctrl).
class CDeviceFileJobReadDvdVtbl {
public:
    virtual ~CDeviceFileJobReadDvdVtbl();
    virtual void s00();
    virtual void s01();
    virtual void s02();
    virtual void s03();
    virtual void s04();
    virtual void s05();
    virtual void s06();
    virtual void s07();
    virtual void s08();
    virtual void s09();
    virtual void s10();
    virtual void s11();
    virtual void s12();
    virtual void s13();
    virtual void s14();
    virtual void s15();
    virtual void s16();
    virtual void s17();
    virtual void s18();
    virtual void s19();
    virtual void s20();
    virtual void s21();
    virtual void s22();
    virtual void s23();
    virtual void s24();
    virtual void s25();
    virtual void s26();
    virtual void s27();
    virtual void s28();
    virtual void s29();
    virtual void s30();
    virtual void s31();
    virtual void s32();
    virtual void s33();
    virtual void s34();
    virtual void s35();
    virtual void s36();
    virtual void s37();
    virtual bool cancel(const char* pFilename);
    virtual bool cancel(CFileHandle* pHandle);
};

CDeviceFileJobReadDvd::CDeviceFileJobReadDvd(const char* pName, CWorkThread* pParent):
CDeviceFileJob(pName, pParent){
    mType = THREAD_CDEVICEFILEJOBREADDVD;
    //Why not initialize mDvdFileInfo??
}

CDeviceFileJobReadDvd::~CDeviceFileJobReadDvd(){}

void CDeviceFileJobReadDvd::cancelCurrent(){
    if(!wkIsCurrent()){
        CDeviceFile::removeFileJob(this);
    }else if(CDeviceFile::func_8044E768()){
        CDeviceFileCri::func_8044FC38();
    }else{
        CDeviceFileDvd::cancelCurrent();
    }
}

bool CDeviceFileJobReadDvd::cancel(const char* pFilename){
    if(mHandle == nullptr) return false;
    //If the filename doesn't match the one in the handle, return early
    if(mHandle->mName != pFilename) return false;

    cancelCurrent();
    return true;
}

bool CDeviceFileJobReadDvd::cancel(CFileHandle* pHandle){
    if(mHandle == nullptr || mHandle != pHandle) return false;
    cancelCurrent();
    return true;
}

void CDeviceFileJobReadDvd::wkUpdate(){
    if(mHandle != nullptr && mHandle->unkInline2()){
        if(mHandle->getRsrc() != nullptr){
            CRsrc::entry(mHandle->getRsrc(), getFilename(), &mHandle->unk160,
            mHandle->getData(), mHandle->getLength(), false);
        }

        mHandle->call(CBM_1);
        CDeviceFile::removeFileJob(this);
    }
}

bool CDeviceFileJobReadDvd::wkStandbyLogin(){
    if(isNoEvent()){
        return CWorkThread::wkStandbyLogin();
    }else if(wkIsCurrent()){
        if(mHandle->checkExistRsrc(CBM_1)){
            CDeviceFile::removeFileJob(this);
        }

        return CWorkThread::wkStandbyLogin();
    }

    return false;
}

bool CDeviceFileJobReadDvd::wkStandbyLogout(){
    //Cancel the associated file handle if there is one
    if(mHandle != nullptr){
        reinterpret_cast<CDeviceFileJobReadDvdVtbl*>(this)->cancel(mHandle);
        return false;
    }

    return CWorkThread::wkStandbyLogout(); //Call base
}

void CDeviceFileJobReadDvd::callCBM3(){
    if(mHandle != nullptr){
        mHandle->call(CBM_3);
    }

    CDeviceFile::removeFileJob(this);
}
