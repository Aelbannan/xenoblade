#include <nw4r/ut.h>

namespace nw4r {
namespace ut {
namespace {

template <typename T> inline void ResolveOffset(T*& rpPtr, void* pBase) {
    char* base = static_cast<char*>(static_cast<void*>(pBase));
    s32 offset = reinterpret_cast<s32>(rpPtr);
    rpPtr = reinterpret_cast<T*>(base + offset);
}

} // namespace

ResFont::ResFont() {
    // Retail ctor stores the vtable pointer after the base-ctor call; with
    // novtable the store is explicit (lbl_eu_8056AF90 = ResFont vtable).
    *(void**)this = (void*)lbl_eu_8056AF90;
}

ResFont::~ResFont() {}

bool ResFont::SetResource(void* pBuffer) {
    BinaryFileHeader* pHeader = static_cast<BinaryFileHeader*>(pBuffer);
    FontInformation* pInfo = NULL;

    if (!IsManaging(NULL)) {
        return false;
    }

    if (pHeader->signature == SIGNATURE_UNPACKED) {
        u8* pCur = static_cast<u8*>(static_cast<void*>(pHeader)) + pHeader->headerSize;
        BinaryBlockHeader* pBlock =
            static_cast<BinaryBlockHeader*>(static_cast<void*>(pCur));

        for (int i = 0; i < pHeader->dataBlocks; i++) {
            if (pBlock->kind == SIGNATURE_FONTINFO) {
                pInfo = reinterpret_cast<FontInformation*>(pBlock + 1);
                break;
            }

            pCur = static_cast<u8*>(static_cast<void*>(pBlock)) + pBlock->size;
            pBlock = static_cast<BinaryBlockHeader*>(static_cast<void*>(pCur));
        }
    } else {
        if (pHeader->version == NW4R_VERSION(1, 4)) {
            if (!IsValidBinaryFile(pHeader, SIGNATURE, NW4R_VERSION(1, 4), 2)) {
                return false;
            }
        } else if (!IsValidBinaryFile(pHeader, SIGNATURE, NW4R_VERSION(1, 2),
                                      2)) {
            return false;
        }

        pInfo = Rebuild(pHeader);
    }

    if (pInfo == NULL) {
        return false;
    }

    SetResourceBuffer(pHeader, pInfo);
    InitReaderFunc(GetEncoding());

    return true;
}

FontInformation* ResFont::Rebuild(BinaryFileHeader* pHeader) {
    u8* pCur = static_cast<u8*>(static_cast<void*>(pHeader)) + pHeader->headerSize;
    BinaryBlockHeader* pBlock =
        static_cast<BinaryBlockHeader*>(static_cast<void*>(pCur));

    FontInformation* pInfo = NULL;

    for (int i = 0; i < pHeader->dataBlocks; i++) {
        switch (pBlock->kind) {
        case SIGNATURE_FONTINFO: {
            pInfo = reinterpret_cast<FontInformation*>(pBlock + 1);
            ResolveOffset<FontTextureGlyph>(pInfo->pGlyph, pHeader);

            if (pInfo->pWidth != 0) {
                ResolveOffset<FontWidth>(pInfo->pWidth, pHeader);
            }

            if (pInfo->pMap != 0) {
                ResolveOffset<FontCodeMap>(pInfo->pMap, pHeader);
            }
            break;
        }

        case SIGNATURE_TEXGLYPH: {
            ResolveOffset<u8>(
                reinterpret_cast<FontTextureGlyph*>(pBlock + 1)->sheetImage,
                pHeader);
            break;
        }

        case SIGNATURE_CHARWIDTH: {
            FontWidth* pWidth = reinterpret_cast<FontWidth*>(pBlock + 1);
            if (pWidth->pNext != 0) {
                ResolveOffset<FontWidth>(pWidth->pNext, pHeader);
            }
            break;
        }

        case SIGNATURE_CHARMAP: {
            FontCodeMap* pMap = reinterpret_cast<FontCodeMap*>(pBlock + 1);
            if (pMap->pNext != 0) {
                ResolveOffset<FontCodeMap>(pMap->pNext, pHeader);
            }
            break;
        }

        case SIGNATURE_GLGR: {
            break;
        }

        default: {
            return NULL;
        }
        }

        pCur = static_cast<u8*>(static_cast<void*>(pBlock)) + pBlock->size;
        pBlock = static_cast<BinaryBlockHeader*>(static_cast<void*>(pCur));
    }

    pHeader->signature = SIGNATURE_UNPACKED;
    return pInfo;
}

} // namespace ut
} // namespace nw4r
