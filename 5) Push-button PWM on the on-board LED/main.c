/* This program lights up or dims the on-board LED with a pushbutton on the AVR device
 * Created: 10/26/2018 1:20:27 AM
 * Author : Giorgi Aptsiauri
 */ 

#include <avr/io.h>

int main(void)
{
    DDRB |= (1 << DDRB5);
	int i = 0x1;
	OCR1B = 0x1;
    while (1) 
    {
		if( PINB & (1 << PINB7))
		{
			if(i > 0x1)
			{
				i-=6;
				OCR1B = i;
			}
			
			while(i == 0x1 && (PINB & (1 << PINB7))) {}; //freeze execution. 
			
			TCNT1 = 0x0000; // clear the timer from its present value to 0
			
			OCR1A = 0x270; //represents 10ms timer
			TCCR1B |= (1 << CS12) | (1 << WGM12); // set pre-scaler to 256 and start the timer
			
			//CREATE A LOGIC 1 SIGNAL (TURN ON LED)
			PORTB |= (1 << PORTB5);
			
			while ( (TIFR1 & (1 << OCF1B) ) == 0){} // wait for OCR1B overflow event
			TIFR1 |= (1 << OCF1B); // reset OCR1B overflow flag
			
			//CREATE A LOGIC 0 SIGNAL (TURN OFF LED)
			PORTB &= ~(1 << PORTB5);
			
			while ( (TIFR1 & (1 << OCF1A) ) == 0){} // wait for OCR1A overflow event
			TIFR1 |= (1 << OCF1A); // reset OCR0A overflow flag
		} else
		{
			if(i < 0x268)
			{
				i+=6;
				OCR1B = i;
			}
			TCNT1 = 0x0000; // clear the timer from its present value to 0
			
			OCR1A = 0x270; //represents 10ms timer
			TCCR1B |= (1 << CS12) | (1 << WGM12); // set pre-scaler to 256 and start the timer
			
			//CREATE A LOGIC 1 SIGNAL (TURN ON LED)
			PORTB |= (1 << PORTB5);
			
			while ( (TIFR1 & (1 << OCF1B) ) == 0){} // wait for OCR1B overflow event
			TIFR1 |= (1 << OCF1B); // reset OCR1B overflow flag
			
			//CREATE A LOGIC 0 SIGNAL (TURN OFF LED)
			PORTB &= ~(1 << PORTB5);
			
			while ( (TIFR1 & (1 << OCF1A) ) == 0){} // wait for OCR1A overflow event
			TIFR1 |= (1 << OCF1A); // reset OCR0A overflow flag
		}
		
    }
}