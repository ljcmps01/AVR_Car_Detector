#ifndef  AVR_W5100_H
#define  AVR_W5100_H



void spi_init();

unsigned char OpenSocket(unsigned char sock, unsigned char eth_protocol, unsigned int tcp_port);
void CloseSocket(unsigned char sock);
void DisconnectSocket(unsigned char sock);
unsigned char Listen(unsigned char sock);
unsigned char Send(unsigned char sock, const unsigned char *buf, unsigned int buflen);
unsigned int Receive(unsigned char sock, unsigned char *buf, unsigned int buflen);
unsigned int ReceivedSize(unsigned char sock);

void my_select(void);
void my_deselect(void);
unsigned char my_xchg(unsigned char val);
void my_reset(void);

void W5100_set_callbacks();

unsigned char W5100_init(\
    unsigned char* MAC,\
    unsigned char* IP,\
    unsigned char* SB,\
    unsigned char* GW\
);

#define MAX_BUF 512 /* largest buffer we can read from chip */

#define HTTP_PORT 80 /* TCP port for HTTP */

#endif /* W5100_H */