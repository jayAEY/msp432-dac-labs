#include <ti/devices/msp432p4xx/driverlib/driverlib.h>
#include<stdio.h>
#include<stdlib.h>
#include<string.h>

#define FREQ_OSC 3000000 // SMCLK F
#define CCR0_VAL 255     // TimerA rollover
#define CCR1_VAL 0    // Pulse high time

#define SW1_BIT 0b00000010
#define SW2_BIT 0b00010000

volatile uint8_t dutyCycle = 0;


uint8_t sinTable[] = {128, 176, 218, 245, 255, 245, 218, 176, 128, 79, 37, 10, 0, 10, 37, 79};

/* Timer_A PWM Configuration Parameter */
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
    volatile uint8_t swPort;

    // Stop watchdog timer
    WDT_A_hold(WDT_A_BASE);

    /* The next snippet of code is used to actually configure the PWM signal:
    Configuring GPIO2.4 as peripheral output for PWM */
    MAP_GPIO_setAsPeripheralModuleFunctionOutputPin(GPIO_PORT_P2, GPIO_PIN4, GPIO_PRIMARY_MODULE_FUNCTION);

    /* Configuring Timer_A to have a period of approximately 500ms an initial duty cycle of 10% of that (3200 ticks) */
    MAP_Timer_A_generatePWM(TIMER_A0_BASE, &pwmConfig);

    //intialize Timer32_0
    MAP_Timer32_initModule(TIMER32_0_BASE, TIMER32_PRESCALER_1, TIMER32_32BIT, TIMER32_PERIODIC_MODE);

    // interrupt
    MAP_Timer32_enableInterrupt(TIMER32_0_BASE);
    MAP_Interrupt_enableInterrupt(INT_T32_INT1);
    MAP_Interrupt_enableMaster();

    //Timer32_0 start value
    MAP_Timer32_setCount(TIMER32_0_BASE, 15000);

    //start Timer32_0
    MAP_Timer32_startTimer(TIMER32_0_BASE, false);

    printf("Starting....\n");
    printf("SMCLK = %5.3f MHz\n",(float)FREQ_OSC/1000000.0); // main clock speed
    printf("F_PWM = %5.1f Hz\n", (float)FREQ_OSC/2/(CCR0_VAL+1) ); // FPWM
    printf("DC = %5.1f Percent\n",100*(float)(CCR1_VAL+1)/(CCR0_VAL+1)); // DC = CCR1/CCR0

    GPIO_setAsInputPinWithPullUpResistor(GPIO_PORT_P1, GPIO_PIN1|GPIO_PIN4);

    while(1)
    {
    swPort = P1IN;

        if ((swPort & SW1_BIT) == 0)
        {
            dutyCycle = 63;
        }

    }
}


void T32_INT1_IRQHandler(void)
{
        Timer32_clearInterruptFlag(TIMER32_0_BASE);
        dutyCycle++;
        MAP_Timer_A_setCompareValue(TIMER_A0_BASE, TIMER_A_CAPTURECOMPARE_REGISTER_1, sinTable[dutyCycle%16]);

}

