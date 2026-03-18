#include "junk/random.h"
#include <assert.h>


void test_rand_call_once() {
    struct junk_rand rand;
    junk_rand_init(0, &rand);
    int r = junk_rand(&rand);
    assert(rand.x == 11);
    assert(r == 0);
}


int main() {
    test_rand_call_once();
    return 0;
}
