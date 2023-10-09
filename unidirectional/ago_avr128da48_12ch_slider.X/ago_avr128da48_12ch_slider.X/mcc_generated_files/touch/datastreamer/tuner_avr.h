
#ifndef __TUNER_AVR_H_       
#define __TUNER_AVR_H_

#include "../touch.h"

#if DEF_TOUCH_DATA_STREAMER_ENABLE == 1U

#define DV_HEADER    0x55 
#define DV_FOOTER    0xAA 

#define UART_RX_BUF_LENGTH 60u

#define HEADER_AWAITING 0u
#define HEADER_RECEIVED 1u
#define DATA_AWAITING 2u
#define DATA_RECEIVED 3u
 

#define SEND_DEBUG_DATA		 0x8000

#define ZERO 0x00  

#define DEBUG_DATA_FRAME_BASE_VAL 0x80

typedef enum
{
	PC_REQUEST_CONFIG_DATA_FROM_MCU		= 0x01,		// sw read PC_REQUEST_CONFIG_DATA_FROM_MCU
	PC_SEND_CONFIG_DATA_TO_MCU			= 0x02,		// sw write	PC_SEND_CONFIG_DATA_TO_MCU
	MCU_SEND_TUNE_DATA_TO_PC			= 0x03,		// send debug data MCU_SEND_TUNE_DATA_TO_PC
	MCU_RESPOND_CONFIG_DATA_TO_PC		= 0x04 		// sw read MCU_RESPOND_CONFIG_DATA_TO_PC
}TYPE_ID_VALUES;

typedef enum
{
	ATtiny161x_321x	   = 0x31,
	AVR_DA			   = 0x32,
	SAMD2x_D1x_L21     = 0x33,
	SAMC2x             = 0x34,
	SAME5x             = 0x35,
	SAMl1x_PIC32CMLE   = 0x36,
	saml22 			   = 0x37,
	pic32cvd		   = 0x38,
	ATtiny81x		   = 0x3A,
	PIC_ADCC_12bit 	   = 0x51,
	PIC_ADCC_10bit     = 0x52
}DEVICE_TYPE;

/***********
 * Config data mask and ids
*/

#define PROJECT_CONFIG_ID  0u
#define SENSOR_NODE_CONFIG_ID 1u
#define SENSOR_KEY_CONFIG_ID 2u
#define COMMON_SENSOR_CONFIG_ID 3u
#define SCROLLER_CONFIG_ID 4u
#define FREQ_HOPPING_AUTO_TUNE_ID 5u
#define SURFACE_CONFIG_ID 6u
#define GESTURE_CONFIG_ID 7u


#define PROJECT_CONFIG_MASK  ((uint8_t) 1<<PROJECT_CONFIG_ID)
#define SENSOR_NODE_CONFIG_MASK ((uint8_t) 1<<(SENSOR_NODE_CONFIG_ID-1))
#define SENSOR_KEY_CONFIG_MASK ((uint8_t) 1<<(SENSOR_KEY_CONFIG_ID-1))
#define COMMON_SENSOR_CONFIG_MASK ((uint8_t) 1<<(COMMON_SENSOR_CONFIG_ID-1))
#define SCROLLER_CONFIG_MASK ((uint8_t) 1<<(SCROLLER_CONFIG_ID-1))
#define FREQ_HOPPING_AUTO_TUNE_MASK ((uint8_t) 1<<(FREQ_HOPPING_AUTO_TUNE_ID-1))
#define SURFACE_CONFIG_MASK ((uint8_t) 1<<(SURFACE_CONFIG_ID-1))
#define GESTURE_CONFIG_MASK ((uint8_t) 1<<(GESTURE_CONFIG_ID-1))

/***********
* debug data mask and ids
*/

#define DEBUG_MASK 0x80u

#define KEY_DEBUG_DATA_ID 		0u
#define SCROLLER_DEBUG_DATA_ID	1u
#define FREQ_HOP_AUTO_TUNE_DATA_ID 		2u
#define SURFACE_DEBUG_DATA_ID 		3u
#define GESTURE_DEBUG_DATA_ID 		4u

#define KEY_DEBUG_MASK ((uint8_t)1<<KEY_DEBUG_DATA_ID)
#define SCROLLER_DEBUG_MASK ((uint8_t)1<<SCROLLER_DEBUG_DATA_ID)
#define FREQ_HOP_AUTO_TUNE_DEBUG_MASK ((uint8_t)1<<FREQ_HOP_AUTO_TUNE_DATA_ID)
#define SURFACE_DEBUG_MASK ((uint8_t)1<<SURFACE_DEBUG_DATA_ID)
#define GESTURE_DEBUG_MASK ((uint8_t)1<<GESTURE_DEBUG_DATA_ID)

typedef enum
{
	SELF_CAP = 0x00,
	MUTUAL_CAP = 0x01
}ACQ_METHOD;

typedef enum
{
	PROTOCOL_VERSION = 0x02		// 0x00000010b - msb 5 bits - Minor version (00000b), lsb first 3 bits - Major version (010b)
}VERSION;

void touchTuneProcess(void);
void touchUartRxComplete(void);
void touchUartTxComplete(void);
void touchTuneInit(void);
void touchTuneNewDataAvailable(void);



#endif

#endif /* __TUNER_AVR_H_ */