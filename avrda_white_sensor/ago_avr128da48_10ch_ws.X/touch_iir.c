
#include "touch_iir.h"

extern qtm_touch_key_data_t          qtlib_key_data_set1[];
extern qtm_acq_node_data_t			 ptc_qtlib_node_stat1[];

#if DEF_USE_TOUCH_IIR == 1

volatile _touch_iirData touch_iirData[DEF_NUM_CHANNELS];

void touch_iirSignalFilter(void){
    uint8_t index;
    volatile uint64_t accumulator;
    
    for (index = 0; index < DEF_NUM_CHANNELS; index++) {
        touch_iirData[index].unFilteredSignal = get_sensor_node_signal(index); // get signal value before filtering for comparison to filtered value
        
        if ((qtlib_key_data_set1[index].sensor_state == QTM_KEY_STATE_INIT) || (qtlib_key_data_set1[index].sensor_state == QTM_KEY_STATE_CAL)) {
            touch_iirData[index].signal = get_sensor_node_signal(index); // get initial value
            touch_iirData[index].signal_m1 = get_sensor_node_signal(index); // get initial value
        }
        else {
        touch_iirData[index].signal_m1 = touch_iirData[index].signal_m0;  // preserve previous signal value for (n-1) = n
        touch_iirData[index].signal_m0 = get_sensor_node_signal(index); // get new signal value (n)
        
        // IIR filter
        accumulator = (uint64_t)(
                (touch_iirData[index].signal * 7) +
                (touch_iirData[index].signal_m1 * 0) +
                (touch_iirData[index].signal_m0 * 1));
        
        touch_iirData[index].signal = accumulator/8;    // average accumulated value
        update_sensor_node_signal(index, touch_iirData[index].signal);  // store back into node data container
        }
    }
}

#endif