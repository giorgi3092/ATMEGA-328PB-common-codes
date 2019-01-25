/*
 * USART.c
 *
 * Created: 10/5/2018 11:31:05 AM
 * Author : Giorgi Aptsiauri
 */ 

#include <avr/io.h>
#include <util/delay.h>


/*Step 1: calculate baud rate*/
#define FOSC 16000000LU // Clock Speed 
#define BAUD 9600 
#define MYUBRR FOSC/16/BAUD-1 

// function to send data
void uart_transmit(unsigned char data)
{
	while( !( UCSR0A & (1<<UDRE0) ) ); // stuck of not empty
	UDR0 = data;
}

unsigned char uart_receive (void)
{
	while(!((UCSR0A) & (1<<RXC0))); // 
	return UDR0;
}

void USART_Init( unsigned int ubrr) 
{
	/* Step 1 (cont.): Set baud rate */ 
	UBRR0H = (unsigned char)(ubrr>>8);
	UBRR0L = (unsigned char)ubrr;
	
	/* Step 2: enable receiver / transmitter */
	UCSR0B |= (1<<TXEN0)|(1<<RXEN0);
	
	/* Step 2: set data format (8-bit data) */
	UCSR0C |= (1<<UCSZ00)|(1<<UCSZ01);
	
}

void print_RED_ID(unsigned char * strPtr)
{
	char c;
	unsigned char * CPstrPtr = strPtr;
	while((c = *CPstrPtr++) != '\0')
		uart_transmit(c);
}

int main(void)
{
	unsigned char RED_ID[] = "GITHUB\n\r";
	
	// USART initialisation
	USART_Init(MYUBRR);
    
	/* Replace with your application code */
    while (1) 
    {
		print_RED_ID(&RED_ID[0]);
		_delay_ms(500);
    }
}