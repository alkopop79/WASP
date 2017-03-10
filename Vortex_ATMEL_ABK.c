/***********************************************************************************************\
*                                                                                               *
*       Example Circuit for Downloading 1 of 4 circuits to the AN221E04                         *
*                                                                                               *
*                                           Copyright (c) Anadigm 2004                          *
*                                                                                               *
*************************************************************************************************
*                                                                                               *
*       File name:          AN221Mega128.c                          Release:    1.0             *
*       Last Edit Date:     5-Feb-04                                                            *
*       Test Date:          5-Feb-04                                                            *
*                                                                                               *
*       Compiler:           avr-gcc 3.2 (June 25 2002)                                          *
*       IDE:                AVR Studio 3.56                                                     *
*       Target:             Atmel ATmega128                                                     *
*                                                                                               *
*       Notes:              1. This software has been written solely to show how to interface   *
*                              the ATmega128 to an AN221E04. No effort has been made to set up  *
*                              the ADC, JTAG, USART etc... These should be done by the user.    *
*                           2. This software was tested on the Mega128-DEV development board    *
*                              from Progressive Resources LLC (www.prllc.com) and the           *
*                              AN221D04-DEV Anadigm development board.                          *
*                                                                                               *
*************************************************************************************************
*                                                                                               *
*       Author:             Dave Lovell                                                         *
*       First Release:      5-Feb-04                                                            *
*                                                                                               *
*       Update History:                                                                         *
*                                                                                               *
*       Rev    Date       Author  Description of Change                                         *
*       -----  ---------  ------  ------------------------------------------------------------  *
*       1.0    5-Feb-04   DL      Preliminary release.                                          *
*                                                                                               *
*************************************************************************************************
*                                                                                               *
*   Copyright (C) 2004, Anadigm All rights reserved.                                            *
*                                                                                               *
*   No part of this document may be reproduced, stored in a retrieval system, or transmitted,   *
*   in any form or by any means, electronic, mechanical, photocopying, recording, or            *
*   otherwise, without the prior written permission of Anadigm.                                 *
*                                                                                               *
\***********************************************************************************************/

#include <io.h>							//this file calls up iom128.h which defines things like PORTA & PIN0 etc
#include <pgmspace.h>						//defines commands for using ROM e.g. 'prog_uchar' and '__elpm_inline'

#define F_CPU	14745600					//clock freq = 14.7456MHz
#define Clks_per_cycle	7					//number of clocks per cycle of while loop in msDelay function
#define Cycles_per_ms	(F_CPU / (1000 * Clks_per_cycle))	//number of cycles of while loop per mS in msDelay function

#define AN221_EXECUTE_Low	cbi(PORTA, PIN0)		//drive EXECUTE low
#define AN221_PORb_High		sbi(PORTA, PIN1)		//drive PORb high
#define AN221_ACTIVATE		bit_is_set(PINA, PIN2)		//sense ACTIVATE high
#define AN221_NO_ERROR		bit_is_set(PINA, PIN3)		//sense ERRb high i.e. no rror
#define AN221_ERRb_Input	outp(0xEB, DDRA)		//make uP drive ERRb
#define AN221_ERRb_Output	outp(0xE3, DDRA)		//make uP sense ERRb
#define AN221_ERRb_Low		cbi(PORTA, PIN3)		//drive ERRb low
#define AN221_ERRb_High		sbi(PORTA, PIN3)		//drive ERRb high
#define AN221_DIN_Low		cbi(PORTA, PIN5)		//drive DIN low
#define AN221_DIN_High		sbi(PORTA, PIN5)		//drive DIN high
#define AN221_DCLK_Low		cbi(PORTA, PIN6)		//drive DCLK low
#define AN221_DCLK_High		sbi(PORTA, PIN6)		//drive DCLK high
#define AN221_CS2b_Low		cbi(PORTA, PIN7)		//drive CS2b low
								
#define Red_LED_ON		cbi(PORTB, PIN0)		//turn on red LED - drive PB0 low
#define Red_LED_OFF		sbi(PORTB, PIN0)		//turn off red LED - drive PB0 high
#define Yellow_LED_ON		cbi(PORTB, PIN2)		//turn on yellow LED - drive PB2 low
#define Yellow_LED_OFF		sbi(PORTB, PIN2)		//turn off yellow LED - drive PB2 high
#define Green_LED_ON		cbi(PORTB, PIN3)		//turn on green LED - drive PB3 low
#define Green_LED_OFF		sbi(PORTB, PIN3)		//turn off green LED - drive PB3 high
#define Pushbutton_1		bit_is_clear(PINF, PIN0)	//pushbutton shorts PF0 to ground
#define Pushbutton_2		bit_is_clear(PINF, PIN1)	//pushbutton shorts PF1 to ground
#define Pushbutton_3		bit_is_clear(PINF, PIN2)	//pushbutton shorts PF2 to ground
#define Pushbutton_4		bit_is_clear(PINF, PIN3)	//pushbutton shorts PF3 to ground

#define PrimarySize1 		155				//155 bytes in primary bit stream for circuit #1
#define PrimarySize2 		155				//155 bytes in primary bit stream for circuit #2
#define PrimarySize3 		155				//155 bytes in primary bit stream for circuit #3
#define PrimarySize4 		155				//155 bytes in primary bit stream for circuit #4

