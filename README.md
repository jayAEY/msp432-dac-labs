# 🌊 MSP432 DAC & Waveform Generation Labs

Two embedded systems labs that generate analog waveforms — **sawtooth**, **triangle**, and **sine** — on a **TI MSP432P401R** LaunchPad. The first builds a DAC from an **R-2R resistor ladder**; the second replaces the ladder with the on-chip **PWM generator**, filtered by an RC low-pass into a clean analog signal. Written in C with TI DriverLib for an Embedded Systems course at RRC Polytech.

## 📦 Labs in this repo

| Lab | DAC Technique | Timers Used | Key Skill |
| :--- | :--- | :--- | :--- |
| [`lab-2.1-r2r-dac`](./lab-2.1-r2r-dac) | 8-bit R-2R resistor ladder on Port 4 | Timer32_0 (ISR) | Lookup-table waveform synthesis, GPIO-driven DAC, switch-controlled amplitude/frequency |
| [`lab-3.1-pwm-dac`](./lab-3.1-pwm-dac) | PWM output + RC reconstruction filter | TimerA_0 (PWM) + Timer32_0 (ISR) | PWM frequency/duty-cycle configuration, live CCR register updates from an ISR |

## 🛠️ Hardware
- TI MSP432P401R LaunchPad (SMCLK @ 3 MHz)
- 8-bit R-2R resistor ladder (Lab 2.1)
- Single-pole RC low-pass filter (R=10kΩ, C=0.1–0.15µF) to reconstruct the analog waveform
- Oscilloscope for waveform capture/verification

## ⚙️ Firmware
- C, TI DriverLib for MSP432
- Code Composer Studio projects
- Sine lookup tables precomputed offline and stored in flash

## 🎯 Skills demonstrated
- Low-level register/peripheral configuration (Timer32, TimerA, PWM, GPIO, interrupts)
- Designing firmware interfaces that sit directly on top of hardware (ISR-driven DAC output)
- Signal reconstruction & basic analog filtering
- Bench validation of firmware behavior against calculated expectations (oscilloscope)
- Clear technical documentation of design decisions and results

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
