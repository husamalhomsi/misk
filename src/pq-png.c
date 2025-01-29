// pq-png.c - write a PQ image to a PNG file

#include <getopt.h>
#include <inttypes.h>
#include <png.h>
#include <setjmp.h>
#include <stdio.h>
#include <stdlib.h>
#include <zlib.h>

#include "pq.h"

// A max length equal to 1 << 15 for a max area equal to 1 << 30 (gibipixel)
#define MAX_LENGTH 32768

int main(int argc, char *argv[]) {
  _Bool best_compression = 0;

  int opt;

  while ((opt = getopt(argc, argv, "p:q:x:y:o:w:h:l:mz")) != -1)
    switch (opt) {
      case 'p': pq_id = strtoul(optarg, 0, 10);
        break;

      case 'q': pq_q = strtoul(optarg, 0, 10);
        break;

      case 'x': pq_x0 = strtoul(optarg, 0, 10);
        break;

      case 'y': pq_y0 = strtoul(optarg, 0, 10);
        break;

      case 'o': pq_x0 = pq_y0 = strtoul(optarg, 0, 10);
        break;

      case 'w': pq_width = strtoul(optarg, 0, 10);
        break;

      case 'h': pq_height = strtoul(optarg, 0, 10);
        break;

      case 'l': pq_width = pq_height = strtoul(optarg, 0, 10);
        break;

      // Place the origin in the middle of the image
      case 'm':
        pq_x0 = -(pq_width  / 2);
        pq_y0 = -(pq_height / 2);
        break;

      case 'z': best_compression = 1;
        break;

      default: return 1;
    }

  if (!pq_defined() || !pq_width || !pq_height || pq_width > MAX_LENGTH || pq_height > MAX_LENGTH)
    return 2;

  png_struct *structp = png_create_write_struct(PNG_LIBPNG_VER_STRING, 0, 0, 0);
  png_info *infop = png_create_info_struct(structp);

  if (!(structp && infop))
    return 3;

  // The longest filename is
  // p10q1000000000w10000h10000x1000000000y1000000000.pq.png
  char filename[56];
  sprintf(filename, "p%" PRIu32 "q%" PRIu32 "w%" PRIu32 "h%" PRIu32 "x%" PRIu32 "y%" PRIu32 ".pq.png",
    pq_id, pq_q, pq_width, pq_height, pq_x0, pq_y0);

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
  png_set_IHDR(structp, infop, pq_width, pq_height, 1, PNG_COLOR_TYPE_GRAY, PNG_INTERLACE_NONE,
    PNG_COMPRESSION_TYPE_BASE, PNG_FILTER_TYPE_BASE);

  pq_canvas = malloc(pq_width * pq_height * sizeof *pq_canvas);
  png_byte *row = malloc((pq_width + 7) / 8 * sizeof *row);

  if (!(pq_canvas && row)) {
    remove(filename);
    return 6;
  }

  pq_painters[pq_id]();
  png_init_io(structp, file);
  png_write_info(structp, infop);

  for (uint32_t y = 0; y < pq_height; ++y) {
    for (uint32_t x = 0; x < pq_width; ++x) {
      if (!(x % 8))
        row[x / 8] = 0;

      if (!pq_canvas[y * pq_width + x])
        row[x / 8] |= 1 << (7 - x % 8);
    }

    png_write_row(structp, row);
  }

  png_write_end(structp, 0);
  png_destroy_write_struct(&structp, &infop);
  free(pq_canvas);
  free(row);
  fclose(file);
  printf("Wrote %s\n", filename);
}
