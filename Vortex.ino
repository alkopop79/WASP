/*

*/
#define chipID			0x01
#define PrimarySize1 		155				//155 bytes in primary bit stream for circuit #1
#define PrimarySize2 		155				//155 bytes in primary bit stream for circuit #2
#define PrimarySize3 		155				//155 bytes in primary bit stream for circuit #3
#define PrimarySize4 		155				//155 bytes in primary bit stream for circuit #4

#define UpdateSize1		34				//34 bytes in reconfig bit stream for circuit #1
#define UpdateSize2		34				//34 bytes in reconfig bit stream for circuit #2
#define UpdateSize3		34				//34 bytes in reconfig bit stream for circuit #3
#define UpdateSize4		34				//34 bytes in reconfig bit stream for circuit #4

int ACTIVATE = 2;
int EXECUTE = 3;
int DCLK = 4;
int DIN = 5;
int ERRb = 6;
int CS2b = 7;

int Yellow_LED = 8;
int Red_LED = 9;
int Green_LED = 10;

const char PrimaryBitStream1[PrimarySize1] =	//set up array in ROM for bitstream #1
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

const char UpdateBitStream1[UpdateSize1] =		//set up array of circuit #1 update bitstream in ROM
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

const char PrimaryBitStream2[PrimarySize2] =	//set up array in ROM for bitstream #2
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

const char PrimaryBitStream3[PrimarySize3] =	//set up array in ROM for bitstream #3
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

const char PrimaryBitStream4[PrimarySize4] =	//set up array in ROM for bitstream #4
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

// const char UpdateBitStream1[UpdateSize1] =		//set up array of circuit #1 update bitstream in ROM
// {  //circuit #1 - 1kHz sinewave oscillator. User should replace the update bitstream below with his own
//   /* The header for the configuration stream */
//   0xD5, /* Synch     */
//   chipID, /* Device ID */
//   0x5 , /* Control   */

//   /* Start of data block */
//   0xC0, /* Byte address:  0 */
//   0x2 , /* Bank address:  2 */
//   0xA , /* Byte count:   10 */

//   /* Data bytes for block */
//   0x1 ,  0x1 ,  0x81,  0x2E,  0x1 ,  0x81,  0xE4,  0x0 ,  
//   0x10,  0xE5,  

//   /* Check Byte: Inverse Synch */
//   0x2A,

//   /* Start of data block */
//   0xCE, /* Byte address: 14 */
//   0x2 , /* Bank address:  2 */
//   0x1 , /* Byte count:    1 */

//   /* Data bytes for block */
//   0x1E,  

//   /* Check Byte: Inverse Synch */
//   0x2A,

//   /* Start of data block */
//   0x94, /* Byte address: 20 */
//   0x2 , /* Bank address:  2 */
//   0x8 , /* Byte count:    8 */

//   /* Data bytes for block */
//   0x1 ,  0x1 ,  0x82,  0x95,  0x0 ,  0x20,  0x0 ,  0x95,  

//   /* Check Byte: Inverse Synch */
//   0x2A
// };

const char UpdateBitStream2[UpdateSize2] =		//set up array of circuit #2 update bitstream in ROM
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

const char UpdateBitStream3[UpdateSize3] =		//set up array of circuit #3 update bitstream in ROM
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

const char UpdateBitStream4[UpdateSize4] =		//set up array of circuit #4 update bitstream in ROM
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


void setup() {
  
  Serial.begin(9600);
  Serial.print("Setting up");
  pinMode(ACTIVATE,INPUT);
  pinMode(EXECUTE,OUTPUT);
  pinMode(DCLK,OUTPUT);
  pinMode(DIN,OUTPUT);
  pinMode(ERRb, INPUT);
  pinMode(Yellow_LED,OUTPUT);
  pinMode(Red_LED,OUTPUT);
  pinMode(Green_LED,OUTPUT);
  
  digitalWrite(Yellow_LED,LOW);
  digitalWrite(Red_LED,LOW);
  digitalWrite(Green_LED,LOW);
  
  digitalWrite(DCLK,LOW);
  digitalWrite(DIN,LOW);
  digitalWrite(EXECUTE,LOW);
  digitalWrite(CS2b,LOW);
  
  delay(20);
    
}

void loop() {

    
}

void AN221_Clock()
{
digitalWrite(DCLK,HIGH);
digitalWrite(DCLK,LOW);
}

void AN221_Write(unsigned char data)
{
	unsigned char bit = 8;
		
	while (bit)
	{
		if (data & 0x80)
			digitalWrite(DIN,HIGH);
		else
			digitalWrite(DIN,LOW);		//if MSB=0 then set data pin low
		AN221_Clock();			//pulse DCLK once
		data <<= 1;			//left shift the data by 1 bit
		bit--;				//dec the value of bit
	}
	digitalWrite(DIN,LOW);
}

