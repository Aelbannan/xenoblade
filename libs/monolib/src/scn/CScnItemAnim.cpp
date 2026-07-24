/**
 * CScnItemAnim - Scene animation item
 *
 * Manages a character animation resource (nw4r::g3d::ResFile) within
 * the scene graph. Provides access to the embedded ResFile handle,
 * animation name, and related metadata.
 */

#include <monolib/scn/CScnItemAnim.hpp>

// LLM-HARNESS-BEGIN: us-804a2864
/**
 * Get a character animation resource by index from the embedded ResFile.
 *
 * Computes `&this->mResFile` (offset 0x0C) and delegates to
 * nw4r::g3d::ResFile::GetResAnmChr(int).  Implemented as a tail
 * call in the original binary.
 */
extern "C" nw4r::g3d::ResAnmChr func_8049E708(CScnItemAnim* self, int index) {
    return self->mResFile.GetResAnmChr(index);
}
// LLM-HARNESS-END: us-804a2864

// LLM-HARNESS-BEGIN: us-804a2a10
/**
 * Return pointer to the animation name buffer at offset 0x10.
 *
 * The name is a C-string copied from the first animation entry
 * during construction.  The buffer is 0x40 bytes.
 */
extern "C" char* func_8049E8B4(CScnItemAnim* self) {
    return self->mName;
}
// LLM-HARNESS-END: us-804a2a10

// LLM-HARNESS-BEGIN: us-804a2a18
/**
 * Return the value at offset 0x54 of the object.
 *
 * This field is initialised to the same value as mResFile during
 * construction and may serve as a cached identifier.
 */
extern "C" nw4r::g3d::ResFile func_8049E8BC(CScnItemAnim* self) {
    return self->mResFileCopy;
}
// LLM-HARNESS-END: us-804a2a18
