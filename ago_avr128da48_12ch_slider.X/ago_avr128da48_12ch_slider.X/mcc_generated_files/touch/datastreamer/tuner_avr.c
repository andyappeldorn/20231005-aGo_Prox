/*******************************************************************************
  Touch Library 3.5.0 Release

  @Company
    Microchip Technology Inc.

  @File Name
    tuner.c

  @Summary
    QTouch Modular Library

  @Description
    Provides the two way datastreamer protocol implementation, transmission of
          module data to data visualizer software using UART port.
	
*******************************************************************************/
/*
© [2023] Microchip Technology Inc. and its subsidiaries.

    Subject to your compliance with these terms, you may use Microchip 
    software and any derivatives exclusively with Microchip products. 
    You are responsible for complying with 3rd party license terms  
    applicable to your use of 3rd party software (including open source  
    software) that may accompany Microchip software. SOFTWARE IS ?AS IS.? 
    NO WARRANTIES, WHETHER EXPRESS, IMPLIED OR STATUTORY, APPLY TO THIS 
    SOFTWARE, INCLUDING ANY IMPLIED WARRANTIES OF NON-INFRINGEMENT,  
    MERCHANTABILITY, OR FITNESS FOR A PARTICULAR PURPOSE. IN NO EVENT 
    WILL MICROCHIP BE LIABLE FOR ANY INDIRECT, SPECIAL, PUNITIVE, 
    INCIDENTAL OR CONSEQUENTIAL LOSS, DAMAGE, COST OR EXPENSE OF ANY 
    KIND WHATSOEVER RELATED TO THE SOFTWARE, HOWEVER CAUSED, EVEN IF 
    MICROCHIP HAS BEEN ADVISED OF THE POSSIBILITY OR THE DAMAGES ARE 
    FORESEEABLE. TO THE FULLEST EXTENT ALLOWED BY LAW, MICROCHIP?S 
    TOTAL LIABILITY ON ALL CLAIMS RELATED TO THE SOFTWARE WILL NOT 
    EXCEED AMOUNT OF FEES, IF ANY, YOU PAID DIRECTLY TO MICROCHIP FOR 
    THIS SOFTWARE.
*/
/*----------------------------------------------------------------------------
  include files
----------------------------------------------------------------------------*/
#include "tuner_avr.h"
#include "../../system/system.h"
#include "string.h"
#if (DEF_TOUCH_DATA_STREAMER_ENABLE == 1u)



/*----------------------------------------------------------------------------
 *     defines
 *----------------------------------------------------------------------------*/

#define SCROLLER_MODULE_OUTPUT 1u

#define FREQ_HOP_AUTO_MODULE_OUTPUT 0u

#define SURFACE_MODULE_OUTPUT 0u

#define GESTURE_MODULE_OUTPUT 0u

#define MAX_OUTPUT_MODULES 2u 

typedef struct tagSensorData_t {
	uint16_t signal_t;
	uint16_t reference;
	int16_t delta;
	uint8_t state;
	uint16_t cc_val;
}sensorData_t;

#define UART_TX_REGISTER				(USART1.TXDATAL)
#define UART_RX_REGISTER                (USART1.RXDATAL)
#define UART_STATUS_REGISTER            (USART1.STATUS)
#define UART_CTRLA_REGISTER             (USART1.CTRLA)
#define STREAMING_DEBUG_DATA			(1U)
#define STREAMING_CONFIG_DATA			(2U)
#define PROJECT_CONFIG_DATA_LEN			(10U)

/*----------------------------------------------------------------------------
  global variables
----------------------------------------------------------------------------*/
extern qtm_acq_avr_da_node_config_t  ptc_seq_node_cfg1[];
extern qtm_touch_key_data_t          qtlib_key_data_set1[];
extern qtm_touch_key_config_t   	 qtlib_key_configs_set1[];
extern qtm_touch_key_group_config_t  qtlib_key_grp_config_set1;
extern qtm_acq_node_data_t			 ptc_qtlib_node_stat1[];

#if (SCROLLER_MODULE_OUTPUT == 1U)
extern qtm_scroller_data_t	  qtm_scroller_data1[];
extern qtm_scroller_control_t qtm_scroller_control1;
extern qtm_scroller_config_t  qtm_scroller_config1[];
#endif


