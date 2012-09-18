#ifndef _CONFIGURATION_H_
#define _CONFIGURATION_H_

#include <BoardConfig.h>
#include <stdio.h>
#include <appTimer.h>
#include <zdo.h>

#define APP_BLINK_PERIOD 1000

#define APP_MIN_BLINK_PERIOD 100

#define APP_MAX_BLINK_PERIOD 10000

#define APP_DEVICE_TYPE DEV_TYPE_ENDDEVICE

#define APP_TIMER_SENDING_PERIOD 10000

#define APP_THRESHOLD_FAILED_TRANSMISSION 4

//-----------------------------------------------
//BOARD_RAVEN
//-----------------------------------------------
#ifdef BOARD_RAVEN
  // Defines USART interface name to be used by application.
  #define APP_USART_CHANNEL USART_CHANNEL_1
#endif

//-----------------------------------------------
//BOARD_USB_DONGLE
//-----------------------------------------------
#ifdef BOARD_USB_DONGLE
  // Defines USART interface name to be used by application.
  #define APP_USART_CHANNEL USART_CHANNEL_VCP
#endif


// Configuraciones de la red 

#define CS_CHANNEL_MASK (1L << 0x0f)
#define CS_EXT_PANID 0xAAAAAAAAAAAAAAAALL
#define CS_UID 0x0LL // Unique Identifier 
#define CS_NWK_UNIQUE_ADDR false
#define CS_NWK_ADDR 0x000B // Network address
#define CS_END_DEVICE_SLEEP_PERIOD 10000

#define CS_APS_DATA_REQ_BUFFERS_AMOUNT 3
#define CS_APS_ACK_FRAME_BUFFERS_AMOUNT 2
#define CS_NWK_BUFFERS_AMOUNT 4
#define CS_NEIB_TABLE_SIZE 8
#define CS_ROUTE_TABLE_SIZE 10
#define CS_ADDRESS_MAP_TABLE_SIZE 2
#define CS_ROUTE_DISCOVERY_TABLE_SIZE 3
#define CS_DUPLICATE_REJECTION_TABLE_SIZE 8
#define CS_NWK_BTT_SIZE 8
#define CS_RF_TX_POWER 3


// Agregan lineas en alguna parte.... 

#endif // _CONFIGURATION_H_
