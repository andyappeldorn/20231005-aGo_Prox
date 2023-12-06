/*******************************************************************************
  Touch Library 3.5.0 Release

  @Company
    Microchip Technology Inc.

  @File Name
    touch.h

  @Summary
    QTouch Modular Library

  @Description
    Provides configuation macros for touch library.
	
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

#ifndef TOUCH_H
#define TOUCH_H

// DOM-IGNORE-BEGIN
#ifdef __cplusplus  // Provide C++ Compatibility

    extern "C" {

#endif
// DOM-IGNORE-END

/*----------------------------------------------------------------------------
 *     include files
 *----------------------------------------------------------------------------*/

#include "include/touch_api_ptc.h"
#include "../../amazon.h"

/**********************************************************/
/******************* Acquisition controls *****************/
/**********************************************************/
/* Defines the Measurement Time in milli seconds.
 * Range: 1 to 255.
 * Default value: 20.
 */
#define DEF_TOUCH_MEASUREMENT_PERIOD_MS 10

/* Defines the Type of sensor
 * Default value: NODE_MUTUAL.
 */

#define DEF_SENSOR_TYPE NODE_SELFCAP_SHIELD
/* Set sensor calibration mode for charge share delay (CSD) ,Prescaler or series resistor.
 * Range: CAL_AUTO_TUNE_NONE / CAL_AUTO_TUNE_RSEL / CAL_AUTO_TUNE_PRSC / CAL_AUTO_TUNE_CSD
 * Default value: CAL_AUTO_TUNE_NONE.
 */

#define DEF_PTC_CAL_OPTION   CAL_AUTO_TUNE_NONE

/* Calibration option to ensure full charge transfer */
/* Bits 7:0 = XX | TT SELECT_TAU | X | CAL_OPTION */
#define DEF_PTC_TAU_TARGET CAL_CHRG_5TAU
#define DEF_PTC_CAL_AUTO_TUNE (uint8_t)((DEF_PTC_TAU_TARGET << CAL_CHRG_TIME_POS) | DEF_PTC_CAL_OPTION)

/* Set default bootup acquisition frequency.
 * Range: FREQ_SEL_0 - FREQ_SEL_15 , FREQ_SEL_SPREAD
 * Default value: FREQ_SEL_0.
 */
#define DEF_SEL_FREQ_INIT FREQ_SEL_0

/*----------------------------------------------------------------------------
 *     defines
 *----------------------------------------------------------------------------*/

/**********************************************************/
/***************** Node Params   ******************/
/**********************************************************/
/* Acquisition Set 1 */
/* Defines the number of sensor nodes in the acquisition set
 * Range: 1 to 65535.
 * Default value: 1
 */
#ifdef USE_10CH_WHITE_SENSOR
#define DEF_NUM_CHANNELS (10)

/* Defines node parameter setting
 * {X-line, Y-line, Charge Share Delay(CSD), NODE_RSEL_PRSC(series resistor, prescaler), NODE_G(Analog Gain , Digital Gain),
 * filter level}
 */
