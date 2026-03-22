#include <stdio.h>
#include <math.h>

#include "include\generator.h"

 void generate_wav(const char* filename, double frequency, double amplitude, double duration){
    int num_samples = SAMPLE_RATE * duration;
    int16_t* samples = malloc(num_samples * sizeof(int16_t));
    if (!samples) {
        fprintf(stderr, "Memory allocation failed\n");
        return;
    }

    double dt = 1.0 / SAMPLE_RATE;
    for (int i = 0; i < num_samples; i++) {
        double t = i * dt;
        double sample = amplitude * sin(2 * PI * frequency * t);
        samples[i] = (int16_t)(sample * 32767.0);
    }

    wav_header header;
    memcpy(header.riff, "RIFF", 4);
    header.file_size = 36 + num_samples * sizeof(int16_t);
    memcpy(header.wave, "WAVE", 4);
    memcpy(header.fmt, "fmt ", 4);
    header.fmt_size = 16;
    header.audio_format = 1; // PCM
    header.num_channels = 1;
    header.sample_rate = SAMPLE_RATE;
    header.byte_rate = SAMPLE_RATE * 1 * 16 / 8;
    header.block_align = 1 * 16 / 8;
    header.bits_per_sample = 16;
    memcpy(header.data, "data", 4);
    header.data_size = num_samples * sizeof(int16_t);

    FILE* file = fopen(filename, "wb");
    if (!file) {
        fprintf(stderr, "Failed to open file\n");
        free(samples);
        return;
    }

    fwrite(&header, sizeof(wav_header), 1, file);
    fwrite(samples, sizeof(int16_t), num_samples, file);

    fclose(file);
    free(samples);
}