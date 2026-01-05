#pragma once

struct MQCalibration {
    float R0;
    bool calibrated;
};

float mq_compute_rs(int adc_value);
float mq_compute_ratio(float rs, float r0);
float mq_ratio_to_ppm(float ratio, int gas_curve_id);
