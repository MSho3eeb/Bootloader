/*
 * rx.h
 *
 *  Created on: Sep 23, 2023
 *      Author: teleb
 */

#ifndef RECIEVER_RX_H_
#define RECIEVER_RX_H_


#include <stdbool.h>
#include <stdint.h>
#include "inc/hw_can.h"
#include "inc/hw_ints.h"
#include "inc/hw_memmap.h"
#include "inc/hw_types.h"
#include "inc/hw_flash.h"

#include "driverlib/can.h"
#include "driverlib/gpio.h"
#include "driverlib/interrupt.h"
#include "driverlib/pin_map.h"
#include "driverlib/sysctl.h"
#include "driverlib/flash.h"


#include "rx_Cfg.h"

void RX(void);


#endif /* RECIEVER_RX_H_ */