#define UpdateSize1		34				//34 bytes in reconfig bit stream for circuit #1
#define UpdateSize2		34				//34 bytes in reconfig bit stream for circuit #2
#define UpdateSize3		34				//34 bytes in reconfig bit stream for circuit #3
#define UpdateSize4		34				//34 bytes in reconfig bit stream for circuit #4

#define chipID			0x01				//ID of FPAA


/* There follows 4 primary bit streams contained in 4 arrays stored in ROM. These bitstreams create circuits containing
   a Sinewave oscillator CAM with it's output connected to O1P/O1N & O2P/O2N. The 4 bitstreams correspond to an output
   frequency of 1kHz, 2kHz, 3kHz, & 4kHz respectively. The user should replace these bitstreams with ones that he has
   generated for his own circuits using the state driven method in AnadigmDesigner2.
   After the 4 primary bit streams there are 4 update bitstreams, also stored in ROM. These are for reconfiguring the
   frequency of the sinewave oscillator to 1 of the 4 frequencies using much shorter bitstreams with no dummy bytes and
   no reset required. Using updates, the change from one frequency to the next is continuous with no break in the
   sinewave.
*/

prog_uchar PrimaryBitStream1[PrimarySize1] =	//set up array in ROM for bitstream #1
{  //circuit #1 - 1kHz sinewave oscillator. User should replace the config bitstream below with his own
   /* The header for the configuration stream */
   0xD5, /* Synch     */
   0xB7, /* JTAG0     */
   0x22, /* JTAG1     */
   0x0 , /* JTAG2     */
   0x80, /* JTAG3     */
   chipID, /* Device ID */
   0x5 , /* Control   */

   /* Start of data block */
   0xCC, /* Byte address: 12 */
   0x0 , /* Bank address:  0 */
   0xC , /* Byte count:   12 */

   /* Data bytes for block */
   0x20,  0x0 ,  0x20,  0x4 ,  0x0 ,  0x2 ,  0x2 ,  0x61,  
   0x0 ,  0x8 ,  0xFF,  0x1 ,  

   /* Check Byte: Inverse Synch */
   0x2A,

   /* Start of data block */
   0xDE, /* Byte address: 30 */
   0x0 , /* Bank address:  0 */
   0x1 , /* Byte count:    1 */

   /* Data bytes for block */
   0xF ,  

   /* Check Byte: Inverse Synch */
   0x2A,

   /* Start of data block */
   0xCE, /* Byte address: 14 */
   0x1 , /* Bank address:  1 */
   0x48, /* Byte count:   72 */

   /* Data bytes for block */
   0x1 ,  0x48,  0x3F,  0x48,  0x3F,  0x1 ,  0x20,  0x20,  
   0xFF,  0x20,  0x4 ,  0xFF,  0x0 ,  0x0 ,  0x48,  0x3F,  
   0x48,  0x3F,  0x1 ,  0x1 ,  0x81,  0x2E,  0x1 ,  0x81,  
   0xE4,  0x0 ,  0x10,  0xE5,  0x0 ,  0x10,  0x0 ,  0x7 ,  
   0x1E,  0x1 ,  0x82,  0x7 ,  0x0 ,  0x0 ,  0x1 ,  0x1 ,  
   0x82,  0x95,  0x0 ,  0x20,  0x0 ,  0x95,  0x0 ,  0x20,  
   0x0 ,  0x48,  0x0 ,  0x5 ,  0xAD,  0x81,  0xAC,  0x0 ,  
   0x0 ,  0x0 ,  0x0 ,  0x8 ,  0x0 ,  0x98,  0x0 ,  0x0 ,  
   0x98,  0x8 ,  0x0 ,  0x0 ,  0x41,  0x9D,  0x81,  0xAC,  

   /* Check Byte: Inverse Synch */
   0x2A,

   /* Start of data block */
   0xDB, /* Byte address: 27 */
   0x3 , /* Bank address:  3 */
   0x5 , /* Byte count:    5 */

   /* Data bytes for block */
   0x90,  0x0 ,  0x90,  0xA ,  0x80,  

   /* Check Byte: Inverse Synch */
   0x2A,

   /* Start of data block */
   0xCD, /* Byte address: 13 */
   0x4 , /* Bank address:  4 */
   0x5 , /* Byte count:    5 */

   /* Data bytes for block */
   0x3 ,  0x0 ,  0x0 ,  0x0 ,  0x3 ,  

   /* Check Byte: Inverse Synch */
   0x2A,

   /* Start of data block */
   0xDE, /* Byte address: 30 */
   0x5 , /* Bank address:  5 */
   0x1 , /* Byte count:    1 */

   /* Data bytes for block */
   0xA ,  

   /* Check Byte: Inverse Synch */
   0x2A,

   /* Start of data block */
   0xCD, /* Byte address: 13 */
   0x6 , /* Bank address:  6 */
   0x5 , /* Byte count:    5 */

   /* Data bytes for block */
   0x3 ,  0x0 ,  0x0 ,  0x0 ,  0x3 ,  

   /* Check Byte: Inverse Synch */
   0x2A,

   /* Start of data block */
   0xDE, /* Byte address: 30 */
   0x7 , /* Bank address:  7 */
   0x1 , /* Byte count:    1 */

   /* Data bytes for block */
   0xA ,  

   /* Check Byte: Inverse Synch */
   0x2A,

   /* Start of data block */
   0xCD, /* Byte address: 13 */
   0x8 , /* Bank address:  8 */
   0x5 , /* Byte count:    5 */

   /* Data bytes for block */
   0x3 ,  0x0 ,  0x0 ,  0x0 ,  0x3 ,  

   /* Check Byte: Inverse Synch */
   0x2A,

   /* Start of data block */
   0x9E, /* Byte address: 30 */
   0x9 , /* Bank address:  9 */
   0x1 , /* Byte count:    1 */

   /* Data bytes for block */
   0xA ,  

   /* Check Byte: Inverse Synch */
   0x2A
};

