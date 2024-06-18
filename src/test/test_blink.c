#define F_CPU 16000000UL
#include <avr/io.h>
#include <util/delay.h>

int main(void) {
  DDRB |= (1 << 5);

  while (1) {
    PINB |= (1 << PINB5);
    _delay_ms(200);
  }
}
