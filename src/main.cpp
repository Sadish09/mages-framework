#include <Arduino.h>

#include "config/sensors_config.h"
#include "config/ewma_params.h"
#include "config/cusum_params.h"
#include "config/safety_limits.h"

#include "core/system_state.h"

#include "utils/ewma.h"
#include "utils/cusum.h"
#include "utils/ring_buffer.h"
#include "utils/stats.h"

#include "env/compensation.h"

#include "ui/display.h"
#include "audio/buzzer.h"

#include "sensors/mq_common.h"
#include <DHT.h>

#include "sensors/mq_common.h"

//good ol pin defs

#define MQ6_PIN        A0
#define MQ135_PIN      A1
#define FLAME_PIN      7

#define DHT_PIN        6
#define DHT_TYPE       DHT11

//Hardware thingamajings

DHT dht(DHT_PIN, DHT_TYPE);

//Filters & detectors

EWMA ewma_mq6(EWMA_ALPHA_MQ_SLOW);
CUSUM cusum_mq6(CUSUM_K_MQ, CUSUM_H_MQ);
SampleRingBuffer<16> mq6_window;

EWMA ewma_mq135(EWMA_ALPHA_MQ_SLOW);
CUSUM cusum_mq135(CUSUM_K_MQ, CUSUM_H_MQ);
SampleRingBuffer<16> mq135_window;

// Configure Global system state here

SystemState system_state;


void setup() {
    Serial.begin(115200);

    pinMode(FLAME_PIN, INPUT);

    dht.begin();
    display_init();

    system_state = {
        .overall_severity = NORMAL,
        .gas_warning = false,
        .flame_detected = false,
        .temp_warning = false
    };
}


void loop() {
    //Temp & Humidity

    float temperature_c = dht.readTemperature();
    float humidity = dht.readHumidity();

    bool env_valid = !(isnan(temperature_c) || isnan(humidity));

    //Flame

    bool flame_detected = (digitalRead(FLAME_PIN) == HIGH);
    system_state.flame_detected = flame_detected;

    //MQ-6 for LPG

    int mq6_adc = analogRead(MQ6_PIN);
    float mq6_rs = mq_compute_rs(mq6_adc);
    float mq6_ratio = mq_compute_ratio(mq6_rs, 10.0f);
    float mq6_ppm = mq_ratio_to_ppm(mq6_ratio, 6);

    if (env_valid) {
        mq6_ppm = compensate_ppm(mq6_ppm, temperature_c, humidity);
    }

    float mq6_filtered = ewma_mq6.update(mq6_ppm);
    bool mq6_cusum = cusum_mq6.update(mq6_filtered);
    mq6_window.push(mq6_filtered);

    //MQ135 for VOC

    int mq135_adc = analogRead(MQ135_PIN);
    float mq135_rs = mq_compute_rs(mq135_adc);
    float mq135_ratio = mq_compute_ratio(mq135_rs, 10.0f);
    float mq135_ppm = mq_ratio_to_ppm(mq135_ratio, 135);

    if (env_valid) {
        mq135_ppm = compensate_ppm(mq135_ppm, temperature_c, humidity);
    }

    float mq135_filtered = ewma_mq135.update(mq135_ppm);
    bool mq135_cusum = cusum_mq135.update(mq135_filtered);
    mq135_window.push(mq135_filtered);


    system_state.overall_severity = NORMAL;
    system_state.gas_warning = false;
    system_state.temp_warning = false;

    //high LPG
    if (mq6_cusum && mq6_filtered > LPG_SAFE_MAX_PPM) {
        system_state.gas_warning = true;
        system_state.overall_severity = WARNING;
    }

    if (mq6_filtered > LPG_SAFE_MAX_PPM * 1.5f) {
        system_state.overall_severity = CRITICAL;
    }

    //high VOC
    if (mq135_cusum && mq135_filtered > VOC_SAFE_MAX_PPM) {
        system_state.overall_severity = max(system_state.overall_severity, WARNING);
    }

    //Overtemp
    if (env_valid && temperature_c > TEMP_WARNING_C) {
        system_state.temp_warning = true;
        system_state.overall_severity = max(system_state.overall_severity, WARNING);
    }

    if (env_valid && temperature_c > TEMP_CRITICAL_C) {
        system_state.overall_severity = CRITICAL;
    }

    //mkb aag
    if (flame_detected && system_state.overall_severity >= WARNING) {
        system_state.overall_severity = CRITICAL;
    }


    display_render(system_state);
    buzzer_signal(system_state.overall_severity);

    // Debug if display dies or smth

    Serial.print("MQ6: "); Serial.print(mq6_filtered);
    Serial.print(" | MQ135: "); Serial.print(mq135_filtered);
    Serial.print(" | T: "); Serial.print(temperature_c);
    Serial.print(" | H: "); Serial.print(humidity);
    Serial.print(" | FLAME: "); Serial.println(flame_detected);

    delay(1000);
}
