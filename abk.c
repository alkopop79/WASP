/******************************************************************************
 *                                                                            *
 * Program : Anadigm Boot Kernel v3.3                                         *
 *           Anadigm, Inc.                                                    *
 *           Copyright © 2005. All Rights Reserved.                           *
 *                                                                            *
 * Author  : Dave Lovell                                      				  *
 *                                                                            *
 * Date    : 3rd May 2005                                                     *
 *                                                                            *
 * Target  : AN221K04-DEV Development Board with PIC16F876                    *
 *                                                                            *
 * Notes   : See readme.txt for details of changes in this version.           *
 *                                                                            *
 ******************************************************************************/

#include <16F876.h>
#include "abk.h"
						
// Configuration bitstreams stored in ROM (for chip detect routines)
const an_Byte an_AN120_PrimaryConfigInfo[] =
{
   0xD5,
   0xB7,
   0x12,
   0x0 ,
   0x30,
   0x2
};

const an_Byte an_AN121_PrimaryConfigInfo[] =
{
   0xD5,
   0xB7,
   0x12,
   0x0 ,
   0x80,
   0x4
};

const an_Byte an_AN220_PrimaryConfigInfo[] =
{
   0xD5,
   0xB7,
   0x22,
   0x0 ,
   0x30,
   0x1
};

const an_Byte an_AN221_PrimaryConfigInfo[] =
{
   0xD5,
   0xB7,
   0x22,
   0x0 ,
   0x80,
   0x3
};

const an_Byte an_ANx2x_PrimaryConfigInfo[] =
{
   0xCC,
   0x0 ,
   0xC ,

   0x20,  0x0 ,  0x20,  0x4 ,  0x0 ,  0x2 ,  0x0 ,  0x7F,
   0x0 ,  0x8 ,  0xFF,  0x1 ,

   0x2A,

   0xDE,
   0x0 ,
   0x1 ,

   0xF ,

   0x2A,

   0xCE,
   0x1 ,
   0x12,

   0x1 ,  0x48,  0x3F,  0x48,  0x3F,  0x1 ,  0x20,  0x0 ,
   0xFF,  0x20,  0x0 ,  0xFF,  0x0 ,  0x0 ,  0x48,  0x3F,
   0x48,  0x3F,

   0x2A,

   0xCD,
   0x2 ,
   0x5 ,

   0x3 ,  0x0 ,  0x0 ,  0x0 ,  0x3 ,

   0x2A,

   0xDE,
   0x3 ,
   0x1 ,

   0xA ,

   0x2A,

   0xCD,
   0x4 ,
   0x5 ,

   0x3 ,  0x0 ,  0x0 ,  0x0 ,  0x3 ,

   0x2A,

   0xDE,
   0x5 ,
   0x1 ,

   0xA ,

   0x2A,

   0xCD,
   0x6 ,
   0x5 ,

   0x3 ,  0x0 ,  0x0 ,  0x0 ,  0x3 ,

   0x2A,

   0xDE,
   0x7 ,
   0x1 ,

   0xA ,

   0x2A,

   0xCD,
   0x8 ,
   0x5 ,

   0x3 ,  0x0 ,  0x0 ,  0x0 ,  0x3 ,

   0x2A,

   0x9E,
   0x9 ,
   0x1 ,

   0xA ,
   0x2A
};

/******************************************************************************
 *                                                                            *
 * Procedure Name : RS_232_GetData                                            *
 *                                                                            *
 * Purpose        : This function retrieves information from the receive      *
 *                  buffer, and returns it to the caller.                     *
 *                                                                            *
 * Inputs         : None                                                      *
 *                                                                            *
 * Outputs        : int8 - The first data available in the FIFO buffer.       *
 *                                                                            *
 ******************************************************************************/
int8 RS_232_GetData(void)
{
	int8 Result;

	while (BufferEmpty);				// Wait until a character is available
	Result = InputBuf[BufferTail];		// Retrieve the data from the buffer...
	BufferTail++;						// ...and update the buffer pointers.
	if (BufferTail == BufferSize)
		BufferTail = 0;
	BufferFull = 0;
	BufferEmpty = (BufferTail == BufferHead);

	return Result;
}