prog_uchar PrimaryBitStream2[PrimarySize2] =	//set up array in ROM for bitstream #2
{  //circuit #2 - 2kHz sinewave oscillator. User should replace the config bitstream below with his own
   /* The header for the configuration stream */
   0xD5, /* Synch     */
   0xB7, /* JTAG0     */
   0x22, /* JTAG1     */
   0x0 , /* JTAG2     */
   0x80, /* JTAG3     */
   chipID, /* Device ID */
   0x5 , /* Control   */

   /* Start of data block */
   0xCC, /* Byte address: 12 */
   0x0 , /* Bank address:  0 */
   0xC , /* Byte count:   12 */

   /* Data bytes for block */
   0x20,  0x0 ,  0x20,  0x4 ,  0x0 ,  0x2 ,  0x2 ,  0x61,  
   0x0 ,  0x8 ,  0xFF,  0x1 ,  

   /* Check Byte: Inverse Synch */
   0x2A,

   /* Start of data block */
   0xDE, /* Byte address: 30 */
   0x0 , /* Bank address:  0 */
   0x1 , /* Byte count:    1 */

   /* Data bytes for block */
   0xF ,  

   /* Check Byte: Inverse Synch */
   0x2A,

   /* Start of data block */
   0xCE, /* Byte address: 14 */
   0x1 , /* Bank address:  1 */
   0x48, /* Byte count:   72 */

   /* Data bytes for block */
   0x1 ,  0x48,  0x3F,  0x48,  0x3F,  0x1 ,  0x20,  0x20,  
   0xFF,  0x20,  0x4 ,  0xFF,  0x0 ,  0x0 ,  0x48,  0x3F,  
   0x48,  0x3F,  0x2 ,  0x1 ,  0x81,  0x5C,  0x1 ,  0x81,  
   0xE4,  0x0 ,  0x10,  0xE4,  0x0 ,  0x10,  0x0 ,  0x7 ,  
   0x5A,  0x1 ,  0x82,  0x7 ,  0x0 ,  0x0 ,  0x3 ,  0x1 ,  
   0x82,  0xDF,  0x0 ,  0x20,  0x0 ,  0xE0,  0x0 ,  0x20,  
   0x0 ,  0x48,  0x0 ,  0x5 ,  0xAD,  0x81,  0xAC,  0x0 ,  
   0x0 ,  0x0 ,  0x0 ,  0x8 ,  0x0 ,  0x98,  0x0 ,  0x0 ,  
   0x98,  0x8 ,  0x0 ,  0x0 ,  0x41,  0x9D,  0x81,  0xAC,  

   /* Check Byte: Inverse Synch */
   0x2A,

   /* Start of data block */
   0xDB, /* Byte address: 27 */
   0x3 , /* Bank address:  3 */
   0x5 , /* Byte count:    5 */

   /* Data bytes for block */
   0x90,  0x0 ,  0x90,  0xA ,  0x80,  

   /* Check Byte: Inverse Synch */
   0x2A,

   /* Start of data block */
   0xCD, /* Byte address: 13 */
   0x4 , /* Bank address:  4 */
   0x5 , /* Byte count:    5 */

   /* Data bytes for block */
   0x3 ,  0x0 ,  0x0 ,  0x0 ,  0x3 ,  

   /* Check Byte: Inverse Synch */
   0x2A,

   /* Start of data block */
   0xDE, /* Byte address: 30 */
   0x5 , /* Bank address:  5 */
   0x1 , /* Byte count:    1 */

   /* Data bytes for block */
   0xA ,  

   /* Check Byte: Inverse Synch */
   0x2A,

   /* Start of data block */
   0xCD, /* Byte address: 13 */
   0x6 , /* Bank address:  6 */
   0x5 , /* Byte count:    5 */

   /* Data bytes for block */
   0x3 ,  0x0 ,  0x0 ,  0x0 ,  0x3 ,  

   /* Check Byte: Inverse Synch */
   0x2A,

   /* Start of data block */
   0xDE, /* Byte address: 30 */
   0x7 , /* Bank address:  7 */
   0x1 , /* Byte count:    1 */

   /* Data bytes for block */
   0xA ,  

   /* Check Byte: Inverse Synch */
   0x2A,

   /* Start of data block */
   0xCD, /* Byte address: 13 */
   0x8 , /* Bank address:  8 */
   0x5 , /* Byte count:    5 */

   /* Data bytes for block */
   0x3 ,  0x0 ,  0x0 ,  0x0 ,  0x3 ,  

   /* Check Byte: Inverse Synch */
   0x2A,

   /* Start of data block */
   0x9E, /* Byte address: 30 */
   0x9 , /* Bank address:  9 */
   0x1 , /* Byte count:    1 */

   /* Data bytes for block */
   0xA ,  

   /* Check Byte: Inverse Synch */
   0x2A
};

