/*
 * Created: 10/15/2018 8:51:21 PM 
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

// function to receive data
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

// keypad PINS initialization. D4-D7 as pull-up inputs. B0-B3 as outputs.
void keypad_Init(void)
{
	// pull-up inputs
	DDRD &= ~(1 << DDRD4) & ~(1 << DDRD5) & ~(1 << DDRD6) & ~(1 << DDRD7);
	PORTD |= (1 << PORTD4) | (1 << PORTD5) | (1 << PORTD6) | (1 << PORTD7);
	
	// outputs
	DDRB |= (1 << DDRB0) | (1 << DDRB1) | (1 << DDRB2) | (1 << DDRB3);
	
	// set outputs to high
	PORTB |= (1 << PORTB0) | (1 << PORTB1) | (1 << PORTB2) | (1 << PORTB3);
}

// keypad scan. 
void keypad_Scan(void)
{
	// setup GPIO pins
	keypad_Init();
	
	// setup keypad matrix
	unsigned char keys[4][4] = { {'1', '2', '3', 'A'},
								 {'4', '5', '6', 'B'},
							     {'7', '8', '9', 'C'},
								 {'*', '0', '#', 'D'}};
	while(1)
	{
		for(unsigned char i = 0; i <= 3; ++i)
		{
			PORTB &= ~(1 << i);
			for(unsigned char j = 0; j <= 3; ++j)
			{
				if(!(PIND & (1 << (j+4))))
				{
					uart_transmit(keys[j][i]);
					while(!(PIND & (1 << (j+4))));
				}
					
			}
			PORTB |= (1 << i);
		}
	}
} 
 
int main(void)
{
    // USART initialization
	USART_Init(MYUBRR);
    while (1) 
    {
		// keypad_Scan();
		keypad_Scan();
    }
}

