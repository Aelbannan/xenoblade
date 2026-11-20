#pragma once

// Owner header for monolib/src/scn/CMdlDynamics.cpp.  The full layouts are
// private to the defining TU; callers only pass pointers to these objects.
class CMdlDynamics;
struct CMdlDynSet;
struct CMdlDynModelRef;

// Real C++ entry points defined in CMdlDynamics.cpp (retail mangles them).
void MdlDyn_MarkAllDynamic(CMdlDynSet* self);
void MdlDyn_FreeAll(CMdlDynamics* self);
void MdlDyn_UpdateAnimSpeed(CMdlDynModelRef* self);
