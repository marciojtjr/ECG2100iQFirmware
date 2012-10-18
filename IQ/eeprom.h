#ifndef EEPROM_RDWR
#define EEPROM_RDWR

#include "eeprom_driver.h"

unsigned long EE_Rd(unsigned int offset);
void EE_Wr(unsigned int offset, unsigned long data);

// offsets to things in EEPROM
enum {
	EE_SERIAL_NUM,							// Used to uniquely identify the peanut
	EE_VOICE_RECORD_LENGTH,			// 0 = nothing has been recorded, otherwise indicates how many milliseconds of voice have been recorded
	EE_ECG_RECORD_LENGTH,				// 0 = nothing has been recorded, otherwise indicates how many milliseconds of ECG have been recorded
	EE_FORMAT,									// holds the desired output format
	EE_BAD_ELAPSED_TIME,				// 1 = we powered off or were reset while tracking elapsed time. So, it can't be trusted
	EE_DELAY,										// used to throttle data uploads
	EE_FLOW_CONTROL,						// 1 = enable RTS flow control, 0 = full speed ahead
	EE_DEBUG,
	EE_VERBOSE,
	EE_ABANDON_TIME,						// pre-acquisition abandonment timer, in seconds
	EE_REZERO_TIME,							// milliseconds of time that must elapse without a rezero before QC indicator unclamps from one bar
	EE_R2,											// baseline roll limit for two bars
	EE_R3,											// baseline roll limit for three bars
	EE_B1,											// threshold to jam QC indicator to one bar (caused by no LF feedback due to saturated lead)
	EE_B2,											// threshold for LFBias to indicate two bars
	EE_B3,											// threshold for LFBias to indicate three bars
	EE_LFBIAS_ENABLE,						// 1 = lf bias enable, 0 = LF bias disable
	EE_LB,											// low battery threshold, in units of mV
	EE_BOOT_VERSION,						// written by the bootstrap loader to indicate the boot version (0xff means version 1)
	EE_APP_CRC,									// CRC of the application section of Flash, compared at startup by the bootstrap loaded to determine app integrity
};

#endif
