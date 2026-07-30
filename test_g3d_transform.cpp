// Host-compiled correctness test for g3d_transform matrix inverses.
// Validates the portable scalar C++ against a double-precision reference oracle.
//
// Build (from repo root):
//   c++ -O2 -std=c++17 -DCOMPAT_ANY=1 \
//       -Iinclude -Ilibs/nw4r/include \
//       test_g3d_transform.cpp libs/nw4r/src/g3d/g3d_transform.cpp -o /tmp/test_g3d
//   /tmp/test_g3d

#include <nw4r/math.h>
#include <cstdio>
#include <cstring>
#include <cmath>
#include <algorithm>
#include <random>

namespace nw4r { namespace g3d { namespace detail {
bool CalcViewNrmMtx(math::MTX33* pOut, const math::MTX34* pMtx);
bool CalcViewTexMtx(math::MTX34* pOut, const math::MTX34* pMtx);
bool CalcInvWorldMtx(math::MTX34* pOut, const math::MTX34* pMtx);
}}}

using nw4r::math::MTX33;
using nw4r::math::MTX34;

// Double-precision reference: true inverse M^-1 via cofactor/adjugate.
static bool ref_inverse(const double M[3][3], double out[3][3]) {
    double a=M[0][0],b=M[0][1],c=M[0][2];
    double d=M[1][0],e=M[1][1],f=M[1][2];
    double g=M[2][0],h=M[2][1],i=M[2][2];
    double C00=e*i-f*h, C01=f*g-i*d, C02=d*h-e*g;
    double C10=c*h-b*i, C11=a*i-c*g, C12=b*g-a*h;
    double C20=b*f-e*c, C21=c*d-f*a, C22=a*e-b*d;
    double det=a*C00+d*C10+g*C20;
    if (std::fabs(det) < 1e-36) return false;
    double inv=1.0/det;
    out[0][0]=C00*inv; out[0][1]=C10*inv; out[0][2]=C20*inv;
    out[1][0]=C01*inv; out[1][1]=C11*inv; out[1][2]=C21*inv;
    out[2][0]=C02*inv; out[2][1]=C12*inv; out[2][2]=C22*inv;
    return true;
}

static MTX34 rand_mtx(std::mt19937& rng, std::uniform_real_distribution<float>& d) {
    MTX34 m;
    m._00=d(rng); m._01=d(rng); m._02=d(rng); m._03=d(rng);
    m._10=d(rng); m._11=d(rng); m._12=d(rng); m._13=d(rng);
    m._20=d(rng); m._21=d(rng); m._22=d(rng); m._23=d(rng);
    return m;
}

