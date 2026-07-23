#ifndef RVL_SDK_GX_INTERNAL_H
#define RVL_SDK_GX_INTERNAL_H
#include <types.h>

#include <revolution/GX/GXTypes.h>
#ifdef __cplusplus
extern "C" {
#endif

/**
 * GX internal structures.
 *
 * Wouldn't be necessary if the public ones didn't include padding; but they do,
 * so there has to be different structure definitions.
 *
 * These internal structures are implemented like the RFL ones since we don't
 * have DWARF info for most GX structures.
 */

/**
 * Declare a public structure from the corresponding internal structure.
 * (Implementation size is included to require that such a structure already
 * exists.)
 */
#define GX_PUBLIC_STRUCT_DECL(name, size)                                      \
    typedef struct _##name {                                                   \
        u32 dummy[((size) - sizeof(name##Impl) + sizeof(name##Impl)) /         \
                  sizeof(u32)];                                                \
    } name;

typedef struct _GXFifoObjImpl {
    void* base;        // at 0x0
    void* end;         // at 0x4
    u32 size;          // at 0x8
    void* hiWatermark; // at 0xC
    void* loWatermark; // at 0x10
    void* readPtr;     // at 0x14
    void* writePtr;    // at 0x18
    u32 count;         // at 0x1C
    u8 wrap;           // at 0x20
    u8 bind_cpu;       // at 0x21
    u8 bind_gp;        // at 0x22
    u8 pad;            // at 0x23
} GXFifoObjImpl;

typedef struct _GXLightObjImpl {
    char UNK_0x0[0xC];
    GXColor color; // at 0xC
    f32 aa;        // at 0x10
    f32 ab;        // at 0x14
    f32 ac;        // at 0x18
    f32 ka;        // at 0x1C
    f32 kb;        // at 0x20
    f32 kc;        // at 0x24
    f32 posX;      // at 0x28
    f32 posY;      // at 0x2C
    f32 posZ;      // at 0x30
    f32 dirX;      // at 0x34
    f32 dirY;      // at 0x38
    f32 dirZ;      // at 0x3C
} GXLightObjImpl;

typedef struct _GXTexObjImpl {
    u32 mode0;
    u32 mode1;
    u32 image0;
    u32 image3;
    void* userData;
    GXTexFmt fmt;
    u32 tlutName;
    u16 loadCnt;
    u8 loadFmt;
    u8 flags;
} GXTexObjImpl;

typedef struct _GXTlutObjImpl {
    u32 tlut;
    u32 loadTlut0;
    u16 numEntries;
} GXTlutObjImpl;

typedef struct _GXTexRegionImpl {
    u32 image1;
    u32 image2;
    u16 sizeEven;
    u16 sizeOdd;
    u8 is32bMipmap;
    u8 isCached;
} GXTexRegionImpl;

typedef struct _GXTlutRegionImpl {
    u32 loadTlut1;
    GXTlutObjImpl tlutObj;
} GXTlutRegionImpl;

#define GX_SETUP_TEXOBJ(l, p) GXTexObjImpl* l = (GXTexObjImpl*)(p);

#define GX_SETUP_ALL_TEXOBJS(l, p, m, q) \
    GXTexObjImpl* l = (GXTexObjImpl*)(p); \
    GXTexRegionImpl* m = (GXTexRegionImpl*)(q);

#define GX_SETUP_TLUTOBJ(l, p) GXTlutObjImpl* l = (GXTlutObjImpl*)(p);

#define GX_SETUP_TREGOBJ(l, p) GXTexRegionImpl* l = (GXTexRegionImpl*)(p);

#define GX_SETUP_TLUTREGOBJ(l, p) GXTlutRegionImpl* l = (GXTlutRegionImpl*)(p);

#ifdef __cplusplus
}
#endif
#endif
