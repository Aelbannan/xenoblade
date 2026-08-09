// zlib 1.2.3 inflate (inflate.c / inftrees.c) -- Monolith Soft monolib fork.
// Retail TU monolib/src/lib/UnkClass_80460C34.cpp.
//
// The retail symbols carry placeholder `__17UnkClass_80460C34Fv` mangling but
// the bodies are the C zlib functions taking real register parameters. They are
// reconstructed as `extern "C"` free functions using the literal mangled names
// (proven repo pattern, MWCC KB ref:14a574cbb4) so the linker symbols match
// retail exactly while keeping real high-level signatures.

#include <harness_catalog.h>

typedef unsigned char  z_uchar;
typedef unsigned short z_ushort;
typedef unsigned int   z_uint;
typedef unsigned long  z_ulong;

typedef void* (*z_alloc_func)(void* opaque, z_uint items, z_uint size);
typedef void  (*z_free_func)(void* opaque, void* address);

// ---- zlib return / flush codes -------------------------------------------
#define Z_OK            0
#define Z_STREAM_END    1
#define Z_NEED_DICT     2
#define Z_ERRNO        (-1)
#define Z_STREAM_ERROR (-2)
#define Z_DATA_ERROR   (-3)
#define Z_MEM_ERROR    (-4)
#define Z_BUF_ERROR    (-5)
#define Z_VERSION_ERROR (-6)

#define Z_NO_FLUSH      0
#define Z_FINISH        4
#define Z_BLOCK         5

#define Z_DEFLATED      8

// ---- code / stream / state layouts (offsets verified vs retail asm) -------
typedef struct {
    z_uchar op;    // 0x00 operation, extra bits, table bits
    z_uchar bits;  // 0x01 bits in this part of the code
    z_ushort val;  // 0x02 offset in table or code value
} z_code;          // sizeof = 4

struct z_inflate_state;

typedef struct z_stream_s {
    z_uchar* next_in;            // 0x00
    z_uint   avail_in;           // 0x04
    z_ulong  total_in;           // 0x08
    z_uchar* next_out;           // 0x0C
    z_uint   avail_out;          // 0x10
    z_ulong  total_out;          // 0x14
    char*    msg;                // 0x18
    z_inflate_state* state;      // 0x1C
    z_alloc_func zalloc;         // 0x20
    z_free_func  zfree;          // 0x24
    void*    opaque;             // 0x28
    int      data_type;          // 0x2C
    z_ulong  adler;              // 0x30
    z_ulong  reserved;           // 0x34
} z_stream;                      // sizeof = 0x38

// inflate_mode -- retail enum is compacted: COPY and LEN_ are folded away.
// Values verified against the retail switch jump table (jumptable_eu_8056D600).
typedef enum {
    HEAD, FLAGS, TIME, OS, EXLEN, EXTRA, NAME, COMMENT, HCRC,
    DICTID, DICT, TYPE, TYPEDO, STORED, COPY_, TABLE, LENLENS, CODELENS,
    LEN, LENEXT, DIST, DISTEXT, MATCH, LIT, CHECK, LENGTH, DONE, BAD, MEM, SYNC
} z_inflate_mode;

#define Z_ENOUGH 2048

struct z_inflate_state {
    z_inflate_mode mode;   // 0x00
    int   last;            // 0x04
    int   wrap;            // 0x08
    int   havedict;        // 0x0C
    int   flags;           // 0x10
    z_uint dmax;           // 0x14
    z_ulong check;         // 0x18
    z_ulong total;         // 0x1C
    void* head;            // 0x20
    z_uint wbits;          // 0x24
    z_uint wsize;          // 0x28
    z_uint whave;          // 0x2C
    z_uint wnext;          // 0x30
    z_uchar* window;       // 0x34
    z_ulong hold;          // 0x38
    z_uint bits;           // 0x3C
    z_uint length;         // 0x40
    z_uint offset;         // 0x44
    z_uint extra;          // 0x48
    const z_code* lencode; // 0x4C
    const z_code* distcode;// 0x50
    z_uint lenbits;        // 0x54
    z_uint distbits;       // 0x58
    z_uint ncode;          // 0x5C
    z_uint nlen;           // 0x60
    z_uint ndist;          // 0x64
    z_uint have;           // 0x68
    z_code* next;          // 0x6C
    z_ushort lens[320];    // 0x070 .. 0x2F0
    z_ushort work[288];    // 0x2F0 .. 0x530
    z_code codes[Z_ENOUGH];// 0x530 .. 0x2530
};

// ---- retail rodata/data referenced by name (relocations must match) -------
extern "C" {
extern const char     lbl_eu_80523AF0[]; // "1.2.3" + inflate message pool
extern const z_code   lbl_80526B70[];    // fixed literal/length table (512)
extern const z_code   lbl_eu_80523A48[]; // fixed distance table (32)
extern const z_ushort lbl_eu_80523AC8[]; // code-length permutation (order[19])
extern const z_ushort lbl_eu_80523C60[]; // lbase (indexed from sym 257)
extern const z_ushort lbl_eu_80523CA0[]; // lext
extern const z_ushort lbl_eu_80523CE0[]; // dbase
extern const z_ushort lbl_eu_80523D20[]; // dext

// adler32 / inflate_fast live in the sibling TU UnkClass_80460308.
z_ulong func_80460308__17UnkClass_80460308Fv(z_ulong adler, const z_uchar* buf, z_uint len);
void    func_80460728__17UnkClass_80460308Fv(z_stream* strm, z_uint out);
}

// C-linkage imports
// (retail symbol names - keep linkage/signatures verbatim)
extern "C" void* memcpy(void* dest, const void* src, unsigned long n);
