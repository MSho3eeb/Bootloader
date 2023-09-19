#include <stdint.h>
#include <stdbool.h>
#include "inc/hw_types.h"
#include "inc/hw_memmap.h"
#include "inc/hw_gpio.h"
#include "inc/hw_memmap.h"
#include "driverlib/gpio.h"
#include "driverlib/pin_map.h"
#include "driverlib/gpio.h"
#include "driverlib/sysctl.h"
#include "driverlib/flash.h"
#include "driverlib/sysctl.h"
#include "driverlib/uart.h"
#include "driverlib/interrupt.h"
#include "tm4c123gh6pm.h"


#define APP1_Address (0x00010000U)
#define APP1_Size (1000)
#define APP2_Address (0x00020000U)
#define APP2_Size (1000)
#define RUNNING_APP (0x00U)
#define BOOTLOADER_VTABLE (0x30000)
#define MYFLAGADDRESS (0x3FFF8)

#pragma DATA_SECTION(RunningAPP, ".RunPACK1")
const uint32_t RunningAPP[0x3FFF];
#pragma DATA_SECTION(ReadyAPP1, ".UpPACK2")
const uint32_t ReadyAPP1[0x4000];
#pragma DATA_SECTION(ReadyAPP2, ".UpPACK3")
const uint32_t ReadyAPP2[0x4000];
#pragma DATA_SECTION(RunningFlag, ".RunFlag")
const uint32_t RunningFlag[2];

uint32_t* RunningFlagPtr = (uint32_t*)MYFLAGADDRESS;
uint32_t SetRunningFlag = 1;

uint32_t app1Reset, app2Reset;

uint32_t *app1;
uint32_t *app2;

uint8_t rec[10000];
uint8_t *ptr = (uint8_t*)rec;

uint8_t data = 0;
uint8_t flag = 0;
uint32_t index = 0;

void recHandler(){

    data = UARTCharGet(UART0_BASE);
    if(data != 0xaa){
        *(ptr+index) = data;
        index++;
    }else{
        flag = 1;
    }

    UARTIntClear(UART0_BASE, 0xFFFF);


}


void moveApptoRun(uint32_t* src, uint32_t dst, uint32_t count){
    uint32_t i;

    for(i = 0; i < count; i += 0x400){
        FlashErase(dst+i);
    }

    FlashProgram(src, dst, count);
}

void forDelay(){
    int i;
    for(i = 0; i < 100000; i++);
}

void main(void)
{
    NVIC_ST_CTRL_R = 0x00;
    NVIC_ST_RELOAD_R = 0;
    if((*RunningFlagPtr == 0xFFFFFFFF)){
        uint32_t* resetAdd = (uint32_t*)0x30004;
        uint32_t reset = *resetAdd;
        FlashErase(0x04);
        FlashProgram(&reset,0x04, 4);
    }


    NVIC_VTABLE_R |= BOOTLOADER_VTABLE;

    if(*RunningFlagPtr == 0x01){
        FlashErase(MYFLAGADDRESS);
        NVIC_APINT_R |= NVIC_APINT_VECTKEY | NVIC_APINT_SYSRESETREQ;
    }

    SYSCTL_RCGCGPIO_R |= (1<<5);
    while((SYSCTL_PRGPIO_R>>5)&1 != 1);

    SysCtlClockSet(SYSCTL_SYSDIV_1 | SYSCTL_USE_OSC | SYSCTL_OSC_MAIN |
                   SYSCTL_XTAL_16MHZ);

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

    GPIO_PORTF_LOCK_R = 0x4C4F434B;
    GPIO_PORTF_CR_R |= (1<<0) | (1<<4) | (1<<2) | (1<<3);
    GPIO_PORTF_DIR_R &= (1<<0) | (1<<4);
    GPIO_PORTF_DIR_R |= (1<<2) | (1<<3);
    GPIO_PORTF_DEN_R |= (1<<0) | (1<<4) | (1<<2) | (1<<3);
    GPIO_PORTF_PUR_R |= (1<<0) | (1<<4);

    uint32_t haveApp = 0;

    uint64_t *pApp1Area = (uint64_t *) APP1_Address;
    uint64_t *pApp2Area = (uint64_t *) APP2_Address;
    uint64_t *RunnigAppArea = (uint64_t *) RUNNING_APP;

    uint8_t App1Empty = *pApp1Area == 0xFFFFFFFFFFFFFFFFU ? 1 : 0;
    uint8_t App2Empty = *pApp2Area == 0xFFFFFFFFFFFFFFFFU ? 1 : 0;
    haveApp = *RunnigAppArea == 0xFFFFFFFFFFFFFFFFU ? 0 : 1;
    uint32_t readyApp = 0;


    uint32_t RunIsBurned = 0;

    while(1){
        if(flag == 1){
            flag = 0;
            if(App1Empty == 0x01 && *RunningFlagPtr != 0x02){
                app1 = (uint32_t*)rec;
                uint32_t BLReset = *(uint32_t*)0x004;
                app1[7] = app1[1];
                app1[1] = BLReset;
                FlashErase(APP1_Address);
                FlashProgram(app1,APP1_Address, 11264);
                readyApp = APP1_Address;
                FlashErase(APP2_Address);
            }else if(App2Empty == 0x01 && *RunningFlagPtr != 0x02){
                app2 = (uint32_t*)rec;
                uint32_t BLReset = *(uint32_t*)0x004;
                app2[7] = app2[1];
                app2[1] = BLReset;
                FlashErase(APP2_Address);
                FlashProgram(app2,APP2_Address, 11264);
                readyApp = APP2_Address;
                FlashErase(APP1_Address);
            }
            forDelay();
            if(RunIsBurned == 0){
                SetRunningFlag = 0x02;
                moveApptoRun((uint32_t*)readyApp, RUNNING_APP, 12000);
                FlashErase(MYFLAGADDRESS);
                FlashProgram(&SetRunningFlag,MYFLAGADDRESS, 4);
                RunIsBurned = 1;
            }

            FlashErase(MYFLAGADDRESS);
            SetRunningFlag = 0x01;
            FlashProgram(&SetRunningFlag,MYFLAGADDRESS, 4);
            NVIC_VTABLE_R = RUNNING_APP;
            __asm(" mov r0, #0x0000000\n");
            __asm(" ldr r1, [r0, #28]");
            __asm(" bx r1\n");
        }else{
            if(((GPIO_PORTF_DATA_R>>4)&1) == 0  && (haveApp == 1)){
                forDelay();
                FlashErase(MYFLAGADDRESS);
                SetRunningFlag = 0x01;
                FlashProgram(&SetRunningFlag,MYFLAGADDRESS, 4);
                NVIC_VTABLE_R = RUNNING_APP;
                __asm(" mov r0, #0x0000000\n");
                __asm(" ldr r1, [r0, #28]");
                __asm(" bx r1\n");
            }
        }
    }

}
