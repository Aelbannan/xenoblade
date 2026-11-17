#pragma once

#include <types.h>

namespace cf {
    // Root interface of the cf object/gimmick hierarchies (RTTI name
    // "cf::IObjectInfo", __RTTI__Q22cf11IObjectInfo). Only the virtual dtor is known to
    // live here; derived classes declare their own slot-1 virtual (e.g.
    // CfGimmick::CfGimmick_DetachManager, CVision/CChainEffect/CSuddenCommu
    // IObjectInfo_UnkVirtualFunc1), so the layout is unchanged. Retail has
    // no IObjectInfo vtable object at all (only the RTTI record and the
    // out-of-line dtor copy), hence novtable.
    class __declspec(novtable) IObjectInfo {
    public:
        virtual ~IObjectInfo(){}
    };
}
