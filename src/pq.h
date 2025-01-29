// pq.h - library for PQ images

#include <stdint.h>
#include <stdlib.h>

#define Q pq_q

#define P(id, formula)                                           \
  static void pq_p##id(void) {                                   \
    uint32_t yn = pq_y0 + pq_height;                             \
    uint32_t xn = pq_x0 + pq_width;                              \
                                                                 \
    for (uint32_t y = pq_y0; y != yn; ++y)                       \
      for (uint32_t x = pq_x0; x != xn; ++x)                     \
        pq_canvas[(y - pq_y0) * pq_width + x - pq_x0] = formula; \
  }

static uint32_t pq_id     = 0;
static uint32_t pq_q      = 1;
static uint32_t pq_width  = 256;
static uint32_t pq_height = 256;
static uint32_t pq_x0     = 0;
static uint32_t pq_y0     = 0;

static _Bool *pq_canvas;

static _Bool pq_defined(void) {
  return pq_id < 50 && (pq_q || !(pq_id / 5 % 2));
}

P(0, (x & y) & Q)
P(1, (x | y) & Q)
P(2, (x ^ y) & Q)
P(3, (x + y) & Q)
P(4, (x * y) & Q)

P(5, (x & y) % Q)
P(6, (x | y) % Q)
P(7, (x ^ y) % Q)
P(8, (x + y) % Q)
P(9, (x * y) % Q)

P(10, (x * x & y * y) & Q)
P(11, (x * x | y * y) & Q)
P(12, (x * x ^ y * y) & Q)
P(13, (x * x + y * y) & Q)
P(14, (x * x * y * y) & Q)

P(15, (x * x & y * y) % Q)
P(16, (x * x | y * y) % Q)
P(17, (x * x ^ y * y) % Q)
P(18, (x * x + y * y) % Q)
P(19, (x * x * y * y) % Q)

P(20, (Q * x & Q * y) & Q)
P(21, (Q * x | Q * y) & Q)
P(22, (Q * x ^ Q * y) & Q)
P(23, (Q * x + Q * y) & Q)
P(24, (Q * x * Q * y) & Q)

P(25, (Q * x & Q * y) % Q)
P(26, (Q * x | Q * y) % Q)
P(27, (Q * x ^ Q * y) % Q)
P(28, (Q * x + Q * y) % Q)
P(29, (Q * x * Q * y) % Q)

P(30, ((Q & x) * x & (Q & y) * y) & Q)
P(31, ((Q | x) * x & (Q | y) * y) & Q)
P(32, ((Q ^ x) * x & (Q ^ y) * y) & Q)
P(33, ((Q + x) * x & (Q + y) * y) & Q)
P(34, ((Q * x) * x & (Q * y) * y) & Q)

P(35, ((Q & x) * x & (Q & y) * y) % Q)
P(36, ((Q | x) * x & (Q | y) * y) % Q)
P(37, ((Q ^ x) * x & (Q ^ y) * y) % Q)
P(38, ((Q + x) * x & (Q + y) * y) % Q)
P(39, ((Q * x) * x & (Q * y) * y) % Q)

P(40, ((Q & x) * x + (Q & y) * y) & Q)
P(41, ((Q | x) * x + (Q | y) * y) & Q)
P(42, ((Q ^ x) * x + (Q ^ y) * y) & Q)
P(43, ((Q + x) * x + (Q + y) * y) & Q)
P(44, ((Q * x) * x + (Q * y) * y) & Q)

P(45, ((Q & x) * x + (Q & y) * y) % Q)
P(46, ((Q | x) * x + (Q | y) * y) % Q)
P(47, ((Q ^ x) * x + (Q ^ y) * y) % Q)
P(48, ((Q + x) * x + (Q + y) * y) % Q)
P(49, ((Q * x) * x + (Q * y) * y) % Q)

static void (*pq_painters[50])(void) = {
  // &    |       ^       +       *
  pq_p0,  pq_p1,  pq_p2,  pq_p3,  pq_p4,  // Class 0
  pq_p5,  pq_p6,  pq_p7,  pq_p8,  pq_p9,  // Class 1
  pq_p10, pq_p11, pq_p12, pq_p13, pq_p14, // Class 2
  pq_p15, pq_p16, pq_p17, pq_p18, pq_p19, // Class 3
  pq_p20, pq_p21, pq_p22, pq_p23, pq_p24, // Class 4
  pq_p25, pq_p26, pq_p27, pq_p28, pq_p29, // Class 5
  pq_p30, pq_p31, pq_p32, pq_p33, pq_p34, // Class 6
  pq_p35, pq_p36, pq_p37, pq_p38, pq_p39, // Class 7
  pq_p40, pq_p41, pq_p42, pq_p43, pq_p44, // Class 8
  pq_p45, pq_p46, pq_p47, pq_p48, pq_p49  // Class 9
};

#undef Q
#undef P
