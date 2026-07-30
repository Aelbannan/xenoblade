#include <harness_catalog.h>

/* Bitstream reader state */
struct BS {
    const u8 *ptr;
    int bit_offset;
    u32 buf;
    int bits;
};

static u32 bs_peek(struct BS *bs, int n) {
    while (bs->bits < n) {
        bs->buf = (bs->buf << 8) | *bs->ptr++;
        bs->bits += 8;
    }
    return bs->buf >> (bs->bits - n);
}

static void bs_skip(struct BS *bs, int n) {
    bs->bits -= n;
    bs->buf &= (1 << bs->bits) - 1;
}

static u32 bs_read(struct BS *bs, int n) {
    u32 v = bs_peek(bs, n);
    bs_skip(bs, n);
    return v;
}

static void bs_align(struct BS *bs) {
    int m = bs->bits & 7;
    if (m) bs_skip(bs, 8 - m);
}

/* Check if B-picture contains no non-intra macroblocks */
int MPV_IsEmptyBpic(const u8 *data, int size, int flags) {
    struct BS bs;
    int mb_count = 0;
    u32 start_code;
    int x, y, width, height;
    int mb_row, mb_col;

    /* Find the picture header */
    {
        int i;
        for (i = 0; i < size - 4; i++) {
            if (data[i] == 0 && data[i+1] == 0 && data[i+2] == 1) {
                u8 type = data[i+3];
                /* Check for picture_start_code (0x00) or slice codes (0x01..0xAF) */
                break;
            }
        }
    }

    bs.ptr = data;
    bs.bit_offset = 0;
    bs.buf = 0;
    bs.bits = 0;

    /* Skip to first slice */
    for (;;) {
        bs_align(&bs);
        if (bs_peek(&bs, 24) == 0x000001) {
            u8 code = (u8)bs_peek(&bs, 32);
            if (code >= 0x01 && code <= 0xAF) {
                break; /* slice */
            }
        }
        if (bs_read(&bs, 1) == 0) break;
    }

    /* Check that there are no non-intra coded macroblocks */
    /* For B-pictures, non-intra = NOT coded */
    /* We need to parse all macroblocks in this slice */
    {
        int slice_vert_pos = bs_read(&bs, 8); /* slice_vertical_position */
        if (slice_vert_pos == 0) return 1; /* empty */
        
        bs_skip(&bs, 8); /* quantiser_scale_code */
        if (bs_peek(&bs, 1)) {
            bs_skip(&bs, 1); /* intra_slice_flag */
            bs_skip(&bs, 7); /* intra_slice_id */
        }
        
        /* Parse macroblocks */
        for (;;) {
            if (bs_peek(&bs, 23) == 0) break; /* next start code */
            
            u32 mb_addr_inc = 0;
            for (;;) {
                u32 v = bs_read(&bs, 1);
                if (v) { mb_addr_inc += 1; break; }
                mb_addr_inc += 1;
                /* macroblock_escape */
                if (bs_peek(&bs, 1)) {
                    bs_skip(&bs, 1);
                    mb_addr_inc += 33;
                }
            }
            
            if (mb_addr_inc > 0x21) {
                /* reached a new slice or picture */
                break;
            }
            
            /* macroblock_type */
            u32 mb_type = bs_read(&bs, 1);
            if (mb_type == 0) {
                mb_type = bs_read(&bs, 2);
                if (mb_type == 0) break;
            }
            
            /* Check if this macroblock is coded (not skipped) */
            /* For B-pictures, check for motion vectors */
            {
                int coded = 0;
                if (mb_type & 1) coded = 1; /* pattern_code */
                if (coded) {
                    /* This macroblock has coded blocks - not empty */
                    return 0;
                }
                /* Skip motion vectors if present */
                if (mb_type & 2) {
                    bs_skip(&bs, 8); /* motion_code */
                }
                if (mb_type & 4) {
                    bs_skip(&bs, 8); /* motion_code */
                }
            }
            
            mb_count++;
            if (mb_count > 1000) break;
        }
    }

    return 1; /* all macroblocks are skipped/empty */
}

/* Check if P-picture contains no non-intra macroblocks */  
int MPV_IsEmptyPpic(const u8 *data, int size, int limit) {
    /* Similar structure to B-pic check but for P-pictures */
    /* P-pictures can have forward-predicted macroblocks */
    struct BS bs;
    int mb_count = 0;
    int result = 1;

    bs.ptr = data;
    bs.buf = 0;
    bs.bits = 0;

    /* Align to byte boundary and scan for start codes */
    for (;;) {
        bs_align(&bs);
        if (bs_peek(&bs, 24) == 0x000001) {
            u8 code = (u8)(bs_peek(&bs, 32) & 0xFF);
            if (code >= 0x01 && code <= 0xAF) {
                bs_skip(&bs, 32);
                
                int slice_vert = bs_read(&bs, 8);
                if (slice_vert == 0) return 1;
                
                bs_skip(&bs, 8); /* quantiser_scale */
                if (bs_peek(&bs, 1)) {
                    bs_skip(&bs, 8); /* intra_slice */
                }
                
                for (;;) {
                    if (bs_peek(&bs, 23) == 0) break;
                    
                    u32 inc = 0;
                    while (!bs_peek(&bs, 1)) {
                        bs_skip(&bs, 1);
                        inc += 1;
                        if (inc > 32) break;
                    }
                    bs_skip(&bs, 1);
                    inc += 1;
                    
                    if (inc > 0x21) break;
                    
                    u32 mb_type = bs_read(&bs, 1);
                    if (mb_type == 0) {
                        u32 t = bs_read(&bs, 2);
                        if (t == 0) break;
                        mb_type = t;
                    }
                    
                    /* Check macroblock for coded blocks */
                    if (mb_type & 1) {
                        /* Has coded DCT blocks - scan pattern */
                        u32 cbp = bs_read(&bs, 6);
                        if (cbp) return 0; /* has coded blocks */
                    }
                    
                    /* Skip motion vectors */
                    if (mb_type & 2) {
                        int f = bs_read(&bs, 1);
                        bs_skip(&bs, 2); /* motion_forward */
                    }
                    if (mb_type & 4) {
                        bs_skip(&bs, 2); /* motion_backward */
                    }
                    
                    mb_count++;
                    if (mb_count > 2000) break;
                }
                break;
            }
        }
        if (bs_read(&bs, 1) == 0) break;
    }

    return result;
}
