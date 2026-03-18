#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <math.h>

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

static void print_usage(const char *prog) {
    fprintf(stderr, "Usage: %s <file.wav> [frequency]", prog);
    fprintf(stderr, "  frequency: expected sine frequency (Hz), defaults to 440.0\n");
}

int main(int argc, char **argv) {
    if (argc < 2) {
        print_usage(argv[0]);
        return 1;
    }

    const char *path = argv[1];
    double expected_frequency = 440.0;
    if (argc >= 3) {
        expected_frequency = atof(argv[2]);
        if (expected_frequency <= 0) {
            fprintf(stderr, "Invalid frequency: %s\n", argv[2]);
            return 2;
        }
    }

    FILE *f = fopen(path, "rb");
    if (!f) {
        perror("fopen");
        return 3;
    }

    wav_header h;
    if (fread(&h, sizeof(wav_header), 1, f) != 1) {
        fprintf(stderr, "Failed to read WAV header\n");
        fclose(f);
        return 4;
    }

    if (memcmp(h.riff, "RIFF", 4) != 0 || memcmp(h.wave, "WAVE", 4) != 0 ||
        memcmp(h.fmt, "fmt ", 4) != 0 || memcmp(h.data, "data", 4) != 0) {
        fprintf(stderr, "File is not a supported WAV file\n");
        fclose(f);
        return 5;
    }

    if (h.audio_format != 1 || h.bits_per_sample != 16) {
        fprintf(stderr, "Only 16-bit PCM WAV is supported (audio_format=%u, bits=%u)\n",
                h.audio_format, h.bits_per_sample);
        fclose(f);
        return 6;
    }

    size_t num_samples = h.data_size / (h.bits_per_sample / 8);
    if (num_samples == 0) {
        fprintf(stderr, "No audio samples found\n");
        fclose(f);
        return 7;
    }

    int16_t *samples = malloc(num_samples * sizeof(int16_t));
    if (!samples) {
        fprintf(stderr, "Memory allocation failed\n");
        fclose(f);
        return 8;
    }

    if (fread(samples, sizeof(int16_t), num_samples, f) != num_samples) {
        fprintf(stderr, "Failed to read sample data\n");
        free(samples);
        fclose(f);
        return 9;
    }

    fclose(f);

    double peak = 0.0;
    double sum_sq = 0.0;
    double sum_sq_error = 0.0;

    double dt = 1.0 / h.sample_rate;
    for (size_t i = 0; i < num_samples; i++) {
        double sample = samples[i] / 32767.0;
        double expected = sin(2.0 * M_PI * expected_frequency * (i * dt));

        double abs_sample = fabs(sample);
        if (abs_sample > peak) {
            peak = abs_sample;
        }

        sum_sq += sample * sample;
        double err = sample - expected;
        sum_sq_error += err * err;
    }

    free(samples);

    double rms = sqrt(sum_sq / num_samples);
    double rms_error = sqrt(sum_sq_error / num_samples);
    double snr_db = 20.0 * log10((rms > 0 ? rms : 1e-12) / (rms_error > 0 ? rms_error : 1e-12));

    double duration = (double)num_samples / h.sample_rate;

    printf("WAV analysis: %s\n", path);
    printf("  sample rate: %u Hz\n", h.sample_rate);
    printf("  channels: %u\n", h.num_channels);
    printf("  duration: %.3f s\n", duration);
    printf("  samples: %zu\n", num_samples);
    printf("  peak amplitude: %.6f\n", peak);
    printf("  RMS: %.6f\n", rms);
    printf("  expected sine: %.3f Hz\n", expected_frequency);
    printf("  estimated SNR vs expected sine: %.2f dB\n", snr_db);

    return 0;
}
