#pragma once

#include <types.h>
#include "monolib/lod/UnkClass_8046A530.hpp"

namespace LOD {

/**
 * LOD memory manager.  Large object; only fields used by matched
 * functions are declared here.  Remaining layout is unknown padding.
 */
class LODMemMan {
public:
    LODMemMan();
    ~LODMemMan();
    float func_8046F01C();
    void func_8046F088();
    void func_80471834();

    void OnFileEvent();
    void func_8046D264();
    void func_8046D898();
    void func_8046DA04();
    void func_8046DA64();
    void func_8046DAC0();
    void func_8046DBC8();
    void func_8046DD9C();
    void func_8046E1DC();
    void func_8046E594();
    void func_8046E5BC();
    void func_8046E6DC();
    void func_8046E770();
    void func_8046E780();
    void func_8046E7D0();
    void func_8046E820();
    void func_8046E8C8();
    void func_8046E920();
    void func_8046E988();
    void func_8046EAE8();
    void func_8046EB50();
    void func_8046EBEC();
    void func_8046EC88();
    void func_8046ECD4();
    void func_8046ED68();
    void func_8046EDD0();
    void func_8046EE9C();
    void func_8046EEE8();
    void func_8046EF30();
    void func_8046EF7C();
    void func_8046F010(float a, float b);
    void func_8046F024();
    void func_8046F090();
    void func_8046F164();
    void func_8046F258();
    void func_8046F594();
    void func_8046FC04();
    void func_8046FEB8();
    void func_8046FF84();
    void func_80470184();
    void func_804702F0();
    void func_80470634();
    void func_804708B4();
    void func_804709FC();
    void func_80470A90();
    void func_80470B10();
    void func_80470DB0();
    void func_80470DCC();
    void func_80470EF8();
    void func_8047108C();
    void func_80471184();
    void func_80471224();
    void func_804712E0();
    void func_8047130C();
    void func_8047133C();
    void func_80471450();
    void func_8047146C();
    void func_80471484();
    void func_8047163C();
    void func_804716B8();
    void func_80471718();
    void func_80471748();
    void func_80471780();
    void func_80471794();
    void func_804717FC();
    void func_80471938();
    void func_804719FC();
    void func_80471A70();
    void func_80471ACC();
    void func_80471BC8();
    void func_80471BF4();
    void func_80471CC4();
    void func_80471CCC();
    void func_80471CDC();
private:
    u8  _00[0x7c];                    // unknown leading fields
    f32 mFloat_7C;                    // offset 0x7c
    u8  _80[0xabc - 0x80];            // unknown padding
    UnkClass_8046A530 mSubObject;     // offset 0xabc (embedded sub-object)
};

} // namespace LOD