void uart_send_frame_header(uint8_t trans_type, uint8_t frame, uint16_t frame_len);
void uart_send_data(uint8_t con_or_debug, uint8_t *data_ptr,  uint8_t data_len);
void copy_run_time_data(uint8_t channel_num);

typedef struct tag_uart_command_info_t
{
	uint8_t transaction_type;
	uint8_t frame_id;
	uint16_t num_of_bytes;
	uint8_t header_status;
} uart_command_info_t;
uart_command_info_t volatile uart_command_info;   

uint8_t tx_data_len = 0;
uint8_t *tx_data_ptr ;

volatile uint8_t  current_debug_data;
volatile uint8_t  uart_tx_in_progress = 0;
volatile uint8_t  uart_frame_header_flag = 1;
volatile uint8_t  config_or_debug = 0;
volatile uint8_t  write_buf_channel_num;
volatile uint8_t  write_buf_read_ptr;
volatile uint16_t command_flags = 0x0000;
volatile uint16_t max_channels_or_scrollers;


#if UART_RX_BUF_LENGTH <= 255 
typedef uint8_t rx_buff_ptr_t;
#else 
typedef uint16_t rx_buff_ptr_t;
#endif
volatile rx_buff_ptr_t read_buf_read_ptr;
volatile rx_buff_ptr_t read_buf_write_ptr;

#if (SCROLLER_MODULE_OUTPUT == 1U)
typedef struct tagtuneScrollerData_t {
	uint8_t status;
	uint16_t contactSize;
	uint16_t position;
}tuneScrollerData_t;
tuneScrollerData_t runtime_scroller_data_arr;
void copy_scroller_run_time_data(uint8_t channel_num);
#endif

sensorData_t runtime_data_arr;

/*******************************************************************************
 * Configuration Macros Parameters
*******************************************************************************/
#define NO_OF_CONFIG_FRAME_ID	 (5u)

#define CONFIG_0_REPEAT			 1u
#define CONFIG_0_PER_CH_LEN	 (10u)
#define CONFIG_0_PTR ((uint8_t*) &proj_config[0])
#define CONFIG_0_LEN ((uint16_t)  PROJECT_CONFIG_DATA_LEN)

#define CONFIG_1_REPEAT			 DEF_NUM_CHANNELS
#define CONFIG_1_PER_CH_LEN		 (sizeof(qtm_acq_avr_da_node_config_t))
#define CONFIG_1_PTR ((uint8_t*) (&ptc_seq_node_cfg1[0].node_xmask))
#define CONFIG_1_LEN ((uint16_t)  (CONFIG_1_PER_CH_LEN * CONFIG_1_REPEAT))

#define CONFIG_2_REPEAT			 DEF_NUM_SENSORS
#define CONFIG_2_PER_CH_LEN		 (sizeof(qtm_touch_key_config_t))
#define CONFIG_2_PTR ((uint8_t*) (&(qtlib_key_configs_set1[0].channel_threshold)))
#define CONFIG_2_LEN ((uint16_t)  (CONFIG_2_PER_CH_LEN * CONFIG_2_REPEAT))

#define CONFIG_3_REPEAT			 1u
#define CONFIG_3_PER_CH_LEN		 (sizeof(qtm_touch_key_group_config_t) - (2U))
#define CONFIG_3_PTR ((uint8_t*) (&qtlib_key_grp_config_set1.sensor_touch_di))
#define CONFIG_3_LEN ((uint8_t)  (CONFIG_3_PER_CH_LEN))

#if SCROLLER_MODULE_OUTPUT == 1u
#define CONFIG_4_REPEAT DEF_NUM_SCROLLERS
#define CONFIG_4_PER_CH_LEN (uint16_t) (sizeof(qtm_scroller_config_t))
#define CONFIG_4_PTR ((uint8_t*) (&qtm_scroller_config1[0]))
#define CONFIG_4_LEN ((uint16_t)  (CONFIG_4_PER_CH_LEN * CONFIG_4_REPEAT))
#endif


/*******************************************************************************
 * Debug Data Configuration
*******************************************************************************/

