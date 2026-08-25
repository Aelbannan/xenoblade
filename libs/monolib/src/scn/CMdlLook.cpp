// Auto-scaffolded catalog TU for monolib/src/scn/CMdlLook
// Replace stubs with high-level C/C++ during decomp.

#include <harness_catalog.h>
#include <monolib/core/code_804E36DC.hpp> // func_80496288 frame-delta query (C ABI)
#include "libs/monolib/src/scn/CMdlAnmEye.hpp" // func_80484EB0 fade query
#include <nw4r/math/math_types.h>
#include <nw4r/math/math_triangular.h>
#include <revolution/MTX.h>
#include <math.h>
#include "monolib/math/CVec3.hpp"
#include "monolib/math/FloatUtils.hpp"  // H3 label-owner decl (lbl_eu_8066A208)

// Model-look controller: keeps a set of vectors/scales synced to a model.
class CMdlLook {
public:
    void* field_0x00;          // vtable (blob-owned)
    u8 field_0x04[0x0c];
    ml::CVec3 field_0x10[2];   // 0x10 per-slot axis vectors
    ml::CVec3 field_0x28;      // 0x28 current facing vector
    ml::CVec3 field_0x34;      // 0x34 tracked vector A
    ml::CVec3 field_0x40;      // 0x40
    ml::CVec3 field_0x4c;      // 0x4c
    ml::CVec3 field_0x58;      // 0x58 scale-ish triple
    f32 field_0x64[2];         // 0x64 per-axis blend factors
    u32 field_0x6c;            // 0x6c slot index (-1 = none)
    u32 field_0x70;            // 0x70 slot index (-1 = none)
    u8 field_0x74[0x734];
    u32 field_0x7a8;           // 0x7a8 mode flags (bit0 = local-space aim)

    CMdlLook();
    ~CMdlLook();
};

// Model-side object queried by func_804E8AAC.
struct MdlLookWork {
    u8 field_0x00[0x04];
    void* field_0x04;          // handle fed to the frame-delta query
    u8 field_0x08[0x7a0];
    u32 field_0x7a8;
};

// sdata2 float pool constants used by the look controller
extern const float lbl_eu_8066A200; // pi/2
extern const float lbl_eu_8066B370; // 0.0f
extern const float lbl_eu_8066B37C; // 0.1f
extern const float lbl_eu_8066B380; // 1.0f
extern const float lbl_eu_8066B384; // radians -> FIdx scale
extern const float lbl_eu_8066B394; // angle feedback scale
extern const float lbl_eu_8066B39C; // frame-delta accumulation scale
extern const float lbl_eu_8066B3A0; // x-blend scale
extern const float lbl_eu_8066B3A4; // yz-blend scale
extern const float lbl_eu_8066B3A8; // dot upper threshold
extern const float lbl_eu_8066B3AC; // dot lower threshold
extern const float lbl_eu_8066B3B0; // -1.0f
extern const float lbl_eu_8066B3B4; // FIdx -> radians scale

// Warning message strings (retail .rodata)
extern const char lbl_eu_805262F0[];
extern const char lbl_eu_805262C8[];
extern const char lbl_eu_8052ADB0[];
extern const char lbl_eu_8052AD88[];

// nw4r::db::Warning (retail mangled symbol)
void Warning__Q24nw4r2dbFPCciPCce(const char*, int, const char*, ...);

extern "C" u32 lbl_eu_805701E0[0x4];

// Stripped retail placeholder symbols need C linkage to emit matching
// reloc names; noinline keeps MWCC from folding them into func_804E8290.
extern void func_80488F44(u8* model);
extern "C" void func_804E830C(CMdlLook* look, u8* unk, u32 a, u32 b);
extern "C" void func_804E8AAC(CMdlLook* look, u8* unk, u32 a, u32 b);

// Model-look controller ctor: vtable + zeroed vectors, cleared slots.
CMdlLook::CMdlLook() {
    field_0x00 = &lbl_eu_805701E0;
    field_0x58.set(lbl_eu_8066B370, lbl_eu_8066B370, lbl_eu_8066B370);
    field_0x6c = (u32)-1;
    field_0x70 = (u32)-1;
    field_0x34 = ml::CVec3::zero;
    field_0x40 = ml::CVec3::zero;
    field_0x4c = ml::CVec3::zero;
}

CMdlLook::~CMdlLook() {}

void func_804E7B38() {}

