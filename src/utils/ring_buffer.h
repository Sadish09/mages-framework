#pragma once

template<int N>
struct SampleRingBuffer {
    float data[N];
    int index = 0;
    bool full = false;

    void push(float x) {
        data[index++] = x;
        if (index >= N) {
            index = 0;
            full = true;
        }
    }

    int size() const {
        return full ? N : index;
    }
};
