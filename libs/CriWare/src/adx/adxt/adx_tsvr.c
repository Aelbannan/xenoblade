// Auto-scaffolded catalog TU for CriWare/src/adx/adxt/adx_tsvr
// Replace stubs with high-level C/C++ during decomp.

#include <harness_catalog.h>

__declspec(section ".rodata") __attribute__((aligned(8))) char lbl_eu_80516A88[0x98] = "E8101201 adxt_trap_entry: not enough data\x00" "E9081001 adxt_stat_decinfo: can't play this number of channels\x00" "E02080842 adxt_ExecHndl: parameter error\x00";

// Forward declarations for external functions
void ADXCRS_Enter(void);
void ADXCRS_Leave(void);
struct ADXT_Tsvr;
void adxt_ExecHndl(struct ADXT_Tsvr* hndl);
void ADXSJD_SetDefFmt(void* base);

// Forward declarations for ADX library functions
u32 ADXSJD_GetDecDtLen(void* sjd);
void ADXSJD_SetTrapNumSmpl(void* sjd, s32 num);
int ADXSJD_GetStat(void* sjd);
int ADXSJD_GetNumChan(void* sjd);
void ADXRNA_SetTransSw(void* rna, int sw);
int ADXT_GetNumSctIbuf(void* self);
int ADXT_GetNumSmplObuf(void* self, int arg);
void ADXRNA_SetPlaySw(void* rna, int sw);
u32 ADXSTM_GetFileSct(void* stream);
u32 ADXSTM_Tell(void* stream);
u32 ADXRNA_GetNumData(void* rna);
u32 ADXRNA_GetNumRoom(void* rna);
u32 ADXSJD_GetBlkSmpl(void* sjd);

// Opaque type definitions
struct ADXT_ChannelObj;
typedef struct ADXAMP_State ADXAMP_State;
typedef struct ADXSJD_State ADXSJD_State;
typedef struct ADXSTM_State ADXSTM_State;
typedef struct ADXRNA_State ADXRNA_State;
typedef struct ADXT_ChannelObj ADXT_ChannelObj;

// ADXT_Tsvr struct definition based on ASM analysis
typedef struct ADXT_Tsvr {
    u8  field_0x00;         // field_0x00: unknown
    s8  state;              // field_0x01: state (1=?, 3=playing, 4=done?)
    u8  format;             // field_0x02: format type
    u8  field_0x03;         // field_0x03: padding or unknown
    ADXSJD_State* sjd;      // field_0x04: sound job descriptor
    ADXSTM_State* stream;   // field_0x08: stream state
    ADXRNA_State* rna;      // field_0x0C: ring buffer/audio output
    ADXT_ChannelObj* field_0x10;  // field_0x10: linked object notified on stream restart
    ADXT_ChannelObj* field_0x14;  // field_0x14: decoder/channel object
    ADXT_ChannelObj* channels[8];  // field_0x18: array of channel/voice object pointers
    u32 field_0x38;         // field_0x38: samples-per-block divisor
    u8  field_0x3C[2];      // fields 0x3C-0x3D: padding to align field_0x3E
    s16 field_0x3E;         // field_0x3E: loop end position
    s16 field_0x40;         // field_0x40: output volume
    s16 field_0x42;         // field_0x42: pan for channel 0
    s16 field_0x44;         // field_0x44: pan for channel 1
    u8  field_0x46[2];      // fields 0x46-0x47: padding
    s32 field_0x48;         // field_0x48: limit value (signed)
    s32 field_0x4C;         // field_0x4C: trap entry counter
    u32 field_0x50;         // field_0x50: required decode buffer length
    u8  field_0x54[4];      // fields 0x54-0x57: unknown
    u8  field_0x58[8];      // fields 0x58-0x5F: unknown
    s16 field_0x60;         // field_0x60: stop position (-1 on EOS)
    u8  field_0x62[0xA];    // fields 0x62-0x6B: unknown
    s8  loopFlag;           // field_0x6C: loop flag (signed)
    u8  field_0x6D;         // field_0x6D: unknown
    s8  flag_0x6E;          // field_0x6E: unknown flag
    u8  field_0x6F;         // field_0x6F: unknown
    s8  flag_0x70;          // field_0x70: pause flag (signed byte)
    s8  flag_0x71;          // field_0x71: prepared flag
    s8  flag_0x72;          // field_0x72: flag (signed)
    s8  flag_0x73;          // field_0x73: flag (signed)
    ADXAMP_State* field_0x74;  // field_0x74: attached amp (NULL when unused)
    u8  field_0x78[0x14];   // fields 0x78-0x8B: unknown
    u32 field_0x8C;         // field_0x8C: loop end sector
    s32 field_0x90;         // field_0x90: loop span in samples
    struct LSC_State* lsc;  // field_0x94: LSC (loudness/sound control?) object
    s8  field_0x98;         // field_0x98: signed flag checked by nlp trap entry
    u8  field_0x99[3];      // fields 0x99-0x9B: unknown
    u32 field_0x9C;         // field_0x9C: cleared on play start
    u32 field_0xA0;         // field_0xA0: callback/data word set on play start
    u32 field_0xA4;         // field_0xA4: accumulated decoded sample count
    s8  field_0xA8;         // field_0xA8: pending-stream-start flag
    u8  field_0xA9[7];      // fields 0xA9-0xAF: padding
    u32 field_0xB0;         // field_0xB0: stream start arg (file name/id)
    u32 field_0xB4;         // field_0xB4: stream start sector offset
    u32 field_0xB8;         // field_0xB8: stream start sector length
    u32 field_0xBC;         // field_0xBC: stream start channel count
    u32 threshold;          // field_0xC0: threshold for decoded data length
} ADXT_Tsvr;

