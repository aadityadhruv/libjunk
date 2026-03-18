#include "junk/random.h"
#include <limits.h>


int junk_rand_init(int seed, struct junk_rand* r) {
    r->x = seed;
    r->m = (1ul << 48);
    r->a = 25214903917;
    r->c = 11;
    return 0;
}
int junk_rand(struct junk_rand* r) {
    // Bitwise AND is optimization for power of 2 moduleo
    // See: https://stackoverflow.com/questions/11076216/re-implement-modulo-using-bit-shifts
    r->x = ((r->a * r->x) + r->c) & (r->m - 1);
    return (r->x >> 16);
}
