// Isolated Broadway/Gekko paired-single backend for named CfCam kernels.
// This file is included only by CfCam.cpp; it does not provide shared helpers.
#ifdef __MWERKS__
extern "C" void func_8006BB04(register nw4r::math::VEC3* vector, register f32 scale) {
    register f32 source, result;
    ASM (
        psq_l    source, 0(vector), 0, 0
        ps_muls0 result, source, scale
        psq_l    source, 8(vector), 1, 0
        psq_st   result, 0(vector), 0, 0
        ps_muls0 result, source, scale
        psq_st   result, 8(vector), 1, 0
    )
}

extern "C" void func_8006C6E8(register nw4r::math::VEC3* out,
                               register const nw4r::math::VEC3* a,
                               register const nw4r::math::VEC3* b) {
    nw4r::math::VEC3 sum;
    register nw4r::math::VEC3* sumPtr = &sum;
    register f32 left, right, result;
    ASM (
        psq_l  left, 0(a), 0, 0
        psq_l  right, 0(b), 0, 0
        ps_add result, left, right
        psq_l  left, 8(a), 1, 0
        psq_l  right, 8(b), 1, 0
        psq_st result, 0(sumPtr), 0, 0
        ps_add result, left, right
        psq_st result, 8(sumPtr), 1, 0
    )
    out->x = sum.x;
    out->y = sum.y;
    out->z = sum.z;
}

extern "C" f32 func_80071CF4(register const nw4r::math::VEC3* a,
                               register const nw4r::math::VEC3* b) {
    nw4r::math::VEC3 copy;
    nw4r::math::VEC3 delta;
    register nw4r::math::VEC3* copyPtr = &copy;
    register nw4r::math::VEC3* deltaPtr = &delta;
    register f32 work0, work1, work2, work3;
    ASM (
        psq_l  work0, 0(a), 0, 0
        psq_l  work1, 0(b), 0, 0
        ps_sub work2, work0, work1
        psq_l  work0, 8(a), 1, 0
        psq_l  work1, 8(b), 1, 0
        psq_st work2, 0(deltaPtr), 0, 0
        ps_sub work2, work0, work1
        lfs    work0, 0(deltaPtr)
        lfs    work1, 4(deltaPtr)
        stfs   work0, 0(copyPtr)
        stfs   work1, 4(copyPtr)
        psq_l  work3, 0(copyPtr), 0, 0
        psq_st work2, 8(deltaPtr), 1, 0
        ps_mul work3, work3, work3
        lfs    work1, 8(deltaPtr)
        stfs   work1, 8(copyPtr)
        ps_madd work0, work1, work1, work3
        ps_sum0 work0, work0, work3, work3
    )
    return work0;
}
#else
extern "C" void func_8006BB04(nw4r::math::VEC3* vector, f32 scale) {
    vector->x *= scale;
    vector->y *= scale;
    vector->z *= scale;
}

extern "C" void func_8006C6E8(nw4r::math::VEC3* out,
                               const nw4r::math::VEC3* a,
                               const nw4r::math::VEC3* b) {
    out->x = a->x + b->x;
    out->y = a->y + b->y;
    out->z = a->z + b->z;
}

extern "C" f32 func_80071CF4(const nw4r::math::VEC3* a,
                               const nw4r::math::VEC3* b) {
    nw4r::math::VEC3 difference;
    difference.x = a->x - b->x;
    difference.y = a->y - b->y;
    difference.z = a->z - b->z;
    return difference.x * difference.x + difference.y * difference.y +
           difference.z * difference.z;
}
#endif