// Opaque LSC state
struct LSC_State;
typedef struct LSC_State LSC_State;

// Channel object with vtable
struct ADXT_ChannelObj {
    struct ADXT_ChannelObj_VTable* vtable;  // vtable at offset 0
};

// Buffer info block filled by func_0x18: data pointer + byte length.
struct ADXT_BufInfo {
    void* data;
    u32   len;
};
typedef struct ADXT_BufInfo ADXT_BufInfo;

// VTable for ADXT_ChannelObj - functions at offsets 0x18, 0x20, 0x24
struct ADXT_ChannelObj_VTable {
    void (*reserved[5])(void);  // functions at offsets 0x00-0x10 (unknown)
    void (*func_0x14)(ADXT_ChannelObj* self);                                        // reset/rewind decoder
    void (*func_0x18)(ADXT_ChannelObj* self, int idx, u32 size, ADXT_BufInfo* info); // acquire decode buffers
    void (*func_0x1C)(ADXT_ChannelObj* self, int idx, ADXT_BufInfo* info);           // re-acquire buffers
    void (*func_0x20)(ADXT_ChannelObj* self, int idx, ADXT_BufInfo* info);           // release/commit buffers
    int (*func_0x24)(ADXT_ChannelObj* self, int arg);  // at offset 0x24
};

/**
 * Wrapper for adxt_ExecHndl that enters/leaves the ADX critical section.
 * This ensures proper serialization when executing ADXT handle operations.
 */
void ADXT_ExecHndl(void* hndl) {
    ADXCRS_Enter();
    adxt_ExecHndl(hndl);
    ADXCRS_Leave();
}

/**
 * Sets the default format for a sound job descriptor.
 * 
 * The function extracts a base pointer from the struct at offset 4 and
 * passes it to ADXSJD_SetDefFmt along with a format parameter.
 * 
 * @param self Pointer to some ADXT struct containing a sound job descriptor pointer at offset 4
 * @param fmt Format parameter (passed to ADXSJD_SetDefFmt to match ABI)
 */
void criware_eu_803878BC(void* self, void* fmt) {
    ADXCRS_Enter();
    // The retail ASM sets r4 before calling ADXSJD_SetDefFmt.
    // Cast to force two-argument calling convention to match retail codegen.
    ((void(*)(void*, void*))ADXSJD_SetDefFmt)(*(void**)((u8*)self + 4), fmt);
    ADXCRS_Leave();
}