prog_uchar PrimaryBitStream3[PrimarySize3] =	//set up array in ROM for bitstream #3
{  //circuit #3 - 3kHz sinewave oscillator. User should replace the config bitstream below with his own
   /* The header for the configuration stream */
   0xD5, /* Synch     */
   0xB7, /* JTAG0     */
   0x22, /* JTAG1     */
   0x0 , /* JTAG2     */
   0x80, /* JTAG3     */
   chipID, /* Device ID */
   0x5 , /* Control   */

   /* Start of data block */
   0xCC, /* Byte address: 12 */
   0x0 , /* Bank address:  0 */
   0xC , /* Byte count:   12 */

   /* Data bytes for block */
   0x20,  0x0 ,  0x20,  0x4 ,  0x0 ,  0x2 ,  0x2 ,  0x61,  
   0x0 ,  0x8 ,  0xFF,  0x1 ,  

   /* Check Byte: Inverse Synch */
   0x2A,

   /* Start of data block */
   0xDE, /* Byte address: 30 */
   0x0 , /* Bank address:  0 */
   0x1 , /* Byte count:    1 */

   /* Data bytes for block */
   0xF ,  

   /* Check Byte: Inverse Synch */
   0x2A,

   /* Start of data block */
   0xCE, /* Byte address: 14 */
   0x1 , /* Bank address:  1 */
   0x48, /* Byte count:   72 */

   /* Data bytes for block */
   0x1 ,  0x48,  0x3F,  0x48,  0x3F,  0x1 ,  0x20,  0x20,  
   0xFF,  0x20,  0x4 ,  0xFF,  0x0 ,  0x0 ,  0x48,  0x3F,  
   0x48,  0x3F,  0x3 ,  0x1 ,  0x81,  0x8A,  0x1 ,  0x81,  
   0xE4,  0x0 ,  0x10,  0xE5,  0x0 ,  0x10,  0x0 ,  0x7 ,  
   0x78,  0x1 ,  0x82,  0x7 ,  0x0 ,  0x0 ,  0x4 ,  0x1 ,  
   0x82,  0xC6,  0x0 ,  0x20,  0x0 ,  0xC7,  0x0 ,  0x20,  
   0x0 ,  0x48,  0x0 ,  0x5 ,  0xAD,  0x81,  0xAC,  0x0 ,  
   0x0 ,  0x0 ,  0x0 ,  0x8 ,  0x0 ,  0x98,  0x0 ,  0x0 ,  
   0x98,  0x8 ,  0x0 ,  0x0 ,  0x41,  0x9D,  0x81,  0xAC,  

   /* Check Byte: Inverse Synch */
   0x2A,

   /* Start of data block */
   0xDB, /* Byte address: 27 */
   0x3 , /* Bank address:  3 */
   0x5 , /* Byte count:    5 */

   /* Data bytes for block */
   0x90,  0x0 ,  0x90,  0xA ,  0x80,  

   /* Check Byte: Inverse Synch */
   0x2A,

   /* Start of data block */
   0xCD, /* Byte address: 13 */
   0x4 , /* Bank address:  4 */
   0x5 , /* Byte count:    5 */

   /* Data bytes for block */
   0x3 ,  0x0 ,  0x0 ,  0x0 ,  0x3 ,  

   /* Check Byte: Inverse Synch */
   0x2A,

   /* Start of data block */
   0xDE, /* Byte address: 30 */
   0x5 , /* Bank address:  5 */
   0x1 , /* Byte count:    1 */

   /* Data bytes for block */
   0xA ,  

   /* Check Byte: Inverse Synch */
   0x2A,

   /* Start of data block */
   0xCD, /* Byte address: 13 */
   0x6 , /* Bank address:  6 */
   0x5 , /* Byte count:    5 */

   /* Data bytes for block */
   0x3 ,  0x0 ,  0x0 ,  0x0 ,  0x3 ,  

   /* Check Byte: Inverse Synch */
   0x2A,

   /* Start of data block */
   0xDE, /* Byte address: 30 */
   0x7 , /* Bank address:  7 */
   0x1 , /* Byte count:    1 */

   /* Data bytes for block */
   0xA ,  

   /* Check Byte: Inverse Synch */
   0x2A,

   /* Start of data block */
   0xCD, /* Byte address: 13 */
   0x8 , /* Bank address:  8 */
   0x5 , /* Byte count:    5 */

   /* Data bytes for block */
   0x3 ,  0x0 ,  0x0 ,  0x0 ,  0x3 ,  

   /* Check Byte: Inverse Synch */
   0x2A,

   /* Start of data block */
   0x9E, /* Byte address: 30 */
   0x9 , /* Bank address:  9 */
   0x1 , /* Byte count:    1 */

   /* Data bytes for block */
   0xA ,  

   /* Check Byte: Inverse Synch */
   0x2A
};

