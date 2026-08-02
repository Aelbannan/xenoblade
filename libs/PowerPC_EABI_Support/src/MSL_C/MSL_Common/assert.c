#include <assert.h>
#include <stdio.h>

extern void abort();

//unused parameters kept to match the retail signature
void __msl_assertion_failed(const char* condition, const char* filename, const char* function, int line) {
    fprintf(stderr, "Assertion (%s) failed in \"%s\", line %d\n", condition, filename, line);
    abort();
}
