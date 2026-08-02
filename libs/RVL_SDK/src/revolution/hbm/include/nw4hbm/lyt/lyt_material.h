#ifndef NW4HBM_LYT_MATERIAL_H
#define NW4HBM_LYT_MATERIAL_H
#include <nw4hbm/types_nw4hbm.h>

#include <cstring>
#include <new>

#include <nw4hbm/lyt/lyt_common.h>
#include <nw4hbm/lyt/lyt_texMap.h>
#include <nw4hbm/lyt/lyt_resources.h>
#include <nw4hbm/lyt/lyt_types.h>

#include <nw4hbm/ut.h>

#include <revolution/GX.h>

namespace nw4hbm {
namespace lyt {

// Forward declarations
class AnimTransform;
class AnimResource;
struct ResBlockSet;

namespace detail {

/******************************************************************************
 *
 * BitGXNums
 *
 ******************************************************************************/
struct BitGXNums {
    u32 texMap : 4;
    u32 texSRT : 4;
    u32 texCoordGen : 4;
    u32 indSRT : 2;
    u32 indStage : 3;
    u32 tevSwap : 1;
    u32 tevStage : 5;
    u32 chanCtrl : 1;
    u32 matCol : 1;
    u32 alpComp : 1;
    u32 blendMode : 1;
};

/******************************************************************************
 *
 * GX memory offsets (inline like retail nw4hbm)
 *
 ******************************************************************************/
inline u32 CalcOffsetTexSRTAry(const BitGXNums& rNums) {
    return rNums.texMap * sizeof(TexMap);
}

inline u32 CalcOffsetTexCoordGenAry(const BitGXNums& rNums) {
    return rNums.texSRT * sizeof(TexSRT) +
           CalcOffsetTexSRTAry(rNums);
}

inline u32 CalcOffsetChanCtrlAry(const BitGXNums& rNums) {
    return rNums.texCoordGen * sizeof(TexCoordGen) +
           CalcOffsetTexCoordGenAry(rNums);
}

inline u32 CalcOffsetMatColAry(const BitGXNums& rNums) {
    return rNums.chanCtrl * sizeof(ChanCtrl) +
           CalcOffsetChanCtrlAry(rNums);
}

inline u32 CalcOffsetTevSwapAry(const BitGXNums& rNums) {
    return rNums.matCol * sizeof(ut::Color) +
           CalcOffsetMatColAry(rNums);
}

inline u32 CalcOffsetGetAlphaCompare(const BitGXNums& rNums) {
    // tevSwap entries occupy 4 bytes each in the GX memory buffer
    return rNums.tevSwap * 4 + CalcOffsetTevSwapAry(rNums);
}

inline u32 CalcOffsetBlendMode(const BitGXNums& rNums) {
    return rNums.alpComp * sizeof(AlphaCompare) +
           CalcOffsetGetAlphaCompare(rNums);
}

inline u32 CalcOffsetIndirectStageAry(const BitGXNums& rNums) {
    return rNums.blendMode * sizeof(BlendMode) +
           CalcOffsetBlendMode(rNums);
}

inline u32 CalcOffsetIndTexSRTAry(const BitGXNums& rNums) {
    return rNums.indStage * sizeof(IndirectStage) +
           CalcOffsetIndirectStageAry(rNums);
}

inline u32 CalcOffsetTevStageAry(const BitGXNums& rNums) {
    return rNums.indSRT * sizeof(TexSRT) +
           CalcOffsetIndTexSRTAry(rNums);
}

} // namespace detail

namespace res {

/******************************************************************************
 *
 * MAT1 binary layout
 *
 ******************************************************************************/
struct Material {
    char name[NW4R_LYT_MATERIAL_NAME_LEN]; // at 0x0
    GXColorS10 tevCols[TEVCOLOR_MAX];      // at 0x14
    GXColor tevKCols[GX_MAX_KCOLOR];       // at 0x2C
    MaterialResourceNum resNum;            // at 0x3C
};

} // namespace res

/******************************************************************************
 *
 * Material
 *
 ******************************************************************************/
class Material {
public:
    Material();
    Material(const res::Material* pRes, const ResBlockSet& rBlockSet);
    virtual ~Material(); // at 0x8

    virtual bool SetupGX(bool modulate, u8 alpha); // at 0xC

    virtual void BindAnimation(AnimTransform* pAnimTrans);   // at 0x10
    virtual void UnbindAnimation(AnimTransform* pAnimTrans); // at 0x14
    virtual void UnbindAllAnimation();                       // at 0x18

    virtual void Animate(); // at 0x1C

    virtual AnimationLink*
    FindAnimationLink(AnimTransform* pAnimTrans); // at 0x20
    virtual void SetAnimationEnable(AnimTransform* pAnimTrans,
                                    bool enable); // at 0x24
    virtual AnimationLink*
    FindAnimationLink(const AnimResource& rResource); // at 0x28
    virtual void SetAnimationEnable(const AnimResource& rResource,
                                    bool enable); // at 0x2C

