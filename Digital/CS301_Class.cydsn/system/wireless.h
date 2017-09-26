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
#define SOP 0x0a
#define PACKET_INTERVAL 2

typedef struct {
    int16_t x;
    int16_t y;
    int16_t orientation;
} DiPoint;

typedef struct{
	    uint16_t        robot_xpos;	
        uint16_t        robot_ypos;			
	    uint16_t		g0_xpos;	 	// 
    	uint16_t	    g0_ypos;	    //
        int16_t         g0_speed;
	    int8_t			g0_direction;   //1-up 2-down 3-right 4-left
	    uint16_t		g1_xpos;	 	// 
    	uint16_t	    g1_ypos;	//
        int16_t         g1_speed;
	    int8_t			g1_direction;		//
	    uint16_t		g2_xpos;	 	// 
    	uint16_t	    g2_ypos;	//
        int16_t         g2_speed;
	    int8_t			g2_direction;		//
        uint32_t    timestamp;
    } RFData;

typedef union {
    uint8_t bytes[25];
    RFData data;
} RFBuffer;

//struct data_main system_state;

void wireless_init();

RFData wireless_get();

bool wireless_check();

/* [] END OF FILE */

#endif /* WIRELESS_H */
