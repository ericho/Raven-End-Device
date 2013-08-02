/**
 * @file protocol_parser.c
 * 
 * @brief Interprete de comandos del protocolo de comunicacion serial 
 * 
 */

#include <raven_end_device.h>
#include <usart.h>

#define USART_RX_BUFFER_LENGTH 5
#define APP_BUFFER_UPDATED 0
#define MAX_APP_BUFFER_SIZE_REACHED 1
#define PACKET_HEADER   0x55
#define PACKET_FOOTER   0xaa
#define REQUEST_DATA_CMD 1
#define WAIT_FOR_CMD_STATE 1
#define CMD_DETECTED_STATE 2



static HAL_UsartDescriptor_t usart_descriptor;
static uint8_t rx_buffer[USART_RX_BUFFER_LENGTH];


void received_bytes(uint16_t read_bytes_length);
static void write_bytes(void);
void send_data_usart(void *data, uint8_t data_length);
uint8_t add_byte_to_buffer(uint8_t data);
void analize_data_received(void);

uint8_t output_command_buffer[6] = {0x55, 0, 6, 0, 0, 0xAA};
uint8_t cmd_state;
uint16_t recv_data_len;


void init_usart(void)
{
    usart_descriptor.tty = USART_CHANNEL_1;
    usart_descriptor.mode = USART_MODE_ASYNC;
    usart_descriptor.flowControl = USART_FLOW_CONTROL_NONE;
    usart_descriptor.baudrate = USART_BAUDRATE_9600;
    usart_descriptor.dataLength = USART_DATA8;
    usart_descriptor.parity = USART_PARITY_NONE;
    usart_descriptor.stopbits = USART_STOPBIT_1;
    usart_descriptor.rxBuffer = rx_buffer;
    usart_descriptor.rxBufferLength = sizeof(rx_buffer);
    usart_descriptor.txBuffer = NULL;
    usart_descriptor.txBufferLength = 0;
    usart_descriptor.rxCallback = received_bytes; // Callback for received data
    usart_descriptor.txCallback = write_bytes; // Callback for transmited data
    HAL_OpenUsart(&usart_descriptor);
    offset_rx_buffer = 0;
    cmd_state = WAIT_FOR_CMD_STATE;
    recv_data_len = 0;
}


void close_usart(void)
{
    HAL_CloseUsart(&usart_descriptor);
}

void received_bytes(uint16_t read_bytes_length)
{
    uint8_t data_received;
    if (read_bytes_length == 1){
        if (HAL_ReadUsart(&usart_descriptor, &data_received, read_bytes_length) == 1) {
            if (add_byte_to_buffer(data_received) == APP_BUFFER_UPDATED){
                analize_data_received();
            }
            else {
                reset_app_buffer();
                return;
            }
            
        }
    }
    return;
}

void analize_data_received(void)
{
    char s[20];
    
    if (offset_rx_buffer >= 3){
        if (cmd_state == WAIT_FOR_CMD_STATE) {
        
            if (app_rx_buffer[0] == PACKET_HEADER){
                // Get data lenght 
                recv_data_len = (uint16_t) (app_rx_buffer[1] << 8);
                recv_data_len |= (uint8_t) app_rx_buffer[2];
                
                if (recv_data_len >= 6 && recv_data_len <= 30){
                    cmd_state = CMD_DETECTED_STATE;
                }
                else {
                    reset_app_buffer();
                    cmd_state = WAIT_FOR_CMD_STATE;
                }
            }
            else {
                reset_app_buffer();
                cmd_state = WAIT_FOR_CMD_STATE;
            }    
        }
        else if (cmd_state == CMD_DETECTED_STATE) {
            if (offset_rx_buffer == recv_data_len) {
                if (app_rx_buffer[offset_rx_buffer - 1] == PACKET_FOOTER) {

                    // Aqui va todo lo relacionado con desempaquetar los datos de los sensores. 
                    sprintf(s, "FOOTER");
                    app_send_lcd_msg(s);
                }
            }
        }
        
    }
}

uint8_t add_byte_to_buffer(uint8_t data)
{
    if (offset_rx_buffer < APP_RX_BUFFER_SIZE){
        app_rx_buffer[offset_rx_buffer] = data;
        offset_rx_buffer++;
        return APP_BUFFER_UPDATED;
    } else 
        return MAX_APP_BUFFER_SIZE_REACHED;
}

void reset_app_buffer(void)
{
    offset_rx_buffer = 0;
}

static void write_bytes(void)
{
    usart_tx_busy = false;
}

void send_data_usart(void *data, uint8_t data_length)
{
    usart_tx_busy = true;
    HAL_WriteUsart(&usart_descriptor, data, data_length);
}

void send_command(uint8_t cmd)
{
//    output_command_buffer[0] = 0x55;
//    output_command_buffer[1] = 0;
//    output_command_buffer[2] = 6;
//    output_command_buffer[3] = 0;
    output_command_buffer[4] = cmd;
//    output_command_buffer[5] = 0xaa;
    send_data_usart((void*)output_command_buffer, 6);
}
