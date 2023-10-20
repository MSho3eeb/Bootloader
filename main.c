#include "define.h"

#if IS_TX == 1
#include "Sender/tx.h"

int main(void)
{
    TX();

    return 0;
}



#else
#include "Reciever/rx.h"

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

int main(void)
{
    VTABLE_OFFSET = BOOTLOADER_VTABLE;
    RX();

    return 0;
}

#endif






