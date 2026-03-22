#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <math.h>

/* wav header layout based on riff wave spec
   basically first 44 bytes of a simple pcm wav */
typedef struct
{
    char riff[4];          // should be riff
    uint32_t file_size;    // file size minus 8 bytes
    char wave[4];          // should be wave
    char fmt[4];           // fmt chunk
    uint32_t fmt_size;     // usually 16 for pcm
    uint16_t audio_format; // 1 means pcm
    uint16_t num_channels; // 1 mono 2 stereo
    uint32_t sample_rate;  // samples per second
    uint32_t byte_rate;    // bytes per second
    uint16_t block_align;  // bytes per frame
    uint16_t bits_per_sample; // bits per sample usually 16
    char data[4];          // data chunk id
    uint32_t data_size;    // size of raw audio data
} wav_header;

/* print usage when args are wrong */
static void print_usage(const char *prog) {
    fprintf(stderr, "Usage: %s <file.wav> [frequency]\n", prog);
    fprintf(stderr, "  frequency: expected sine frequency (Hz), defaults to 440.0\n");
}

int main(int argc, char **argv) {

    // need at least file path
    if (argc < 2) {
        print_usage(argv[0]);
        return 1;
    }

    // input file path
    const char *path = argv[1];

    // default frequency if not provided
    double expected_frequency = 440.0;

    // optional frequency override
    if (argc >= 3) {
        expected_frequency = atof(argv[2]);

        // reject bad values
        if (expected_frequency <= 0) {
            fprintf(stderr, "Invalid frequency: %s\n", argv[2]);
            return 2;
        }
    }

    // open file in binary mode
    FILE *f = fopen(path, "rb");
    if (!f) {
        perror("fopen");
        return 3;
    }

    // read header
    wav_header h;
    if (fread(&h, sizeof(wav_header), 1, f) != 1) {
        fprintf(stderr, "Failed to read WAV header\n");
        fclose(f);
        return 4;
    }

    // basic validation of wav structure
    // just checking magic strings not bulletproof but enough
    if (memcmp(h.riff, "RIFF", 4) != 0 || memcmp(h.wave, "WAVE", 4) != 0 ||
        memcmp(h.fmt, "fmt ", 4) != 0 || memcmp(h.data, "data", 4) != 0) {
        fprintf(stderr, "File is not a supported WAV file\n");
        fclose(f);
        return 5;
    }

    // only support simple 16 bit pcm
    if (h.audio_format != 1 || h.bits_per_sample != 16) {
        fprintf(stderr, "Only 16-bit PCM WAV is supported (audio_format=%u, bits=%u)\n",
                h.audio_format, h.bits_per_sample);
        fclose(f);
        return 6;
    }

    // total number of samples across all channels
    size_t num_samples = h.data_size / (h.bits_per_sample / 8);

    // nothing to process
    if (num_samples == 0) {
        fprintf(stderr, "No audio samples found\n");
        fclose(f);
        return 7;
    }

    // allocate buffer for samples
    int16_t *samples = malloc(num_samples * sizeof(int16_t));
    if (!samples) {
        fprintf(stderr, "Memory allocation failed\n");
        fclose(f);
        return 8;
    }

    // read raw sample data
    if (fread(samples, sizeof(int16_t), num_samples, f) != num_samples) {
        fprintf(stderr, "Failed to read sample data\n");
        free(samples);
        fclose(f);
        return 9;
    }

    fclose(f);

    // peak amplitude tracking
    double peak = 0.0;

    // sum of squares for rms
    double sum_sq = 0.0;

    // sum of squared error vs expected sine
    double sum_sq_error = 0.0;

    // time step per sample
    double dt = 1.0 / h.sample_rate;

    // iterate over all samples
    for (size_t i = 0; i < num_samples; i++) {

        // normalize from int16 to -1 1 range
        double sample = samples[i] / 32767.0;

        // ideal sine value at this sample index
        double expected = sin(2.0 * 3.141592653589793 * expected_frequency * (i * dt));

        // absolute value for peak detection
        double abs_sample = fabs(sample);
        if (abs_sample > peak) {
            peak = abs_sample;
        }

        // accumulate energy
        sum_sq += sample * sample;

        // error vs ideal sine
        double err = sample - expected;
        sum_sq_error += err * err;
    }

    free(samples);

    // rms amplitude of signal
    double rms = sqrt(sum_sq / num_samples);

    // rms of error signal
    double rms_error = sqrt(sum_sq_error / num_samples);

    // snr in db using rms ratio
    double snr_db = 20.0 * log10((rms > 0 ? rms : 1e-12) / (rms_error > 0 ? rms_error : 1e-12));

    // duration in seconds
    double duration = (double)num_samples / h.sample_rate;

    // print results
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