extern s32 ADXSJD_GetLpStartPos(void* sj);
extern s32 ADXSJD_GetLpStartOfst(void* sj);
extern s32 ADXSJD_GetLpEndPos(void* sj);
extern void ADXSJD_TakeSnapshot(void* sj);
extern void ADXSJD_SetTrapCnt(void* sj, s32 a);
extern void ADXSJD_SetTrapDtLen(void* sj, s32 a);
extern void ADXSJD_SetDecPos(void* sj, s32 a);
extern void ADXSJD_EntryTrapFunc(void* sj, void* fn, void* a);
extern s32 ADXSJD_GetFormat(ADXSJD_State* sjd);
extern void ADXSJD_SetMaxDecSmpl(ADXSJD_State* sjd, s32 val);
extern s32 ADXSJD_GetTotalNumSmpl(ADXSJD_State* sjd);
void adxt_trap_entry(ADXT_Tsvr* tsvr);

// Loop-start trap entry: snapshot the SJD and arm the decoder trap so
// decoding restarts from the loop begin offset.
void adxt_trap_entry_lps(void* self) {
    ADXT_Tsvr* tsvr = (ADXT_Tsvr*)self;
    void* sj = tsvr->sjd;
    s32 start;
    s32 ofst;
    s32 end;
    s32 d;
    start = ADXSJD_GetLpStartPos(sj);
    ofst = ADXSJD_GetLpStartOfst(sj);
    end = ADXSJD_GetLpEndPos(sj);
    ADXSJD_TakeSnapshot(sj);
    ADXSJD_SetTrapCnt(sj, 0);
    d = end - start;
    *(u32*)((u8*)tsvr + 0x90) = (u32)d;
    ADXSJD_SetTrapNumSmpl(sj, d);
    ADXSJD_SetTrapDtLen(sj, ofst);
    ADXSJD_SetDecPos(sj, start);
    ADXSJD_EntryTrapFunc(sj, (void*)adxt_trap_entry, tsvr);
}

extern void ADXCRS_Lock(void);
extern void ADXCRS_Unlock(void);
extern s32 ADX_DecodeFooter(u8* data, s32 maxLen, s16* outLen);
extern s32 ADX_ScanInfoCodeWav(const u8* codes, s32 size, u16* outLen);
extern s32 ADX_ScanInfoCode(const u16* codes, s32 size, u16* outLen);
extern void SJ_SplitChunk(const ADXT_BufInfo* src, int size, ADXT_BufInfo* dst1,
                          ADXT_BufInfo* dst2);

/**
 * Non-loop trap handler: invoked when decoding reaches the end of the stream.
 *
 * Scans the final decoded chunks for metadata (footer / info codes), then
 * either stops linked playback or splits the trailing chunks around the
 * discovered metadata and restarts the decoder so the metadata is consumed.
 * Finally restarts the SJD and re-arms trap parameters when the decoder
 * reports state 2.
 */
