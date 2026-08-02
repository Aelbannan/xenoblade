#include "PowerPC_EABI_Support/MSL_C/MSL_Common/rand.h"

#define RAND_MULTIPLIER 1103515245 // standard C LCG multiplier
#define RAND_INCREMENT 12345       // standard C LCG increment
#define RAND_MASK 0x7FFF

static u32 random_next = 1;

//LCG (linear congruential generator) RNG algorithm
int rand() {
    random_next = random_next * RAND_MULTIPLIER + RAND_INCREMENT;
    return ((random_next >> 16) & RAND_MASK);
}

//not present in the retail binary; kept commented out for reference
//void srand(u32 seed)
//{
//   random_next = seed;
//}
