#include <ti/devices/msp432p4xx/driverlib/driverlib.h>
#include<stdio.h>
#include<stdlib.h>
#include<string.h>

#define FREQ_OSC 3000000 // SMCLK F
#define CCR0_VAL 255     // TimerA rollover
#define CCR1_VAL 0    // Pulse high time

volatile uint8_t cnt = 0;
volatile uint8_t DACValue = 128; // initialize to first sin value

uint8_t sinTable[] = {128, 176, 218, 245, 255, 245, 218, 176, 128, 79, 37, 10, 0, 10, 37, 79};

// Timer_A PWM Configuration. F = 5859 Hz
Timer_A_PWMConfig pwmConfig =
{
    TIMER_A_CLOCKSOURCE_SMCLK,          // timerA clock source
    TIMER_A_CLOCKSOURCE_DIVIDER_2,      // prescaler
    CCR0_VAL,                           // CCR0 value
    TIMER_A_CAPTURECOMPARE_REGISTER_1,  // compare to 2nd CCR
    TIMER_A_OUTPUTMODE_RESET_SET,       // output mode
    CCR1_VAL                            // CCR1 value
};

int main(void)
{
    // Stop watchdog timer
    MAP_WDT_A_holdTimer();

    // Configuring GPIO2.4 as peripheral output for PWM 
    MAP_GPIO_setAsPeripheralModuleFunctionOutputPin(GPIO_PORT_P2, GPIO_PIN4, GPIO_PRIMARY_MODULE_FUNCTION);

    MAP_Timer_A_generatePWM(TIMER_A0_BASE, &pwmConfig);

    // Configure Timer32
    MAP_Timer32_initModule(TIMER32_0_BASE, TIMER32_PRESCALER_1, TIMER32_32BIT, TIMER32_PERIODIC_MODE);

    MAP_Timer32_enableInterrupt(TIMER32_0_BASE);
    MAP_Interrupt_enableInterrupt(INT_T32_INT1);
    MAP_Interrupt_enableMaster();

    // Timer32 F = 200Hz Fwave = 12.5 Hz
    MAP_Timer32_setCount(TIMER32_0_BASE, 14999);

    //start Timer32_0
    MAP_Timer32_startTimer(TIMER32_0_BASE, false);

    printf("\n\nStarting\n");

    while(1);
}


void T32_INT1_IRQHandler(void)
{
        MAP_Timer32_clearInterruptFlag(TIMER32_0_BASE);

        DACValue = sinTable[cnt%16];
        MAP_Timer_A_setCompareValue(TIMER_A0_BASE, TIMER_A_CAPTURECOMPARE_REGISTER_1, DACValue);
        cnt++;
}