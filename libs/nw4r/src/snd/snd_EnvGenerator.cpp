#include <nw4r/snd.h>

extern "C" __declspec(section ".rodata") __attribute__((aligned(8))) const short lbl_eu_8051FC40[128] = {
    -723, -722, -721, -651, -601, -562, -530, -503,
    -480, -460, -442, -425, -410, -396, -383, -371,
    -360, -349, -339, -330, -321, -313, -305, -297,
    -289, -282, -276, -269, -263, -257, -251, -245,
    -239, -234, -229, -224, -219, -214, -210, -205,
    -201, -196, -192, -188, -184, -180, -176, -173,
    -169, -165, -162, -158, -155, -152, -149, -145,
    -142, -139, -136, -133, -130, -127, -125, -122,
    -119, -116, -114, -111, -109, -106, -103, -101,
    -99, -96, -94, -91, -89, -87, -85, -82,
    -80, -78, -76, -74, -72, -70, -68, -66,
    -64, -62, -60, -58, -56, -54, -52, -50,
    -49, -47, -45, -43, -42, -40, -38, -36,
    -35, -33, -31, -30, -28, -27, -25, -23,
    -22, -20, -19, -17, -16, -14, -13, -11,
    -10, -8, -7, -6, -4, -3, -1, 0
};
extern "C" __declspec(section ".rodata") __attribute__((aligned(8))) const float lbl_eu_8051FD40[128] = {
    0.9992175102233887f, 0.9984325766563416f, 0.9976451992988586f, 0.9968553185462952f,
    0.9960628747940063f, 0.995267927646637f, 0.9944704174995422f, 0.9936704039573669f,
    0.9928677082061768f, 0.992062509059906f, 0.9912546277046204f, 0.9904441237449646f,
    0.9896308779716492f, 0.9888151288032532f, 0.987996518611908f, 0.9871752262115479f,
    0.9863511919975281f, 0.9855244159698486f, 0.9846948981285095f, 0.9838625192642212f,
    0.9830272793769836f, 0.9821892976760864f, 0.9813482761383057f, 0.9805045127868652f,
    0.9796578288078308f, 0.9788081049919128f, 0.9779555201530457f, 0.9770998954772949f,
    0.9762412905693054f, 0.9753797054290771f, 0.9745150208473206f, 0.9736471772193909f,
    0.9727762937545776f, 0.9719023108482361f, 0.9710251092910767f, 0.9701448082923889f,
    0.9692612290382385f, 0.9683743715286255f, 0.9674844145774841f, 0.9665910005569458f,
    0.9656944274902344f, 0.964794397354126f, 0.9638910293579102f, 0.9629842042922974f,
    0.9620739817619324f, 0.96116042137146f, 0.960243284702301f, 0.9593225717544556f,
    0.9583984017372131f, 0.9574705958366394f, 0.9565392136573792f, 0.9556041955947876f,
    0.95466548204422f, 0.9537230730056763f, 0.9527769088745117f, 0.9518269896507263f,
    0.9508731961250305f, 0.9499157071113586f, 0.9489542245864868f, 0.9479888081550598f,
    0.9470195174217224f, 0.9460461735725403f, 0.945068895816803f, 0.9440875053405762f,
    0.9431020021438599f, 0.942112386226654f, 0.941118597984314f, 0.9401205778121948f,
    0.9391183853149414f, 0.9381117820739746f, 0.937100887298584f, 0.93608558177948f,
    0.9350659251213074f, 0.9340416789054871f, 0.9330130815505981f, 0.931979775428772f,
    0.9309419989585876f, 0.9298995137214661f, 0.9288523197174072f, 0.9278004169464111f,
    0.9267436265945435f, 0.9256821274757385f, 0.9246156215667725f, 0.9235442280769348f,
    0.922467827796936f, 0.9213864207267761f, 0.9202998280525208f, 0.9192081093788147f,
    0.9181112051010132f, 0.9170091152191162f, 0.9159016013145447f, 0.9147887229919434f,
    0.9136703014373779f, 0.9125465154647827f, 0.9114171266555786f, 0.9102820754051208f,
    0.9091414213180542f, 0.9079949259757996f, 0.9068427085876465f, 0.9056844711303711f,
    0.9045203924179077f, 0.9033501744270325f, 0.9021739959716797f, 0.9009916186332703f,
    0.8998029232025146f, 0.8986080288887024f, 0.8974065780639648f, 0.8961988091468811f,
    0.8949844241142273f, 0.8900598883628845f, 0.882462203502655f, 0.8759247064590454f,
    0.8691861033439636f, 0.8636406064033508f, 0.8535788059234619f, 0.8430188894271851f,
    0.8286135196685791f, 0.8149098753929138f, 0.8002172112464905f, 0.7780662775039673f,
    0.7554749846458435f, 0.7242125272750854f, 0.682823896408081f, 0.6329169273376465f,
    0.5592135190963745f, 0.4551410973072052f, 0.32987698912620544f, 0.0f
};