    void AddAnimationLink(AnimationLink* pAnimLink);

    GXColorS10 GetTevColor(u32 idx) const {
        return mTevCols[idx];
    }
    void SetTevColor(u32 idx, const GXColorS10& rColor) {
        mTevCols[idx] = rColor;
    }

    ut::Color GetTevKColor(u32 idx) const {
        return mTevKCols[idx];
    }
    void SetTevKColor(u32 idx, ut::Color color) {
        mTevKCols[idx] = color;
    }

    u8 GetTextureNum() const {
        return mGXMemNum.texMap;
    }
    u8 GetTextureCap() const {
        return mGXMemCap.texMap;
    }
    void SetTextureNum(u8 num);

    u8 GetTexSRTCap() const {
        return mGXMemCap.texSRT;
    }

    u8 GetTexCoordGenCap() const {
        return mGXMemCap.texCoordGen;
    }
    void SetTexCoordGenNum(u8 num);

    u8 GetIndTexSRTCap() const {
        return mGXMemCap.indSRT;
    }

    u8 GetTevStageNum() const {
        return mGXMemNum.tevStage;
    }
    void SetTevStageNum(u8 num) {
        if (num > 0) {
            TevStage* const pTevStage = GetTevStageAry();

            for (u32 i = mGXMemNum.tevStage; i < num; i++) {
                new (&pTevStage[i]) TevStage();
            }

            mGXMemNum.tevStage = num;
        }
    }

    void SetIndStageNum(u8 num) {
        if (num > 0) {
            IndirectStage* const pIndStage = GetIndirectStageAry();

            for (u32 i = mGXMemNum.indStage; i < num; i++) {
                new (&pIndStage[i]) IndirectStage();
            }

            mGXMemNum.indStage = num;
        }
    }

    void SetColorElement(u32 idx, s16 value);

    bool IsTevSwapCap() const {
        return mGXMemCap.tevSwap;
    }
    bool IsChanCtrlCap() const {
        return mGXMemCap.chanCtrl;
    }
    bool IsMatColorCap() const {
        return mGXMemCap.matCol;
    }
    bool IsAlphaCompareCap() const {
        return mGXMemCap.alpComp;
    }
    bool IsBlendModeCap() const {
        return mGXMemCap.blendMode;
    }

    const TexMap* GetTexMapAry() const {
        return detail::ConvertOffsToPtr<TexMap>(mpGXMem, 0);
    }
    TexMap* GetTexMapAry() {
        return detail::ConvertOffsToPtr<TexMap>(mpGXMem, 0);
    }

    const TexSRT* GetTexSRTAry() const {
        return detail::ConvertOffsToPtr<TexSRT>(mpGXMem,
                                                CalcOffsetTexSRTAry(mGXMemCap));
    }
    TexSRT* GetTexSRTAry();

    const TexCoordGen* GetTexCoordGenAry() const {
        return detail::ConvertOffsToPtr<TexCoordGen>(
            mpGXMem, CalcOffsetTexCoordGenAry(mGXMemCap));
    }
    TexCoordGen* GetTexCoordGenAry();

    const ChanCtrl* GetChanCtrlAry() const {
        return detail::ConvertOffsToPtr<ChanCtrl>(
            mpGXMem, CalcOffsetChanCtrlAry(mGXMemCap));
    }
    ChanCtrl* GetChanCtrlAry() {
        return detail::ConvertOffsToPtr<ChanCtrl>(
            mpGXMem, CalcOffsetChanCtrlAry(mGXMemCap));
    }

    const ut::Color* GetMatColAry() const {
        return detail::ConvertOffsToPtr<ut::Color>(
            mpGXMem, CalcOffsetMatColAry(mGXMemCap));
    }
    ut::Color* GetMatColAry() {
        return detail::ConvertOffsToPtr<ut::Color>(
            mpGXMem, CalcOffsetMatColAry(mGXMemCap));
    }

    const TevSwapMode* GetTevSwapAry() const {
        return detail::ConvertOffsToPtr<TevSwapMode>(
            mpGXMem, CalcOffsetTevSwapAry(mGXMemCap));
    }
    TevSwapMode* GetTevSwapAry() {
        return detail::ConvertOffsToPtr<TevSwapMode>(
            mpGXMem, CalcOffsetTevSwapAry(mGXMemCap));
    }

    const AlphaCompare* GetAlphaComparePtr() const {
        return detail::ConvertOffsToPtr<AlphaCompare>(
            mpGXMem, CalcOffsetGetAlphaCompare(mGXMemCap));
    }
    AlphaCompare* GetAlphaComparePtr() {
        return detail::ConvertOffsToPtr<AlphaCompare>(
            mpGXMem, CalcOffsetGetAlphaCompare(mGXMemCap));
    }

