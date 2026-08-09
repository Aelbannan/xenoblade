// Auto-scaffolded catalog TU for kyoshin/cf/code_800C17DC
// Replace stubs with high-level C/C++ during decomp.

#include <string.h>
#include <stdlib.h>
#include "PowerPC_EABI_Support/MSL_C/MSL_Common/extras.h"

#include "kyoshin/harness_catalog.hpp"
#include "kyoshin/cf/code_800C17DC.hpp"
#include "kyoshin/code_800AA008.hpp"

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

int func_800C17DC(const char* str, int* type, int* len) {
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
int func_800C1A18(CmTextProc* self, char* out, int maxLen) {
    if (self->cursor[0] == 0) {
        out[0] = 0;
        return 0;
    }
    int pos = 0;
    int result = 1;
    while (self->cursor[0] != 0) {
        int type, lenz;
        u32 ch = func_800C17DC(self->cursor, &type, &lenz);
        pos += lenz;
        if (pos >= maxLen) {
            result = 0;
            break;
        }
        if (type == 1) {
            out[0] = self->cursor[0];
            out++;
            self->cursor++;
        } else if (type == 2) {
            out[0] = self->cursor[0];
            out[1] = self->cursor[1];
            out += 2;
            self->cursor += 2;
        } else if (type == 3) {
            if (ch == 0) break;
            self->cursor += lenz;
            break;
        }
    }
    out[0] = 0;
    return result;
}

/// Split a cue-script line into words, storing a pointer to each word start in
/// `out`.  Words begin at a letter (A-Z/a-z) or a two-byte char and are
/// separated by spaces/tabs; `#` and `//` start a comment that ends the line.
/// The source buffer is modified in place (separators become NUL).  Returns the
/// number of words.  (`maxLen` is accepted for ABI/register setup but unused.)
int func_800C1900(char* str, char** out, int maxLen) {
    char* s = str;
    char** o = out;
    int count = 0;
    int offset = 0;
    int inWord = 0;
    while (s[0] != 0) {
        int type, lenz;
        u32 ch = func_800C17DC(s, &type, &lenz);
        if (type == 1) {
            if ((u16)ch == '#') break;
            if ((u16)ch == '/' && s[1] == '/') break;
            u16 c = (u16)ch;
            if (c == ' ' || c == '\t') {
                if (inWord) s[0] = 0;
                inWord = 0;
            } else if ((c >= 'A' && c <= 'Z') || (c >= 'a' && c <= 'z')) {
                if (!inWord) {
                    o[offset] = s;
                    inWord = 1;
                    count++;
                    offset++;
                }
            }
        } else if (type == 2) {
            if (!inWord) {
                o[offset] = s;
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
/// `key` (0-5). `name` is hashed via func_800AA714; `value` (optional) is an
/// atoi-able id.  `key==0` writes the single-entry base set.
void func_800C1B30(CmTextProc* self, int key, const char* name, const char* value) {
    u32 hash = func_800AA714(name);
    if (hash == 0) return;
    u16 seq = self->buf->cntAll;
    u16 arg = 0;
    if (value) arg = (u16)atoi(value);
    switch (key) {
    case 0: {
        u16 i = self->buf->cnt0;
        self->buf->cnt0 = i + 1;
        self->buf->set0[i].hash = hash;
        self->buf->set0[i].field4 = arg;
        self->buf->set0[i].field6 = seq;
        break;
    }
    case 1: {
        u16 i = self->buf->cnt1;
        self->buf->cnt1 = i + 1;
        self->buf->set1[i].hash = hash;
        self->buf->set1[i].field4 = arg;
        self->buf->set1[i].field6 = seq;
        break;
    }
    case 2: {
        u16 i = self->buf->cnt2;
        self->buf->cnt2 = i + 1;
        self->buf->set2[i].hash = hash;
        self->buf->set2[i].field4 = arg;
        self->buf->set2[i].field6 = seq;
        break;
    }
    case 3: {
        u16 i = self->buf->cnt3;
        self->buf->cnt3 = i + 1;
        self->buf->set3[i].hash = hash;
        self->buf->set3[i].field4 = arg;
        self->buf->set3[i].field6 = seq;
        break;
    }
    case 4: {
        u16 i = self->buf->cnt4;
        self->buf->cnt4 = i + 1;
        self->buf->set4[i].hash = hash;
        self->buf->set4[i].field4 = arg;
        self->buf->set4[i].field6 = seq;
        break;
    }
    case 5: {
        u16 i = self->buf->cnt5;
        self->buf->cnt5 = i + 1;
        self->buf->set5[i].hash = hash;
        self->buf->set5[i].field4 = arg;
        self->buf->set5[i].field6 = seq;
        break;
    }
    }
}

/// Parse a cue-script text buffer into the table owned by `self`.  Each line is
/// a keyword token followed by up to two parameters (`KEYWORD name value`); the
/// keyword must appear in the lbl_eu_8052A528 dispatch table.  `buf` receives
/// the 0x628-byte table that gets filled.
void func_800C1CC4(CmTextProc* self, const char* text, void* buf) {
    if (buf == 0) return;
    self->buf = (CmTextTable*)buf;
    memset(self->buf, 0, 0x628);
    self->base = text;
    self->cursor = text;
    for (;;) {
        char token[0x100];
        if (!func_800C1A18(self, token, 0xff)) break;
        if (token[0] == '#') continue;
        if (token[0] == '/' && token[1] == '/') continue;
        char* names[0x10];
        names[1] = 0;
        names[2] = 0;
        if (!func_800C1900(token, names, 0x10)) continue;
        int id = -1;
        for (int i = 0; i < 6; i++) {
            if (stricmp(names[0], lbl_eu_8052A528[i].word) == 0) {
                id = (int)lbl_eu_8052A528[i].id;
                break;
            }
        }
        if (id == -1) continue;
        if (id != 5) self->buf->cntAll = self->buf->cntAll + 1;
        func_800C1B30(self, id, names[1], names[2]);
    }
}

// Routes to func_800C1CC4 with the global flag record as the leading
// argument and passes its two arguments through unchanged.
void func_800C1CAC(u32 arg0, u32 arg1) {
    func_800C1CC4((CmTextProc*)&lbl_eu_805739E8, (const char*)arg0, (void*)arg1);
}

// Register a callback into the table slot selected by field20 and set the
// continue flag that the dispatch loop func_800C1EB8 checks.
void func_800C1E9C(void (*callback)(void), u8 flag) {
    lbl_eu_805739F8.fns[lbl_eu_805739F8.field20] = callback;
    lbl_eu_805739F8.field24 = flag;
}

// Variadic prologue dump: on entry MWCC spills every register-argument into
// this frame so callers can read them back out of memory. Empty body.
void func_800C1DF0(...) {}

// Default no-op callback stored in the callback table at lbl_eu_805739F8.
// Called by func_800C1EB8 when it iterates the callback array and a slot has no
// registered handler.  The empty body corresponds to a single blr instruction.
extern "C" void func_800C1F28(void) {}

// (Re)initialise the callback table: clear every slot, reset the active index
// and flag, then install the default no-op callback in slot 0.
void func_800C1E40() {
    lbl_eu_805739F8.field20 = 0;
    lbl_eu_805739F8.field24 = 0;
    memset(&lbl_eu_805739F8, 0, 0x20);
    lbl_eu_805739F8.fns[lbl_eu_805739F8.field20] = func_800C1F28;
}

// Dispatch loop: while the active callback slot is populated, clear the
// continue flag, invoke the callback, and keep iterating if the callback
// re-set the flag.
void func_800C1EB8() {
    if (lbl_eu_805739F8.fns[lbl_eu_805739F8.field20] != 0) {
        do {
            lbl_eu_805739F8.field24 = 0;
            lbl_eu_805739F8.fns[lbl_eu_805739F8.field20]();
        } while (lbl_eu_805739F8.field24 != 0);
    }
}

// Default no-op callback stored in the callback table at lbl_eu_805739F8.
// Called by func_800C1EB8 when it iterates the callback array and a slot has no
// registered handler.  The empty body corresponds to a single blr instruction.
extern "C" void sinit_800C1F2C() {
    lbl_eu_805739F8.field20 = 0;
    lbl_eu_805739F8.field24 = 0;
}