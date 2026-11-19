// CriWare cvFs -- device independent file system front end (Wii).
//
// cvFs keeps a table of registered devices; every device supplies an I/F
// table (CVFS_IF) of function pointers that cvFs dispatches the API calls to.

#include <harness_catalog.h>
#include <string.h>

extern void CRICRW_Strcpy(char *dst, s32 maxlen, const char *src);
extern s32 CRICRW_Sprintf(char *buf, s32 size, const char *fmt, ...);

#define CVFS_MAX_DEV 32
#define CVFS_MAX_HNDL 40
#define CVFS_DEVNAME_LEN 12
#define CVFS_NAME_MAX 0x129

#define CVFS_OP_CHKNAME 0x64
#define CVFS_OP_RETRY 0x190

typedef void (*CVFS_ERRFN)(void *arg, const char *msg, s32 level);

// Device I/F table.  A device registers a function returning a pointer to one
// of these (see cvFsAddDev); the entries cvFs itself dispatches are named.
typedef struct {
    void (*ExecServer)(void);                              // +0x00
    void (*EntryErrFunc)(void *fn, void *arg);             // +0x04
    s32 (*GetFileSize)(const char *fname);                 // +0x08
    void *unk0C;                                           // +0x0C
    void *(*Open)(const char *fname, s32 size, s32 mode);  // +0x10
    void (*Close)(void *work);                             // +0x14
    s32 (*Seek)(void *work, s32 offset, s32 origin);       // +0x18
    s32 (*Tell)(void *work);                               // +0x1C
    s32 (*ReqRd)(void *work, s32 size, void *buf);         // +0x20
    void *unk24;                                           // +0x24
    void (*StopTr)(void *work);                            // +0x28
    s32 (*GetStat)(void *work);                            // +0x2C
    s32 (*GetSctLen)(void *work);                          // +0x30
    void (*SetSctLen)(void *work, s32 sctlen);             // +0x34
    s32 (*GetNumTr)(void *work);                           // +0x38
    void *unk3C;                                           // +0x3C
    s32 (*IsExistFile)(const char *fname);                 // +0x40
    void *unk44[7];                                        // +0x44 .. +0x5C
    s32 (*OptFn1)(void *work, s32 op, void *p1, void *p2); // +0x60
    void *unk64;                                           // +0x64
} CVFS_IF;

// One registered device: the I/F table plus the (upper case) device name.
typedef struct {
    CVFS_IF *ifn;                  // +0x00
    char name[CVFS_DEVNAME_LEN];   // +0x04
} CVFS_DEV;

// An open file: the device I/F table plus the device's own work pointer.
typedef struct {
    CVFS_IF *ifn;  // +0x00
    void *work;    // +0x04
} CVFS_HNDL;

// Global work.  `errarg` is the user argument of the error callback (the
// callback pointer itself lives in cvfs_err_fn right before this object).
typedef struct {
    void *errarg;                     // +0x00
    void *unk04;                      // +0x04
    void *unk08;                      // +0x08
    CVFS_HNDL hndl[CVFS_MAX_HNDL];    // +0x0C
} CVFS_WORK;

static CVFS_ERRFN cvfs_err_fn;
static CVFS_WORK cvfs_work;
static CVFS_DEV cvfs_dev[CVFS_MAX_DEV];
static char cvfs_defdev[CVFS_DEVNAME_LEN];
static char cvfs_path[CVFS_NAME_MAX];

