#pragma once

enum Severity {
    NORMAL,
    ELEVATED,
    WARNING,
    CRITICAL
};

struct Alert {
    Severity severity;
    float metric;
};

Alert evaluate_anomaly(float value,
                       float cusum_score,
                       float safety_limit);
