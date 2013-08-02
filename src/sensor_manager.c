/**
 * @file sensor_manager.c
 * 
 * @brief Funciones relacionadas con la lectura de datos de los sensores. 
 * 
 */

#include <types.h>
#include <taskManager.h>
#include <raven_end_device.h>

extern app_message_request_t app_message;

static void get_lqi_rssi(void);
static void temperature_data_ready(bool result, uint16_t temperature);
static void battery_data_ready(bool result, uint16_t battery);
static void (*callback)(void);

/**
 * Start sensors 
 */ 
 
void start_sensor_manager(void)
{
    BSP_OpenTemperatureSensor();
    BSP_OpenBatterySensor();
}

/**
 * Stop sensors
 */ 

void stop_sensor_manager(void)
{
    BSP_CloseTemperatureSensor();
    BSP_CloseBatterySensor();
}

/**
 * Request data from temperature sensor
 */ 

void read_sensors(void (*sensor_readed)(void))
{
    get_lqi_rssi();
    // Agregar aqui funciones que obtengan informacion de otros sensores. 
    // Cada funcion debe alimentar a la estructura app_message.data
    // Se deben evitar ciclos infinitos. 
    send_command(1);
    BSP_ReadTemperatureData(temperature_data_ready);
    callback = sensor_readed;
}

/**
 * Read lqi and Rssi values
 */
static void get_lqi_rssi(void)
{
    ZDO_GetLqiRssi_t req;
    req.nodeAddr = CPU_TO_LE16(0);
    ZDO_GetLqiRssi(&req);
    app_message.data.lqi = req.lqi;
    app_message.data.rssi = req.rssi;
}

/**
 * Conf temperature data ready
 */ 

static void temperature_data_ready(bool result, uint16_t temperature)
{
    if (result)
        app_message.data.temperatura = CPU_TO_LE16(temperature);
    else
        app_message.data.temperatura = CPU_TO_LE16(0);

    BSP_ReadBatteryData(battery_data_ready);
}


/**
 * Conf battery data ready
 */

static void battery_data_ready(bool result, uint16_t battery)
{
    if (result)
        app_message.data.bateria = CPU_TO_LE16(battery);
    else
        app_message.data.bateria = CPU_TO_LE16(0);
    callback();
}



