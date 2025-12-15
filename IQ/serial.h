#ifdef BOOTLOADER
void initbootuart( unsigned char value, unsigned char scaleFactor );
void sendchar( unsigned char );
unsigned char recchar( void );
void send_xon( void );
void send_xoff( void );
void disconnect_uart( void );
void flush_uart( void );
void sendStr(const char *s);
#endif