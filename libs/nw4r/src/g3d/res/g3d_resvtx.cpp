#include <nw4r/g3d.h>

namespace nw4r {
namespace g3d {

/******************************************************************************
 *
 * ResVtxPos
 *
 ******************************************************************************/
void ResVtxPos::SetArray() {
    if (IsValid()) {
        GXSetArray(GX_VA_POS, GetData(), ref().stride);
    }
}

void ResVtxPos::GetArray(const void** ppBase, u8* pStride) const {
    if (ppBase != NULL) {
        *ppBase = GetData();
    }

    if (pStride) {
        *pStride = ref().stride;
    }
}

void ResVtxPos::CopyTo(void* pDst) const {
    if (!IsValid()) {
        return;
    }

    u32 size = GetSize();
    detail::Copy32ByteBlocks(pDst, ptr(), size);
    DCStoreRange(pDst, size);
}

/******************************************************************************
 *
 * ResVtxNrm
 *
 ******************************************************************************/
void ResVtxNrm::SetArray() {
    if (IsValid()) {
        GXSetArray(GX_VA_NRM, GetData(), ref().stride);
    }
}

void ResVtxNrm::GetArray(const void** ppBase, u8* pStride) const {
    if (ppBase != NULL) {
        *ppBase = GetData();
    }

    if (pStride) {
        *pStride = ref().stride;
    }
}

void ResVtxNrm::CopyTo(void* pDst) const {
    if (!IsValid()) {
        return;
    }

    u32 size = GetSize();
    detail::Copy32ByteBlocks(pDst, ptr(), size);
    DCStoreRange(pDst, size);
}

/******************************************************************************
 *
 * ResVtxClr
 *
 ******************************************************************************/
void ResVtxClr::SetArray(GXAttr attr) {
    if (IsValid() && (attr == GX_VA_CLR0 || attr == GX_VA_CLR1)) {
        GXSetArray(attr, GetData(), ref().stride);
    }
}

void ResVtxClr::GetArray(const void** ppBase, u8* pStride) const {
    if (ppBase != NULL) {
        *ppBase = GetData();
    }

    if (pStride) {
        *pStride = ref().stride;
    }
}

void ResVtxClr::CopyTo(void* pDst) const {
    if (!IsValid()) {
        return;
    }

    u32 size = GetSize();
    detail::Copy32ByteBlocks(pDst, ptr(), size);
    DCStoreRange(pDst, size);
}

/******************************************************************************
 *
 * ResVtxTexCoord
 *
 ******************************************************************************/
void ResVtxTexCoord::GetArray(const void** ppBase, u8* pStride) const {
    if (ppBase != NULL) {
        *ppBase = GetData();
    }

    if (pStride) {
        *pStride = ref().stride;
    }
}

/******************************************************************************
 *
 * DCStore
 *
 ******************************************************************************/
void ResVtxPos::DCStore(bool sync) {
    void* pBase = &ref();
    u32 size = GetSize();

    if (sync) {
        DC::StoreRange(pBase, size);
    } else {
        DC::StoreRangeNoSync(pBase, size);
    }
}

void ResVtxNrm::DCStore(bool sync) {
    void* pBase = &ref();
    u32 size = GetSize();

    if (sync) {
        DC::StoreRange(pBase, size);
    } else {
        DC::StoreRangeNoSync(pBase, size);
    }
}

void ResVtxClr::DCStore(bool sync) {
    void* pBase = &ref();
    u32 size = GetSize();

    if (sync) {
        DC::StoreRange(pBase, size);
    } else {
        DC::StoreRangeNoSync(pBase, size);
    }
}

void ResVtxTexCoord::DCStore(bool sync) {
    void* pBase = &ref();
    u32 size = GetSize();

    if (sync) {
        DC::StoreRange(pBase, size);
    } else {
        DC::StoreRangeNoSync(pBase, size);
    }
}

/******************************************************************************
 *
 * ResVtxFurVec DCStore
 *
 ******************************************************************************/
void ResVtxFurVec::DCStore(bool sync) {
    void* pBase = &ref();
    u32 size = ref().size;

    if (sync) {
        DC::StoreRange(pBase, size);
    } else {
        DC::StoreRangeNoSync(pBase, size);
    }
}

/******************************************************************************
 *
 * ResVtxFurPos DCStore
 *
 ******************************************************************************/
void ResVtxFurPos::DCStore(bool sync) {
    void* pBase = &ref();
    u32 size = ref().size;

    if (sync) {
        DC::StoreRange(pBase, size);
    } else {
        DC::StoreRangeNoSync(pBase, size);
    }
}

/******************************************************************************
 *
 * ResVtxFurPos SetArray
 *
 ******************************************************************************/
void ResVtxFurPos::SetArray(unsigned short idx) {
    if (!IsValid()) {
        return;
    }

    ResVtxFurPosData& r = ref();

    // Two-step validation pattern matches retail codegen
    int index = idx;
    int nLayer = r.numLayer;

    int valid = 0;
    int hasArray = 0;
    if (r.toFurPosArray != 0 && index < nLayer) {
        hasArray = 1;
    }
    if (hasArray != 0) {
        if (index >= 0) {
            valid = 1;
        }
    }

    void* pData;
    if (valid != 0) {
        pData = (u8*)&r + r.toFurPosArray + idx * r.ofsLayer;
    } else {
        pData = NULL;
    }

    GXSetArray(GX_VA_POS, pData, r.stride);
}

} // namespace g3d
} // namespace nw4r
