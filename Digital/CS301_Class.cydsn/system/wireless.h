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
#include <stdbool.h>

//RF 
#define SOP 0xaa
#define PACKET_INTERVAL 2
#define RF_MMPERPIXEL 2.5f
#define RF_DEGREESPERUNIT 5

typedef struct {
    int16_t x;
    int16_t y;
    int16_t orientation;
} DiPoint;

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
        uint32_t    timestamp;
    } RFData;

typedef union {
    uint8_t bytes[32];
    RFData data;
} RFBuffer;

//struct data_main system_state;

void wireless_init();

RFData wireless_get();

bool wireless_check();

/* [] END OF FILE */

#endif /* WIRELESS_H */
