#pragma once
// Deleted: ResHandleLocal / ParentInt98Local fake pads are gone.
// Real owners: parent+0x98 is ::CScnItemModel (vfunc64/vfunc88, see
// libs/monolib/src/scn/CScnItemModel.hpp); parent+0x98-slot check is
// cf::CfObject::CfObject_checkTargetState (0x98). CfResReloadImpl.cpp now
// calls the real classes directly; this header is kept empty so the
// historic include path does not break.
