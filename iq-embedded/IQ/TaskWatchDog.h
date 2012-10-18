#ifndef TaskWatchDog_h
#define TaskWatchDog_h

// Watchdog Timers
// The watchdog task watches various timers that are set within other tasks to allow detection of lock up conditions

#define FLASH_WATCH_TIME  90	// erase all can take up to 80 seconds, according to the M23P32 datasheet

extern unsigned char USBWatchTimer;
extern unsigned char FlashWatchTimer;
extern unsigned char AcqWatchTimer;


#endif
