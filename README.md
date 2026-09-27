# 🌊 MSP432 DAC & Waveform Generation Labs

Two embedded systems labs focused on generating analog waveforms (**sawtooth**, **triangle**, and **sine**) using a **TI MSP432P401R** LaunchPad. 

The first lab builds a manual DAC from scratch using an **R-2R resistor ladder**. The second lab replaces the resistor ladder by using the MSP432's built-in **PWM generator** paired with an RC low-pass filter to smooth the signal into a clean analog wave. 


## 📦 Labs in this repo

| Lab | DAC Technique | Timers Used | Key Skill |
| :--- | :--- | :--- | :--- |
| [`lab-2.1-r2r-dac`](./lab-2.1-r2r-dac) | 8-bit R-2R resistor ladder on Port 4 | Timer32_0 (ISR) | Lookup-table waveform synthesis, GPIO-driven DAC, switch-controlled amplitude/frequency |
| [`lab-3.1-pwm-dac`](./lab-3.1-pwm-dac) | PWM output + RC reconstruction filter | TimerA_0 (PWM) + Timer32_0 (ISR) | PWM frequency/duty-cycle configuration, live CCR register updates from an ISR |

---

## 🔬 Testing & Hardware Setup

### 🛠️ Hardware Components
* **TI MSP432P401R LaunchPad** (SMCLK configured to 3 MHz)
* **8-bit R-2R resistor ladder** (used for Lab 2.1)
* RC low-pass filter (R=10kΩ, C=0.1–0.15µF) to smooth the PWM waveform
* **Oscilloscope** to capture and verify the final waveforms

### 💻 Firmware Implementation
* Developed in **C** using **TI DriverLib** for MSP432
* Programmed and built with **Code Composer Studio (CCS)**
* **Sine Generation:** Lookup tables precomputed offline and stored in flash to keep execution fast
* **Interrupt Handling:** Low-level register and peripheral setup (Timer32, TimerA, PWM, and GPIO interrupts) to handle timing-critical signal output

---

## 📁 Repo structure
```
msp432-dac-labs/
├── lab-2.1-r2r-dac/
│   ├── README.md
│   └── src/
│       ├── main_A_B_sawtooth_triangle.c
│       ├── main_C_sine_lookup.c
│       ├── main_D_sine_timer32_isr.c
│       └── main_E_sine_switch_control.c
└── lab-3.1-pwm-dac/
    ├── README.md
    └── src/
        ├── main_A_pwm_33pct.c
        ├── main_B_pwm_sawtooth.c
        └── main_C_pwm_sine.c
```