void adxt_nlp_trap_entry(ADXT_Tsvr* tsvr) {
    ADXSJD_State* sjd;
    ADXT_ChannelObj* dec;
    ADXT_BufInfo info28;
    ADXT_BufInfo info20;
    ADXT_BufInfo info18;
    ADXT_BufInfo info10;
    s16 posA;
    s16 posB;
    s32 oldA;
    s32 res1;
    s32 res2;

    dec = tsvr->field_0x14;
    sjd = tsvr->sjd;

    if ((s8)tsvr->field_0x98 == 0) {
        return;
    }
    posB = 0;
    posA = 0;

    ADXCRS_Lock();
    dec->vtable->func_0x18(dec, 1, 0x7FFFFFFF, &info28);
    dec->vtable->func_0x18(dec, 1, 0x7FFFFFFF, &info18);

    // Format 0: parse the footer; on failure just drop the link and finish.
    if (ADXSJD_GetFormat(sjd) == 0 && ADX_DecodeFooter(info28.data, info28.len, &posA) != 0) {
        ADXT_SetLnkSw(tsvr, 0);
        dec->vtable->func_0x1C(dec, 1, &info18);
        dec->vtable->func_0x1C(dec, 1, &info28);
        ADXCRS_Unlock();
        return;
    }

    // Scan both chunks for metadata codes starting after the footer position.
    // A zero return means no metadata was found in that chunk.
    oldA = posA;
    if (ADXSJD_GetFormat(sjd) == 1) {
        res1 = ADX_ScanInfoCodeWav((const u8*)info28.data + oldA, info28.len - oldA, (u16*)&posA);
        if (res1 == 0) {
            res2 = -1;
        } else {
            res2 = ADX_ScanInfoCodeWav((const u8*)info18.data, info18.len, (u16*)&posB);
        }
    } else {
        res1 = ADX_ScanInfoCode((const u16*)info28.data + oldA, info28.len - oldA, (u16*)&posA);
        if (res1 == 0) {
            res2 = -1;
        } else {
            res2 = ADX_ScanInfoCode((const u16*)info18.data, info18.len, (u16*)&posB);
        }
    }

    if (res1 != 0 && res2 != 0) {
        // Metadata found in both chunks: unlink playback and release everything.
        ADXT_SetLnkSw(tsvr, 0);
        dec->vtable->func_0x1C(dec, 1, &info18);
        dec->vtable->func_0x1C(dec, 1, &info28);
        ADXCRS_Unlock();
        return;
    }

    if (res1 == 0) {
        // No metadata in the first chunk: keep chunk 18 whole, trim chunk 28.
        dec->vtable->func_0x1C(dec, 1, &info18);
        SJ_SplitChunk(&info28, oldA + posA, &info28, &info20);
        dec->vtable->func_0x20(dec, 0, &info28);
        dec->vtable->func_0x1C(dec, 1, &info20);
    } else {
        // Metadata in the first chunk: commit chunk 28 whole, trim chunk 18.
        dec->vtable->func_0x20(dec, 0, &info28);
        SJ_SplitChunk(&info18, posB, &info18, &info10);
        dec->vtable->func_0x20(dec, 0, &info18);
        dec->vtable->func_0x1C(dec, 1, &info10);
    }

    ADXCRS_Unlock();
    tsvr->field_0xA4 += ADXSJD_GetDecNumSmpl(sjd);
    ADXSJD_Stop(sjd);
    ADXSJD_Start(sjd);
    ADXSJD_ExecHndl(sjd);

    if (ADXSJD_GetStat(sjd) != 2) {
        ADXT_SetLnkSw(tsvr, 0);
        return;
    }

    // Decoder restarted cleanly: cap decode size and reset the trap counters.
    ADXSJD_SetMaxDecSmpl(sjd, tsvr->field_0x48);
    ADXSJD_SetTrapNumSmpl(sjd, ADXSJD_GetTotalNumSmpl(sjd));
    ADXSJD_SetTrapDtLen(sjd, 0);
    ADXSJD_SetTrapCnt(sjd, 0);
}

extern void ADXSTM_SetEos(ADXSTM_State* stm, s32 val);
extern void ADXSTM_EntryEosFunc(ADXSTM_State* stm, void (*fn)(void*), void* arg);
extern void adxt_start_stm(ADXT_Tsvr* self, u32 fname, u32 sctOfst, u32 sctLen, s32 numChan);
extern void adxt_eos_entry(void* stream);
extern s32 ADXSJD_GetSfreq(ADXSJD_State* sjd);
extern s32 ADXSJD_GetNumLoop(ADXSJD_State* sjd);
extern s32 ADXSJD_GetLpEndOfst(ADXSJD_State* sjd);
extern s32 ADXSJD_GetOutBps(ADXSJD_State* sjd);
extern s32 ADXSJD_GetSpsdInfo(ADXSJD_State* sjd);
extern void ADXRNA_SetBitPerSmpl(ADXRNA_State* rna, s32 val);
extern void ADXRNA_SetSfreq(ADXRNA_State* rna, s32 val);
extern void ADXRNA_SetNumChan(ADXRNA_State* rna, s32 val);
extern void ADXRNA_SetTotalNumSmpl(ADXRNA_State* rna, s32 val);
extern void ADXRNA_SetStmHdInfo(ADXRNA_State* rna, s32 info);
extern void ADXT_Stop(ADXT_Tsvr* self);
extern void ADXT_SetOutVol(ADXT_Tsvr* self, s16 vol);
extern void ADXT_GetTranspose(ADXT_Tsvr* self, s32* outType, s32* outVal);
extern void ADXT_SetTranspose(ADXT_Tsvr* self, s32 type, s32 val);
extern void ADXT_SetOutPan(ADXT_Tsvr* self, s32 chan, s16 pan);
extern void ADXAMP_SetSfreq(ADXAMP_State* amp, s32 sfreq);
extern void ADXERR_ItoA2(s32 val1, s32 val2, char* buf, s32 bufSize);
extern void ADXERR_CallErrFunc2_(char* msg, char* buf);
// Optional observer invoked once decode parameters are configured
void (*lbl_eu_805E4F70)(ADXT_Tsvr* tsvr, s32 sfreq, s32 nchan, s32 total);
extern int ADXT_GetNumChan(ADXT_Tsvr* self);

