#include "monolib/math.hpp"

extern "C" {
    s8 lbl_eu_80665580;
}


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

        u32 x = *pNext++;
        x ^= (x >> 0xB);
        x ^= (x << 7) & 0x9D2C5680;
        x ^= (x << 0xF) & 0xEFC60000;
        x ^= x >> 0x12;
        return x;
    }

    u32 MTRand::rand31(){
        return rand32() >> 1;
    }

    //func_80435B5C -- retail is MWCC's builtin u32->f32 cast idiom
    //(lis 0x4330/stw, lfd, fsubs vs the pooled 2^52 magic) -- KB ref:bc80cd6fac.
    float MTRand::randFloat() {
        return (f32)rand32() * 2.3283064e-10f;
    }

    //func_80435BF8
    float MTRand::randFloat1() {
        return (0.5f + (f32)rand32()) * 2.3283064e-10f;
    }

} //namespace ml
