    #include <ti/devices/msp432p4xx/driverlib/driverlib.h>
    #include<stdio.h>
    #include<stdlib.h>

    #define DAC_LATCH_HI(); { P2OUT |= BIT7; }
    #define DAC_LATCH_LO(); { P2OUT &= ~BIT7; }
    #define DAC_CS_HI(); { P2OUT |= BIT6; }
    #define DAC_CS_LO(); { P2OUT &= ~BIT6; }

    #define DAC_CONFIG_BITS 0b0101

    void Init_Clock(void);
    void Init_SPI(void);
    void Init_GPIO(void);
    void Write_MCP4901(void);

    uint8_t DAC1V = 0x4E;
    uint8_t DAC2V = 0x9b;

    volatile uint8_t DACByte = 0x4E;
    volatile uint8_t txByte;


    int main(void)
    {
        Init_Clock( );
        Init_SPI( );
        Init_GPIO( );

        MAP_WDT_A_holdTimer();
        MAP_Timer32_initModule(TIMER32_0_BASE, TIMER32_PRESCALER_1, TIMER32_32BIT, TIMER32_PERIODIC_MODE);
        MAP_Timer32_enableInterrupt(TIMER32_0_BASE);
        MAP_Interrupt_enableInterrupt(INT_T32_INT1);
        MAP_Interrupt_enableMaster();
        MAP_Timer32_setCount(TIMER32_0_BASE, 2999999);
        MAP_Timer32_startTimer(TIMER32_0_BASE, false);

        while(1);
    }

    void T32_INT1_IRQHandler(void)
    {
        MAP_Timer32_clearInterruptFlag(TIMER32_0_BASE);
        Write_MCP4901();

    }

    void Write_MCP4901(void)
    {
        if (DACByte == DAC1V)
        {
            DACByte = DAC2V;
        }
        else if (DACByte == DAC2V)
        {
            DACByte = DAC1V;
        }

    txByte = DACByte;

        DAC_CS_LO();
            UCB0TXBUF = (DAC_CONFIG_BITS << 4 | (txByte >> 4));
            while((UCB0IFG & UCTXIFG) == 0);
            UCB0TXBUF = txByte << 4;
            while((UCB0STATW & UCBUSY) > 0 );
        DAC_CS_HI();

        DAC_LATCH_LO();
        DAC_LATCH_HI();
    }

    void Init_Clock(void)
    {
        //Setting DCO to 12MHz
        CS_setDCOCenteredFrequency(CS_DCO_FREQUENCY_12); //Lets just do this this way............
        /* Initialize main clock branches*/
        CS_initClockSignal(CS_MCLK, CS_DCOCLK_SELECT, CS_CLOCK_DIVIDER_4 );
        CS_initClockSignal(CS_HSMCLK, CS_DCOCLK_SELECT, CS_CLOCK_DIVIDER_4 );
        CS_initClockSignal(CS_SMCLK, CS_DCOCLK_SELECT, CS_CLOCK_DIVIDER_4 );

    }


    void Init_GPIO(void)
    {
        GPIO_setAsOutputPin(GPIO_PORT_P2,GPIO_PIN7);
        GPIO_setAsOutputPin(GPIO_PORT_P2,GPIO_PIN6);

        DAC_LATCH_HI();
        DAC_CS_HI();

        GPIO_setAsPeripheralModuleFunctionInputPin(GPIO_PORT_P1,GPIO_PIN5 | GPIO_PIN6 | GPIO_PIN7, GPIO_PRIMARY_MODULE_FUNCTION);
    }


    void Init_SPI(void)
    {
        const eUSCI_SPI_MasterConfig spiMasterConfig =
            {
            EUSCI_B_SPI_CLOCKSOURCE_SMCLK, // SMCLK Clock Source
            3000000, // SMCLK = DCO = 3MHZ
            500000, // SPICLK = 500khz
            EUSCI_B_SPI_MSB_FIRST, // MSB First
            EUSCI_B_SPI_PHASE_DATA_CHANGED_ONFIRST_CAPTURED_ON_NEXT, // Phase
            EUSCI_B_SPI_CLOCKPOLARITY_INACTIVITY_HIGH, // High polarity
            EUSCI_B_SPI_3PIN // 3Wire SPI Mode
            };

        SPI_initMaster(EUSCI_B0_BASE, &spiMasterConfig);

        SPI_enableModule(EUSCI_B0_BASE);

    }


