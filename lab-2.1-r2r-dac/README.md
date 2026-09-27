# 🎛️ Lab 2.1 – R-2R DAC Waveform Generator

A collection of standalone programs that explore analog waveform generation using an **8-bit R-2R resistor ladder** driven from Port 4 of the MSP432P401R. 
Each file is an independent exercise, culminating in a final program that uses onboard switches to dynamically control a sine wave's frequency and amplitude.

## 🚀 Lab Overview

* **Standalone Exercises:** Separate source files cover individual waveform types (sawtooth, triangle, or sine).
* **8-bit discrete DAC:** Built manually using an external resistor ladder.
* **Live Switch Control:** Features live frequency and amplitude scaling (implemented only in the final sine wave program, Part E).

---

## 🔬 Hardware Setup & Wiring

### Pin Assignments

| MSP432 Port | Connected Hardware | Description |
| :--- | :--- | :--- |
| **`P4.0 – P4.7`** | R-2R Resistor Ladder | Inputs for the 8-bit digital value |
| **`DAC-VOUT`** | Oscilloscope / Filter Input | The combined analog output from the ladder |
| **`P1.1`** | Onboard Switch (S1) | Frequency selection (Used in Part E only) |
| **`P1.4`** | Onboard Switch (S2) | Amplitude selection (Used in Part E only) |

---

## ⚙️ Program Breakdown

Each file below is an independent application that must be compiled and flashed separately.

| Part | Source File | Type | Notes |
| :--- | :--- | :--- | :--- |
| **A / B** | [`main_A_B_sawtooth_triangle.c`](./src/main_A_B_sawtooth_triangle.c) | Free-running | Generates either a sawtooth or a triangle wave using a software counter. Choose the wave type at compile time via `#define SAWTOOTH` or `#define TRIANGLE`. |
| **C** | [`main_C_sine_lookup.c`](./src/main_C_sine_lookup.c) | Free-running | Steps through a 16-entry lookup table inside the main loop to build a rough sine wave. |
| **D** | [`main_D_sine_timer32_isr.c`](./src/main_D_sine_timer32_isr.c) | ISR-driven | Uses the same 16-entry table, but introduces a Timer32_0 interrupt to clock out steps at a precise, stable 100 Hz rate. |
| **E** | [`main_E_sine_switch_control.c`](./src/main_E_sine_switch_control.c) | ISR-driven | Expands on Part D by adding live control via onboard switches. S1 toggles the frequency (100 Hz / 50 Hz) and S2 toggles the amplitude (100% / 50%). |
| **F** | *(Hardware only)* | Analog Filter | A single-pole RC filter (R = 10 kΩ, C = 0.1 to 0.15 μF) used to smooth out the stair-step DAC steps into a continuous wave. |

---

## 📐 Timing Calculations

* **Parts A–C:** These run free in the main loop without a hardware timer. Output frequency depends entirely on compiler optimizations and CPU execution speed.
* **Part D & E (Timer32_0):** Configured with SMCLK = 3 MHz and a period register value of 1874 (1875 total clock cycles per tick).

\[\text{ISR Rate} = \frac{3,000,000\text{ Hz}}{1875} = 1600\text{ Hz}\]

\[\text{Sine Wave Frequency} = \frac{1600\text{ Hz}}{16\text{ steps}} = 100\text{ Hz}\]

* **Part E Dynamics:** Pressing S1 doubles the timer step delay to drop the final sine frequency to **50 Hz**. Pressing S2 bit-shifts the lookup table output values right by 1 to cut the amplitude exactly in half.

---

## 📁 Files
* [`src/main_A_B_sawtooth_triangle.c`](./src/main_A_B_sawtooth_triangle.c)
* [`src/main_C_sine_lookup.c`](./src/main_C_sine_lookup.c)
* [`src/main_D_sine_timer32_isr.c`](./src/main_D_sine_timer32_isr.c)
* [`src/main_E_sine_switch_control.c`](./src/main_E_sine_switch_control.c)