void AN221_Reset(unsigned char chip)
{
	unsigned char i;
	Serial.print("Configuration reset");
	
	if (chip == 0xFF)
	{
		pinMode(ERRb, OUTPUT);			//make uP drive ERRb
		digitalWrite(ERRb,LOW);				//drive ERRb low
		for (i = 0; i < 16; i++)
			AN221_Clock();			//wait 16 clocks
		digitalWrite(ERRb,HIGH);			//drive ERRb high
		pinMode(ERRb, INPUT);			//make uP sense ERRb again
		
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
		
		delay(35);				//and wait 35mS
	}
}

void AN221_Reconfig(char update)
{
	unsigned char i;
	Serial.print("Reconfiguaration");
	digitalWrite(Yellow_LED,HIGH);							//turn on yellow LED to show configuration is taking place
	digitalWrite(Red_LED,LOW);							//turn off the red LED
	digitalWrite(Green_LED,LOW);							//turn off the green LED

	switch (update)
	{
		case '1':
			for (i = 0 ; i < UpdateSize1 ; i++)
				AN221_Write(&UpdateBitStream1[i]);	//send update bitstream #1 (from ROM)
			break;
		case '2':
			for (i = 0 ; i < UpdateSize2 ; i++)
				AN221_Write(&UpdateBitStream2[i]);	//send update bitstream #2 (from ROM)
			break;
		case '3':
			for (i = 0 ; i < UpdateSize3 ; i++)
				AN221_Write(&UpdateBitStream3[i]);	//send update bitstream #3 (from ROM)
			break;
		case '4':
			for (i = 0 ; i < UpdateSize4 ; i++)
				AN221_Write(&UpdateBitStream4[i]);	//send update bitstream #4 (from ROM)
			break;
	}
	
	AN221_Write(0x00);						//send 1 dummy byte (don't need dummy bytes at start of reconfig)
	
	int ACTIVATE_state = digitalRead(ACTIVATE);
	int ERRb_state = digitalRead(ERRb);
		
	if (ACTIVATE_state && ERRb_state)				//if ACTIVATE is high & no error then....
	{
		digitalWrite(Green_LED,HIGH);						//turn on green LED &...
		digitalWrite(Red_LED,LOW);						//turn off red LED
	}
	else								//else if ACTIVATE not high or ERRb is low then....
	{
		digitalWrite(Red_LED,HIGH);						//turn on red LED &...
		digitalWrite(Green_LED,LOW);						//turn off green LED
	}
	digitalWrite(Yellow_LED,LOW);							//configuration over so turn off yellow LED
}

void AN221_PriConfig(char circuit)
{
	unsigned int i;
	Serial.print("Preconfiguration");
	digitalWrite(Yellow_LED,HIGH);							//turn on yellow LED to show configuration is taking place
	digitalWrite(Red_LED,LOW);							//turn off the red LED
	digitalWrite(Green_LED,LOW);							//turn off the green LED
	Serial.print("Configuration begin...");
	
	AN221_Reset(chipID);						//reset the FPAA
	
	for (i = 0 ; i < 5 ; i++)
		AN221_Write(0x00);					//send 5 dummy bytes
		
	switch (circuit)
	{
		case '1':
			for (i = 0 ; i < PrimarySize1 ; i++)
				AN221_Write(&PrimaryBitStream1[i]);	//send config bitstream #1 (from ROM)
			break;
		case '2':
			for (i = 0 ; i < PrimarySize2 ; i++)
				AN221_Write(&PrimaryBitStream2[i]);	//send config bitstream #2 (from ROM)
			break;
		case '3':
			for (i = 0 ; i < PrimarySize3 ; i++)
				AN221_Write(&PrimaryBitStream3[i]);	//send config bitstream #3 (from ROM)
			break;
		case '4':
			for (i = 0 ; i < PrimarySize4 ; i++)
				AN221_Write(&PrimaryBitStream4[i]);	//send config bitstream #4 (from ROM)
			break;
	}
		
	AN221_Write(0x00);						//send 1 dummy byte
	
	int ACTIVATE_state = digitalRead(ACTIVATE);
	int ERRb_state = digitalRead(ERRb);
		
	if (ACTIVATE_state && ERRb_state)				//if ACTIVATE is high & no error then....
	{
		digitalWrite(Green_LED,HIGH);						//turn on green LED &...
		digitalWrite(Red_LED,LOW);						//turn off red LED
	}
	else								//else if ACTIVATE not high or ERRb is low then....
	{
		digitalWrite(Red_LED,HIGH);						//turn on red LED &...
		digitalWrite(Green_LED,LOW);						//turn off green LED
		Serial.print("Configuration error!");
	}
	digitalWrite(Yellow_LED,LOW);							//configuration over so turn off yellow LED
	Serial.print("Configuration finished");
}



