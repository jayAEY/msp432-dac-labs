# 🌊 MSP432 DAC & Waveform Generation Labs

A collection of labs from my Embedded Systems class focused on generating analog waveforms (**sawtooth**, **triangle**, and **sine**) using a **TI MSP432P401R** LaunchPad. 

The first lab implements an 8-bit DAC using an **R-2R resistor ladder** connected to GPIO outputs. The second replaces the resistor ladder by using the MSP432's built-in **PWM generator** paired with an RC low-pass filter to smooth the signal into a clean analog wave. The third drives a dedicated **MCP4901 8-bit SPI DAC** over a serial bus. 


## 📦 Labs in this repo

| Lab | DAC Technique | Timers Used | Key Skill |
| :--- | :--- | :--- | :--- |
| [`lab-2.1-r2r-dac`](./lab-2.1-r2r-dac) | 8-bit R-2R resistor ladder on Port 4 | Timer32_0 (ISR) | Lookup-table waveform synthesis, GPIO-driven DAC, switch-controlled amplitude/frequency |
| [`lab-3.1-pwm-dac`](./lab-3.1-pwm-dac) | PWM output + RC reconstruction filter | TimerA_0 (PWM) + Timer32_0 (ISR) | PWM frequency/duty-cycle configuration, live CCR register updates from an ISR |
| [`lab-4.1-spi-dac`](./lab-4.1-spi-dac) | MCP4901 8-bit SPI DAC | eUSCI_B0 (SPI) + Timer32_0 (ISR) | SPI master configuration, /CS and /LDAC timing, serial DAC command words |

---

## 🔬 Testing & Hardware Setup

### 🛠️ Hardware Components
* **TI MSP432P401R LaunchPad** (SMCLK configured to 3 MHz)
* **8-bit R-2R resistor ladder** (used for Lab 2.1)
* RC low-pass filter (R=10kΩ, C=0.1–0.15µF) to smooth the PWM waveform
* **MCP4901 8-bit SPI DAC** (used for Lab 4.1)
* **Oscilloscope** to capture and verify the final waveforms (captures for each lab are in its `results/` folder)

### 💻 Firmware Implementation
* Developed in **C** using **TI DriverLib** for MSP432
* Programmed and built with **Code Composer Studio (CCS)**
* **Sine Generation:** Lookup tables generated offline and stored in flash memory for efficient runtime waveform generation.
* **Interrupt Handling:** Low-level register and peripheral setup (Timer32, TimerA, PWM, and GPIO interrupts) to handle timing-critical signal output
* **Serial Communication:** SPI master on eUSCI_B0 driving the MCP4901, with GPIO-controlled /CS and /LDAC

---

## 📁 Repo structure
```
msp432-dac-labs/
├── README.md
├── lab-2.1-r2r-dac/
│   ├── README.md
│   ├── results/
│   │   ├── r2r_100hz_raw.jpg
│   │   └── r2r_100hz_filtered.jpg
│   └── src/
│       ├── main_A_B_sawtooth_triangle.c
│       ├── main_C_sine_lookup.c
│       ├── main_D_sine_timer32_isr.c
│       └── main_E_sine_switch_control.c
├── lab-3.1-pwm-dac/
│   ├── README.md
│   ├── results/
│   │   ├── pwm_12_5hz_raw_vs_filtered.jpg
│   │   └── pwm_12_5hz_zoom.jpg
│   └── src/
│       ├── main_A_pwm_33pct.c
│       ├── main_B_pwm_sawtooth.c
│       └── main_C_pwm_sine.c
└── lab-4.1-spi-dac/
    ├── README.md
    ├── results/
    │   ├── spi_100hz_raw_vs_filtered.jpg
    │   └── spi_100hz_filtered_with_spi_signal.jpg
    └── src/
        ├── main_B_spi_dac_timer32.c
        └── main_extra_spi_sine.c
```
