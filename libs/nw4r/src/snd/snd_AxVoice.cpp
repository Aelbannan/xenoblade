// Retail mangles the AxVoice ADPCM API against nw4r::snd::AdpcmParam
// (Q34nw4r3snd10AdpcmParam); the read-only headers resolve their unqualified
// "AdpcmParam" to the detail-scope struct instead. Include the types header
// first, define the snd-scope struct, then temporarily remap the identifier
// so the remaining headers' declarations pick up the retail type.
#include <nw4r/types_nw4r.h>
#include <nw4r/snd/snd_Types.h>
namespace nw4r {
namespace snd {
struct AdpcmParam {
    u16 coef[16];   // at 0x0
    u16 gain;       // at 0x20
    u16 pred_scale; // at 0x22
    u16 yn1;        // at 0x24
    u16 yn2;        // at 0x26
};
} // namespace snd
} // namespace nw4r
#define AdpcmParam snd::AdpcmParam
#include <nw4r/snd.h>
#include <nw4r/ut.h>
#undef AdpcmParam

#include <cstring>

// Retail .sdata2 float-pool entries shared by the AxVoice src helpers
// (values confirmed against port/data_defs.cpp). Global-scope variable
// declarations are not mangled by MWCC; referencing the labels keeps the
// sda21 relocs pointed at the shared pool entries instead of TU-local
// duplicates.
extern const f32 lbl_eu_80669E90; // 32000.0f (AX_SAMPLE_RATE)
extern const f32 lbl_eu_80669EA8; // 65535.0f
extern const f32 lbl_eu_80669EAC; // 0.0f
extern const f32 lbl_eu_80669EB0; // 65536.0f

// Retail per-channel biquad coefficient provider objects. Indexed by the
// raw biquad selector; NULL entries mean "no provider -> clear the filter".
extern void* lbl_eu_80637C20[];

// AxVoice::IsNeedNextUpdate is not declared in snd_AxVoice.h (header is
// read-only this session); the retail symbol is referenced verbatim so
// SetMix emits the `bl` with the correct reloc.
extern "C" bool IsNeedNextUpdate__Q44nw4r3snd6detail7AxVoiceCFRCQ54nw4r3snd6detail7AxVoice8MixParam(
    const nw4r::snd::detail::AxVoice* pThis,
    const nw4r::snd::detail::AxVoice::MixParam& rParam);