/**
 * State 1 handler: fetch decode parameters from the decoder and configure
 * output before playback starts.
 *
 * For streamed formats (<= 1) with a pending start flag, reset the linked
 * decoder object and restart the stream unless it already reached its end.
 * Then, when the decoder reports ready (stat 2), compute the decode block
 * size from the sample rate / divisor, install loop or end traps, and push
 * sample format info to the ring buffer, volume/pan/transpose settings to
 * the server, and the sample rate to the attached amp. Stat 4 means the
 * decoder hit an unrecoverable state: advance to state 6.
 */
void adxt_stat_decinfo(ADXT_Tsvr* tsvr) {
    ADXSJD_State* sjd = tsvr->sjd;
    char numBuf[0x10];
    s32 transType = 0;
    s32 transVal = 0;
    s32 stat;

    if ((u32)tsvr->format <= 1 && tsvr->field_0xA8 == 1) {
        if (ADXSTM_GetStat(tsvr->stream) == 2) {
            // Stream already exhausted: nothing left to configure.
            return;
        }
        {
            ADXT_ChannelObj* link = tsvr->field_0x10;
            if (link != NULL) {
                link->vtable->func_0x14(link);
            }
            adxt_start_stm(tsvr, tsvr->field_0xB0, tsvr->field_0xB4,
                           tsvr->field_0xB8, tsvr->field_0xBC);
            tsvr->field_0xA8 = 0;
        }
    }

    stat = ADXSJD_GetStat(sjd);
    if (stat == 2) {
        s32 chan = ADXSJD_GetNumChan(sjd);
        if (chan > (s8)tsvr->field_0x03) {
            // More channels in the file than the handle was created for.
            ADXERR_ItoA2(chan, tsvr->field_0x03, numBuf, 0x10);
            ADXERR_CallErrFunc2_(&lbl_eu_80516A88[0x2a], numBuf);
            ADXT_Stop(tsvr);
            return;
        }

        {
            s32 sfreq = ADXSJD_GetSfreq(sjd);
            s32 numLoop = ADXSJD_GetNumLoop(sjd);
            s32 blkSmpl;

            if (ADXSJD_GetFormat(sjd) == 0xa) {
                tsvr->field_0x48 = sfreq / tsvr->field_0x38;
                blkSmpl = ADXSJD_GetBlkSmpl(sjd);
            } else {
                if (numLoop > 0) {
                    s32 d = sfreq / tsvr->field_0x38;
                    tsvr->field_0x48 = (d << 2) - d;
                } else {
                    s32 d = sfreq / tsvr->field_0x38;
                    tsvr->field_0x48 = ((d << 2) - d) / 2;
                }
                blkSmpl = ADXSJD_GetBlkSmpl(sjd) * 2;
            }
            // Round the block size up to a whole multiple of blkSmpl.
            tsvr->field_0x48 = (tsvr->field_0x48 + blkSmpl) / blkSmpl * blkSmpl;
            ADXSJD_SetMaxDecSmpl(sjd, tsvr->field_0x48);

            if (numLoop > 0) {
                if (tsvr->format != 2) {
                    s32 lpEndOfst = ADXSJD_GetLpEndOfst(sjd);
                    // Loop end byte offset rounded up to whole sectors.
                    tsvr->field_0x8C = (lpEndOfst + 0x7ff) >> 11;
                    tsvr->field_0x50 = 0x800 - ((lpEndOfst + 0x7ff) >> 11);
                    ADXSTM_SetEos(tsvr->stream, tsvr->field_0x8C);
                    ADXSTM_EntryEosFunc(tsvr->stream, adxt_eos_entry, tsvr);
                } else {
                    tsvr->field_0x50 = 0;
                }
                (void)ADXSJD_GetLpEndPos(sjd);
                tsvr->field_0x90 = ADXSJD_GetLpStartPos(sjd);
                ADXSJD_SetTrapNumSmpl(sjd, tsvr->field_0x90);
                ADXSJD_SetTrapDtLen(sjd, 0);
                ADXSJD_SetTrapCnt(sjd, 0);
                ADXSJD_EntryTrapFunc(sjd, adxt_trap_entry_lps, tsvr);
            } else {
                if (tsvr->stream != NULL) {
                    ADXSTM_SetEos(tsvr->stream, 0x7fff);
                }
                ADXSJD_SetTrapNumSmpl(sjd, ADXSJD_GetTotalNumSmpl(sjd));
                ADXSJD_SetTrapDtLen(sjd, 0);
                ADXSJD_SetTrapCnt(sjd, 0);
                ADXSJD_EntryTrapFunc(sjd, adxt_nlp_trap_entry, tsvr);
            }
        }

        {
            s32 sfreq = ADXSJD_GetSfreq(sjd);
            s32 nchan = ADXSJD_GetNumChan(sjd);
            s32 total = ADXSJD_GetTotalNumSmpl(sjd);

            ADXRNA_SetBitPerSmpl(tsvr->rna, ADXSJD_GetOutBps(sjd));
            ADXRNA_SetSfreq(tsvr->rna, sfreq);
            ADXRNA_SetNumChan(tsvr->rna, nchan);
            ADXRNA_SetTotalNumSmpl(tsvr->rna, total);
            ADXT_SetOutVol(tsvr, tsvr->field_0x40);
            ADXT_GetTranspose(tsvr, &transType, &transVal);
            if (transType != 0 || transVal != 0) {
                ADXT_SetTranspose(tsvr, transType, transVal);
            }
            if (ADXSJD_GetNumChan(tsvr->sjd) == 1) {
                ADXT_SetOutPan(tsvr, 0, tsvr->field_0x42);
            } else {
                ADXT_SetOutPan(tsvr, 0, tsvr->field_0x42);
                ADXT_SetOutPan(tsvr, 1, tsvr->field_0x44);
            }
            if (tsvr->field_0x74 != NULL) {
                ADXAMP_SetSfreq(tsvr->field_0x74, sfreq);
            }
            if (ADXSJD_GetFormat(sjd) == 2) {
                ADXRNA_SetStmHdInfo(tsvr->rna, ADXSJD_GetSpsdInfo(sjd));
            }
            ADXRNA_SetTransSw(tsvr->rna, 1);
            if (lbl_eu_805E4F70 != NULL) {
                lbl_eu_805E4F70(tsvr, sfreq, nchan, total);
            }
        }
        tsvr->state = 2;
    } else if (stat == 4) {
        tsvr->state = 6;
    }
}
extern u32 lbl_eu_805E26DC;

