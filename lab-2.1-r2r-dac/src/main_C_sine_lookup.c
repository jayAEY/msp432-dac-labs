#include <ti/devices/msp432p4xx/driverlib/driverlib.h>
#include<stdio.h>
#include<stdlib.h>

int main(void)
{
    volatile uint8_t cnt = 0;
    uint8_t sinTable[] = {128, 176, 218, 245, 255, 245, 218, 176, 128, 79, 37, 10, 0, 10, 37, 79};

    // Stop watchdog timer
    WDT_A_hold(WDT_A_BASE);

    // Set P4 to output direction
    GPIO_setAsOutputPin(GPIO_PORT_P4, PIN_ALL8);

     while(1)
    {
//        printf("%d  %d          %d\n", cnt, cnt%16, sinTable[cnt%16]);
        cnt++;
        P4OUT = sinTable[cnt%16];
    }
}

