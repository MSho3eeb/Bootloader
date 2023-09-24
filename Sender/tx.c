/*
 * tx.c
 *
 *  Created on: Sep 23, 2023
 *      Author: teleb
 */

#include "tx.h"
#include "../define.h"
#include "../SampleImages/HEX1.h"
#include "../SampleImages/HEX2.h"

volatile uint32_t g_ui32MsgCountTX = 0;
volatile bool g_bErrFlagTX = false;
volatile bool start = false;
volatile bool end = false;
volatile bool reset = false;


#if IS_TX == 1

volatile uint8_t data = 0;
volatile uint8_t flag = 0;
volatile uint32_t index = 0;
volatile uint32_t counter = 0;
const uint32_t img_data[IMG_SIZE];
uint8_t *ptr = (uint8_t*)img_data;


void SimpleDelay(void)
{
    SysCtlDelay(16000);
}


void CANIntHandler(void)
{
    uint32_t ui32Status;
    ui32Status = CANIntStatus(CAN0_BASE, CAN_INT_STS_CAUSE);

    if(ui32Status == CAN_INT_INTID_STATUS)
    {
        ui32Status = CANStatusGet(CAN0_BASE, CAN_STS_CONTROL);
        g_bErrFlagTX = 1;
    }
    else if(ui32Status == 1)
    {
        CANIntClear(CAN0_BASE, 1);
        g_ui32MsgCountTX++;
        g_bErrFlagTX = 0;
        GPIOPinWrite(GPIO_PORTF_BASE, GPIO_PIN_3, GPIO_PIN_3);
    }
    else if (ui32Status == 2)
    {
        CANIntClear(CAN0_BASE, 2);
        g_bErrFlagTX = 0;
        end = true;
    }
    else if(ui32Status == 3)
    {
        CANIntClear(CAN0_BASE, 3);
        g_bErrFlagTX = 0;
        start = true;
    }
    else if(ui32Status == 4)
    {
        CANIntClear(CAN0_BASE, 4);
        g_bErrFlagTX = 0;
        reset = true;
    }
    else
    {

    }
}

