#include "junk/random.h"
#include <assert.h>
#include <stdlib.h>
#include <string.h>


void test_rand_call_once() {
    struct junk_rand* rand = junk_rand_init(0);
    int r = junk_rand(rand);
    assert(rand->x == 11);
    assert(r == 0);
    free(rand);
}


int main() {
    test_rand_call_once();
    return 0;
}
