#include <ti/devices/msp432p4xx/driverlib/driverlib.h>
#include<stdio.h>
#include<stdlib.h>

#define TRIANGLE
//#define SAWTOOTH

#define UP +1
#define DOWN -1

int main(void)
{
    uint8_t my_count = 0x00;
    int8_t increment = UP;

     // Stop watchdog timer
     WDT_A_hold(WDT_A_BASE);

     // port 4 used as output
     GPIO_setAsOutputPin(GPIO_PORT_P4, PIN_ALL8);

     while(1)
     {
         P4OUT = my_count; // output my_count on port 4
//         printf("Count = %02x\n",my_count);// initial check
#ifdef SAWTOOTH
         my_count ++;
#endif
#ifdef TRIANGLE

         my_count += increment;
         if(my_count == 255)
         {
             increment = DOWN;
         }
         if(my_count == 0)
         {
             increment = UP;
         }


#endif
     }

}