namespace nw4r {
namespace snd {
namespace detail {

AxVoice::AxVoice()
    : mWaveData(NULL),
      mFirstMixUpdateFlag(false),
      mReserveForFreeFlag(false),
      mCallback(NULL),
      mCallbackData(NULL) {}

AxVoice::~AxVoice() {}

void AxVoice::Setup(const void* pWave, Format fmt, int rate) {
    // Retail inlines the interrupt guard here as plain
    // OSDisableInterrupts/OSRestoreInterrupts calls.
    u32 enabled = OSDisableInterrupts();

    mWaveData = pWave;

    std::memset(&mMixPrev, 0, sizeof(MixParam));

    mFormat = fmt;
    mSampleRate = rate;
    mFirstMixUpdateFlag = true;

    OSRestoreInterrupts(enabled);
}

bool AxVoice::IsPlayFinished() const {
    ut::AutoInterruptLock lock;

    if (mWaveData == NULL) {
        return false;
    }

    u32 dspAddr = GetCurrentPlayingDspAddress();
    const void* pBuffer = AxManager::GetInstance().GetZeroBufferAddress();
    u32 samples = GetDspAddressBySample(pBuffer, 0, mFormat);

    u32 addr = samples;

    // NOTE: the header's legacy Format placeholders are unused here; retail
    // switches on the raw SampleFormat codes (1 = PCM16, 2 = PCM8, 3 = ADPCM).
    switch (mFormat) {
    case SAMPLE_FORMAT_DSP_ADPCM: {
        addr += 0x200;
        break;
    }

    case SAMPLE_FORMAT_PCM_S8: {
        addr += 0x100;
        break;
    }

    case SAMPLE_FORMAT_PCM_S16: {
        addr += 0x80;
        break;
    }

    default:
        return false;
    }

    if (samples <= dspAddr && dspAddr < addr) {
        return true;
    }

    return false;
}

void AxVoice::SetLoopStart(const void* pBase, u32 samples) {
    ut::AutoInterruptLock lock;

    if (!mVpb.IsAvailable()) {
        return;
    }

    u32 addr = GetDspAddressBySample(pBase, samples, mFormat);
    mVpb.SetVoiceLoopAddr(addr);
}

void AxVoice::SetLoopEnd(const void* pBase, u32 samples) {
    ut::AutoInterruptLock lock;

    if (!mVpb.IsAvailable()) {
        return;
    }

    u32 addr = GetDspAddressBySample(pBase, samples - 1, mFormat);
    mVpb.SetVoiceEndAddr(addr);
}

void AxVoice::SetLoopFlag(bool loop) {
    ut::AutoInterruptLock lock;

    if (!mVpb.IsAvailable()) {
        return;
    }

    mVpb.SetVoiceLoop(loop);
}

void AxVoice::StopAtPoint(const void* pBase, u32 samples) {
    ut::AutoInterruptLock lock;

    if (!mVpb.IsAvailable()) {
        return;
    }

    const void* pBuffer = AxManager::GetInstance().GetZeroBufferAddress();
    u32 begin = GetDspAddressBySample(pBuffer, 0, mFormat);
    u32 end = GetDspAddressBySample(pBase, samples - 1, mFormat);

    mVpb.SetVoiceLoopAddr(begin);
    mVpb.SetVoiceEndAddr(end);
    mVpb.SetVoiceLoop(false);
}

bool AxVoice::IsDataAddressCoverd(const void* pBegin, const void* pEnd) const {
    ut::AutoInterruptLock lock;

    if (!mVpb.IsAvailable()) {
        return false;
    }

    return mWaveData != NULL && (pBegin <= mWaveData && mWaveData <= pEnd);
}

u32 AxVoice::GetCurrentPlayingSample() const {
    ut::AutoInterruptLock lock;

    if (!mVpb.IsAvailable()) {
        return 0;
    }

    if (mWaveData == NULL) {
        return 0;
    }

    if (IsPlayFinished()) {
        u32 end = GetLoopEndDspAddress();
        u32 samples = GetSampleByDspAddress(mWaveData, end, mFormat);
        return samples + 1;
    }

    u32 now = GetCurrentPlayingDspAddress();
    u32 samples = GetSampleByDspAddress(mWaveData, now, mFormat);
    return samples;
}

u32 AxVoice::GetCurrentPlayingDspAddress() const {
    return mVpb.GetCurrentAddress();
}

u32 AxVoice::GetLoopEndDspAddress() const {
    return mVpb.GetEndAddress();
}

void AxVoice::VoiceCallback(void* pArg) {
    ut::AutoInterruptLock lock;

    AXVPB* pVpb = static_cast<AXVPB*>(pArg);
    AxVoice* p = reinterpret_cast<AxVoice*>(pVpb->userContext);

    p->mVpb.Clear();
    AxVoiceManager::GetInstance().ReserveForFreeAxVoice(p);
}

u32 AxVoice::GetDspAddressBySample(const void* pBase, u32 samples, Format fmt) {
    // Fresh-temp form: keeps the cached pointer live across repeated inlined
    // expansions of this helper (retail copies then conditionally overwrites).
    const void* pPhys = pBase;
    if (pBase != NULL) {
        pPhys = OSCachedToPhysical(pBase);
    }

    u32 addr = 0;

    switch (fmt) {
    case SAMPLE_FORMAT_DSP_ADPCM: {
        // clang-format off
        addr = (samples / AX_ADPCM_SAMPLES_PER_FRAME * AX_ADPCM_NIBBLES_PER_FRAME) +
               (samples % AX_ADPCM_SAMPLES_PER_FRAME) +
               (reinterpret_cast<u32>(pPhys) * sizeof(u16)) +
               sizeof(u16);
        // clang-format on
        break;
    }

    case SAMPLE_FORMAT_PCM_S8: {
        addr = reinterpret_cast<u32>(pPhys) + samples;

        break;
    }
    case SAMPLE_FORMAT_PCM_S16: {
        addr = reinterpret_cast<u32>(pPhys) / sizeof(u16) + samples;
        break;
    }
    }

    return addr;
}

u32 AxVoice::GetSampleByDspAddress(const void* pBase, u32 addr, Format fmt) {
    if (pBase != NULL) {
        pBase = OSCachedToPhysical(pBase);
    }

    u32 samples = 0;

    switch (fmt) {
    case SAMPLE_FORMAT_DSP_ADPCM: {
        samples = addr - reinterpret_cast<u32>(pBase) * sizeof(u16);
        // clang-format off
        samples = (samples % AX_ADPCM_NIBBLES_PER_FRAME) +
                  (samples / AX_ADPCM_NIBBLES_PER_FRAME * AX_ADPCM_SAMPLES_PER_FRAME) -
                  sizeof(u16);
        // clang-format on
        break;
    }

    case SAMPLE_FORMAT_PCM_S8: {
        samples = addr - reinterpret_cast<u32>(pBase);
        break;
    }

    case SAMPLE_FORMAT_PCM_S16: {
        samples = addr - reinterpret_cast<u32>(pBase) / sizeof(u16);
        break;
    }
    }

    return samples;
}

u32 AxVoice::GetSampleByByte(u32 addr, Format fmt) {
    u32 samples = 0;
    u32 frac;

    switch (fmt) {
    case SAMPLE_FORMAT_DSP_ADPCM: {
        samples = addr / AX_ADPCM_FRAME_SIZE * AX_ADPCM_SAMPLES_PER_FRAME;
        frac = addr % AX_ADPCM_FRAME_SIZE;
        if (frac != 0) {
            samples += (frac - 1) * sizeof(u16);
        }
        break;
    }

    case SAMPLE_FORMAT_PCM_S8: {
        samples = addr;
        break;
    }

    case SAMPLE_FORMAT_PCM_S16: {
        samples = addr / sizeof(u16);
        break;
    }
    }

    return samples;
}

void AxVoice::SetPriority(u32 priority) {
    mVpb.SetVoicePriority(priority);
}

void AxVoice::SetVoiceType(VoiceType type) {
    mVpb.SetVoiceType(type);
}

void AxVoice::EnableRemote(bool enable) {
    mVpb.SetVoiceRmtOn(enable);
}

void AxVoice::ResetDelta() {
    ut::AutoInterruptLock lock;

    if (!mVpb.IsAvailable()) {
        return;
    }

    mVpb.UpdateDelta();

    AXPBMIX mix;

    mix.vL = mMixPrev.vL;
    mix.vDeltaL = 0;

    mix.vR = mMixPrev.vR;
    mix.vDeltaR = 0;

    mix.vAuxAL = mMixPrev.vAuxAL;
    mix.vDeltaAuxAL = 0;

    mix.vAuxAR = mMixPrev.vAuxAR;
    mix.vDeltaAuxAR = 0;

    mix.vAuxBL = mMixPrev.vAuxBL;
    mix.vDeltaAuxBL = 0;

    mix.vAuxBR = mMixPrev.vAuxBR;
    mix.vDeltaAuxBR = 0;

    mix.vAuxCL = mMixPrev.vAuxCL;
    mix.vDeltaAuxCL = 0;

    mix.vAuxCR = mMixPrev.vAuxCR;
    mix.vDeltaAuxCR = 0;

    mix.vS = mMixPrev.vS;
    mix.vDeltaS = 0;

    mix.vAuxAS = mMixPrev.vAuxAS;
    mix.vDeltaAuxAS = 0;

    mix.vAuxBS = mMixPrev.vAuxBS;
    mix.vDeltaAuxBS = 0;

    mix.vAuxCS = mMixPrev.vAuxCS;
    mix.vDeltaAuxCS = 0;

    mVpb.SetVoiceMix(mix, true);
}

void AxVoice::SetAddr(bool loop, const void* pWave, u32 offset, u32 loopStart,
                      u32 loopEnd) {
    ut::AutoInterruptLock lock;

    u32 startAddr;
    u32 loopAddr;
    u32 endAddr;

    if (!mVpb.IsAvailable()) {
        return;
    }

    if (offset > loopEnd) {
        const void* pBuffer = AxManager::GetInstance().GetZeroBufferAddress();
        loop = false;

        startAddr = GetDspAddressBySample(pBuffer, 0, mFormat);
        loopAddr = GetDspAddressBySample(pBuffer, 0, mFormat);
        endAddr = GetDspAddressBySample(pBuffer, 1, mFormat);
    } else {
        if (loop) {
            loopAddr = GetDspAddressBySample(pWave, loopStart, mFormat);
        } else {
            const void* pBuffer = AxManager::GetInstance().GetZeroBufferAddress();
            loopAddr = GetDspAddressBySample(pBuffer, 0, mFormat);
        }

        startAddr = GetDspAddressBySample(pWave, offset, mFormat);
        endAddr = GetDspAddressBySample(pWave, loopEnd - 1, mFormat);
    }

    AXPBADDR addr;

    addr.loopFlag = loop;

    // Translate the internal sample-format code into the AX PB format code.
    switch (mFormat) {
    case SAMPLE_FORMAT_PCM_S16:
        addr.format = FORMAT_PCM16;
        break;

    case SAMPLE_FORMAT_PCM_S8:
        addr.format = FORMAT_PCM8;
        break;

    case SAMPLE_FORMAT_DSP_ADPCM:
    default:
        addr.format = FORMAT_ADPCM;
        break;
    }

    addr.loopAddressHi = loopAddr >> 16;
    addr.loopAddressLo = loopAddr;

    addr.endAddressHi = endAddr >> 16;
    addr.endAddressLo = endAddr;

    addr.currentAddressHi = startAddr >> 16;
    addr.currentAddressLo = startAddr;

    mVpb.SetVoiceAddr(addr);
}

void AxVoice::SetSrcType(SrcType type, f32 pitch) {
    ut::AutoInterruptLock lock;

    if (!mVpb.IsAvailable()) {
        return;
    }

    if (type == SRC_4TAP_AUTO) {
        f32 ratio = GetDspRatio(pitch);

        if (ratio > 4.0f / 3.0f) {
            type = SRC_4TAP_8K;
        } else if (ratio > 1.0f) {
            type = SRC_4TAP_12K;
        } else {
            type = SRC_4TAP_16K;
        }
    }

    mVpb.SetVoiceSrcType(type);
}

void AxVoice::SetAdpcm(const snd::AdpcmParam* pParam) {
    ut::AutoInterruptLock lock;
    AXPBADPCM adpcm;

    if (!mVpb.IsAvailable()) {
        return;
    }

    switch (mFormat) {
    case SAMPLE_FORMAT_DSP_ADPCM: {
        std::memcpy(adpcm.a, pParam->coef, sizeof(adpcm.a));
        adpcm.gain = pParam->gain;
        adpcm.pred_scale = pParam->pred_scale;
        adpcm.yn1 = pParam->yn1;
        adpcm.yn2 = pParam->yn2;
        break;
    }

    case SAMPLE_FORMAT_PCM_S16: {
        std::memset(adpcm.a, 0, sizeof(adpcm.a));
        adpcm.gain = 0x800;
        adpcm.pred_scale = 0;
        adpcm.yn1 = 0;
        adpcm.yn2 = 0;
        break;
    }

    case SAMPLE_FORMAT_PCM_S8: {
        std::memset(adpcm.a, 0, sizeof(adpcm.a));
        adpcm.gain = 0x100;
        adpcm.pred_scale = 0;
        adpcm.yn1 = 0;
        adpcm.yn2 = 0;
        break;
    }
    }

    mVpb.SetVoiceAdpcm(adpcm);
}

void AxVoice::SetAdpcmLoop(const AdpcmLoopParam* pParam) {
    ut::AutoInterruptLock lock;
    AXPBADPCMLOOP loop;

    if (!mVpb.IsAvailable()) {
        return;
    }

    // Retail compares the raw field against 3 (SAMPLE_FORMAT_DSP_ADPCM);
    // the header's legacy Format enum values are placeholders.
    if (mFormat == SAMPLE_FORMAT_DSP_ADPCM) {
        loop.loop_pred_scale = pParam->loop_pred_scale;
        loop.loop_yn1 = pParam->loop_yn1;
        loop.loop_yn2 = pParam->loop_yn2;
    } else {
        loop.loop_pred_scale = 0;
        loop.loop_yn1 = 0;
        loop.loop_yn2 = 0;
    }

    mVpb.SetVoiceAdpcmLoop(loop);
}

bool AxVoice::SetMix(const MixParam& rParam) {
    ut::AutoInterruptLock lock;

    if (!mVpb.IsAvailable()) {
        return false;
    }

    if (mFirstMixUpdateFlag || !IsRun()) {
        // Retail copies field-by-field (12x lhz/sth), not as a struct assign.
        mMixPrev.vL = rParam.vL;
        mMixPrev.vR = rParam.vR;
        mMixPrev.vS = rParam.vS;
        mMixPrev.vAuxAL = rParam.vAuxAL;
        mMixPrev.vAuxAR = rParam.vAuxAR;
        mMixPrev.vAuxAS = rParam.vAuxAS;
        mMixPrev.vAuxBL = rParam.vAuxBL;
        mMixPrev.vAuxBR = rParam.vAuxBR;
        mMixPrev.vAuxBS = rParam.vAuxBS;
        mMixPrev.vAuxCL = rParam.vAuxCL;
        mMixPrev.vAuxCR = rParam.vAuxCR;
        mMixPrev.vAuxCS = rParam.vAuxCS;
        mFirstMixUpdateFlag = false;
    }

    // Retail keeps the "any field changed" test out-of-line in a const
    // member helper; declared below with its retail entry point.
    bool needUpdate = ::IsNeedNextUpdate__Q44nw4r3snd6detail7AxVoiceCFRCQ54nw4r3snd6detail7AxVoice8MixParam(
        this, rParam);

    AXPBMIX mix;

    // Interleaved per-channel: retail batches the value fills first and
    // defers every delta computation until after them.
    mix.vL = mMixPrev.vL;
    mix.vR = mMixPrev.vR;
    mix.vS = mMixPrev.vS;
    mix.vAuxAL = mMixPrev.vAuxAL;
    mix.vAuxAR = mMixPrev.vAuxAR;
    mix.vAuxAS = mMixPrev.vAuxAS;
    mix.vAuxBL = mMixPrev.vAuxBL;
    mix.vAuxBR = mMixPrev.vAuxBR;
    mix.vAuxBS = mMixPrev.vAuxBS;
    mix.vAuxCL = mMixPrev.vAuxCL;
    mix.vAuxCR = mMixPrev.vAuxCR;
    mix.vAuxCS = mMixPrev.vAuxCS;

    // Retail guards each delta with an explicit equality fast-path.
    int vDeltaL = (mMixPrev.vL == rParam.vL)
                      ? 0
                      : CalcAxvpbDelta(mMixPrev.vL, rParam.vL);
    int vDeltaR = (mMixPrev.vR == rParam.vR)
                      ? 0
                      : CalcAxvpbDelta(mMixPrev.vR, rParam.vR);
    int vDeltaS = (mMixPrev.vS == rParam.vS)
                      ? 0
                      : CalcAxvpbDelta(mMixPrev.vS, rParam.vS);
    int vDeltaAuxAL = (mMixPrev.vAuxAL == rParam.vAuxAL)
                          ? 0
                          : CalcAxvpbDelta(mMixPrev.vAuxAL, rParam.vAuxAL);
    int vDeltaAuxAR = (mMixPrev.vAuxAR == rParam.vAuxAR)
                          ? 0
                          : CalcAxvpbDelta(mMixPrev.vAuxAR, rParam.vAuxAR);
    int vDeltaAuxAS = (mMixPrev.vAuxAS == rParam.vAuxAS)
                          ? 0
                          : CalcAxvpbDelta(mMixPrev.vAuxAS, rParam.vAuxAS);
    int vDeltaAuxBL = (mMixPrev.vAuxBL == rParam.vAuxBL)
                          ? 0
                          : CalcAxvpbDelta(mMixPrev.vAuxBL, rParam.vAuxBL);
    int vDeltaAuxBR = (mMixPrev.vAuxBR == rParam.vAuxBR)
                          ? 0
                          : CalcAxvpbDelta(mMixPrev.vAuxBR, rParam.vAuxBR);
    int vDeltaAuxBS = (mMixPrev.vAuxBS == rParam.vAuxBS)
                          ? 0
                          : CalcAxvpbDelta(mMixPrev.vAuxBS, rParam.vAuxBS);
    int vDeltaAuxCL = (mMixPrev.vAuxCL == rParam.vAuxCL)
                          ? 0
                          : CalcAxvpbDelta(mMixPrev.vAuxCL, rParam.vAuxCL);
    int vDeltaAuxCR = (mMixPrev.vAuxCR == rParam.vAuxCR)
                          ? 0
                          : CalcAxvpbDelta(mMixPrev.vAuxCR, rParam.vAuxCR);
    int vDeltaAuxCS = (mMixPrev.vAuxCS == rParam.vAuxCS)
                          ? 0
                          : CalcAxvpbDelta(mMixPrev.vAuxCS, rParam.vAuxCS);

    mix.vDeltaL = vDeltaL;
    mix.vDeltaR = vDeltaR;
    mix.vDeltaS = vDeltaS;

    mix.vDeltaAuxAL = vDeltaAuxAL;
    mix.vDeltaAuxAR = vDeltaAuxAR;
    mix.vDeltaAuxAS = vDeltaAuxAS;

    mix.vDeltaAuxBL = vDeltaAuxBL;
    mix.vDeltaAuxBR = vDeltaAuxBR;
    mix.vDeltaAuxBS = vDeltaAuxBS;

    mix.vDeltaAuxCL = vDeltaAuxCL;
    mix.vDeltaAuxCR = vDeltaAuxCR;
    mix.vDeltaAuxCS = vDeltaAuxCS;

    mVpb.SetVoiceMix(mix, false);

    if (rParam.vL == 0 || vDeltaL == 0) {
        mMixPrev.vL = rParam.vL;
    } else {
        mMixPrev.vL += vDeltaL * AX_SAMPLES_PER_FRAME;
    }

    if (rParam.vR == 0 || vDeltaR == 0) {
        mMixPrev.vR = rParam.vR;
    } else {
        mMixPrev.vR += vDeltaR * AX_SAMPLES_PER_FRAME;
    }

    if (rParam.vS == 0 || vDeltaS == 0) {
        mMixPrev.vS = rParam.vS;
    } else {
        mMixPrev.vS += vDeltaS * AX_SAMPLES_PER_FRAME;
    }

    if (rParam.vAuxAL == 0 || vDeltaAuxAL == 0) {
        mMixPrev.vAuxAL = rParam.vAuxAL;
    } else {
        mMixPrev.vAuxAL += vDeltaAuxAL * AX_SAMPLES_PER_FRAME;
    }

    if (rParam.vAuxAR == 0 || vDeltaAuxAR == 0) {
        mMixPrev.vAuxAR = rParam.vAuxAR;
    } else {
        mMixPrev.vAuxAR += vDeltaAuxAR * AX_SAMPLES_PER_FRAME;
    }

    if (rParam.vAuxAS == 0 || vDeltaAuxAS == 0) {
        mMixPrev.vAuxAS = rParam.vAuxAS;
    } else {
        mMixPrev.vAuxAS += vDeltaAuxAS * AX_SAMPLES_PER_FRAME;
    }

    if (rParam.vAuxBL == 0 || vDeltaAuxBL == 0) {
        mMixPrev.vAuxBL = rParam.vAuxBL;
    } else {
        mMixPrev.vAuxBL += vDeltaAuxBL * AX_SAMPLES_PER_FRAME;
    }

    if (rParam.vAuxBR == 0 || vDeltaAuxBR == 0) {
        mMixPrev.vAuxBR = rParam.vAuxBR;
    } else {
        mMixPrev.vAuxBR += vDeltaAuxBR * AX_SAMPLES_PER_FRAME;
    }

    if (rParam.vAuxBS == 0 || vDeltaAuxBS == 0) {
        mMixPrev.vAuxBS = rParam.vAuxBS;
    } else {
        mMixPrev.vAuxBS += vDeltaAuxBS * AX_SAMPLES_PER_FRAME;
    }

    if (rParam.vAuxCL == 0 || vDeltaAuxCL == 0) {
        mMixPrev.vAuxCL = rParam.vAuxCL;
    } else {
        mMixPrev.vAuxCL += vDeltaAuxCL * AX_SAMPLES_PER_FRAME;
    }

    if (rParam.vAuxCR == 0 || vDeltaAuxCR == 0) {
        mMixPrev.vAuxCR = rParam.vAuxCR;
    } else {
        mMixPrev.vAuxCR += vDeltaAuxCR * AX_SAMPLES_PER_FRAME;
    }

    if (rParam.vAuxCS == 0 || vDeltaAuxCS == 0) {
        mMixPrev.vAuxCS = rParam.vAuxCS;
    } else {
        mMixPrev.vAuxCS += vDeltaAuxCS * AX_SAMPLES_PER_FRAME;
    }

    return needUpdate;
}

void AxVoice::SetRmtMix(const RemoteMixParam& rParam) {
    AXPBRMTMIX mix;

    mix.vMain0 = rParam.vMain0;
    mix.vDeltaMain0 = 0;

    mix.vAux0 = rParam.vAux0;
    mix.vDeltaAux0 = 0;

    mix.vMain1 = rParam.vMain1;
    mix.vDeltaMain1 = 0;

    mix.vAux1 = rParam.vAux1;
    mix.vDeltaAux1 = 0;

    mix.vMain2 = rParam.vMain2;
    mix.vDeltaMain2 = 0;

    mix.vAux2 = rParam.vAux2;
    mix.vDeltaAux2 = 0;

    mix.vMain3 = rParam.vMain3;
    mix.vDeltaMain3 = 0;

    mix.vAux3 = rParam.vAux3;
    mix.vDeltaAux3 = 0;

    mVpb.SetVoiceRmtMix(mix);
}

void AxVoice::SetSrc(f32 ratio, bool initial) {
    ut::AutoInterruptLock lock;

    if (!mVpb.IsAvailable()) {
        return;
    }

    if (initial) {
        // Local (not the parameter): retail keeps this in scratch floats.
        // Retail clamps the ratio only when (re)initializing the SRC block.
        f32 dspRatio = ut::Clamp(
            GetDspRatio(ratio), lbl_eu_80669EAC, lbl_eu_80669EA8);

        AXPBSRC src;

        u32 ratioBits = lbl_eu_80669EB0 * dspRatio;

        src.ratioHi = ratioBits >> 16 & 0xFFFF;
        src.ratioLo = ratioBits & 0xFFFF;

        src.currentAddressFrac = 0;

        src.last_samples[0] = 0;
        src.last_samples[1] = 0;
        src.last_samples[2] = 0;
        src.last_samples[3] = 0;

        mVpb.SetVoiceSrc(src);
    } else {
        mVpb.SetVoiceSrcRatio(GetDspRatio(ratio));
    }
}

void AxVoice::SetVe(f32 volume, f32 initVolume) {
    ut::AutoInterruptLock lock;

    if (!mVpb.IsAvailable()) {
        return;
    }

    mVpb.SetVoiceVe(32767 * volume, 32767 * initVolume);
}

void AxVoice::SetLpf(u16 freq) {
    ut::AutoInterruptLock lock;

    if (!mVpb.IsAvailable()) {
        return;
    }

    if (freq >= 16000) {
        AXPBLPF lpf;
        lpf.on = 0;
        lpf.yn1 = 0;
        mVpb.SetVoiceLpf(lpf);
    } else if (mVpb.IsLpfEnable()) {
        u16 a0, b0;
        AXGetLpfCoefs(freq, &a0, &b0);
        mVpb.SetVoiceLpfCoefs(a0, b0);
    } else {
        AXPBLPF lpf;
        lpf.on = AX_PB_LPF_ON;
        lpf.yn1 = 0;
        AXGetLpfCoefs(freq, &lpf.a0, &lpf.b0);
        mVpb.SetVoiceLpf(lpf);
    }
}

void AxVoice::SetRemoteFilter(u8 filter) {
    ut::AutoInterruptLock lock;

    if (!mVpb.IsAvailable()) {
        return;
    }

    if (filter == 0) {
        AXPBRMTIIR iir;
        iir.lpf.on = 0;

        mVpb.SetVoiceRmtIIR(iir);
    } else if (mVpb.IsRmtIirEnable()) {
        u16 b0, b1, b2, a1, a2;
        Util::GetRemoteFilterCoefs(filter, &b0, &b1, &b2, &a1, &a2);

        mVpb.SetVoiceRmtIIRCoefs(AX_PB_BIQUAD_ON, b0, b1, b2, a1, a2);
    } else {
        AXPBRMTIIR iir;
        iir.biquad.on = AX_PB_BIQUAD_ON;
        iir.biquad.xn1 = 0;
        iir.biquad.xn2 = 0;
        iir.biquad.yn1 = 0;
        iir.biquad.yn2 = 0;

        Util::GetRemoteFilterCoefs(filter, &iir.biquad.b0, &iir.biquad.b1,
                                   &iir.biquad.b2, &iir.biquad.a1,
                                   &iir.biquad.a2);
        mVpb.SetVoiceRmtIIR(iir);
    }
}

void AxVoice::CalcOffsetAdpcmParam(u16* pPredScale, u16* pYN1, u16* pYN2,
                                   u32 offset, const void* pData,
                                   const snd::AdpcmParam& rParam) {
    AXPBADPCM adpcm;
    std::memcpy(adpcm.a, rParam.coef, sizeof(adpcm.a));
    adpcm.gain = rParam.gain;
    adpcm.pred_scale = rParam.pred_scale;
    adpcm.yn1 = rParam.yn1;
    adpcm.yn2 = rParam.yn2;

    // Pass the raw AX sample-format code (3 = DSP ADPCM); the header's Format
    // placeholders don't match the retail encoding.
    const void* pBuffer = pData;
    u32 addr = GetDspAddressBySample(pBuffer, 0,
                                     static_cast<Format>(SAMPLE_FORMAT_DSP_ADPCM));
    u32 end = GetDspAddressBySample(pData, offset,
                                    static_cast<Format>(SAMPLE_FORMAT_DSP_ADPCM));

    while (addr < end) {
        if (addr % AX_ADPCM_NIBBLES_PER_FRAME == 0) {
            adpcm.pred_scale =
                *static_cast<u8*>(OSPhysicalToCached(addr / sizeof(u16)));

            addr += sizeof(u16);
        }

        u8 byte = *static_cast<u8*>(OSPhysicalToCached(addr / sizeof(u16)));

        u8 nibble;
        if (addr % sizeof(u16) != 0) {
            nibble = byte & 0x0F;
        } else {
            nibble = byte >> 4;
        }

        DecodeDspAdpcm(&adpcm, nibble);
        addr++;
    }

    *pPredScale = adpcm.pred_scale;
    *pYN1 = adpcm.yn1;
    *pYN2 = adpcm.yn2;
}

void AxVoiceParamBlock::Sync() {
    ut::AutoInterruptLock lock;

    if (!IsAvailable()) {
        return;
    }

    mVpb->pb.ve.currentVolume = mPrevVeSetting.currentVolume;

    s16 deltaIn =
        (mVolume - mPrevVeSetting.currentVolume) / AX_SAMPLES_PER_FRAME;
    s16 deltaOut = (deltaIn + (deltaIn > 0 ? 1 : 0) != 0) ? 1 : -1;

    int predIn =
        mPrevVeSetting.currentVolume + (deltaIn * AX_SAMPLES_PER_FRAME);
    int predOut =
        mPrevVeSetting.currentVolume + (deltaOut * AX_SAMPLES_PER_FRAME);

    if (ut::Abs(mVolume - predIn) < ut::Abs(mVolume - predOut)) {
        mVpb->pb.ve.currentDelta = deltaIn;
    } else {
        mVpb->pb.ve.currentDelta = deltaOut;
    }

    int nextVolume = mPrevVeSetting.currentVolume +
                     (mVpb->pb.ve.currentDelta * AX_SAMPLES_PER_FRAME);

    if (nextVolume < 0) {
        mVpb->pb.ve.currentDelta =
            -mPrevVeSetting.currentVolume / AX_SAMPLES_PER_FRAME;
    } else if (nextVolume > 32767) {
        mVpb->pb.ve.currentDelta =
            (32767 - mPrevVeSetting.currentVolume) / AX_SAMPLES_PER_FRAME;
    }

    if (mVpb->pb.ve.currentDelta == 0 && mPrevVeSetting.currentDelta == 0) {
        mVpb->pb.ve.currentVolume = mVolume;
    }

    mSync &= ~AX_PBSYNC_VE_DELTA;
    mSync |= AX_PBSYNC_VE;

    mPrevVeSetting.currentVolume = mVpb->pb.ve.currentVolume;
    mPrevVeSetting.currentDelta = mVpb->pb.ve.currentDelta;

    mVpb->sync |= mSync;
    mSync = 0;
}

bool AxVoiceParamBlock::IsRmtIirEnable() const {
    return IsAvailable() && mVpb->pb.rmtIIR.biquad.on == AX_PB_BIQUAD_ON;
}

AxVoiceParamBlock::AxVoiceParamBlock()
    : mVpb(NULL),
      mSync(0),
      mPrevVeSetting(),
      mFirstVeUpdateFlag(false),
      mVolume(DEFAULT_VOLUME) {
    mPrevVeSetting.currentVolume = DEFAULT_VOLUME;
    mPrevVeSetting.currentDelta = 0;
}

void AxVoiceParamBlock::Set(AXVPB* pVpb) {
    mVpb = pVpb;
    mSync = 0;
    mFirstVeUpdateFlag = true;
    mVolume = DEFAULT_VOLUME;
    mPrevVeSetting.currentVolume = DEFAULT_VOLUME;
    mPrevVeSetting.currentDelta = 0;
}

void AxVoiceParamBlock::Clear() {
    mVpb = NULL;
    mSync = 0;
}

void AxVoiceParamBlock::SetVoiceType(u16 type) {
    ut::AutoInterruptLock lock;

    if (!IsAvailable()) {
        return;
    }

    mVpb->pb.type = type;
    mSync |= AX_PBSYNC_TYPE;
}

void AxVoiceParamBlock::SetVoiceVe(u16 volume, u16 initVolume) {
    ut::AutoInterruptLock lock;

    if (!IsAvailable()) {
        return;
    }

    if (mFirstVeUpdateFlag) {
        mPrevVeSetting.currentVolume = initVolume;
        mFirstVeUpdateFlag = false;
    }

    mVolume = volume;
}

void AxVoiceParamBlock::SetVoiceMix(const AXPBMIX& rMix, bool syncNow) {
    ut::AutoInterruptLock lock;

    if (!IsAvailable()) {
        return;
    }

    const u16* pSrc = reinterpret_cast<const u16*>(&rMix);
    u16* pDst = reinterpret_cast<u16*>(&mVpb->pb.mix);

    u32 ctrl = 0;

    // vL
    if ((*pDst++ = *pSrc++) != 0) {
        ctrl |= AX_MIXER_CTRL_L;
    }
    // vDeltaL
    if ((*pDst++ = *pSrc++) != 0) {
        ctrl |= AX_MIXER_CTRL_DELTA | AX_MIXER_CTRL_L;
    }
    // vR
    if ((*pDst++ = *pSrc++) != 0) {
        ctrl |= AX_MIXER_CTRL_R;
    }
    // vDeltaR
    if ((*pDst++ = *pSrc++) != 0) {
        ctrl |= AX_MIXER_CTRL_DELTA | AX_MIXER_CTRL_R;
    }

    // vAuxAL
    if ((*pDst++ = *pSrc++) != 0) {
        ctrl |= AX_MIXER_CTRL_AL;
    }
    // vDeltaAuxAL
    if ((*pDst++ = *pSrc++) != 0) {
        ctrl |= AX_MIXER_CTRL_DELTA_A | AX_MIXER_CTRL_AL;
    }
    // vAuxAR
    if ((*pDst++ = *pSrc++) != 0) {
        ctrl |= AX_MIXER_CTRL_AR;
    }
    // vDeltaAuxAR
    if ((*pDst++ = *pSrc++) != 0) {
        ctrl |= AX_MIXER_CTRL_DELTA_A | AX_MIXER_CTRL_AR;
    }

    // vAuxBL
    if ((*pDst++ = *pSrc++) != 0) {
        ctrl |= AX_MIXER_CTRL_BL;
    }
    // vDeltaAuxBL
    if ((*pDst++ = *pSrc++) != 0) {
        ctrl |= AX_MIXER_CTRL_DELTA_B | AX_MIXER_CTRL_BL;
    }
    // vAuxBR
    if ((*pDst++ = *pSrc++) != 0) {
        ctrl |= AX_MIXER_CTRL_BR;
    }
    // vDeltaAuxBR
    if ((*pDst++ = *pSrc++) != 0) {
        ctrl |= AX_MIXER_CTRL_DELTA_B | AX_MIXER_CTRL_BR;
    }

    // vAuxCL
    if ((*pDst++ = *pSrc++) != 0) {
        ctrl |= AX_MIXER_CTRL_CL;
    }
    // vDeltaAuxCL
    if ((*pDst++ = *pSrc++) != 0) {
        ctrl |= AX_MIXER_CTRL_DELTA_C | AX_MIXER_CTRL_CL;
    }
    // vAuxCR
    if ((*pDst++ = *pSrc++) != 0) {
        ctrl |= AX_MIXER_CTRL_CR;
    }
    // vDeltaAuxCR
    if ((*pDst++ = *pSrc++) != 0) {
        ctrl |= AX_MIXER_CTRL_DELTA_C | AX_MIXER_CTRL_CR;
    }

    // vS
    if ((*pDst++ = *pSrc++) != 0) {
        ctrl |= AX_MIXER_CTRL_S;
    }
    // vDeltaS
    if ((*pDst++ = *pSrc++) != 0) {
        ctrl |= AX_MIXER_CTRL_DELTA_S | AX_MIXER_CTRL_S;
    }

    // vAuxAS
    if ((*pDst++ = *pSrc++) != 0) {
        ctrl |= AX_MIXER_CTRL_AS;
    }
    // vDeltaAuxAS
    if ((*pDst++ = *pSrc++) != 0) {
        ctrl |= AX_MIXER_CTRL_DELTA_AS | AX_MIXER_CTRL_AS;
    }

    // vAuxBS
    if ((*pDst++ = *pSrc++) != 0) {
        ctrl |= AX_MIXER_CTRL_BS;
    }
    // vDeltaAuxBS
    if ((*pDst++ = *pSrc++) != 0) {
        ctrl |= AX_MIXER_CTRL_DELTA_BS | AX_MIXER_CTRL_BS;
    }

    // vAuxCS
    if ((*pDst++ = *pSrc++) != 0) {
        ctrl |= AX_MIXER_CTRL_CS;
    }
    // vDeltaAuxCS
    if ((*pDst++ = *pSrc++) != 0) {
        ctrl |= AX_MIXER_CTRL_DELTA_CS | AX_MIXER_CTRL_CS;
    }

    mVpb->pb.mixerCtrl = ctrl;

    if (syncNow) {
        mVpb->sync |= AX_PBSYNC_MIX | AX_PBSYNC_MIXER_CTRL;
    } else {
        mSync |= AX_PBSYNC_MIX | AX_PBSYNC_MIXER_CTRL;
    }
}

void AxVoiceParamBlock::SetVoiceLoop(u16 loop) {
    ut::AutoInterruptLock lock;

    if (!IsAvailable()) {
        return;
    }

    mVpb->pb.addr.loopFlag = loop;

    if (!(mVpb->sync & AX_PBSYNC_ADDR)) {
        mVpb->sync |= AX_PBSYNC_LOOP_FLAG;
    }
}

void AxVoiceParamBlock::SetVoiceLoopAddr(u32 addr) {
    ut::AutoInterruptLock lock;

    if (!IsAvailable()) {
        return;
    }

    mVpb->pb.addr.loopAddressHi = addr >> 16 & 0xFFFF;
    mVpb->pb.addr.loopAddressLo = addr & 0xFFFF;

    if (!(mVpb->sync & AX_PBSYNC_ADDR)) {
        mVpb->sync |= AX_PBSYNC_LOOP_ADDR;
    }
}

void AxVoiceParamBlock::SetVoiceEndAddr(u32 addr) {
    ut::AutoInterruptLock lock;

    if (!IsAvailable()) {
        return;
    }

    mVpb->pb.addr.endAddressHi = addr >> 16 & 0xFFFF;
    mVpb->pb.addr.endAddressLo = addr & 0xFFFF;

    if (!(mVpb->sync & AX_PBSYNC_ADDR)) {
        mVpb->sync |= AX_PBSYNC_END_ADDR;
    }
}

void AxVoiceParamBlock::SetVoiceAdpcm(const AXPBADPCM& rAdpcm) {
    ut::AutoInterruptLock lock;

    if (!IsAvailable()) {
        return;
    }

    std::memcpy(&mVpb->pb.adpcm, &rAdpcm, sizeof(AXPBADPCM));
    mSync |= AX_PBSYNC_ADPCM;
}

void AxVoiceParamBlock::SetVoiceSrcType(u32 type) {
    ut::AutoInterruptLock lock;

    if (!IsAvailable()) {
        return;
    }

    switch (type) {
    case AxVoice::SRC_NONE: {
        mVpb->pb.srcSelect = 2;
        break;
    }

    case AxVoice::SRC_LINEAR: {
        mVpb->pb.srcSelect = 1;
        break;
    }

    case AxVoice::SRC_4TAP_8K: {
        mVpb->pb.srcSelect = 0;
        mVpb->pb.coefSelect = 0;
        break;
    }

    case AxVoice::SRC_4TAP_12K: {
        mVpb->pb.srcSelect = 0;
        mVpb->pb.coefSelect = 1;
        break;
    }

    case AxVoice::SRC_4TAP_16K: {
        mVpb->pb.srcSelect = 0;
        mVpb->pb.coefSelect = 2;
        break;
    }
    }

    mSync |= AX_PBSYNC_SELECT;
}

void AxVoiceParamBlock::SetVoiceSrc(const AXPBSRC& rSrc) {
    ut::AutoInterruptLock lock;

    if (!IsAvailable()) {
        return;
    }

    std::memcpy(&mVpb->pb.src, &rSrc, sizeof(AXPBSRC));
    mSync &= ~AX_PBSYNC_SRC_RATIO;
    mSync |= AX_PBSYNC_SRC;
}

void AxVoiceParamBlock::SetVoiceSrcRatio(f32 ratio) {
    ut::AutoInterruptLock lock;

    if (!IsAvailable()) {
        return;
    }

    u32 ratioU32 = 65536 * ratio;
    mVpb->pb.src.ratioHi = ratioU32 >> 16 & 0xFFFF;
    mVpb->pb.src.ratioLo = ratioU32 & 0xFFFF;

    if (!(mSync & AX_PBSYNC_SRC)) {
        mSync |= AX_PBSYNC_SRC_RATIO;
    }
}

void AxVoiceParamBlock::SetVoiceAdpcmLoop(const AXPBADPCMLOOP& rLoop) {
    ut::AutoInterruptLock lock;

    if (!IsAvailable()) {
        return;
    }

    std::memcpy(&mVpb->pb.adpcmLoop, &rLoop, sizeof(AXPBADPCMLOOP));
    mSync |= AX_PBSYNC_ADPCM_LOOP;
}

void AxVoiceParamBlock::SetVoiceLpf(const AXPBLPF& rLpf) {
    ut::AutoInterruptLock lock;

    if (!IsAvailable()) {
        return;
    }

    std::memcpy(&mVpb->pb.lpf, &rLpf, sizeof(AXPBLPF));
    mSync |= AX_PBSYNC_LPF;
}

void AxVoiceParamBlock::SetVoiceLpfCoefs(u16 a0, u16 b0) {
    ut::AutoInterruptLock lock;

    if (!IsAvailable()) {
        return;
    }

    mVpb->pb.lpf.a0 = a0;
    mVpb->pb.lpf.b0 = b0;
    mSync |= AX_PBSYNC_LPF_COEFS;
}

void AxVoiceParamBlock::SetVoiceRmtOn(u16 on) {
    ut::AutoInterruptLock lock;

    if (!IsAvailable()) {
        return;
    }

    mVpb->pb.remote = on;
    mSync |= AX_PBSYNC_REMOTE;
}

void AxVoiceParamBlock::SetVoiceRmtMix(const AXPBRMTMIX& rMix) {
    ut::AutoInterruptLock lock;

    if (!IsAvailable()) {
        return;
    }

    u16* pDst = reinterpret_cast<u16*>(&mVpb->pb.rmtMix);
    const u16* pSrc = reinterpret_cast<const u16*>(&rMix);

    u16 ctrl = 0;

    // vMain0
    if ((*pDst++ = *pSrc++) != 0) {
        ctrl |= AX_MIXER_CTRL_RMT_M0;
    }
    // vDeltaMain0
    if ((*pDst++ = *pSrc++) != 0) {
        ctrl |= AX_MIXER_CTRL_RMT_DELTA_M0;
    }
    // vAux0
    if ((*pDst++ = *pSrc++) != 0) {
        ctrl |= AX_MIXER_CTRL_RMT_A0;
    }
    // vDeltaAux0
    if ((*pDst++ = *pSrc++) != 0) {
        ctrl |= AX_MIXER_CTRL_RMT_DELTA_A0;
    }

    // vMain1
    if ((*pDst++ = *pSrc++) != 0) {
        ctrl |= AX_MIXER_CTRL_RMT_M1;
    }
    // vDeltaMain1
    if ((*pDst++ = *pSrc++) != 0) {
        ctrl |= AX_MIXER_CTRL_RMT_DELTA_M1;
    }
    // vAux1
    if ((*pDst++ = *pSrc++) != 0) {
        ctrl |= AX_MIXER_CTRL_RMT_A1;
    }
    // vDeltaAux1
    if ((*pDst++ = *pSrc++) != 0) {
        ctrl |= AX_MIXER_CTRL_RMT_DELTA_A1;
    }

    // vMain2
    if ((*pDst++ = *pSrc++) != 0) {
        ctrl |= AX_MIXER_CTRL_RMT_M2;
    }
    // vDeltaMain2
    if ((*pDst++ = *pSrc++) != 0) {
        ctrl |= AX_MIXER_CTRL_RMT_DELTA_M2;
    }
    // vAux2
    if ((*pDst++ = *pSrc++) != 0) {
        ctrl |= AX_MIXER_CTRL_RMT_A2;
    }
    // vDeltaAux2
    if ((*pDst++ = *pSrc++) != 0) {
        ctrl |= AX_MIXER_CTRL_RMT_DELTA_A2;
    }

    // vMain3
    if ((*pDst++ = *pSrc++) != 0) {
        ctrl |= AX_MIXER_CTRL_RMT_M3;
    }
    // vDeltaMain3
    if ((*pDst++ = *pSrc++) != 0) {
        ctrl |= AX_MIXER_CTRL_RMT_DELTA_M3;
    }
    // vAux3
    if ((*pDst++ = *pSrc++) != 0) {
        ctrl |= AX_MIXER_CTRL_RMT_A3;
    }
    // vDeltaAux3
    if ((*pDst++ = *pSrc++) != 0) {
        ctrl |= AX_MIXER_CTRL_RMT_DELTA_A3;
    }

    mVpb->pb.rmtMixerCtrl = ctrl;
    mSync |= AX_PBSYNC_RMT_MIXER_CTRL | AX_PBSYNC_RMTMIX;
}

void AxVoiceParamBlock::SetVoiceRmtIIR(const AXPBRMTIIR& rIir) {
    ut::AutoInterruptLock lock;

    if (!IsAvailable()) {
        return;
    }

    std::memcpy(&mVpb->pb.rmtIIR, &rIir, sizeof(AXPBRMTIIR));
    mSync |= AX_PBSYNC_RMTIIR;
}

void AxVoiceParamBlock::SetVoiceRmtIIRCoefs(u16 type, ...) {
    ut::AutoInterruptLock lock;

    int i;
    int argc;

    if (!IsAvailable()) {
        return;
    }

    if (type == AX_PB_LPF_ON) {
        argc = 2;
    } else if (type == AX_PB_BIQUAD_ON) {
        argc = 5;
    } else {
        return;
    }

    std::va_list argv;
    u16 coefs[5];

    va_start(argv, type);
    for (i = 0; i < argc; i++) {
        coefs[i] = va_arg(argv, s32);
    }
    va_end(argv);

    if (type == AX_PB_LPF_ON) {
        mVpb->pb.rmtIIR.lpf.a0 = coefs[0];
        mVpb->pb.rmtIIR.lpf.b0 = coefs[1];
        mSync |= AX_PBSYNC_RMTIIR_LPF_COEFS;
    } else {
        mVpb->pb.rmtIIR.biquad.b0 = coefs[0];
        mVpb->pb.rmtIIR.biquad.b1 = coefs[1];
        mVpb->pb.rmtIIR.biquad.b2 = coefs[2];
        mVpb->pb.rmtIIR.biquad.a1 = coefs[3];
        mVpb->pb.rmtIIR.biquad.a2 = coefs[4];
        mSync |= AX_PBSYNC_RMTIIR_BIQUAD_COEFS;
    }
}

void AxVoiceParamBlock::UpdateDelta() {
    ut::AutoInterruptLock lock;

    if (!IsAvailable()) {
        return;
    }

    mPrevVeSetting.currentVolume +=
        mVpb->pb.ve.currentDelta * AX_SAMPLES_PER_FRAME;

    mVpb->pb.ve.currentVolume = mPrevVeSetting.currentVolume;
    mVpb->pb.ve.currentDelta = 0;

    mVpb->sync |= AX_PBSYNC_VE;
}

} // namespace detail
} // namespace snd
} // namespace nw4r