prog_uchar PrimaryBitStream4[PrimarySize4] =	//set up array in ROM for bitstream #4
{  //circuit #4 - 4kHz sinewave oscillator. User should replace the config bitstream below with his own
   /* The header for the configuration stream */
   0xD5, /* Synch     */
   0xB7, /* JTAG0     */
   0x22, /* JTAG1     */
   0x0 , /* JTAG2     */
   0x80, /* JTAG3     */
   chipID, /* Device ID */
   0x5 , /* Control   */

   /* Start of data block */
   0xCC, /* Byte address: 12 */
   0x0 , /* Bank address:  0 */
   0xC , /* Byte count:   12 */

   /* Data bytes for block */
   0x20,  0x0 ,  0x20,  0x4 ,  0x0 ,  0x2 ,  0x2 ,  0x61,  
   0x0 ,  0x8 ,  0xFF,  0x1 ,  

   /* Check Byte: Inverse Synch */
   0x2A,

   /* Start of data block */
   0xDE, /* Byte address: 30 */
   0x0 , /* Bank address:  0 */
   0x1 , /* Byte count:    1 */

   /* Data bytes for block */
   0xF ,  

   /* Check Byte: Inverse Synch */
   0x2A,

   /* Start of data block */
   0xCE, /* Byte address: 14 */
   0x1 , /* Bank address:  1 */
   0x48, /* Byte count:   72 */

   /* Data bytes for block */
   0x1 ,  0x48,  0x3F,  0x48,  0x3F,  0x1 ,  0x20,  0x20,  
   0xFF,  0x20,  0x4 ,  0xFF,  0x0 ,  0x0 ,  0x48,  0x3F,  
   0x48,  0x3F,  0x4 ,  0x1 ,  0x81,  0xB7,  0x1 ,  0x81,  
   0xE3,  0x0 ,  0x10,  0xE4,  0x0 ,  0x10,  0x0 ,  0x7 ,  
   0xB4,  0x1 ,  0x82,  0x7 ,  0x0 ,  0x0 ,  0x6 ,  0x1 ,  
   0x82,  0xDF,  0x0 ,  0x20,  0x0 ,  0xE0,  0x0 ,  0x20,  
   0x0 ,  0x48,  0x0 ,  0x5 ,  0xAD,  0x81,  0xAC,  0x0 ,  
   0x0 ,  0x0 ,  0x0 ,  0x8 ,  0x0 ,  0x98,  0x0 ,  0x0 ,  
   0x98,  0x8 ,  0x0 ,  0x0 ,  0x41,  0x9D,  0x81,  0xAC,  

   /* Check Byte: Inverse Synch */
   0x2A,

   /* Start of data block */
   0xDB, /* Byte address: 27 */
   0x3 , /* Bank address:  3 */
   0x5 , /* Byte count:    5 */

   /* Data bytes for block */
   0x90,  0x0 ,  0x90,  0xA ,  0x80,  

   /* Check Byte: Inverse Synch */
   0x2A,

   /* Start of data block */
   0xCD, /* Byte address: 13 */
   0x4 , /* Bank address:  4 */
   0x5 , /* Byte count:    5 */

   /* Data bytes for block */
   0x3 ,  0x0 ,  0x0 ,  0x0 ,  0x3 ,  

   /* Check Byte: Inverse Synch */
   0x2A,

   /* Start of data block */
   0xDE, /* Byte address: 30 */
   0x5 , /* Bank address:  5 */
   0x1 , /* Byte count:    1 */

   /* Data bytes for block */
   0xA ,  

   /* Check Byte: Inverse Synch */
   0x2A,

   /* Start of data block */
   0xCD, /* Byte address: 13 */
   0x6 , /* Bank address:  6 */
   0x5 , /* Byte count:    5 */

   /* Data bytes for block */
   0x3 ,  0x0 ,  0x0 ,  0x0 ,  0x3 ,  

   /* Check Byte: Inverse Synch */
   0x2A,

   /* Start of data block */
   0xDE, /* Byte address: 30 */
   0x7 , /* Bank address:  7 */
   0x1 , /* Byte count:    1 */

   /* Data bytes for block */
   0xA ,  

   /* Check Byte: Inverse Synch */
   0x2A,

   /* Start of data block */
   0xCD, /* Byte address: 13 */
   0x8 , /* Bank address:  8 */
   0x5 , /* Byte count:    5 */

   /* Data bytes for block */
   0x3 ,  0x0 ,  0x0 ,  0x0 ,  0x3 ,  

   /* Check Byte: Inverse Synch */
   0x2A,

   /* Start of data block */
   0x9E, /* Byte address: 30 */
   0x9 , /* Bank address:  9 */
   0x1 , /* Byte count:    1 */

   /* Data bytes for block */
   0xA ,  

   /* Check Byte: Inverse Synch */
   0x2A
};

