#ifndef PPC_FIXTURE_EXEC_H
#define PPC_FIXTURE_EXEC_H

#include <types.h>

#include "ppc_fixture_cases.inc"

typedef struct PpcFixtureActual {
    u32 result;
    u32 cr;
    u32 xer;
    u64 fpr[32];
    u32 fpscr;
    u32 mem[PPC_FIXTURE_MEM_WORDS];
} PpcFixtureActual;

/* Execute one data-driven fixture payload in the guest. */
void ppc_fixture_run_case(const PpcFixtureCase* fixture, u32* sandbox, PpcFixtureActual* actual);

#endif
