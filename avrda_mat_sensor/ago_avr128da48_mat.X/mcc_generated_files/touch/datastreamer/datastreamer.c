/*******************************************************************************
  Touch Library 3.5.0 Release

  @Company
    Microchip Technology Inc.

  @File Name
    datastreamer.c

  @Summary
    QTouch Modular Library

  @Description
    Provides the datastreamer protocol implementation, transmission of
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
#include "datastreamer.h"
#include "../../system/system.h"
#include "../../../mat_decode.h"
#include "../../../amazon.h"
#include "../../../app_object_counter.h"

#if (DEF_TOUCH_DATA_STREAMER_ENABLE == 1u)

/*----------------------------------------------------------------------------
 *     defines
 *----------------------------------------------------------------------------*/
#define ACQ_MODULE_AUTOTUNE_OUTPUT 0
#define FREQ_HOP_AUTO_MODULE_OUTPUT 0
#define SCROLLER_MODULE_OUTPUT 0

#define SURFACE_MODULE_OUTPUT 0


/*----------------------------------------------------------------------------
  global variables
----------------------------------------------------------------------------*/
extern qtm_acquisition_control_t qtlib_acq_set1;
extern qtm_touch_key_control_t   qtlib_key_set1;
extern qtm_touch_key_config_t    qtlib_key_configs_set1[DEF_NUM_SENSORS];

extern uint8_t module_error_code;

uint8_t data[] = {
    0x5F, 0xB4, 0x00, 0x86, 0x4A, 0x03, 0xEB, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xAA, 0x55, 0x01, 0x6E, 0xA0};

/*----------------------------------------------------------------------------
  prototypes
----------------------------------------------------------------------------*/
void datastreamer_transmit(uint8_t data);

/*----------------------------------------------------------------------------
 *   function definitions
 *----------------------------------------------------------------------------*/

/*============================================================================
void datastreamer_init(void)
------------------------------------------------------------------------------
Purpose: Initialization for datastreamer module
Input  : none
Output : none
Notes  :
============================================================================*/
void datastreamer_init(void)
{
}

/*============================================================================
void datastreamer_transmit(uint8_t data_byte)
------------------------------------------------------------------------------
Purpose: Transmits the single byte through the configured UART port.
Input  : Byte to be transmitted
Output : none
Notes  :
============================================================================*/
void datastreamer_transmit(uint8_t data_byte)
{
  while(!USART1_IsTxReady());
	/* Write the data bye */
  USART1_Write(data_byte);
}

