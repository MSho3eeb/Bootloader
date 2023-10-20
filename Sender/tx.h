/*
 * tx.h
 *
 *  Created on: Sep 23, 2023
 *      Author: teleb
 */

#ifndef SENDER_TX_H_
#define SENDER_TX_H_

#include <stdbool.h>
#include <stdint.h>
#include "inc/hw_can.h"
#include "inc/hw_ints.h"
#include "inc/hw_memmap.h"
#include "driverlib/can.h"
#include "driverlib/gpio.h"
#include "driverlib/interrupt.h"
#include "driverlib/pin_map.h"
#include "driverlib/sysctl.h"
#include "driverlib/uart.h"

#include "inc/hw_types.h"
#include "inc/hw_flash.h"
#include "driverlib/flash.h"
#include "tx_Cfg.h"
#include "tm4c123gh6pm.h"




void TX(void);





#endif /* SENDER_TX_H_ */
