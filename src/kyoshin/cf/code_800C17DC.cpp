// Auto-scaffolded catalog TU for kyoshin/cf/code_800C17DC
// Replace stubs with high-level C/C++ during decomp.

#include <string.h>
#include <stdlib.h>
#include "PowerPC_EABI_Support/MSL_C/MSL_Common/extras.h"

#include "kyoshin/harness_catalog.hpp"
#include "kyoshin/cf/code_800C17DC.hpp"

/// Classify one decoded char stream: report whether the bytes at `str` form a
/// plain char (type 1), a two-byte sequence (type 2) or a newline/end (type 3),
/// and how many source bytes to consume.  Returns the decoded character (for a
/// two-byte char that is the combined 16-bit value).
// Shift-JIS byte classification.  A leading byte is 0x81-0x9f or 0xe0-0xef
// (warps to (c+0x7f)<=0x1e and (c+0x20)<=0xf); a trailing byte is 0x40-0x7e or
// 0x80-0xfc.  Materialised so MWCC emits the bitmath booleanize for the second
// (wrap-around) range rather than a second branch.
static int isLeadByte(unsigned char c) {
    int r;
    if ((unsigned char)(c + 0x7f) <= 0x1e) {
        r = 1;
    } else {
        r = (unsigned char)(c + 0x20) <= 0x0f;
    }
    return r;
}
static int isTrailByte(unsigned char c) {
    int r;
    if ((unsigned char)(c + 0xc0) <= 0x3e) {
        r = 1;
    } else {
        r = (unsigned char)(c + 0x80) <= 0x7c;
    }
    return r;
}

int CmText_DecodeSjisChar(const char* str, int* type, int* len) {
    unsigned char c = (unsigned char)str[0];
    if (isLeadByte(c)) {
        if (isTrailByte((unsigned char)str[1])) {
            *len = 2;
            *type = 2;
            return ((unsigned char)str[0] << 8) | (unsigned char)str[1];
        }
    }
    if ((unsigned char)c == (unsigned char)'\n') {
        *len = 1;
        *type = 3;
        return '\n';
    }
    if ((unsigned char)c == (unsigned char)'\r' && (unsigned char)str[1] == (unsigned char)'\n') {
        *len = 2;
        *type = 3;
        return '\n';
    }
    if (c == 0) {
        *len = 1;
        *type = 3;
        return 0;
    }
    *len = 1;
    *type = 1;
    return (unsigned char)str[0];
}

/// Read the next whitespace-delimited word out of the cursor at `self->cursor`
/// into `out`, stopping at a newline or when `maxLen` characters have been
/// consumed.  Returns 1 if something was consumed, 0 if the cursor is empty.
int CmText_ReadWord(CmTextProc* self, char* out, int maxLen) {
    char* o = out;
    if ((u8)self->cursor[0] == 0) {
        out[0] = 0;
        return 0;
    }
    int pos = 0;
    int result = 1;
    while ((u8)self->cursor[0] != 0) {
        int type, lenz;
        u32 ch = CmText_DecodeSjisChar(self->cursor, &type, &lenz);
        pos += lenz;
        if (pos >= maxLen) {
            result = 0;
            break;
        }
        if (type == 1) {
            const char* cur = self->cursor;
            o[0] = cur[0];
            o++;
            self->cursor = cur + 1;
        } else if (type == 2) {
            const char* cur = self->cursor;
            o[0] = cur[0];
            self->cursor = cur + 1;
            o[1] = (cur + 1)[0];
            self->cursor = cur + 2;
            o += 2;
        } else if (type == 3) {
            if ((u16)ch == 0) break;
            self->cursor += lenz;
            break;
        }
    }
    o[0] = 0;
    return result;
}

/// Split a cue-script line into words, storing a pointer to each word start in
/// `out`.  Words begin at a letter (A-Z/a-z) or a two-byte char and are
/// separated by spaces/tabs; `#` and `//` start a comment that ends the line.
/// The source buffer is modified in place (separators become NUL).  Returns the
/// number of words.  (`maxLen` is accepted for ABI/register setup but unused.)
int func_800C1900(char* str, char** out, int maxLen) {
    int count = 0;
    char* s = str;
    char** o = out;
    int inWord = 0, offset = 0;
    while ((u8)s[0] != 0) {
        int type, lenz;
        int ch = CmText_DecodeSjisChar(s, &type, &lenz);
        if (type == 1) {
            if ((u16)ch == '#') break;
            if ((u16)ch == '/' && (u8)s[1] == '/') break;
            u16 c = (u16)ch;
            if (c == ' ' || c == '\t') {
                if (inWord) s[0] = 0;
                inWord = 0;
            } else if ((c >= 'A' && c <= 'Z') || (c >= 'a' && c <= 'z')) {
                if (!inWord) {
                    out[offset] = s;
                    inWord = 1;
                    count++;
                    offset++;
                }
            }
        } else if (type == 2) {
            if (!inWord) {
                out[offset] = s;
                inWord = 1;
                count++;
                offset++;
            }
        } else {
            break;
        }
        s += lenz;
    }
    s[0] = 0;
    return count;
}

/// Register a (filename-hash, id, seq) entry into the table set selected by
/// `key` (0-5). `name` is hashed via Tok_ParseFile; `value` (optional) is an
/// atoi-able id.  `key==0` writes the single-entry base set.
///
/// Known residual vs retail (us-800c2578, HIGH_MATCH): retail reloads
/// `self->buf` inside every key arm (per-branch `lwz`) and folds the key==0
/// hash store into `stwux r31,r3,r5`; our hoisted `tbl` local keeps the size
/// gate green but leaves that arm as `stwx`+`add`. The correct per-branch
/// shape costs +4 TU bytes and fails split-size, so the hoisted form stays.
void CmText_RegisterEntry(CmTextProc* self, int key, const char* name, const char* value) {
    u32 hash = Tok_ParseFile(name);
    if (hash == 0) return;
    u16 seq = self->buf->cntAll;
    CmTextTable* tbl = self->buf;
    u16 id = 0;
    if (value != 0) id = (u16)atoi(value);
    if (key == 0) {
        u16 i = tbl->cnt0++;
        tbl->set0[i].hash = hash;
        tbl->set0[i].field4 = id;
        tbl->set0[i].field6 = seq;
    } else if (key == 1) {
        u16 i = tbl->cnt1++;
        tbl->set1[i].hash = hash;
        tbl->set1[i].field4 = id;
        tbl->set1[i].field6 = seq;
    } else if (key == 2) {
        u16 i = tbl->cnt2++;
        tbl->set2[i].hash = hash;
        tbl->set2[i].field4 = id;
        tbl->set2[i].field6 = seq;
    } else if (key == 3) {
        u16 i = tbl->cnt3++;
        tbl->set3[i].hash = hash;
        tbl->set3[i].field4 = id;
        tbl->set3[i].field6 = seq;
    } else if (key == 4) {
        u16 i = tbl->cnt4++;
        tbl->set4[i].hash = hash;
        tbl->set4[i].field4 = id;
        tbl->set4[i].field6 = seq;
    } else if (key == 5) {
        u16 i = tbl->cnt5++;
        tbl->set5[i].hash = hash;
        tbl->set5[i].field4 = id;
        tbl->set5[i].field6 = seq;
    }
}

/// Parse a cue-script text buffer into the table owned by `self`.  Each line is
/// a keyword token followed by up to two parameters (`KEYWORD name value`); the
/// keyword must appear in the lbl_eu_8052A528 dispatch table.  `buf` receives
/// the 0x628-byte table that gets filled.
void CmText_ParseTextBuffer(CmTextProc* self, const char* text, CmTextTable* buf) {
    if (buf == 0) return;
    self->buf = buf;
    memset(self->buf, 0, 0x628);
    self->base = text;
    self->cursor = text;
    char* names[0x10];
    for (;;) {
        // Token buffer sits high in the frame in retail.
        char token[0x100];
        if (!CmText_ReadWord(self, token, 0xff)) return;
        // Comment markers: '#' starts a comment, as does '//'.
        if ((int)(u8)token[0] == 0x23) continue;
        if ((int)(u8)token[0] == 0x2f && (int)(u8)token[1] == 0x2f) continue;
        names[2] = 0;
        names[1] = 0;
        if (!func_800C1900(token, names, 0x10)) continue;
        // Look the keyword up in the dispatch table; unknown keywords are skipped.
        int id = -1;
        for (u32 i = 0; i < 6; i++) {
            if (stricmp(names[0], lbl_eu_8052A528[i].word) == 0) {
                id = (int)lbl_eu_8052A528[i].id;
                break;
            }
        }
        if (id == -1) continue;
        if (id != 5) self->buf->cntAll++;
        CmText_RegisterEntry(self, id, names[1], names[2]);
    }
}

// Routes to CmText_ParseTextBuffer with the global flag record as the leading
// argument and passes its two arguments through unchanged.
extern "C" void CmText_ParseWithStaticProc(u32 arg0, u32 arg1) {
    CmText_ParseTextBuffer((CmTextProc*)&lbl_eu_805739E8, (const char*)arg0, (CmTextTable*)arg1);
}

// Register a callback into the table slot selected by field20 and set the
// continue flag that the dispatch loop CmText_DispatchCallbacks checks.
extern "C" void CmText_RegisterCallback(void (*callback)(void), u8 flag) {
    lbl_eu_805739F8.fns[lbl_eu_805739F8.field20] = callback;
    lbl_eu_805739F8.field24 = flag;
}

// Variadic prologue dump: on entry MWCC spills every register-argument into
// this frame so callers can read them back out of memory. Empty body.
extern "C" void CmText_VariadicNoop(...) {}

// Default no-op callback stored in the callback table at lbl_eu_805739F8.
// Called by CmText_DispatchCallbacks when it iterates the callback array and a slot has no
// registered handler.  The empty body corresponds to a single blr instruction.
extern "C" void CmText_DefaultNoopCallback(void) {}

// (Re)initialise the callback table: clear every slot, reset the active index
// and flag, then install the default no-op callback in slot 0.
extern "C" void CmText_InitCallbackTable() {
    lbl_eu_805739F8.field20 = 0;
    lbl_eu_805739F8.field24 = 0;
    memset(&lbl_eu_805739F8, 0, 0x20);
    lbl_eu_805739F8.fns[lbl_eu_805739F8.field20] = CmText_DefaultNoopCallback;
}

// Dispatch loop: while the active callback slot is populated, clear the
// continue flag, invoke the callback, and keep iterating if the callback
// re-set the flag.
extern "C" void CmText_DispatchCallbacks() {
    if (lbl_eu_805739F8.fns[lbl_eu_805739F8.field20] != 0) {
        do {
            lbl_eu_805739F8.field24 = 0;
            lbl_eu_805739F8.fns[lbl_eu_805739F8.field20]();
        } while (lbl_eu_805739F8.field24 != 0);
    }
}

// Default no-op callback stored in the callback table at lbl_eu_805739F8.
// Called by CmText_DispatchCallbacks when it iterates the callback array and a slot has no
// registered handler.  The empty body corresponds to a single blr instruction.
extern "C" void sinit_800C1F2C() {
    lbl_eu_805739F8.field20 = 0;
    lbl_eu_805739F8.field24 = 0;
}
// ---------------------------------------------------------------------------

// ---------------------------------------------------------------------------
// Retail .bss (0x38): the parser-state record and the 0x28 callback table,
// both already typed in include/kyoshin/cf/code_800C17DC.hpp.
Unk805739E8 lbl_eu_805739E8;
Unk805739F8 lbl_eu_805739F8;

// ---------------------------------------------------------------------------
// Retail .sdata2 (0x28): the six scenario-keyword strings used by the
// lbl_eu_8052A528 dispatch table.  Offsets follow retail
// (0x00/0x04/0x08/0x0C/0x14/0x1C), so the two strings after the 7-byte
// "effect" get an explicit 4-byte alignment; retail's .sdata2 extent is 0x28.
__declspec(section ".sdata2") char lbl_eu_80666B58[4] = "chr";
__declspec(section ".sdata2") char lbl_eu_80666B5C[4] = "anm";
__declspec(section ".sdata2") char lbl_eu_80666B60[4] = "obj";
__declspec(section ".sdata2") char lbl_eu_80666B64[7] = "effect";
__declspec(section ".sdata2") __attribute__((aligned(4))) char lbl_eu_80666B6C[6] = "mapse";
__declspec(section ".sdata2") __attribute__((aligned(4))) char lbl_eu_80666B74[5] = "pack";

// ---------------------------------------------------------------------------
// Retail .data owned by this TU: nine pointer tables (CfObjectMove's vtable
// plus per-class dispatch tables) whose retail image is 909 R_PPC_ADDR32
// relocations over zero bytes -- objcopy shows zeros because the linker
// applies them.  Slots are spelled as pointer initialisers so MWCC emits the
// same ADDR32 relocs (CSysWinSave.cpp precedent); non-relocated words keep
// their retail immediates.  Target symbol spellings are the retail linker
// names (C linkage) -- the mangled-looking ones are the exact symbol names.