/******************************************************************************
 *                                                                            *
 * Procedure Name : interrupt                                                 *
 *                                                                            *
 * Purpose        : This function provides the interrupt handler for the PIC  *
 *                  microcontroller. It handles all serial transactions.      *
 *                                                                            *
 * Inputs         : None                                                      *
 *                                                                            *
 * Outputs        : None                                                      *
 *                                                                            *
 ******************************************************************************/
#int_rda
void interrupt(void)
{
	if (kbhit())
	{
		InputBuf[BufferHead] = getc();
		BufferHead++;
		if (BufferHead == BufferSize)
			BufferHead = 0;
		BufferFull = (BufferHead == BufferTail);
		BufferEmpty = 0;
	}
}

/******************************************************************************
 *                                                                            *
 * Procedure Name : getHEXNumber                                              *
 *                                                                            *
 * Purpose        : This function retrieves an ASCII hex value from the       *
 *                  serial port and converts it to binary.                    *
 *                                                                            *
 * Inputs         : None                                                      *
 *                                                                            *
 * Outputs        : int8 - The binary value that was read as ASCII            *
 *                                                                            *
 ******************************************************************************/
int8 getHEXNumber(void)
{
	int8 Result, Result2, input;

	// Retrieve the most-significant nibble
	input = RS_232_GetData();
	if (input >= 'a')
		input = (input - ('a' - 'A'));
	if (input >= 'A')
		Result = ((input - 'A') + 10);
	else
		Result = input - '0';
	Result <<= 4;

	// Retrieve the least-significant nibble
	input = RS_232_GetData();
	if (input >= 'a')
		input = (input - ('a' - 'A'));
	if (input >= 'A')
		Result2 = ((input - 'A') + 10);
	else
		Result2 = (input - '0');

	return (Result + Result2);
}

/******************************************************************************
 *                                                                            *
 * Procedure Name : send_byte_to_chip                                         *
 *                                                                            *
 * Purpose        : This function sends a byte of data to the FPAA.           *
 *                                                                            *
 * Inputs         : int8 data - The data byte to send to the FPAA.   		  *
 *                                                                            *
 * Outputs        : None                                                      *
 *                                                                            *
 ******************************************************************************/
void send_byte_to_chip(int8 data)
{
	int8 bit = 8;

	while (bit)
	{
		if (data & 0x80)
			output_high(DIN);			//if MSB=1 then set data pin high
		output_high(DCLK);				//set DCLK pin high
		output_low(DCLK);				//set DCLK pin low
		output_low(DIN);				//set data pin low
		data <<= 1;						//left shift the data by 1 bit
		bit--;							//dec the value of bit
	}
}

/******************************************************************************
 *                                                                            *
 * Procedure Name : config_clocks                                             *
 *                                                                            *
 * Purpose        : This function provides a way to run the configuration     *
 *                  clock to the FPAA 16 times. It is needed during software  *
 *                  resets of the devices.                                    *
 *                                                                            *
 * Inputs         : None                                                      *
 *                                                                            *
 * Outputs        : None                                                      *
 *                                                                            *
 ******************************************************************************/
void config_clocks(void)
{
		output_high(DCLK);
		output_low(DCLK);
		output_high(DCLK);
		output_low(DCLK);
		output_high(DCLK);
		output_low(DCLK);
		output_high(DCLK);
		output_low(DCLK);
		output_high(DCLK);
		output_low(DCLK);
		output_high(DCLK);
		output_low(DCLK);
		output_high(DCLK);
		output_low(DCLK);
		output_high(DCLK);
		output_low(DCLK);
		output_high(DCLK);
		output_low(DCLK);
		output_high(DCLK);
		output_low(DCLK);
		output_high(DCLK);
		output_low(DCLK);
		output_high(DCLK);
		output_low(DCLK);
		output_high(DCLK);
		output_low(DCLK);
		output_high(DCLK);
		output_low(DCLK);
		output_high(DCLK);
		output_low(DCLK);
		output_high(DCLK);
		output_low(DCLK);
}

