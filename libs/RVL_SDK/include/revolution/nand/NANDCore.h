#ifndef RVL_SDK_NAND_CORE_H
#define RVL_SDK_NAND_CORE_H
#include <types.h>

#include <revolution/NAND/nand.h>
#ifdef __cplusplus
extern "C" {
#endif

void nandRemoveTailToken(char* newp, const char* oldp);
void nandGetHeadToken(char* head, char* rest, const char* path);
void nandGetRelativeName(char* name, const char* path);
void nandConvertPath(char* abs, const char* dir, const char* rel);
BOOL nandIsPrivatePath(const char* path);
BOOL nandIsUnderPrivatePath(const char* path);
BOOL nandIsInitialized(void);
s32 nandConvertErrorCode(s32 result);
void nandGenerateAbsPath(char* abs, const char* rel);
s32 NANDInit(void);
s32 NANDGetHomeDir(char* out);
s32 nandChangeDir(const char* path, NANDCommandBlock* block, BOOL async,
                  BOOL priv);
void nandChangeDirCallback(s32 result, void* arg);
s32 NANDChangeDirAsync(const char* path, NANDAsyncCallback callback,
                       NANDCommandBlock* block);
void nandCallback(s32 result, void* arg);
s32 nandGetType(const char* path, u8* type, NANDCommandBlock* block, BOOL async,
                BOOL priv);
void nandGetTypeCallback(s32 result, void* arg);
BOOL nandOnShutdown(BOOL final, u32 event);
void nandShutdownCallback(s32 result, void* arg);
s32 NANDPrivateGetTypeAsync(const char* path, u8* type,
                            NANDAsyncCallback callback,
                            NANDCommandBlock* block);
const char* nandGetHomeDir(void);
void NANDInitBanner(NANDBanner* banner, u32 flags, const wchar_t* title,
                    const wchar_t* subtitle);

/* Absent from Xenoblade retail NANDCore.o; see NANDOpenClose.c extras. */
void nandGetParentDirectory(char* dir, const char* path);
s32 NANDGetCurrentDir(char* out);
s32 NANDGetType(const char* path, u8* type);

#ifdef __cplusplus
}
#endif
#endif
