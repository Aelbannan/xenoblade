// Auto-scaffolded catalog TU for kyoshin/cf/code_800C17DC
// Replace stubs with high-level C/C++ during decomp.

#include <string.h>
#include <stdlib.h>
#include "PowerPC_EABI_Support/MSL_C/MSL_Common/extras.h"

#include "kyoshin/harness_catalog.hpp"
#include "kyoshin/cf/code_800C17DC.hpp"

extern "C" void CmText_DefaultNoopCallback(void);

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

/// Split a cue-script line into words, storing a pointer to each word start in
/// `out`.  Words begin at a letter (A-Z/a-z) or a two-byte char and are
/// separated by spaces/tabs; `#` and `//` start a comment that ends the line.
/// The source buffer is modified in place (separators become NUL).  Returns the
/// number of words.  (`maxLen` is accepted for ABI/register setup but unused.)
int func_800C1900(char* str, char** out, int maxLen) {
    int count = 0;
    char* s = str;
    int inWord = 0;
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
                    out[count] = s;
                    inWord = 1;
                    count++;
                }
            }
        } else if (type == 2) {
            if (!inWord) {
                out[count] = s;
                inWord = 1;
                count++;
            }
        } else {
            break;
        }
        s += lenz;
    }
    s[0] = 0;
    return count;
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
            *o++ = *self->cursor++;
        } else if (type == 2) {
            *o++ = *self->cursor++;
            *o++ = *self->cursor++;
        } else if (type == 3) {
            if ((u16)ch == 0) break;
            self->cursor += lenz;
            break;
        }
    }
    o[0] = 0;
    return result;
}


/// Register a (filename-hash, id, seq) entry into the table set selected by
/// `key` (0-5). `name` is hashed via Tok_ParseFile; `value` (optional) is an
/// atoi-able id.  `key==0` writes the single-entry base set.
///
/// Known residual vs retail (us-800c2578): shape and size now match (per-arm
/// `self->buf` reload, `stwux` key==0 arm); only register coloring differs
/// (retail keeps `id` in r0 and the counter in r3/r4).
void CmText_RegisterEntry(CmTextProc* self, int key, const char* name, const char* value) {
    u32 hash = Tok_ParseFile(name);
    if (hash == 0) return;
    u16 seq = self->buf->cntAll;
    u16 id = 0;
    if (value != 0) {
        id = atoi(value);
    }
    if (key == 0) {
        CmTextTable* tbl = self->buf;
        TextEntry* e = &tbl->set0[tbl->cnt0];
        tbl->cnt0++;
        e->hash = hash;
        e->field4 = id;
        e->field6 = seq;
    } else if (key == 1) {
        CmTextTable* tbl = self->buf;
        TextEntry* e = &tbl->set1[tbl->cnt1];
        tbl->cnt1++;
        e->hash = hash;
        e->field4 = id;
        e->field6 = seq;
    } else if (key == 2) {
        CmTextTable* tbl = self->buf;
        TextEntry* e = &tbl->set2[tbl->cnt2];
        tbl->cnt2++;
        e->hash = hash;
        e->field4 = id;
        e->field6 = seq;
    } else if (key == 3) {
        CmTextTable* tbl = self->buf;
        TextEntry* e = &tbl->set3[tbl->cnt3];
        tbl->cnt3++;
        e->hash = hash;
        e->field4 = id;
        e->field6 = seq;
    } else if (key == 4) {
        CmTextTable* tbl = self->buf;
        TextEntry* e = &tbl->set4[tbl->cnt4];
        tbl->cnt4++;
        e->hash = hash;
        e->field4 = id;
        e->field6 = seq;
    } else if (key == 5) {
        CmTextTable* tbl = self->buf;
        TextEntry* e = &tbl->set5[tbl->cnt5];
        tbl->cnt5++;
        e->hash = hash;
        e->field4 = id;
        e->field6 = seq;
    }
}

// Routes to CmText_ParseTextBuffer with the global flag record as the leading
// argument and passes its two arguments through unchanged.
extern "C" void CmText_ParseWithStaticProc(const char* arg0, CmTextTable* arg1) {
    CmText_ParseTextBuffer(&sCmTextParseState, arg0, arg1);
}


/// Parse a cue-script text buffer into the table owned by `self`.  Each line is
/// a keyword token followed by up to two parameters (`KEYWORD name value`); the
/// keyword must appear in the sCmTextKeywords dispatch table.  `buf` receives
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
            if (stricmp(names[0], sCmTextKeywords[i].word) == 0) {
                id = (int)sCmTextKeywords[i].id;
                break;
            }
        }
        if (id == -1) continue;
        if (id != 5) self->buf->cntAll++;
        CmText_RegisterEntry(self, id, names[1], names[2]);
    }
}


