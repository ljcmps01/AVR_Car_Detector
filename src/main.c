#include "drivers/avr_w5100.h"
#include <avr/io.h>
#include <avr/pgmspace.h>
#include <stdio.h>
#include <string.h>
#include <util/delay.h>

#include "../external/w5100/w5100.h"

#define MAC                                                                    \
  { 0x00, 0x16, 0x36, 0xDE, 0x58, 0xF6 }
#define IP                                                                     \
  { 192, 168, 20, 233 }
#define SUBNET                                                                 \
  { 255, 255, 255, 0 }
#define GATEWAY                                                                \
  { 192, 168, 20, 1 }

unsigned char buf[MAX_BUF];

/*
 *  Define the SPI port, used to exchange data with a W5100 chip.
 */
#define SPI_PORT PORTB /* target-specific port containing the SPI lines */
#define SPI_DDR DDRB   /* target-specific DDR for the SPI port lines */

#define CS_DDR DDRB   /* target-specific DDR for chip-select */
#define CS_PORT PORTB /* target-specific port used as chip-select */
#define CS_BIT 2      /* target-specific port line used as chip-select */

#define RESET_DDR DDRC   /* target-specific DDR for reset */
#define RESET_PORT PORTC /* target-specific port used for reset */
#define RESET_BIT 6      /* target-specific port line used as reset */

int main(void) {
  uint8_t mac[] = MAC;
  uint8_t ip[] = IP;
  uint8_t subnet[] = SUBNET;
  uint8_t gateway[] = GATEWAY;

  unsigned int sockaddr;
  unsigned char mysocket;
  unsigned int rsize;

  mysocket = 0; // magic number! declare the socket number we will use (0-3)
  sockaddr = W5100_SKT_BASE(
      mysocket); // calc address of W5100 register set for this socket

  spi_init();

  /*
   *  Configure the W5100 device to handle PING requests.
   *  This requires configuring the chip, not a specific socket.
   */
  W5100_init(mac, ip, subnet,
             gateway); // config the W5100 (MAC, TCP address, subnet, etc

  /*
   *  The main loop.  Control stays in this loop forever, processing any
   * received packets and sending any requested data.
   */
  while (1) {
    switch (W51_read(sockaddr +
                     W5100_SR_OFFSET)) // based on current status of socket...
    {
    case W5100_SKT_SR_CLOSED: // if socket is closed...
      if (OpenSocket(mysocket, W5100_SKT_MR_TCP, HTTP_PORT) ==
          mysocket) // if successful opening a socket...
      {
        Listen(mysocket);
        _delay_ms(1);
      }
      break;

    case W5100_SKT_SR_ESTABLISHED:    // if socket connection is established...
      rsize = ReceivedSize(mysocket); // find out how many bytes
      if (rsize > 0) {
        if (Receive(mysocket, buf, rsize) != W5100_OK)
          break; // if we had problems, all done
                 /*
                  *  Add code here to process the payload from the packet.
                  *
                  *  For now, we just ignore the payload and send a canned HTML page so
                  * the client at least          knows we are alive.
                  */
        strcpy_P((char *)buf, PSTR("HTTP/1.0 200 OK\r\nContent-Type: "
                                   "text/html\r\nPragma: no-cache\r\n\r\n"));
        strcat_P((char *)buf, PSTR("<html>\r\n<body>\r\n"));
        strcat_P(
            (char *)buf,
            PSTR("<title>Karl's W5100 web server (ATmega644p)</title>\r\n"));
        strcat_P((char *)buf, PSTR("<h2>Karl's ATmega328p web server using "
                                   "Wiznet W5100 chip</h2>\r\n"));
        strcat_P((char *)buf, PSTR("<br /><hr>\r\n"));
        if (Send(mysocket, buf, strlen((char *)buf)) == W5100_FAIL)
          break; // just throw out the packet for now

        strcpy_P((char *)buf, PSTR("This is part 2 of the page."));
        strcat_P((char *)buf, PSTR("</body>\r\n</html>\r\n"));
        if (Send(mysocket, buf, strlen((char *)buf)) == W5100_FAIL)
          break; // just throw out the packet for now

        DisconnectSocket(mysocket);
      } else // no data yet...
      {
        _delay_us(10);
      }
      break;

    case W5100_SKT_SR_FIN_WAIT:
    case W5100_SKT_SR_CLOSING:
    case W5100_SKT_SR_TIME_WAIT:
    case W5100_SKT_SR_CLOSE_WAIT:
    case W5100_SKT_SR_LAST_ACK:
      CloseSocket(mysocket);
      break;
    }
  }

  return 0;
}
