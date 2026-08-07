#include "kyoshin/help/CHelp_Sp.hpp"
#include "kyoshin/cf/CfGameManager.hpp"
#include "kyoshin/cf/object/CfObjectPc.hpp"

extern cf::CfObjectPc* func_800BFC68(cf::CfObjectMove* objMove);

// Interface shim to dispatch through the CfObjectPc vtable at slot 0x160.
// The 89th virtual (vtable offset 0x160) returns an f32 (a measured value/
// distance) that CHelp_Sp compares against mThreshold. Using a genuine virtual
// call makes MWCC emit the r12 vtable-dispatch sequence matching retail,
// instead of a raw function-pointer chain.
struct CfObjectPcSpeed {
    virtual void _v000();
    virtual void _v004();
    virtual void _v008();
    virtual void _v00C();
    virtual void _v010();
    virtual void _v014();
    virtual void _v018();
    virtual void _v01C();
    virtual void _v020();
    virtual void _v024();
    virtual void _v028();
    virtual void _v02C();
    virtual void _v030();
    virtual void _v034();
    virtual void _v038();
    virtual void _v03C();
    virtual void _v040();
    virtual void _v044();
    virtual void _v048();
    virtual void _v04C();
    virtual void _v050();
    virtual void _v054();
    virtual void _v058();
    virtual void _v05C();
    virtual void _v060();
    virtual void _v064();
    virtual void _v068();
    virtual void _v06C();
    virtual void _v070();
    virtual void _v074();
    virtual void _v078();
    virtual void _v07C();
    virtual void _v080();
    virtual void _v084();
    virtual void _v088();
    virtual void _v08C();
    virtual void _v090();
    virtual void _v094();
    virtual void _v098();
    virtual void _v09C();
    virtual void _v0A0();
    virtual void _v0A4();
    virtual void _v0A8();
    virtual void _v0AC();
    virtual void _v0B0();
    virtual void _v0B4();
    virtual void _v0B8();
    virtual void _v0BC();
    virtual void _v0C0();
    virtual void _v0C4();
    virtual void _v0C8();
    virtual void _v0CC();
    virtual void _v0D0();
    virtual void _v0D4();
    virtual void _v0D8();
    virtual void _v0DC();
    virtual void _v0E0();
    virtual void _v0E4();
    virtual void _v0E8();
    virtual void _v0EC();
    virtual void _v0F0();
    virtual void _v0F4();
    virtual void _v0F8();
    virtual void _v0FC();
    virtual void _v100();
    virtual void _v104();
    virtual void _v108();
    virtual void _v10C();
    virtual void _v110();
    virtual void _v114();
    virtual void _v118();
    virtual void _v11C();
    virtual void _v120();
    virtual void _v124();
    virtual void _v128();
    virtual void _v12C();
    virtual void _v130();
    virtual void _v134();
    virtual void _v138();
    virtual void _v13C();
    virtual void _v140();
    virtual void _v144();
    virtual void _v148();
    virtual void _v14C();
    virtual void _v150();
    virtual void _v154();
    virtual float _v160(); // vtable offset 0x160
};

namespace cf {
bool CHelp_Sp::func_802B8654() {
    CfObjectPc* objPc = func_800BFC68(CfGameManager::getPlayer(0));
    if (objPc != nullptr) {
        f32 value = reinterpret_cast<CfObjectPcSpeed*>(objPc)->_v160();
        return mThreshold <= value;
    }
    return false;
}
} // namespace cf