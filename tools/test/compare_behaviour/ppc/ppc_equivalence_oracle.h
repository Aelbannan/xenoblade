#ifndef PPC_EQUIVALENCE_ORACLE_H
#define PPC_EQUIVALENCE_ORACLE_H

#include <types.h>

#define PPC_EQ_ORACLE_CASES 40
#define PPC_EQ_WORDS_PER_CASE 3

/* Each case writes result, CR, and XER. Memory cases use result and clear CR/XER. */
void ppc_equivalence_oracle(u32* output, u32* memory);

#endif
