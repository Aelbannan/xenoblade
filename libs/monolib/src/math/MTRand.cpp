#include "monolib/math.hpp"

extern "C" {
    extern const f64 lbl_eu_8066A1D8;
    extern const f32 lbl_eu_8066A1D0;
    extern const f32 lbl_eu_8066A1E0;
    s8 lbl_eu_80665580;
}

// MWCC u32 -> f64 conversion: build the 0x43300000-prefixed bit pattern in
// memory and subtract the named retail .sdata2 magic (lbl_eu_8066A1D8 = 2^52)
// so the pool reloc matches retail (lfd lbl_eu_8066A1D8@sda21) instead of an
// MWCC-synthesised @N entry (CFloorMap.cpp / CSysWinScenarioLog.cpp convention).
union U32ToF64Conv {
    u32 w[2];
    f64 d;
};

namespace ml{

    MTRand* MTRand::getInstance(){
        static MTRand instance;
        if(!lbl_eu_80665580){
            instance.srand(0x012BD6AA);
            instance.pNext = instance.state;
            lbl_eu_80665580 = 1;
        }
        return &instance;
    }

    void MTRand::srand(u32 seed) {
        if(seed == 0) seed = 0x012BD6AA;

        state[0] = seed;

        for(int i = 1; i < N; i++){
            state[i] = 0x6C078965 * (state[i - 1] ^ (state[i - 1] >> 30)) + i;
        }

        left = 1;
        initialized = true;
    }

    void MTRand::nextMt(){
        if(!initialized){
            srand(0x1571);
        }

        left = N;
        pNext = state;

        int i;
        u32* p = state;

        for(i = N - M + 1; --i; ++p) {
            *p = twist(p[M], p[0], p[1]);
        }
        for(i = M; --i; ++p) {
            *p = twist(p[M - N], p[0], p[1]);
        }
        *p = twist(p[M - N], p[0], state[0]);
    }

    inline u32 MTRand::rand32() {
        left--;
        if(left <= 0) nextMt();

        u32 r4 = *pNext++;
        r4 ^= (r4 >> 0xB);
        r4 ^= (r4 << 7) & 0x9D2C5680;
        r4 ^= (r4 << 0xF) & 0xEFC60000;
        r4 ^= r4 >> 0x12;
        return r4;
    }

    u32 MTRand::rand31(){
        return rand32() >> 1;
    }

    //func_80435B5C
    float MTRand::randFloat() {
        U32ToF64Conv conv;
        conv.w[0] = 0x43300000;
        conv.w[1] = rand32();
        return (f32)(conv.d - lbl_eu_8066A1D8) * lbl_eu_8066A1D0;
    }

    //func_80435BF8
    float MTRand::randFloat1() {
        U32ToF64Conv conv;
        conv.w[0] = 0x43300000;
        conv.w[1] = rand32();
        return (lbl_eu_8066A1E0 + (f32)(conv.d - lbl_eu_8066A1D8)) * lbl_eu_8066A1D0;
    }

} //namespace ml