// Error message pool shared by the whole cvFs API (offsets below are the
// individual messages: #1/#2/#3 ... per entry point).
static const char cvfs_errmsg[] =
    "cvFsAddDev #1:illegal device name\0"
    "cvFsAddDev #2:illegal I/F func name\0"
    "cvFsAddDev #3:can not add device\0"
    "cvFsDelDev #1:illegal device name\0"
    "cvFsSetDefDev #1:illegal device name\0"
    "cvFsSetDefDev #2:unknown device name\0"
    "cvFsOpen #1:illegal file name\0"
    "cvFsOpen #3:can not allocate handle\0"
    "%s:%s\0"
    "cvFsOpen #2:illegal device name\0"
    "cvFsOpen #4:device not found\0"
    "cvFsOpen #5:vtbl error\0"
    "cvFsOpen #6:can not open file\0"
    "cvFsClose #1:handle error\0"
    "cvFsClose #2:vtbl error\0"
    "cvFsTell #1:handle error\0"
    "cvFsTell #2:vtbl error\0"
    "cvFsSeek #1:handle error\0"
    "cvFsSeek #2:vtbl error\0"
    "cvFsReqRd #1:handle error\0"
    "cvFsReqRd #2:vtbl error\0"
    "cvFsReqWr #1:handle error\0"
    "cvFsReqWr #2:vtbl error\0"
    "cvFsStopTr #1:handle error\0"
    "cvFsStopTr #2:vtbl error\0"
    "cvFsGetStat #1:handle error\0"
    "cvFsGetStat #2:vtbl error\0"
    "cvFsGetFileSize #1:illegal file name\0"
    "cvFsGetFileSize #2:illegal device name\0"
    "cvFsGetFileSize #3:device not found\0"
    "cvFsGetFileSize #4:vtbl error\0"
    "cvFsGetFileSizeEx #1:illegal file name\0"
    "cvFsGetFileSizeEx #2:illegal device name\0"
    "cvFsGetFileSizeEx #3:device not found\0"
    "cvFsGetFileSizeEx #4:vtbl error\0"
    "cvFsGetFileSizeByHndl #1:illegal file handle\0"
    "cvFsGetFileSizeByHndl #2:command not found\0"
    "cvFsIsFileOpened #1:illegal file handle\0"
    "cvFsGetFreeSize #5:device not found\0"
    "cvFsGetFreeSize #6:vtbl error\0"
    "cvFsGetSctLen #1:handle error\0"
    "cvFsGetSctLen #2:vtbl error\0"
    "cvFsSetSctLen #3:handle error\0"
    "cvFsSetSctLen #4:vtbl error\0"
    "cvFsGetNumTr #1:handle error\0"
    "cvFsGetNumTr #2:vtbl error\0"
    "cvFsChangeDir #1:illegal directory name\0"
    "cvFsChangeDir #2:illegal device name\0"
    "cvFsChangeDir #3:device not found\0"
    "cvFsChangeDir #4:vtbl error\0"
    "cvFsIsExistFile #1:illegal file name\0"
    "cvFsIsExistFile #2:illegal device name\0"
    "cvFsIsExistFile #3:device not found\0"
    "cvFsIsExistFile #4:vtbl error\0"
    "cvFsGetMaxByteRate #1:handle error\0"
    "cvFsGetMaxByteRate #2:vtbl error\0"
    "cvFsMakeDir #1:illegal directory name\0"
    "cvFsMakeDir #2:illegal device name\0"
    "cvFsMakeDir #3:device not found\0"
    "cvFsMakeDir #4:vtbl error\0"
    "cvFsRemoveDir #1:illegal directory name\0"
    "cvFsRemoveDir #2:illegal device name\0"
    "cvFsRemoveDir #3:device not found\0"
    "cvFsRemoveDir #4:vtbl error\0"
    "cvFsDeleteFile #1:illegal file name\0"
    "cvFsDeleteFile #2:illegal device name\0"
    "cvFsDeleteFile #3:device not found\0"
    "cvFsDeleteFile #4:vtbl error\0"
    "cvFsGetDevName #1:vtbl error\0"
    "cvFsOptFn1 #1:handle error\0"
    "cvFsOptFn1 #2:vtbl error\0"
    "cvFsOptFn2 #1:handle error\0"
    "cvFsOptFn2 #2:vtbl error\0"
    "cvFsSetCurVolume #1:illegal device name\0"
    "cvFsSetCurVolume #2:illegal image handle\0"
    "cvFsSetCurVolume #3:device not found\0"
    "cvFsAddVolumeEx #1:illegal device name\0"
    "cvFsAddVolumeEx #2:illegal volume name\0"
    "cvFsAddVolumeEx #3:illegal image handle\0"
    "cvFsAddVolumeEx #3:device not found\0"
    "cvFsDelVolume #1:illegal device name\0"
    "cvFsDelVolume #2:illegal volume name\0"
    "cvFsDelVolume #3:device not found\0"
    "cvFsGetVolumeInfo #1:illegal device name\0"
    "cvFsGetVolumeInfo #2:illegal volume name\0"
    "cvFsGetVolumeInfo #3:device not found\0"
    "cvFsSetDefVol #1:illegal device name\0"
    "cvFsSetDefVol #2:illegal volume name\0"
    "cvFsSetDefVol #3:device not found\0"
    "cvFsReqClose #1:illegal file handle\0"
    "cvFsIsAvailableRetry #1:illegal file handle\0";