// If any component of the tracked vector differs from the input vector,
// reset the z scale, then adopt the new vector.
void func_804E8220(CMdlLook* look, const ml::CVec3* vec) {
    if (look->field_0x34 != *vec) {
        look->field_0x58.z = lbl_eu_8066B37C;
    }
    look->field_0x34 = *vec;
}

void func_804E8284(u8* self, float a, float b) { *(float*)(self + 0x58) = a; *(float*)(self + 0x5C) = b; }

// Re-syncs the model-look slots: when either slot index matches the given
// index, refresh that slot's transform/vector state.
void func_804E8290(CMdlLook* look, u8* unk, u32 idx1, u32 idx2) {
    if (look->field_0x6c == idx1) {
        func_804E830C(look, unk, idx1, idx2);
    }
    if (look->field_0x70 == idx1) {
        func_804E8AAC(look, unk, idx1, idx2);
    }
}

// Stripped retail placeholder symbols need C linkage to emit matching
// reloc names; noinline keeps MWCC from folding them into func_804E8290.
extern void func_80488F44(u8* model);

extern "C" __declspec(noinline) void func_804E830C(CMdlLook* look, u8* unk, u32 a, u32 b) {
    func_80488F44(unk);
}

// Aims the model's facing axes toward a target direction: builds the
// local-space aim vector, blends per-axis angles toward it while accumulating
// quaternion deltas, converts back to euler angles, then writes
// (rotation * base) into the output matrix.
extern "C" __declspec(noinline) void func_804E8AAC(CMdlLook* look, u8* unkBytes,
                                                  u32 arg2, u32 arg3) {
    MdlLookWork* unk = reinterpret_cast<MdlLookWork*>(unkBytes);
    Mtx* outMtx = *reinterpret_cast<Mtx**>(arg3);
    const f32 eps = lbl_eu_8066A208;

    nw4r::math::MTX34 workMtx;    // composed base matrix
    nw4r::math::MTX34 invMtx;
    nw4r::math::MTX34 rotMtx;
    nw4r::math::MTX34 tmpMtx;
    ml::CVec3 dir;                // normalized local-space aim direction
    Quaternion quat;              // accumulated axis rotation
    ml::CVec3 diff;               // remaining aim error
    Quaternion dq;                // per-axis delta quaternion
    ml::CVec3 crossV;             // rotation axis
    f32 rollAng;
    f32 pitchAng;
    f32 yawAng;
    f32 angles[2];                // per-axis current angles

    // Retail keeps an (always-true) address check around both matrix copies.
    if (&workMtx != NULL) {
        PSMTXCopy(*outMtx, workMtx.m);
    }

    f32 delta = func_80496288(unk->field_0x04);
    f32 fade = func_80484EB0(unk) * delta;
    if (fade == lbl_eu_8066B370) {
        // Faded out: only guard against a fully-degenerate facing vector.
        if (!(unk->field_0x7a8 & 1)) {
            if (__fabs(look->field_0x40.x) > eps && __fabs(look->field_0x40.y) > eps
                && __fabs(look->field_0x40.z) > eps) {
                return;
            }
        }
    } else {
        f32 s24 = func_80496288(unk->field_0x04);
        if (s24 > lbl_eu_8066B380) {
            s24 = lbl_eu_8066B380;
        }
        if (!(unk->field_0x7a8 & 1)) {
            // World-space path: collapse the facing vector toward its z.
            if (!(__fabs(look->field_0x40.x) > eps && __fabs(look->field_0x40.y) > eps
                  && __fabs(look->field_0x40.z) > eps)) {
                f32 k1 = lbl_eu_8066B3A0 * s24;
                f32 k2 = lbl_eu_8066B3A4 * s24;
                look->field_0x40.x *= k1;
                look->field_0x40.y *= k1;
                look->field_0x40.z *= k1;
                look->field_0x64[0] -= look->field_0x64[0] * k2;
                look->field_0x64[1] -= look->field_0x64[1] * k2;
                if (look->field_0x58.z < lbl_eu_8066B37C) {
                    look->field_0x58.z -= lbl_eu_8066B37C;
                }
            }
        } else {
            // Local-space path: transform the target into model space and
            // swing the axes toward it via accumulated quaternions.
            PSMTXInverse(workMtx.m, invMtx.m);
            PSMTXMultVec(invMtx.m, look->field_0x34, dir);
            if (!(__fabs(dir.x) > eps && __fabs(dir.y) > eps && __fabs(dir.z) > eps)) {
                f32 len2 = dir.x * dir.x + dir.y * dir.y + dir.z * dir.z;
                if (len2 == lbl_eu_8066B370) {
                    dir = ml::CVec3::zero;
                } else {
                    PSVECNormalize(dir, dir);
                }

                angles[0] = look->field_0x58.y;
                angles[1] = look->field_0x58.x;

                int first = 0;
                for (int k = 0; k < 2; k++) {
                    ml::CVec3 axis = look->field_0x10[k];
                    f32 s = axis.x * dir.x + axis.y * dir.y + axis.z * dir.z;
                    ml::CVec3 sv;
                    sv.x = axis.x * s;
                    sv.y = axis.y * s;
                    ml::CVec3 ap;
                    ap.x = sv.x;
                    ap.y = sv.y;
                    ml::CVec3 d2;
                    d2.x = dir.x - ap.x;
                    d2.y = dir.y - ap.y;
                    sv.z = axis.z * s;
                    ap.z = sv.z;
                    d2.z = dir.z - ap.z;
                    diff = d2;
                    // Skip axes whose aim error is fully degenerate.
                    if (!(__fabs(d2.x) > eps && __fabs(d2.y) > eps && __fabs(d2.z) > eps)) {
                        f32 len2b = diff.x * diff.x + diff.y * diff.y + diff.z * diff.z;
                        if (len2b == lbl_eu_8066B370) {
                            diff = ml::CVec3::zero;
                        } else {
                            PSVECNormalize(diff, diff);
                        }
                        f32 dotAD = look->field_0x28.x * diff.x + look->field_0x28.y * diff.y
                                  + look->field_0x28.z * diff.z;
                        if (dotAD >= lbl_eu_8066B3A8 || dotAD < lbl_eu_8066B3AC) {
                            // Nearly parallel/antiparallel: pull the angle home directly.
                            look->field_0x64[k] -= look->field_0x64[k] * diff.x;
                        } else {
                            f32 cs = nw4r::math::CosFIdx(lbl_eu_8066B384 * angles[k]);
                            PSVECCrossProduct(look->field_0x28, diff, crossV);
                            f32 ang;
                            if (cs >= dotAD) {
                                ang = angles[k];
                            } else {
                                if (dotAD < lbl_eu_8066B3B0) {
                                    dotAD = lbl_eu_8066B3B0;
                                }
                                if (dotAD > lbl_eu_8066B380) {
                                    dotAD = lbl_eu_8066B380;
                                }
                                if (dotAD > lbl_eu_8066B380 && dotAD < lbl_eu_8066B3B0) {
                                    Warning__Q24nw4r2dbFPCciPCce(lbl_eu_805262F0, 0xef,
                                                                 lbl_eu_805262C8);
                                }
                                ang = acos(dotAD);
                            }
                            // Flip the rotation sense when the axis opposes the error.
                            f32 sd = crossV.x * axis.x + crossV.y * axis.y + crossV.z * axis.z;
                            if (sd < lbl_eu_8066B370) {
                                ang = -ang;
                            }
                            look->field_0x64[k] +=
                                (ang - look->field_0x64[k]) * (diff.x * s24);
                        }
                        // Build the per-axis delta quaternion and accumulate it.
                        f32 fidx = lbl_eu_8066B394 * look->field_0x64[k];
                        f32 svIdx = lbl_eu_8066B384 * fidx;
                        f32 sn = nw4r::math::SinFIdx(svIdx);
                        dq.x = axis.x * sn;
                        dq.y = axis.y * sn;
                        dq.z = axis.z * sn;
                        dq.w = nw4r::math::CosFIdx(svIdx);
                        if (first == 0) {
                            quat = dq;
                            first = 1;
                        } else {
                            PSQUATMultiply(&quat, &dq, &quat);
                        }
                    }
                }

                // Quaternion -> euler angles.
                f32 yy = quat.y + quat.y;
                f32 zz = quat.z + quat.z;
                f32 xx = quat.x + quat.x;
                f32 t = quat.w * yy - quat.x * zz;
                if (t >= lbl_eu_8066B380) {
                    if (t > lbl_eu_8066B3B0) {
                        t = lbl_eu_8066B3B0;
                    }
                } else {
                    t = lbl_eu_8066B380;
                }
                if (t < lbl_eu_8066B3B0) {
                    t = lbl_eu_8066B3B0;
                } else if (t > lbl_eu_8066B380) {
                    t = lbl_eu_8066B380;
                }
                if (t > lbl_eu_8066B380 && t < lbl_eu_8066B3B0) {
                    Warning__Q24nw4r2dbFPCciPCce(lbl_eu_8052ADB0, 0xe4, lbl_eu_8052AD88);
                }

                pitchAng = asin(t);
                f32 cxx = quat.x * xx;
                f32 cxy = quat.x * yy;
                f32 cyy = quat.y * yy;
                f32 czz = quat.z * zz;
                f32 cwz = quat.w * zz;
                if (pitchAng >= lbl_eu_8066A200) {
                    yawAng = lbl_eu_8066B3B4
                             * nw4r::math::Atan2FIdx(cxy - cwz,
                                                     lbl_eu_8066B380 - (cxx + czz));
                    rollAng = lbl_eu_8066B370;
                } else if (pitchAng <= -lbl_eu_8066A200) {
                    rollAng = -(lbl_eu_8066B3B4
                                * nw4r::math::Atan2FIdx(cxy - cwz,
                                                        lbl_eu_8066B380 - (cxx + czz)));
                    yawAng = lbl_eu_8066B370;
                } else {
                    f32 numY = quat.y * zz + quat.w * xx;
                    nw4r::math::Atan2FIdx(numY, lbl_eu_8066B380 - (cxx + cyy));
                    yawAng = lbl_eu_8066B3B4 * numY;
                    rollAng = lbl_eu_8066B3B4
                              * nw4r::math::Atan2FIdx(cxy + cwz,
                                                      lbl_eu_8066B380 - (cyy + czz));
                }

                look->field_0x40.x = rollAng;
                look->field_0x40.y = pitchAng;
                look->field_0x40.z = yawAng;
                f32 nv = lbl_eu_8066B39C * s24 + look->field_0x58.z;
                look->field_0x58.z = nv;
                if (nv > lbl_eu_8066B3A0) {
                    look->field_0x58.z = lbl_eu_8066B3A0;
                }
            }
        }
    }

    // Compose rotation * base and hand it back to the caller.
    nw4r::math::MTX34RotXYZFIdx(&rotMtx, lbl_eu_8066B384 * look->field_0x40.x,
                                lbl_eu_8066B384 * look->field_0x40.y,
                                lbl_eu_8066B384 * look->field_0x40.z);
    PSMTXConcat(workMtx.m, rotMtx.m, tmpMtx.m);
    workMtx = tmpMtx;
    if (&workMtx != NULL) {
        PSMTXCopy(workMtx.m, *outMtx);
    } else {
        PSMTXIdentity(*outMtx);
    }
}

