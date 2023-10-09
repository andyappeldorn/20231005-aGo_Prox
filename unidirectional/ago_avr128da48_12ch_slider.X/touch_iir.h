/* 
 * File:   touch_iir.h
 * Author: andya
 *
 * Created on September 22, 2022, 12:59 PM
 */

#ifndef TOUCH_IIR_H
#define	TOUCH_IIR_H

#ifdef	__cplusplus
extern "C" {
#endif

#include "mcc_generated_files/touch/include/touch_api_ptc.h"
#include "mcc_generated_files/touch/touch.h"    
    
//#if DEF_USE_TOUCH_IIR == 1

    typedef struct {
        uint32_t signal; // filtered signal
        uint32_t signal_m0; // n-0
        uint32_t signal_m1; // n-1
        uint32_t unFilteredSignal;   // un-filtered signal value
    } _touch_iirData;

    extern volatile _touch_iirData touch_iirData[DEF_NUM_CHANNELS];

    void touch_iirSignalFilter(void);
    
//#endif

#ifdef	__cplusplus
}
#endif

#endif	/* TOUCH_IIR_H */