int main() {
    std::mt19937 rng(42);
    std::uniform_real_distribution<float> d(-100.0f, 100.0f);
    const int N = 100000;
    int fail = 0;
    float maxerr_inv = 0, maxerr_nrm = 0, maxerr_tex = 0;

    // --- CalcInvWorldMtx: must equal true M^-1 (3x3) + affine translation ---
    for (int t = 0; t < N; ++t) {
        MTX34 m = rand_mtx(rng, d);
        MTX34 out; std::memset(&out, 0, sizeof(out));
        bool ok = nw4r::g3d::detail::CalcInvWorldMtx(&out, &m);
        double M[3][3]={{m._00,m._01,m._02},{m._10,m._11,m._12},{m._20,m._21,m._22}};
        double ref[3][3];
        bool ref_ok = ref_inverse(M, ref);
        if (ok != ref_ok) { printf("InvWorld singularity mismatch t=%d\n", t); ++fail; continue; }
        if (!ok) continue;
        float e=0;
        e=std::max(e,std::fabs(out._00-(float)ref[0][0]));
        e=std::max(e,std::fabs(out._01-(float)ref[0][1]));
        e=std::max(e,std::fabs(out._02-(float)ref[0][2]));
        e=std::max(e,std::fabs(out._10-(float)ref[1][0]));
        e=std::max(e,std::fabs(out._11-(float)ref[1][1]));
        e=std::max(e,std::fabs(out._12-(float)ref[1][2]));
        e=std::max(e,std::fabs(out._20-(float)ref[2][0]));
        e=std::max(e,std::fabs(out._21-(float)ref[2][1]));
        e=std::max(e,std::fabs(out._22-(float)ref[2][2]));
        double t0=m._03,t1=m._13,t2=m._23;
        e=std::max(e,std::fabs(out._03-(float)-(ref[0][0]*t0+ref[0][1]*t1+ref[0][2]*t2)));
        e=std::max(e,std::fabs(out._13-(float)-(ref[1][0]*t0+ref[1][1]*t1+ref[1][2]*t2)));
        e=std::max(e,std::fabs(out._23-(float)-(ref[2][0]*t0+ref[2][1]*t1+ref[2][2]*t2)));
        maxerr_inv=std::max(maxerr_inv,e);
        if (e > 1e-2f) { printf("InvWorld FAIL t=%d e=%g\n",t,e); ++fail; }
    }

    // --- CalcViewNrmMtx: must equal M^-T ---
    for (int t = 0; t < N; ++t) {
        MTX34 m = rand_mtx(rng, d);
        MTX33 out; std::memset(&out, 0, sizeof(out));
        bool ok = nw4r::g3d::detail::CalcViewNrmMtx(&out, &m);
        double M[3][3]={{m._00,m._01,m._02},{m._10,m._11,m._12},{m._20,m._21,m._22}};
        double ref[3][3];
        bool ref_ok = ref_inverse(M, ref);
        if (ok != ref_ok) { printf("NrmMtx singularity mismatch t=%d\n", t); ++fail; continue; }
        if (!ok) continue;
        // M^-T[R][C] = ref[C][R]
        float e=0;
        e=std::max(e,std::fabs(out._00-(float)ref[0][0]));
        e=std::max(e,std::fabs(out._01-(float)ref[1][0]));
        e=std::max(e,std::fabs(out._02-(float)ref[2][0]));
        e=std::max(e,std::fabs(out._10-(float)ref[0][1]));
        e=std::max(e,std::fabs(out._11-(float)ref[1][1]));
        e=std::max(e,std::fabs(out._12-(float)ref[2][1]));
        e=std::max(e,std::fabs(out._20-(float)ref[0][2]));
        e=std::max(e,std::fabs(out._21-(float)ref[1][2]));
        e=std::max(e,std::fabs(out._22-(float)ref[2][2]));
        maxerr_nrm=std::max(maxerr_nrm,e);
        if (e > 1e-2f) { printf("NrmMtx FAIL t=%d e=%g\n",t,e); ++fail; }
    }

    // --- CalcViewTexMtx: M^-T into 3x3, translation column = 0 ---
    for (int t = 0; t < N; ++t) {
        MTX34 m = rand_mtx(rng, d);
        MTX34 out; std::memset(&out, 0xEE, sizeof(out));
        bool ok = nw4r::g3d::detail::CalcViewTexMtx(&out, &m);
        double M[3][3]={{m._00,m._01,m._02},{m._10,m._11,m._12},{m._20,m._21,m._22}};
        double ref[3][3];
        bool ref_ok = ref_inverse(M, ref);
        if (ok != ref_ok) { printf("TexMtx singularity mismatch t=%d\n", t); ++fail; continue; }
        if (!ok) continue;
        float e=0;
        e=std::max(e,std::fabs(out._00-(float)ref[0][0]));
        e=std::max(e,std::fabs(out._01-(float)ref[1][0]));
        e=std::max(e,std::fabs(out._02-(float)ref[2][0]));
        e=std::max(e,std::fabs(out._10-(float)ref[0][1]));
        e=std::max(e,std::fabs(out._11-(float)ref[1][1]));
        e=std::max(e,std::fabs(out._12-(float)ref[2][1]));
        e=std::max(e,std::fabs(out._20-(float)ref[0][2]));
        e=std::max(e,std::fabs(out._21-(float)ref[1][2]));
        e=std::max(e,std::fabs(out._22-(float)ref[2][2]));
        e=std::max(e,std::fabs(out._03)); e=std::max(e,std::fabs(out._13)); e=std::max(e,std::fabs(out._23));
        maxerr_tex=std::max(maxerr_tex,e);
        if (e > 1e-2f) { printf("TexMtx FAIL t=%d e=%g\n",t,e); ++fail; }
    }

    // --- Singular-path behavior checks ---
    {
        // Rank-deficient matrix (row1 = 2*row0): det = 0.
        MTX34 s;
        s._00=1; s._01=2; s._02=3; s._03=7;
        s._10=2; s._11=4; s._12=6; s._13=8;
        s._20=5; s._21=6; s._22=1; s._23=9;

        // NrmMtx singular: must copy input 3x3 into output.
        MTX33 nout; std::memset(&nout, 0, sizeof(nout));
        bool n = nw4r::g3d::detail::CalcViewNrmMtx(&nout, &s);
        if (n || nout._00!=1||nout._01!=2||nout._02!=3||nout._10!=2||nout._11!=4||nout._12!=6
              ||nout._20!=5||nout._21!=6||nout._22!=1) {
            printf("NrmMtx singular-path FAIL (expected input copy, ret=%d)\n", n); ++fail;
        }

        // TexMtx singular in-place (pOut==pMtx): matrix must be untouched.
        MTX34 tinplace = s;
        bool ti = nw4r::g3d::detail::CalcViewTexMtx(&tinplace, &tinplace);
        if (ti || tinplace._03!=7 || tinplace._13!=8 || tinplace._23!=9 || tinplace._00!=1) {
            printf("TexMtx in-place singular FAIL (translation not preserved, ret=%d _03=%f)\n", ti, tinplace._03); ++fail;
        }

        // TexMtx singular out-of-place: copy 3x3, zero translation.
        MTX34 tout; std::memset(&tout, 0xEE, sizeof(tout));
        bool to = nw4r::g3d::detail::CalcViewTexMtx(&tout, &s);
        if (to || tout._00!=1||tout._11!=4||tout._22!=1 || tout._03!=0||tout._13!=0||tout._23!=0) {
            printf("TexMtx out-of-place singular FAIL (ret=%d _03=%f)\n", to, tout._03); ++fail;
        }

        // InvWorldMtx singular: no stores (output untouched), return false.
        MTX34 iout; std::memset(&iout, 0xAB, sizeof(iout));
        float sentinel = iout._00;
        bool iw = nw4r::g3d::detail::CalcInvWorldMtx(&iout, &s);
        if (iw || iout._00 != sentinel) {
            printf("InvWorld singular FAIL (expected no stores, ret=%d)\n", iw); ++fail;
        }
    }

    printf("\n=== Results ===\n");
    printf("CalcInvWorldMtx max err: %g\n", maxerr_inv);
    printf("CalcViewNrmMtx  max err: %g\n", maxerr_nrm);
    printf("CalcViewTexMtx  max err: %g\n", maxerr_tex);
    printf("Total failures: %d (of %d randomized + 4 singular checks)\n", fail, 3*N);
    printf(fail == 0 ? "ALL PASS\n" : "FAILURES PRESENT\n");
    return fail == 0 ? 0 : 1;
}