// ===== Dissolved monolibdata2 (blob surgery) data owned by this TU =====
// forward refs (in-block definitions + foreign data labels)
extern "C" const char lbl_eu_80524810[0x9];
extern "C" u32 lbl_eu_8066B3C8;
extern "C" u32 lbl_eu_8066B3C0;
extern "C" u32 lbl_eu_8066B3B8;
extern "C" u32 lbl_eu_80663CA0[0x2];
// foreign function pointers (namespace-scoped extern "C" so the
// declarations cannot collide with in-scope class members; the
// C linkage keeps the emitted reloc names unmangled)
namespace D2Blob {
extern "C" void __dt__8CMdlLookFv();
}

// [.data] 0x805701E0-0x805701FC (28 bytes)
extern "C" u32 lbl_eu_805701E0[0x4] = { (u32)&lbl_eu_80663CA0, 0x00000000, (u32)&D2Blob::__dt__8CMdlLookFv, 0x00000000 };
extern "C" u32 lbl_eu_805701F0[0x3] = { (u32)&lbl_eu_8066B3B8, (u32)&lbl_eu_8066B3C0, (u32)&lbl_eu_8066B3C8 };

// [.rodata] 0x80524810-0x80524830 (32 bytes)
extern "C" __declspec(align(4)) const char lbl_eu_80524810[0x9] = { 0x43, 0x4D, 0x64, 0x6C, 0x4C, 0x6F, 0x6F, 0x6B, 0x00 };
extern "C" __declspec(align(4)) const char lbl_eu_8052481C[0x14] = { 0x4A, 0x55, 0x73, 0x70, 0x41, 0x00, 0x4A, 0x55, 0x68, 0x65, 0x61, 0x64, 0x00, 0x6D, 0x6F, 0x76, 0x65, 0x00, 0x00, 0x00 };

// [.sdata] 0x80663CA0-0x80663CB0 (16 bytes)
extern "C" u32 lbl_eu_80663CA0[0x2] = { (u32)&lbl_eu_80524810, 0x00000000 };
extern "C" u32 lbl_eu_80663CA8[0x1] = { 0x72656600 };
extern "C" u32 lbl_eu_80663CAC[0x1] = { 0x72656600 };