/**
 * State 2 handler: prepare playback.
 *
 * First, if the decoder is not keeping up (data below the capped limit and
 * room above one block) or the SJD reports playing, kick off output: unless
 * already paused/flagged, enable the ring-buffer play switch, clear the
 * start word, install the play-start data word, mark state 3 and set the
 * prepared flag.
 *
 * Then, while the SJD reports playing, zero-fill each channel's decode
 * buffer (numChan * limit * 2 bytes total) via its vtable acquire/release
 * methods, and clear the started flag.
 */
void adxt_stat_prep(ADXT_Tsvr* self) {
    ADXRNA_State* rna = self->rna;
    ADXSJD_State* sjd = self->sjd;
    u32 numData = ADXRNA_GetNumData(rna);
    u32 numRoom = ADXRNA_GetNumRoom(rna);

    // Cap the limit at 0x2000 samples.
    s32 lim = self->field_0x48 < 0x2000 ? self->field_0x48 : 0x2000;

    // Start output when the decoder has fallen behind OR the SJD is playing.
    if ((s32)numData >= lim || (s32)numRoom <= (s32)ADXSJD_GetBlkSmpl(sjd)
        || ADXSJD_GetStat(self->sjd) == 3) {
        // All paths set flag_0x71; non-paused paths also advance to state 3.
        if (self->flag_0x70 == 0) {
            if (self->flag_0x72 == 0) {
                ADXRNA_SetPlaySw(rna, 1);
                self->field_0x9C = 0;
                self->field_0xA0 = lbl_eu_805E26DC;
            }
            self->state = 3;
        }
        self->flag_0x71 = 1;
    }

    if (ADXSJD_GetStat(self->sjd) == 3) {
        int i;
        int numChan = ADXT_GetNumChan(self);
        u32 size = (self->field_0x48 * numChan) << 1;
        for (i = 0; i < numChan; i++) {
            ADXT_ChannelObj* ch = self->channels[i];
            ADXT_BufInfo info;
            ch->vtable->func_0x18(ch, 0, size, &info);
            memset(info.data, 0, info.len);
            ch->vtable->func_0x20(ch, 1, &info);
        }
    }

    self->flag_0x73 = 0;
}

