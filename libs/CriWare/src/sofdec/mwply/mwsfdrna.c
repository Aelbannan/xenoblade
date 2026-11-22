// Auto-scaffolded catalog TU for CriWare/src/sofdec/mwply/mwsfdrna
// Replace stubs with high-level C/C++ during decomp.

#include <harness_catalog.h>
#include "libs/CriWare/src/sofdec/mwply/mwsfd_types.h"

void MWSFRNA_SetOutVol(MWSFDPLY* self, s32 vol) { SFD_SetOutVol(self->sfd, vol); }

s32 MWSFRNA_GetOutVol(MWSFDPLY* self) { return SFD_GetOutVol(self->sfd); }
