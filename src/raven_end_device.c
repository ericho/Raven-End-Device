/**
 * Programa principal, nodo raven. 
 */ 

#include <taskManager.h>
#include <zdo.h>
#include <configServer.h>
#include <aps.h>
#include <mac.h>
#include <raven_end_device.h>



/**
 *  Prototypes
 */ 

void APS_DataIndCoord(APS_DataInd_t *indData);
void APS_DataIndRouter(APS_DataInd_t *indData); // quitar?
void APS_DataIndDevice(APS_DataInd_t *indData);
void app_post_global_task(void);

/**
 * Global variables
 */ 

AppState_t appState = APP_INITING_STATE;
//DeviceState_t appDeviceState = INITIAL_DEVICE_STATE;

app_message_request_t app_message;
// Primitiva APS_DataReq
APS_DataReq_t   message_params;
HAL_AppTimer_t  device_timer;
ZDO_SleepReq_t sleep_request;


/**
 * Local variables
 */ 

// Request params
static ZDO_StartNetworkReq_t network_params; 
static APS_RegisterEndpointReq_t    endpoint_params;
static HAL_AppTimer_t   stating_network_timer;
static ZDO_ZdpReq_t     leave_req;
SimpleDescriptor_t  simple_descriptor;
static DeviceType_t deviceType;
/**
 * Local functions
 */

static void ZDO_StartNetworkConf(ZDO_StartNetworkConf_t *confirm_info);
static void startingNetworkTimerFired(void);
static void zdpLeaveResp(ZDO_ZdpResp_t *zdpResp);
static void appTaskHandler(void);
void ZDO_WakeUpInd(void);
static void APS_DataConf(APS_DataConf_t *confInfo);
void data_sensor_readed(void);
void ZDO_SleepConf(ZDO_SleepConf_t *conf);

void APL_TaskHandler(void)
{
    bool rx_on_when_idle_flag = true;
    switch (appState)
    {
        case APP_INITING_STATE:
            app_starting();

            sleep_request.ZDO_SleepConf = ZDO_SleepConf;
            // Configurando descriptores principales
            simple_descriptor.endpoint = LANIA_ENDPOINT;
            simple_descriptor.AppProfileId = LANIA_PROFILE_ID;
            simple_descriptor.AppDeviceId = LANIA_DEVICE_ID;
            simple_descriptor.AppDeviceVersion = LANIA_DEVICE_VERSION;

            endpoint_params.simpleDescriptor = &simple_descriptor;
            app_message.data.nodeType = DEVICE_TYPE_END_DEVICE;
            endpoint_params.APS_DataInd = APS_DataIndDevice;
            app_message.data.tipo_tarjeta = RAVEN_NODE;
                
            // Obteniendo direccion extendida de la EEPROM
            ExtAddr_t extAddr;
            CS_ReadParameter(CS_UID_ID, &extAddr);
            app_message.data.extAddr = extAddr;

            // Necesario para entrar en modo sleep
            rx_on_when_idle_flag = false;  
            CS_WriteParameter(CS_RX_ON_WHEN_IDLE_ID, &rx_on_when_idle_flag);

            // Tipo de dispositivo.
            deviceType = app_message.data.nodeType;
            CS_WriteParameter(CS_DEVICE_TYPE_ID, &deviceType);
            // Registrando descriptores
            APS_RegisterEndpointReq(&endpoint_params);
            
            // Configurando parametros para enviar mensajes 
            message_params.profileId                = simple_descriptor.AppProfileId;
            message_params.dstAddrMode              = APS_SHORT_ADDRESS;
            message_params.dstAddress.shortAddress  = CPU_TO_LE16(0);
            message_params.dstEndpoint              = LANIA_ENDPOINT;
            message_params.clusterId                = CPU_TO_LE16(1);
            message_params.srcEndpoint              = simple_descriptor.endpoint;
            message_params.asduLength               = sizeof(app_message.data);
            message_params.asdu                     = (uint8_t *)(&app_message.data);
            message_params.txOptions.acknowledgedTransmission = 1;
            message_params.radius                   = 0x0;
            message_params.APS_DataConf             = APS_DataConf;

            appState = APP_STARTING_NETWORK_STATE;
            app_post_global_task();
            break;

        case APP_STARTING_NETWORK_STATE:
            // Iniciando la red. Falta agregar visualizacion 
            // en LCD y/o LEDS
            network_starting();
            network_params.ZDO_StartNetworkConf = ZDO_StartNetworkConf;
            ZDO_StartNetworkReq(&network_params);
            break;

        case APP_IN_NETWORK_STATE:
            // visualizar red iniciada
            network_started();
            appState = READING_SENSORS_STATE;
            app_post_global_task();
            break;

        case READING_SENSORS_STATE:
            start_sensor_manager();
            read_sensors(data_sensor_readed);
            break;

        case SENSORS_READED_STATE:
            stop_sensor_manager();
            // Visualizar datos leidos. 
            appState = SENDING_DATA_STATE;
            app_post_global_task();
            break;

        case SENDING_DATA_STATE:
            air_tx_started();
            APS_DataReq(&message_params);
            break;

        case START_SLEEP_STATE:
            visual_sleep();
            if (!lcd_ready_to_sleep())
            {
                appState = WAIT_LCD_TO_SLEEP;
                app_post_global_task();
            }
            else
            {
                ZDO_SleepReq(&sleep_request);
            }
            break;
        case WAIT_LCD_TO_SLEEP:
            if (!lcd_ready_to_sleep())
            {
                app_post_global_task();
            }
            else
            {
                ZDO_SleepReq(&sleep_request);
            }
            break;
    }
}

