#include <ti/devices/msp432p4xx/driverlib/driverlib.h>
#include<stdio.h>
#include<stdlib.h>

volatile uint32_t timerCount = 0;
volatile uint8_t MyTimerFlag = 0;
uint8_t sinTable[] = {128, 176, 218, 245, 255, 245, 218, 176, 128, 79, 37, 10, 0, 10, 37, 79};


int main(void)
{

    MAP_WDT_A_holdTimer();

    MAP_Timer32_initModule(TIMER32_0_BASE, TIMER32_PRESCALER_1, TIMER32_32BIT, TIMER32_PERIODIC_MODE);

    MAP_Timer32_setCount(TIMER32_0_BASE, 1874);

    MAP_Timer32_enableInterrupt(TIMER32_0_BASE);
    MAP_Interrupt_enableInterrupt(INT_T32_INT1);
    MAP_Interrupt_enableMaster();

    MAP_Timer32_startTimer(TIMER32_0_BASE, false);

    // Stop watchdog timer
    WDT_A_hold(WDT_A_BASE);

    // Set P4 to output direction
    GPIO_setAsOutputPin(GPIO_PORT_P4, PIN_ALL8);

    while(1);
}


void T32_INT1_IRQHandler(void)
{
    MAP_Timer32_clearInterruptFlag(TIMER32_0_BASE);
    timerCount++;
    P4OUT = sinTable[timerCount%16];
    MyTimerFlag = 1;
}