#define DATA_0_PTR 			((uint8_t*)&runtime_data_arr.signal_t)
#define DATA_0_ID 			(DEBUG_MASK + KEY_DEBUG_DATA_ID)
#define DATA_0_LEN			(uint16_t) sizeof(sensorData_t)
#define DATA_0_REPEAT 		DEF_NUM_CHANNELS
#define DATA_0_FRAME_LEN 	(uint16_t) (DATA_0_LEN * DATA_0_REPEAT)

#if SCROLLER_MODULE_OUTPUT == 1u
#define DATA_1_PTR 		((uint8_t*)&runtime_scroller_data_arr.status)
#define DATA_1_ID 			(DEBUG_MASK + SCROLLER_DEBUG_DATA_ID)
#define DATA_1_LEN			(uint16_t) (sizeof(tuneScrollerData_t))
#define DATA_1_REPEAT 		DEF_NUM_SCROLLERS
#define DATA_1_FRAME_LEN 	(uint16_t) (DATA_1_LEN * DATA_1_REPEAT)
#endif

#define OUTPUT_MODULE_CNT 2u


/* configuration details */

uint8_t read_buffer[UART_RX_BUF_LENGTH];
uint8_t proj_config[PROJECT_CONFIG_DATA_LEN] = {PROTOCOL_VERSION, AVR_DA,(SELF_CAP),(DEF_NUM_CHANNELS),(SENSOR_NODE_CONFIG_MASK | SENSOR_KEY_CONFIG_MASK | COMMON_SENSOR_CONFIG_MASK|SCROLLER_CONFIG_MASK),(0U),(0U),(KEY_DEBUG_MASK|SCROLLER_DEBUG_MASK),(0U),(0U) };  
uint8_t frame_per_channel_len[NO_OF_CONFIG_FRAME_ID] = { CONFIG_0_PER_CH_LEN,CONFIG_1_PER_CH_LEN,CONFIG_2_PER_CH_LEN,CONFIG_3_PER_CH_LEN,CONFIG_4_PER_CH_LEN};				
uint16_t frame_len_lookup[NO_OF_CONFIG_FRAME_ID]  =    { CONFIG_0_LEN,CONFIG_1_LEN,CONFIG_2_LEN,CONFIG_3_LEN,CONFIG_4_LEN};	
uint8_t *configPointerArray[NO_OF_CONFIG_FRAME_ID] = {CONFIG_0_PTR,CONFIG_1_PTR,CONFIG_2_PTR,CONFIG_3_PTR,CONFIG_4_PTR};
uint16_t frame_num_keys_scrollers[NO_OF_CONFIG_FRAME_ID] = {CONFIG_0_REPEAT,CONFIG_1_REPEAT,CONFIG_2_REPEAT,CONFIG_3_REPEAT,CONFIG_4_REPEAT};
static uint8_t frameIdToConfigID[NO_OF_CONFIG_FRAME_ID] = {PROJECT_CONFIG_ID,SENSOR_NODE_CONFIG_ID,SENSOR_KEY_CONFIG_ID,COMMON_SENSOR_CONFIG_ID,SCROLLER_CONFIG_ID};

/* output data details */
uint8_t *debugFramePointerArray[MAX_OUTPUT_MODULES]  = {DATA_0_PTR,DATA_1_PTR};
static uint8_t debug_frame_id[MAX_OUTPUT_MODULES]		  = {DATA_0_ID,DATA_1_ID};
uint8_t debug_frame_data_len[MAX_OUTPUT_MODULES]  = {DATA_0_LEN,DATA_1_LEN};
uint8_t debug_frame_total_len[MAX_OUTPUT_MODULES] = {DATA_0_FRAME_LEN,DATA_1_FRAME_LEN};	
uint8_t debug_num_ch_scroller[MAX_OUTPUT_MODULES] = {DATA_0_REPEAT,DATA_1_REPEAT};
void (*debug_func_ptr[MAX_OUTPUT_MODULES])(uint8_t ch) = {copy_run_time_data,copy_scroller_run_time_data};

uint8_t getDebugIndex(uint8_t debugFramId) {
	uint8_t arrayIndex = 0u;
	for(uint8_t cnt = 0u; cnt < OUTPUT_MODULE_CNT; cnt++) {
		if(debug_frame_id[cnt] == debugFramId){
			arrayIndex = cnt;
			break;
		}
	}
	return arrayIndex;
}

