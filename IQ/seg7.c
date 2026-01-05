#include <avr/io.h>
#include <avr/pgmspace.h>

#ifndef BOOTLOADER
unsigned char LPChar;	// holds the current 7 segment character
unsigned char Priority;	// holds the current 7 segment priority, high priority writes will override low priority writes
#endif

void Seg7A(char on){
	PORTE.DIRSET = PIN0_bm;
	PORTE.PIN0CTRL = PORT_OPC_BUSKEEPER_gc;
	if(on){
		PORTE.OUTCLR = PIN0_bm;
	}
	else{
		PORTE.OUTSET = PIN0_bm;
	}
}

void Seg7B(char on){
	PORTE.DIRSET = PIN1_bm;
	PORTE.PIN1CTRL = PORT_OPC_BUSKEEPER_gc;
	if(on){
		PORTE.OUTCLR = PIN1_bm;
	}
	else{
		PORTE.OUTSET = PIN1_bm;
	}
}

void Seg7C(char on){
	
	if(MCU.JTAGUID == 0xff){
		PORTE.DIRSET = PIN2_bm;
		PORTE.PIN2CTRL = PORT_OPC_BUSKEEPER_gc;
		if(on){
			PORTE.OUTCLR = PIN2_bm;
		}
		else{
			PORTE.OUTSET = PIN2_bm;
		}
	}
	else{
		
		PORTF.DIRSET = PIN4_bm;
		PORTF.PIN4CTRL = PORT_OPC_BUSKEEPER_gc;
		if(on){
			PORTF.OUTCLR = PIN4_bm;
		}
		else{
			PORTF.OUTSET = PIN4_bm;
		}
	}
}

void Seg7D(char on){
	if(MCU.JTAGUID == 0xff){
		PORTE.DIRSET = PIN3_bm;
		PORTE.PIN3CTRL = PORT_OPC_BUSKEEPER_gc;
		if(on){
			PORTE.OUTCLR = PIN3_bm;
		}
		else{
			PORTE.OUTSET = PIN3_bm;
		}
	}
	else{
		PORTF.DIRSET = PIN5_bm;
		PORTF.PIN5CTRL = PORT_OPC_BUSKEEPER_gc;
		if(on){
			PORTF.OUTCLR = PIN5_bm;
		}
		else{
			PORTF.OUTSET = PIN5_bm;
		}
	}
}

void Seg7E(char on){
	PORTE.DIRSET = PIN4_bm;
	PORTE.PIN4CTRL = PORT_OPC_BUSKEEPER_gc;
	if(on){
		PORTE.OUTCLR = PIN4_bm;
	}
	else{
		PORTE.OUTSET = PIN4_bm;
	}
}

void Seg7F(char on){
	PORTE.DIRSET = PIN5_bm;
	PORTE.PIN6CTRL = PORT_OPC_BUSKEEPER_gc;
	if(on){
		PORTE.OUTCLR = PIN5_bm;
	}
	else{
		PORTE.OUTSET = PIN5_bm;
	}
}

void Seg7G(char on){
	PORTE.DIRSET = PIN6_bm;
	PORTE.PIN5CTRL = PORT_OPC_BUSKEEPER_gc;
	if(on){
		PORTE.OUTCLR = PIN6_bm;
	}
	else{
		PORTE.OUTSET = PIN6_bm;
	}
}

void Seg7DP(char on){
	PORTE.DIRSET = PIN7_bm;
	PORTE.PIN7CTRL = PORT_OPC_BUSKEEPER_gc;
	if(on){
		PORTE.OUTCLR = PIN7_bm;
	}
	else{
		PORTE.OUTSET = PIN7_bm;
	}
}

#ifndef BOOTLOADER

