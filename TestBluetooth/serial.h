#ifdef BOOTLOADER
void initbootuart( unsigned char value, unsigned char scaleFactor );
void sendchar( unsigned char );
unsigned char recchar( void );
#endif