extern "C" {
void CAIAction_UnkVirtualFunc1__Q22cf9CAIActionFv();
void CAIAction_UnkVirtualFunc2__Q22cf9CAIActionFv();
void CActorParam_UnkVirtualFunc101__Q22cf11CActorParamFv();
void CActorParam_UnkVirtualFunc102__Q22cf11CActorParamFv();
void CActorParam_UnkVirtualFunc103__Q22cf11CActorParamFv();
void CActorParam_UnkVirtualFunc104__Q22cf11CActorParamFv();
void CActorParam_UnkVirtualFunc105__Q22cf11CActorParamFv();
void CActorParam_UnkVirtualFunc106__Q22cf11CActorParamFv();
void CActorParam_UnkVirtualFunc107__Q22cf11CActorParamFv();
void CActorParam_UnkVirtualFunc108__Q22cf11CActorParamFv();
void CActorParam_UnkVirtualFunc109__Q22cf11CActorParamFv();
void CActorParam_UnkVirtualFunc110__Q22cf11CActorParamFv();
void CActorParam_UnkVirtualFunc111__Q22cf11CActorParamFv();
void CActorParam_UnkVirtualFunc112__Q22cf11CActorParamFv();
void CActorParam_UnkVirtualFunc113__Q22cf11CActorParamFv();
void CActorParam_UnkVirtualFunc114__Q22cf11CActorParamFv();
void CActorParam_UnkVirtualFunc115__Q22cf11CActorParamFv();
void CActorParam_UnkVirtualFunc117__Q22cf11CActorParamFv();
void CActorParam_UnkVirtualFunc118__Q22cf11CActorParamFv();
void CActorParam_UnkVirtualFunc119__Q22cf11CActorParamFv();
void CActorParam_UnkVirtualFunc120__Q22cf11CActorParamFv();
void CActorParam_UnkVirtualFunc121__Q22cf11CActorParamFv();
void CActorParam_UnkVirtualFunc122__Q22cf11CActorParamFv();
void CActorParam_UnkVirtualFunc123__Q22cf11CActorParamFv();
void CActorParam_UnkVirtualFunc124__Q22cf11CActorParamFv();
void CActorParam_UnkVirtualFunc128__Q22cf11CActorParamFv();
void CActorParam_UnkVirtualFunc130__Q22cf11CActorParamFv();
void CActorParam_UnkVirtualFunc131__Q22cf11CActorParamFv();
void CActorParam_UnkVirtualFunc134__Q22cf11CActorParamFv();
void CActorParam_UnkVirtualFunc136__Q22cf11CActorParamFv();
void CActorParam_UnkVirtualFunc137__Q22cf11CActorParamFv();
void CActorParam_UnkVirtualFunc139__Q22cf11CActorParamFv();
void CActorParam_UnkVirtualFunc143__Q22cf11CActorParamFv();
void CActorParam_UnkVirtualFunc145__Q22cf11CActorParamFv();
void CActorParam_UnkVirtualFunc146__Q22cf11CActorParamFv();
void CActorParam_UnkVirtualFunc147__Q22cf11CActorParamFv();
void CActorParam_UnkVirtualFunc148__Q22cf11CActorParamFv();
void CActorParam_UnkVirtualFunc151__Q22cf11CActorParamFv();
void CActorParam_UnkVirtualFunc155__Q22cf11CActorParamFv();
void CActorParam_UnkVirtualFunc158__Q22cf11CActorParamFv();
void CActorParam_UnkVirtualFunc159__Q22cf11CActorParamFv();
void CActorParam_UnkVirtualFunc15__Q22cf11CActorParamFv();
void CActorParam_UnkVirtualFunc161__Q22cf11CActorParamFv();
void CActorParam_UnkVirtualFunc162__Q22cf11CActorParamFv();
void CActorParam_UnkVirtualFunc163__Q22cf11CActorParamFv();
void CActorParam_UnkVirtualFunc166__Q22cf10CfObjectPcFv();
void CActorParam_UnkVirtualFunc167__Q22cf10CfObjectPcFv();
void CActorParam_UnkVirtualFunc168__Q22cf11CActorParamFv();
void CActorParam_UnkVirtualFunc169__Q22cf11CActorParamFv();
void CActorParam_UnkVirtualFunc170__Q22cf11CActorParamFv();
void CActorParam_UnkVirtualFunc171__Q22cf11CActorParamFv();
void CActorParam_UnkVirtualFunc172__Q22cf11CActorParamFv();
void CActorParam_UnkVirtualFunc173__Q22cf10CfObjectPcFv();
void CActorParam_UnkVirtualFunc174__Q22cf11CActorParamFv();
void CActorParam_UnkVirtualFunc175__Q22cf11CActorParamFv();
void CActorParam_UnkVirtualFunc176__Q22cf10CfObjectPcFv();
void CActorParam_UnkVirtualFunc177__Q22cf11CActorParamFv();
void CActorParam_UnkVirtualFunc178__Q22cf10CfObjectPcFv();
void CActorParam_UnkVirtualFunc179__Q22cf13CfObjectActorFv();
void CActorParam_UnkVirtualFunc17__Q22cf11CActorParamFv();
void CActorParam_UnkVirtualFunc180__Q22cf13CfObjectActorFv();
void CActorParam_UnkVirtualFunc181__Q22cf11CActorParamFv();
void CActorParam_UnkVirtualFunc18__Q22cf11CActorParamFv();
void CActorParam_UnkVirtualFunc1__Q22cf13CfObjectActorFv();
void CActorParam_UnkVirtualFunc20__Q22cf11CActorParamFv();
void CActorParam_UnkVirtualFunc22__Q22cf11CActorParamFv();
void CActorParam_UnkVirtualFunc24__Q22cf11CActorParamFv();
void CActorParam_UnkVirtualFunc25__Q22cf11CActorParamFv();
void CActorParam_UnkVirtualFunc27__Q22cf11CActorParamFv();
void CActorParam_UnkVirtualFunc28__Q22cf11CActorParamFv();
void CActorParam_UnkVirtualFunc2__Q22cf13CfObjectActorFv();
void CActorParam_UnkVirtualFunc30__Q22cf11CActorParamFv();
void CActorParam_UnkVirtualFunc31__Q22cf11CActorParamFv();
void CActorParam_UnkVirtualFunc32__Q22cf11CActorParamFv();
void CActorParam_UnkVirtualFunc36__Q22cf11CActorParamFv();
void CActorParam_UnkVirtualFunc39__Q22cf11CActorParamFv();
void CActorParam_UnkVirtualFunc3__Q22cf13CfObjectActorFv();
void CActorParam_UnkVirtualFunc41__Q22cf11CActorParamFv();
void CActorParam_UnkVirtualFunc42__Q22cf11CActorParamFv();
void CActorParam_UnkVirtualFunc43__Q22cf11CActorParamFv();
void CActorParam_UnkVirtualFunc44__Q22cf11CActorParamFv();
void CActorParam_UnkVirtualFunc45__Q22cf11CActorParamFv();
void CActorParam_UnkVirtualFunc49__Q22cf11CActorParamFv();
void CActorParam_UnkVirtualFunc50__Q22cf11CActorParamFv();
void CActorParam_UnkVirtualFunc51__Q22cf11CActorParamFv();
void CActorParam_UnkVirtualFunc53__Q22cf11CActorParamFv();
void CActorParam_UnkVirtualFunc56__Q22cf11CActorParamFv();
void CActorParam_UnkVirtualFunc57__Q22cf11CActorParamFv();
void CActorParam_UnkVirtualFunc58__Q22cf11CActorParamFv();
void CActorParam_UnkVirtualFunc59__Q22cf11CActorParamFv();
void CActorParam_UnkVirtualFunc62__Q22cf11CActorParamFv();
void CActorParam_UnkVirtualFunc63__Q22cf11CActorParamFv();
void CActorParam_UnkVirtualFunc64__Q22cf11CActorParamFv();
void CActorParam_UnkVirtualFunc66__Q22cf11CActorParamFv();
void CActorParam_UnkVirtualFunc69__Q22cf11CActorParamFv();
void CActorParam_UnkVirtualFunc71__Q22cf11CActorParamFv();
void CActorParam_UnkVirtualFunc72__Q22cf11CActorParamFv();
void CActorParam_UnkVirtualFunc73__Q22cf11CActorParamFv();
void CActorParam_UnkVirtualFunc75__Q22cf11CActorParamFv();
void CActorParam_UnkVirtualFunc77__Q22cf11CActorParamFv();
void CActorParam_UnkVirtualFunc78__Q22cf11CActorParamFv();
void CActorParam_UnkVirtualFunc79__Q22cf11CActorParamFv();
void CActorParam_UnkVirtualFunc80__Q22cf11CActorParamFv();
void CActorParam_UnkVirtualFunc84__Q22cf11CActorParamFv();
void CActorParam_UnkVirtualFunc87__Q22cf11CActorParamFv();
void CActorParam_UnkVirtualFunc88__Q22cf10CfObjectPcFv();
void CActorParam_UnkVirtualFunc89__Q22cf11CActorParamFv();
void CActorParam_UnkVirtualFunc91__Q22cf11CActorParamFv();
void CActorParam_UnkVirtualFunc92__Q22cf11CActorParamFv();
void CActorParam_UnkVirtualFunc93__Q22cf11CActorParamFv();
void CActorParam_UnkVirtualFunc97__Q22cf11CActorParamFv();
void CActorParam_UnkVirtualFunc98__Q22cf11CActorParamFv();
void CActorParam_UnkVirtualFunc99__Q22cf11CActorParamFv();
void CActorParam_UnkVirtualFunc9__Q22cf11CActorParamFv();
void CActorParam_accumulateTension__Q22cf11CActorParamFv();
void CActorParam_addAccumCurrency__Q22cf11CActorParamFUl();
void CActorParam_addArtsGauge__Q22cf11CActorParamFv();
void CActorParam_addGauge__Q22cf13CfObjectActorFv();
void CActorParam_addHate__Q22cf13CfObjectActorFv();
void CActorParam_addHp__Q22cf13CfObjectActorFv();
void CActorParam_addSecondCurrency__Q22cf11CActorParamFUl();
void CActorParam_addSecondGauge__Q22cf13CfObjectActorFv();
void CActorParam_addSpentCurrency__Q22cf11CActorParamFUl();
void CActorParam_applyDamage__Q22cf13CfObjectActorFv();
void CActorParam_applyEventStatus__Q22cf11CActorParamFv();
void CActorParam_applyStatusHeal__Q22cf11CActorParamFv();
void CActorParam_applyStoredArtsGauge__Q22cf11CActorParamFv();
void CActorParam_applyStoredHitCharge__Q22cf11CActorParamFv();
void CActorParam_applyStoredHp__Q22cf11CActorParamFv();
void CActorParam_applyTargetEvent__Q22cf11CActorParamFv();
void CActorParam_commitArtsStatus__Q22cf11CActorParamFv();
void CActorParam_findGaugeEntry__Q22cf11CActorParamFv();
void CActorParam_findMaxGaugeEntry__Q22cf11CActorParamFv();
void CActorParam_findMinGaugeEntry__Q22cf11CActorParamFv();
void CActorParam_getActorLevel__Q22cf11CActorParamFv();
void CActorParam_getActorModeFlags__Q22cf11CActorParamFv();
void CActorParam_getActorType__Q22cf11CActorParamFv();
void CActorParam_getArtsDataBlock__Q22cf11CActorParamFv();
void CActorParam_getArtsLevel__Q22cf11CActorParamFv();
void CActorParam_getArtsSlotIds__Q22cf11CActorParamFv();
void CActorParam_getArtsSyncBlock__Q22cf11CActorParamFv();
void CActorParam_getBattleHitFlags__Q22cf11CActorParamFv();
void CActorParam_getBattleParams__Q22cf11CActorParamFv();
void CActorParam_getBattleStats__Q22cf11CActorParamFv();
void CActorParam_getCopyParam__Q22cf11CActorParamFv();
void CActorParam_getCurrencyBalance__Q22cf10CfObjectPcFv();
void CActorParam_getDamageScale__Q22cf11CActorParamFv();
void CActorParam_getHp__Q22cf11CActorParamFv();
void CActorParam_getMoveRate__Q22cf11CActorParamFv();
void CActorParam_getMoveRecord__Q22cf11CActorParamFv();
void CActorParam_getScale__Q22cf13CfObjectActorFv();
void CActorParam_getSpentCurrency__Q22cf11CActorParamFv();
void CActorParam_getStatsSlot__Q22cf11CActorParamFv();
void CActorParam_getStatusCount__Q22cf11CActorParamFv();
void CActorParam_getStatusSyncBlock__Q22cf11CActorParamFv();
void CActorParam_getStatusTable__Q22cf11CActorParamFv();
void CActorParam_getTensionStatus__Q22cf11CActorParamFv();
void CActorParam_initStatusCounter__Q22cf11CActorParamFv();
void CActorParam_isBattleLocked__Q22cf11CActorParamFv();
void CActorParam_raiseTensionValue__Q22cf11CActorParamFv();
void CActorParam_reinitBattleStats__Q22cf11CActorParamFv();
void CActorParam_resetAllGaugeEntries__Q22cf11CActorParamFv();
void CActorParam_resetArtsMatrix__Q22cf11CActorParamFv();
void CActorParam_resetArtsStatus__Q22cf10CfObjectPcFv();
void CActorParam_resetArtsStatus__Q22cf13CfObjectActorFv();
void CActorParam_resetGaugeBattleEntry__Q22cf11CActorParamFv();
void CActorParam_resetGaugeEntry__Q22cf11CActorParamFv();
void CActorParam_resetGaugesAndRefresh__Q22cf11CActorParamFv();
void CActorParam_resetSecondGaugeBattleEntry__Q22cf11CActorParamFv();
void CActorParam_resetTensionState__Q22cf11CActorParamFv();
void CActorParam_setGaugeFlagA__Q22cf11CActorParamFUs();
void CActorParam_setGaugeFlagB__Q22cf11CActorParamFUs();
void CActorParam_setGaugeFloat__Q22cf11CActorParamFf();
void CActorParam_setGaugeRateA__Q22cf11CActorParamFf();
void CActorParam_setGaugeRateB__Q22cf11CActorParamFf();
void CActorParam_setHp__Q22cf13CfObjectActorFv();
void CActorParam_setMoveRate__Q22cf11CActorParamFf();
void CActorParam_setScale__Q22cf13CfObjectActorFv();
void CActorParam_setSpentCurrency__Q22cf11CActorParamFUl();
void CActorParam_setStatByte__Q22cf11CActorParamFUc();
void CActorParam_setStatScale__Q22cf11CActorParamFf();
void CActorParam_setTensionPoints__Q22cf11CActorParamFv();
void CActorParam_setupActorState__Q22cf11CActorParamFv();
void CActorState_setObjStateBit3__Q22cf11CActorStateFv();
void CBattleState_UnkVirtualFunc12__Q22cf12CBattleStateFv();
void CBattleState_UnkVirtualFunc14__Q22cf12CBattleStateFv();
void CBattleState_UnkVirtualFunc15__Q22cf12CBattleStateFv();
void CBattleState_UnkVirtualFunc16__Q22cf12CBattleStateFv();
void CBattleState_UnkVirtualFunc19__Q22cf12CBattleStateFv();
void CBattleState_UnkVirtualFunc30__Q22cf12CBattleStateFv();
void CBattleState_UnkVirtualFunc32__Q22cf12CBattleStateFv();
void CBattleState_UnkVirtualFunc4__Q22cf12CBattleStateFv();
void CBattleState_UnkVirtualFunc9__Q22cf12CBattleStateFv();
void CBattleState_addStagingEntry__Q22cf12CBattleStateFPQ22cf17CBattleStateEntry();
void CBattleState_applyArtsTable__Q22cf12CBattleStateFPCQ22cf20CBattleStateSrcEntry();
void CBattleState_applyEventEntry__Q22cf12CBattleStateFPQ22cf17CBattleStateEntry();
void CBattleState_callOwnerObject__Q22cf13CfObjectActorFv();
void CBattleState_clearEntriesByMask__Q22cf12CBattleStateFUl();
void CBattleState_clearStagingEntries__Q22cf12CBattleStateFv();
void CBattleState_clearStatusId__Q22cf12CBattleStateFv();
void CBattleState_copyStatusEntries__Q22cf12CBattleStateFv();
void CBattleState_enterStatusEntry__Q22cf12CBattleStateFPQ22cf17CBattleStateEntry();
void CBattleState_fetchStatusEntry__Q22cf12CBattleStateFv();
void CBattleState_getEntryArray__Q22cf12CBattleStateFv();
void CBattleState_getEntryByIndex__Q22cf12CBattleStateFv();
void CBattleState_getEventEntry__Q22cf12CBattleStateFv();
void CBattleState_getEventMask__Q22cf12CBattleStateFUl();
void CBattleState_getLinkedActorId__Q22cf13CfObjectActorFv();
void CBattleState_getOwner__Q22cf13CfObjectActorFv();
void CBattleState_getParamPtrName__Q22cf13CfObjectActorFv();
void CBattleState_getStagingRecord__Q22cf12CBattleStateFv();
void CBattleState_getStatusBlock__Q22cf12CBattleStateFv();
void CBattleState_getStatusEntry__Q22cf12CBattleStateFv();
void CBattleState_getStatusMask__Q22cf12CBattleStateFUl();
void CBattleState_getStatusSlot__Q22cf12CBattleStateFi();
void CBattleState_notifyEntryUpdated__Q22cf13CfObjectActorFv();
void CBattleState_removeKeyedEntries__Q22cf12CBattleStateFPQ22cf17CBattleStateEntry();
void CObjectParam_UnkVirtualFunc1__Q22cf12CObjectParamFv();
void CObjectParam_UnkVirtualFunc2__Q22cf12CObjectParamFv();
void CObjectParam_UnkVirtualFunc2__Q22cf12CfObjectMoveFv();
void CObjectParam_UnkVirtualFunc4__Q22cf12CObjectParamFv();
void CObjectParam_callPcFunc800C0__Q22cf10CfObjectPcFv();
void CObjectParam_getParamPtr__Q22cf13CfObjectActorFv();
void CObjectParam_getSelfObjectId__Q22cf12CObjectParamFv();
void CObjectParam_hasObjectName__Q22cf12CObjectParamFv();
void CObjectParam_signalActionEnd__Q22cf12CObjectParamFv();
void CObjectState_UnkVirtualFunc8__Q22cf12CObjectStateFv();
void CObjectState_UnkVirtualFunc9__Q22cf12CObjectStateFv();
void CObjectState_applyStateFlags__Q22cf12CObjectStateFv();
void CObjectState_checkStateFlags__Q22cf12CObjectStateFv();
void CObjectState_clearStateFlags8__Q22cf12CObjectStateFv();
void CObjectState_clearStateWord4__Q22cf12CObjectStateFv();
void CObjectState_clearStateWord8__Q22cf12CObjectStateFv();
void CObjectState_getStateData__Q22cf12CObjectStateFv();
void CObjectState_setStateBitFlag__Q22cf12CObjectStateFv();
void CObjectState_setStateBitMask0__Q22cf12CObjectStateFv();
void CObjectState_setStateBitMask2__Q22cf12CObjectStateFv();
void CObjectState_setStateBitMask3__Q22cf12CObjectStateFv();
void CObjectState_setStateBitMask__Q22cf12CObjectStateFv();
void CfObjectActor_clearActionSrcId__Q22cf13CfObjectActorFv();
void CfObjectActor_clearStatusPair__Q22cf13CfObjectActorFv();
void CfObjectActor_flushStatusPair__Q22cf13CfObjectActorFv();
void CfObjectActor_getActionSrcId__Q22cf13CfObjectActorFv();
void CfObjectActor_getAdjustedFacing__Q22cf13CfObjectActorFv();
void CfObjectActor_hasActorScale__Q22cf13CfObjectActorFv();
void CfObjectActor_pushRefreshValue__Q22cf13CfObjectActorFv();
void CfObjectActor_readActionScale__Q22cf13CfObjectActorFv();
void CfObjectActor_readFacingAngle__Q22cf13CfObjectActorFv();
void CfObjectActor_sharesMoveFlags__Q22cf13CfObjectActorFv();
void CfObjectActor_storeActionSrcId__Q22cf13CfObjectActorFv();
void CfObjectActor_takeDamageValue__Q22cf13CfObjectActorFv();
void CfObjectModel_UnkVirtualFunc10__Q22cf13CfObjectModelFv();
void CfObjectModel_UnkVirtualFunc11__Q22cf13CfObjectModelFv();
void CfObjectModel_UnkVirtualFunc12__Q22cf13CfObjectModelFv();
void CfObjectModel_UnkVirtualFunc13__Q22cf13CfObjectModelFv();
void CfObjectModel_UnkVirtualFunc14__Q22cf13CfObjectModelFv();
void CfObjectModel_UnkVirtualFunc15__Q22cf13CfObjectModelFv();
void CfObjectModel_UnkVirtualFunc16__Q22cf13CfObjectModelFv();
void CfObjectModel_UnkVirtualFunc17__Q22cf13CfObjectModelFv();
void CfObjectModel_UnkVirtualFunc18__Q22cf12CfObjectMoveFv();
void CfObjectModel_UnkVirtualFunc19__Q22cf12CfObjectMoveFv();
void CfObjectModel_UnkVirtualFunc1__Q22cf12CfObjectMoveFv();
void CfObjectModel_UnkVirtualFunc20__Q22cf13CfObjectModelFv();
void CfObjectModel_UnkVirtualFunc2__Q22cf12CfObjectMoveFv();
void CfObjectModel_UnkVirtualFunc3__Q22cf13CfObjectModelFv();
void CfObjectModel_UnkVirtualFunc4__Q22cf12CfObjectMoveFv();
void CfObjectModel_UnkVirtualFunc5__Q22cf12CfObjectMoveFv();
void CfObjectModel_UnkVirtualFunc6__Q22cf12CfObjectMoveFv();
void CfObjectModel_UnkVirtualFunc7__Q22cf13CfObjectModelFv();
void CfObjectModel_UnkVirtualFunc8__Q22cf13CfObjectModelFv();
void CfObjectModel_UnkVirtualFunc9__Q22cf13CfObjectModelFv();
void CfObjectMove_UnkVirtualFunc10__Q22cf12CfObjectMoveFv();
void CfObjectMove_UnkVirtualFunc15__Q22cf12CfObjectMoveFv();
void CfObjectMove_UnkVirtualFunc6__Q22cf12CfObjectMoveFv();
void CfObjectMove_advanceNpcTarget__Q22cf12CfObjectMoveFv();
void CfObjectMove_attachEffectSlot__Q22cf10CfObjectPcFv();
void CfObjectMove_attachEffectSlot__Q22cf12CfObjectMoveFv();
void CfObjectMove_commandNpcTarget__Q22cf12CfObjectMoveFv();
void CfObjectMove_detachModelList__Q22cf12CfObjectMoveFv();
void CfObjectMove_dropMoveRequest__Q22cf12CfObjectMoveFv();
void CfObjectMove_flushEffectSlots__Q22cf12CfObjectMoveFv();
void CfObjectMove_getMovementRate__Q22cf12CfObjectMoveFv();
void CfObjectMove_hasActorScale__Q22cf13CfObjectActorFv();
void CfObjectMove_isEffectSlotUsed__Q22cf12CfObjectMoveFv();
void CfObjectMove_loadResourceById__Q22cf12CfObjectMoveFv();
void CfObjectMove_queryNpcAdvance__Q22cf12CfObjectMoveFv();
void CfObjectMove_recordMoveValue__Q22cf12CfObjectMoveFv();
void CfObjectMove_refreshNpcState__Q22cf12CfObjectMoveFv();
void CfObjectMove_releaseSlotById__Q22cf12CfObjectMoveFv();
void CfObjectMove_restartNpcTarget__Q22cf12CfObjectMoveFv();
void CfObjectMove_setAllEffectBits__Q22cf12CfObjectMoveFv();
void CfObjectMove_setEffectSlotBit__Q22cf12CfObjectMoveFv();
void CfObjectMove_signalEffectSlot__Q22cf12CfObjectMoveFv();
void CfObjectMove_storeActionSrcId__Q22cf13CfObjectActorFv();
void CfObjectMove_transferSlotBits__Q22cf12CfObjectMoveFv();
void CfObjectMove_updateNpcTarget__Q22cf12CfObjectMoveFv();
void CfObjectMove_writeTargetField__Q22cf12CfObjectMoveFv();
void CfObjectPoint_CopyPosWords();
void CfObject_UnkVirtualFunc11__Q22cf8CfObjectFv();
void CfObject_UnkVirtualFunc12__Q22cf12CfObjectMoveFv();
void CfObject_UnkVirtualFunc12__Q22cf8CfObjectFv();
void CfObject_UnkVirtualFunc20__Q22cf13CfObjectModelFv();
void CfObject_UnkVirtualFunc21__Q22cf8CfObjectFv();
void CfObject_UnkVirtualFunc22__Q22cf12CfObjectMoveFv();
void CfObject_UnkVirtualFunc22__Q22cf8CfObjectFv();
void CfObject_UnkVirtualFunc24__Q22cf13CfObjectModelFv();
void CfObject_UnkVirtualFunc24__Q22cf8CfObjectFv();
void CfObject_UnkVirtualFunc25__Q22cf12CfObjectMoveFv();
void CfObject_UnkVirtualFunc25__Q22cf8CfObjectFv();
void CfObject_UnkVirtualFunc26__Q22cf12CfObjectMoveFv();
void CfObject_UnkVirtualFunc26__Q22cf8CfObjectFv();
void CfObject_UnkVirtualFunc27__Q22cf12CfObjectMoveFPv();
void CfObject_UnkVirtualFunc27__Q22cf8CfObjectFPv();
void CfObject_UnkVirtualFunc28__Q22cf13CfObjectModelFv();
void CfObject_UnkVirtualFunc28__Q22cf8CfObjectFv();
void CfObject_UnkVirtualFunc2__Q22cf13CfObjectModelFv();
void CfObject_UnkVirtualFunc30__Q22cf12CfObjectMoveFv();
void CfObject_UnkVirtualFunc30__Q22cf8CfObjectFv();
void CfObject_UnkVirtualFunc32__Q22cf12CfObjectMoveFv();
void CfObject_UnkVirtualFunc32__Q22cf8CfObjectFv();
void CfObject_UnkVirtualFunc33__Q22cf12CfObjectMoveFv();
void CfObject_UnkVirtualFunc33__Q22cf8CfObjectFv();
void CfObject_UnkVirtualFunc34__Q22cf13CfObjectModelFv();
void CfObject_UnkVirtualFunc34__Q22cf8CfObjectFv();
void CfObject_UnkVirtualFunc35__Q22cf13CfObjectModelFv();
void CfObject_UnkVirtualFunc35__Q22cf8CfObjectFv();
void CfObject_UnkVirtualFunc36__Q22cf13CfObjectModelFv();
void CfObject_UnkVirtualFunc36__Q22cf8CfObjectFv();
void CfObject_UnkVirtualFunc37__Q22cf12CfObjectMoveFv();
void CfObject_UnkVirtualFunc37__Q22cf8CfObjectFv();
void CfObject_UnkVirtualFunc38__Q22cf12CfObjectMoveFv();
void CfObject_UnkVirtualFunc38__Q22cf8CfObjectFv();
void CfObject_UnkVirtualFunc39__Q22cf12CfObjectMoveFv();
void CfObject_UnkVirtualFunc39__Q22cf8CfObjectFv();
void CfObject_UnkVirtualFunc3__Q22cf8CfObjectFv();
void CfObject_UnkVirtualFunc40__Q22cf12CfObjectMoveFv();
void CfObject_UnkVirtualFunc40__Q22cf8CfObjectFv();
void CfObject_UnkVirtualFunc41__Q22cf8CfObjectFv();
void CfObject_UnkVirtualFunc42__Q22cf12CfObjectMoveFv();
void CfObject_UnkVirtualFunc42__Q22cf8CfObjectFv();
void CfObject_UnkVirtualFunc43__Q22cf12CfObjectMoveFv();
void CfObject_UnkVirtualFunc43__Q22cf8CfObjectFv();
void CfObject_UnkVirtualFunc44__Q22cf8CfObjectFv();
void CfObject_UnkVirtualFunc45__Q22cf12CfObjectMoveFv();
void CfObject_UnkVirtualFunc45__Q22cf8CfObjectFv();
void CfObject_UnkVirtualFunc4__Q22cf12CfObjectMoveFv();
void CfObject_UnkVirtualFunc50__Q22cf12CfObjectMoveFv();
void CfObject_UnkVirtualFunc50__Q22cf8CfObjectFv();
void CfObject_UnkVirtualFunc51__Q22cf12CfObjectMoveFv();
void CfObject_UnkVirtualFunc51__Q22cf8CfObjectFv();
void CfObject_UnkVirtualFunc52__Q22cf13CfObjectModelFv();
void CfObject_UnkVirtualFunc52__Q22cf8CfObjectFv();
void CfObject_UnkVirtualFunc53__Q22cf13CfObjectModelFv();
void CfObject_UnkVirtualFunc53__Q22cf8CfObjectFv();
void CfObject_UnkVirtualFunc54__Q22cf13CfObjectModelFv();
void CfObject_UnkVirtualFunc54__Q22cf8CfObjectFv();
void CfObject_UnkVirtualFunc55__Q22cf13CfObjectModelFv();
void CfObject_UnkVirtualFunc55__Q22cf8CfObjectFv();
void CfObject_UnkVirtualFunc56__Q22cf13CfObjectModelFv();
void CfObject_UnkVirtualFunc56__Q22cf8CfObjectFv();
void CfObject_UnkVirtualFunc57__Q22cf12CfObjectMoveFv();
void CfObject_UnkVirtualFunc57__Q22cf8CfObjectFv();
void CfObject_UnkVirtualFunc59__Q22cf12CfObjectMoveFv();
void CfObject_UnkVirtualFunc59__Q22cf8CfObjectFv();
void CfObject_UnkVirtualFunc5__Q22cf12CfObjectMoveFv();
void CfObject_UnkVirtualFunc5__Q22cf8CfObjectFv();
void CfObject_UnkVirtualFunc60__Q22cf12CfObjectMoveFv();
void CfObject_UnkVirtualFunc60__Q22cf8CfObjectFv();
void CfObject_UnkVirtualFunc62__Q22cf12CfObjectMoveFv();
void CfObject_UnkVirtualFunc62__Q22cf8CfObjectFv();
void CfObject_UnkVirtualFunc63__Q22cf13CfObjectModelFv();
void CfObject_UnkVirtualFunc63__Q22cf8CfObjectFv();
void CfObject_UnkVirtualFunc66__Q22cf12CfObjectMoveFv();
void CfObject_UnkVirtualFunc67__Q22cf13CfObjectModelFv();
void CfObject_UnkVirtualFunc67__Q22cf8CfObjectFv();
void CfObject_UnkVirtualFunc68__Q22cf13CfObjectModelFv();
void CfObject_UnkVirtualFunc69__Q22cf13CfObjectModelFv();
void CfObject_UnkVirtualFunc69__Q22cf8CfObjectFv();
void CfObject_UnkVirtualFunc6__Q22cf12CfObjectMoveFv();
void CfObject_UnkVirtualFunc70__Q22cf12CfObjectMoveFv();
void CfObject_UnkVirtualFunc70__Q22cf8CfObjectFv();
void CfObject_UnkVirtualFunc71__Q22cf13CfObjectModelFv();
void CfObject_UnkVirtualFunc71__Q22cf8CfObjectFv();
void CfObject_UnkVirtualFunc72__Q22cf13CfObjectModelFv();
void CfObject_UnkVirtualFunc72__Q22cf8CfObjectFv();
void CfObject_UnkVirtualFunc73__Q22cf13CfObjectModelFv();
void CfObject_UnkVirtualFunc73__Q22cf8CfObjectFv();
void CfObject_UnkVirtualFunc7__Q22cf12CfObjectMoveFv();
void CfObject_checkTargetState__Q22cf12CfObjectMoveFv();
void CfObject_checkTargetState__Q22cf8CfObjectFv();
void CfObject_createMoveTarget__Q22cf12CfObjectMoveFv();
void CfObject_createMoveTarget__Q22cf8CfObjectFv();
void CfObject_forwardNpcAction__Q22cf12CfObjectMoveFv();
void CfObject_forwardNpcAction__Q22cf8CfObjectFv();
void CfObject_forwardSubObject__Q22cf12CfObjectMoveFv();
void CfObject_forwardSubObject__Q22cf8CfObjectFv();
void CfObject_getCurrentTarget__Q22cf12CfObjectMoveFv();
void CfObject_getCurrentTarget__Q22cf8CfObjectFv();
void CfObject_getMoveHeadAngle__Q22cf13CfObjectActorFv();
void CfObject_getMoveHeadAngle__Q22cf13CfObjectModelFv();
void CfObject_getMoveHeadAngle__Q22cf8CfObjectFv();
void CfObject_getMoveRateScale__Q22cf12CfObjectMoveFv();
void CfObject_getMoveRateScale__Q22cf8CfObjectFv();
void CfObject_getMoveSpeedRate__Q22cf12CfObjectMoveFv();
void CfObject_getMoveSpeedRate__Q22cf8CfObjectFv();
void CfObject_getPosVector__Q22cf12CfObjectMoveFv();
void CfObject_getPosVector__Q22cf8CfObjectFv();
void CfObject_isMoveActiveNow__Q22cf12CfObjectMoveFv();
void CfObject_isMoveActiveNow__Q22cf8CfObjectFv();
void CfObject_notifyEventDone__Q22cf13CfObjectModelFv();
void CfObject_pcInitEventStat__Q22cf10CfObjectPcFv();
void CfObject_pcRunMoveUpdate__Q22cf10CfObjectPcFv();
void CfObject_pcSyncArtsEntry__Q22cf10CfObjectPcFv();
void CfObject_pushRefreshExtra__Q22cf12CfObjectMoveFv();
void CfObject_pushRefreshExtra__Q22cf8CfObjectFv();
void CfObject_pushRefreshValue__Q22cf12CfObjectMoveFf();
void CfObject_pushRefreshValue__Q22cf13CfObjectActorFf();
void CfObject_pushRefreshValue__Q22cf8CfObjectFf();
void CfObject_queryTargetState__Q22cf12CfObjectMoveFv();
void CfObject_queryTargetState__Q22cf8CfObjectFv();
void CfObject_readRefreshValue__Q22cf12CfObjectMoveFv();
void CfObject_readRefreshValue__Q22cf8CfObjectFv();
void CfObject_setAnimSlotEntry__Q22cf12CfObjectMoveFv();
void CfObject_setAnimSlotEntry__Q22cf8CfObjectFv();
void CfObject_setMoveBusyState__Q22cf12CfObjectMoveFi();
void CfObject_setMoveBusyState__Q22cf8CfObjectFi();
void CfObject_setMoveHeadAngle__Q22cf12CfObjectMoveFv();
void CfObject_setMoveHeadAngle__Q22cf8CfObjectFf();
void CfObject_setMoveReadyFlag__Q22cf12CfObjectMoveFv();
void CfObject_setMoveReadyFlag__Q22cf8CfObjectFv();
void CfObject_setMoveTargetPtr__Q22cf12CfObjectMoveFv();
void CfObject_setMoveTargetPtr__Q22cf8CfObjectFv();
void CfObject_setMoveTargetVec__Q22cf12CfObjectMoveFv();
void CfObject_syncEnableState__Q22cf10CfObjectPcFv();
void __dt__Q22cf10CfObjectPcFv();
void __dt__Q22cf11CfObjectNpcFv();
void __dt__Q22cf12CfObjectMoveFv();
void __dt__Q22cf13CfObjectPointFv();
void applyArtsParam__Q22cf10CfObjectPcFv();
void dispatchPlayerBranch__Q22cf10CfObjectPcFv();
void enablePcFlag__Q22cf10CfObjectPcFv();
void finalizePcCleanup__Q22cf10CfObjectPcFv();
void loadNpcParams__Q22cf11CfObjectNpcFv();
void refreshArtsData__Q22cf10CfObjectPcFv();
void updatePc__Q22cf10CfObjectPcFv();
void getIconType__Q22cf11CfObjectNpcFv();
void getRltMeet__Q22cf11CfObjectNpcFv();
void handleMoveState__Q22cf10CfObjectPcFv();
void initNpcFlags__Q22cf11CfObjectNpcFv();
void initialize__Q22cf10CfObjectPcFv();
void initialize__Q22cf11CfObjectObjFv();
void isCollEnabled__Q22cf13CfObjectPointFv();
u32 lbl_eu_806618D8[];
u32 lbl_eu_806618E8[];
u32 lbl_eu_806618F8[];
u32 lbl_eu_80661900[];
u32 lbl_eu_806619A0[];
u32 lbl_eu_80661CB0[];
u32 lbl_eu_80661D10[];
u32 lbl_eu_80661D20[];
void loadIconType__Q22cf11CfObjectNpcFv();
void loadPointData__Q22cf13CfObjectPointFv();
void notifyChildUpdate__Q22cf13CfObjectPointFv();
void releasePointLink__Q22cf13CfObjectPointFv();
void resetActionTable__Q22cf10CfObjectPcFv();
void resetArtsState__Q22cf10CfObjectPcFv();
void resetPointFlags__Q22cf13CfObjectPointFv();
void reset__Q22cf11CfObjectObjFv();
void scanArtsEntries__Q22cf10CfObjectPcFv();
void setChildPoint__Q22cf13CfObjectPointFv();
void setPointEnabled__Q22cf13CfObjectPointFv();
void setPointPosition__Q22cf13CfObjectPointFv();
void setupActionTable__Q22cf10CfObjectPcFv();
void syncArtsEntry__Q22cf10CfObjectPcFv();
void triggerActionRefreshA__Q22cf10CfObjectPcFv();
void triggerActionRefreshB__Q22cf10CfObjectPcFv();
void updateNpcDialog__Q22cf11CfObjectNpcFv();
void updatePointState__Q22cf13CfObjectPointFv();
void update__Q22cf11CfObjectObjFv();
void validatePointState__Q22cf13CfObjectPointFv();
}

