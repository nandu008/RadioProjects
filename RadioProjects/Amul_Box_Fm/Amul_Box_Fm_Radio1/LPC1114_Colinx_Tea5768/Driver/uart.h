#ifndef __UART_H 
#define __UART_H


#define RS485_ENABLED      0
#define TX_INTERRUPT      0      /* 0 if TX uses polling, 1 interrupt driven. */
#define MODEM_TEST         0

#define IER_RBR      0x01
#define IER_THRE   0x02
#define IER_RLS      0x04

#define IIR_PEND   0x01
#define IIR_RLS      0x03
#define IIR_RDA      0x02
#define IIR_CTI      0x06
#define IIR_THRE   0x01

#define LSR_RDR      0x01
#define LSR_OE      0x02
#define LSR_PE      0x04
#define LSR_FE      0x08
#define LSR_BI      0x10
#define LSR_THRE   0x20
#define LSR_TEMT   0x40
#define LSR_RXFE   0x80

#define UART_BUFSIZE      3u

extern volatile uint32_t UARTStatus;
extern volatile uint8_t  UARTTxEmpty ;
extern volatile uint8_t  UARTBuffer[UART_BUFSIZE];
extern volatile uint32_t UARTCount;

void ModemInit( void );
void UARTInit(uint32_t Baudrate);
void UART_IRQHandler(void);
void UARTSend(uint8_t *BufferPtr, uint32_t Length);

#endif /* end __UART_H */
/*****************************************************************************
**                            End Of File
******************************************************************************/
