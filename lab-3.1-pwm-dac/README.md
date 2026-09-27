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

\[\text{PWM Frequency} = \frac{3,000,000\text{ Hz}}{15,000} = 200\text{ Hz}\]

\[\text{Duty Cycle} = \frac{4,950}{15,000} = 33\%\]

### Part B & C: PWM Carrier & Interrupt Pacing
The PWM module is configured with a \(/2\) clock prescaler and a period length of 256 clock counts (\(\text{CCR0} = 255\)). 

\[\text{PWM Carrier Frequency} = \frac{3,000,000\text{ Hz}}{2 \times 256} \approx 5859\text{ Hz}\]

The pacing clock (**Timer32_0**) uses a period register value of 15,000 to trigger an update interrupt every 5 ms:

\[\text{ISR Update Rate} = \frac{3,000,000\text{ Hz}}{15,000} = 200\text{ Hz}\]

* **Part B Sawtooth Wave:** The duty cycle increments by 1 step every ISR tick and wraps around every 256 counts.
\[\text{Sawtooth Frequency} = \frac{200\text{ Hz ISR Rate}}{256\text{ steps}} \approx 0.78\text{ Hz}\]

* **Part C Sine Wave:** The duty cycle streams values out of the 16-entry sine table every ISR tick.
\[\text{Sine Wave Frequency} = \frac{200\text{ Hz ISR Rate}}{16\text{ steps}} = 12.5\text{ Hz}\]

---

## 📁 Files
* [`src/main_A_pwm_33pct.c`](./src/main_A_pwm_33pct.c)
* [`src/main_B_pwm_sawtooth.c`](./src/main_B_pwm_sawtooth.c)
* [`src/main_C_pwm_sine.c`](./src/main_C_pwm_sine.c)
