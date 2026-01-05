#pragma once

struct MovingAverage {
    float sum = 0.0f;
    int count = 0;

    float update(float x) {
        sum += x;
        count++;
        return sum / count;
    }
};