#define NODE_0_PARAMS                                                                                               \
{                                                                                                                  \
    Y(22)|Y(6)|Y(5)|Y(4)|Y(18)|Y(17)|Y(16)|Y(21)|Y(20)|Y(19),  Y(7), SENSOR_CSD, PRSC_DIV_SEL_12, NODE_GAIN(A_GAIN, D_GAIN), COMMON_FILTER_LEVEL                  \
}
#define NODE_1_PARAMS                                                                                               \
{                                                                                                                  \
   Y(22)|Y(7)|Y(5)|Y(4)|Y(18)|Y(17)|Y(16)|Y(21)|Y(20)|Y(19),  Y(6), SENSOR_CSD, PRSC_DIV_SEL_12, NODE_GAIN(A_GAIN, D_GAIN), COMMON_FILTER_LEVEL                  \
}
#define NODE_2_PARAMS                                                                                               \
{                                                                                                                  \
   Y(22)|Y(7)|Y(6)|Y(4)|Y(18)|Y(17)|Y(16)|Y(21)|Y(20)|Y(19),  Y(5), SENSOR_CSD, PRSC_DIV_SEL_12, NODE_GAIN(A_GAIN, D_GAIN), COMMON_FILTER_LEVEL                  \
}
#define NODE_3_PARAMS                                                                                               \
{                                                                                                                  \
   Y(22)|Y(7)|Y(6)|Y(5)|Y(18)|Y(17)|Y(16)|Y(21)|Y(20)|Y(19),  Y(4), SENSOR_CSD, PRSC_DIV_SEL_12, NODE_GAIN(A_GAIN, D_GAIN), COMMON_FILTER_LEVEL                  \
}
#define NODE_4_PARAMS                                                                                               \
{                                                                                                                  \
   Y(22)|Y(7)|Y(6)|Y(5)|Y(4)|Y(17)|Y(16)|Y(21)|Y(20)|Y(19),  Y(18), SENSOR_CSD, PRSC_DIV_SEL_12, NODE_GAIN(A_GAIN, D_GAIN), COMMON_FILTER_LEVEL                  \
}
#define NODE_5_PARAMS                                                                                               \
{                                                                                                                  \
   Y(22)|Y(7)|Y(6)|Y(5)|Y(4)|Y(18)|Y(16)|Y(21)|Y(20)|Y(19),  Y(17), SENSOR_CSD, PRSC_DIV_SEL_12, NODE_GAIN(A_GAIN, D_GAIN), COMMON_FILTER_LEVEL                  \
}
#define NODE_6_PARAMS                                                                                               \
{                                                                                                                  \
   Y(22)|Y(7)|Y(6)|Y(5)|Y(4)|Y(18)|Y(17)|Y(21)|Y(20)|Y(19),  Y(16), SENSOR_CSD, PRSC_DIV_SEL_12, NODE_GAIN(A_GAIN, D_GAIN), COMMON_FILTER_LEVEL                  \
}
#define NODE_7_PARAMS                                                                                               \
{                                                                                                                  \
   Y(22)|Y(7)|Y(6)|Y(5)|Y(4)|Y(18)|Y(17)|Y(16)|Y(20)|Y(19),  Y(21), SENSOR_CSD, PRSC_DIV_SEL_12, NODE_GAIN(A_GAIN, D_GAIN), COMMON_FILTER_LEVEL                  \
}
#define NODE_8_PARAMS                                                                                               \
{                                                                                                                  \
   Y(22)|Y(7)|Y(6)|Y(5)|Y(4)|Y(18)|Y(17)|Y(16)|Y(21)|Y(19),  Y(20), SENSOR_CSD, PRSC_DIV_SEL_12, NODE_GAIN(A_GAIN, D_GAIN), COMMON_FILTER_LEVEL                  \
}
#define NODE_9_PARAMS                                                                                               \
{                                                                                                                  \
   Y(22)|Y(7)|Y(6)|Y(5)|Y(4)|Y(18)|Y(17)|Y(16)|Y(21)|Y(20),  Y(19), SENSOR_CSD, PRSC_DIV_SEL_12, NODE_GAIN(A_GAIN, D_GAIN), COMMON_FILTER_LEVEL                  \
}
#endif

#ifdef USE_32CH_INTERPOSER_SENSOR
// use pin configuration for 32 Channel Interposer Board
#define DEF_NUM_CHANNELS (16)
        
    /* Defines node parameter setting
     * {X-line, Y-line, Charge Share Delay(CSD), NODE_RSEL_PRSC(series resistor, prescaler), NODE_G(Analog Gain , Digital Gain),
     * filter level}
     */
