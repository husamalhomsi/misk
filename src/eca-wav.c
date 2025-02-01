// eca-wav.c - write an elementary cellular automaton as audio to a WAVE file

#include <getopt.h>
#include <inttypes.h>
#include <limits.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "eca.h"
#include "wafer.h"

#define AMP 127
#define P2P 254
#define MAX 255
#define MID 128
#define MIN 1

int main(int argc, char *argv[]) {
  uint8_t  rule       = 18;
  uint32_t seed       = 0;
  uint32_t cell_count = 256;
  uint32_t gen_count  = 128;

  int opt;

  while ((opt = getopt(argc, argv, "r:s:c:g:")) != -1)
    switch (opt) {
      case 'r': rule = strtoul(optarg, 0, 10);
        break;

      case 's': seed = strtoul(optarg, 0, 10);
        break;

      case 'c': cell_count = strtoul(optarg, 0, 10);
        break;

      case 'g': gen_count = strtoul(optarg, 0, 10);
        break;

      default: return 1;
    }

  size_t sample_count = gen_count * cell_count;

  if (!sample_count)
    return 2;

  uint8_t *audio = malloc(sample_count * sizeof *audio);
  _Bool *cells = malloc(cell_count * sizeof *cells);
  _Bool *accumulators = malloc(cell_count * sizeof *accumulators);

  if (!(audio && cells && accumulators))
    return 3;

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
      audio[g * cell_count + c] = cells[c];

      accumulators[c] = eca_rule(rule,
        cells[c == 0 ? last_cell_position : c - 1],
        cells[c],
        cells[c == last_cell_position ? 0 : c + 1]);
    }

    memcpy(cells, accumulators, cell_count);
  }

  free(cells);
  free(accumulators);

  _Bool no_alive_cells = 1;
  size_t first_alive_cell_position, last_alive_cell_position, i;

  // Search for the first alive cell
  for (i = 0; i < sample_count; ++i)
    if (audio[i]) {
      no_alive_cells = 0;
      first_alive_cell_position = i;
      break;
    }

  // Silence
  if (no_alive_cells)
    for (i = 0; i < sample_count; ++i)
      audio[i] = MID;
  // Linear interpolation
  else {
    i = sample_count;

    // Search for the last alive cell
    while (i--)
      if (audio[i]) {
        last_alive_cell_position = i;
        break;
      }

    // First line
    if (first_alive_cell_position)
      for (i = 0; i < first_alive_cell_position; ++i)
        audio[i] = MID + i * AMP / first_alive_cell_position;

    _Bool up = 0;
    i = first_alive_cell_position;

    // Mid lines
    while (i < last_alive_cell_position) {
      size_t from = i;

      while (!audio[++i]);

      size_t span = i - from;

      for (size_t j = 0; j < span; ++j)
        audio[from + j] = up ?
          MIN + j * P2P / span :
          MAX - j * P2P / span;

      up = !up;
    }

    size_t span = sample_count - last_alive_cell_position;

    // Last line
    for (i = 0; i < span; ++i)
      audio[last_alive_cell_position + i] = up ?
        MIN + i * AMP / span :
        MAX - i * AMP / span;
  }

  // The longest filename is
  // r100s1000000000c1000000000g1000000000.eca.wav
  char filename[46];
  sprintf(filename, "r%" PRIu8 "s%" PRIu32 "c%" PRIu32 "g%" PRIu32 ".eca.wav",
    rule, seed, cell_count, gen_count);

  wafer_file *file = wafer_open(filename);

  if (!file)
    return 4;

  wafer_set_channels(file, 1);
  wafer_set_samples_per_sec(file, 44100);

  if (!(wafer_write_metadata(file) &&
        wafer_write_data(audio, sample_count, file) &&
        wafer_close(file))) {
    remove(filename);
    return 5;
  }

  free(audio);
  printf("Wrote %s\n", filename);
}
