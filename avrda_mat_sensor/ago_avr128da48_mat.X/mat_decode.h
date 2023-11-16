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
    } _mat_key_state;
    
#define MAT_POS_THRESHOLD   50
#define MAT_POS_HYSTERESIS  5
#define MAT_POS_DI_COUNT  5
#define MAT_NEG_THRESHOLD   50
#define MAT_NEG_HYSTERESIS  5
#define MAT_NEG_DI_COUNT  5
    
    typedef struct {
        _mat_decode_process_state decode_state;
        _mat_key_state key_state; /* status of key */
        uint8_t init_counter;
        uint16_t channel_reference; /* Reference signal */
        uint8_t di_pos_counter;
        uint8_t di_neg_counter;
    } _mat_decode_data_t;

    extern volatile _mat_decode_data_t mat_decode_data[DEF_NUM_SENSORS];

    void mat_decode_process(void);

#ifdef	__cplusplus
}
#endif

#endif	/* MAT_DECODE_H */

