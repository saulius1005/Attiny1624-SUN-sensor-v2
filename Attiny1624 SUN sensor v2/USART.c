/*
 * USART.c
 *
 * Created: 2024-09-09 14:58:39
 *  Author: Saulius
 */ 

  #include "Settings.h"

  static FILE USART_stream = FDEV_SETUP_STREAM(USART0_printChar, NULL, _FDEV_SETUP_WRITE);

  void USART0_init(){
	USART0.CTRLA = USART_RS485_bm; //turn on RS485 driver controll (ST485EBDR)
	USART0.BAUD = (uint16_t)USART0_BAUD_RATE(115200);//baud rate 115200bps
	USART0.CTRLB = USART_RXEN_bm | USART_TXEN_bm | USART_RXMODE_CLK2X_gc; //receiver, transmitter enabled double speed
	USART0.CTRLC = USART_CMODE_ASYNCHRONOUS_gc | USART_CHSIZE_8BIT_gc | USART_PMODE_DISABLED_gc | USART_SBMODE_1BIT_gc; //asinch, 8b character, no parity, stop 1 b
	stdout = &USART_stream;
  }

  void USART0_sendChar(char c){
	  while (!(USART0.STATUS & USART_DREIF_bm));
	  USART0.TXDATAL = c;
  }

  int USART0_printChar(char c, FILE *stream){
	  USART0_sendChar(c);
	  return 0;
  }

  void USART0_sendString(char *str){
	  for(size_t i = 0; i < strlen(str); i++){
		  USART0_sendChar(str[i]);
	  }
  }

  char USART0_readChar(){
	  USART0.STATUS = USART_RXCIF_bm;// clear buffer before reading
	  while (!(USART0.STATUS & USART_RXCIF_bm));
	  return USART0.RXDATAL;
  }