// Global variables referenced in ASM (likely debug/communication)
// These need to be declared for proper linking
u32 lbl_eu_805E4F74;
u32 lbl_eu_805E4F78;

/**
 * Handles streaming playback state machine for ADXT.
 *
 * This function manages the playback state when the ADXT state is 3 (playing).
 * It checks various conditions to determine when to start/stop audio output.
 *
 * The function uses a flag (flag_0x73) to track whether playback has been started:
 * - When flag_0x73 == 0: checks if conditions are right to start playback
 * - When flag_0x73 != 0: checks if conditions indicate we should stop
 */
void criware_80385320(ADXT_Tsvr* self) {
    // Load fields early to match register allocation pattern
    ADXRNA_State* rna = self->rna;
    ADXSJD_State* sjd = self->sjd;
    ADXSTM_State* stream = self->stream;

    // Check state first - early return if not playing (state != 3)
    if (self->state != 3) {
        return;
    }
    
    if (self->flag_0x73 == 0) {
        // Path 1: Not yet playing - check if we should start
        if (ADXT_GetNumSctIbuf(self) > 1) {
            return;
        }
        if (ADXT_GetNumSmplObuf(self, 0) >= 0x10) {
            return;
        }
        // Start playback
        ADXRNA_SetPlaySw(rna, 0);
        self->flag_0x73 = 1;
    } else {
        // Path 2: Already playing - check if we should stop
        // Calculate remaining data in stream
        u32 fileSct = ADXSTM_GetFileSct(stream);
        u32 tellPos = ADXSTM_Tell(stream);
        s32 remaining = (s32)(fileSct - tellPos);
        
        // Clamp remaining to loop end position (field_0x3E)
        s16 loopEnd = self->field_0x3E;
        if (loopEnd < remaining) {
            remaining = loopEnd;
        }
        
        // Check if we have enough input buffers (numSctIbuf > remaining)
        if (ADXT_GetNumSctIbuf(self) <= remaining) {
            return;
        }
        
        // Check if we have output buffers
        if (ADXT_GetNumSmplObuf(self, 0) <= 0) {
            return;
        }
        
        // Get ring buffer status
        u32 numData = ADXRNA_GetNumData(rna);
        u32 numRoom = ADXRNA_GetNumRoom(rna);
        
        // Apply limit cap (max 0x2000)
        s32 lim = (s32)self->field_0x48 < 0x2000 ? (s32)self->field_0x48 : 0x2000;
        
        // Only stop if there is buffer headroom and SJD status is 3
        if ((s32)numData < lim && (s32)numRoom > (s32)ADXSJD_GetBlkSmpl(sjd)) {
            if (ADXSJD_GetStat(sjd) != 3) {
                return;
            }
        }
        
        // Stop playback if flag_0x72 is 0
        if (self->flag_0x72 == 0) {
            ADXRNA_SetPlaySw(rna, 1);
            self->flag_0x73 = 0;
        }
    }
}

/**
 * Handles status updates when ADXT is in "playing" state.
 *
 * This function checks if the decoded data length has reached the threshold,
 * and if so, sets a trap for the decoder. It then checks the SJD status,
 * and if playing (status 3), iterates through channel objects to check their
 * status via virtual function calls. If all channels report status < 0x40,
 * it transitions the state machine by setting the trans switch and updating state.
 *
 * @param self Pointer to the ADXT server struct
 */
