#pragma once
/**
 * Provide various functions to generate psuedo-random numbers
 *
 */

#include <stdint.h>
struct junk_rand {
    uint64_t x;
    uint64_t m;
    uint64_t a;
    uint64_t c;
};

/**
 * Seed a junk_rand generator.
 * The default values for the LCG are the same as the Java
 * values. See https://en.wikipedia.org/wiki/Linear_congruential_generator#Parameters_in_common_use
 *
 * @param seed generator seed
 * @param r Output generator
 * @return 0 on success
 */
int junk_rand_init(int seed, struct junk_rand* r);
int junk_rand(struct junk_rand* r);
