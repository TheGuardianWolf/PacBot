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

#define USE_RF RF_BT_SELECT_Write(0);
#define USE_BT RF_BT_SELECT_Write(1);

//RF 
#define SOP 0xaa
#define PACKETSIZE 32    
#define RXSTRINGSIZE 64 // 4+3+[4+4+4]+[4+4+4+4]+[4+4+4+4]+[4+4+4+4]+delimiters, i.e. no more than 64

#define RF_UNKNOWN 10

uint8_t flag_rx = 0;
uint8_t flag_packet = 0;

uint8_t dataready_flag =0;

uint8_t flag_rf_transmission_active = RF_UNKNOWN; // [TRUE if receieving data, FALSE is not, UNKNOWN at startup]

typedef union {
    int16_t a[17];
    struct{
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
    } s;
} datatype;   


//struct data_main system_state;

void uart_init();

/* [] END OF FILE */

#endif /* WIRELESS_H */
