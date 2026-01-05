#pragma once

struct EWMA {
    float alpha;
    float value;
    bool initialized;

    EWMA(float a)
        : alpha(a), value(0.0f), initialized(false) {}

    float update(float x) {
        if (!initialized) {
            value = x;
            initialized = true;
        } else {
            value = alpha * x + (1.0f - alpha) * value;
        }
        return value;
    }
};
