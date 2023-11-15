#include "mat_decode.h"

volatile _mat_decode_data_t mat_decode_data[DEF_NUM_SENSORS];

void mat_decode_process(void) {
    uint8_t index;
    volatile uint64_t accumulator;

    for (index = 0; index < DEF_NUM_CHANNELS; index++) {

        if ((qtlib_key_data_set1[index].sensor_state == QTM_KEY_STATE_INIT) || (qtlib_key_data_set1[index].sensor_state == QTM_KEY_STATE_CAL)) {
            // do nothing in sensor uninitialized state
//            touch_iirData[index].signal = get_sensor_node_signal(index); // get initial value
//            touch_iirData[index].signal_m1 = get_sensor_node_signal(index); // get initial value
        } else {
            /* maintain reference */
            // is sensor reference valid?  if not, init with signal value
            if(mat_decode_data[index].channel_reference == 0){
                mat_decode_data[index].channel_reference = get_sensor_node_signal(index);
            }
            // else, average signal with reference
            // @todo - improve averaging function
            else{
                mat_decode_data[index].channel_reference = ((mat_decode_data[index].channel_reference*3) + get_sensor_node_signal(index))/4;
            }
            
            /* calculate delta */
            // no
            
            /* compare against upper and lower thresholds */
            // compare against upper threshold
            if(get_sensor_node_signal(index) > (mat_decode_data[index].channel_reference + POSITIVE_THRESHOLD)){
                // positive threshold event
            }
            else if(get_sensor_node_signal(index) < (mat_decode_data[index].channel_reference - NEGATIVE_THRESHOLD)){
                // negative threshold event
            }
        }
    }
}

/***** TODO *****/
/* add mat decode data to data streamer output c and ds files and test */