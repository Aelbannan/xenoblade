#include <revolution/MTX.h>

//unused
void C_MTXMultVec(){
}

void PSMTXMultVec(const Mtx mtx, const Vec* vec, Vec* out) {
    out->x = mtx[0][0] * vec->x + mtx[0][1] * vec->y + mtx[0][2] * vec->z + mtx[0][3];
    out->y = mtx[1][0] * vec->x + mtx[1][1] * vec->y + mtx[1][2] * vec->z + mtx[1][3];
    out->z = mtx[2][0] * vec->x + mtx[2][1] * vec->y + mtx[2][2] * vec->z + mtx[2][3];
}

//unused
void C_MTXMultVecArray(){
}

//unused
asm void PSMTXMultVecArray(){
}

//unused
void C_MTXMultVecSR(){
}

//unused
asm void PSMTXMultVecSR(const Mtx m, const Vec* vec1, Vec* vec2){
}

//unused
void C_MTXMultVecArraySR(){
}

//unused
asm void PSMTXMultVecArraySR(){
}
