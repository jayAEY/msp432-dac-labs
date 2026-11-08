#include <ti/devices/msp432p4xx/driverlib/driverlib.h>
#include<stdio.h>
#include<stdlib.h>

// #define TEST
volatile uint8_t DACValue = 128; // initialize to first sin value
volatile uint32_t cnt = 0;

uint8_t sinTable[] = {128, 176, 218, 245, 255, 245, 218, 176, 128, 79, 37, 10, 0, 10, 37, 79};


int main(void)
{

    // Stop watchdog timer
    MAP_WDT_A_holdTimer();

    // Configure GPIO
    MAP_GPIO_setAsOutputPin(GPIO_PORT_P4, PIN_ALL8);

    while(1)
    {
        #ifdef TEST
            // note this delays the DAC output. Turn off in production. 
            printf("Count:%6u | Count%%16:%u | DACValue:%3u   \n",
                  cnt, cnt%16, DACValue);
        #endif
        
        DACValue = sinTable[cnt%16];
        
        P4OUT = DACValue;
        cnt++;
    }
}