/******************************************************************************
 *                                                                            *
 * Procedure Name : reset_chip                                                *
 *                                                                            *
 * Purpose        : This function sends a soft reset command to the chip      *
 *                  chain. All devices in the chain should be reset if the    *
 *                  chipID parameter is 0xFF. Otherwise the specific chip     *
 *                  with that chip ID will be reset.                          *
 *                                                                            *
 * Inputs         : int8 chipID - The device to be reset. 0xFF will reset all *
 *                                devices. Any other value will reset only the*
 *                                device with a matching ID. 				  *
 *                                                                            *
 * Outputs        : None                                                      *
 *                                                                            *
 ******************************************************************************/
void reset_chip(int8 chipID)
{
	int8 loop;

	putc(XOFF);

	if (chipID == 0xFF)
	{
		set_tris_b(port_b_inputs & make_ERR_output);		//make PIC drive ERRb
		output_low(ERRb);									//& drive it low
		config_clocks();									//wait 16 clocks
		output_high(ERRb);									//drive it high
		set_tris_b(port_b_inputs & make_ERR_input);			//& make PIC sense ERRb again
	}
	else
	{
		for (loop = 0; loop < 5; loop++)
			send_byte_to_chip(0x00);						//dummy bytes

		send_byte_to_chip(0xD5);
		send_byte_to_chip(chipID);
		send_byte_to_chip(0x48);							//software reset
	}
	config_clocks();										//send 32 clocks
	config_clocks();
	putc(XON);
}

/******************************************************************************
 *                                                                            *
 * Procedure Name : configure_chip_from_input                                 *
 *                                                                            *
 * Purpose        : This function provides a conduit for data coming from the *
 *                  serial port and sends it to the FPAA.                     *
 *                                                                            *
 * Inputs         : int8 PrimaryConfig - if == config_chip then starts with   *
 *                   					reset, if == update_chip then no reset*
 *                                                                            *
 * Outputs        : None                                                      *
 *                                                                            *
 ******************************************************************************/
void configure_chip_from_input(int8 PrimaryConfig)
{
	int16	num_bytes;	// The number of bytes to be transferred
	int8	data_read;  // Data read from the serial port.

	Status_flag = OK;

	if (PrimaryConfig)
		reset_chip(0xFF);

	// Retrieve the number of bytes we will be moving.
	num_bytes = getHEXNumber();
	num_bytes = (num_bytes << 8) + getHEXNumber();

	// Move the data as long as we have data to send, and we haven't timed out
	while (num_bytes)
	{
		data_read = getHEXNumber();
		if (Status_flag == OK)
		{
			send_byte_to_chip(data_read);

			if (!(input(ERRb)))
				Status_flag = CONFIG_ERROR;
		}
		num_bytes--;
		num_bytes--;
	}
}

/******************************************************************************
 *                                                                            *
 * Procedure Name : readADC                                                   *
 *                                                                            *
 * Purpose        : This function reads one of the Analog-to-Digital          *
 *                  converter channels and returns the result.                *
 *                                                                            *
 * Inputs         : int8 channel - The A/D channel to read.                   *
 *                                                                            *
 * Outputs        : int8 - The 8-bit result of the channel read.              *
 *                                                                            *
 ******************************************************************************/
int8 readADC(int8 channel)
{
	set_adc_channel(channel);
	delay_us(10);
	return (Read_ADC());
}

/******************************************************************************
 *                                                                            *
 * Procedure Name : readSPI                                                   *
 *                                                                            *
 * Purpose        : This function reads the SPI port and returns the result.  *
 *                  If there is no activity on the SPI port, it returns 255.  *
 *                                                                            *
 * Inputs         : none                                                      *
 *                                                                            *
 * Outputs        : int8 - The 8-bit result                          		  *
 *                                                                            *
 ******************************************************************************/
