#pragma once

#include <types.h>

class CScn;

// Minimal reslist template for CTaskGameEff destructor emission.
// The full definition in monolib/util/reslist.hpp stores T by value
// (illegal for abstract CScn); retail uses a padding-based layout.
template <typename T>
class _reslist_base {
public:
    virtual ~_reslist_base();
    char _pad[0x1F];
};

template <typename T>
class reslist : public _reslist_base<T> {
public:
    virtual ~reslist();
    char _pad2[0x20 - sizeof(_reslist_base<T>)];
};

class CTaskGameEff {
public:
    CTaskGameEff();
    virtual ~CTaskGameEff();
    void Init();
    void Term();

    // TODO: add fields
    void Move();
    void cbRenderBefore();
    void Draw();
};

