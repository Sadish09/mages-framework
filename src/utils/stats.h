#pragma once
#include "ring_buffer.h"

inline float compute_mean(const float* data, int n) {
    float sum = 0.0f;
    for (int i = 0; i < n; i++) {
        sum += data[i];
    }
    return sum / n;
}

inline float compute_variance(const float* data, int n) {
    float mean = compute_mean(data, n);
    float var = 0.0f;

    for (int i = 0; i < n; i++) {
        float d = data[i] - mean;
        var += d * d;
    }
    return var / n;
}

inline float compute_slope(const float* data, int n) {
    if (n < 2) return 0.0f;
    return data[n - 1] - data[0];
}