prog_uchar UpdateBitStream1[UpdateSize1] =		//set up array of circuit #1 update bitstream in ROM
{  //circuit #1 - 1kHz sinewave oscillator. User should replace the update bitstream below with his own
   /* The header for the configuration stream */
   0xD5, /* Synch     */
   chipID, /* Device ID */
   0x5 , /* Control   */

   /* Start of data block */
   0xC0, /* Byte address:  0 */
   0x2 , /* Bank address:  2 */
   0xA , /* Byte count:   10 */

   /* Data bytes for block */
   0x1 ,  0x1 ,  0x81,  0x2E,  0x1 ,  0x81,  0xE4,  0x0 ,  
   0x10,  0xE5,  

   /* Check Byte: Inverse Synch */
   0x2A,

   /* Start of data block */
   0xCE, /* Byte address: 14 */
   0x2 , /* Bank address:  2 */
   0x1 , /* Byte count:    1 */

   /* Data bytes for block */
   0x1E,  

   /* Check Byte: Inverse Synch */
   0x2A,

   /* Start of data block */
   0x94, /* Byte address: 20 */
   0x2 , /* Bank address:  2 */
   0x8 , /* Byte count:    8 */

   /* Data bytes for block */
   0x1 ,  0x1 ,  0x82,  0x95,  0x0 ,  0x20,  0x0 ,  0x95,  

   /* Check Byte: Inverse Synch */
   0x2A
};

prog_uchar UpdateBitStream2[UpdateSize2] =		//set up array of circuit #2 update bitstream in ROM
{  //circuit #2 - 2kHz sinewave oscillator. User should replace the update bitstream below with his own
  /* The header for the configuration stream */
   0xD5, /* Synch     */
   chipID, /* Device ID */
   0x5 , /* Control   */

   /* Start of data block */
   0xC0, /* Byte address:  0 */
   0x2 , /* Bank address:  2 */
   0xA , /* Byte count:   10 */

   /* Data bytes for block */
   0x2 ,  0x1 ,  0x81,  0x5C,  0x1 ,  0x81,  0xE4,  0x0 ,  
   0x10,  0xE4,  

   /* Check Byte: Inverse Synch */
   0x2A,

   /* Start of data block */
   0xCE, /* Byte address: 14 */
   0x2 , /* Bank address:  2 */
   0x1 , /* Byte count:    1 */

   /* Data bytes for block */
   0x5A,  

   /* Check Byte: Inverse Synch */
   0x2A,

   /* Start of data block */
   0x94, /* Byte address: 20 */
   0x2 , /* Bank address:  2 */
   0x8 , /* Byte count:    8 */

   /* Data bytes for block */
   0x3 ,  0x1 ,  0x82,  0xDF,  0x0 ,  0x20,  0x0 ,  0xE0,  

   /* Check Byte: Inverse Synch */
   0x2A
};

prog_uchar UpdateBitStream3[UpdateSize3] =		//set up array of circuit #3 update bitstream in ROM
{  //circuit #3 - 3kHz sinewave oscillator. User should replace the update bitstream below with his own
   /* The header for the configuration stream */
   0xD5, /* Synch     */
   chipID, /* Device ID */
   0x5 , /* Control   */

   /* Start of data block */
   0xC0, /* Byte address:  0 */
   0x2 , /* Bank address:  2 */
   0xA , /* Byte count:   10 */

   /* Data bytes for block */
   0x3 ,  0x1 ,  0x81,  0x8A,  0x1 ,  0x81,  0xE4,  0x0 ,  
   0x10,  0xE5,  

   /* Check Byte: Inverse Synch */
   0x2A,

   /* Start of data block */
   0xCE, /* Byte address: 14 */
   0x2 , /* Bank address:  2 */
   0x1 , /* Byte count:    1 */

   /* Data bytes for block */
   0x78,  

   /* Check Byte: Inverse Synch */
   0x2A,

   /* Start of data block */
   0x94, /* Byte address: 20 */
   0x2 , /* Bank address:  2 */
   0x8 , /* Byte count:    8 */

   /* Data bytes for block */
   0x4 ,  0x1 ,  0x82,  0xC6,  0x0 ,  0x20,  0x0 ,  0xC7,  

   /* Check Byte: Inverse Synch */
   0x2A
};

prog_uchar UpdateBitStream4[UpdateSize4] =		//set up array of circuit #4 update bitstream in ROM
{  //circuit #4 - 4kHz sinewave oscillator. User should replace the update bitstream below with his own
   /* The header for the configuration stream */
   0xD5, /* Synch     */
   chipID, /* Device ID */
   0x5 , /* Control   */

   /* Start of data block */
   0xC0, /* Byte address:  0 */
   0x2 , /* Bank address:  2 */
   0xA , /* Byte count:   10 */

   /* Data bytes for block */
   0x4 ,  0x1 ,  0x81,  0xB7,  0x1 ,  0x81,  0xE3,  0x0 ,  
   0x10,  0xE4,  

   /* Check Byte: Inverse Synch */
   0x2A,

   /* Start of data block */
   0xCE, /* Byte address: 14 */
   0x2 , /* Bank address:  2 */
   0x1 , /* Byte count:    1 */

   /* Data bytes for block */
   0xB4,  

   /* Check Byte: Inverse Synch */
   0x2A,

   /* Start of data block */
   0x94, /* Byte address: 20 */
   0x2 , /* Bank address:  2 */
   0x8 , /* Byte count:    8 */

   /* Data bytes for block */
   0x6 ,  0x1 ,  0x82,  0xDF,  0x0 ,  0x20,  0x0 ,  0xE0,  

   /* Check Byte: Inverse Synch */
   0x2A
};

/******************************************************************************
 *                                                                            *
 * Procedure Name : ms_Delay                                                  *
 *                                                                            *
 * Purpose        : This function creates a delay in mS                       *
 *                                                                            *
 * Inputs         : unsigned int delay - the delay in mS                      *
 *                                                                            *
 * Outputs        : None                                                      *
 *                                                                            *
 ******************************************************************************/