#define NODE_0_PARAMS                                                                                               \
{                                                                                                                  \
   Y(37)|Y(1)|Y(2)|Y(3)|Y(4)|Y(5)|Y(6)|Y(7)|Y(8)|Y(9)|Y(10)|Y(11)|Y(12)|Y(13)|Y(16)|Y(17),  Y(0), 15, PRSC_DIV_SEL_8, NODE_GAIN(A_GAIN, D_GAIN), COMMON_FILTER_LEVEL                  \
}
#define NODE_1_PARAMS                                                                                               \
{                                                                                                                  \
   Y(37)|Y(0)|Y(2)|Y(3)|Y(4)|Y(5)|Y(6)|Y(7)|Y(8)|Y(9)|Y(10)|Y(11)|Y(12)|Y(13)|Y(16)|Y(17),  Y(1), 15, PRSC_DIV_SEL_8, NODE_GAIN(A_GAIN, D_GAIN), COMMON_FILTER_LEVEL                  \
}
#define NODE_2_PARAMS                                                                                               \
{                                                                                                                  \
   Y(37)|Y(0)|Y(1)|Y(3)|Y(4)|Y(5)|Y(6)|Y(7)|Y(8)|Y(9)|Y(10)|Y(11)|Y(12)|Y(13)|Y(16)|Y(17),  Y(2), 15, PRSC_DIV_SEL_8, NODE_GAIN(A_GAIN, D_GAIN), COMMON_FILTER_LEVEL                  \
}
#define NODE_3_PARAMS                                                                                               \
{                                                                                                                  \
   Y(37)|Y(0)|Y(1)|Y(2)|Y(4)|Y(5)|Y(6)|Y(7)|Y(8)|Y(9)|Y(10)|Y(11)|Y(12)|Y(13)|Y(16)|Y(17),  Y(3), 15, PRSC_DIV_SEL_8, NODE_GAIN(A_GAIN, D_GAIN), COMMON_FILTER_LEVEL                  \
}
#define NODE_4_PARAMS                                                                                               \
{                                                                                                                  \
   Y(37)|Y(0)|Y(1)|Y(2)|Y(3)|Y(5)|Y(6)|Y(7)|Y(8)|Y(9)|Y(10)|Y(11)|Y(12)|Y(13)|Y(16)|Y(17),  Y(4), 15, PRSC_DIV_SEL_8, NODE_GAIN(A_GAIN, D_GAIN), COMMON_FILTER_LEVEL                  \
}
#define NODE_5_PARAMS                                                                                               \
{                                                                                                                  \
   Y(37)|Y(0)|Y(1)|Y(2)|Y(3)|Y(4)|Y(6)|Y(7)|Y(8)|Y(9)|Y(10)|Y(11)|Y(12)|Y(13)|Y(16)|Y(17),  Y(5), 15, PRSC_DIV_SEL_8, NODE_GAIN(A_GAIN, D_GAIN), COMMON_FILTER_LEVEL                  \
}
#define NODE_6_PARAMS                                                                                               \
{                                                                                                                  \
   Y(37)|Y(0)|Y(1)|Y(2)|Y(3)|Y(4)|Y(5)|Y(7)|Y(8)|Y(9)|Y(10)|Y(11)|Y(12)|Y(13)|Y(16)|Y(17),  Y(6), 15, PRSC_DIV_SEL_8, NODE_GAIN(A_GAIN, D_GAIN), COMMON_FILTER_LEVEL                  \
}
#define NODE_7_PARAMS                                                                                               \
{                                                                                                                  \
   Y(37)|Y(0)|Y(1)|Y(2)|Y(3)|Y(4)|Y(5)|Y(6)|Y(8)|Y(9)|Y(10)|Y(11)|Y(12)|Y(13)|Y(16)|Y(17),  Y(7), 15, PRSC_DIV_SEL_8, NODE_GAIN(A_GAIN, D_GAIN), COMMON_FILTER_LEVEL                  \
}
#define NODE_8_PARAMS                                                                                               \
{                                                                                                                  \
   Y(37)|Y(0)|Y(1)|Y(2)|Y(3)|Y(4)|Y(5)|Y(6)|Y(7)|Y(9)|Y(10)|Y(11)|Y(12)|Y(13)|Y(16)|Y(17),  Y(8), 15, PRSC_DIV_SEL_8, NODE_GAIN(A_GAIN, D_GAIN), COMMON_FILTER_LEVEL                  \
}
#define NODE_9_PARAMS                                                                                               \
{                                                                                                                  \
   Y(37)|Y(0)|Y(1)|Y(2)|Y(3)|Y(4)|Y(5)|Y(6)|Y(7)|Y(8)|Y(10)|Y(11)|Y(12)|Y(13)|Y(16)|Y(17),  Y(9), 15, PRSC_DIV_SEL_8, NODE_GAIN(A_GAIN, D_GAIN), COMMON_FILTER_LEVEL                  \
}
#define NODE_10_PARAMS                                                                                               \
{                                                                                                                  \
   Y(37)|Y(0)|Y(1)|Y(2)|Y(3)|Y(4)|Y(5)|Y(6)|Y(7)|Y(8)|Y(9)|Y(11)|Y(12)|Y(13)|Y(16)|Y(17),  Y(10), 15, PRSC_DIV_SEL_8, NODE_GAIN(A_GAIN, D_GAIN), COMMON_FILTER_LEVEL                  \
}
#define NODE_11_PARAMS                                                                                               \
{                                                                                                                  \
   Y(37)|Y(0)|Y(1)|Y(2)|Y(3)|Y(4)|Y(5)|Y(6)|Y(7)|Y(8)|Y(9)|Y(10)|Y(12)|Y(13)|Y(16)|Y(17),  Y(11), 15, PRSC_DIV_SEL_8, NODE_GAIN(A_GAIN, D_GAIN), COMMON_FILTER_LEVEL                  \
}
#define NODE_12_PARAMS                                                                                               \
{                                                                                                                  \
   Y(37)|Y(0)|Y(1)|Y(2)|Y(3)|Y(4)|Y(5)|Y(6)|Y(7)|Y(8)|Y(9)|Y(10)|Y(11)|Y(13)|Y(16)|Y(17),  Y(12), 15, PRSC_DIV_SEL_8, NODE_GAIN(A_GAIN, D_GAIN), COMMON_FILTER_LEVEL                  \
}
#define NODE_13_PARAMS                                                                                               \
{                                                                                                                  \
   Y(37)|Y(0)|Y(1)|Y(2)|Y(3)|Y(4)|Y(5)|Y(6)|Y(7)|Y(8)|Y(9)|Y(10)|Y(11)|Y(12)|Y(16)|Y(17),  Y(13), 15, PRSC_DIV_SEL_8, NODE_GAIN(A_GAIN, D_GAIN), COMMON_FILTER_LEVEL                  \
}
#define NODE_14_PARAMS                                                                                               \
{                                                                                                                  \
   Y(37)|Y(0)|Y(1)|Y(2)|Y(3)|Y(4)|Y(5)|Y(6)|Y(7)|Y(8)|Y(9)|Y(10)|Y(11)|Y(12)|Y(13)|Y(17),  Y(16), 15, PRSC_DIV_SEL_8, NODE_GAIN(A_GAIN, D_GAIN), COMMON_FILTER_LEVEL                  \
}
#define NODE_15_PARAMS                                                                                               \
{                                                                                                                  \
   Y(37)|Y(0)|Y(1)|Y(2)|Y(3)|Y(4)|Y(5)|Y(6)|Y(7)|Y(8)|Y(9)|Y(10)|Y(11)|Y(12)|Y(13)|Y(16),  Y(17), 15, PRSC_DIV_SEL_8, NODE_GAIN(A_GAIN, D_GAIN), COMMON_FILTER_LEVEL                  \
}
#endif
/**********************************************************/
/***************** Key Params   ******************/
/**********************************************************/
/* Defines the number of key sensors
 * Range: 1 to 65535.
 * Default value: 1
 */
