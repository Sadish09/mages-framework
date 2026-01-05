#pragma once
#include "anomaly/anomaly_engine.h"

struct SystemState {
    Severity overall_severity;
    bool gas_warning;
    bool flame_detected;
    bool temp_warning;
};