/*============================================================================
void datastreamer_output(void)
------------------------------------------------------------------------------
Purpose: Forms the datastreamer frame based on the configured modules, Tranmits
         the frame as single packet through UART port.
Input  : none
Output : none
Notes  :
============================================================================*/
void mat_datastreamer_output(void)
{
	int16_t           i, temp_int_calc;
	static uint8_t    sequence = 0u;
	uint16_t          u16temp_output;
	uint8_t           u8temp_output, send_header;
	volatile uint16_t count_bytes_out;
    
	send_header = sequence & (0x0f);
	if (send_header == 0) {
		for (i = 0; i < sizeof(data); i++) {
			datastreamer_transmit(data[i]);
		}
	}

	// Start token
	datastreamer_transmit(0x55);

	// Frame Start
	datastreamer_transmit(sequence);

	for (count_bytes_out = 0u; count_bytes_out < DEF_NUM_CHANNELS; count_bytes_out++) {
		/* Signals */
		u16temp_output = get_sensor_node_signal(count_bytes_out);
		datastreamer_transmit((uint8_t)u16temp_output);
		datastreamer_transmit((uint8_t)(u16temp_output >> 8u));
        
        /* mat reference */
        u16temp_output = mat_decode_data[count_bytes_out].channel_reference;
		datastreamer_transmit((uint8_t)u16temp_output);
		datastreamer_transmit((uint8_t)(u16temp_output >> 8u));

		/* Reference */
//		u16temp_output = get_sensor_node_reference(count_bytes_out);        // keys reference
//		datastreamer_transmit((uint8_t)u16temp_output);
//		datastreamer_transmit((uint8_t)(u16temp_output >> 8u));

		/* Touch delta */
		temp_int_calc = get_sensor_node_signal(count_bytes_out);
        temp_int_calc -= mat_decode_data[count_bytes_out].channel_reference;
//		temp_int_calc -= get_sensor_node_reference(count_bytes_out);        // keys reference
		u16temp_output = (uint16_t)(temp_int_calc);
		datastreamer_transmit((uint8_t)u16temp_output);
		datastreamer_transmit((uint8_t)(u16temp_output >> 8u));

		/* Comp Caps */
		u16temp_output = get_sensor_cc_val(count_bytes_out);
		datastreamer_transmit((uint8_t)u16temp_output);
		datastreamer_transmit((uint8_t)(u16temp_output >> 8u));

#if (ACQ_MODULE_AUTOTUNE_OUTPUT == 1)

#if (DEF_PTC_CAL_OPTION == CAL_AUTO_TUNE_CSD)
		/* CSD */
		u8temp_output = qtlib_acq_set1.qtm_acq_node_config[count_bytes_out].node_csd;
		datastreamer_transmit(u8temp_output);
#else
		/* Prescalar */
		u8temp_output = NODE_PRSC(qtlib_acq_set1.qtm_acq_node_config[count_bytes_out].node_rsel_prsc);
		datastreamer_transmit(u8temp_output);
#endif

#endif
		/* State */
        u8temp_output = mat_decode_data[count_bytes_out].lane_state;
        if(u8temp_output == IDLE){
            datastreamer_transmit(0x01);
        }
        else if(u8temp_output == POS_DETECT)
        {
            datastreamer_transmit(0x02);
        }
        else if(u8temp_output == NEG_DETECT){
            datastreamer_transmit(0x00);
        }
        
        /* Decode State */
        u8temp_output = mat_decode_data[count_bytes_out].decode_state;
        datastreamer_transmit(u8temp_output);

		/* Threshold */
//		datastreamer_transmit(qtlib_key_configs_set1[count_bytes_out].channel_threshold);
        
        /* Positive threshold */
        temp_int_calc = (int16_t)(mat_decode_data[count_bytes_out].channel_pos_threshold);
		datastreamer_transmit((uint8_t)temp_int_calc);
		datastreamer_transmit((uint8_t)(temp_int_calc >> 8u));
        /* Positive hysteresis value */
        temp_int_calc = (int16_t)(mat_decode_data[count_bytes_out].channel_pos_hysteresis);
		datastreamer_transmit((uint8_t)temp_int_calc);
		datastreamer_transmit((uint8_t)(temp_int_calc >> 8u));
        
        /* Negative threshold */
        temp_int_calc = (int16_t)(-mat_decode_data[count_bytes_out].channel_neg_threshold);
		datastreamer_transmit((uint8_t)temp_int_calc);
		datastreamer_transmit((uint8_t)(temp_int_calc >> 8u));
        /* Negative hysteresis value */
        temp_int_calc = (int16_t)(-mat_decode_data[count_bytes_out].channel_neg_hysteresis);
		datastreamer_transmit((uint8_t)temp_int_calc);
		datastreamer_transmit((uint8_t)(temp_int_calc >> 8u));

        /* Object counter values */
        temp_int_calc = (int16_t) (shelf_data[count_bytes_out].lane_object_counter);
        datastreamer_transmit((uint8_t) temp_int_calc);
        datastreamer_transmit((uint8_t) (temp_int_calc >> 8u));
	}

#if (SCROLLER_MODULE_OUTPUT == 1)

	for (count_bytes_out = 0u; count_bytes_out < qtm_scroller_control1.qtm_scroller_group_config->num_scrollers;
	     count_bytes_out++) {

		/* State */
		u8temp_output = qtm_scroller_control1.qtm_scroller_data[count_bytes_out].scroller_status;
		if (0u != (u8temp_output & 0x01)) {
			datastreamer_transmit(0x01);
		} else {
			datastreamer_transmit(0x00);
		}

		/* Delta */
		u16temp_output = qtm_scroller_control1.qtm_scroller_data[count_bytes_out].contact_size;
		datastreamer_transmit((uint8_t)u16temp_output);
		datastreamer_transmit((uint8_t)(u16temp_output >> 8u));

		/* Threshold */
		u16temp_output = qtm_scroller_control1.qtm_scroller_config[count_bytes_out].contact_min_threshold;
		datastreamer_transmit((uint8_t)u16temp_output);
		datastreamer_transmit((uint8_t)(u16temp_output >> 8u));

		/* filtered position */
		u16temp_output = qtm_scroller_control1.qtm_scroller_data[count_bytes_out].position;
		datastreamer_transmit((uint8_t)(u16temp_output & 0x00FFu));
		datastreamer_transmit((uint8_t)((u16temp_output & 0xFF00u) >> 8u));
	}

#endif

#if (FREQ_HOP_AUTO_MODULE_OUTPUT == 1)

	/* Frequency selection - from acq module */
	datastreamer_transmit(qtlib_acq_set1.qtm_acq_node_group_config->freq_option_select);

	for (count_bytes_out = 0u; count_bytes_out < qtm_freq_hop_autotune_control1.qtm_freq_hop_autotune_config->num_freqs; count_bytes_out++) {
		/* Frequencies */
		datastreamer_transmit(qtm_freq_hop_autotune_control1.qtm_freq_hop_autotune_config->median_filter_freq[count_bytes_out]);
	}
#endif

	/* Other Debug Parameters */
	datastreamer_transmit(module_error_code);
    
	/* Frame End */
	datastreamer_transmit(sequence++);

	/* End token */
	datastreamer_transmit(~0x55);
}

