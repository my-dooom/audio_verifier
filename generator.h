#include <stdint.h>
#include <stdlib.h>
#include <string.h>

#define PI 3.141592653589793
#define SAMPLE_RATE 44100
#define FREQUENCY 440.0 // A4 note
#define AMPLITUDE 1.0
#define DURATION 1.0 // seconds

typedef struct
{
    char riff[4];
    uint32_t file_size;
    char wave[4];
    char fmt[4];
    uint32_t fmt_size;
    uint16_t audio_format;
    uint16_t num_channels;
    uint32_t sample_rate;
    uint32_t byte_rate;
    uint16_t block_align;
    uint16_t bits_per_sample;
    char data[4];
    uint32_t data_size;
} wav_header;

void generate_wav(const char *filename, double frequency, double amplitude, double duration);