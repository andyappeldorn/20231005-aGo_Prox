/*
 * MAIN Generated Driver File
 * 
 * @file main.c
 * 
 * @defgroup main MAIN
 * 
 * @brief This is the generated driver implementation file for the MAIN driver.
 *
 * @version MAIN Driver Version 1.0.0
 */

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
#include "mcc_generated_files/system/system.h"
#include "mat_decode.h"

/*
    Main application
 */

extern volatile uint8_t measurement_done_touch;

/* enum for app state machine states */
typedef enum {
    APP_IDLE,
    APP_FIRST_POS_DETECT,
    APP_DETECT_ADD_OR_REMOVE,
} _app_object_detect_state_t;

struct shelf_data {
    int16_t lane_object_counter[DEF_NUM_CHANNELS];
    int32_t tmr_delay_to_check_object_status[DEF_NUM_CHANNELS];
    _app_object_detect_state_t app_object_detect_state[DEF_NUM_CHANNELS];
};

void app_timer_event_count(void) {
    /* loop all sensors */
    /* if state is active, decrement counter */
    /* prevent overflow of counter register */
    /* reload counter once serviced */
}
/**********************************/
/*** MAIN *************************/
/**********************************/
int main(void) {
    uint8_t index;

    SYSTEM_Initialize();

    AGO_mat_decode_init();

    TCB0_OverflowCallbackRegister(app_timer_event_count);

    while (1) {
        touch_process(); // measure and decode all sensor lines

        if (measurement_done_touch == 1) {
            measurement_done_touch = 0;
#if 0
            /* PROCESS */
            /* check sensor status of all channels */
            /* wait 500ms to wait for associate to stop touching */
            /* is key state added or removed */
            /* count up or down lane value */
            /* calibrate sensor */
            /* return */
            /* /PROCESS */

            /* check sensor status of all channels */
            for (index = 0; index < DEF_NUM_CHANNELS; index++) {
                /* was a sensor activated */
                switch (shelf_data.app_object_detect_state[index]) {
                    case APP_IDLE:
                        /* look for positive detection */
                        /* ? if neg detection, reset states? */
                        switch (mat_decode_data[index].decode_state) {
                            case IDLE:
                                /* do nothing */
                                break;
                            case POS_DETECT:
                                break;
                            case NEG_DETECT:
                                break;
                            default:
                                break;
                        }
                        break;
                    case APP_FIRST_POS_DETECT:
                        break;
                    case APP_DETECT_ADD_OR_REMOVE:
                        break;
                    default:
                        break;
                }




            }
#endif
            
        }

        /*Push button calibration operation*/
        if (Button_GetValue() == 0) {
            for (index = 0; index < DEF_NUM_CHANNELS; index++) {
                calibrate_node(index);
                AGO_mat_decode_init();
            }
        }
    }
}
