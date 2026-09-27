#include <ti/devices/msp432p4xx/driverlib/driverlib.h>
#include<stdio.h>
#include<stdlib.h>

#define SW1_BIT 0b00000010
#define SW2_BIT 0b00010000

volatile uint8_t swPort;

volatile uint8_t outputAmplitude = 1;
volatile uint8_t amplitudeDivisor = 1;

volatile uint32_t cnt = 0;
volatile uint8_t cntDivisor = 1;

volatile bool flag = false;

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


    // Set P4 to output direction
    GPIO_setAsOutputPin(GPIO_PORT_P4, PIN_ALL8);
    GPIO_setAsInputPinWithPullUpResistor(GPIO_PORT_P1, GPIO_PIN1|GPIO_PIN4);

    printf("\n\nStarting\n");
//    printf("%10u %10u %10u %10u       cntDivisor = %2u\n", cnt, cnt/cntDivisor, cnt%16, (cnt/cntDivisor)%16, cntDivisor);

    MAP_Timer32_startTimer(TIMER32_0_BASE, false);

    while(1)
    {
        swPort = P1IN;

          // frequency
        if((swPort & SW1_BIT) == 0)
        {
            cntDivisor = 2;
        }
        else
        {
            cntDivisor = 1;
        }

        // amplitude
        if((swPort & SW2_BIT) == 0)
        {
            amplitudeDivisor = 2;
        }
        else
        {
            amplitudeDivisor = 1;
        }


        if(flag == true)
        {
              //f test
//            printf("%10u %10u %10u %10u       cntDivisor = %2u   %10u\n", cnt, cnt/cntDivisor, cnt%16, (cnt/cntDivisor)%16, cntDivisor, sinTable[(cnt/cntDivisor)%16]);
//            //atest
//            printf("%10u %10u %10u %10u       cntDivisor = %2u   %10u\n", outputAmplitude, outputAmplitude/amplitudeDivisor, outputAmplitude/amplitudeDivisor%16, (outputAmplitude/amplitudeDivisor)%16, amplitudeDivisor, sinTable[(outputAmplitude/amplitudeDivisor)%16]);

            flag = false;
        }
    }
}


void T32_INT1_IRQHandler(void)
{
    MAP_Timer32_clearInterruptFlag(TIMER32_0_BASE);
    cnt++;
    outputAmplitude++;
    //f
    P4OUT = sinTable[((cnt/cntDivisor)%16)]/amplitudeDivisor;
    //a
//    P4OUT = (outputAmplitude/amplitudeDivisor);
    flag = true;
}



