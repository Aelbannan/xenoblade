// Auto-scaffolded catalog TU for CriWare/src/sofdec/sfdcore/sfh/sfh_local
// Replace stubs with high-level C/C++ during decomp.

#include <harness_catalog.h>

// Reads `n` bytes at `p` as an integer: "L" shifts the byte stream in from
// the end of the window (p[n-1] .. p[0]), "B" from the start (p[0] .. p[n-1]);
// only the low 32 (or 64) bits of the shifted-in stream are kept.

u32 SFHLOCAL_GetNbyteL(const u8* p, int n) {
    u32 r = 0;
    int i;
    for (i = n - 1; i >= 0; i--) {
        r = (r << 8) | p[i];
    }
    return r;
}

u32 SFHLOCAL_GetNbyteB(const u8* p, int n) {
    u32 r = 0;
    int i;
    for (i = 0; i < n; i++) {
        r = (r << 8) | p[i];
    }
    return r;
}

u64 SFHLOCAL_GetNbyteB64(const u8* p, int n) {
    u64 r = 0;
    int i;
    for (i = 0; i < n; i++) {
        r = (r << 8) | p[i];
    }
    return r;
}

int SFHLOCAL_GetSizeofMember(int a, int b) { return b - a; }
