/*
 * rx_Cfg.h
 *
 *  Created on: Sep 23, 2023
 *      Author: teleb
 */

#ifndef RECIEVER_RX_CFG_H_
#define RECIEVER_RX_CFG_H_


#define CAN_MSG_ID_RESET            0x00001000
#define CAN_MSG_ID_START            0x00001001
#define CAN_MSG_ID_END              0x00001002
#define CAN_MSG_ID_DATA             0x00001003
#define CAN_MSG_ID_DATALEN          0x00001004

#define ADDRESS_BOOTLOADER_1        0x00000000
#define ADDRESS_BANK_1              0x00010000
#define ADDRESS_BANK_2              0x00020000
#define ADDRESS_VARS                0x00030000

#define VTABLE_OFFSET               (*((volatile unsigned long *)0xE000ED08))
#define PROGRAM_TO_RUN              (*((volatile unsigned long *)ADDRESS_VARS))






#endif /* RECIEVER_RX_CFG_H_ */