#ifdef USE_10CH_WHITE_SENSOR
#define DEF_NUM_SENSORS (10)
#endif
        
#ifdef USE_32CH_INTERPOSER_SENSOR
#define DEF_NUM_SENSORS (16)
#endif
        
/* Defines Key Sensor setting
 * {Sensor Threshold, Sensor Hysterisis, Sensor AKS}
 */
#define KEY_0_PARAMS                                                                                            \
{                                                                                                              \
    COMMON_KEY_THRESHOLD, HYST_25, NO_AKS_GROUP                       \
}
#define KEY_1_PARAMS                                                                                            \
{                                                                                                              \
    COMMON_KEY_THRESHOLD, HYST_25, NO_AKS_GROUP                       \
}
#define KEY_2_PARAMS                                                                                            \
{                                                                                                              \
    COMMON_KEY_THRESHOLD, HYST_25, NO_AKS_GROUP                       \
}
#define KEY_3_PARAMS                                                                                            \
{                                                                                                              \
    COMMON_KEY_THRESHOLD, HYST_25, NO_AKS_GROUP                       \
}
#define KEY_4_PARAMS                                                                                            \
{                                                                                                              \
    COMMON_KEY_THRESHOLD, HYST_25, NO_AKS_GROUP                       \
}
#define KEY_5_PARAMS                                                                                            \
{                                                                                                              \
    COMMON_KEY_THRESHOLD, HYST_25, NO_AKS_GROUP                       \
}
#define KEY_6_PARAMS                                                                                            \
{                                                                                                              \
    COMMON_KEY_THRESHOLD, HYST_25, NO_AKS_GROUP                       \
}
#define KEY_7_PARAMS                                                                                            \
{                                                                                                              \
    COMMON_KEY_THRESHOLD, HYST_25, NO_AKS_GROUP                       \
}
#define KEY_8_PARAMS                                                                                            \
{                                                                                                              \
    COMMON_KEY_THRESHOLD, HYST_25, NO_AKS_GROUP                       \
}
#define KEY_9_PARAMS                                                                                            \
{                                                                                                              \
    COMMON_KEY_THRESHOLD, HYST_25, NO_AKS_GROUP                       \
}
#ifdef USE_32CH_INTERPOSER_SENSOR
// add 6 additional keys for Amazon 16 chan mat
        
