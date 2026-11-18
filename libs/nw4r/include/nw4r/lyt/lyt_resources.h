#ifndef NW4R_LYT_RESOURCES_H
#define NW4R_LYT_RESOURCES_H
#include <nw4r/types_nw4r.h>

#include <nw4r/lyt/lyt_types.h>

namespace nw4r {
namespace lyt {

// Forward declarations
class ResourceAccessor;

namespace res {

/******************************************************************************
 *
 * BinaryFileHeader
 *
 ******************************************************************************/
struct BinaryFileHeader {
    char signature[4]; // at 0x0
    u16 byteOrder;     // at 0x4
    u16 version;       // at 0x6
    u32 fileSize;      // at 0x8
    u16 headerSize;    // at 0xC
    u16 dataBlocks;    // at 0xE
};

/******************************************************************************
 *
 * DataBlockHeader
 *
 ******************************************************************************/
struct DataBlockHeader {
    char kind[4]; // at 0x0
    u32 size;     // at 0x4
};

/******************************************************************************
 *
 * Texture
 *
 ******************************************************************************/
struct Texture {
    u32 nameStrOffset;         // at 0x0
    u8 type;                   // at 0x4
    u8 PADDING_0x5[0x8 - 0x5]; // at 0x5
};
/******************************************************************************
 *
 * TextureList
 *
 ******************************************************************************/
struct TextureList {
    res::DataBlockHeader blockHeader; // at 0x0
    u16 texNum;                       // at 0x8
    u8 PADDING_0xA[0xC - 0xA];        // at 0xA
};

/******************************************************************************
 *
 * Font
 *
 ******************************************************************************/
struct Font {
    u32 nameStrOffset;         // at 0x0
    u8 type;                   // at 0x4
    u8 PADDING_0x5[0x8 - 0x5]; // at 0x5
};
/******************************************************************************
 *
 * FontList
 *
 ******************************************************************************/
struct FontList {
    res::DataBlockHeader blockHeader; // at 0x0
    u16 fontNum;                      // at 0x8
    u8 PADDING_0xA[0xC - 0xA];        // at 0xA
};

/******************************************************************************
 *
 * MaterialList
 *
 ******************************************************************************/
struct MaterialList {
    res::DataBlockHeader blockHeader; // at 0x0
    u16 materialNum;                  // at 0x8
    u8 PADDING_0xA[0xC - 0xA];        // at 0xA
};

/******************************************************************************
 *
 * TexMap
 *
 ******************************************************************************/
struct TexMap {
    u16 texIdx; // at 0x0
    u8 wrapS;   // at 0x2
    u8 wrapT;   // at 0x3

    GXTexWrapMode GetWarpModeS() const;
    GXTexWrapMode GetWarpModeT() const;
    GXTexFilter GetMinFilter() const;
    GXTexFilter GetMagFilter() const;
};

/******************************************************************************
 *
 * MaterialResourceNum
 *
 ******************************************************************************/
struct MaterialResourceNum {
    u32 bits; // at 0x0

    u8 GetTexMapNum() const;
    u8 GetTexSRTNum() const;
    u8 GetTexCoordGenNum() const;
    bool HasTevSwapTable() const;
    u8 GetIndTexSRTNum() const;
    u8 GetIndTexStageNum() const;
    u8 GetTevStageNum() const;
    bool HasAlphaCompare() const;
    bool HasBlendMode() const;
    u8 GetChanCtrlNum() const;
    u8 GetMatColNum() const;
};

} // namespace res

/******************************************************************************
 *
 * ResBlockSet
 *
 ******************************************************************************/
struct ResBlockSet {
    const res::TextureList* pTextureList;   // at 0x0
    const res::FontList* pFontList;         // at 0x4
    const res::MaterialList* pMaterialList; // at 0x8
    ResourceAccessor* pResAccessor;         // at 0xC
};

} // namespace lyt
} // namespace nw4r

#endif