void adxt_stat_playing(ADXT_Tsvr* self) {
    // Check loop flag - if set, skip trap setting
    if (self->loopFlag == 0) {
        // Check threshold - if zero, skip
        if (self->threshold != 0) {
            u32 decLen = ADXSJD_GetDecDtLen(self->sjd);
            // Use explicit unsigned comparison to match retail cmpl
            if (!((int)decLen < (int)self->threshold)) {
                ADXSJD_SetTrapNumSmpl(self->sjd, -1);
            }
        }
    }

    // Get SJD status
    int stat = ADXSJD_GetStat(self->sjd);
    
    if (stat == 3) {
        // Get number of channels
        int numChan = ADXSJD_GetNumChan(self->sjd);
        lbl_eu_805E4F74 = numChan;  // Store to global (debug/communication)
        
        // Iterate through channel array
        int i;
        for (i = 0; i < numChan; i++) {
            ADXT_ChannelObj* chanObj = self->channels[i];
            
            // Call virtual function at vtable offset 0x24 (func_0x24)
            int result = chanObj->vtable->func_0x24(chanObj, 1);
            lbl_eu_805E4F78 = result;  // Store to global (debug/communication)
            
            // If result >= 0x40, break early (channel needs attention)
            if (result >= 0x40) {
                break;
            }
        }
        
        // If all channels processed (none returned >= 0x40)
        if (i == numChan) {
            // Disable data transfer and transition state
            ADXRNA_SetTransSw(self->rna, 0);
            self->state = 4;  // Transition to next state
        }
    }
    
    // Call next state handler
    criware_80385320(self);
}

// Error message data referenced by the null-handle error path

u32 lbl_eu_805E4F7C;
extern int ADXERR_CallErrFunc1_(char* msg);
extern int ADXT_GetStat(ADXT_Tsvr* self);
extern int ADXSTM_GetStat(ADXSTM_State* stm);
extern int LSC_GetStat(LSC_State* lsc);
extern void ADXSJD_TermSupply(ADXSJD_State* sjd);
extern void adxt_stat_playing(ADXT_Tsvr* self);

/**
 * Streaming server state-machine dispatcher.
 *
 * Runs one step of the ADXT handle state machine: dispatches on the current
 * state byte (field_0x01), handles the end-of-data transition for state 4,
 * then terminates supply / stops playback when the stream or LSC reports
 * completion.
 */
void adxt_ExecHndl(ADXT_Tsvr* hndl) {
    if (hndl == NULL) {
        ADXERR_CallErrFunc1_(&lbl_eu_80516A88[0x69]);
        return;
    }

    if (hndl->state == 3) {
        adxt_stat_playing(hndl);
    } else if (hndl->state == 1) {
        adxt_stat_decinfo(hndl);
    } else if (hndl->state == 2) {
        adxt_stat_prep(hndl);
    } else if (hndl->state == 4) {
        // All stream data consumed: record final size, and once the ring
        // buffer drains, stop playback and move to the end state.
        lbl_eu_805E4F7C = ADXRNA_GetNumData(hndl->rna);
        if ((int)ADXRNA_GetNumData(hndl->rna) <= 0) {
            ADXRNA_SetPlaySw(hndl->rna, 0);
            hndl->state = 5;
        }
    }

    // Terminate decoder supply depending on format while stream exists
    if (hndl->stream != NULL && ADXT_GetStat(hndl) != 0) {
        s8 fmt = hndl->format;
        switch (fmt) {
        case 0:
        case 1:
            if (ADXSTM_GetStat(hndl->stream) == 3) {
                ADXSJD_TermSupply(hndl->sjd);
            }
            break;
        case 2:
            ADXSJD_TermSupply(hndl->sjd);
            break;
        }
    }

    // Stream reached EOS: flag stop position and enter stopping state
    if (hndl->stream != NULL && ADXSTM_GetStat(hndl->stream) == 4) {
        hndl->field_0x60 = -1;
        hndl->state = 6;
    }

    // Same treatment when the LSC object reports completion
    if (hndl->lsc != NULL && LSC_GetStat(hndl->lsc) == 3) {
        hndl->field_0x60 = -1;
        hndl->state = 6;
    }
}