void msDelay(unsigned int delay)
{
	unsigned int cycles;
	
	while (delay--)
	{
		cycles = Cycles_per_ms;
		while (cycles--);
	}
}

/******************************************************************************
 *                                                                            *
 * Procedure Name : AN221_Clock                                               *
 *                                                                            *
 * Purpose        : This function clocks the FPAA once                        *
 *                                                                            *
 * Inputs         : None                                                      *
 *                                                                            *
 * Outputs        : None                                                      *
 *                                                                            *
 ******************************************************************************/

void AN221_Clock(void)
{
	AN221_DCLK_High;
	AN221_DCLK_Low;
}

/******************************************************************************
 *                                                                            *
 * Procedure Name : AN221_Write                                               *
 *                                                                            *
 * Purpose        : This function writes a data byte to the FPAA serially     *
 *                                                                            *
 * Inputs         : unsigned char data - the data byte to be written          *
 *                                                                            *
 * Outputs        : None                                                      *
 *                                                                            *
 ******************************************************************************/

void AN221_Write(unsigned char data)
{
	unsigned char bit = 8;
		
	while (bit)
	{
		if (data & 0x80)
			AN221_DIN_High;		//if MSB=1 then set data pin high
		else
			AN221_DIN_Low;		//if MSB=0 then set data pin low
		AN221_Clock();			//pulse DCLK once
		data <<= 1;			//left shift the data by 1 bit
		bit--;				//dec the value of bit
	}
	AN221_DIN_Low;				//return DIN low
}

/******************************************************************************
 *                                                                            *
 * Procedure Name : AN221_Reset                                               *
 *                                                                            *
 * Purpose        : This function sends a soft reset command to the chip      *
 *                  chain. All devices in the chain should be reset if the    *
 *                  chipID parameter is 0xFF. Otherwise the specific chip     *
 *                  with that chip ID will be reset.                          *
 *                                                                            *
 * Inputs         : unsigned char chip - The device to be reset. 0xFF will    *
 *                                       reset all devices. Any other value   *
 *                                       will reset only the device with a    *
 *                                       matching ID.                         *
 *                                                                            *
 * Outputs        : None                                                      *
 *                                                                            *
 ******************************************************************************/

void AN221_Reset(unsigned char chip)
{
	unsigned char i;
	
	if (chip == 0xFF)
	{
		AN221_ERRb_Input;			//make uP drive ERRb
		AN221_ERRb_Low;				//drive ERRb low
		for (i = 0; i < 16; i++)
			AN221_Clock();			//wait 16 clocks
		AN221_ERRb_High;			//drive ERRb high
		AN221_ERRb_Output;			//make uP sense ERRb again
		
		for (i = 0; i < 32; i++)
			AN221_Clock();			//send 32 clocks
	}
	else
	{		
		for (i = 0; i < 5; i++)
			AN221_Write(0x00);		//send 5 dummy bytes

		AN221_Write(0xD5);			//send synch byte
		AN221_Write(chip);			//send chip ID
		AN221_Write(0x48);			//send 0x48 to control byte to do soft reset

		for (i = 0; i < 32; i++)
			AN221_Clock();			//send 32 clocks
		
		msDelay(35);				//and wait 35mS
	}
}

/******************************************************************************
 *                                                                            *
 * Procedure Name : AN221_Reconfig                                            *
 *                                                                            *
 * Purpose        : This function does a reconfiguration (update) of the      *
 *                  circuit in the FPAA                                       *
 *                                                                            *
 * Inputs         : char update - the update to be loaded                     *
 *                                                                            *
 * Outputs        : None                                                      *
 *                                                                            *
 ******************************************************************************/

void AN221_Reconfig(char update)
{
	unsigned char i;
	
	Yellow_LED_ON;							//turn on yellow LED to show configuration is taking place
	Red_LED_OFF;							//turn off the red LED
	Green_LED_OFF;							//turn off the green LED

	switch (update)
	{
		case '1':
			for (i = 0 ; i < UpdateSize1 ; i++)
				AN221_Write(__elpm_inline(&UpdateBitStream1[i]));	//send update bitstream #1 (from ROM)
			break;
		case '2':
			for (i = 0 ; i < UpdateSize2 ; i++)
				AN221_Write(__elpm_inline(&UpdateBitStream2[i]));	//send update bitstream #2 (from ROM)
			break;
		case '3':
			for (i = 0 ; i < UpdateSize3 ; i++)
				AN221_Write(__elpm_inline(&UpdateBitStream3[i]));	//send update bitstream #3 (from ROM)
			break;
		case '4':
			for (i = 0 ; i < UpdateSize4 ; i++)
				AN221_Write(__elpm_inline(&UpdateBitStream4[i]));	//send update bitstream #4 (from ROM)
			break;
	}
	
	AN221_Write(0x00);						//send 1 dummy byte (don't need dummy bytes at start of reconfig)
		
	if (AN221_ACTIVATE && AN221_NO_ERROR)				//if ACTIVATE is high & no error then....
	{
		Green_LED_ON;						//turn on green LED &...
		Red_LED_OFF;						//turn off red LED
	}
	else								//else if ACTIVATE not high or ERRb is low then....
	{
		Red_LED_ON;						//turn on red LED &...
		Green_LED_OFF;						//turn off green LED
	}
	Yellow_LED_OFF;							//configuration over so turn off yellow LED
}