#define CVFS_ERR(off)                                                        \
    do {                                                                     \
        if (cvfs_err_fn != NULL)                                             \
            cvfs_err_fn(cvfs_work.errarg, &cvfs_errmsg[off], 0);             \
    } while (0)

void getDevName(char *dev, char *name, const char *fname);

// Upper-case a device name in place (a trailing terminator included).
static inline void cvfs_upper(char *name) {
    u32 i;
    u32 len = strlen(name) + 1;
    for (i = 0; i < len; i++) {
        if ((u8)(name[i] - 'a') <= 25) name[i] -= 32;
    }
}

// Look a device up by (already upper-cased) name.
static inline CVFS_IF *cvfs_findDev(const char *name) {
    u32 i;
    s32 len = strlen(name);
    for (i = 0; i < CVFS_MAX_DEV; i++) {
        if (strncmp(name, cvfs_dev[i].name, len) == 0) return cvfs_dev[i].ifn;
    }
    return NULL;
}

// Is `name` the name of a registered device?
static inline s32 cvfs_isDev(const char *name, s32 len) {
    s32 i;
    for (i = 0; i < CVFS_MAX_DEV; i++) {
        if (strncmp(name, cvfs_dev[i].name, len) == 0) return 1;
    }
    return 0;
}

// Fall back to the default device name when the caller passed none.
static inline void cvfs_setDefDev(char *name) {
    s32 len = strlen(cvfs_defdev);
    if (cvfs_defdev[0] == 0) {
        name[0] = 0;
    } else {
        memcpy(name, cvfs_defdev, len + 1);
    }
}

// Resolve the device named in `dev` (substituting the default device name
// when it is empty) and, if the device asks for it, rewrite `name` to the
// fully qualified "device:file" form.
static inline CVFS_IF *cvfs_getIfn(char *dev, char *name, const char *fname) {
    CVFS_IF *ifn;
    char *devp;
    s32 ret;

    ifn = NULL;
    devp = dev;
    if (dev[0] == 0) {
        cvfs_setDefDev(dev);
        if (dev[0] == 0) return NULL;
    }
    if (devp == NULL) devp = cvfs_defdev;
    ifn = cvfs_findDev(devp);
    if (ifn == NULL) {
        ret = 0;
    } else if (ifn->OptFn1 != NULL) {
        ret = ifn->OptFn1(NULL, CVFS_OP_CHKNAME, NULL, NULL);
    } else {
        ret = 0;
    }
    if (ret == 1) {
        CRICRW_Strcpy(cvfs_path, CVFS_NAME_MAX, name);
        CRICRW_Sprintf(name, CVFS_NAME_MAX, &cvfs_errmsg[0x115], devp,
                       cvfs_path);
    }
    ifn = cvfs_findDev(dev);
    if (ifn == NULL) {
        cvfs_setDefDev(dev);
        ifn = cvfs_findDev(dev);
        if (ifn == NULL) return NULL;
        CRICRW_Strcpy(name, CVFS_NAME_MAX, fname);
    }
    return ifn;
}

void cvFsCallUsrErrFn(void) {
    if (cvfs_err_fn == NULL) return;
    ((void (*)(void *))cvfs_err_fn)(cvfs_work.errarg);
}

