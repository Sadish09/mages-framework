---

# Gas & Environmental Anomaly Detection System

This project implements a **microcontroller-based gas and environmental monitoring system** using commonly available MQ-series gas sensors, a temperature–humidity sensor, and a flame detector.
The system is designed to operate reliably in real-world conditions where sensors drift, warm up slowly, and rarely behave exactly like their datasheets suggest.

The primary goal is **early detection of abnormal environmental conditions**, like a T0 industrial system.

---

## What this system does

* Monitors **LPG / combustible gases** using MQ-6
* Monitors **general VOC activity** using MQ-135
* Tracks **ambient temperature and humidity** we used DHT11, good enough for just beta correction
* Detects **open flame presence** via a digital flame sensor; a multichannel one would be great
* Identifies **abnormal changes over time**, not just raw thresholds
* Escalates conditions into **Normal → Warning → Critical** states
* Provides clear **visual and audible alerts**

The system is intended to be **continuously powered**, left running for long periods, and expected to tolerate sensor drift and noisy environments.

---

## Design philosophy

This is not a “ppm meter”.

MQ sensors are inherently non-linear, humidity-sensitive, and slow to stabilize. Treating them as precision instruments is a mistake. Instead, this system treats them as **trend and change indicators**, which is what they are actually good at.

To that end:

* **EWMA (Exponentially Weighted Moving Average)** is used to suppress noise without killing response time
* **CUSUM change detection** is used to detect statistically meaningful deviations from normal behavior
* Fixed limits are used only to **interpret risk and severity**, not to decide whether something changed
* Flame detection is treated as a **contextual escalator**, not a standalone alarm source

If a sensor glitches, spikes, or drifts slowly, the system should *notice*, but it should not panic.

---

## Architecture overview

The firmware follows a straightforward, deterministic pipeline:

```
Sensor Read
  → Environmental Compensation
  → EWMA Filtering
  → CUSUM Change Detection
  → Temporal Analysis
  → Severity Classification
  → Display & Buzzer Output
```

Each stage is intentionally simple and debuggable.

---

## Supported hardware

* **MQ-6** – LPG / butane / propane detection
* **MQ-135** – VOC / air quality trend detection
* **DHT11** – Temperature and humidity (used for compensation, not precision)
* **Digital flame sensor** – Direct flame presence input
* **OLED display (SSD1306 class)** – Status and alerts
* **Active buzzer** – Audible warning signaling

Tested with Arduino-class boards (including newer Cortex-M based variants). Porting to STM32 or ESP platforms is straightforward.

---

## Why EWMA + CUSUM?

Gas leaks, overheating, and combustion events don’t usually announce themselves as clean step functions. They show up as **persistent deviation**, **rate-of-rise**, or **combined weak signals across sensors**.

EWMA handles the noise.
CUSUM answers the only question that really matters:

> “Has the behavior changed enough that I should care?”

This approach works even when:

* Only one gas sensor is present
* Baseline drifts slowly over time
* Environmental conditions are changing normally

---

## Safety limits and severity

Absolute limits still exist, but they are used **after** change detection to decide *how bad* a situation is, not *whether something happened*.

Severity levels:

* **NORMAL** – expected environmental behavior
* **WARNING** – sustained abnormal condition
* **CRITICAL** – unsafe condition or multi-sensor confirmation

Flame detection combined with elevated gas or temperature always escalates to **CRITICAL**. That is intentional.

---

## Notes from the hardware side

* MQ sensors need warm-up time. Don’t trust anything immediately after power-on.
* R0 calibration is treated as a baseline, not a constant of nature.
* Humidity compensation matters more than most people think.
* If your flame sensor chatters, fix it in hardware first. Software can only do so much.
* If you’re testing with a lighter, you are not simulating a real leak if you want accurate simulatons write your own matlab scripts

---

## Intended use

* Embedded safety prototypes
* Environmental monitoring projects
* Research demonstrations
* Educational systems that behave like real hardware

Not intended to replace certified safety equipment.

---

## Final note

This system is designed the way you’d design something you expect to leave powered on, unattended, and slightly abused.
If it’s quiet, it should stay quiet.
If something changes, it should tell you **clearly** — and only when it actually matters.

---


