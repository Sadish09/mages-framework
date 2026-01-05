#pragma once

inline float clamp(float x, float min_val, float max_val) {
    if (x < min_val) return min_val;
    if (x > max_val) return max_val;
    return x;
}

inline float absf(float x) {
    return (x < 0.0f) ? -x : x;
}