void cvFsAddDev(const char *name, CVFS_IF *(*getifn)(void)) {
    CVFS_IF *ifn;
    CVFS_IF *old;
    s32 i;

    if (name == NULL) {
        CVFS_ERR(0x0);
        return;
    }
    if (getifn == NULL) {
        CVFS_ERR(0x22);
        return;
    }
    cvfs_upper((char *)name);
    ifn = getifn();
    old = cvfs_findDev(name);
    if (old != NULL) {
    } else {
        for (i = 0; i < CVFS_MAX_DEV; i++) {
            if (cvfs_dev[i].name[0] == 0) break;
        }
        if (i == CVFS_MAX_DEV) {
            ifn = NULL;
        } else {
            cvfs_dev[i].ifn = ifn;
            memcpy(cvfs_dev[i].name, name, strlen(name) + 1);
        }
    }
    if (ifn == NULL) {
        CVFS_ERR(0x46);
        return;
    }
    if (ifn->EntryErrFunc != NULL) ifn->EntryErrFunc(cvFsCallUsrErrFn, NULL);
}

void cvFsSetDefDev(const char *name) {
    s32 len;

    if (name == NULL) {
        CVFS_ERR(0x89);
        return;
    }
    len = strlen(name);
    if (len == 0) {
        cvfs_defdev[0] = 0;
        return;
    }
    cvfs_upper((char *)name);
    if (cvfs_isDev(name, len) == 1) {
        memcpy(cvfs_defdev, name, len + 1);
    } else {
        CVFS_ERR(0xae);
    }
}

void *cvFsOpen(const char *fname, s32 size, s32 mode) {
    char dev[CVFS_NAME_MAX];
    char name[CVFS_NAME_MAX];
    s32 i;
    CVFS_HNDL *hndl;

    if (fname == NULL) {
        CVFS_ERR(0xd3);
        return NULL;
    }
    getDevName(dev, name, fname);
    if (name[0] == 0) {
        CVFS_ERR(0xd3);
        return NULL;
    }
    for (i = 0; i < CVFS_MAX_HNDL; i++) {
        if (cvfs_work.hndl[i].work == NULL) break;
    }
    hndl = &cvfs_work.hndl[i];
    if (i == CVFS_MAX_HNDL) hndl = NULL;
    if (hndl == NULL) {
        CVFS_ERR(0xf1);
        return NULL;
    }
    hndl->ifn = cvfs_getIfn(dev, name, fname);
    if (dev == NULL) {
        hndl->work = NULL;
        hndl->ifn = NULL;
        CVFS_ERR(0x11b);
        return NULL;
    }
    if (hndl->ifn == NULL) {
        hndl->work = NULL;
        hndl->ifn = NULL;
        CVFS_ERR(0x13b);
        return NULL;
    }
    if (hndl->ifn->Open != NULL) {
        hndl->work = hndl->ifn->Open(name, size, mode);
    } else {
        hndl->work = NULL;
        hndl->ifn = NULL;
        CVFS_ERR(0x158);
        return NULL;
    }
    if (hndl->work == NULL) {
        hndl->work = NULL;
        hndl->ifn = NULL;
        CVFS_ERR(0x16f);
        return NULL;
    }
    return hndl;
}

// Split "device:file" into its device and file parts.
void getDevName(char *dev, char *name, const char *fname) {
    s32 i;
    s32 j;

    if (fname == NULL) return;
    for (i = 0; i < CVFS_NAME_MAX; i++) {
        if (fname[i] == ':' || fname[i] == 0) break;
        dev[i] = fname[i];
    }
    if (fname[i] == 0) {
        dev[i] = 0;
        memcpy(name, dev, strlen(dev) + 1);
        dev[0] = 0;
        return;
    }
    dev[i] = 0;
    i++;
    if (i == 2) {
        i = 0;
        dev[0] = 0;
    }
    for (j = i; j < CVFS_NAME_MAX; j++) {
        if (fname[j] == 0) break;
        name[j - i] = fname[j];
    }
    name[j - i] = 0;
    cvfs_upper(dev);
}

void cvFsClose(void *hndl) {
    CVFS_HNDL *h = (CVFS_HNDL *)hndl;

    if (h == NULL) {
        CVFS_ERR(0x18d);
        return;
    }
    if (h->ifn->Close != NULL) {
        h->ifn->Close(h->work);
        h->work = NULL;
        h->ifn = NULL;
    } else {
        CVFS_ERR(0x1a7);
    }
}

