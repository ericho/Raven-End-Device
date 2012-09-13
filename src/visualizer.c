/*
 * @file visualizer.c
 * 
 * @brief Funciones de visualizacion usando LCD y LEDs
 */ 

#include <raven_end_device.h>

void app_starting(void);
void network_starting(void);
void network_started(void);
void air_tx_started(void);
void air_tx_finished(void);
void visual_wakeup(void);
void visual_sleep(void);
int lcd_ready_to_sleep(void);

/**
 * Inicia operacion del visualizador, Vista inicial. 
 */ 

void app_starting(void)
{
//    app_open_leds();
    app_open_lcd();
    app_send_lcd_cmd(LCD_CMD_SYMB_ANTENNA_LEVEL_2);
    app_send_lcd_cmd(LCD_CMD_LED_ON);
    app_send_lcd_cmd(LCD_CMD_SYMB_RAVEN_ON);
    app_send_lcd_msg("INIT   ");
    
}

/**
 * Visualizacion de inicio de red
 */ 

void network_starting(void)
{
    app_send_lcd_cmd(LCD_CMD_LED_ON);
    app_send_lcd_cmd(LCD_CMD_SYMB_SPACE_SUN);
    app_send_lcd_msg("CONN   ");
}

/**
 * Visualizacion de red iniciada
 */

void network_started(void)
{
    app_send_lcd_cmd(LCD_CMD_LED_ON);
    app_send_lcd_cmd(LCD_CMD_SYMB_SPACE_SUN);
    app_send_lcd_cmd(LCD_CMD_SYMB_ZIGBEE_ON);
    app_send_lcd_msg("RED    ");
}

/**
 * Visualizacion al enviar datos 
 */ 

void air_tx_started(void)
{
    app_send_lcd_cmd(LCD_CMD_SYMB_TRX_TX);
}

/**
 * Visualizacion al recibir datos
 */ 

void air_tx_finished(void)
{
    app_send_lcd_cmd(LCD_CMD_SYMB_TRX_OFF);
}


/**
 * Visualizacion al despertar
 */ 

void visual_wakeup(void)
{
    app_send_lcd_cmd(LCD_CMD_SYMB_ANTENNA_LEVEL_2);
    app_send_lcd_cmd(LCD_CMD_SYMB_ZIGBEE_ON);
    app_send_lcd_cmd(LCD_CMD_LED_ON);
    app_send_lcd_cmd(LCD_CMD_SYMB_RAVEN_ON);
    app_send_lcd_cmd(LCD_CMD_SYMB_SPACE_SUN);
    
}

/**
 * Visualizacion al dormir
 */ 

void visual_sleep(void)
{
    
    app_send_lcd_cmd(LCD_CMD_SYMB_ANTENNA_LEVEL_0);
    app_send_lcd_cmd(LCD_CMD_SYMB_SPACE_MOON);
    app_send_lcd_cmd(LCD_CMD_LED_OFF);
    app_send_lcd_cmd(LCD_CMD_SYMB_TRX_OFF);
}

/**
 * Verifica si las operaciones del LCD estan completas. 
 */ 

int lcd_ready_to_sleep(void)
{
  return lcd_operation_completed();
}