/**
 * Confirmacion recibida de ZDO_StartNetwork
 */ 

static void ZDO_StartNetworkConf(ZDO_StartNetworkConf_t *confirm_info)
{
    if (confirm_info->status == ZDO_SUCCESS_STATUS)
        appState = APP_IN_NETWORK_STATE;
    else
        appState = APP_STARTING_NETWORK_STATE;
    
    app_post_global_task();
}

/**
 * Callback para lectura de sensores 
 */ 

void data_sensor_readed(void)
{
    appState = SENSORS_READED_STATE;
    app_post_global_task();
}

/**
 * Callback para recepcion de datos 
 */ 

void APS_DataIndDevice(APS_DataInd_t *indData)
{
    (void)indData;
}

/**
 * Confirmacion de datos enviados 
 */ 

static void APS_DataConf(APS_DataConf_t *confInfo)
{
    if (confInfo->status == APS_SUCCESS_STATUS)
    {
        air_tx_finished();
        appState = START_SLEEP_STATE;
    }
    app_post_global_task();
}

/**
 * Cambia entre estados invocando al manejador de tareas 
 */ 

void app_post_global_task()
{
    SYS_PostTask(APL_TASK_ID);
}


/**
 * Funcion para Binding (obligatoria)
 */

void ZDO_BindIndication(ZDO_BindInd_t *bindInd)
{
    (void)bindInd;
}

/**
 * Notificaciones de actividad en la red
 */ 

void ZDO_MgmtNwkUpdateNotf(ZDO_MgmtNwkUpdateNotf_t *nwkParams)
{
  (void)nwkParams;
}


/**
 * Confirmacion de sleep
 */ 

void ZDO_SleepConf(ZDO_SleepConf_t *conf)
{
    if (conf->status != ZDO_SUCCESS_STATUS)
    {
        appState = START_SLEEP_STATE;
        app_post_global_task();
    }
    
}

/**
 * Confirmacion de wakeup del dispositivo
 */ 

void ZDO_WakeUpInd(void)
{
    visual_wakeup();
    appState = READING_SENSORS_STATE;
    app_post_global_task();
}


/**
 * Funcion para Unbinding (obligatoria)
 */ 

void ZDO_UnbindIndication(ZDO_UnbindInd_t *unbindInd)
{
    (void)unbindInd;
}

/**
 * The main function
 */ 
int main (void)
{
    SYS_SysInit();

    for(;;)
    {
        SYS_RunTask();
    }
}