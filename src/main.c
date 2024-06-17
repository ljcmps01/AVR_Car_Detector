// MCU Config Setup
#define F_CPU 16000000UL
#define BAUD 9600
#define MYUBRR F_CPU / 16 / BAUD - 1

#include <avr/interrupt.h>
#include <avr/io.h>
#include <util/delay.h>

#include "drivers/myUSART.h"

uint8_t flag_pc = 0;

int main(void) {
  PORTB |= (1 << PINB5);
  DDRB |= (1 << 5);

  DDRC &= ~0xf;
  PORTC |= 0xff;

  // Deshabilito interrupciones global
  cli();

  // Habilito PCINT1 (8-14)
  PCICR |= (1 << PCIE1);
  PCMSK1 |= (1 << PCINT8) | (1 << PCINT9) | (1 << PCINT10) | (1 << PCINT11);

  // Habilito interrupciones globalmente
  sei();

  USART_Init(MYUBRR);
  USART_transmitFormatted("Inicio de programa\n");
  USART_transmitFormatted("PORTC status: %d\n", PINC);

  uint8_t irStatus = 0;
  uint8_t step = 0;
  uint8_t contadorAutos = 0;

  while (1) {

    // Si hay un cambio en el pin de lectura
    if (flag_pc) {
      irStatus = ~PINC & 0xf;

      switch (step) {
      case 0:
        USART_transmitFormatted("Puerto C: %d\tPaso: %d\n", irStatus, step);

        if (irStatus == 1) {
          USART_transmitFormatted("Sensor A activado\n");
          step = 1;
        } else
          step = 0;

        break;

      case 1:

        USART_transmitFormatted("Puerto C: %d\tPaso: %d\n", irStatus, step);

        if (irStatus == 3) {
          USART_transmitFormatted("Sensor A y B activados\n");
          step = 2;
        } else
          step = 0;
        break;

      case 2:
        USART_transmitFormatted("Puerto C: %d\tPaso: %d\n", irStatus, step);

        if (irStatus == 2) {
          USART_transmitFormatted("Sensor B activado\n");
          step = 3;
        }

        else
          step = 0;

        break;

      case 3:
        USART_transmitFormatted("Puerto C: %d\tPaso: %d\n", irStatus, step);

        if (irStatus == 0) {
          USART_transmitFormatted("Entro un auto\nHay %d autos\n",
                                  ++contadorAutos);
        } else
          step = 0;

        break;

      default:
        USART_transmitFormatted("Caso prohibido!\n\
					Puerto C: %d\tPaso: %d\n",
                                irStatus, step);
        step = 0;
        break;
      }

      USART_transmitFormatted("Interrupcion detectada!\nPuerto C: %d\n",
                              irStatus);
      flag_pc = 0;
    }

    // Toggleo el led
    //  PINB |= (1<<PINB5);
    _delay_ms(.1);
  }
}

ISR(PCINT1_vect) { flag_pc = 1; }