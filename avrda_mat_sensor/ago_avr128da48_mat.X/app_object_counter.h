/* 
 * File:   app_object_counter.h
 * Author: C13999
 *
 * Created on November 17, 2023, 4:16 PM
 */

#ifndef APP_OBJECT_COUNTER_H
#define	APP_OBJECT_COUNTER_H

#ifdef	__cplusplus
extern "C" {
#endif

    /* enum for app state machine states */
    typedef enum {
        APP_IDLE,
        APP_FIRST_POS_DETECT,
        APP_DETECT_ADD_OR_REMOVE,
    } _app_object_detect_state_t;

    typedef struct shelf_data {
        int16_t lane_object_counter;
        int32_t tmr_delay_to_check_object_status;
        _app_object_detect_state_t app_object_detect_state;
    } _app_shelf_data;

    extern volatile _app_shelf_data shelf_data[DEF_NUM_CHANNELS];
    
    void app_timer_event_count(void);
    void object_counter_process(void);

#ifdef	__cplusplus
}
#endif

#endif	/* APP_OBJECT_COUNTER_H */