uint8_t getConfigIndex(uint8_t frameid) {
	uint8_t arrayIndex = 0u;
	for(uint8_t cnt = 0u; cnt < NO_OF_CONFIG_FRAME_ID; cnt++) {
		if(frameIdToConfigID[cnt] == frameid){
			arrayIndex = cnt;
			break;
		}
	}
	return arrayIndex;
}

void touchTuneInit(void){
    // ABEIE disabled; DREIE disabled; LBME disabled; RS485 DISABLE; RXCIE enabled; RXSIE enabled; TXCIE enabled; 
    UART_CTRLA_REGISTER = 0xD0;
}



#if (SCROLLER_MODULE_OUTPUT == 1U)
void copy_scroller_run_time_data(uint8_t channel_num)
{
	uint16_t position_temp; //, ref_temp ; 
	uint16_t delta_temp ;	
	uint8_t *temp_ptr = &runtime_scroller_data_arr.status;
 	/* Slider State */	
	
	if(qtm_scroller_control1.qtm_scroller_data[channel_num].scroller_status & 0x01)
		*temp_ptr++ = 1;
	else
		*temp_ptr++ = 0;
			
	/* Slider Delta */
	delta_temp = qtm_scroller_control1.qtm_scroller_data[channel_num].contact_size;
	*temp_ptr++ = (delta_temp & 0xFF);
	*temp_ptr++ = (delta_temp >> 8);
	 
	/* filtered position */
	position_temp = (qtm_scroller_control1.qtm_scroller_data[channel_num].position);//get_scroller_position(channel_num);
	*temp_ptr++ = (position_temp & 0xFF);
	*temp_ptr++ = (position_temp >> 8);
	
}
#endif



void copy_run_time_data(uint8_t channel_num)
{
	uint16_t signal_temp, ref_temp ; int16_t delta_temp ;
	uint8_t *temp_ptr = (uint8_t *) &runtime_data_arr.signal_t;
	
	signal_temp = ptc_qtlib_node_stat1[channel_num].node_acq_signals;
	*temp_ptr++ = (signal_temp & 0xFF); 
	*temp_ptr++ = (signal_temp >> 8);
	
	ref_temp = qtlib_key_data_set1[channel_num].channel_reference;
	*temp_ptr++ = (ref_temp & 0xFF);
	*temp_ptr++ = (ref_temp >> 8);
	
	delta_temp = signal_temp - ref_temp;
	*temp_ptr++ = (delta_temp & 0xFF);
	*temp_ptr++ = (delta_temp >> 8);

	if(qtlib_key_data_set1[channel_num].sensor_state & 0x80) {
		*temp_ptr++ = 1;
	}
	else {
		*temp_ptr++ = 0;
	}
	*temp_ptr++ = (ptc_qtlib_node_stat1[channel_num].node_comp_caps & 0xFF);
	*temp_ptr++ = (ptc_qtlib_node_stat1[channel_num].node_comp_caps >> 8U) ;
}

uint8_t uart_get_char(void)
{
	uint8_t data = read_buffer[read_buf_read_ptr];
	read_buf_read_ptr++;
	if (read_buf_read_ptr == UART_RX_BUF_LENGTH) {
		read_buf_read_ptr = 0;
	}
	return data;
}

void uart_get_string(uint8_t *data_recv, uint16_t len)
{
	for(uint16_t idx = 0; idx < len; idx++)
	{
		*data_recv = uart_get_char();
		data_recv++;		
	}
}

void UART_Write(uint8_t data) {

	UART_TX_REGISTER = data;
}

void uart_send_data_wait(uint8_t data)
{
	uart_tx_in_progress = 1;
	UART_Write(data);
	while (uart_tx_in_progress == 1)
	;
}

void uart_send_data(uint8_t con_or_debug, uint8_t *data_ptr,  uint8_t data_len) {
	if (uart_tx_in_progress == 0) {
		config_or_debug           = con_or_debug;
		uart_tx_in_progress       = 1;
		write_buf_channel_num	  = 1;
		tx_data_ptr			      = data_ptr;
		tx_data_len			      = data_len;
		uint8_t data 			  = tx_data_ptr[0];
		write_buf_read_ptr        = 1;
		UART_Write(data);
	}
}