    const BlendMode* GetBlendModePtr() const {
        return detail::ConvertOffsToPtr<BlendMode>(
            mpGXMem, CalcOffsetBlendMode(mGXMemCap));
    }
    BlendMode* GetBlendModePtr() {
        return detail::ConvertOffsToPtr<BlendMode>(
            mpGXMem, CalcOffsetBlendMode(mGXMemCap));
    }

    const IndirectStage* GetIndirectStageAry() const {
        return detail::ConvertOffsToPtr<IndirectStage>(
            mpGXMem, CalcOffsetIndirectStageAry(mGXMemCap));
    }
    IndirectStage* GetIndirectStageAry() {
        return detail::ConvertOffsToPtr<IndirectStage>(
            mpGXMem, CalcOffsetIndirectStageAry(mGXMemCap));
    }

    const TexSRT* GetIndTexSRTAry() const {
        return detail::ConvertOffsToPtr<TexSRT>(mpGXMem,
                                                CalcOffsetIndTexSRTAry(mGXMemCap));
    }
    TexSRT* GetIndTexSRTAry();

    const TevStage* GetTevStageAry() const {
        return detail::ConvertOffsToPtr<TevStage>(mpGXMem,
                                                  CalcOffsetTevStageAry(mGXMemCap));
    }
    TevStage* GetTevStageAry() {
        return detail::ConvertOffsToPtr<TevStage>(mpGXMem,
                                                  CalcOffsetTevStageAry(mGXMemCap));
    }

    const TexMap& GetTexture(u8 idx) const {
        return GetTexMapAry()[idx];
    }
    void GetTexture(GXTexObj* pTexObj, u8 idx) const {
        GetTexMapAry()[idx].Get(pTexObj);
    }
    void SetTexture(u8 idx, const TexMap& rTexMap) {
        GetTexMapAry()[idx].Set(rTexMap);
    }
    void SetTextureNoWrap(u8 idx, const TexMap& rTexMap) {
        GetTexMapAry()[idx].SetNoWrap(rTexMap);
    }
    void SetTexture(u8 idx, const GXTexObj& rTexObj);
    void SetTextureNoWrap(u8 idx, TPLPalette* pTpl);

    const TexSRT& GetTexSRT(u32 idx) const {
        return GetTexSRTAry()[idx];
    }
    void SetTexSRT(const TexSRT& rTexSRT, u32 idx) {
        GetTexSRTAry()[idx] = rTexSRT;
    }

    void SetTexSRTElement(u32 srt, u32 idx, f32 value) {
        f32* const pArray = reinterpret_cast<f32*>(&GetTexSRTAry()[srt]);
        pArray[idx] = value;
    }

    void SetTexCoordGen(u32 idx, TexCoordGen gen) {
        GetTexCoordGenAry()[idx] = gen;
    }

    ut::Color GetMatColor() const {
        return GetMatColAry()[0];
    }

    void SetIndTexSRTElement(u32 srt, u32 idx, f32 value) {
        f32* const pArray = reinterpret_cast<f32*>(&GetIndTexSRTAry()[srt]);
        pArray[idx] = value;
    }

    void SetName(const char* pName) {
        std::strncpy(mName, pName, NW4R_LYT_MATERIAL_NAME_LEN);
    }
    const char* GetName() const {
        return mName;
    }

    bool IsUserAllocated() const {
        return mbUserAllocated;
    }

    void ReserveGXMem(u8 texMapNum, u8 texSrtNum, u8 texCoordGenNum,
                  u8 tevStageNum, bool allocTevSwap, u8 indStageNum,
                  u8 indSrtNum, bool allocChanCtrl, bool allocMatCol,
                  bool allocAlpComp, bool allocBlendMode);

protected:
    static const int MAX_TEX_SRT = (GX_TEXMTX9 - GX_TEXMTX0) / 3 + 1;
    static const int MAX_IND_SRT = (GX_ITM_2 - GX_ITM_0) + 1;

protected:
    char mName[NW4R_LYT_MATERIAL_NAME_LEN]; // at 0x04

    AnimationLinkList mAnimList; // at 0x18

    GXColorS10 mTevCols[TEVCOLOR_MAX];  // at 0x24
    ut::Color mTevKCols[GX_MAX_KCOLOR]; // at 0x3C

    detail::BitGXNums mGXMemCap; // at 0x4C
    detail::BitGXNums mGXMemNum; // at 0x50
    bool mbUserAllocated;        // at 0x54
    void* mpGXMem;               // at 0x58
};

/******************************************************************************
 *
 * Functions
 *
 ******************************************************************************/
namespace detail {

Size GetTextureSize(Material* pMaterial, u8 idx);

} // namespace detail
} // namespace lyt
} // namespace nw4hbm

#endif
