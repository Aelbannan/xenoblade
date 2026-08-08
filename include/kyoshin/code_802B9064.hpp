#pragma once

#include <types.h>

class CCharVoice;

// C-linkage imports (retail symbol names - keep linkage/signatures verbatim)
// Retail voice-hook symbols are unmangled C globals (defined in the
// NonMatching retail CVS_* objects); extern "C" keeps the refs linkable
// since MWCC would otherwise mangle C++ free functions.
extern "C" bool func_802A330C(u32 size, u32 align);
extern "C" bool func_802A34E4(u32 size);
extern "C" int func_802A3D54(CCharVoice* voicePtr, int voiceId, int arg);