/*============================================================================
void datastreamer_output(void)
------------------------------------------------------------------------------
Purpose: Forms the datastreamer frame based on the configured modules, Tranmits
         the frame as single packet through UART port.
Input  : none
Output : none
Notes  :
============================================================================*/
void datastreamer_output(void)
{
	int16_t           i, temp_int_calc;
	static uint8_t    sequence = 0u;
	uint16_t          u16temp_output;
	uint8_t           u8temp_output, send_header;
	volatile uint16_t count_bytes_out;

	send_header = sequence & (0x0f);
	if (send_header == 0) {
		for (i = 0; i < sizeof(data); i++) {
			datastreamer_transmit(data[i]);
		}
	}

	// Start token
	datastreamer_transmit(0x55);

	// Frame Start
	datastreamer_transmit(sequence);

	for (count_bytes_out = 0u; count_bytes_out < DEF_NUM_CHANNELS; count_bytes_out++) {
		/* Signals */
		u16temp_output = get_sensor_node_signal(count_bytes_out);
		datastreamer_transmit((uint8_t)u16temp_output);
		datastreamer_transmit((uint8_t)(u16temp_output >> 8u));

		/* Reference */
		u16temp_output = get_sensor_node_reference(count_bytes_out);
		datastreamer_transmit((uint8_t)u16temp_output);
		datastreamer_transmit((uint8_t)(u16temp_output >> 8u));

		/* Touch delta */
		temp_int_calc = get_sensor_node_signal(count_bytes_out);
		temp_int_calc -= get_sensor_node_reference(count_bytes_out);
		u16temp_output = (uint16_t)(temp_int_calc);
		datastreamer_transmit((uint8_t)u16temp_output);
		datastreamer_transmit((uint8_t)(u16temp_output >> 8u));

		/* Comp Caps */
		u16temp_output = get_sensor_cc_val(count_bytes_out);
		datastreamer_transmit((uint8_t)u16temp_output);
		datastreamer_transmit((uint8_t)(u16temp_output >> 8u));

#if (ACQ_MODULE_AUTOTUNE_OUTPUT == 1)

#if (DEF_PTC_CAL_OPTION == CAL_AUTO_TUNE_CSD)
		/* CSD */
		u8temp_output = qtlib_acq_set1.qtm_acq_node_config[count_bytes_out].node_csd;
		datastreamer_transmit(u8temp_output);
#else
		/* Prescalar */
		u8temp_output = NODE_PRSC(qtlib_acq_set1.qtm_acq_node_config[count_bytes_out].node_rsel_prsc);
		datastreamer_transmit(u8temp_output);
#endif

#endif
		/* State */
		u8temp_output = get_sensor_state(count_bytes_out);
		if (0u != (u8temp_output & 0x80)) {
			datastreamer_transmit(0x01);
		} else {
			datastreamer_transmit(0x00);
		}

		/* Threshold */
		datastreamer_transmit(qtlib_key_configs_set1[count_bytes_out].channel_threshold);
	}

#if (SCROLLER_MODULE_OUTPUT == 1)

	for (count_bytes_out = 0u; count_bytes_out < qtm_scroller_control1.qtm_scroller_group_config->num_scrollers;
	     count_bytes_out++) {

		/* State */
		u8temp_output = qtm_scroller_control1.qtm_scroller_data[count_bytes_out].scroller_status;
		if (0u != (u8temp_output & 0x01)) {
			datastreamer_transmit(0x01);
		} else {
			datastreamer_transmit(0x00);
		}

		/* Delta */
		u16temp_output = qtm_scroller_control1.qtm_scroller_data[count_bytes_out].contact_size;
		datastreamer_transmit((uint8_t)u16temp_output);
		datastreamer_transmit((uint8_t)(u16temp_output >> 8u));

		/* Threshold */
		u16temp_output = qtm_scroller_control1.qtm_scroller_config[count_bytes_out].contact_min_threshold;
		datastreamer_transmit((uint8_t)u16temp_output);
		datastreamer_transmit((uint8_t)(u16temp_output >> 8u));

		/* filtered position */
		u16temp_output = qtm_scroller_control1.qtm_scroller_data[count_bytes_out].position;
		datastreamer_transmit((uint8_t)(u16temp_output & 0x00FFu));
		datastreamer_transmit((uint8_t)((u16temp_output & 0xFF00u) >> 8u));
	}

#endif

#if (FREQ_HOP_AUTO_MODULE_OUTPUT == 1)

	/* Frequency selection - from acq module */
	datastreamer_transmit(qtlib_acq_set1.qtm_acq_node_group_config->freq_option_select);

	for (count_bytes_out = 0u; count_bytes_out < qtm_freq_hop_autotune_control1.qtm_freq_hop_autotune_config->num_freqs; count_bytes_out++) {
		/* Frequencies */
		datastreamer_transmit(qtm_freq_hop_autotune_control1.qtm_freq_hop_autotune_config->median_filter_freq[count_bytes_out]);
	}
#endif

	/* Other Debug Parameters */
	datastreamer_transmit(module_error_code);

	/* Frame End */
	datastreamer_transmit(sequence++);

	/* End token */
	datastreamer_transmit(~0x55);
}

#endif