// Retail types this helper's format argument as the raw nw4r::snd::SampleFormat
// encoding (0 = PCM_S32/none, 1 = PCM_S16, 2 = PCM_S8, 3 = DSP_ADPCM), not the
// legacy AxVoice::Format placeholder enum used by the read-only header, so the
// definition carries the retail symbol directly.
namespace {

// View of the per-voice AX structures written by SetBiquad: AxVoice keeps a
// pointer to its DSP parameter block at offset 0x0 (voice biquad state at
// +0xEA) and the DSP sync flag word directly at offset 0x4.
struct AxBiquadData {
    u8 field_0x00[0xEA];
    u16 biquad[10];        // on,xn1,xn2,yn1,yn2,b0..a2
};

struct AxHeaderView {
    void* pData;           // at 0x0 -> AxBiquadData
    u32 sync;              // at 0x4
};

// Biquad coefficient set handed back by a provider.
struct BiquadCoefs {
    u16 b0;
    u16 b1;
    u16 b2;
    u16 a1;
    u16 a2;
};

// Full biquad filter state written to the voice parameter block.
struct BiquadBlock {
    u16 on;
    u16 xn1;
    u16 xn2;
    u16 yn1;
    u16 yn2;
    u16 b0;
    u16 b1;
    u16 b2;
    u16 a1;
    u16 a2;
};

// Abstract biquad coefficient provider; GetBiquadCoefs sits at vtable slot
// 3 (offset 0xC).
class IBiquadProvider {
public:
    virtual ~IBiquadProvider();
    virtual void GetBiquadCoefs(int biquadType, BiquadCoefs* pCoefs,
                                f32 value);
};

} // namespace

