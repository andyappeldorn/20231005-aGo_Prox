#include "mat_decode.h"

volatile _mat_decode_data_t mat_decode_data[DEF_NUM_SENSORS];

void AGO_mat_decode_init(void) {
    /* set all sensor mat decode states to INIT */
    uint8_t index;
    for (index = 0; index < DEF_NUM_CHANNELS; index++) {
        mat_decode_data[index].decode_state = MAT_DECODE_STATE_INIT; // set all sensors to
        mat_decode_data[index].key_state = IDLE;
        mat_decode_data[index].init_counter = 10; // number of times to stay in init state
        mat_decode_data[index].di_pos_counter = MAT_POS_DI_COUNT;
        mat_decode_data[index].di_neg_counter = MAT_NEG_DI_COUNT;
        mat_decode_data[index].channel_pos_threshold = MAT_POS_THRESHOLD;
        mat_decode_data[index].channel_pos_hysteresis = MAT_POS_THRESHOLD - MAT_POS_HYSTERESIS;
        mat_decode_data[index].channel_neg_threshold = MAT_NEG_THRESHOLD;
        mat_decode_data[index].channel_neg_hysteresis = MAT_NEG_THRESHOLD - MAT_NEG_HYSTERESIS;
    }
}

void mat_decode_process(void) {
    uint8_t index;
    volatile _mat_decode_process_state this_decode_state;

    for (index = 0; index < DEF_NUM_CHANNELS; index++) {
        this_decode_state = mat_decode_data[index].decode_state;
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
                mat_decode_data[index].channel_delta = (int16_t) (ptc_qtlib_node_stat1[index].node_acq_signals - mat_decode_data[index].channel_reference); // calculate delta = signal - reference
                /* compare against upper and lower thresholds */
                if (mat_decode_data[index].channel_delta >= mat_decode_data[index].channel_pos_threshold) {
                    /* delta > positive threshold */
                    if (mat_decode_data[index].di_pos_counter > 0) {
                        /* wait for consecutive samples above pos threshold */
                        mat_decode_data[index].di_pos_counter--; // count down di counter register
                    } else {
                        /* consecutive decodes above positive threshold, confirmed action */
                        mat_decode_data[index].decode_state = MAT_DECODE_STATE_DETECT;
                        mat_decode_data[index].key_state = POS_DETECT;
                    }
                } else if (mat_decode_data[index].channel_delta <= (-mat_decode_data[index].channel_neg_threshold)) {
                    /* compare against negative threshold */
                    if (mat_decode_data[index].di_neg_counter > 0) {
                        /* wait for consecutive samples below negative threshold */
                        mat_decode_data[index].di_neg_counter--;
                    } else {
                        /* consecutive decodes below negative threshold, confirmed action */
                        mat_decode_data[index].decode_state = MAT_DECODE_STATE_DETECT;
                        mat_decode_data[index].key_state = NEG_DETECT;
                    }
                } else {
                    mat_decode_data[index].decode_state = MAT_DECODE_STATE_MEASURE;
                    mat_decode_data[index].key_state = IDLE;
                    mat_decode_data[index].di_pos_counter = MAT_POS_DI_COUNT;
                    mat_decode_data[index].di_neg_counter = MAT_NEG_DI_COUNT;
                }
                break;
                
            case (MAT_DECODE_STATE_DETECT):
                /* sensor triggered, no reference compensation */
                mat_decode_data[index].channel_delta = (int16_t) (ptc_qtlib_node_stat1[index].node_acq_signals - mat_decode_data[index].channel_reference); // calculate delta = signal - reference
                /* hysteresis for release */
                if (mat_decode_data[index].key_state == POS_DETECT) {
                    if (mat_decode_data[index].channel_delta < mat_decode_data[index].channel_pos_hysteresis) {
                        /* signal is below hysteresis value*/
                        mat_decode_data[index].key_state = IDLE;
                        mat_decode_data[index].decode_state = MAT_DECODE_STATE_MEASURE;
                    } else {

                    }
                } else if (mat_decode_data[index].key_state == NEG_DETECT) {
                    if (mat_decode_data[index].channel_delta > -mat_decode_data[index].channel_neg_hysteresis) {
                        /* signal is above hysteresis value */
                        mat_decode_data[index].key_state = IDLE;
                        mat_decode_data[index].decode_state = MAT_DECODE_STATE_MEASURE;
                    } else{
                        
                    }
                } else {
                    mat_decode_data[index].key_state = IDLE;
                    mat_decode_data[index].decode_state = MAT_DECODE_STATE_MEASURE;
                }
                break;
                
            default:
                mat_decode_data[index].key_state = IDLE;
                mat_decode_data[index].decode_state = MAT_DECODE_STATE_MEASURE;
                break;
                
        }
    }
}
