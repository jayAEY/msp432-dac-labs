#include <ti/devices/msp432p4xx/driverlib/driverlib.h>
#include<stdio.h>
#include<stdlib.h>

#define SW1_BIT 0b00000010
#define SW2_BIT 0b00010000
// #define TEST

volatile uint8_t swPort;

volatile uint32_t cnt = 0;

volatile uint8_t aDivisor = 1;
volatile uint8_t fDivisor = 1;

volatile uint8_t DACValue = 128; // initialize to first sin value


volatile bool flag = false;


uint8_t sinTable[] = {128, 176, 218, 245, 255, 245, 218, 176, 128, 79, 37, 10, 0, 10, 37, 79};


int main(void)
{
    // Stop watchdog timer
    MAP_WDT_A_holdTimer();

    // Configure Timer32
    MAP_Timer32_initModule(TIMER32_0_BASE, TIMER32_PRESCALER_1, TIMER32_32BIT, TIMER32_PERIODIC_MODE);

    MAP_Timer32_setCount(TIMER32_0_BASE, 1874);

    MAP_Timer32_enableInterrupt(TIMER32_0_BASE);
    MAP_Interrupt_enableInterrupt(INT_T32_INT1);
    MAP_Interrupt_enableMaster();

    // Configure GPIO
    MAP_GPIO_setAsOutputPin(GPIO_PORT_P4, PIN_ALL8);
    MAP_GPIO_setAsInputPinWithPullUpResistor(GPIO_PORT_P1, GPIO_PIN1|GPIO_PIN4);

    MAP_Timer32_startTimer(TIMER32_0_BASE, false);

    printf("\n\nStarting\n");

    while(1)
    {
        swPort = P1IN;

        // frequency SW logic
        if((swPort & SW1_BIT) == 0)
        {
            fDivisor = 2;
        }
        else
        {
            fDivisor = 1;
        }

        // amplitude sw logic 
        if((swPort & SW2_BIT) == 0)
        {
            aDivisor = 2;
        }
        else
        {
            aDivisor = 1;
        }


        if(flag == true)
        {
        #ifdef TEST   
            // note this delays the DAC output. Turn off in production.   
            printf("Count:%6u |  F Divisor:%u | A Divisor:%u | DACValue:%3u   \n",
                  cnt, fDivisor, aDivisor, DACValue);
        #endif
        flag = false;
        }
    }
}


void T32_INT1_IRQHandler(void)
{
    MAP_Timer32_clearInterruptFlag(TIMER32_0_BASE);

    DACValue = sinTable[((cnt/fDivisor)%16)]/aDivisor;
    
    P4OUT = DACValue;
    cnt++;
    flag = true;
}