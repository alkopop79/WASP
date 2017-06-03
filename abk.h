/************************************************************************************************
*                                                                            					*
* File Name		:	abk.h                                               						*
*                                                                            					*
* Purpose		: 	header file for abk.c            											*	
*                                                                            					*
* Author		:	Dave Lovell                                               					*
*                                                                            					*
* Date			:	3rd May 2005                                                 				*
*                                                                            					*
************************************************************************************************/
#device *=16 ADC=8
#use delay(clock=18432000)											//PIC clock = 18.432MHz
#fuses HS,NOWDT,PUT,NOPROTECT,NOBROWNOUT,NOLVP,NOCPD,WRT,NODEBUG	//set the CONFIG bits
#use rs232(baud=57600, xmit=PIN_C6, rcv=PIN_C7)  					//set up RS232

// Defines for communications
#define STX 			   	0x02
#define ETX 			   	0x03
#define XON 			   	17
#define XOFF 				19
#define null 			  	0x00
#define OK 				   	0
#define CONFIG_ERROR 		1
#define UNKNOWN_COMMAND		4

// Define commands
#define version_info 		'#'
#define sw_status 			'?'
#define config_chip 	   	'0'
#define update_chip 	   	'1'
#define read_PIC_SPI 		'W'
#define read_PIC_ADC 		'V'
#define reset_chips 	   	'R'
#define identify_chips 		'T'
#define command_set 	   	'S'
#define pin_status 			'P'
#define execute_pulse		'E'

// Define port inputs (A 1-bit makes a pin an input)
#define port_a_inputs 		0x3F	//P8 (4 pin header), SARSYNC, LCCb
#define port_b_inputs   	0xC5	//ACTIVATE, ERRb, J23 (programming jumper), CTS
#define port_c_inputs		0x9C    //RX (from RS232), SARDATA, SARCLK, LCCb

// Define pins for FPAA signals
#define ACTIVATE 			PIN_B7	
#define ERRb 				PIN_B6	
#define CS2b 				PIN_B3
#define LCCb				PIN_C2
#define EXECUTE 			PIN_A4
#define DIN  				PIN_C1
#define DCLK 				PIN_C0
#define SYNCBIT				PIN_A5

// Defines for turning ERRb pin around
#define make_ERR_input		0xFF
#define make_ERR_output		0xBF

// Define pins for handshaking & LEDs
#define CTS_to_PC  			PIN_B1			//Clear To Send to the PC (DTE)
#define RTS_from_PC   		PIN_B2			//Request To Send from the PC (DTE)
#define CONFIG_OK 			PIN_B4			//green LED
#define CONFIG_ERR			PIN_B5			//red LED

// RS-232 Communications receive buffer and status variables
#define BufferSize 			40
int8 Status_flag = OK;
int8 InputBuf[BufferSize];
int8 BufferHead = 0;
int8 BufferTail = 0;
int8 BufferFull = 0;
int8 BufferEmpty = 1;

// Chip detect defines
#define an_AN120			0
#define an_AN121			1
#define an_AN220			2
#define an_AN221			3

#define an_apiChipCount		4
#define MaxChips			20

int8 ChipCount = 0;
int8 ChipType[MaxChips];

typedef	int8 an_Byte;

// Function declarations
int8 RS_232_GetData(void);
#int_rda
void interrupt(void);
int8 getHEXNumber(void);
void send_byte_to_chip(int8 data);
void config_clocks(void);
void reset_chip(int8 chipID);
void configure_chip_from_input(int8 PrimaryConfig);
int8 readADC(int8 channel);
int8 readSPI(void);
void initialize(void);
int8 TestChipType(int8 Type, int8 SuppressPullup);
int1 ChipsPresent(void);
void DetectChips(void);
void pinTest(void);