int8 readSPI(void)
{
	int16 SPIalive = 65535;
	int8 SPIdata = 255;
	
	setup_adc_ports(RA0_RA1_ANALOG_RA3_RA2_REF);	//make RA5 (SARSync) a digital pin
	while (input(SYNCBIT) && SPIalive)				//wait for sync to go low or timeout
		SPIalive--;
	if (SPIalive)									//if sync is active then go get reading
	{	
		SPIdata = SPI_read();						//do a dummy read to empty SPI port buffer
		while (!spi_data_is_in());					//wait for new data to appear
		SPIdata = SPI_read();						//read SPI port
	}
	setup_adc_ports(ALL_ANALOG);					//make RA0-5 all analog inputs again
	return SPIdata;
}

/******************************************************************************
 *                                                                            *
 * Procedure Name : initialize                                                *
 *                                                                            *
 * Purpose        : This function sets up the initial configuration of the PIC*
 *                                                                            *
 * Inputs         : None                                                      *
 *                                                                            *
 * Outputs        : None                                                      *
 *                                                                            *
 ******************************************************************************/
void initialize(void)
{
	enable_interrupts(GLOBAL);		//enable global interrupts
	enable_interrupts(INT_RDA);		//enable interrupt on RS232 receive data
	port_b_pullups(false);			//disable port B pull-ups

	//set up connections between PIC & FPAA
	set_tris_a(port_a_inputs);
	set_tris_b(port_b_inputs);
	set_tris_c(port_c_inputs);
	output_high(CS2b);
	output_low(EXECUTE);
	output_low(DCLK);
	output_low(DIN);
	output_high(CONFIG_OK);	     //turn on green LED
	output_high(CONFIG_ERR);     //turn on red LED

	// Configure RS-232 port
	setup_uart(57600);

	// Configure the A2D converter
	setup_adc_ports(ALL_ANALOG);
   	setup_adc(ADC_CLOCK_INTERNAL);

	// Configure the SPI Interface
	setup_spi(SPI_SLAVE | SPI_H_TO_L);					//set up SPI port

	delay_ms(200);					//wait for power on reset to finish (needs 35ms)
	output_low(CONFIG_OK);	//turn off green LED
	output_low(CONFIG_ERR);	//turn off red LED
}

/******************************************************************************
 *                                                                            *
 * Procedure Name : TestChipType                                              *
 *                                                                            *
 * Purpose        : This function tests the FPAA type                         *
 *                                                                            *
 * Inputs         : int8 Type - FPAA version to test                 		  *
 *                : int8 SuppressPullup - suppress ACT pull-up flag  		  *
 *                                                                            *
 * Outputs        : int8 - returns 1 if FPAA type tested +ve         		  *
 *                                                                            *
 ******************************************************************************/
int8 TestChipType(int8 Type, int8 SuppressPullup)
{
	int8 ChipData, Loop;


	for (Loop = 0; Loop < 5; Loop++)
		send_byte_to_chip(0x00);

	for (Loop = 0; Loop < 6; Loop++)
	{
		switch (Type)
		{
			case an_AN120:
				ChipData = an_AN120_PrimaryConfigInfo[Loop];
			break;
			case an_AN121:
				ChipData = an_AN121_PrimaryConfigInfo[Loop];
			break;
			case an_AN220:
				ChipData = an_AN220_PrimaryConfigInfo[Loop];
			break;
			case an_AN221:
				ChipData = an_AN221_PrimaryConfigInfo[Loop];
			break;
			default:
			break;
		}
		send_byte_to_chip(ChipData);

		if ((ChipData) && (!(input(ERRb))))
		{
			config_clocks();
			return 0;
		}
	}

	if (SuppressPullup)
		send_byte_to_chip(0x04);
	else
		send_byte_to_chip(0x05);

	for (Loop = 0; Loop < 99; Loop++)
		send_byte_to_chip(an_ANx2x_PrimaryConfigInfo[Loop]);

	config_clocks();
	return 1;
}

