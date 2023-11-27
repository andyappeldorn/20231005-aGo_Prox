#include "mcc_generated_files/touch/touch.h"
#include "app_object_counter.h"

volatile _app_shelf_data shelf_data[DEF_NUM_CHANNELS];

void app_timer_event_count(void) {
    uint8_t index;
    
    for(index=0; index<DEF_NUM_CHANNELS; index++){
        shelf_data[index].lane_object_counter++;
    }
    /* loop all sensors */
    /* if state is active, decrement counter */
    /* prevent overflow of counter register */
    /* reload counter once serviced */
}

void object_counter_process(void) {
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
