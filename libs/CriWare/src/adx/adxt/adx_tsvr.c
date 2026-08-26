// Auto-scaffolded catalog TU for CriWare/src/adx/adxt/adx_tsvr
// Replace stubs with high-level C/C++ during decomp.

#include <harness_catalog.h>

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
typedef struct ADXSJD_State ADXSJD_State;
typedef struct ADXSTM_State ADXSTM_State;
typedef struct ADXRNA_State ADXRNA_State;
typedef struct ADXT_ChannelObj ADXT_ChannelObj;

// ADXT_Tsvr struct definition based on ASM analysis
typedef struct ADXT_Tsvr {
    u8  field_0x00;         // field_0x00: unknown
    s8  state;              // field_0x01: state (1=?, 3=playing, 4=done?)
    s8  format;             // field_0x02: format type
    u8  field_0x03;         // field_0x03: padding or unknown
    ADXSJD_State* sjd;      // field_0x04: sound job descriptor
    ADXSTM_State* stream;   // field_0x08: stream state
    ADXRNA_State* rna;      // field_0x0C: ring buffer/audio output
    u8  field_0x10[8];      // fields 0x10-0x17: unknown
    ADXT_ChannelObj* channels[9];  // field_0x18: array of channel/voice object pointers
    u8  field_0x3C[2];      // fields 0x3C-0x3D: padding to align field_0x3E
    s16 field_0x3E;         // field_0x3E: loop end position
    u8  field_0x40[8];      // fields 0x40-0x47: unknown
    u32 field_0x48;         // field_0x48: limit value
    u8  field_0x4C[12];     // fields 0x4C-0x57: unknown
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
    u8  field_0x74[0x1A];   // fields 0x74-0x8D: unknown
    s16 numLoop;            // field_0x8E: number of loops (signed half word)
    u8  field_0x90[4];      // fields 0x90-0x93: sample count
    struct LSC_State* lsc;  // field_0x94: LSC (loudness/sound control?) object
    u8  field_0x98[4];      // fields 0x98-0x9B: unknown
    u32 field_0x9C;         // field_0x9C: cleared on play start
    u32 field_0xA0;         // field_0xA0: callback/data word set on play start
    u8  field_0xA4[0x1C];   // fields 0xA4-0xBF: unknown
    u32 threshold;          // field_0xC0: threshold for decoded data length
} ADXT_Tsvr;

// Opaque LSC state
struct LSC_State;
typedef struct LSC_State LSC_State;

// Channel object with vtable
struct ADXT_ChannelObj {
    struct ADXT_ChannelObj_VTable* vtable;  // vtable at offset 0
};

// VTable for ADXT_ChannelObj - functions at offsets 0x18, 0x20, 0x24
struct ADXT_ChannelObj_VTable {
    void (*reserved[6])(void);  // functions at offsets 0x00-0x14 (unknown)
    void (*func_0x18)(ADXT_ChannelObj* self, int idx, u32 size, void* info);  // acquire decode buffers
    void (*func_0x20)(ADXT_ChannelObj* self, int idx, void* info);            // release/commit buffers
    int (*func_0x24)(ADXT_ChannelObj* self, int arg);  // at offset 0x24
};

// Buffer info block filled by func_0x18: data pointer + byte length.
struct ADXT_BufInfo {
    void* data;
    u32   len;
};
typedef struct ADXT_BufInfo ADXT_BufInfo;

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
void adxt_trap_entry(void);

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

void adxt_trap_entry() {}

extern s32 ADXSJD_GetLpStartOfst(void* sj);
extern void ADXSTM_Seek(void* stm, s32 pos);
extern void ADXSTM_SetEos(void* stm, s32 a);

/**
 * End-of-stream handler.
 * Queries the loop start offset up front; then depending on format/loop flag,
 * either marks the stream EOS (0x7FFFFFFF), arms a decoder trap, or seeks the
 * stream to the loop point (offset >> 11, arithmetic shift).
 */
void adxt_eos_entry(ADXT_Tsvr* tsvr) {
    ADXSTM_State* stm = tsvr->stream;
    ADXSJD_State* sjd = tsvr->sjd;
    s32 lpOfst;
    if (stm == NULL || sjd == NULL) return;
    lpOfst = ADXSJD_GetLpStartOfst(sjd);
    if (tsvr->format == 4) {
        ADXSTM_SetEos(tsvr->stream, 0x7FFFFFFF);
        return;
    }
    if (tsvr->loopFlag == 0) {
        if (!((s32)ADXSJD_GetDecDtLen(tsvr->sjd) < (s32)tsvr->threshold)) {
            ADXSJD_SetTrapNumSmpl(tsvr->sjd, -1);
        }
        ADXSTM_SetEos(tsvr->stream, 0x7FFFFFFF);
        return;
    }
    ADXSTM_Seek(stm, lpOfst / 2048);
}

void adxt_nlp_trap_entry() {}

extern void adxt_stat_decinfo(ADXT_Tsvr* self);
extern int ADXT_GetNumChan(ADXT_Tsvr* self);
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
    u32 lim = self->field_0x48 < 0x2000 ? self->field_0x48 : 0x2000;

    // Start output when the decoder has fallen behind OR the SJD is playing.
    if (!((s32)numData < (s32)lim && (s32)numRoom > (s32)ADXSJD_GetBlkSmpl(sjd))
        || ADXSJD_GetStat(sjd) == 3) {
        if (self->flag_0x70 != 0) {
            self->flag_0x71 = 1;
        } else if (self->flag_0x72 != 0) {
            self->state = 3;
            self->flag_0x71 = 1;
        } else {
            ADXRNA_SetPlaySw(rna, 1);
            self->field_0x9C = 0;
            self->field_0xA0 = lbl_eu_805E26DC;
            self->state = 3;
            self->flag_0x71 = 1;
        }
    }

    if (ADXSJD_GetStat(sjd) == 3) {
        int numChan = ADXT_GetNumChan(self);
        u32 size = (self->field_0x48 * numChan) << 1;
        int i;
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
extern u32 lbl_eu_805E4F74;
extern u32 lbl_eu_805E4F78;

/**
 * Handles streaming playback state machine for ADXT.
 *
 * This function manages the playback state when the ADXT state is 3 (playing).
 * It checks various conditions to determine when to start/stop audio output.
 *
 * The function uses a flag (flag_0x73) to track whether playback has been started:
 * - When flag_0x73 is 0: checks if conditions are right to start playback
 * - When flag_0x73 is non-zero: checks if conditions indicate we should stop
 *
 * @param self Pointer to the ADXT server struct
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
extern char lbl_eu_80516A88[];
extern u32 lbl_eu_805E4F7C;
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
