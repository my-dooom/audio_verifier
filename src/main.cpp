#include <stdio.h>

#include "include\generator.h"
#include "include\getopt.h"

void print_usage(const char *prog_name) {
  printf("Usage: %s [options] <input_file> <output_file>\n", prog_name);
  printf("Options:\n");
  printf("  -h, --help    Show this help message\n");
  printf("  -v, --verbose Enable verbose output\n");
}

int main(int argc, char *argv[]) {
  int opt;
  while ((opt = getopt(argc, argv, "hv")) != -1) {
    switch (opt) {
    case 'h':
      print_usage(argv[0]);
      return 0;
    default:
      print_usage(argv[0]);
      return 1;
    }
  }
  // generate_wav("sinusoid.wav", FREQUENCY, AMPLITUDE, DURATION);
  return 0;
}
