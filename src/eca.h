// eca.h - library for elementary cellular automata

#include <stdint.h>

#define RAND_LSB 0
#define RAND_MSB 31

// Generic rule application
static inline _Bool eca_rule(uint8_t number, _Bool p, _Bool q, _Bool r) {
  return number >> (p << 2 | q << 1 | r) & 1;
}

// Specific rule application
static inline _Bool eca_rule_30(_Bool p, _Bool q, _Bool r) {
  return p ^ (q | r);
}

static uint32_t eca_next = 1;

static uint32_t eca_rand(void) {
  uint32_t accumulator = 0;

  for (int b = RAND_LSB; b <= RAND_MSB; ++b)
    accumulator |= (uint32_t) eca_rule_30(
      (uint32_t) 1 << (b == RAND_MSB ? RAND_LSB : b + 1) & eca_next,
      (uint32_t) 1 <<                             b      & eca_next,
      (uint32_t) 1 << (b == RAND_LSB ? RAND_MSB : b - 1) & eca_next) << b;

  return eca_next = accumulator;
}

static void eca_srand(uint32_t seed) {
  eca_next = seed;
}

#undef RAND_LSB
#undef RAND_MSB
