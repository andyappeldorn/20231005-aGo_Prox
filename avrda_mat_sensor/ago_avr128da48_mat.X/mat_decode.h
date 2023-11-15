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
    
typedef struct
{
  uint8_t sensor_state;     /* idle, positive detect, negative detect */
  uint16_t channel_reference;    /* Reference signal */
//  qtm_acq_node_data_t* node_data_struct_ptr;      /* Pointer to node data structure */
}_mat_decode_data_t;

extern volatile _mat_decode_data_t mat_decode_data[DEF_NUM_SENSORS];

void mat_decode_process(void);

#ifdef	__cplusplus
}
#endif

#endif	/* MAT_DECODE_H */

