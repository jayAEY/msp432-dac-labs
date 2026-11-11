#include <ti/devices/msp432p4xx/driverlib/driverlib.h>
#include<stdio.h>
#include<stdlib.h>

// Macros for setting LDAC and CS values
#define DAC_LATCH_HI(); { P2OUT |= BIT7; }
#define DAC_LATCH_LO(); { P2OUT &= ~BIT7; }
#define DAC_CS_HI(); { P2OUT |= BIT6; }
#define DAC_CS_LO(); { P2OUT &= ~BIT6; }

#define DAC_CONFIG_BITS 0b0111 // write data, Vref buffer, AV = 1, low power mode off

#define N_SAMPLES 16

void Init_Clock(void);
void Init_SPI(void);
void Init_GPIO(void);
void Write_MCP4901(void);

volatile uint8_t DACByte = 128;
volatile uint8_t txByte;
volatile uint32_t cnt = 0;

// Fwave = 100Hz
uint8_t sinTable[N_SAMPLES] = {128, 176, 218, 245, 255, 245, 218, 176, 128, 79, 37, 10, 0, 10, 37, 79};


int main(void)
{
    // Stop watchdog timer
    MAP_WDT_A_holdTimer();

    Init_Clock();
    Init_SPI();
    Init_GPIO();

    // Configure Timer32 with 1600 Hz
    MAP_Timer32_initModule(TIMER32_0_BASE, TIMER32_PRESCALER_1, TIMER32_32BIT, TIMER32_PERIODIC_MODE);
    MAP_Timer32_enableInterrupt(TIMER32_0_BASE);
    MAP_Interrupt_enableInterrupt(INT_T32_INT1);
    MAP_Interrupt_enableMaster();
    MAP_Timer32_setCount(TIMER32_0_BASE, 1874);
    MAP_Timer32_startTimer(TIMER32_0_BASE, false);

    while(1);
}


void T32_INT1_IRQHandler(void)
{
    MAP_Timer32_clearInterruptFlag(TIMER32_0_BASE);
    Write_MCP4901();

}


void Write_MCP4901(void)
{
DACByte = sinTable[cnt % N_SAMPLES];
txByte = DACByte;

cnt++;

    // Manually writing 2 byte SPI frame to registers
    DAC_CS_LO(); // enable CS
        UCB0TXBUF = ((DAC_CONFIG_BITS << 4) | (txByte >> 4)); // write 1st byte to TX BUF
        while((UCB0IFG & UCTXIFG) == 0); // wait until TXBUF data is moved to TX shift register
        UCB0TXBUF = txByte << 4; // write 2nd byte to TXBUF
        while((UCB0STATW & UCBUSY) > 0); // wait until TX is done
    DAC_CS_HI(); // disable CS

    DAC_LATCH_LO(); // Pulse LDAC 
    DAC_LATCH_HI();
}


void Init_Clock(void)
{
    // Setting DCO to 12MHz
    MAP_CS_setDCOCenteredFrequency(CS_DCO_FREQUENCY_12);
    // Initialize main clock branches
    MAP_CS_initClockSignal(CS_MCLK, CS_DCOCLK_SELECT, CS_CLOCK_DIVIDER_4);
    MAP_CS_initClockSignal(CS_HSMCLK, CS_DCOCLK_SELECT, CS_CLOCK_DIVIDER_4);
    MAP_CS_initClockSignal(CS_SMCLK, CS_DCOCLK_SELECT, CS_CLOCK_DIVIDER_4);
}


void Init_GPIO(void)
{
    MAP_GPIO_setAsOutputPin(GPIO_PORT_P2,GPIO_PIN7);
    MAP_GPIO_setAsOutputPin(GPIO_PORT_P2,GPIO_PIN6);

    DAC_LATCH_HI();
    DAC_CS_HI();

    MAP_GPIO_setAsPeripheralModuleFunctionInputPin(GPIO_PORT_P1,GPIO_PIN5 | GPIO_PIN6 | GPIO_PIN7, GPIO_PRIMARY_MODULE_FUNCTION);
}


void Init_SPI(void)
{
    const eUSCI_SPI_MasterConfig spiMasterConfig =
        {
        EUSCI_B_SPI_CLOCKSOURCE_SMCLK, // SMCLK Clock Source
        3000000, // SMCLK = DCO = 3MHZ
        500000, // SPICLK = 500khz
        EUSCI_B_SPI_MSB_FIRST, // MSB First
        EUSCI_B_SPI_PHASE_DATA_CHANGED_ONFIRST_CAPTURED_ON_NEXT, // Phase
        EUSCI_B_SPI_CLOCKPOLARITY_INACTIVITY_HIGH, // High polarity
        EUSCI_B_SPI_3PIN // 3Wire SPI Mode
        };

    MAP_SPI_initMaster(EUSCI_B0_BASE, &spiMasterConfig);

    MAP_SPI_enableModule(EUSCI_B0_BASE);
}