/******************************************************************************
 *                                                                            *
 * Procedure Name : ChipsPresent                                              *
 *                                                                            *
 * Purpose        : This function tests the FPAA type                         *
 *                                                                            *
 * Inputs         : none                                                      *
 *                                                                            *
 * Outputs        : int1 - returns 1 if FPAA(s) present, 0 if not             *
 *                                                                            *
 ******************************************************************************/
int1 ChipsPresent(void)
{
	const int8 ChipData[] = { 0xD5, 0xB7, 0xFF, 0xFF, 0xFF, 0x00 };
	int8 DataPtr;

	config_clocks();

   //Send illegal data and see if an error occurs. If ERRb goes low, we know there is a chip listening to us.
	for (DataPtr = 0; DataPtr < 6; DataPtr++)
	{
		send_byte_to_chip(ChipData[DataPtr]);

		if (!(input(ERRb)))
			return 1;
	}
	return 0;
}

/******************************************************************************
 *                                                                            *
 * Procedure Name : DetectChips                                               *
 *                                                                            *
 * Purpose        : This function returns the types of FPAAs in the system    *
 *                                                                            *
 * Inputs         : none                                                      *
 *                                                                            *
 * Outputs        : none                                                      *
 *                                                                            *
 ******************************************************************************/
void DetectChips(void)
{
	int8 TypeLoop, ChipLoop, Abort = 0;

	ChipCount = 0;

	reset_chip(0xFF);

	if (ChipsPresent())
	{
		while ((ChipCount < MaxChips) && (!(input(ACTIVATE))) && (!Abort))
		{
			for (TypeLoop = an_AN120; TypeLoop <= an_AN221; TypeLoop++)
			{
				reset_chip(0xFF);

				for (ChipLoop = 0; ChipLoop < ChipCount; ChipLoop++)
					TestChipType(ChipType[ChipLoop], 1);

				if (TestChipType(TypeLoop, 0))
				{
					ChipType[ChipCount++] = TypeLoop;
					switch (TypeLoop)
					{
						case an_AN120:
							printf("120E04|");
						break;
						case an_AN121:
							printf("121E04|");
						break;
						case an_AN220:
							printf("220E04|");
						break;
						case an_AN221:
							printf("221E04|");
						break;
					}
					break;
				}
				else
				{
					if (TypeLoop == an_AN221)
					{
						printf("UNKNOWN|");
						Abort = 1;
					}
				}
			}
		}
	}
	printf("END");
	putc(null);

	reset_chip(0xFF);
}

/******************************************************************************
 *                                                                            *
 * Procedure Name : pinTest                                                   *
 *                                                                            *
 * Purpose        : This function checks the state of the ACTIVATE or ERR pin.*
 *                  The 'P' command must be followed by an 'A' or 'E' to tell *
 *                  the function which pin to test. Then by an 'H' or 'L' for *
 *                  high or low. Finally, the user should do a s/w status     *
 *                  check to determine if the quizzed state was correct.      *
 *                                                                            *
 * Inputs         : None                                                      *
 *                                                                            *
 * Outputs        : None                                                      *
 *                                                                            *
 ******************************************************************************/
void pinTest(void)
{
	int8 Pin, PinStateAssert;

	Pin = RS_232_GetData();
	if ((Pin != 'A') && (Pin != 'E'))
	{
		Status_flag = UNKNOWN_COMMAND;
		return;
	}
	PinStateAssert = RS_232_GetData();
	if ((PinStateAssert != 'H') && (PinStateAssert != 'h') &&
	    (PinStateAssert != 'L') && (PinStateAssert != 'l'))
	{
		Status_flag = UNKNOWN_COMMAND;
		return;
	}

	switch (Pin)
	{
		case 'A' :
		{
			switch (PinStateAssert)
			{
				case 'H':
				case 'h':
						if (input(ACTIVATE) != 1)
							Status_flag = CONFIG_ERROR;
						break;
				case 'L':
				case 'l':
						if (input(ACTIVATE) != 0)
							Status_flag = CONFIG_ERROR;
						break;
			}
		}
		break;
		case 'E' :
		{
			switch (PinStateAssert)
			{
				case 'H':
				case 'h':
						if (input(ERRb) != 1)
							Status_flag = CONFIG_ERROR;
						break;
				case 'L':
				case 'l':
						if (input(ERRb) != 0)
							Status_flag = CONFIG_ERROR;
						break;
			}
		}
		break;
	}
}

