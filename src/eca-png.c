// eca-png.c - write an elementary cellular automaton to a PNG file

#include <getopt.h>
#include <inttypes.h>
#include <png.h>
#include <setjmp.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <zlib.h>

#include "eca.h"

// A max length equal to 1 << 15 for a max area equal to 1 << 30 (gibipixel)
#define MAX_LENGTH 32768

int main(int argc, char *argv[]) {
  _Bool    best_compression = 0;
  uint8_t  rule             = 18;
  uint32_t seed             = 0;
  uint32_t cell_count       = 256;
  uint32_t gen_count        = 128;

  int opt;

  while ((opt = getopt(argc, argv, "r:s:c:g:z")) != -1)
    switch (opt) {
      case 'r': rule = strtoul(optarg, 0, 10);
        break;

      case 's': seed = strtoul(optarg, 0, 10);
        break;

      case 'c': cell_count = strtoul(optarg, 0, 10);
        break;

      case 'g': gen_count = strtoul(optarg, 0, 10);
        break;

      case 'z': best_compression = 1;
        break;

      default: return 1;
    }

  if (!cell_count || !gen_count || cell_count > MAX_LENGTH || gen_count > MAX_LENGTH)
    return 2;

  png_struct *structp = png_create_write_struct(PNG_LIBPNG_VER_STRING, 0, 0, 0);
  png_info *infop = png_create_info_struct(structp);

  if (!(structp && infop))
    return 3;

  // The longest filename is
  // r100s1000000000c10000g10000.eca.png
  char filename[36];
  sprintf(filename, "r%" PRIu8 "s%" PRIu32 "c%" PRIu32 "g%" PRIu32 ".eca.png",
    rule, seed, cell_count, gen_count);

  FILE *file = fopen(filename, "wb");

  if (!file)
    return 4;

  if (setjmp(png_jmpbuf(structp))) {
    remove(filename);
    return 5;
  }

  png_set_compression_strategy(structp, Z_DEFAULT_STRATEGY);
  png_set_compression_level(structp, best_compression ? Z_BEST_COMPRESSION : Z_DEFAULT_COMPRESSION);
  png_set_filter(structp, PNG_FILTER_TYPE_BASE, PNG_FILTER_NONE);
  png_set_IHDR(structp, infop, cell_count, gen_count, 1, PNG_COLOR_TYPE_GRAY, PNG_INTERLACE_NONE,
    PNG_COMPRESSION_TYPE_BASE, PNG_FILTER_TYPE_BASE);

  png_init_io(structp, file);
  png_write_info(structp, infop);

  png_byte *row = malloc((cell_count + 7) / 8 * sizeof *row);
  _Bool *cells = malloc(cell_count * sizeof *cells);
  _Bool *accumulators = malloc(cell_count * sizeof *accumulators);

  if (!(row && cells && accumulators)) {
    remove(filename);
    return 6;
  }

  if (seed) {
    eca_srand(seed);

    for (uint32_t c = 0; c < cell_count; ++c)
      cells[c] = eca_rand() & 1;
  }
  else {
    for (uint32_t c = 0; c < cell_count; ++c)
      cells[c] = 0;

    cells[cell_count / 2] = 1;
  }

  uint32_t last_cell_position = cell_count - 1;

  for (uint32_t g = 0; g < gen_count; ++g) {
    for (uint32_t c = 0; c < cell_count; ++c) {
      if (!(c % 8))
        row[c / 8] = 0;

      if (cells[c])
        row[c / 8] |= 1 << (7 - c % 8);

      accumulators[c] = eca_rule(rule,
        cells[c == 0 ? last_cell_position : c - 1],
        cells[c],
        cells[c == last_cell_position ? 0 : c + 1]);
    }

    png_write_row(structp, row);
    memcpy(cells, accumulators, cell_count);
  }

  png_write_end(structp, 0);
  png_destroy_write_struct(&structp, &infop);
  free(row);
  free(cells);
  free(accumulators);
  fclose(file);
  printf("Wrote %s\n", filename);
}
