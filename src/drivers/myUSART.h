#ifndef myUSART_h
#define myUSART_h

void USART_Init( unsigned int ubrr);
void USART_transmit( unsigned char data );
void USART_putstring(char *StringPtr);
void USART_transmitFormatted(const char* format, ...);

#endif