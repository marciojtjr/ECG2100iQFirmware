#ifndef SEG7_H
#define SEG7_H

#ifndef BOOTLOADER
#define S7_BATT_EMPTY	0x2a
#define S7_BATT_HALF	0x2b
#define S7_BATT_FULL	0x2c

#define S7_SEG_A	0x21
#define S7_SEG_B	0x22
#define S7_SEG_C	0x23
#define S7_SEG_D	0x24
#define S7_SEG_E	0x25
#define S7_SEG_F	0x26
#define S7_SEG_G	0x27
#endif

// individual segment controls
void Seg7A(bool on);
void Seg7B(bool on);
void Seg7C(bool on);
void Seg7D(bool on);
void Seg7E(bool on);
void Seg7F(bool on);
void Seg7G(bool on);
void Seg7DP(bool on);

#ifndef BOOTLOADER
void InitSeg7();	// blanks display (including DP)
void Seg7HP(unsigned char c);	// high priority write. Writing a ' ' lowers the priority
void Seg7LP(unsigned char c);	// low priority write, blocked if the LED contains a high priority character (other than ' ')
#endif

#endif
