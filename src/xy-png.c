// xy-png.c - write an XY image to a PNG file

#include <getopt.h>
#include <png.h>
#include <setjmp.h>
#include <stdio.h>
#include <stdlib.h>
#include <zlib.h>

#define BLACK  0
#define WHITE  1
#define FILLER 2

// A max length equal to 1 << 15 for a max area equal to 1 << 30 (gibipixel)
#define MAX_LENGTH 32768

static unsigned width  = 256;
static unsigned height = 256;

// Flood-fill a white area with a filler color
static void flood(png_byte image[], unsigned x, unsigned y) {
  unsigned long i = y * width + x;

  if (x >= width || y >= height || image[i] != WHITE)
    return;

  image[i] = FILLER;

  flood(image, x - 1, y); // Left
  flood(image, x + 1, y); // Right
  flood(image, x, y - 1); // Up
  flood(image, x, y + 1); // Down
}

int main(int argc, char *argv[]) {
  _Bool stalactites           = 0;
  _Bool horizontal_projection = 0;
  _Bool vertical_projection   = 0;
  _Bool best_compression      = 0;

  int opt;

  while ((opt = getopt(argc, argv, "w:h:l:sHVz")) != -1)
    switch (opt) {
      case 'w': width = strtoul(optarg, 0, 10);
        break;

      case 'h': height = strtoul(optarg, 0, 10);
        break;

      case 'l': width = height = strtoul(optarg, 0, 10);
        break;

      case 's': stalactites = 1;
        break;

      case 'H': horizontal_projection = 1;
        break;

      case 'V': vertical_projection = 1;
        break;

      case 'z': best_compression = 1;
        break;

      default: return 1;
    }

  unsigned long area = width * height;

  if (!area || width > MAX_LENGTH || height > MAX_LENGTH)
    return 2;

  png_struct *structp = png_create_write_struct(PNG_LIBPNG_VER_STRING, 0, 0, 0);
  png_info *infop = png_create_info_struct(structp);

  if (!(structp && infop))
    return 3;

  // The longest filename is
  // w10000h10000s.xy.png
  char filename[21];
  sprintf(filename, "w%uh%u%s.xy.png",
    width, height, stalactites ? "s" : horizontal_projection ? "H" : vertical_projection ? "V" : "");

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
  png_set_IHDR(structp, infop, width, height, 1, PNG_COLOR_TYPE_GRAY, PNG_INTERLACE_NONE,
    PNG_COMPRESSION_TYPE_BASE, PNG_FILTER_TYPE_BASE);

  png_byte *image = malloc(area * sizeof *image);

  if (!image) {
    remove(filename);
    return 6;
  }

  unsigned shorter = width < height ? width : height;
  unsigned longer  = width > height ? width : height;

  for (unsigned long i = 0; i < area; ++i)
    image[i] = BLACK;

  for (unsigned i = 0; i < shorter; ++i)
    for (unsigned j = i; j < longer; ++j)
      image[i * j] = WHITE;

  if (stalactites) {
    flood(image, 0, 0);

    for (unsigned long i = 0; i < area; ++i)
      if (image[i] == WHITE)
        image[i] = BLACK;
  }
  else if (horizontal_projection)
    for (unsigned x = 0; x < width; ++x) {
      unsigned colored = 0;

      for (unsigned y = 0; y < height; ++y)
        if (image[y * width + x] == WHITE)
          image[colored++ * width + x] = WHITE;

      for (unsigned y = colored; y < height; ++y)
        image[y * width + x] = BLACK;
    }
  else if (vertical_projection)
    for (unsigned y = 0; y < height; ++y) {
      unsigned colored = 0;

      for (unsigned x = 0; x < width; ++x)
        if (image[y * width + x] == WHITE)
          image[y * width + colored++] = WHITE;

      for (unsigned x = colored; x < width; ++x)
        image[y * width + x] = BLACK;
    }

  png_init_io(structp, file);
  png_write_info(structp, infop);

  for (unsigned y = 0; y < height; ++y) {
    for (unsigned x = 0; x < width; ++x) {
      png_byte pixel = image[y * width + x];

      if (!(x % 8))
        image[x / 8] = 0;

      if (pixel)
        image[x / 8] |= 1 << (7 - x % 8);
    }

    png_write_row(structp, image);
  }

  png_write_end(structp, 0);
  png_destroy_write_struct(&structp, &infop);
  free(image);
  fclose(file);
  printf("Wrote %s\n", filename);
}