rx_buff_ptr_t uart_min_num_bytes_received(void)
{
	int16_t retvar =  (read_buf_write_ptr - read_buf_read_ptr);
	if (retvar < 0) 
	{
		retvar = retvar + UART_RX_BUF_LENGTH;
	}
	return (rx_buff_ptr_t)(retvar);
}
void touchTuneNewDataAvailable(void)
{
	command_flags |= SEND_DEBUG_DATA;
}
void uart_send_frame_header(uint8_t trans_type, uint8_t frame,uint16_t frame_len)
{
	uart_frame_header_flag = 0;
	uart_send_data_wait(DV_HEADER);
 	uart_send_data_wait(trans_type);
 	uart_send_data_wait(frame);	
	uart_send_data_wait(frame_len & 0xFF);
	uart_send_data_wait(frame_len>>8);
	uart_frame_header_flag = 1;
}

void uart_recv_frame_data(uint8_t frame_id)
{
	static uint8_t ch_num;
	uint8_t num_data , temp_frame_len;
	uint8_t arrayIndex = getConfigIndex(frame_id);
	

	num_data = uart_min_num_bytes_received();
	temp_frame_len = frame_per_channel_len[arrayIndex];
	uint8_t temp_array[temp_frame_len];
	
	while(num_data > temp_frame_len )
	{   
		for(uint8_t cnt = 0; cnt < temp_frame_len; cnt++) {
			temp_array[cnt] = uart_get_char();
		}
		uint8_t *temp_ptr = configPointerArray[arrayIndex];
		if(frame_num_keys_scrollers[arrayIndex] != 1u){
			temp_ptr += (ch_num*temp_frame_len);		 
		}			
		memcpy(temp_ptr, temp_array, temp_frame_len);
		if((frame_id == SENSOR_NODE_CONFIG_ID) || (frame_id == SENSOR_KEY_CONFIG_ID)) {
			calibrate_node(ch_num);
		}
	
        ch_num++;
		num_data -= (temp_frame_len);

		if(ch_num == frame_num_keys_scrollers[arrayIndex]) {
			ch_num = 0;
			uart_command_info.header_status = HEADER_AWAITING;
			command_flags &= ~( (uint16_t)1 << (frame_id));
			uart_get_char(); // reading footer
			break;
		}
	}
}
void touchTuneProcess(void)
{
	static uint8_t debug_index = 0;
	static uint8_t init_uart = 0;
	uint8_t num_bytes_received;
	uint8_t arrayIndex = 0u;
	num_bytes_received = uart_min_num_bytes_received();

	if(init_uart == 0) {
        touchTuneInit();
		init_uart = 1;
    }

	switch (uart_command_info.header_status) {
		case HEADER_AWAITING:
			if (num_bytes_received > 5u)
			{
				if (uart_get_char() == DV_HEADER)
				{
					uart_get_string( (uint8_t *) &uart_command_info.transaction_type, 4); // uart_command_info.transaction_type ,uart_command_info.frame_id,uart_command_info.num_of_bytes
					uart_command_info.header_status		= DATA_AWAITING;
				}
			}
			break;
		case DATA_AWAITING:
			if(uart_command_info.transaction_type == PC_SEND_CONFIG_DATA_TO_MCU) // user has pressed write to kit
			{
				if(uart_command_info.num_of_bytes >= UART_RX_BUF_LENGTH) {
					uart_recv_frame_data(uart_command_info.frame_id);
				}
				else if (num_bytes_received > uart_command_info.num_of_bytes) //total length of bytes + footer
				{
					command_flags |= (1 << (uart_command_info.frame_id )); // (uart_command_info.frame_id - CONFIG_INFO)
					uart_command_info.header_status	= DATA_RECEIVED;
				}
			}
			else if (uart_command_info.transaction_type == PC_REQUEST_CONFIG_DATA_FROM_MCU) // read from kit
			{
				if(num_bytes_received > 1) // Data length = 1 + footer
				{
					if ((uart_get_char() == ZERO) && (uart_get_char() == DV_FOOTER) ) // requesting configuration
					{
						command_flags |= (1 << (uart_command_info.frame_id )); // (uart_command_info.frame_id - CONFIG_INFO)
						uart_command_info.header_status	= DATA_RECEIVED;
					}
				}
			}
		break;
		case DATA_RECEIVED:
			if((command_flags & 0x0FFF) && (uart_tx_in_progress == 0)) {
				if (uart_command_info.transaction_type == PC_REQUEST_CONFIG_DATA_FROM_MCU) // requesting configuration
				{
					arrayIndex = getConfigIndex(uart_command_info.frame_id);
					uart_send_frame_header(MCU_RESPOND_CONFIG_DATA_TO_PC, uart_command_info.frame_id,frame_len_lookup[arrayIndex]);
					uart_send_data(STREAMING_CONFIG_DATA,configPointerArray[arrayIndex],frame_len_lookup[arrayIndex]);
					uart_command_info.header_status = HEADER_AWAITING;
					command_flags &= ~(1<<uart_command_info.frame_id);
				}
				else if(uart_command_info.transaction_type == PC_SEND_CONFIG_DATA_TO_MCU)
				{
					uart_recv_frame_data(uart_command_info.frame_id);
				}
			}
		break;
		default:
			uart_command_info.header_status = HEADER_AWAITING;
		break;
	}

	/* to send periodic data */
	if((command_flags & SEND_DEBUG_DATA) && (uart_tx_in_progress == 0)) {
		
		while(debug_func_ptr[debug_index] == NULL) {
            debug_index++;
            if(debug_index == MAX_OUTPUT_MODULES) {
                debug_index = 0;
            }
        }

		current_debug_data = debug_frame_id[debug_index];
		
		uart_send_frame_header(MCU_SEND_TUNE_DATA_TO_PC, current_debug_data, debug_frame_total_len[debug_index]);
								
		(debug_func_ptr[debug_index])(0);
		
		max_channels_or_scrollers = debug_num_ch_scroller[debug_index];
		
		uart_send_data(STREAMING_DEBUG_DATA, debugFramePointerArray[debug_index], debug_frame_data_len[debug_index]);
		
 		debug_index++;
		
 		if(debug_index == MAX_OUTPUT_MODULES) {
 			debug_index = 0;
 		}
	}
}