#define KEY_10_PARAMS                                                                                            \
{                                                                                                              \
    COMMON_KEY_THRESHOLD, HYST_25, NO_AKS_GROUP                       \
}
#define KEY_11_PARAMS                                                                                            \
{                                                                                                              \
    COMMON_KEY_THRESHOLD, HYST_25, NO_AKS_GROUP                       \
}
#define KEY_12_PARAMS                                                                                            \
{                                                                                                              \
    COMMON_KEY_THRESHOLD, HYST_25, NO_AKS_GROUP                       \
}
#define KEY_13_PARAMS                                                                                            \
{                                                                                                              \
    COMMON_KEY_THRESHOLD, HYST_25, NO_AKS_GROUP                       \
}
#define KEY_14_PARAMS                                                                                            \
{                                                                                                              \
    COMMON_KEY_THRESHOLD, HYST_25, NO_AKS_GROUP                       \
}
#define KEY_15_PARAMS                                                                                            \
{                                                                                                              \
    COMMON_KEY_THRESHOLD, HYST_25, NO_AKS_GROUP                       \
}
#endif        

 /* De-bounce counter for additional measurements to confirm touch detection
 * Range: 0 to 255.
 * Default value: 2.
 */
#define DEF_TOUCH_DET_INT 2

/* De-bounce counter for additional measurements to confirm away from touch signal
 * to initiate Away from touch re-calibration.
 * Range: 0 to 255.
 * Default value: 5.
 */
#define DEF_ANTI_TCH_DET_INT 0

/* Threshold beyond with automatic sensor recalibration is initiated.
 * Range: RECAL_100/ RECAL_50 / RECAL_25 / RECAL_12_5 / RECAL_6_25 / MAX_RECAL
 * Default value: RECAL_100.
 */
#define DEF_ANTI_TCH_RECAL_THRSHLD RECAL_100

/* Rate at which sensor reference value is adjusted towards sensor signal value
 * when signal value is greater than reference.
 * Units: 200ms
 * Range: 0-255
 * Default value: 20u = 4 seconds.
 */
#define DEF_TCH_DRIFT_RATE 20

/* Rate at which sensor reference value is adjusted towards sensor signal value
 * when signal value is less than reference.
 * Units: 200ms
 * Range: 0-255
 * Default value: 5u = 1 second.
 */
#define DEF_ANTI_TCH_DRIFT_RATE 5

/* Time to restrict drift on all sensor when one or more sensors are activated.
 * Units: 200ms
 * Range: 0-255
 * Default value: 20u = 4 seconds.
 */
#define DEF_DRIFT_HOLD_TIME 20

/* Set mode for additional sensor measurements based on touch activity.
 * Range: REBURST_NONE / REBURST_UNRESOLVED / REBURST_ALL
 * Default value: REBURST_UNRESOLVED
 */
#define DEF_REBURST_MODE REBURST_UNRESOLVED

/* Sensor maximum ON duration upon touch.
 * Range: 0-255
 * Default value: 0
 */
#define DEF_MAX_ON_DURATION 0

/**********************************************************/
/********* Frequency Hop Module ****************/
/**********************************************************/

/* sets the frequency steps for hop.
 * Range: 3 to 7.
 * Default value: 3
 */
#define NUM_FREQ_STEPS 3

/* PTC Sampling Delay Selection - 0 to 15 PTC CLK cycles */
#define DEF_MEDIAN_FILTER_FREQUENCIES FREQ_SEL_1,FREQ_SEL_3,FREQ_SEL_7


/**********************************************************/
/***************** Communication - Data Streamer ******************/
/**********************************************************/
#define DEF_TOUCH_DATA_STREAMER_ENABLE 1u
#define DATA_STREAMER_BOARD_TYPE USER_BOARD

#include "datastreamer/datastreamer.h"

/**********************************************************/
/***************** IIR - Signal Filter ******************/
/**********************************************************/

#define DEF_USE_TOUCH_IIR 1u    // enable filter
extern qtm_acq_node_data_t ptc_qtlib_node_stat1[DEF_NUM_CHANNELS];
extern qtm_touch_key_data_t qtlib_key_data_set1[DEF_NUM_SENSORS];

/**********************************************************/


// DOM-IGNORE-BEGIN
#ifdef __cplusplus  // Provide C++ Compatibility

}

#endif
// DOM-IGNORE-END
#endif // TOUCH_H