const unsigned char Seg7Table[128] PROGMEM = {
//  .gfedcba
					 0,	// (sp)   32 0x20 
	0b00000001,	// !      33 0x21	// baseline bar for QC
	0b00000010,	// "      34 0x22 // two bars
	0b00000100,	// #      35 0x23 // three bars
	0b00001000,	// $      36 0x24 
	0b00010000,	// %      37 0x25 
	0b00100000,	// &      38 0x26 
	0b01000000,	// '      39 0x27 
	0b00111001,	// (      40 0x28 
	0b00001111,	// )      41 0x29 
	0b00001000,	// *      42 0x2a // QC baseline, 1 bar
	0b01001000,	// +      43 0x2b // QC fair, 2 bars
	0b01001001,	// ,      44 0x2c // QC good, 3 bars
	0b01000000,	// -      45 0x2d 
	0b10000000,	// .      46 0x2e 
					 0,	// /      47 0x2f 
	0b00111111,	// 0      48 0x30 
	0b00000110,	// 1      49 0x31 
	0b01011011,	// 2      50 0x32 
	0b01001111,	// 3      51 0x33 
	0b01100110,	// 4      52 0x34 
	0b01101101,	// 5      53 0x35 
	0b01111101,	// 6      54 0x36 
	0b00000111,	// 7      55 0x37 
	0b01111111,	// 8      56 0x38 
	0b01100111,	// 9      57 0x39 
					 0,	// :      58 0x3a 
					 0,	// ;      59 0x3b 
					 0,	// <      60 0x3c 
	0b01001000,	// =      61 0x3d 
					 0,	// >      62 0x3e 
	0b01010011,	// ?      63 0x3f 
					 0,	// @      64 0x40 
	0b01110111,	// A      65 0x41 
	0b01111100,	// B      66 0x42 
	0b00111001,	// C      67 0x43 
	0b01011110,	// D      68 0x44 
	0b01111001,	// E      69 0x45 
	0b01110001,	// F      70 0x46 
	0b00111101,	// G      71 0x47 
	0b01110110,	// H      72 0x48 
	0b00000110,	// I      73 0x49 
	0b00011110,	// J      74 0x4a 
					 0,	// K      75 0x4b 
	0b00111000,	// L      76 0x4c 
					 0,	// M      77 0x4d 
	0b00110111,	// N      78 0x4e 
	0b00111111,	// O      79 0x4f 
	0b01110011,	// P      80 0x50 
					 0,	// Q      81 0x51 
	0b01010000,	// R      82 0x52 
	0b01101101,	// S      83 0x53 
	0b01111000,	// T      84 0x54 
	0b00111110,	// U      85 0x55 
					 0,	// V      86 0x56 
					 0,	// W      87 0x57 
					 0,	// X      88 0x58 
					 0,	// Y      89 0x59 
					 0,	// Z      90 0x5a 
	0b00111001,	// [      91 0x5b 
					 0,	// \      92 0x5c 
	0b00001111,	// ]      93 0x5d 
					 0,	// ^      94 0x5e 
	0b00001000,	// _      95 0x5f 
	0b00100000,	// `      96 0x60
	0b01110111,	// a      97 0x61
	0b01111100,	// b      98 0x62
	0b01011000,	// c      99 0x63
	0b01011110,	// d     100 0x64
	0b01111001,	// e     101 0x65
	0b01110001,	// f     102 0x66
	0b01101111,	// g     103 0x67
	0b01110100,	// h     104 0x68
	0b00000100,	// i     105 0x69
	0b00011110,	// j     106 0x6a
					 0,	// k     107 0x6b
	0b00000110,	// l     108 0x6c
					 0,	// m     109 0x6d
	0b01010100,	// n     110 0x6e
	0b01011100,	// o     111 0x6f
	0b01110011,	// p     112 0x70
					 0,	// q     113 0x71
	0b01010000,	// r     114 0x72
	0b01101101,	// s     115 0x73
	0b01111000,	// t     116 0x74
	0b00011100,	// u     117 0x75
					 0,	// v     118 0x76
					 0,	// w     119 0x77
					 0,	// x     120 0x78
					 0,	// y     121 0x79
					 0,	// z     122 0x7a
	0b00111001,	// {     123 0x7b
	0b00000110,	// |     124 0x7c
	0b00001111,	// }     125 0x7d
	0b00100000,	// ~     126 0x7e
					 0,	// (del) 127 0x7f
};

//------------------------------------------------------------------------------------
// Display the specified character on the LED
void Seg7(unsigned char c){

	if(c<' '){
		return;
	}

	c&=0x7f;	// constrain to ASCII
	c-=' '; 	// lop off the control characters

	c = pgm_read_byte(&Seg7Table[c]);	// get the segment map

	Seg7A(0);	// all segments off
	Seg7B(0);
	Seg7C(0);
	Seg7D(0);
	Seg7E(0);
	Seg7F(0);
	Seg7G(0);

	if(c & 1){
 		Seg7A(1);
	}
	if(c & 2){
		Seg7B(1);
	}
	if(c & 4){
		Seg7C(1);
	}
	if(c & 8){
		Seg7D(1);
	}
	if(c & 0x10){
		Seg7E(1);
	}
	if(c & 0x20){
		Seg7G(1);  // bit 5 (f) = top left = physical Seg7G
	}
	if(c & 0x40){
		Seg7F(1);  // bit 6 (g) = middle = physical Seg7F
	}
}


//------------------------------------------------------------------------------------
// If the current display priority is low, display the specified character on the LED
// Store the character for recall when the priority lowers or if it gets pre-empted
void Seg7LP(unsigned char c){
	LPChar = c;	// remember this in case it gets pre-empted by a HP character
	if(Priority==0){
		Seg7(c);
	}
}

//------------------------------------------------------------------------------------
// Display the specified character on the LED, regardless of what's already showing.
// If the character is not a blank, set the display priority to 1 to block low level accesses
// If the character is blank set the display priority to 0 to allow low level accesses 
void Seg7HP(unsigned char c){
	if(c == ' '){
		Priority = 0;
		Seg7LP(LPChar);	// restore whatever was displaying at low priority
	}
	else{
		Priority = 1;
	}
	Seg7(c);
}

static void SegTest()
{
	// Turn all segments off initially
	Seg7A(0);
	Seg7B(0);
	Seg7C(0);
	Seg7D(0);
	Seg7E(0);
	Seg7F(0);
	Seg7G(0);
	Seg7DP(0);
	_delay_ms(250);

	// Turn on each segment one at a time
	Seg7A(1); 
	_delay_ms(250); 
	Seg7A(0);
	Seg7B(1); 
	_delay_ms(250); 
	Seg7B(0);
	Seg7C(1); 
	_delay_ms(250); 
	Seg7C(0);
	Seg7D(1); 
	_delay_ms(250); 
	Seg7D(0);
	Seg7E(1); 
	_delay_ms(250); 
	Seg7E(0);
	Seg7F(1); 
	_delay_ms(250); 
	Seg7F(0);
	Seg7G(1); 
	_delay_ms(250); 
	Seg7G(0);
	Seg7DP(1); 
	_delay_ms(250); 
	Seg7DP(0);

}
//------------------------------------------------------------------------------------
// initialze the seven segment LED for zero power consumption and set priority to low level
void InitSeg7(void){
	Priority = 0;	// set current priority to low
	//SegTest();
	Seg7LP(' ');	// turn off all segments
	Seg7DP(0);		// turn off the decimal point
}

#endif