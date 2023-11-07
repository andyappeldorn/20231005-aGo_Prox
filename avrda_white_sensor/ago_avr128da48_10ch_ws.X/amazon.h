/* Microchip Technology Inc. and its subsidiaries.  You may use this software 
 * and any derivatives exclusively with Microchip products. 
 * 
 * THIS SOFTWARE IS SUPPLIED BY MICROCHIP "AS IS".  NO WARRANTIES, WHETHER 
 * EXPRESS, IMPLIED OR STATUTORY, APPLY TO THIS SOFTWARE, INCLUDING ANY IMPLIED 
 * WARRANTIES OF NON-INFRINGEMENT, MERCHANTABILITY, AND FITNESS FOR A 
 * PARTICULAR PURPOSE, OR ITS INTERACTION WITH MICROCHIP PRODUCTS, COMBINATION 
 * WITH ANY OTHER PRODUCTS, OR USE IN ANY APPLICATION. 
 *
 * IN NO EVENT WILL MICROCHIP BE LIABLE FOR ANY INDIRECT, SPECIAL, PUNITIVE, 
 * INCIDENTAL OR CONSEQUENTIAL LOSS, DAMAGE, COST OR EXPENSE OF ANY KIND 
 * WHATSOEVER RELATED TO THE SOFTWARE, HOWEVER CAUSED, EVEN IF MICROCHIP HAS 
 * BEEN ADVISED OF THE POSSIBILITY OR THE DAMAGES ARE FORESEEABLE.  TO THE 
 * FULLEST EXTENT ALLOWED BY LAW, MICROCHIP'S TOTAL LIABILITY ON ALL CLAIMS 
 * IN ANY WAY RELATED TO THIS SOFTWARE WILL NOT EXCEED THE AMOUNT OF FEES, IF 
 * ANY, THAT YOU HAVE PAID DIRECTLY TO MICROCHIP FOR THIS SOFTWARE.
 *
 * MICROCHIP PROVIDES THIS SOFTWARE CONDITIONALLY UPON YOUR ACCEPTANCE OF THESE 
 * TERMS. 
 */

/* 
 * File:   amazon.h
 * Author: ScottC
 * Comments: custom .h file for Amazon shelf proximity project
 * Revision history: Rev 1 10-11-2023
 */

// This is a guard condition so that contents of this file are not included
// more than once.  
#ifndef AMAZON_H
#define	AMAZON_H

/* use the defines below to set sensor parameters */
#define SENSOR_LEN_INCHES 24
#define NUMBER_OF_SENSORS 10

/* Defines to set all sensor params at once by changing here */

/* Gain setting can be GAIN_1, GAIN_2, GAIN_4, GAIN_8, GAIN_16 */
#define A_GAIN GAIN_2       // Analog Gain
#define D_GAIN GAIN_8       // Digital Gain
#define SENSOR_CSD 10

#define COMMON_KEY_THRESHOLD 20

/* Digital Oversampling can be FILTER_LEVEL_1, 2, 4, 8, 16, 32, 64 */
#define COMMON_FILTER_LEVEL FILTER_LEVEL_64     // 

#endif	/* AMAZON_H */