void recHandler();
void TX(void)
{
    tCANMsgObject sCANMessageStart;
    tCANMsgObject sCANMessageEnd;
    tCANMsgObject sCANMessage;
    tCANMsgObject sCANMessageReset;
    uint32_t ui32MsgData;
    uint8_t *pui8MsgData;

    pui8MsgData = (uint8_t *)&ui32MsgData;

    SysCtlPeripheralEnable(SYSCTL_PERIPH_UART0);
    SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOA);
    GPIOPinConfigure(GPIO_PA0_U0RX);
    GPIOPinConfigure(GPIO_PA1_U0TX);
    GPIOPinTypeUART(GPIO_PORTA_BASE, GPIO_PIN_0 | GPIO_PIN_1);
    UARTConfigSetExpClk(UART0_BASE, SysCtlClockGet(), 115200,
                        (UART_CONFIG_WLEN_8 | UART_CONFIG_STOP_ONE |
                                UART_CONFIG_PAR_NONE));
    UARTFIFODisable(UART0_BASE);
    UARTIntRegister(UART0_BASE, recHandler);
    UARTIntEnable(UART0_BASE, UART_INT_RX);
    IntEnable(INT_UART0);

    while(flag == 0);

    UARTIntDisable(UART0_BASE, UART_INT_RX);
    IntDisable(INT_UART0);

    SysCtlClockSet(SYSCTL_SYSDIV_1 | SYSCTL_USE_OSC | SYSCTL_OSC_MAIN | SYSCTL_XTAL_16MHZ);

    SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOB);

    SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOF);
    GPIOPinTypeGPIOOutput(GPIO_PORTF_BASE, GPIO_PIN_1 | GPIO_PIN_2 | GPIO_PIN_3);

    GPIOPinConfigure(GPIO_PB4_CAN0RX);
    GPIOPinConfigure(GPIO_PB5_CAN0TX);

    GPIOPinTypeCAN(GPIO_PORTB_BASE, GPIO_PIN_4 | GPIO_PIN_5);

    SysCtlPeripheralEnable(SYSCTL_PERIPH_CAN0);

    CANInit(CAN0_BASE);

    CANBitRateSet(CAN0_BASE, SysCtlClockGet(), 1000000);

    CANIntRegister(CAN0_BASE, CANIntHandler); // if using dynamic vectors

    CANIntEnable(CAN0_BASE, CAN_INT_MASTER | CAN_INT_ERROR | CAN_INT_STATUS);

    IntEnable(INT_CAN0);

    CANEnable(CAN0_BASE);

    sCANMessage.ui32MsgID = CAN_MSG_ID_DATA;
    sCANMessage.ui32MsgIDMask = 0;
    sCANMessage.ui32Flags = MSG_OBJ_TX_INT_ENABLE;
    sCANMessage.ui32MsgLen = sizeof(pui8MsgData);
    sCANMessage.pui8MsgData = pui8MsgData;

    sCANMessageEnd.ui32MsgID = CAN_MSG_ID_END;
    sCANMessageEnd.ui32MsgIDMask = 0;
    sCANMessageEnd.ui32Flags = MSG_OBJ_TX_INT_ENABLE;
    sCANMessageEnd.ui32MsgLen = 1;
    sCANMessageEnd.pui8MsgData = pui8MsgData;

    sCANMessageStart.ui32MsgID = CAN_MSG_ID_START;
    sCANMessageStart.ui32MsgIDMask = 0;
    sCANMessageStart.ui32Flags = MSG_OBJ_TX_INT_ENABLE;
    sCANMessageStart.ui32MsgLen = 1;
    sCANMessageStart.pui8MsgData = pui8MsgData;

    sCANMessageReset.ui32MsgID = CAN_MSG_ID_RESET;
    sCANMessageReset.ui32MsgIDMask = 0;
    sCANMessageReset.ui32Flags = MSG_OBJ_TX_INT_ENABLE;
    sCANMessageReset.ui32MsgLen = 1;
    sCANMessageReset.pui8MsgData = pui8MsgData;

    GPIOPinWrite(GPIO_PORTF_BASE, GPIO_PIN_1 | GPIO_PIN_2 | GPIO_PIN_3, 0);
    GPIOPinWrite(GPIO_PORTF_BASE, GPIO_PIN_1, GPIO_PIN_1);

    CANMessageSet(CAN0_BASE, 4, &sCANMessageReset, MSG_OBJ_TYPE_TX);
    while(!reset);

    ui32MsgData = FLASH_ON_BANK_1;                                                  /* change this */
    CANMessageSet(CAN0_BASE, 3, &sCANMessageStart, MSG_OBJ_TYPE_TX);
    while(!start);

    GPIOPinWrite(GPIO_PORTF_BASE, GPIO_PIN_1 | GPIO_PIN_2 | GPIO_PIN_3, 0);
    GPIOPinWrite(GPIO_PORTF_BASE, GPIO_PIN_3, GPIO_PIN_3);

    while(g_ui32MsgCountTX < IMG_SIZE)                                            /* change this */
    {
        ui32MsgData = img_data[g_ui32MsgCountTX];                 /* change this */
        CANMessageSet(CAN0_BASE, 1, &sCANMessage, MSG_OBJ_TYPE_TX);
        SimpleDelay();
    }
    GPIOPinWrite(GPIO_PORTF_BASE, GPIO_PIN_1 | GPIO_PIN_2 | GPIO_PIN_3, 0);
    GPIOPinWrite(GPIO_PORTF_BASE, GPIO_PIN_2, GPIO_PIN_2);
    CANMessageSet(CAN0_BASE, 2, &sCANMessageEnd, MSG_OBJ_TYPE_TX);
    while(!end);

    GPIOPinWrite(GPIO_PORTF_BASE, GPIO_PIN_1 | GPIO_PIN_2 | GPIO_PIN_3, 0);


    while(1);

}

void recHandler(){

    data = UARTCharGet(UART0_BASE);
    if(data == 0x20){
        counter++;
    }
    if(counter != 445){
        *(ptr+index) = data;
        index++;
    }else{
        flag = 1;
    }
    if(data == 0xaa){
        flag = 1;
    }

    UARTIntClear(UART0_BASE, 0xFFFF);

}
#endif
