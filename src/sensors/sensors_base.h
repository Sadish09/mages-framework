#pragma once

struct SensorReading {
    float value;
    bool valid;
};

class Sensor {
public:
    virtual void update() = 0;
    virtual SensorReading read() = 0;
    virtual const char* name() = 0;
    virtual ~Sensor() = default;
};
