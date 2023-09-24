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

int main(void)
{
    RX();

    return 0;
}

#endif