// Variadic prologue dump: on entry MWCC spills every register-argument into
// this frame so callers can read them back out of memory. Empty body.
extern "C" void CmText_VariadicNoop(...) {}



// (Re)initialise the callback table: clear every slot, reset the active index
// and flag, then install the default no-op callback in slot 0.
extern "C" void CmText_InitCallbackTable() {
    sCmTextCallbackTable.field20 = 0;
    sCmTextCallbackTable.field24 = 0;
    memset(&sCmTextCallbackTable, 0, sizeof(sCmTextCallbackTable.fns));
    sCmTextCallbackTable.fns[sCmTextCallbackTable.field20] = CmText_DefaultNoopCallback;
}


// Register a callback into the table slot selected by field20 and set the
// continue flag that the dispatch loop CmText_DispatchCallbacks checks.
extern "C" void CmText_RegisterCallback(void (*callback)(void), u8 flag) {
    sCmTextCallbackTable.fns[sCmTextCallbackTable.field20] = callback;
    sCmTextCallbackTable.field24 = flag;
}



// Dispatch loop: while the active callback slot is populated, clear the
// continue flag, invoke the callback, and keep iterating if the callback
// re-set the flag.
extern "C" void CmText_DispatchCallbacks() {
    if (sCmTextCallbackTable.fns[sCmTextCallbackTable.field20] != 0) {
        do {
            sCmTextCallbackTable.field24 = 0;
            sCmTextCallbackTable.fns[sCmTextCallbackTable.field20]();
        } while (sCmTextCallbackTable.field24 != 0);
    }
}



// Default no-op callback stored in the callback table at sCmTextCallbackTable.
// Called by CmText_DispatchCallbacks when it iterates the callback array and a slot has no
// registered handler.  The empty body corresponds to a single blr instruction.
extern "C" void CmText_DefaultNoopCallback(void) {}



// Default no-op callback stored in the callback table at sCmTextCallbackTable.
// Called by CmText_DispatchCallbacks when it iterates the callback array and a slot has no
// registered handler.  The empty body corresponds to a single blr instruction.
extern "C" void sinit_800C1F2C() {
    sCmTextCallbackTable.field20 = 0;
    sCmTextCallbackTable.field24 = 0;
}

// ---------------------------------------------------------------------------

// ---------------------------------------------------------------------------
// Retail .bss (0x38): the parser-state record and the 0x28 callback table,
// both already typed in include/kyoshin/cf/code_800C17DC.hpp.
// name-guess: no real retail name at 0x805739E8 (JP lbl_805739E8 is a string label);
// this 0x10-byte record is the CmTextProc used by CmText_ParseWithStaticProc.
CmTextProc sCmTextParseState;
// name-guess: no real retail name at 0x805739F8 (JP __vt__9CMdlMouth is a different
// 0x10-byte .data object); this 0x28-byte record owns the callback array/flags.
Unk805739F8 sCmTextCallbackTable;

// ---------------------------------------------------------------------------
// Retail .sdata2 (0x28): MWCC pools the six scenario-keyword string literals
// from the dispatch table below at 0x00/0x04/0x08/0x0C/0x14/0x1C in first-use
// order, with the padding after "effect" and "mapse" emitted by the compiler;
// no separate string definitions are needed.

// ---------------------------------------------------------------------------
// Retail .data owned by this TU (0x30): the scenario-keyword dispatch table.
// name-guess: no real retail name at 0x8052A528 (JP has no object at this address);
// this is the six-entry scenario keyword dispatch table used by ParseTextBuffer.
TextKeyword sCmTextKeywords[6] = {
    { "chr", 0x0 },
    { "anm", 0x1 },
    { "obj", 0x2 },
    { "effect", 0x3 },
    { "mapse", 0x4 },
    { "pack", 0x5 },
};

// Retail .sdata2 ends with a 4-byte zero word at 0x24 (the retail split names
// it gap_11_80666B7C_sdata2); kept so the section extent matches retail's 0x28,
// since ldscript.lcf concatenates sections positionally.  Needs a global
// definition plus the pragma: MWCC routes a static/const zero word to .sbss2
// even with the section declspec.
#pragma section data_type ".sdata2" ".sdata2"
__declspec(section ".sdata2") __attribute__((used)) u32 gap_11_80666B7C_sdata2[1] = { 0 };
#pragma section data_type ".data" ".bss"
