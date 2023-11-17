/* 
 * File:   mat_decode.h
 * Author: C13999
 *
 * Created on November 14, 2023, 4:33 PM
 */

#ifndef MAT_DECODE_H
#define	MAT_DECODE_H

#ifdef	__cplusplus
extern "C" {
#endif

#include "mcc_generated_files/touch/include/touch_api_ptc.h"
#include "mcc_generated_files/touch/touch.h"
#include "amazon.h"

    typedef enum {
        MAT_DECODE_STATE_INIT,
        MAT_DECODE_STATE_MEASURE,
        MAT_DECODE_STATE_DETECT
    } _mat_decode_process_state;

    typedef enum {
        IDLE,
        POS_DETECT,
        NEG_DETECT
    } _mat_lane_detect_state;
    

    
    typedef struct {
        _mat_decode_process_state decode_state; // status of decode process
        _mat_lane_detect_state lane_state; // status of lane
        uint8_t init_counter;
        uint16_t channel_reference;
        int16_t channel_delta;
        int16_t channel_pos_threshold;
        int16_t channel_pos_hysteresis;
        int16_t channel_neg_threshold;
        int16_t channel_neg_hysteresis;
        uint8_t di_pos_counter;
        uint8_t di_neg_counter;
    } _mat_decode_data_t;

    extern volatile _mat_decode_data_t mat_decode_data[DEF_NUM_SENSORS];
    
    void AGO_mat_decode_init(void);
    void mat_decode_process(void);

#ifdef	__cplusplus
}
#endif

#endif	/* MAT_DECODE_H */

