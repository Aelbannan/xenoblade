#include <nw4r/g3d.h>

namespace nw4r {
namespace g3d {

void ResPltt::DCStore(bool sync) {
    void* pBase = &ref();
    u32 size = ref().header.size;

    if (sync) {
        DC::StoreRange(pBase, size);
    } else {
        DC::StoreRangeNoSync(pBase, size);
    }
}

bool ResTex::GetTexObjParam(void** ppTexData, u16* pWidth, u16* pHeight,
                            GXTexFmt* pFormat, f32* pMinLod, f32* pMaxLod,
                            GXBool* pMipMap) const {
    const ResTexData& r = ref();

    if (IsCIFmt()) {
        return false;
    }

    if (ppTexData != NULL) {
        *ppTexData = const_cast<void*>(GetTexData());
    }

    if (pWidth != NULL) {
        *pWidth = GetWidth();
    }

    if (pHeight != NULL) {
        *pHeight = GetHeight();
    }

    if (pFormat != NULL) {
        *pFormat = r.fmt;
    }

    if (pMinLod != NULL) {
        *pMinLod = r.min_lod;
    }

    if (pMaxLod != NULL) {
        *pMaxLod = r.max_lod;
    }

    if (pMipMap != NULL) {
        *pMipMap = r.mipmap_level > 1;
    }

    return true;
}

bool ResTex::GetTexObjCIParam(void** ppTexData, u16* pWidth, u16* pHeight,
                              GXCITexFmt* pFormatCI, f32* pMinLod, f32* pMaxLod,
                              GXBool* pMipMap) const {
    const ResTexData& r = ref();

    if (!IsCIFmt()) {
        return false;
    }

    if (ppTexData != NULL) {
        *ppTexData = const_cast<void*>(GetTexData());
    }

    if (pWidth != NULL) {
        *pWidth = GetWidth();
    }

    if (pHeight != NULL) {
        *pHeight = GetHeight();
    }

    if (pFormatCI != NULL) {
        *pFormatCI = r.cifmt;
    }

    if (pMinLod != NULL) {
        *pMinLod = r.min_lod;
    }

    if (pMaxLod != NULL) {
        *pMaxLod = r.max_lod;
    }

    if (pMipMap != NULL) {
        *pMipMap = r.mipmap_level > 1;
    }

    return true;
}

void ResTex::Init() {
    ResTexData& r = ref();
    DC::FlushRangeNoSync(&r, r.header.size);
}

} // namespace g3d
} // namespace nw4r

// LLM-HARNESS-BEGIN: us-803df8fc
extern "C" void CheckRevision__Q34nw4r3g3d7ResPlttCFv() {}
// LLM-HARNESS-END: us-803df8fc
// LLM-HARNESS-BEGIN: us-803df920
extern "C" void GetParent__Q34nw4r3g3d6ResTexFv() {}
// LLM-HARNESS-END: us-803df920
// LLM-HARNESS-BEGIN: us-803df940
bool GetTexObjParam__Q34nw4r3g3d6ResTexCFPPvPUsPUsP9_GXTexFmtPfPfPUc(const void* self, void** texData, unsigned short* width, unsigned short* height, int* format, float* minLod, float* maxLod, unsigned char* mipmap) { const unsigned char* data = *reinterpret_cast<const unsigned char* const*>(self); if ((*reinterpret_cast<const unsigned int*>(data + 0x18) & 1u) != 0u) return false; if (texData != 0) { unsigned int offset = *reinterpret_cast<const unsigned int*>(data + 0x10); *texData = offset != 0u ? const_cast<unsigned char*>(data) + offset : 0; } if (width != 0) *width = *reinterpret_cast<const unsigned short*>(data + 0x1c); if (height != 0) *height = *reinterpret_cast<const unsigned short*>(data + 0x1e); if (format != 0) *format = *reinterpret_cast<const int*>(data + 0x20); if (minLod != 0) *minLod = *reinterpret_cast<const float*>(data + 0x28); if (maxLod != 0) *maxLod = *reinterpret_cast<const float*>(data + 0x2c); if (mipmap != 0) *mipmap = static_cast<unsigned char>(*reinterpret_cast<const unsigned int*>(data + 0x24) == 1u); return true; }
// LLM-HARNESS-END: us-803df940
// LLM-HARNESS-BEGIN: us-803df9fc
bool GetTexObjCIParam__Q34nw4r3g3d6ResTexCFPPvPUsPUsP11_GXCITexFmtPfPfPUc(const void* self, void** texObj, unsigned short* width, unsigned short* height, void* format, float* min, float* max, unsigned char* mipmaps) { const unsigned char* data = (const unsigned char*)*(const void* const*)self; if ((*(const unsigned int*)(data + 0x18) & 1) == 0) return false; if (texObj) { unsigned int offset = *(const unsigned int*)(data + 0x10); *texObj = offset ? (void*)(data + offset) : 0; } if (width) *width = *(const unsigned short*)(data + 0x1c); if (height) *height = *(const unsigned short*)(data + 0x1e); if (format) *(unsigned int*)format = *(const unsigned int*)(data + 0x20); if (min) *min = *(const float*)(data + 0x28); if (max) *max = *(const float*)(data + 0x2c); if (mipmaps) *mipmaps = (unsigned char)(*(const unsigned int*)(data + 0x24) > 1); return true; }
// LLM-HARNESS-END: us-803df9fc
// LLM-HARNESS-BEGIN: us-803dfab8
extern "C" void CheckRevision__Q34nw4r3g3d6ResTexCFv() {}
// LLM-HARNESS-END: us-803dfab8
