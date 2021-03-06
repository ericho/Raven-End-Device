/**
 * @file raven_end_device.h
 * 
 * @brief Include file for main raven end device app
 * 
 * @author Erich Cordoba: erich.cm@gmail.com \n
 * Laboratorio Nacional de Informatica Avanzada: http://www.lania.mx
 * 
 * 
 */ 


#include <macAddr.h>
#include <appFramework.h>
#include <configServer.h>
#include <appTimer.h>
#include <aps.h>
#include <uid.h>
#include <zdo.h>
#include <dbg.h>
#include "leds.h"
#include "buttons.h"
#include "sliders.h"
#include "lcd.h"

/**
 * Defines section
 */ 

#define DEVICE1_EXT_ADDR    0x01ULL
#define LANIA_ENDPOINT 1
#define LANIA_PROFILE_ID CCPU_TO_LE16(1)
#define LANIA_DEVICE_ID CCPU_TO_LE16(1)
#define LANIA_DEVICE_VERSION 1
#define RAVEN_NODE 1
#define SERIALDONGLE_NODE 2

// Commands 
#define REQUEST_DATA_SENSOR 0x01
#define CONFIGURE_MODE      0x02
#define START_OPERATION     0x03
#define STOP_OPERATION      0x04
#define SLEEP_NODE          0x05

// Operation modes
#define REQUEST_RESPONSE_MODE   0x01
#define SLEEPING_MODE           0x02
#define SLEEPING_RF_MODE        0x03


#define APP_RX_BUFFER_SIZE  255

typedef enum
{
    APP_INITING_STATE,
    APP_STARTING_NETWORK_STATE,
    APP_STARTING_NETWORK_DONE,
    APP_IN_NETWORK_STATE,
    WAIT_FOR_CMD_STATE,
    PROCESS_CMD_STATE,
    READING_SENSORS_STATE,
    SENSORS_READED_STATE,
    SENDING_DATA_STATE,
    START_SLEEP_STATE,
    START_RF_SLEEP_STATE,
    WAIT_LCD_TO_SLEEP,
    APP_LEAVING_NETWORK_STATE,
    APP_STOP_STATE
} AppState_t;

typedef struct
{
    uint8_t nodeType;
    uint8_t tipo_tarjeta;
    ExtAddr_t   extAddr;
    ShortAddr_t shortAddr;
    uint8_t     lqi;
    int8_t      rssi;
    uint16_t    sensor_humedad;
    uint16_t    sensor_inclinacion;
    uint16_t    sensor_precipitacion;
    uint16_t    sensor_desplazamiento;
    uint16_t    bateria;
    uint16_t    temperatura;
} app_message_t;

typedef struct
{
    uint8_t     header[APS_ASDU_OFFSET];
    app_message_t   data;
    uint8_t     footer[APS_AFFIX_LENGTH - APS_ASDU_OFFSET];
} app_message_request_t;


typedef struct
{
	uint8_t command;
	uint8_t op_mode;
	uint8_t samples;
	uint8_t time;
}app_command_node_t;


extern void read_sensors(void (*sensor_readed)(void));
extern void start_sensor_manager(void);
extern void stop_sensor_manager(void);

bool usart_tx_busy;
bool new_data_on_buffer;
uint8_t offset_rx_buffer;
uint8_t app_rx_buffer[APP_RX_BUFFER_SIZE];

#define app_open_lcd        BSP_OpenLcd
#define app_send_lcd_cmd    BSP_SendLcdCmd
#define app_send_lcd_msg    BSP_SendLcdMsg
#define app_open_leds       BSP_OpenLeds
#define app_close_leds      BSP_CloseLeds
#define app_on_led          BSP_OnLed
#define app_off_led         BSP_OffLed
#define app_toggle_led      BSP_ToggleLed
#define lcd_operation_completed BSP_IsLcdOperationCompleted
