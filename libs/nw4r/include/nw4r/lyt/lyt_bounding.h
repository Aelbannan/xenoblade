#ifndef NW4R_LYT_BOUNDING_H
#define NW4R_LYT_BOUNDING_H
#include <nw4r/types_nw4r.h>

#include <nw4r/lyt/lyt_pane.h>

#include <nw4r/ut.h>

// C-linkage imports (retail symbol names - keep linkage/signatures verbatim)
extern "C" nw4r::ut::detail::RuntimeTypeInfo lbl_eu_80665470;
extern "C" nw4r::ut::detail::RuntimeTypeInfo lbl_eu_80665498;

// Retail vtable for Bounding lives in the shared data pool
// (lbl_eu_80569B80, outside this split slice).
extern "C" void* lbl_eu_80569B80[];

namespace nw4r {
namespace lyt {

// Forward declarations
struct ResBlockSet;

namespace res {

/******************************************************************************
 *
 * BND1 binary layout
 *
 ******************************************************************************/
struct Bounding : Pane {
    static const u32 SIGNATURE = FOURCC('b', 'n', 'd', '1');
};

} // namespace res

/******************************************************************************
 *
 * Bounding
 *
 ******************************************************************************/
// __declspec(novtable): the retail vtable lives in the shared data pool
// (lbl_eu_80569B80), so MWCC must not emit a local __vt__Bounding; the ctor
// stores the extern vtable address explicitly (retail's vptr store is the
// last statement of the ctor body).
class __declspec(novtable) Bounding : public Pane {
public:
    NW4R_UT_RTTI_DECL(Bounding);

public:
    Bounding(const res::Bounding* pRes, const ResBlockSet& rBlockSet);
    virtual ~Bounding(); // at 0x8

    virtual void DrawSelf(const DrawInfo& rInfo); // at 0x18
};

} // namespace lyt
} // namespace nw4r

#endif
