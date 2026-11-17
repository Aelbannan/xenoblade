#pragma once

#include "monolib/work/CTTask.hpp"

class IUICf;

// CTTask<IUICf> is defined inline: retail emits one weak copy of each function
// in the first unit that needs it (kyoshin/CMainMenu.cpp).
template <>
inline CTTask<IUICf>::~CTTask() {}

// Common task base of the in-game menus.
class IUICf : public CTTask<IUICf> {
public:
    IUICf() : mField54(0), mField55(0) {}

    u8 mField54; // 0x54
    u8 mField55; // 0x55
}; // size: 0x58

template <>
inline void CTTask<IUICf>::Move() {
    if (mMoveFunc) {
        (static_cast<IUICf*>(this)->*mMoveFunc)();
    }
}

template <>
inline void CTTask<IUICf>::Draw() {
    if (mDrawFunc) {
        (static_cast<IUICf*>(this)->*mDrawFunc)();
    }
}