namespace nw4r {
namespace snd {
namespace detail {


// VOLUME_INIT is not defined here: the retail split carries no .sdata2 and the
// retail image supplies the static member via its data slice. The ctor passes
// lbl_eu_80669F30 explicitly to match retail relocs; no function in this TU
// odr-uses the default arg.

EnvGenerator::EnvGenerator() {
    Init(lbl_eu_80669F30);
}

void EnvGenerator::Init(f32 db) {
    SetAttack(127);
    mHold = 0;
    mDecay = lbl_eu_80669F34;
    mSustain = 127;
    mRelease = lbl_eu_80669F34;
    mValue = lbl_eu_80669F38 * db;
    mStatus = STATUS_ATTACK;
}

void EnvGenerator::Reset(f32 db) {
    mValue = lbl_eu_80669F38 * db;
    mStatus = STATUS_ATTACK;
}

f32 EnvGenerator::GetValue() const {
    // Constant on the left: MWCC loads the named 0.0f into f1 before the
    // field (retail lfs f1, lbl_eu_80669F3C@sda2 first, fcmpu f1, f0).
    if (mStatus == STATUS_ATTACK && lbl_eu_80669F3C == mAttack) {
        return lbl_eu_80669F3C;
    }

    return mValue / lbl_eu_80669F38;
}

void EnvGenerator::Update(int msec) {
    switch (mStatus) {
    case STATUS_ATTACK: {
        int i = msec;

        while (i-- > 0) {
            mValue = mValue * mAttack;
            if (mValue > lbl_eu_80669F40) {
                mValue = lbl_eu_80669F3C;
                mStatus = STATUS_HOLD;
                mHoldCounter = mHold;
                return;
            }
        }
        break;
    }

    case STATUS_HOLD: {
        if (msec < mHoldCounter) {
            mHoldCounter -= msec;
        } else {
            msec -= mHoldCounter;
            mHoldCounter = 0;
            mStatus = STATUS_DECAY;
        }

        if (mStatus != STATUS_DECAY) {
            break;
        }
        // FALLTHROUGH
    }

    case STATUS_DECAY: {
        f32 target = (f32)lbl_eu_8051FC40[mSustain];
        mValue -= mDecay * (f32)msec;

        if (mValue < target) {
            mValue = target;
            mStatus = STATUS_SUSTAIN;
        }
        break;
    }

    case STATUS_SUSTAIN: {
        break;
    }

    case STATUS_RELEASE: {
        mValue -= mRelease * (f32)msec;
        break;
    }
    }
}


void EnvGenerator::SetAttack(int attack) {
    mAttack = lbl_eu_8051FD40[attack];
}

void EnvGenerator::SetDecay(int decay) {
    mDecay = CalcRelease(decay);
}

void EnvGenerator::SetSustain(int sustain) {
    mSustain = sustain;
}

void EnvGenerator::SetRelease(int release) {
    mRelease = CalcRelease(release);
}

void EnvGenerator::SetHold(int hold) {
    mHold = ((hold + 1) * (hold + 1)) / 4;
}

} // namespace detail
} // namespace snd
} // namespace nw4r
