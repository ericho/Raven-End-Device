#ifndef _CONFIGURATION_H_
#define _CONFIGURATION_H_

#include <BoardConfig.h>
#include <stdio.h>
#include <appTimer.h>
#include <zdo.h>

#define APP_BLINK_PERIOD 1000

#define APP_MIN_BLINK_PERIOD 100

#define APP_MAX_BLINK_PERIOD 10000

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


#endif // _CONFIGURATION_H_
