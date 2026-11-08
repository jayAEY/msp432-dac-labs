#include <ti/devices/msp432p4xx/driverlib/driverlib.h>
#include<stdio.h>
#include<stdlib.h>
#include<string.h>

#define FREQ_OSC 3000000 // SMCLK F
#define CCR0_VAL 14999   // TimerA rollover
#define CCR1_VAL 4950    // Pulse high time

// Timer_A PWM Configuration. F = 200 Hz %DC = 33%
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
    MAP_WDT_A_holdTimer();

    // Configuring GPIO2.4 as peripheral output for PWM 
    MAP_GPIO_setAsPeripheralModuleFunctionOutputPin(GPIO_PORT_P2, GPIO_PIN4,GPIO_PRIMARY_MODULE_FUNCTION);

    MAP_Timer_A_generatePWM(TIMER_A0_BASE, &pwmConfig);

    printf("\n\nStarting\n");
    printf("SMCLK = %5.3f MHz\n",(float)FREQ_OSC/1000000.0); // main clock speed
    printf("F_PWM = %5.1f Hz\n",(float)FREQ_OSC/(CCR0_VAL+1)); // PWM F
    printf("DC = %5.1f Percent\n",100*(float)CCR1_VAL/(CCR0_VAL+1)); // DC = CCR1/CCR0

    while(1);
}