/******************************************************************************
 *                                                                            *
 * Procedure Name : main                                                      *
 *                                                                            *
 * Purpose        : This is our main application entry point. It runs the     *
 *                  command processor, which accepts commands from the serial *
 *                  port and processes them.                                  *
 *                                                                            *
 * Inputs         : None                                                      *
 *                                                                            *
 * Outputs        : None                                                      *
 *                                                                            *
 ******************************************************************************/
void main(void)
{
	int8 command;

	initialize();			// set up inital configuration of the PIC & FPAA

	output_high(CTS_to_PC);	//deassert clear to send to the PC

	while (1)
	{
		// Check serial port to see if a command has started. Commands start with a "STX" character (0x02)
		while (input(RTS_from_PC));			//wait until we get a request to send from the PC
		output_low(CTS_to_PC);				//assert clear to send to the PC
		command = RS_232_GetData();
		if (command == STX)
		{
			command = RS_232_GetData();
			if (command != sw_status)
				Status_flag = OK;
			switch (command)
			{
				case config_chip:		// The command '0' does a full configuration
				case update_chip:		// The command '1' does a reconfiguration
					{
						output_low(CONFIG_OK);
						output_low(CONFIG_ERR);
						output_low(CS2b);
						configure_chip_from_input(command == config_chip);
						if (Status_flag == OK)
							output_high(CONFIG_OK);
						else
							output_high(CONFIG_ERR);
					}
					break;
				case version_info:		// The command '#' gives back the version information on the board.
					{
						printf("Anadigm AN221K04-DEV BOARD v2.0x\n");
                     	printf("ABK Version: 3.3\nRev Date: 3 May 2005\n");
						putc(null);
					}
					break;
				case sw_status:			// The command '?' returns the current software status.
					{
						putc('0' + Status_flag);
					}
					break;
				case read_PIC_ADC:		// The command 'V' reads the ADC of the PIC
					{
						putc(readADC(RS_232_GetData()));
					}
					break;
				case read_PIC_SPI: 		// The command 'W' reads the SPI port of the PIC
					{
						putc(readSPI());
					}
					break;
				case reset_chips:		// The command 'R' resets the FPAA
					{
						output_low(CONFIG_OK);
						output_low(CONFIG_ERR);
						reset_chip(getHEXNumber());
					}
					break;
				case identify_chips:	// The command 'T' returns the types of FPAAs in the system
					{
						output_low(CONFIG_OK);
						output_low(CONFIG_ERR);
						output_low(CS2b);
						DetectChips();
					}
					break;
				case command_set:		// The command 'S' returns the set of commands
					{
						printf("01EPRSTVW?#");
						putc(null);
					}
					break;
				case pin_status:		// The command 'P' returns the state of the ACTIVATE or ERR pin.
					{
						pinTest();
					}
					break;
				case execute_pulse:		// The command 'E' pulses the EXECUTE pin for 1us with 2 DCLKs
					{
						output_high(EXECUTE);
						output_high(DCLK);
						output_low(DCLK);
						output_high(DCLK);
						output_low(DCLK);
						output_low(EXECUTE);
					}
					break;
				default:
					{
						Status_flag = UNKNOWN_COMMAND;
					}
					break;
			}
			command = 0x00;
			while (command != ETX)			//look for ETX (0x03)
				command = RS_232_GetData();
		}
		output_high(CTS_to_PC);		//deassert clear to send to the PC
	}
}