extern "C" u32
    GetSampleByDspAddress__Q44nw4r3snd6detail7AxVoiceFPCvUlQ34nw4r3snd12SampleFormat(
        const void* pBase, u32 addr, nw4r::snd::SampleFormat fmt) {
    if (pBase != NULL) {
        pBase = OSCachedToPhysical(pBase);
    }

    u32 samples = 0;

    switch (fmt) {
    case nw4r::snd::SAMPLE_FORMAT_PCM_S8: {
        samples = addr - reinterpret_cast<u32>(pBase);
        break;
    }

    case nw4r::snd::SAMPLE_FORMAT_DSP_ADPCM: {
        // Nibble address -> sample index within the DSP ADPCM stream.
        samples = addr - reinterpret_cast<u32>(pBase) * sizeof(u16);
        // clang-format off
        samples = (samples % AX_ADPCM_NIBBLES_PER_FRAME) +
                  (samples / AX_ADPCM_NIBBLES_PER_FRAME * AX_ADPCM_SAMPLES_PER_FRAME) -
                  sizeof(u16);
        // clang-format on
        break;
    }

    case nw4r::snd::SAMPLE_FORMAT_PCM_S16: {
        samples = addr - reinterpret_cast<u32>(pBase) / sizeof(u16);
        break;
    }
    }

    return samples;
}