#endif

void touchUartTxComplete(void)
{
	#if (DEF_TOUCH_DATA_STREAMER_ENABLE == 1u)
	uint8_t arrayIndex = 0u;
	// USART TX complete interrupt
	UART_STATUS_REGISTER |= (1 << USART_TXCIF_bp);

	if (uart_frame_header_flag != 1)
	{
		uart_tx_in_progress = 0;
	} 
	else 
	{
		if (write_buf_read_ptr < tx_data_len )
		{
			UART_Write(tx_data_ptr[write_buf_read_ptr]);
			write_buf_read_ptr++;
		} else {
			if(config_or_debug == STREAMING_CONFIG_DATA) {
				UART_Write(DV_FOOTER);
				uart_tx_in_progress		= 0;
			} else if(config_or_debug == STREAMING_DEBUG_DATA) {
				/* per channel data are sent channel by channel to reduce RAM requirements */
				if (write_buf_channel_num < max_channels_or_scrollers)
				{
					arrayIndex = getDebugIndex(current_debug_data);
					(*debug_func_ptr[arrayIndex])(write_buf_channel_num);  
					write_buf_read_ptr = 1;
					write_buf_channel_num++;
					UART_Write(tx_data_ptr[0]);
				}
				else if(write_buf_channel_num == max_channels_or_scrollers)
				{
					write_buf_channel_num++;
					command_flags &= ~(SEND_DEBUG_DATA); 
					UART_Write(DV_FOOTER);
				}
				else
				{
					uart_tx_in_progress = 0;
				}
			}
		}
	}
	#endif
}

void touchUartRxComplete(void)
{
	#if (DEF_TOUCH_DATA_STREAMER_ENABLE == 1u)
	read_buffer[read_buf_write_ptr] = UART_RX_REGISTER;
	read_buf_write_ptr++;
	if (read_buf_write_ptr == UART_RX_BUF_LENGTH) {
		read_buf_write_ptr = 0;
	}
	#endif
}

/* Interrupt service routine for RX complete */
ISR(USART1_RXC_vect)
{
    touchUartRxComplete();
}
/* Interrupt service routine for TX complete */
ISR(USART1_TXC_vect)
{
    touchUartTxComplete();
}
