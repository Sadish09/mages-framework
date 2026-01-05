#pragma once

struct CUSUM {
    float k;        // reference value
    float h;        // threshold
    float s_pos;    // positive sum

    CUSUM(float k_ref, float h_ref)
        : k(k_ref), h(h_ref), s_pos(0.0f) {}

    bool update(float x) {
        s_pos = (s_pos + x - k);
        if (s_pos < 0.0f) {
            s_pos = 0.0f;
        }
        return (s_pos > h);
    }

    void reset() {
        s_pos = 0.0f;
    }
};