// IsNeedNextUpdate compares AxVoice's private mMixPrev (offset 0x1E) against
// rParam field-by-field. The header is read-only this session, so the retail
// entry point keeps its verbatim symbol and reads mMixPrev through a
// layout-matched view instead of member access.
namespace {
struct MixPrevView {
    u8 field_0x00[0x1E]; // AxVoice header up to mMixPrev
    u16 vL;
    u16 vR;
    u16 vS;
    u16 vAuxAL;
    u16 vAuxAR;
    u16 vAuxAS;
    u16 vAuxBL;
    u16 vAuxBR;
    u16 vAuxBS;
    u16 vAuxCL;
    u16 vAuxCR;
    u16 vAuxCS;
};
} // namespace

extern "C" bool IsNeedNextUpdate__Q44nw4r3snd6detail7AxVoiceCFRCQ54nw4r3snd6detail7AxVoice8MixParam(
    const nw4r::snd::detail::AxVoice* pThis,
    const nw4r::snd::detail::AxVoice::MixParam& rParam) {
    const MixPrevView& rPrev = *reinterpret_cast<const MixPrevView*>(pThis);

    if (rPrev.vL != rParam.vL) {
        return true;
    }
    if (rPrev.vR != rParam.vR) {
        return true;
    }
    if (rPrev.vS != rParam.vS) {
        return true;
    }
    if (rPrev.vAuxAL != rParam.vAuxAL) {
        return true;
    }
    if (rPrev.vAuxAR != rParam.vAuxAR) {
        return true;
    }
    if (rPrev.vAuxAS != rParam.vAuxAS) {
        return true;
    }
    if (rPrev.vAuxBL != rParam.vAuxBL) {
        return true;
    }
    if (rPrev.vAuxBR != rParam.vAuxBR) {
        return true;
    }
    if (rPrev.vAuxBS != rParam.vAuxBS) {
        return true;
    }
    if (rPrev.vAuxCL != rParam.vAuxCL) {
        return true;
    }
    if (rPrev.vAuxCR != rParam.vAuxCR) {
        return true;
    }

    return rPrev.vAuxCS != rParam.vAuxCS;
}