/******************************************************************************
 *                                                                            *
 * Procedure Name : AN221_PriConfig                                           *
 *                                                                            *
 * Purpose        : This function does a primary configuration of the circuit *
 *                  into the FPAA   				              *
 *                                                                            *
 * Inputs         : char circuit - the circuit to be loaded                   *
 *                                                                            *
 * Outputs        : None                                                      *
 *                                                                            *
 ******************************************************************************/

void AN221_PriConfig(char circuit)
{
	unsigned int i;
	
	Yellow_LED_ON;							//turn on yellow LED to show configuration is taking place
	Red_LED_OFF;							//turn off the red LED
	Green_LED_OFF;							//turn off the green LED
	
	AN221_Reset(chipID);						//reset the FPAA
	
	for (i = 0 ; i < 5 ; i++)
		AN221_Write(0x00);					//send 5 dummy bytes
		
	switch (circuit)
	{
		case '1':
			for (i = 0 ; i < PrimarySize1 ; i++)
				AN221_Write(__elpm_inline(&PrimaryBitStream1[i]));	//send config bitstream #1 (from ROM)
			break;
		case '2':
			for (i = 0 ; i < PrimarySize2 ; i++)
				AN221_Write(__elpm_inline(&PrimaryBitStream2[i]));	//send config bitstream #2 (from ROM)
			break;
		case '3':
			for (i = 0 ; i < PrimarySize3 ; i++)
				AN221_Write(__elpm_inline(&PrimaryBitStream3[i]));	//send config bitstream #3 (from ROM)
			break;
		case '4':
			for (i = 0 ; i < PrimarySize4 ; i++)
				AN221_Write(__elpm_inline(&PrimaryBitStream4[i]));	//send config bitstream #4 (from ROM)
			break;
	}
		
	AN221_Write(0x00);						//send 1 dummy byte
	
	if (AN221_ACTIVATE && AN221_NO_ERROR)				//if ACTIVATE is high & no error then....
	{
		Green_LED_ON;						//turn on green LED &...
		Red_LED_OFF;						//turn off red LED
	}
	else								//else if ACTIVATE not high or ERRb is low then....
	{
		Red_LED_ON;						//turn on red LED &...
		Green_LED_OFF;						//turn off green LED
	}
	Yellow_LED_OFF;							//configuration over so turn off yellow LED
}

/******************************************************************************
 *                                                                            *
 * Procedure Name : initialize                                                *
 *                                                                            *
 * Purpose        : This function takes care of initializing the processor    *
 *                  and the FPAA 	                                      *
 *                                                                            *
 * Inputs         : None                                                      *
 *                                                                            *
 * Outputs        : None                                                      *
 *                                                                            *
 ******************************************************************************/

void initialize(void)
{
	outp(0xE3, DDRA);		//set PA4 (LCCb) & PA3 (ERRb) & PA2 (ACTIVATE) as inputs, rest as outputs
	outp(0xFF, DDRB);		//set all port B to outputs to drive LEDs
	outp(0x00, DDRF);		//set all port F as inputs for pushbuttons
	PORTA = 0x00;			//disable pull-ups on PA2,3,4
	PORTB = 0xFF;			//all LEDs off
	PORTF = 0xFF;			//enable pull-ups on port F
	 
	Red_LED_OFF;			//turn off red LED
	Green_LED_OFF;			//turn off green LED
	Yellow_LED_OFF;			//turn off yellow LED

	AN221_DCLK_Low;			//set up initial state of FPAA pins
	AN221_DIN_Low;
	AN221_EXECUTE_Low;
	AN221_CS2b_Low;
	AN221_PORb_High;
	
	msDelay(20);			//wait a bit
}

/******************************************************************************
 *                                                                            *
 * Procedure Name : main                                                      *
 *                                                                            *
 * Purpose        : This is our main application entry point. It looks for 1  *
 *                  of 4 buttons being pressed & loads the corresponding      *
 *                  reconfiguration bitstream. The user can change these for  *
 *                  primary configurations if required. Reconfigurations use  *
 *                  far fewer bytes & there is no need to reset the chip, so  *
 *                  there is no break in analog performance.                  *
 *                                                                            *
 * Inputs         : None                                                      *
 *                                                                            *
 * Outputs        : int 1 - redundant but compiler seems to require it        *
 *                                                                            *
 ******************************************************************************/

int main(void)
{
	initialize();					//set up uP and FPAA
	
	AN221_PriConfig('1');				//load circuit #1 as default
	
	while (1)
	{
		msDelay(1000);				//wait a second
	
		if (Pushbutton_1)
			AN221_Reconfig('1');		//load update 1 if pushbutton 1 pressed
		else if (Pushbutton_2)
			AN221_Reconfig('2');		//load update 2 if pushbutton 2 pressed
		else if (Pushbutton_3)
			AN221_Reconfig('3');		//load update 3 if pushbutton 3 pressed
		else if (Pushbutton_4)
			AN221_Reconfig('4');		//load update 4 if pushbutton 4 pressed
	}
	return 1;
}
