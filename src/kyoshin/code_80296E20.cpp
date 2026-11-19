// kyoshin/code_80296E20 - chain gauge helpers (two-float gauge at CChainCombo+0xC).
//
// MWCC emits a TU's generated code - the static initialiser and the MI
// adjusting thunks - at the END of its .text, so retail's CSysWinSave .text
// stops right after [@108@__dt__ / @112@cbRenderBefore / @112@__dt__] at
// 0x80296E20. The three functions that follow (0x80296E20..0x80296E70) are
// this TU, and they own the whole .sdata2 pool at 0x80668BA0..0x80668BB0:
// retail's CSysWinSave .sdata2 is empty, and the five pool relocations that
// reference it come from these bodies (.text 0x86C/0x87C/0x88C/0x890/0x8A8 in
// the retail split object).
//
// The unit has no other data: CSysWinSave keeps the neighbouring .rodata
// (0x8050FD60..0x8050FD88), .data (0x805389A0..0x80538AA0), .sdata and .sbss
// slices, and the next unit starts at 0x80296E70 / 0x80668BB0.

#include <types.h>

// sdata2 scalar used by setChainGauge; defined after the pooling functions so
// its 8-byte slot lands last, like retail's third .sdata2 entry.
extern const volatile f64 kChainGaugeTwo;

// C++ linkage: retail's symbols are the C++ mangled initChainGauge__FPv /
// resetChainGauge__FPv (both take the gauge as void*).
void initChainGauge(void* p) {
    float* self = (float*)p;
    float v = 0.0f;
    self[0] = v;
    self[1] = v;
}

void resetChainGauge(void* p) {
    float* self = (float*)p;
    float v = 0.0f;
    self[0] = v;
    self[1] = v;
}

// ---------------------------------------------------------------------------
// setChainGauge (us-80296e40)
// Write a base value to [out+0] and [out+4]. If the incoming parameter is
// below the threshold A, additionally store a corrected value:
//   corrected = C * (param - A) - A
// to both slots (the whole point: clamp/recentre toward a target).
// ---------------------------------------------------------------------------
extern "C" void setChainGauge(float* self, float param) {
    // Birth order b,a (Rule A: first-declared volatile takes the lowest
    // FPR, so b lands f0 and a lands f2); assignment order keeps retail's
    // load sequence (BA4 into f2 before BA0 into f0).
    float b, a;
    a = 1.0f;
    b = 0.0f;
    self[0] = b;
    self[1] = b;
    if (a < param) {
        param -= a;
        // 8-byte 2.0 pooled via the file-scope const volatile below (lands
        // last like retail's third slot); reading its low word emits a
        // plain lfs with no conversion code. Volatile blocks MWCC folding
        // the known constant through the pointer read (which would collapse
        // it to a 4-byte pool entry).
        float c = *(const f32*)&kChainGaugeTwo;
        float r = (c * param) - a;
        self[1] = r;
        self[0] = r;
    }
}

// 8-byte 2.0 constant for setChainGauge above. File-scope const volatile
// (global linkage so the earlier use compiles; volatile blocks const-
// folding) placed after the literal-pooling functions so it emits last,
// matching retail's third sdata2 slot.
const volatile f64 kChainGaugeTwo = 2.0;