// SetBiquad is not declared in snd_AxVoice.h (header is read-only this
// session), so the definition keeps the retail member symbol and receives
// `this` explicitly; the voice's parameter-block pointer lives at offset
// 0x0 of AxVoice.
namespace {
AxBiquadData* GetAxPbOf(const nw4r::snd::detail::AxVoice* pVoice) {
    return reinterpret_cast<AxBiquadData*>(
        *reinterpret_cast<void* const*>(pVoice));
}

AxHeaderView* GetAxHeader(nw4r::snd::detail::AxVoice* pVoice) {
    return reinterpret_cast<AxHeaderView*>(pVoice);
}
} // namespace
extern "C" void SetBiquad__Q44nw4r3snd6detail7AxVoiceFUcf(
    nw4r::snd::detail::AxVoice* pThis, unsigned char biquad, f32 value) {
    nw4r::ut::AutoInterruptLock lock;

    if (GetAxPbOf(pThis) == NULL) {
        return;
    }

    // Make sure the AX manager (which owns the provider table) is live.
    nw4r::snd::detail::AxManager::GetInstance();

    // Sequential availability checks clear the flag; either failure routes
    // to the "filter off" path.
    void* pProvider = lbl_eu_80637C20[biquad];
    bool enable = true;
    if (biquad == 0) {
        enable = false;
    }
    if (pProvider == NULL) {
        enable = false;
    }

    if (!enable) {
        // No provider (or selector 0): silence the filter. Only the state
        // half of the block is cleared before the bulk copy.
        BiquadBlock clear;
        clear.on = 0;
        clear.xn1 = 0;
        clear.xn2 = 0;
        clear.yn1 = 0;
        clear.yn2 = 0;

        u32 lock = OSDisableInterrupts();
        if (GetAxPbOf(pThis) == NULL) {
            OSRestoreInterrupts(lock);
        } else {
            std::memcpy(GetAxPbOf(pThis)->biquad, &clear, sizeof(BiquadBlock));
            GetAxHeader(pThis)->sync |= AX_PBSYNC_BIQUAD;
            OSRestoreInterrupts(lock);
        }
    } else {
        BiquadCoefs coefs;
        u32 lock;

        static_cast<IBiquadProvider*>(pProvider)
            ->GetBiquadCoefs(biquad, &coefs, value);

        bool biquadOn =
            GetAxPbOf(pThis) != NULL &&
            GetAxPbOf(pThis)->biquad[0] == AX_PB_BIQUAD_ON;

        if (biquadOn) {
            // Filter already running: update only the coefficients.
            u16 b0;
            u16 b1;
            u16 b2;
            u16 a1;
            u16 a2;

            // Reverse-order evaluation matches the retail temp scheduling.
            a2 = coefs.a2;
            a1 = coefs.a1;
            b2 = coefs.b2;
            b1 = coefs.b1;
            b0 = coefs.b0;

            lock = OSDisableInterrupts();
            if (GetAxPbOf(pThis) == NULL) {
                OSRestoreInterrupts(lock);
            } else {
                GetAxPbOf(pThis)->biquad[5] = b0;
                GetAxPbOf(pThis)->biquad[6] = b1;
                GetAxPbOf(pThis)->biquad[7] = b2;
                GetAxPbOf(pThis)->biquad[8] = a1;
                GetAxPbOf(pThis)->biquad[9] = a2;
                GetAxHeader(pThis)->sync |= AX_PBSYNC_BIQUAD_COEFS;
                OSRestoreInterrupts(lock);
            }
        } else {
            // Cold start: reset the history registers and program the new
            // coefficients in one block.
            BiquadBlock iir;
            iir.on = AX_PB_BIQUAD_ON;
            iir.xn1 = 0;
            iir.xn2 = 0;
            iir.yn1 = 0;
            iir.yn2 = 0;
            iir.b0 = coefs.b0;
            iir.b1 = coefs.b1;
            iir.b2 = coefs.b2;
            iir.a1 = coefs.a1;
            iir.a2 = coefs.a2;

            lock = OSDisableInterrupts();
            if (GetAxPbOf(pThis) == NULL) {
                OSRestoreInterrupts(lock);
            } else {
                std::memcpy(GetAxPbOf(pThis)->biquad, &iir,
                            sizeof(BiquadBlock));
                GetAxHeader(pThis)->sync |= AX_PBSYNC_BIQUAD;
                OSRestoreInterrupts(lock);
            }
        }
    } // lock restores the outer interrupt level
}
