#include <vector>
#include <complex>
#include <fftw3.h>  // Or kissfft
#include <cmath>
#include <cstdint>

extern "C" {
    // C-callable function: Analyze samples, return dominant freq
    double analyze_fft(const int16_t* samples, size_t num_samples, uint32_t sample_rate) {
        // Convert to double
        std::vector<double> input(num_samples);
        for (size_t i = 0; i < num_samples; ++i) {
            input[i] = samples[i] / 32767.0;
        }

        // FFT (using fftw)
        fftw_complex* out = (fftw_complex*) fftw_malloc(sizeof(fftw_complex) * num_samples);
        fftw_plan plan = fftw_plan_dft_r2c_1d(num_samples, input.data(), out, FFTW_ES`TIMATE);
        fftw_execute(plan);

        // Find peak frequency
        double max_magnitude = 0;
        size_t peak_index = 0;
        for (size_t i = 1; i < num_samples / 2; ++i) {  // Ignore DC and Nyquist
            double mag = std::abs(std::complex<double>(out[i][0], out[i][1]));
            if (mag > max_magnitude) {
                max_magnitude = mag;
                peak_index = i;
            }
        }

        double dominant_freq = (double)peak_index * sample_rate / num_samples;

        fftw_destroy_plan(plan);
        fftw_free(out);
        return dominant_freq;
    }
}