#include "mat_decode.h"

volatile _mat_decode_data_t mat_decode_data[DEF_NUM_SENSORS];

void AGO_mat_decode_init(void) {
    /* set all sensor mat decode states to INIT */
    uint8_t index;
    for (index = 0; index < DEF_NUM_CHANNELS; index++) {
        mat_decode_data[index].decode_state = MAT_DECODE_STATE_INIT; // set all sensors to 
        mat_decode_data[index].init_counter = 10; // number of times to stay in init state
    }
}

void mat_decode_process(void) {
    uint8_t index;
    volatile int16_t touch_delta;
    volatile _mat_decode_process_state this_decode_state;
    uint8_t this_key_state;
    //    volatile uint64_t accumulator;

    for (index = 0; index < DEF_NUM_CHANNELS; index++) {
        this_decode_state = mat_decode_data[index].decode_state;
        this_key_state = mat_decode_data[index].key_state;
        /* maintain reference */
        switch (this_decode_state) {
            case (MAT_DECODE_STATE_INIT):
                /* sensor reference not initialized, set to signal value */
                mat_decode_data[index].channel_reference = ptc_qtlib_node_stat1[index].node_acq_signals;
                if (mat_decode_data[index].init_counter-- == 0) {
                    mat_decode_data[index].decode_state = MAT_DECODE_STATE_MEASURE;
                }
                break;
            case (MAT_DECODE_STATE_MEASURE):
                /* adjust reference drift */
                if (mat_decode_data[index].channel_reference < ptc_qtlib_node_stat1[index].node_acq_signals) {
                    /* reference < signal */
                    mat_decode_data[index].channel_reference++;
                } else if (mat_decode_data[index].channel_reference > ptc_qtlib_node_stat1[index].node_acq_signals) {
                    /* reference > signal */
                    mat_decode_data[index].channel_reference--;
                } else {
                    /* reference = signal */
                    /* do nothing */
                }

                /* compare signal to thresholds */
                touch_delta = (int16_t) (mat_decode_data[index].channel_reference - ptc_qtlib_node_stat1[index].node_acq_signals); // calculate delta
                /* compare against upper and lower thresholds */
                if (touch_delta >= MAT_POS_THRESHOLD) {
                    /* delta > positive threshold */
                    if (mat_decode_data[index].di_pos_counter > 0) {
                        /* wait for consecutive samples above pos threshold */
                        mat_decode_data[index].di_pos_counter--; // count down di counter register
                    } else {
                        /* consecutive decodes above pos threshold, confirmed action */
                        mat_decode_data[index].decode_state = MAT_DECODE_STATE_DETECT;
                        mat_decode_data[index].key_state = POS_DETECT;
                    }
                    // add hysteresis 
                } else if (touch_delta <= -MAT_NEG_THRESHOLD) {
                    /* compare against negative threshold */
                    if (mat_decode_data[index].di_neg_counter > 0) {
                        /* wait for consecutive samples below neg threshold */
                        mat_decode_data[index].di_neg_counter--;
                    } else {
                        /* consecutive decodes below neg threshold, confirmed action */
                        mat_decode_data[index].decode_state = MAT_DECODE_STATE_DETECT;
                        mat_decode_data[index].key_state = NEG_DETECT;
                    }
                    // add hysteresis
                } else {
                    //            mat_decode_data[index].decode_state = MAT_DECODE_STATE_MEASURE;
                    //            mat_decode_data[index].key_state = IDLE;
                    mat_decode_data[index].di_pos_counter = MAT_POS_DI_COUNT;
                    mat_decode_data[index].di_neg_counter = MAT_NEG_DI_COUNT;
                }

                break;
            case (MAT_DECODE_STATE_DETECT):
                /* sensor triggered, no reference compensation */

                break;
            default:
                break;
                //                mat_decode_data[index].decode_state = MAT_DECODE_STATE_INIT;
        }
    }
}

/***** TODO *****/
/* add mat decode data to data streamer output c and ds files and test */