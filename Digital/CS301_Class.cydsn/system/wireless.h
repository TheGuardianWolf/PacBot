#ifndef WIRELESS_H
#define WIRELESS_H
/* ========================================
 *
 * Copyright YOUR COMPANY, THE YEAR
 * All Rights Reserved
 * UNPUBLISHED, LICENSED SOFTWARE.
 *
 * CONFIDENTIAL AND PROPRIETARY INFORMATION
 * WHICH IS THE PROPERTY OF your company.
 *
 * ========================================
*/

#include <stdint.h>

//RF 
#define SOP 0xaa

typedef struct{
	    int8_t            rssi;	
        uint8_t           index;			// index number of packet. incremented number
	    int16_t			robot_xpos;	 	// 
    	int16_t			robot_ypos;		//
        int16_t         robot_orientation;
	    int16_t			g0_xpos;		//
    	int16_t			g0_ypos;		//
	    int16_t			g0_speed;		//
    	int16_t		g0_direction;	//
    	int16_t			g1_xpos;		//
    	int16_t			g1_ypos;		//
	    int16_t			g1_speed;		//
        int16_t		g1_direction;	//
        int16_t			g2_xpos;		//
        int16_t			g2_ypos;		//
        int16_t			g2_speed;		//
        int16_t		g2_direction;	//
    } RFData;

typedef union {
    uint8_t bytes[32];
    RFData data;
} RFBuffer;  

//struct data_main system_state;

void wireless_init();

RFData wireless_get();

/* [] END OF FILE */

#endif /* WIRELESS_H */