s32 cvFsTell(void *hndl) {
    CVFS_HNDL *h = (CVFS_HNDL *)hndl;
    s32 ret;

    if (h == NULL) {
        CVFS_ERR(0x1bf);
        return 0;
    }
    if (h->ifn->Tell != NULL) {
        ret = h->ifn->Tell(h->work);
    } else {
        ret = 0;
        CVFS_ERR(0x1d8);
    }
    return ret;
}

s32 cvFsSeek(void *hndl, s32 offset, s32 origin) {
    CVFS_HNDL *h = (CVFS_HNDL *)hndl;
    s32 ret;

    if (h == NULL) {
        CVFS_ERR(0x1ef);
        return 0;
    }
    if (h->ifn->Seek != NULL) {
        ret = h->ifn->Seek(h->work, offset, origin);
    } else {
        ret = 0;
        CVFS_ERR(0x208);
    }
    return ret;
}

s32 cvFsReqRd(void *hndl, s32 size, void *buf) {
    CVFS_HNDL *h = (CVFS_HNDL *)hndl;
    s32 ret;

    if (h == NULL) {
        CVFS_ERR(0x21f);
        return 0;
    }
    if (h->ifn->ReqRd != NULL) {
        ret = h->ifn->ReqRd(h->work, size, buf);
    } else {
        ret = 0;
        CVFS_ERR(0x239);
    }
    return ret;
}

void cvFsStopTr(void *hndl) {
    CVFS_HNDL *h = (CVFS_HNDL *)hndl;

    if (h == NULL) {
        CVFS_ERR(0x283);
        return;
    }
    if (h->ifn->StopTr != NULL) {
        h->ifn->StopTr(h->work);
        return;
    }
    CVFS_ERR(0x29e);
}

void cvFsExecServer(void) {
    s32 i;

    for (i = 0; i < CVFS_MAX_DEV; i++) {
        if (cvfs_dev[i].ifn != NULL) {
            if (cvfs_dev[i].ifn->ExecServer != NULL)
                cvfs_dev[i].ifn->ExecServer();
        }
    }
}

s32 cvFsGetStat(void *hndl) {
    CVFS_HNDL *h = (CVFS_HNDL *)hndl;
    s32 ret = 3;

    if (h == NULL) {
        CVFS_ERR(0x2b7);
        return 3;
    }
    if (h->ifn->GetStat != NULL) {
        ret = h->ifn->GetStat(h->work);
    } else {
        CVFS_ERR(0x2d3);
    }
    return ret;
}

s32 cvFsGetFileSize(const char *fname) {
    char dev[CVFS_NAME_MAX];
    char name[CVFS_NAME_MAX];
    CVFS_IF *ifn;

    if (fname == NULL) {
        CVFS_ERR(0x2ed);
        return 0;
    }
    getDevName(dev, name, fname);
    if (name[0] == 0) {
        CVFS_ERR(0x2ed);
        return 0;
    }
    ifn = cvfs_getIfn(dev, name, fname);
    if (dev == NULL) {
        CVFS_ERR(0x312);
    }
    if (ifn == NULL) {
        CVFS_ERR(0x339);
    }
    if (ifn->GetFileSize == NULL) {
        CVFS_ERR(0x35d);
        return 0;
    }
    return ifn->GetFileSize(name);
}

void cvFsEntryErrFunc(CVFS_ERRFN fn, void *arg) {
    if (fn == NULL) {
        cvfs_err_fn = NULL;
        cvfs_work.errarg = NULL;
    } else {
        cvfs_err_fn = fn;
        cvfs_work.errarg = arg;
    }
}

s32 cvFsIsAvailableRetry(void *hndl) {
    CVFS_HNDL *h = (CVFS_HNDL *)hndl;

    if (h == NULL) {
        CVFS_ERR(0xb7d);
        return 0;
    }
    if (h->ifn->OptFn1 != NULL)
        return h->ifn->OptFn1(h->work, CVFS_OP_RETRY, NULL, NULL);
    return 1;
}
