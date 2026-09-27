#include <ti/devices/msp432p4xx/driverlib/driverlib.h>
#include<stdio.h>
#include<stdlib.h>
#include<string.h>

#define FREQ_OSC 3000000 // SMCLK F
#define CCR0_VAL 15000   // TimerA rollover
#define CCR1_VAL 4950    // Pulse high time

/* Timer_A PWM Configuration Parameter */
Timer_A_PWMConfig pwmConfig =
{
    TIMER_A_CLOCKSOURCE_SMCLK,          // timerA clock source
    TIMER_A_CLOCKSOURCE_DIVIDER_1,      // prescaler
    CCR0_VAL,                           // CCR0 value
    TIMER_A_CAPTURECOMPARE_REGISTER_1,  // compare to 2nd CCR
    TIMER_A_OUTPUTMODE_RESET_SET,       // output mode
    CCR1_VAL                            // CCR1 value
};

int main(void)
{

    // Stop watchdog timer
    WDT_A_hold(WDT_A_BASE);

    /* The next snippet of code is used to actually configure the PWM signal:
    Configuring GPIO2.4 as peripheral output for PWM */
    MAP_GPIO_setAsPeripheralModuleFunctionOutputPin(GPIO_PORT_P2, GPIO_PIN4,GPIO_PRIMARY_MODULE_FUNCTION);

    /* Configuring Timer_A to have a period of approximately 500ms an initial duty cycle of 10% of that (3200 ticks) */
    MAP_Timer_A_generatePWM(TIMER_A0_BASE, &pwmConfig);

    printf("Starting....\n");
    printf("SMCLK = %5.3f MHz\n",(float)FREQ_OSC/1000000.0); // main clock speed
    printf("F_PWM = %5.1f Hz\n",(float)FREQ_OSC/CCR0_VAL); // main clock/CCR0
    printf("DC = %5.1f Percent\n",100*(float)CCR1_VAL/CCR0_VAL); // DC = CCR1/CCR0

    while(1)
    {
    //nothing to do here
    }
}
