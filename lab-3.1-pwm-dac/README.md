# 🎛️ Lab 3.1 – PWM DAC Waveform Generator

Replaces the external R-2R resistor ladder from Lab 2.1 by using the MSP432's internal **TimerA PWM generator**. The high-frequency switching output is passed through an RC low-pass filter to smooth and reconstruct analog sawtooth and sine waveforms. 

## 🚀 Lab Overview

* **PWM Reconstruction:** Converts digital pulse-width modulation into true analog signals using basic hardware filtering.
* **Dynamic Duty Cycle:** Uses background timer interrupts to update the Capture/Compare registers on-the-fly.
* **Standalone Exercises:** Independent source files exploring fixed duty cycles, sawtooth generation, and table-driven sine generation.

---

## 🔬 Hardware Setup & Wiring

### Pin Assignments

| MSP432 Port | Connected Hardware | Description |
| :--- | :--- | :--- |
| **`P2.4`** | TA0.1 PWM Output | Connects to the oscilloscope channel and the input of the RC filter |
| **`Filter Output`** | Oscilloscope Channel 2 | Cleaned analog signal monitored alongside the raw PWM wave |

---

## ⚙️ Program Breakdown

Each file below is an independent application that must be compiled and flashed separately.

| Part | Source File | Type | Notes |
| :--- | :--- | :--- | :--- |
| **A** | [`main_A_pwm_33pct.c`](./src/main_A_pwm_33pct.c) | Hardware PWM | Configures TimerA_0 to run continuously at a fixed 33% duty cycle. Used for baseline scope calibration. |
| **B** | [`main_B_pwm_sawtooth.c`](./src/main_B_pwm_sawtooth.c) | ISR-driven | Generates a sawtooth envelope. A Timer32 interrupt triggers every 5 ms to incrementally ramp the TimerA CCR1 register from 0 to 255. |
| **C** | [`main_C_pwm_sine.c`](./src/main_C_pwm_sine.c) | ISR-driven | Generates a sine wave. The same 5 ms Timer32 interrupt streams the 16-entry lookup table values directly into the CCR1 register. |
| **D** | *(Hardware only)* | Analog Filter | A RC filter (R = 10 kΩ, C = 0.1 μF) used to strip away the high-frequency PWM switching carrier, leaving behind the smooth analog wave. |

---

## 📐 Timing Calculations

All exercises configure the sub-master clock (SMCLK) to run at **3 MHz**.

### Part A: Fixed PWM
* **Period Register (CCR0):** 15,000
* **Duty Cycle Register (CCR1):** 4,950
* **PWM Frequency:** 3,000,000 Hz / 15,000 = **200 Hz**
* **Duty Cycle:** 4,950 / 15,000 = **33%**

### Part B & C: PWM Carrier & Interrupt Pacing
The PWM module uses a /2 clock prescaler and a period length of 256 clock counts (CCR0 = 255).
* **PWM Carrier Frequency:** 3,000,000 Hz / 2 / 256 = **5859 Hz**

The pacing clock (Timer32_0) uses a period register value of 15,000 to trigger an update interrupt every 5 ms:
* **ISR Update Rate:** 3,000,000 Hz / 15,000 = **200 Hz**

Waveform Generation Outputs:
* **Part B Sawtooth Wave:** The duty cycle increments by 1 step every ISR tick and wraps around every 256 counts.
  * Sawtooth Frequency = 200 Hz ISR / 256 steps = **0.78 Hz**
* **Part C Sine Wave:** The duty cycle streams values out of the 16-entry sine table every ISR tick.
  * Sine Wave Frequency = 200 Hz ISR / 16 steps = **12.5 Hz**

---


## 📁 Files
* [`src/main_A_pwm_33pct.c`](./src/main_A_pwm_33pct.c)
* [`src/main_B_pwm_sawtooth.c`](./src/main_B_pwm_sawtooth.c)
* [`src/main_C_pwm_sine.c`](./src/main_C_pwm_sine.c)