// C++ linkage: defined in CfObjectObj.cpp (retail flat-named deleting dtor).
namespace cf { class CfObjectObj; }
cf::CfObjectObj* __dt__800BFA14(cf::CfObjectObj* ths, int deleteFlag);

void* lbl_eu_80529690[] = {
    (void*)&lbl_eu_806618D8, 0, (void*)CObjectState_setStateBitMask__Q22cf12CObjectStateFv, (void*)CObjectState_checkStateFlags__Q22cf12CObjectStateFv,
    (void*)CObjectState_setStateBitFlag__Q22cf12CObjectStateFv, (void*)CObjectState_clearStateWord4__Q22cf12CObjectStateFv, (void*)CObjectState_applyStateFlags__Q22cf12CObjectStateFv, (void*)CObjectState_clearStateWord8__Q22cf12CObjectStateFv,
    (void*)CObjectState_clearStateFlags8__Q22cf12CObjectStateFv, (void*)CObjectState_UnkVirtualFunc8__Q22cf12CObjectStateFv, (void*)CObjectState_UnkVirtualFunc9__Q22cf12CObjectStateFv, (void*)CObjectState_setStateBitMask0__Q22cf12CObjectStateFv,
    (void*)CObjectState_getStateData__Q22cf12CObjectStateFv, (void*)CObjectState_setStateBitMask2__Q22cf12CObjectStateFv, (void*)CObjectState_setStateBitMask3__Q22cf12CObjectStateFv, (void*)CObjectParam_UnkVirtualFunc1__Q22cf12CObjectParamFv,
    (void*)CObjectParam_UnkVirtualFunc2__Q22cf12CfObjectMoveFv, (void*)CObjectParam_hasObjectName__Q22cf12CObjectParamFv, (void*)CObjectParam_UnkVirtualFunc4__Q22cf12CObjectParamFv, (void*)CObjectParam_getSelfObjectId__Q22cf12CObjectParamFv,
    (void*)CObjectParam_signalActionEnd__Q22cf12CObjectParamFv, (void*)__dt__Q22cf12CfObjectMoveFv, (void*)CfObject_UnkVirtualFunc2__Q22cf13CfObjectModelFv, (void*)CfObject_UnkVirtualFunc3__Q22cf8CfObjectFv,
    (void*)CfObject_UnkVirtualFunc4__Q22cf12CfObjectMoveFv, (void*)CfObject_UnkVirtualFunc5__Q22cf12CfObjectMoveFv, (void*)CfObject_UnkVirtualFunc6__Q22cf12CfObjectMoveFv, (void*)CfObject_UnkVirtualFunc7__Q22cf12CfObjectMoveFv,
    (void*)CfObject_notifyEventDone__Q22cf13CfObjectModelFv, (void*)CfObject_isMoveActiveNow__Q22cf12CfObjectMoveFv, (void*)CfObject_forwardSubObject__Q22cf12CfObjectMoveFv, (void*)CfObject_UnkVirtualFunc11__Q22cf8CfObjectFv,
    (void*)CfObject_UnkVirtualFunc12__Q22cf12CfObjectMoveFv, (void*)CfObject_queryTargetState__Q22cf12CfObjectMoveFv, (void*)CfObject_pushRefreshValue__Q22cf12CfObjectMoveFf, (void*)CfObject_getMoveSpeedRate__Q22cf12CfObjectMoveFv,
    (void*)CfObject_pushRefreshExtra__Q22cf12CfObjectMoveFv, (void*)CfObject_readRefreshValue__Q22cf12CfObjectMoveFv, (void*)CfObject_checkTargetState__Q22cf12CfObjectMoveFv, (void*)CfObject_setMoveTargetVec__Q22cf12CfObjectMoveFv,
    (void*)CfObject_UnkVirtualFunc20__Q22cf13CfObjectModelFv, (void*)CfObject_UnkVirtualFunc21__Q22cf8CfObjectFv, (void*)CfObject_UnkVirtualFunc22__Q22cf12CfObjectMoveFv, (void*)CfObject_getPosVector__Q22cf12CfObjectMoveFv,
    (void*)CfObject_UnkVirtualFunc24__Q22cf13CfObjectModelFv, (void*)CfObject_UnkVirtualFunc25__Q22cf12CfObjectMoveFv, (void*)CfObject_UnkVirtualFunc26__Q22cf12CfObjectMoveFv, (void*)CfObject_UnkVirtualFunc27__Q22cf12CfObjectMoveFPv,
    (void*)CfObject_UnkVirtualFunc28__Q22cf13CfObjectModelFv, (void*)CfObject_setMoveHeadAngle__Q22cf12CfObjectMoveFv, (void*)CfObject_UnkVirtualFunc30__Q22cf12CfObjectMoveFv, (void*)CfObject_getMoveHeadAngle__Q22cf13CfObjectModelFv,
    (void*)CfObject_UnkVirtualFunc32__Q22cf12CfObjectMoveFv, (void*)CfObject_UnkVirtualFunc33__Q22cf12CfObjectMoveFv, (void*)CfObject_UnkVirtualFunc34__Q22cf13CfObjectModelFv, (void*)CfObject_UnkVirtualFunc35__Q22cf13CfObjectModelFv,
    (void*)CfObject_UnkVirtualFunc36__Q22cf13CfObjectModelFv, (void*)CfObject_UnkVirtualFunc37__Q22cf12CfObjectMoveFv, (void*)CfObject_UnkVirtualFunc38__Q22cf12CfObjectMoveFv, (void*)CfObject_UnkVirtualFunc39__Q22cf12CfObjectMoveFv,
    (void*)CfObject_UnkVirtualFunc40__Q22cf12CfObjectMoveFv, (void*)CfObject_UnkVirtualFunc41__Q22cf8CfObjectFv, (void*)CfObject_UnkVirtualFunc42__Q22cf12CfObjectMoveFv, (void*)CfObject_UnkVirtualFunc43__Q22cf12CfObjectMoveFv,
    (void*)CfObject_UnkVirtualFunc44__Q22cf8CfObjectFv, (void*)CfObject_UnkVirtualFunc45__Q22cf12CfObjectMoveFv, (void*)CfObject_setMoveTargetPtr__Q22cf12CfObjectMoveFv, (void*)CfObject_createMoveTarget__Q22cf12CfObjectMoveFv,
    (void*)CfObject_getCurrentTarget__Q22cf12CfObjectMoveFv, (void*)CfObject_forwardNpcAction__Q22cf12CfObjectMoveFv, (void*)CfObject_UnkVirtualFunc50__Q22cf12CfObjectMoveFv, (void*)CfObject_UnkVirtualFunc51__Q22cf12CfObjectMoveFv,
    (void*)CfObject_UnkVirtualFunc52__Q22cf13CfObjectModelFv, (void*)CfObject_UnkVirtualFunc53__Q22cf13CfObjectModelFv, (void*)CfObject_UnkVirtualFunc54__Q22cf13CfObjectModelFv, (void*)CfObject_UnkVirtualFunc55__Q22cf13CfObjectModelFv,
    (void*)CfObject_UnkVirtualFunc56__Q22cf13CfObjectModelFv, (void*)CfObject_UnkVirtualFunc57__Q22cf12CfObjectMoveFv, (void*)CfObject_getMoveRateScale__Q22cf12CfObjectMoveFv, (void*)CfObject_UnkVirtualFunc59__Q22cf12CfObjectMoveFv,
    (void*)CfObject_UnkVirtualFunc60__Q22cf12CfObjectMoveFv, (void*)CfObject_setAnimSlotEntry__Q22cf12CfObjectMoveFv, (void*)CfObject_UnkVirtualFunc62__Q22cf12CfObjectMoveFv, (void*)CfObject_UnkVirtualFunc63__Q22cf13CfObjectModelFv,
    (void*)CfObject_setMoveBusyState__Q22cf12CfObjectMoveFi, (void*)CfObject_setMoveReadyFlag__Q22cf12CfObjectMoveFv, (void*)CfObject_UnkVirtualFunc66__Q22cf12CfObjectMoveFv, (void*)CfObject_UnkVirtualFunc67__Q22cf13CfObjectModelFv,
    (void*)CfObject_UnkVirtualFunc68__Q22cf13CfObjectModelFv, (void*)CfObject_UnkVirtualFunc69__Q22cf13CfObjectModelFv, (void*)CfObject_UnkVirtualFunc70__Q22cf12CfObjectMoveFv, (void*)CfObject_UnkVirtualFunc71__Q22cf13CfObjectModelFv,
    (void*)CfObject_UnkVirtualFunc72__Q22cf13CfObjectModelFv, (void*)CfObject_UnkVirtualFunc73__Q22cf13CfObjectModelFv, (void*)CfObjectModel_UnkVirtualFunc1__Q22cf12CfObjectMoveFv, (void*)CfObjectModel_UnkVirtualFunc2__Q22cf12CfObjectMoveFv,
    (void*)CfObjectModel_UnkVirtualFunc3__Q22cf13CfObjectModelFv, (void*)CfObjectModel_UnkVirtualFunc4__Q22cf12CfObjectMoveFv, (void*)CfObjectModel_UnkVirtualFunc5__Q22cf12CfObjectMoveFv, (void*)CfObjectModel_UnkVirtualFunc6__Q22cf12CfObjectMoveFv,
    (void*)CfObjectModel_UnkVirtualFunc7__Q22cf13CfObjectModelFv, (void*)CfObjectModel_UnkVirtualFunc8__Q22cf13CfObjectModelFv, (void*)CfObjectModel_UnkVirtualFunc9__Q22cf13CfObjectModelFv, (void*)CfObjectModel_UnkVirtualFunc10__Q22cf13CfObjectModelFv,
    (void*)CfObjectModel_UnkVirtualFunc11__Q22cf13CfObjectModelFv, (void*)CfObjectModel_UnkVirtualFunc12__Q22cf13CfObjectModelFv, (void*)CfObjectModel_UnkVirtualFunc13__Q22cf13CfObjectModelFv, (void*)CfObjectModel_UnkVirtualFunc14__Q22cf13CfObjectModelFv,
    (void*)CfObjectModel_UnkVirtualFunc15__Q22cf13CfObjectModelFv, (void*)CfObjectModel_UnkVirtualFunc16__Q22cf13CfObjectModelFv, (void*)CfObjectModel_UnkVirtualFunc17__Q22cf13CfObjectModelFv, (void*)CfObjectModel_UnkVirtualFunc18__Q22cf12CfObjectMoveFv,
    (void*)CfObjectModel_UnkVirtualFunc19__Q22cf12CfObjectMoveFv, (void*)CfObjectModel_UnkVirtualFunc20__Q22cf13CfObjectModelFv, (void*)CfObjectMove_dropMoveRequest__Q22cf12CfObjectMoveFv, (void*)CfObjectMove_detachModelList__Q22cf12CfObjectMoveFv,
    (void*)CfObjectMove_releaseSlotById__Q22cf12CfObjectMoveFv, (void*)CfObjectMove_recordMoveValue__Q22cf12CfObjectMoveFv, (void*)CfObjectMove_getMovementRate__Q22cf12CfObjectMoveFv, (void*)CfObjectMove_UnkVirtualFunc6__Q22cf12CfObjectMoveFv,
    (void*)CfObjectMove_refreshNpcState__Q22cf12CfObjectMoveFv, (void*)CfObjectMove_updateNpcTarget__Q22cf12CfObjectMoveFv, (void*)CfObjectMove_queryNpcAdvance__Q22cf12CfObjectMoveFv, (void*)CfObjectMove_UnkVirtualFunc10__Q22cf12CfObjectMoveFv,
    (void*)CfObjectMove_writeTargetField__Q22cf12CfObjectMoveFv, (void*)CfObjectMove_advanceNpcTarget__Q22cf12CfObjectMoveFv, (void*)CfObjectMove_commandNpcTarget__Q22cf12CfObjectMoveFv, (void*)CfObjectMove_restartNpcTarget__Q22cf12CfObjectMoveFv,
    (void*)CfObjectMove_UnkVirtualFunc15__Q22cf12CfObjectMoveFv, (void*)CfObjectMove_attachEffectSlot__Q22cf12CfObjectMoveFv, (void*)CfObjectMove_setEffectSlotBit__Q22cf12CfObjectMoveFv, (void*)CfObjectMove_transferSlotBits__Q22cf12CfObjectMoveFv,
    (void*)CfObjectMove_isEffectSlotUsed__Q22cf12CfObjectMoveFv, (void*)CfObjectMove_signalEffectSlot__Q22cf12CfObjectMoveFv, (void*)CfObjectMove_flushEffectSlots__Q22cf12CfObjectMoveFv, (void*)CfObjectMove_setAllEffectBits__Q22cf12CfObjectMoveFv,
    (void*)CfObjectMove_loadResourceById__Q22cf12CfObjectMoveFv, 0,
};
void* lbl_eu_805298B8[] = {
    (void*)&lbl_eu_80661D10, 0, (void*)CObjectState_setStateBitMask__Q22cf12CObjectStateFv, (void*)CObjectState_checkStateFlags__Q22cf12CObjectStateFv,
    (void*)CObjectState_setStateBitFlag__Q22cf12CObjectStateFv, (void*)CObjectState_clearStateWord4__Q22cf12CObjectStateFv, (void*)CObjectState_applyStateFlags__Q22cf12CObjectStateFv, (void*)CObjectState_clearStateWord8__Q22cf12CObjectStateFv,
    (void*)CObjectState_clearStateFlags8__Q22cf12CObjectStateFv, (void*)CObjectState_UnkVirtualFunc8__Q22cf12CObjectStateFv, (void*)CObjectState_UnkVirtualFunc9__Q22cf12CObjectStateFv, (void*)CObjectState_setStateBitMask0__Q22cf12CObjectStateFv,
    (void*)CObjectState_getStateData__Q22cf12CObjectStateFv, (void*)CObjectState_setStateBitMask2__Q22cf12CObjectStateFv, (void*)CObjectState_setStateBitMask3__Q22cf12CObjectStateFv, (void*)CObjectParam_UnkVirtualFunc1__Q22cf12CObjectParamFv,
(void*)loadNpcParams__Q22cf11CfObjectNpcFv,
    (void*)CObjectParam_signalActionEnd__Q22cf12CObjectParamFv, (void*)__dt__Q22cf11CfObjectNpcFv, (void*)initNpcFlags__Q22cf11CfObjectNpcFv, (void*)CfObject_UnkVirtualFunc3__Q22cf8CfObjectFv,
    (void*)updateNpcDialog__Q22cf11CfObjectNpcFv, (void*)CfObject_UnkVirtualFunc5__Q22cf12CfObjectMoveFv, (void*)CfObject_UnkVirtualFunc6__Q22cf12CfObjectMoveFv, (void*)CfObject_UnkVirtualFunc7__Q22cf12CfObjectMoveFv,
    (void*)CfObject_notifyEventDone__Q22cf13CfObjectModelFv, (void*)CfObject_isMoveActiveNow__Q22cf12CfObjectMoveFv, (void*)CfObject_forwardSubObject__Q22cf12CfObjectMoveFv, (void*)CfObject_UnkVirtualFunc11__Q22cf8CfObjectFv,
    (void*)CfObject_UnkVirtualFunc12__Q22cf12CfObjectMoveFv, (void*)CfObject_queryTargetState__Q22cf12CfObjectMoveFv, (void*)CfObject_pushRefreshValue__Q22cf12CfObjectMoveFf, (void*)CfObject_getMoveSpeedRate__Q22cf12CfObjectMoveFv,
    (void*)CfObject_pushRefreshExtra__Q22cf12CfObjectMoveFv, (void*)CfObject_readRefreshValue__Q22cf12CfObjectMoveFv, (void*)CfObject_checkTargetState__Q22cf12CfObjectMoveFv, (void*)CfObject_setMoveTargetVec__Q22cf12CfObjectMoveFv,
    (void*)CfObject_UnkVirtualFunc20__Q22cf13CfObjectModelFv, (void*)CfObject_UnkVirtualFunc21__Q22cf8CfObjectFv, (void*)CfObject_UnkVirtualFunc22__Q22cf12CfObjectMoveFv, (void*)CfObject_getPosVector__Q22cf12CfObjectMoveFv,
    (void*)CfObject_UnkVirtualFunc24__Q22cf13CfObjectModelFv, (void*)CfObject_UnkVirtualFunc25__Q22cf12CfObjectMoveFv, (void*)CfObject_UnkVirtualFunc26__Q22cf12CfObjectMoveFv, (void*)CfObject_UnkVirtualFunc27__Q22cf12CfObjectMoveFPv,
    (void*)CfObject_UnkVirtualFunc28__Q22cf13CfObjectModelFv, (void*)CfObject_setMoveHeadAngle__Q22cf12CfObjectMoveFv, (void*)CfObject_UnkVirtualFunc30__Q22cf12CfObjectMoveFv, (void*)CfObject_getMoveHeadAngle__Q22cf13CfObjectModelFv,
    (void*)CfObject_UnkVirtualFunc32__Q22cf12CfObjectMoveFv, (void*)CfObject_UnkVirtualFunc33__Q22cf12CfObjectMoveFv, (void*)CfObject_UnkVirtualFunc34__Q22cf13CfObjectModelFv, (void*)CfObject_UnkVirtualFunc35__Q22cf13CfObjectModelFv,
    (void*)CfObject_UnkVirtualFunc36__Q22cf13CfObjectModelFv, (void*)CfObject_UnkVirtualFunc37__Q22cf12CfObjectMoveFv, (void*)CfObject_UnkVirtualFunc38__Q22cf12CfObjectMoveFv, (void*)CfObject_UnkVirtualFunc39__Q22cf12CfObjectMoveFv,
    (void*)CfObject_UnkVirtualFunc40__Q22cf12CfObjectMoveFv, (void*)CfObject_UnkVirtualFunc41__Q22cf8CfObjectFv, (void*)CfObject_UnkVirtualFunc42__Q22cf12CfObjectMoveFv, (void*)CfObject_UnkVirtualFunc43__Q22cf12CfObjectMoveFv,
    (void*)CfObject_UnkVirtualFunc44__Q22cf8CfObjectFv, (void*)CfObject_UnkVirtualFunc45__Q22cf12CfObjectMoveFv, (void*)CfObject_setMoveTargetPtr__Q22cf12CfObjectMoveFv, (void*)CfObject_createMoveTarget__Q22cf12CfObjectMoveFv,
    (void*)CfObject_getCurrentTarget__Q22cf12CfObjectMoveFv, (void*)CfObject_forwardNpcAction__Q22cf12CfObjectMoveFv, (void*)CfObject_UnkVirtualFunc50__Q22cf12CfObjectMoveFv, (void*)CfObject_UnkVirtualFunc51__Q22cf12CfObjectMoveFv,
    (void*)CfObject_UnkVirtualFunc52__Q22cf13CfObjectModelFv, (void*)CfObject_UnkVirtualFunc53__Q22cf13CfObjectModelFv, (void*)CfObject_UnkVirtualFunc54__Q22cf13CfObjectModelFv, (void*)CfObject_UnkVirtualFunc55__Q22cf13CfObjectModelFv,
    (void*)CfObject_UnkVirtualFunc56__Q22cf13CfObjectModelFv, (void*)CfObject_UnkVirtualFunc57__Q22cf12CfObjectMoveFv, (void*)CfObject_getMoveRateScale__Q22cf12CfObjectMoveFv, (void*)CfObject_UnkVirtualFunc59__Q22cf12CfObjectMoveFv,
    (void*)CfObject_UnkVirtualFunc60__Q22cf12CfObjectMoveFv, (void*)CfObject_setAnimSlotEntry__Q22cf12CfObjectMoveFv, (void*)CfObject_UnkVirtualFunc62__Q22cf12CfObjectMoveFv, (void*)CfObject_UnkVirtualFunc63__Q22cf13CfObjectModelFv,
    (void*)CfObject_setMoveBusyState__Q22cf12CfObjectMoveFi, (void*)CfObject_setMoveReadyFlag__Q22cf12CfObjectMoveFv, (void*)CfObject_UnkVirtualFunc66__Q22cf12CfObjectMoveFv, (void*)CfObject_UnkVirtualFunc67__Q22cf13CfObjectModelFv,
    (void*)CfObject_UnkVirtualFunc68__Q22cf13CfObjectModelFv, (void*)CfObject_UnkVirtualFunc69__Q22cf13CfObjectModelFv, (void*)CfObject_UnkVirtualFunc70__Q22cf12CfObjectMoveFv, (void*)CfObject_UnkVirtualFunc71__Q22cf13CfObjectModelFv,
    (void*)CfObject_UnkVirtualFunc72__Q22cf13CfObjectModelFv, (void*)CfObject_UnkVirtualFunc73__Q22cf13CfObjectModelFv, (void*)CfObjectModel_UnkVirtualFunc1__Q22cf12CfObjectMoveFv, (void*)CfObjectModel_UnkVirtualFunc2__Q22cf12CfObjectMoveFv,
    (void*)CfObjectModel_UnkVirtualFunc3__Q22cf13CfObjectModelFv, (void*)CfObjectModel_UnkVirtualFunc4__Q22cf12CfObjectMoveFv, (void*)CfObjectModel_UnkVirtualFunc5__Q22cf12CfObjectMoveFv, (void*)CfObjectModel_UnkVirtualFunc6__Q22cf12CfObjectMoveFv,
    (void*)CfObjectModel_UnkVirtualFunc7__Q22cf13CfObjectModelFv, (void*)CfObjectModel_UnkVirtualFunc8__Q22cf13CfObjectModelFv, (void*)CfObjectModel_UnkVirtualFunc9__Q22cf13CfObjectModelFv, (void*)CfObjectModel_UnkVirtualFunc10__Q22cf13CfObjectModelFv,
    (void*)CfObjectModel_UnkVirtualFunc11__Q22cf13CfObjectModelFv, (void*)CfObjectModel_UnkVirtualFunc12__Q22cf13CfObjectModelFv, (void*)CfObjectModel_UnkVirtualFunc13__Q22cf13CfObjectModelFv, (void*)CfObjectModel_UnkVirtualFunc14__Q22cf13CfObjectModelFv,
    (void*)CfObjectModel_UnkVirtualFunc15__Q22cf13CfObjectModelFv, (void*)CfObjectModel_UnkVirtualFunc16__Q22cf13CfObjectModelFv, (void*)CfObjectModel_UnkVirtualFunc17__Q22cf13CfObjectModelFv, (void*)CfObjectModel_UnkVirtualFunc18__Q22cf12CfObjectMoveFv,
    (void*)CfObjectModel_UnkVirtualFunc19__Q22cf12CfObjectMoveFv, (void*)CfObjectModel_UnkVirtualFunc20__Q22cf13CfObjectModelFv, (void*)CfObjectMove_dropMoveRequest__Q22cf12CfObjectMoveFv, (void*)CfObjectMove_detachModelList__Q22cf12CfObjectMoveFv,
    (void*)CfObjectMove_releaseSlotById__Q22cf12CfObjectMoveFv, (void*)CfObjectMove_recordMoveValue__Q22cf12CfObjectMoveFv, (void*)CfObjectMove_getMovementRate__Q22cf12CfObjectMoveFv, (void*)CfObjectMove_UnkVirtualFunc6__Q22cf12CfObjectMoveFv,
    (void*)CfObjectMove_refreshNpcState__Q22cf12CfObjectMoveFv, (void*)CfObjectMove_updateNpcTarget__Q22cf12CfObjectMoveFv, (void*)CfObjectMove_queryNpcAdvance__Q22cf12CfObjectMoveFv, (void*)CfObjectMove_UnkVirtualFunc10__Q22cf12CfObjectMoveFv,
    (void*)CfObjectMove_writeTargetField__Q22cf12CfObjectMoveFv, (void*)CfObjectMove_advanceNpcTarget__Q22cf12CfObjectMoveFv, (void*)CfObjectMove_commandNpcTarget__Q22cf12CfObjectMoveFv, (void*)CfObjectMove_restartNpcTarget__Q22cf12CfObjectMoveFv,
    (void*)CfObjectMove_UnkVirtualFunc15__Q22cf12CfObjectMoveFv, (void*)CfObjectMove_attachEffectSlot__Q22cf12CfObjectMoveFv, (void*)CfObjectMove_setEffectSlotBit__Q22cf12CfObjectMoveFv, (void*)CfObjectMove_transferSlotBits__Q22cf12CfObjectMoveFv,
    (void*)CfObjectMove_isEffectSlotUsed__Q22cf12CfObjectMoveFv, (void*)CfObjectMove_signalEffectSlot__Q22cf12CfObjectMoveFv, (void*)CfObjectMove_flushEffectSlots__Q22cf12CfObjectMoveFv, (void*)CfObjectMove_setAllEffectBits__Q22cf12CfObjectMoveFv,
    (void*)CfObjectMove_loadResourceById__Q22cf12CfObjectMoveFv, (void*)loadIconType__Q22cf11CfObjectNpcFv, (void*)getIconType__Q22cf11CfObjectNpcFv, (void*)getRltMeet__Q22cf11CfObjectNpcFv,
};
void* lbl_eu_80529AE8[] = {
    (void*)&lbl_eu_80661900, 0, (void*)&lbl_eu_806618F8, 0,
    (void*)&lbl_eu_806618F0, 0, (void*)&lbl_eu_806618E8, 0,
    (void*)&lbl_eu_806618D8, 0, 0, 0,
};
void* lbl_eu_80529B18[] = {
    (void*)&lbl_eu_80661900, 0, (void*)&lbl_eu_806618F8, 0,
    (void*)&lbl_eu_806618F0, 0, (void*)&lbl_eu_806618E8, 0,
    (void*)&lbl_eu_806618D8, 0, (void*)&lbl_eu_80661D20, 0,
    0,
};
void* lbl_eu_80529B4C[] = {
    (void*)&lbl_eu_80661D20, 0, (void*)CObjectState_setStateBitMask__Q22cf12CObjectStateFv, (void*)CObjectState_checkStateFlags__Q22cf12CObjectStateFv,
    (void*)CObjectState_setStateBitFlag__Q22cf12CObjectStateFv, (void*)CObjectState_clearStateWord4__Q22cf12CObjectStateFv, (void*)CObjectState_applyStateFlags__Q22cf12CObjectStateFv, (void*)CObjectState_clearStateWord8__Q22cf12CObjectStateFv,
    (void*)CObjectState_clearStateFlags8__Q22cf12CObjectStateFv, (void*)CObjectState_UnkVirtualFunc8__Q22cf12CObjectStateFv, (void*)CObjectState_UnkVirtualFunc9__Q22cf12CObjectStateFv, (void*)CObjectState_setStateBitMask0__Q22cf12CObjectStateFv,
    (void*)CObjectState_getStateData__Q22cf12CObjectStateFv, (void*)CObjectState_setStateBitMask2__Q22cf12CObjectStateFv, (void*)CObjectState_setStateBitMask3__Q22cf12CObjectStateFv, (void*)CObjectParam_UnkVirtualFunc1__Q22cf12CObjectParamFv,
    (void*)CObjectParam_UnkVirtualFunc2__Q22cf12CfObjectMoveFv, (void*)CObjectParam_hasObjectName__Q22cf12CObjectParamFv, (void*)reset__Q22cf11CfObjectObjFv, (void*)CObjectParam_getSelfObjectId__Q22cf12CObjectParamFv,
    (void*)CObjectParam_signalActionEnd__Q22cf12CObjectParamFv, (void*)__dt__800BFA14, (void*)initialize__Q22cf11CfObjectObjFv, (void*)CfObject_UnkVirtualFunc3__Q22cf8CfObjectFv,
    (void*)update__Q22cf11CfObjectObjFv, (void*)CfObject_UnkVirtualFunc5__Q22cf12CfObjectMoveFv, (void*)CfObject_UnkVirtualFunc6__Q22cf12CfObjectMoveFv, (void*)CfObject_UnkVirtualFunc7__Q22cf12CfObjectMoveFv,
    (void*)CfObject_notifyEventDone__Q22cf13CfObjectModelFv, (void*)CfObject_isMoveActiveNow__Q22cf12CfObjectMoveFv, (void*)CfObject_forwardSubObject__Q22cf12CfObjectMoveFv, (void*)CfObject_UnkVirtualFunc11__Q22cf8CfObjectFv,
    (void*)CfObject_UnkVirtualFunc12__Q22cf12CfObjectMoveFv, (void*)CfObject_queryTargetState__Q22cf12CfObjectMoveFv, (void*)CfObject_pushRefreshValue__Q22cf12CfObjectMoveFf, (void*)CfObject_getMoveSpeedRate__Q22cf12CfObjectMoveFv,
    (void*)CfObject_pushRefreshExtra__Q22cf12CfObjectMoveFv, (void*)CfObject_readRefreshValue__Q22cf12CfObjectMoveFv, (void*)CfObject_checkTargetState__Q22cf12CfObjectMoveFv, (void*)CfObject_setMoveTargetVec__Q22cf12CfObjectMoveFv,
    (void*)CfObject_UnkVirtualFunc20__Q22cf13CfObjectModelFv, (void*)CfObject_UnkVirtualFunc21__Q22cf8CfObjectFv, (void*)CfObject_UnkVirtualFunc22__Q22cf12CfObjectMoveFv, (void*)CfObject_getPosVector__Q22cf12CfObjectMoveFv,
    (void*)CfObject_UnkVirtualFunc24__Q22cf13CfObjectModelFv, (void*)CfObject_UnkVirtualFunc25__Q22cf12CfObjectMoveFv, (void*)CfObject_UnkVirtualFunc26__Q22cf12CfObjectMoveFv, (void*)CfObject_UnkVirtualFunc27__Q22cf12CfObjectMoveFPv,
    (void*)CfObject_UnkVirtualFunc28__Q22cf13CfObjectModelFv, (void*)CfObject_setMoveHeadAngle__Q22cf12CfObjectMoveFv, (void*)CfObject_UnkVirtualFunc30__Q22cf12CfObjectMoveFv, (void*)CfObject_getMoveHeadAngle__Q22cf13CfObjectModelFv,
    (void*)CfObject_UnkVirtualFunc32__Q22cf12CfObjectMoveFv, (void*)CfObject_UnkVirtualFunc33__Q22cf12CfObjectMoveFv, (void*)CfObject_UnkVirtualFunc34__Q22cf13CfObjectModelFv, (void*)CfObject_UnkVirtualFunc35__Q22cf13CfObjectModelFv,
    (void*)CfObject_UnkVirtualFunc36__Q22cf13CfObjectModelFv, (void*)CfObject_UnkVirtualFunc37__Q22cf12CfObjectMoveFv, (void*)CfObject_UnkVirtualFunc38__Q22cf12CfObjectMoveFv, (void*)CfObject_UnkVirtualFunc39__Q22cf12CfObjectMoveFv,
    (void*)CfObject_UnkVirtualFunc40__Q22cf12CfObjectMoveFv, (void*)CfObject_UnkVirtualFunc41__Q22cf8CfObjectFv, (void*)CfObject_UnkVirtualFunc42__Q22cf12CfObjectMoveFv, (void*)CfObject_UnkVirtualFunc43__Q22cf12CfObjectMoveFv,
    (void*)CfObject_UnkVirtualFunc44__Q22cf8CfObjectFv, (void*)CfObject_UnkVirtualFunc45__Q22cf12CfObjectMoveFv, (void*)CfObject_setMoveTargetPtr__Q22cf12CfObjectMoveFv, (void*)CfObject_createMoveTarget__Q22cf12CfObjectMoveFv,
    (void*)CfObject_getCurrentTarget__Q22cf12CfObjectMoveFv, (void*)CfObject_forwardNpcAction__Q22cf12CfObjectMoveFv, (void*)CfObject_UnkVirtualFunc50__Q22cf12CfObjectMoveFv, (void*)CfObject_UnkVirtualFunc51__Q22cf12CfObjectMoveFv,
    (void*)CfObject_UnkVirtualFunc52__Q22cf13CfObjectModelFv, (void*)CfObject_UnkVirtualFunc53__Q22cf13CfObjectModelFv, (void*)CfObject_UnkVirtualFunc54__Q22cf13CfObjectModelFv, (void*)CfObject_UnkVirtualFunc55__Q22cf13CfObjectModelFv,
    (void*)CfObject_UnkVirtualFunc56__Q22cf13CfObjectModelFv, (void*)CfObject_UnkVirtualFunc57__Q22cf12CfObjectMoveFv, (void*)CfObject_getMoveRateScale__Q22cf12CfObjectMoveFv, (void*)CfObject_UnkVirtualFunc59__Q22cf12CfObjectMoveFv,
    (void*)CfObject_UnkVirtualFunc60__Q22cf12CfObjectMoveFv, (void*)CfObject_setAnimSlotEntry__Q22cf12CfObjectMoveFv, (void*)CfObject_UnkVirtualFunc62__Q22cf12CfObjectMoveFv, (void*)CfObject_UnkVirtualFunc63__Q22cf13CfObjectModelFv,
    (void*)CfObject_setMoveBusyState__Q22cf12CfObjectMoveFi, (void*)CfObject_setMoveReadyFlag__Q22cf12CfObjectMoveFv, (void*)CfObject_UnkVirtualFunc66__Q22cf12CfObjectMoveFv, (void*)CfObject_UnkVirtualFunc67__Q22cf13CfObjectModelFv,
    (void*)CfObject_UnkVirtualFunc68__Q22cf13CfObjectModelFv, (void*)CfObject_UnkVirtualFunc69__Q22cf13CfObjectModelFv, (void*)CfObject_UnkVirtualFunc70__Q22cf12CfObjectMoveFv, (void*)CfObject_UnkVirtualFunc71__Q22cf13CfObjectModelFv,
    (void*)CfObject_UnkVirtualFunc72__Q22cf13CfObjectModelFv, (void*)CfObject_UnkVirtualFunc73__Q22cf13CfObjectModelFv, (void*)CfObjectModel_UnkVirtualFunc1__Q22cf12CfObjectMoveFv, (void*)CfObjectModel_UnkVirtualFunc2__Q22cf12CfObjectMoveFv,
    (void*)CfObjectModel_UnkVirtualFunc3__Q22cf13CfObjectModelFv, (void*)CfObjectModel_UnkVirtualFunc4__Q22cf12CfObjectMoveFv, (void*)CfObjectModel_UnkVirtualFunc5__Q22cf12CfObjectMoveFv, (void*)CfObjectModel_UnkVirtualFunc6__Q22cf12CfObjectMoveFv,
    (void*)CfObjectModel_UnkVirtualFunc7__Q22cf13CfObjectModelFv, (void*)CfObjectModel_UnkVirtualFunc8__Q22cf13CfObjectModelFv, (void*)CfObjectModel_UnkVirtualFunc9__Q22cf13CfObjectModelFv, (void*)CfObjectModel_UnkVirtualFunc10__Q22cf13CfObjectModelFv,
    (void*)CfObjectModel_UnkVirtualFunc11__Q22cf13CfObjectModelFv, (void*)CfObjectModel_UnkVirtualFunc12__Q22cf13CfObjectModelFv, (void*)CfObjectModel_UnkVirtualFunc13__Q22cf13CfObjectModelFv, (void*)CfObjectModel_UnkVirtualFunc14__Q22cf13CfObjectModelFv,
    (void*)CfObjectModel_UnkVirtualFunc15__Q22cf13CfObjectModelFv, (void*)CfObjectModel_UnkVirtualFunc16__Q22cf13CfObjectModelFv, (void*)CfObjectModel_UnkVirtualFunc17__Q22cf13CfObjectModelFv, (void*)CfObjectModel_UnkVirtualFunc18__Q22cf12CfObjectMoveFv,
    (void*)CfObjectModel_UnkVirtualFunc19__Q22cf12CfObjectMoveFv, (void*)CfObjectModel_UnkVirtualFunc20__Q22cf13CfObjectModelFv, (void*)CfObjectMove_dropMoveRequest__Q22cf12CfObjectMoveFv, (void*)CfObjectMove_detachModelList__Q22cf12CfObjectMoveFv,
    (void*)CfObjectMove_releaseSlotById__Q22cf12CfObjectMoveFv, (void*)CfObjectMove_recordMoveValue__Q22cf12CfObjectMoveFv, (void*)CfObjectMove_getMovementRate__Q22cf12CfObjectMoveFv, (void*)CfObjectMove_UnkVirtualFunc6__Q22cf12CfObjectMoveFv,
    (void*)CfObjectMove_refreshNpcState__Q22cf12CfObjectMoveFv, (void*)CfObjectMove_updateNpcTarget__Q22cf12CfObjectMoveFv, (void*)CfObjectMove_queryNpcAdvance__Q22cf12CfObjectMoveFv, (void*)CfObjectMove_UnkVirtualFunc10__Q22cf12CfObjectMoveFv,
    (void*)CfObjectMove_writeTargetField__Q22cf12CfObjectMoveFv, (void*)CfObjectMove_advanceNpcTarget__Q22cf12CfObjectMoveFv, (void*)CfObjectMove_commandNpcTarget__Q22cf12CfObjectMoveFv, (void*)CfObjectMove_restartNpcTarget__Q22cf12CfObjectMoveFv,
    (void*)CfObjectMove_UnkVirtualFunc15__Q22cf12CfObjectMoveFv, (void*)CfObjectMove_attachEffectSlot__Q22cf12CfObjectMoveFv, (void*)CfObjectMove_setEffectSlotBit__Q22cf12CfObjectMoveFv, (void*)CfObjectMove_transferSlotBits__Q22cf12CfObjectMoveFv,
    (void*)CfObjectMove_isEffectSlotUsed__Q22cf12CfObjectMoveFv, (void*)CfObjectMove_signalEffectSlot__Q22cf12CfObjectMoveFv, (void*)CfObjectMove_flushEffectSlots__Q22cf12CfObjectMoveFv, (void*)CfObjectMove_setAllEffectBits__Q22cf12CfObjectMoveFv,
    (void*)CfObjectMove_loadResourceById__Q22cf12CfObjectMoveFv,
};
void* lbl_eu_80529D70[] = {
    (void*)&lbl_eu_80661900, 0, (void*)&lbl_eu_806618F8, 0,
    (void*)&lbl_eu_806618F0, 0, (void*)&lbl_eu_806618E8, 0,
    (void*)&lbl_eu_806618D8, 0, 0, 0,
};
void* lbl_eu_80529DA0[] = {
    (void*)&lbl_eu_806619A0, 0, (void*)CActorState_setObjStateBit3__Q22cf11CActorStateFv, (void*)&lbl_eu_806619A0,
    (void*)0xFFFFFFF8, (void*)CBattleState_getOwner__Q22cf13CfObjectActorFv, (void*)CBattleState_callOwnerObject__Q22cf13CfObjectActorFv, (void*)CBattleState_getParamPtrName__Q22cf13CfObjectActorFv,
    (void*)CBattleState_UnkVirtualFunc4__Q22cf12CBattleStateFv, (void*)CBattleState_enterStatusEntry__Q22cf12CBattleStateFPQ22cf17CBattleStateEntry, (void*)CBattleState_addStagingEntry__Q22cf12CBattleStateFPQ22cf17CBattleStateEntry, (void*)CBattleState_clearStatusId__Q22cf12CBattleStateFv,
    (void*)CBattleState_applyEventEntry__Q22cf12CBattleStateFPQ22cf17CBattleStateEntry, (void*)CBattleState_UnkVirtualFunc9__Q22cf12CBattleStateFv, (void*)CBattleState_removeKeyedEntries__Q22cf12CBattleStateFPQ22cf17CBattleStateEntry, (void*)CBattleState_clearEntriesByMask__Q22cf12CBattleStateFUl,
    (void*)CBattleState_UnkVirtualFunc12__Q22cf12CBattleStateFv, (void*)CBattleState_getStatusSlot__Q22cf12CBattleStateFi, (void*)CBattleState_UnkVirtualFunc14__Q22cf12CBattleStateFv, (void*)CBattleState_UnkVirtualFunc15__Q22cf12CBattleStateFv,
    (void*)CBattleState_UnkVirtualFunc16__Q22cf12CBattleStateFv, (void*)CBattleState_notifyEntryUpdated__Q22cf13CfObjectActorFv, (void*)CBattleState_getLinkedActorId__Q22cf13CfObjectActorFv, (void*)CBattleState_UnkVirtualFunc19__Q22cf12CBattleStateFv,
    (void*)CBattleState_getEntryByIndex__Q22cf12CBattleStateFv, (void*)CBattleState_fetchStatusEntry__Q22cf12CBattleStateFv, (void*)CBattleState_getEventEntry__Q22cf12CBattleStateFv, (void*)CBattleState_getStatusEntry__Q22cf12CBattleStateFv,
    (void*)CBattleState_copyStatusEntries__Q22cf12CBattleStateFv, (void*)CBattleState_getEntryArray__Q22cf12CBattleStateFv, (void*)CBattleState_applyArtsTable__Q22cf12CBattleStateFPCQ22cf20CBattleStateSrcEntry, (void*)CBattleState_getStatusBlock__Q22cf12CBattleStateFv,
    (void*)CBattleState_getStagingRecord__Q22cf12CBattleStateFv, (void*)CBattleState_clearStagingEntries__Q22cf12CBattleStateFv, (void*)CBattleState_UnkVirtualFunc30__Q22cf12CBattleStateFv, (void*)CBattleState_getEventMask__Q22cf12CBattleStateFUl,
    (void*)CBattleState_UnkVirtualFunc32__Q22cf12CBattleStateFv, (void*)CBattleState_getStatusMask__Q22cf12CBattleStateFUl, (void*)CActorParam_UnkVirtualFunc1__Q22cf13CfObjectActorFv, (void*)CActorParam_UnkVirtualFunc2__Q22cf13CfObjectActorFv,
    (void*)CActorParam_UnkVirtualFunc3__Q22cf13CfObjectActorFv, (void*)CActorParam_resetArtsStatus__Q22cf10CfObjectPcFv, (void*)CActorParam_commitArtsStatus__Q22cf11CActorParamFv, (void*)CActorParam_resetArtsStatus__Q22cf13CfObjectActorFv,
    (void*)CActorParam_setupActorState__Q22cf11CActorParamFv, (void*)CActorParam_resetArtsMatrix__Q22cf11CActorParamFv, (void*)CActorParam_UnkVirtualFunc9__Q22cf11CActorParamFv, (void*)CActorParam_applyTargetEvent__Q22cf11CActorParamFv,
    (void*)CActorParam_applyStatusHeal__Q22cf11CActorParamFv, (void*)CActorParam_applyEventStatus__Q22cf11CActorParamFv, (void*)CActorParam_resetGaugesAndRefresh__Q22cf11CActorParamFv, (void*)CActorParam_setStatByte__Q22cf11CActorParamFUc,
    (void*)CActorParam_UnkVirtualFunc15__Q22cf11CActorParamFv, (void*)CActorParam_setStatScale__Q22cf11CActorParamFf, (void*)CActorParam_UnkVirtualFunc17__Q22cf11CActorParamFv, (void*)CActorParam_UnkVirtualFunc18__Q22cf11CActorParamFv,
    (void*)CActorParam_getActorType__Q22cf11CActorParamFv, (void*)CActorParam_UnkVirtualFunc20__Q22cf11CActorParamFv, (void*)CActorParam_setScale__Q22cf13CfObjectActorFv, (void*)CActorParam_UnkVirtualFunc22__Q22cf11CActorParamFv,
    (void*)CActorParam_getScale__Q22cf13CfObjectActorFv, (void*)CActorParam_UnkVirtualFunc24__Q22cf11CActorParamFv, (void*)CActorParam_UnkVirtualFunc25__Q22cf11CActorParamFv, (void*)CActorParam_getArtsLevel__Q22cf11CActorParamFv,
    (void*)CActorParam_UnkVirtualFunc27__Q22cf11CActorParamFv, (void*)CActorParam_UnkVirtualFunc28__Q22cf11CActorParamFv, (void*)CActorParam_getActorLevel__Q22cf11CActorParamFv, (void*)CActorParam_UnkVirtualFunc30__Q22cf11CActorParamFv,
    (void*)CActorParam_UnkVirtualFunc31__Q22cf11CActorParamFv, (void*)CActorParam_UnkVirtualFunc32__Q22cf11CActorParamFv, (void*)CActorParam_setHp__Q22cf13CfObjectActorFv, (void*)CActorParam_addHp__Q22cf13CfObjectActorFv,
    (void*)CActorParam_applyDamage__Q22cf13CfObjectActorFv, (void*)CActorParam_UnkVirtualFunc36__Q22cf11CActorParamFv, (void*)CActorParam_getHp__Q22cf11CActorParamFv, (void*)CActorParam_getDamageScale__Q22cf11CActorParamFv,
    (void*)CActorParam_UnkVirtualFunc39__Q22cf11CActorParamFv, (void*)CActorParam_applyStoredHp__Q22cf11CActorParamFv, (void*)CActorParam_UnkVirtualFunc41__Q22cf11CActorParamFv, (void*)CActorParam_UnkVirtualFunc42__Q22cf11CActorParamFv,
    (void*)CActorParam_UnkVirtualFunc43__Q22cf11CActorParamFv, (void*)CActorParam_UnkVirtualFunc44__Q22cf11CActorParamFv, (void*)CActorParam_UnkVirtualFunc45__Q22cf11CActorParamFv, (void*)CActorParam_applyStoredHitCharge__Q22cf11CActorParamFv,
    (void*)CActorParam_setTensionPoints__Q22cf11CActorParamFv, (void*)CActorParam_addArtsGauge__Q22cf11CActorParamFv, (void*)CActorParam_UnkVirtualFunc49__Q22cf11CActorParamFv, (void*)CActorParam_UnkVirtualFunc50__Q22cf11CActorParamFv,
    (void*)CActorParam_UnkVirtualFunc51__Q22cf11CActorParamFv, (void*)CActorParam_applyStoredArtsGauge__Q22cf11CActorParamFv, (void*)CActorParam_UnkVirtualFunc53__Q22cf11CActorParamFv, (void*)CActorParam_addGauge__Q22cf13CfObjectActorFv,
    (void*)CActorParam_setGaugeFlagA__Q22cf11CActorParamFUs, (void*)CActorParam_UnkVirtualFunc56__Q22cf11CActorParamFv, (void*)CActorParam_UnkVirtualFunc57__Q22cf11CActorParamFv, (void*)CActorParam_UnkVirtualFunc58__Q22cf11CActorParamFv,
    (void*)CActorParam_UnkVirtualFunc59__Q22cf11CActorParamFv, (void*)CActorParam_addSecondGauge__Q22cf13CfObjectActorFv, (void*)CActorParam_setGaugeFlagB__Q22cf11CActorParamFUs, (void*)CActorParam_UnkVirtualFunc62__Q22cf11CActorParamFv,
    (void*)CActorParam_UnkVirtualFunc63__Q22cf11CActorParamFv, (void*)CActorParam_UnkVirtualFunc64__Q22cf11CActorParamFv, (void*)CActorParam_setGaugeRateA__Q22cf11CActorParamFf, (void*)CActorParam_UnkVirtualFunc66__Q22cf11CActorParamFv,
    (void*)CActorParam_resetGaugeBattleEntry__Q22cf11CActorParamFv, (void*)CActorParam_setGaugeRateB__Q22cf11CActorParamFf, (void*)CActorParam_UnkVirtualFunc69__Q22cf11CActorParamFv, (void*)CActorParam_resetSecondGaugeBattleEntry__Q22cf11CActorParamFv,
    (void*)CActorParam_UnkVirtualFunc71__Q22cf11CActorParamFv, (void*)CActorParam_UnkVirtualFunc72__Q22cf11CActorParamFv, (void*)CActorParam_UnkVirtualFunc73__Q22cf11CActorParamFv, (void*)CActorParam_setMoveRate__Q22cf11CActorParamFf,
    (void*)CActorParam_UnkVirtualFunc75__Q22cf11CActorParamFv, (void*)CActorParam_getMoveRate__Q22cf11CActorParamFv, (void*)CActorParam_UnkVirtualFunc77__Q22cf11CActorParamFv, (void*)CActorParam_UnkVirtualFunc78__Q22cf11CActorParamFv,
    (void*)CActorParam_UnkVirtualFunc79__Q22cf11CActorParamFv, (void*)CActorParam_UnkVirtualFunc80__Q22cf11CActorParamFv, (void*)CActorParam_setSpentCurrency__Q22cf11CActorParamFUl, (void*)CActorParam_addSpentCurrency__Q22cf11CActorParamFUl,
    (void*)CActorParam_addAccumCurrency__Q22cf11CActorParamFUl, (void*)CActorParam_UnkVirtualFunc84__Q22cf11CActorParamFv, (void*)CActorParam_getSpentCurrency__Q22cf11CActorParamFv, (void*)CActorParam_getCurrencyBalance__Q22cf10CfObjectPcFv,
    (void*)CActorParam_UnkVirtualFunc87__Q22cf11CActorParamFv, (void*)CActorParam_UnkVirtualFunc88__Q22cf10CfObjectPcFv, (void*)CActorParam_UnkVirtualFunc89__Q22cf11CActorParamFv, (void*)CActorParam_addSecondCurrency__Q22cf11CActorParamFUl,
    (void*)CActorParam_UnkVirtualFunc91__Q22cf11CActorParamFv, (void*)CActorParam_UnkVirtualFunc92__Q22cf11CActorParamFv, (void*)CActorParam_UnkVirtualFunc93__Q22cf11CActorParamFv, (void*)CActorParam_getArtsDataBlock__Q22cf11CActorParamFv,
    (void*)CActorParam_getStatusSyncBlock__Q22cf11CActorParamFv, (void*)CActorParam_getArtsSyncBlock__Q22cf11CActorParamFv, (void*)CActorParam_UnkVirtualFunc97__Q22cf11CActorParamFv, (void*)CActorParam_UnkVirtualFunc98__Q22cf11CActorParamFv,
    (void*)CActorParam_UnkVirtualFunc99__Q22cf11CActorParamFv, (void*)CActorParam_getBattleParams__Q22cf11CActorParamFv, (void*)CActorParam_UnkVirtualFunc101__Q22cf11CActorParamFv, (void*)CActorParam_UnkVirtualFunc102__Q22cf11CActorParamFv,
    (void*)CActorParam_UnkVirtualFunc103__Q22cf11CActorParamFv, (void*)CActorParam_UnkVirtualFunc104__Q22cf11CActorParamFv, (void*)CActorParam_UnkVirtualFunc105__Q22cf11CActorParamFv, (void*)CActorParam_UnkVirtualFunc106__Q22cf11CActorParamFv,
    (void*)CActorParam_UnkVirtualFunc107__Q22cf11CActorParamFv, (void*)CActorParam_UnkVirtualFunc108__Q22cf11CActorParamFv, (void*)CActorParam_UnkVirtualFunc109__Q22cf11CActorParamFv, (void*)CActorParam_UnkVirtualFunc110__Q22cf11CActorParamFv,
    (void*)CActorParam_UnkVirtualFunc111__Q22cf11CActorParamFv, (void*)CActorParam_UnkVirtualFunc112__Q22cf11CActorParamFv, (void*)CActorParam_UnkVirtualFunc113__Q22cf11CActorParamFv, (void*)CActorParam_UnkVirtualFunc114__Q22cf11CActorParamFv,
    (void*)CActorParam_UnkVirtualFunc115__Q22cf11CActorParamFv, (void*)CActorParam_setGaugeFloat__Q22cf11CActorParamFf, (void*)CActorParam_UnkVirtualFunc117__Q22cf11CActorParamFv, (void*)CActorParam_UnkVirtualFunc118__Q22cf11CActorParamFv,
    (void*)CActorParam_UnkVirtualFunc119__Q22cf11CActorParamFv, (void*)CActorParam_UnkVirtualFunc120__Q22cf11CActorParamFv, (void*)CActorParam_UnkVirtualFunc121__Q22cf11CActorParamFv, (void*)CActorParam_UnkVirtualFunc122__Q22cf11CActorParamFv,
    (void*)CActorParam_UnkVirtualFunc123__Q22cf11CActorParamFv, (void*)CActorParam_UnkVirtualFunc124__Q22cf11CActorParamFv, (void*)CActorParam_getArtsSlotIds__Q22cf11CActorParamFv, (void*)CActorParam_getCopyParam__Q22cf11CActorParamFv,
    (void*)CActorParam_getStatusTable__Q22cf11CActorParamFv, (void*)CActorParam_UnkVirtualFunc128__Q22cf11CActorParamFv, (void*)CActorParam_getBattleStats__Q22cf11CActorParamFv, (void*)CActorParam_UnkVirtualFunc130__Q22cf11CActorParamFv,
    (void*)CActorParam_UnkVirtualFunc131__Q22cf11CActorParamFv, (void*)CActorParam_getMoveRecord__Q22cf11CActorParamFv, (void*)CActorParam_getStatsSlot__Q22cf11CActorParamFv, (void*)CActorParam_UnkVirtualFunc134__Q22cf11CActorParamFv,
    (void*)CActorParam_reinitBattleStats__Q22cf11CActorParamFv, (void*)CActorParam_UnkVirtualFunc136__Q22cf11CActorParamFv, (void*)CActorParam_UnkVirtualFunc137__Q22cf11CActorParamFv, (void*)CActorParam_isBattleLocked__Q22cf11CActorParamFv,
    (void*)CActorParam_UnkVirtualFunc139__Q22cf11CActorParamFv, (void*)CActorParam_addHate__Q22cf13CfObjectActorFv, (void*)CActorParam_resetGaugeEntry__Q22cf11CActorParamFv, (void*)CActorParam_resetAllGaugeEntries__Q22cf11CActorParamFv,
    (void*)CActorParam_UnkVirtualFunc143__Q22cf11CActorParamFv, (void*)CActorParam_findGaugeEntry__Q22cf11CActorParamFv, (void*)CActorParam_UnkVirtualFunc145__Q22cf11CActorParamFv, (void*)CActorParam_UnkVirtualFunc146__Q22cf11CActorParamFv,
    (void*)CActorParam_UnkVirtualFunc147__Q22cf11CActorParamFv, (void*)CActorParam_UnkVirtualFunc148__Q22cf11CActorParamFv, (void*)CActorParam_findMaxGaugeEntry__Q22cf11CActorParamFv, (void*)CActorParam_findMinGaugeEntry__Q22cf11CActorParamFv,
    (void*)CActorParam_UnkVirtualFunc151__Q22cf11CActorParamFv, (void*)CActorParam_getTensionStatus__Q22cf11CActorParamFv, (void*)CActorParam_accumulateTension__Q22cf11CActorParamFv, (void*)CActorParam_raiseTensionValue__Q22cf11CActorParamFv,
    (void*)CActorParam_UnkVirtualFunc155__Q22cf11CActorParamFv, (void*)CActorParam_resetTensionState__Q22cf11CActorParamFv, (void*)CActorParam_getStatusCount__Q22cf11CActorParamFv, (void*)CActorParam_UnkVirtualFunc158__Q22cf11CActorParamFv,
    (void*)CActorParam_UnkVirtualFunc159__Q22cf11CActorParamFv, (void*)CActorParam_initStatusCounter__Q22cf11CActorParamFv, (void*)CActorParam_UnkVirtualFunc161__Q22cf11CActorParamFv, (void*)CActorParam_UnkVirtualFunc162__Q22cf11CActorParamFv,
    (void*)CActorParam_UnkVirtualFunc163__Q22cf11CActorParamFv, (void*)CActorParam_getActorModeFlags__Q22cf11CActorParamFv, (void*)CActorParam_getBattleHitFlags__Q22cf11CActorParamFv, (void*)CActorParam_UnkVirtualFunc166__Q22cf10CfObjectPcFv,
    (void*)CActorParam_UnkVirtualFunc167__Q22cf10CfObjectPcFv, (void*)CActorParam_UnkVirtualFunc168__Q22cf11CActorParamFv, (void*)CActorParam_UnkVirtualFunc169__Q22cf11CActorParamFv, (void*)CActorParam_UnkVirtualFunc170__Q22cf11CActorParamFv,
    (void*)CActorParam_UnkVirtualFunc171__Q22cf11CActorParamFv, (void*)CActorParam_UnkVirtualFunc172__Q22cf11CActorParamFv, (void*)CActorParam_UnkVirtualFunc173__Q22cf10CfObjectPcFv, (void*)CActorParam_UnkVirtualFunc174__Q22cf11CActorParamFv,
    (void*)CActorParam_UnkVirtualFunc175__Q22cf11CActorParamFv, (void*)CActorParam_UnkVirtualFunc176__Q22cf10CfObjectPcFv, (void*)CActorParam_UnkVirtualFunc177__Q22cf11CActorParamFv, (void*)CActorParam_UnkVirtualFunc178__Q22cf10CfObjectPcFv,
    (void*)CActorParam_UnkVirtualFunc179__Q22cf13CfObjectActorFv, (void*)CActorParam_UnkVirtualFunc180__Q22cf13CfObjectActorFv, (void*)CActorParam_UnkVirtualFunc181__Q22cf11CActorParamFv, (void*)&lbl_eu_806619A0,
    (void*)0xFFFFCC80, (void*)CAIAction_UnkVirtualFunc1__Q22cf9CAIActionFv, (void*)CAIAction_UnkVirtualFunc2__Q22cf9CAIActionFv, (void*)&lbl_eu_806619A0,
    (void*)0xFFFFC164, (void*)CObjectState_setStateBitMask__Q22cf12CObjectStateFv, (void*)CObjectState_checkStateFlags__Q22cf12CObjectStateFv, (void*)CObjectState_setStateBitFlag__Q22cf12CObjectStateFv,
    (void*)CObjectState_clearStateWord4__Q22cf12CObjectStateFv, (void*)CObjectState_applyStateFlags__Q22cf12CObjectStateFv, (void*)CObjectState_clearStateWord8__Q22cf12CObjectStateFv, (void*)CObjectState_clearStateFlags8__Q22cf12CObjectStateFv,
    (void*)CObjectState_UnkVirtualFunc8__Q22cf12CObjectStateFv, (void*)CObjectState_UnkVirtualFunc9__Q22cf12CObjectStateFv, (void*)CObjectState_setStateBitMask0__Q22cf12CObjectStateFv, (void*)CObjectState_getStateData__Q22cf12CObjectStateFv,
    (void*)CObjectState_setStateBitMask2__Q22cf12CObjectStateFv, (void*)CObjectState_setStateBitMask3__Q22cf12CObjectStateFv, (void*)CObjectParam_UnkVirtualFunc1__Q22cf12CObjectParamFv, (void*)CObjectParam_getParamPtr__Q22cf13CfObjectActorFv,
    (void*)CObjectParam_hasObjectName__Q22cf12CObjectParamFv, (void*)CObjectParam_callPcFunc800C0__Q22cf10CfObjectPcFv, (void*)CObjectParam_getSelfObjectId__Q22cf12CObjectParamFv, (void*)CObjectParam_signalActionEnd__Q22cf12CObjectParamFv,
    (void*)finalizePcCleanup__Q22cf10CfObjectPcFv, (void*)CfObject_pcInitEventStat__Q22cf10CfObjectPcFv, (void*)CfObject_syncEnableState__Q22cf10CfObjectPcFv, (void*)CfObject_pcRunMoveUpdate__Q22cf10CfObjectPcFv,
    (void*)CfObject_UnkVirtualFunc5__Q22cf12CfObjectMoveFv, (void*)CfObject_pcSyncArtsEntry__Q22cf10CfObjectPcFv, (void*)CfObject_UnkVirtualFunc7__Q22cf12CfObjectMoveFv, (void*)CfObject_notifyEventDone__Q22cf13CfObjectModelFv,
    (void*)CfObject_isMoveActiveNow__Q22cf12CfObjectMoveFv, (void*)CfObject_forwardSubObject__Q22cf12CfObjectMoveFv, (void*)CfObject_UnkVirtualFunc11__Q22cf8CfObjectFv, (void*)CfObject_UnkVirtualFunc12__Q22cf12CfObjectMoveFv,
    (void*)CfObject_queryTargetState__Q22cf12CfObjectMoveFv, (void*)CfObject_pushRefreshValue__Q22cf13CfObjectActorFf, (void*)CfObject_getMoveSpeedRate__Q22cf12CfObjectMoveFv, (void*)CfObject_pushRefreshExtra__Q22cf12CfObjectMoveFv,
    (void*)CfObject_readRefreshValue__Q22cf12CfObjectMoveFv, (void*)CfObject_checkTargetState__Q22cf12CfObjectMoveFv, (void*)CfObject_setMoveTargetVec__Q22cf12CfObjectMoveFv, (void*)CfObject_UnkVirtualFunc20__Q22cf13CfObjectModelFv,
    (void*)CfObject_UnkVirtualFunc21__Q22cf8CfObjectFv, (void*)CfObject_UnkVirtualFunc22__Q22cf12CfObjectMoveFv, (void*)CfObject_getPosVector__Q22cf12CfObjectMoveFv, (void*)CfObject_UnkVirtualFunc24__Q22cf13CfObjectModelFv,
    (void*)CfObject_UnkVirtualFunc25__Q22cf12CfObjectMoveFv, (void*)CfObject_UnkVirtualFunc26__Q22cf12CfObjectMoveFv, (void*)CfObject_UnkVirtualFunc27__Q22cf12CfObjectMoveFPv, (void*)CfObject_UnkVirtualFunc28__Q22cf13CfObjectModelFv,
    (void*)CfObject_setMoveHeadAngle__Q22cf12CfObjectMoveFv, (void*)CfObject_UnkVirtualFunc30__Q22cf12CfObjectMoveFv, (void*)CfObject_getMoveHeadAngle__Q22cf13CfObjectActorFv, (void*)CfObject_UnkVirtualFunc32__Q22cf12CfObjectMoveFv,
    (void*)CfObject_UnkVirtualFunc33__Q22cf12CfObjectMoveFv, (void*)CfObject_UnkVirtualFunc34__Q22cf13CfObjectModelFv, (void*)CfObject_UnkVirtualFunc35__Q22cf13CfObjectModelFv, (void*)CfObject_UnkVirtualFunc36__Q22cf13CfObjectModelFv,
    (void*)CfObject_UnkVirtualFunc37__Q22cf12CfObjectMoveFv, (void*)CfObject_UnkVirtualFunc38__Q22cf12CfObjectMoveFv, (void*)CfObject_UnkVirtualFunc39__Q22cf12CfObjectMoveFv, (void*)CfObject_UnkVirtualFunc40__Q22cf12CfObjectMoveFv,
    (void*)CfObject_UnkVirtualFunc41__Q22cf8CfObjectFv, (void*)CfObject_UnkVirtualFunc42__Q22cf12CfObjectMoveFv, (void*)CfObject_UnkVirtualFunc43__Q22cf12CfObjectMoveFv, (void*)CfObject_UnkVirtualFunc44__Q22cf8CfObjectFv,
    (void*)CfObject_UnkVirtualFunc45__Q22cf12CfObjectMoveFv, (void*)CfObject_setMoveTargetPtr__Q22cf12CfObjectMoveFv, (void*)CfObject_createMoveTarget__Q22cf12CfObjectMoveFv, (void*)CfObject_getCurrentTarget__Q22cf12CfObjectMoveFv,
    (void*)CfObject_forwardNpcAction__Q22cf12CfObjectMoveFv, (void*)CfObject_UnkVirtualFunc50__Q22cf12CfObjectMoveFv, (void*)CfObject_UnkVirtualFunc51__Q22cf12CfObjectMoveFv, (void*)CfObject_UnkVirtualFunc52__Q22cf13CfObjectModelFv,
    (void*)CfObject_UnkVirtualFunc53__Q22cf13CfObjectModelFv, (void*)CfObject_UnkVirtualFunc54__Q22cf13CfObjectModelFv, (void*)CfObject_UnkVirtualFunc55__Q22cf13CfObjectModelFv, (void*)CfObject_UnkVirtualFunc56__Q22cf13CfObjectModelFv,
    (void*)CfObject_UnkVirtualFunc57__Q22cf12CfObjectMoveFv, (void*)CfObject_getMoveRateScale__Q22cf12CfObjectMoveFv, (void*)CfObject_UnkVirtualFunc59__Q22cf12CfObjectMoveFv, (void*)CfObject_UnkVirtualFunc60__Q22cf12CfObjectMoveFv,
    (void*)CfObject_setAnimSlotEntry__Q22cf12CfObjectMoveFv, (void*)CfObject_UnkVirtualFunc62__Q22cf12CfObjectMoveFv, (void*)CfObject_UnkVirtualFunc63__Q22cf13CfObjectModelFv, (void*)CfObject_setMoveBusyState__Q22cf12CfObjectMoveFi,
    (void*)CfObject_setMoveReadyFlag__Q22cf12CfObjectMoveFv, (void*)CfObject_UnkVirtualFunc66__Q22cf12CfObjectMoveFv, (void*)CfObject_UnkVirtualFunc67__Q22cf13CfObjectModelFv, (void*)CfObject_UnkVirtualFunc68__Q22cf13CfObjectModelFv,
    (void*)CfObject_UnkVirtualFunc69__Q22cf13CfObjectModelFv, (void*)CfObject_UnkVirtualFunc70__Q22cf12CfObjectMoveFv, (void*)CfObject_UnkVirtualFunc71__Q22cf13CfObjectModelFv, (void*)CfObject_UnkVirtualFunc72__Q22cf13CfObjectModelFv,
    (void*)CfObject_UnkVirtualFunc73__Q22cf13CfObjectModelFv, (void*)CfObjectModel_UnkVirtualFunc1__Q22cf12CfObjectMoveFv, (void*)CfObjectModel_UnkVirtualFunc2__Q22cf12CfObjectMoveFv, (void*)CfObjectModel_UnkVirtualFunc3__Q22cf13CfObjectModelFv,
    (void*)CfObjectModel_UnkVirtualFunc4__Q22cf12CfObjectMoveFv, (void*)CfObjectModel_UnkVirtualFunc5__Q22cf12CfObjectMoveFv, (void*)CfObjectModel_UnkVirtualFunc6__Q22cf12CfObjectMoveFv, (void*)CfObjectModel_UnkVirtualFunc7__Q22cf13CfObjectModelFv,
    (void*)CfObjectModel_UnkVirtualFunc8__Q22cf13CfObjectModelFv, (void*)CfObjectModel_UnkVirtualFunc9__Q22cf13CfObjectModelFv, (void*)CfObjectModel_UnkVirtualFunc10__Q22cf13CfObjectModelFv, (void*)CfObjectModel_UnkVirtualFunc11__Q22cf13CfObjectModelFv,
    (void*)CfObjectModel_UnkVirtualFunc12__Q22cf13CfObjectModelFv, (void*)CfObjectModel_UnkVirtualFunc13__Q22cf13CfObjectModelFv, (void*)CfObjectModel_UnkVirtualFunc14__Q22cf13CfObjectModelFv, (void*)CfObjectModel_UnkVirtualFunc15__Q22cf13CfObjectModelFv,
    (void*)CfObjectModel_UnkVirtualFunc16__Q22cf13CfObjectModelFv, (void*)CfObjectModel_UnkVirtualFunc17__Q22cf13CfObjectModelFv, (void*)CfObjectModel_UnkVirtualFunc18__Q22cf12CfObjectMoveFv, (void*)CfObjectModel_UnkVirtualFunc19__Q22cf12CfObjectMoveFv,
    (void*)CfObjectModel_UnkVirtualFunc20__Q22cf13CfObjectModelFv, (void*)CfObjectMove_dropMoveRequest__Q22cf12CfObjectMoveFv, (void*)CfObjectMove_detachModelList__Q22cf12CfObjectMoveFv, (void*)CfObjectMove_releaseSlotById__Q22cf12CfObjectMoveFv,
    (void*)CfObjectMove_recordMoveValue__Q22cf12CfObjectMoveFv, (void*)CfObjectMove_getMovementRate__Q22cf12CfObjectMoveFv, (void*)CfObjectMove_storeActionSrcId__Q22cf13CfObjectActorFv, (void*)CfObjectMove_refreshNpcState__Q22cf12CfObjectMoveFv,
    (void*)CfObjectMove_updateNpcTarget__Q22cf12CfObjectMoveFv, (void*)CfObjectMove_queryNpcAdvance__Q22cf12CfObjectMoveFv, (void*)CfObjectMove_UnkVirtualFunc10__Q22cf12CfObjectMoveFv, (void*)CfObjectMove_writeTargetField__Q22cf12CfObjectMoveFv,
    (void*)CfObjectMove_advanceNpcTarget__Q22cf12CfObjectMoveFv, (void*)CfObjectMove_commandNpcTarget__Q22cf12CfObjectMoveFv, (void*)CfObjectMove_restartNpcTarget__Q22cf12CfObjectMoveFv, (void*)CfObjectMove_hasActorScale__Q22cf13CfObjectActorFv,
    (void*)CfObjectMove_attachEffectSlot__Q22cf10CfObjectPcFv, (void*)CfObjectMove_setEffectSlotBit__Q22cf12CfObjectMoveFv, (void*)CfObjectMove_transferSlotBits__Q22cf12CfObjectMoveFv, (void*)CfObjectMove_isEffectSlotUsed__Q22cf12CfObjectMoveFv,
    (void*)CfObjectMove_signalEffectSlot__Q22cf12CfObjectMoveFv, (void*)CfObjectMove_flushEffectSlots__Q22cf12CfObjectMoveFv, (void*)CfObjectMove_setAllEffectBits__Q22cf12CfObjectMoveFv, (void*)CfObjectMove_loadResourceById__Q22cf12CfObjectMoveFv,
    (void*)__dt__Q22cf10CfObjectPcFv, (void*)CfObjectActor_hasActorScale__Q22cf13CfObjectActorFv, (void*)CfObjectActor_clearStatusPair__Q22cf13CfObjectActorFv, (void*)CfObjectActor_flushStatusPair__Q22cf13CfObjectActorFv,
    (void*)CfObjectActor_takeDamageValue__Q22cf13CfObjectActorFv, (void*)CfObjectActor_readFacingAngle__Q22cf13CfObjectActorFv, (void*)CfObjectActor_getAdjustedFacing__Q22cf13CfObjectActorFv, (void*)CfObjectActor_readActionScale__Q22cf13CfObjectActorFv,
    (void*)CfObjectActor_sharesMoveFlags__Q22cf13CfObjectActorFv, (void*)CfObjectActor_pushRefreshValue__Q22cf13CfObjectActorFv, (void*)CfObjectActor_storeActionSrcId__Q22cf13CfObjectActorFv, (void*)CfObjectActor_getActionSrcId__Q22cf13CfObjectActorFv,
    (void*)CfObjectActor_clearActionSrcId__Q22cf13CfObjectActorFv, (void*)initialize__Q22cf10CfObjectPcFv, (void*)enablePcFlag__Q22cf10CfObjectPcFv, (void*)updatePc__Q22cf10CfObjectPcFv,
    (void*)syncArtsEntry__Q22cf10CfObjectPcFv, (void*)refreshArtsData__Q22cf10CfObjectPcFv, (void*)scanArtsEntries__Q22cf10CfObjectPcFv, (void*)resetArtsState__Q22cf10CfObjectPcFv,
    (void*)applyArtsParam__Q22cf10CfObjectPcFv, (void*)dispatchPlayerBranch__Q22cf10CfObjectPcFv, (void*)resetActionTable__Q22cf10CfObjectPcFv, (void*)setupActionTable__Q22cf10CfObjectPcFv,
    (void*)triggerActionRefreshA__Q22cf10CfObjectPcFv, (void*)triggerActionRefreshB__Q22cf10CfObjectPcFv, (void*)handleMoveState__Q22cf10CfObjectPcFv, 0,
};
void* lbl_eu_8052A3B0[] = {
    (void*)&lbl_eu_80661CB0, 0, (void*)CObjectState_setStateBitMask__Q22cf12CObjectStateFv, (void*)CObjectState_checkStateFlags__Q22cf12CObjectStateFv,
    (void*)CObjectState_setStateBitFlag__Q22cf12CObjectStateFv, (void*)CObjectState_clearStateWord4__Q22cf12CObjectStateFv, (void*)CObjectState_applyStateFlags__Q22cf12CObjectStateFv, (void*)CObjectState_clearStateWord8__Q22cf12CObjectStateFv,
    (void*)CObjectState_clearStateFlags8__Q22cf12CObjectStateFv, (void*)CObjectState_UnkVirtualFunc8__Q22cf12CObjectStateFv, (void*)CObjectState_UnkVirtualFunc9__Q22cf12CObjectStateFv, (void*)CObjectState_setStateBitMask0__Q22cf12CObjectStateFv,
    (void*)CObjectState_getStateData__Q22cf12CObjectStateFv, (void*)CObjectState_setStateBitMask2__Q22cf12CObjectStateFv, (void*)CObjectState_setStateBitMask3__Q22cf12CObjectStateFv, (void*)CObjectParam_UnkVirtualFunc1__Q22cf12CObjectParamFv,
    (void*)CObjectParam_UnkVirtualFunc2__Q22cf12CObjectParamFv, (void*)CObjectParam_hasObjectName__Q22cf12CObjectParamFv, (void*)loadPointData__Q22cf13CfObjectPointFv, (void*)CObjectParam_getSelfObjectId__Q22cf12CObjectParamFv,
    (void*)CObjectParam_signalActionEnd__Q22cf12CObjectParamFv, (void*)__dt__Q22cf13CfObjectPointFv, (void*)validatePointState__Q22cf13CfObjectPointFv, (void*)resetPointFlags__Q22cf13CfObjectPointFv,
    (void*)updatePointState__Q22cf13CfObjectPointFv, (void*)CfObject_UnkVirtualFunc5__Q22cf8CfObjectFv, (void*)releasePointLink__Q22cf13CfObjectPointFv, (void*)notifyChildUpdate__Q22cf13CfObjectPointFv,
    (void*)setChildPoint__Q22cf13CfObjectPointFv, (void*)CfObject_isMoveActiveNow__Q22cf8CfObjectFv, (void*)CfObject_forwardSubObject__Q22cf8CfObjectFv, (void*)CfObject_UnkVirtualFunc11__Q22cf8CfObjectFv,
    (void*)CfObject_UnkVirtualFunc12__Q22cf8CfObjectFv, (void*)CfObject_queryTargetState__Q22cf8CfObjectFv, (void*)CfObject_pushRefreshValue__Q22cf8CfObjectFf, (void*)CfObject_getMoveSpeedRate__Q22cf8CfObjectFv,
    (void*)CfObject_pushRefreshExtra__Q22cf8CfObjectFv, (void*)CfObject_readRefreshValue__Q22cf8CfObjectFv, (void*)CfObject_checkTargetState__Q22cf8CfObjectFv, (void*)CfObjectPoint_CopyPosWords,
    (void*)setPointPosition__Q22cf13CfObjectPointFv, (void*)CfObject_UnkVirtualFunc21__Q22cf8CfObjectFv, (void*)CfObject_UnkVirtualFunc22__Q22cf8CfObjectFv, (void*)CfObject_getPosVector__Q22cf8CfObjectFv,
    (void*)CfObject_UnkVirtualFunc24__Q22cf8CfObjectFv, (void*)CfObject_UnkVirtualFunc25__Q22cf8CfObjectFv, (void*)CfObject_UnkVirtualFunc26__Q22cf8CfObjectFv, (void*)CfObject_UnkVirtualFunc27__Q22cf8CfObjectFPv,
    (void*)CfObject_UnkVirtualFunc28__Q22cf8CfObjectFv, (void*)CfObject_setMoveHeadAngle__Q22cf8CfObjectFf, (void*)CfObject_UnkVirtualFunc30__Q22cf8CfObjectFv, (void*)CfObject_getMoveHeadAngle__Q22cf8CfObjectFv,
    (void*)CfObject_UnkVirtualFunc32__Q22cf8CfObjectFv, (void*)CfObject_UnkVirtualFunc33__Q22cf8CfObjectFv, (void*)CfObject_UnkVirtualFunc34__Q22cf8CfObjectFv, (void*)CfObject_UnkVirtualFunc35__Q22cf8CfObjectFv,
    (void*)CfObject_UnkVirtualFunc36__Q22cf8CfObjectFv, (void*)CfObject_UnkVirtualFunc37__Q22cf8CfObjectFv, (void*)CfObject_UnkVirtualFunc38__Q22cf8CfObjectFv, (void*)CfObject_UnkVirtualFunc39__Q22cf8CfObjectFv,
    (void*)CfObject_UnkVirtualFunc40__Q22cf8CfObjectFv, (void*)CfObject_UnkVirtualFunc41__Q22cf8CfObjectFv, (void*)CfObject_UnkVirtualFunc42__Q22cf8CfObjectFv, (void*)CfObject_UnkVirtualFunc43__Q22cf8CfObjectFv,
    (void*)CfObject_UnkVirtualFunc44__Q22cf8CfObjectFv, (void*)CfObject_UnkVirtualFunc45__Q22cf8CfObjectFv, (void*)CfObject_setMoveTargetPtr__Q22cf8CfObjectFv, (void*)CfObject_createMoveTarget__Q22cf8CfObjectFv,
    (void*)CfObject_getCurrentTarget__Q22cf8CfObjectFv, (void*)CfObject_forwardNpcAction__Q22cf8CfObjectFv, (void*)CfObject_UnkVirtualFunc50__Q22cf8CfObjectFv, (void*)CfObject_UnkVirtualFunc51__Q22cf8CfObjectFv,
    (void*)CfObject_UnkVirtualFunc52__Q22cf8CfObjectFv, (void*)CfObject_UnkVirtualFunc53__Q22cf8CfObjectFv, (void*)CfObject_UnkVirtualFunc54__Q22cf8CfObjectFv, (void*)CfObject_UnkVirtualFunc55__Q22cf8CfObjectFv,
    (void*)CfObject_UnkVirtualFunc56__Q22cf8CfObjectFv, (void*)CfObject_UnkVirtualFunc57__Q22cf8CfObjectFv, (void*)CfObject_getMoveRateScale__Q22cf8CfObjectFv, (void*)CfObject_UnkVirtualFunc59__Q22cf8CfObjectFv,
    (void*)CfObject_UnkVirtualFunc60__Q22cf8CfObjectFv, (void*)CfObject_setAnimSlotEntry__Q22cf8CfObjectFv, (void*)CfObject_UnkVirtualFunc62__Q22cf8CfObjectFv, (void*)CfObject_UnkVirtualFunc63__Q22cf8CfObjectFv,
    (void*)CfObject_setMoveBusyState__Q22cf8CfObjectFi, (void*)CfObject_setMoveReadyFlag__Q22cf8CfObjectFv, (void*)setPointEnabled__Q22cf13CfObjectPointFv, (void*)CfObject_UnkVirtualFunc67__Q22cf8CfObjectFv,
    (void*)isCollEnabled__Q22cf13CfObjectPointFv, (void*)CfObject_UnkVirtualFunc69__Q22cf8CfObjectFv, (void*)CfObject_UnkVirtualFunc70__Q22cf8CfObjectFv, (void*)CfObject_UnkVirtualFunc71__Q22cf8CfObjectFv,
    (void*)CfObject_UnkVirtualFunc72__Q22cf8CfObjectFv, (void*)CfObject_UnkVirtualFunc73__Q22cf8CfObjectFv,
};
TextKeyword lbl_eu_8052A528[6] = {
    { (const char*)lbl_eu_80666B58, 0x0 },
    { (const char*)lbl_eu_80666B5C, 0x1 },
    { (const char*)lbl_eu_80666B60, 0x2 },
    { (const char*)lbl_eu_80666B64, 0x3 },
    { (const char*)lbl_eu_80666B6C, 0x4 },
    { (const char*)lbl_eu_80666B74, 0x5 },
};

// Retail .sdata2 ends with a 4-byte zero word at 0x24 (the retail split names
// it gap_11_80666B7C_sdata2); kept so the section extent matches retail's 0x28,
// since ldscript.lcf concatenates sections positionally.  Needs a global
// definition plus the pragma: MWCC routes a static/const zero word to .sbss2
// even with the section declspec.
#pragma section data_type ".sdata2" ".sdata2"
__declspec(section ".sdata2") __attribute__((used)) u32 gap_11_80666B7C_sdata2[1] = { 0 };
#pragma section data_type ".data" ".bss"
