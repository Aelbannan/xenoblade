// zlib 1.2.3 inflate — see UnkClass_80460C34.hpp for layout/rationale.
#include "UnkClass_80460C34.hpp"

extern "C" void* memcpy(void* dest, const void* src, unsigned long n);

// ---------------------------------------------------------------------------
// inflateReset  (retail func_80460C34__17UnkClass_80460C34Fv, 0x80)
// ---------------------------------------------------------------------------
extern "C" int func_80460C34__17UnkClass_80460C34Fv(z_stream* strm) {
    z_inflate_state* state;

    if (strm == 0 || strm->state == 0) return Z_STREAM_ERROR;
    state = strm->state;
    strm->total_in = strm->total_out = state->total = 0;
    strm->msg = 0;
    strm->adler = 1;
    state->mode = HEAD;
    state->last = 0;
    state->havedict = 0;
    state->dmax = 32768U;
    state->head = 0;
    state->wsize = 0;
    state->whave = 0;
    state->wnext = 0;
    state->hold = 0;
    state->bits = 0;
    state->lencode = state->distcode = state->next = state->codes;
    return Z_OK;
}

// ---------------------------------------------------------------------------
// inflateInit2_  (retail func_80460CB4__17UnkClass_80460C34Fv, 0x118)
// ---------------------------------------------------------------------------
extern "C" int func_80460CB4__17UnkClass_80460C34Fv(
        z_stream* strm, int windowBits, const char* version, int stream_size) {
    z_inflate_state* state;

    if (version == 0 || version[0] != lbl_eu_80523AF0[0] ||
        stream_size != (int)sizeof(z_stream))
        return Z_VERSION_ERROR;
    if (strm == 0) return Z_STREAM_ERROR;
    strm->msg = 0;
    state = (z_inflate_state*)((z_alloc_func)strm->zalloc)(
        strm->opaque, 1, sizeof(z_inflate_state));
    if (state == 0) return Z_MEM_ERROR;
    strm->state = state;
    if (windowBits < 0) {
        state->wrap = 0;
        windowBits = -windowBits;
    }
    else {
        state->wrap = (windowBits >> 4) + 1;
    }
    if ((z_uint)(windowBits - 8) > 7U) {
        ((z_free_func)strm->zfree)(strm->opaque, state);
        strm->state = 0;
        return Z_STREAM_ERROR;
    }
    state->wbits = (z_uint)windowBits;
    state->window = 0;
    return func_80460C34__17UnkClass_80460C34Fv(strm);
}

// ---------------------------------------------------------------------------
// inflateInit_  (retail func_80460DCC__17UnkClass_80460C34Fv, 0x10)
// ---------------------------------------------------------------------------
extern "C" int func_80460DCC__17UnkClass_80460C34Fv(
        z_stream* strm, const char* version, int stream_size) {
    return func_80460CB4__17UnkClass_80460C34Fv(strm, 15, version, stream_size);
}

// ---------------------------------------------------------------------------
// updatewindow  (retail local func_80460DDC__17UnkClass_80460C34Fv, 0x17C)
// ---------------------------------------------------------------------------
extern "C" int func_80460DDC__17UnkClass_80460C34Fv(z_stream* strm, z_uint out) {
    z_inflate_state* state;
    z_uint copy, dist;

    state = strm->state;
    if (state->window == 0) {
        state->window = (z_uchar*)((z_alloc_func)strm->zalloc)(
            strm->opaque, 1U << state->wbits, sizeof(z_uchar));
        if (state->window == 0) return 1;
    }
    if (state->wsize == 0) {
        state->wsize = 1U << state->wbits;
        state->wnext = 0;
        state->whave = 0;
    }
    copy = out - strm->avail_out;
    if (copy >= state->wsize) {
        memcpy(state->window, strm->next_out - state->wsize, state->wsize);
        state->wnext = 0;
        state->whave = state->wsize;
    }
    else {
        dist = state->wsize - state->wnext;
        if (dist > copy) dist = copy;
        memcpy(state->window + state->wnext, strm->next_out - copy, dist);
        copy -= dist;
        if (copy) {
            memcpy(state->window, strm->next_out - copy, copy);
            state->wnext = copy;
            state->whave = state->wsize;
        }
        else {
            state->wnext += dist;
            if (state->wnext == state->wsize) state->wnext = 0;
            if (state->whave < state->wsize) state->whave += dist;
        }
    }
    return 0;
}

// ---------------------------------------------------------------------------
// inflateEnd  (retail func_80461FE0__17UnkClass_80460C34Fv, 0x88)
// ---------------------------------------------------------------------------
extern "C" int func_80461FE0__17UnkClass_80460C34Fv(z_stream* strm) {
    z_inflate_state* state;

    if (strm == 0 || strm->state == 0 || strm->zfree == 0)
        return Z_STREAM_ERROR;
    state = strm->state;
    if (state->window != 0)
        ((z_free_func)strm->zfree)(strm->opaque, state->window);
    ((z_free_func)strm->zfree)(strm->opaque, state);
    strm->state = 0;
    return Z_OK;
}
