#include "myUSART.h"

#include <avr/io.h> 
#include <stdio.h>

void USART_Init( unsigned int ubrr) {
	/*Set baud rate */
	UBRR0H = (unsigned char)(ubrr>>8);
	UBRR0L = (unsigned char)ubrr;

	/* Enable receiver and transmitter */
	UCSR0B = (1<<RXEN0)|(1<<TXEN0);
	/* Set frame format: 8data, 2stop bit */
	UCSR0C = (1<<USBS0)|(3<<UCSZ00);
}

void USART_transmit( unsigned char data ) { 
	/* Wait for empty transmit buffer */ 
	while ( !( UCSR0A & (1<<UDRE0)) ) ; 
	/* Put data into buffer, sends the data */ 
	UDR0 = data; 
}


void USART_putstring(char *StringPtr){
	while(*StringPtr != 0x00){
		USART_transmit(*StringPtr);
		StringPtr++;
	}
}

void USART_transmitFormatted(const char* format, ...) {
    char buffer[128];  // Adjust buffer size as needed
    va_list args;
    va_start(args, format);
    vsnprintf(buffer, sizeof(buffer), format, args);
    va_end(args);
    USART_putstring(buffer);
}