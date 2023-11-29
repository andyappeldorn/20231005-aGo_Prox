#include <xc.h>

#include "mcc_generated_files/touch/touch.h"
#include "app_object_counter.h"
#include "mat_decode.h"

volatile _app_shelf_data shelf_data[DEF_NUM_CHANNELS];
volatile uint16_t app_tmr_ms_counter;

void app_timer_event_count(void) {
    app_tmr_ms_counter++;

    //    uint8_t index;
    //
    //    for (index = 0; index < DEF_NUM_CHANNELS; index++) {
    //        shelf_data[index].tmr_touched_object_event_time++;
    //    }
    /* loop all sensors */
    /* if state is active, decrement counter */
    /* prevent overflow of counter register */
    /* reload counter once serviced */
}

uint16_t app_get_current_time(void) {
    uint16_t current_time;
    current_time = app_tmr_ms_counter;

    return current_time;
}

uint16_t app_get_elapsed_time(uint16_t past_time) {
    /* note: this timer method does not support cases where
     elapsed time is greater than 65,535ms, uint16 size */
    int32_t elapsed_time;
    int16_t current_time;
    current_time = app_tmr_ms_counter;

    if (current_time > past_time) {
        elapsed_time = current_time - past_time;
    } else if (current_time < past_time) {
        elapsed_time = current_time + (0xFFFF - past_time);
    }
    return elapsed_time;
}

void object_counter_process_init(void) {
    uint8_t index;

    for (index = 0; index < DEF_NUM_CHANNELS; index++) {
        shelf_data[index].app_object_detect_state = APP_IDLE;
        shelf_data[index].lane_object_counter = 0;
    }
}

void object_counter_process(void) {
    /* PROCESS */
    /* check sensor status of all channels */
    /* wait 500ms to wait for associate to stop touching */
    /* is key state added or removed */
    /* count up or down lane value */
    /* calibrate sensor */
    /* return */
    /* /PROCESS */
    uint8_t index;
    uint16_t elapsed_time;
    uint8_t trap = 0;
    /* check sensor status of all channels */
    for (index = 0; index < DEF_NUM_CHANNELS; index++) {
        /* was a sensor activated */
        switch (shelf_data[index].app_object_detect_state) {
            case APP_IDLE:
                /* look for positive detection */
                /* ? if neg detection, reset states? */

                if (mat_decode_data[index].lane_state == IDLE) {
                    /* do nothing */
                    /* do we ever hit this? */
                    shelf_data[index].tmr_touched_object_event_time = app_get_current_time(); // get current time of touch event
                } else if (mat_decode_data[index].lane_state == POS_DETECT) {
                    /* positive change was detected,
                     * user touched object on lane sensor
                     * save time of object touch action */
                    shelf_data[index].tmr_touched_object_event_time = app_get_current_time(); // get current time of touch event
                    shelf_data[index].tmr_elapsed_time = 0; // init elapsed time
                    shelf_data[index].app_object_detect_state = APP_FIRST_POS_DETECT; // set state to first positive detect
                } else if (mat_decode_data[index].lane_state == NEG_DETECT) {
                    /* do nothing */
                    /* reset state machine? */
                    shelf_data[index].tmr_elapsed_time = app_get_elapsed_time(shelf_data[index].tmr_touched_object_event_time);
                    if (shelf_data[index].tmr_elapsed_time > 5000) {
                        /* sensor reset process */
                        calibrate_node(index); // re-calibrate cc cap for lane sensor
                        mat_decode_init_sensor(index);
                        shelf_data[index].app_object_detect_state = APP_IDLE;
                    }
                } else {
                    /* do nothing */
                    /* reset state machine? */
                }

                break;
            case APP_FIRST_POS_DETECT:
                /* wait for elapsed time */
                shelf_data[index].tmr_elapsed_time = app_get_elapsed_time(shelf_data[index].tmr_touched_object_event_time);
                if (shelf_data[index].tmr_elapsed_time > 1000) {
                    shelf_data[index].app_object_detect_state = APP_DETECT_ADD_OR_REMOVE;
                }

                break;
            case APP_DETECT_ADD_OR_REMOVE:
                /* check lane state */
                if (mat_decode_data[index].lane_state == POS_DETECT) {
                    /* positive change was detected */
                    shelf_data[index].lane_object_counter++;
                } else if (mat_decode_data[index].lane_state == NEG_DETECT) {
                    /* negative change was detected */
                    shelf_data[index].lane_object_counter--;
                } else {
                    /* no change detected, reset app state to idle */
                    shelf_data[index].app_object_detect_state = APP_IDLE;
                }
                /* set state to wait for calibrate */
                shelf_data[index].app_object_detect_state = APP_WAIT_FOR_CALIBRATE;

                break;
            case APP_WAIT_FOR_CALIBRATE:
                /* wait time to calibrate for user to release object */
                shelf_data[index].tmr_elapsed_time = app_get_elapsed_time(shelf_data[index].tmr_touched_object_event_time);
                if (shelf_data[index].tmr_elapsed_time > 2500) {
                    /* sensor reset process */
                    calibrate_node(index); // re-calibrate cc cap for lane sensor
                    mat_decode_init_sensor(index);
                    shelf_data[index].app_object_detect_state = APP_IDLE;
                }

                break;
            default:
                trap = 1;
                //                shelf_data[index].app_object_detect_state = APP_IDLE;
                break;
        }
        if (trap > 0) {
            NOP();
        }

    }
}
