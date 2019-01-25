/*
 * With this program, you can control the brightness of the on-board LED with a potentiometer
 * Author: Giorgi Aptsiauri
 */
#define F_CPU 16000000LU
#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>

volatile double fraction;

// ADC => when interrupt happens
ISR (TIMER0_COMPA_vect)  // timer0 Compare register A interrupt
{
	ADCSRA |= (1<<ADSC);  // start conversion
	while(ADCSRA & (1<<ADSC));
	fraction = (ADC)/(1024.0);
	OCR1B = fraction*OCR1A;
}

// compare match A Interrupt
ISR (TIMER1_COMPA_vect)
{
	PORTB |= (1 << PORTB5);
}

// compare match B Interrupt
ISR (TIMER1_COMPB_vect)
{
	PORTB &= ~(1 << PORTB5);
}

int main(void)
{	
	cli();
	
	// set the on-board LED as output
	DDRB |= (1<<DDRB5);		// AREF = AVcc
	ADMUX = (1<<REFS0);		// ADC Enable and pre-scaler of 128
	// 16000000/128 = 125000
	//104 us per conversion
	ADCSRA = (1<<ADEN)|(1<<ADPS2)|(1<<ADPS1)|(1<<ADPS0);
	
	//	timer to ADC every ~4ms
	// this timer is set up to fire an interrupt every 4 ms	*/
	TCCR0A |= (1 << WGM01); // Set the Timer Mode to CTC
	OCR0A = 0xF9; //represents ~4ms timer
	TIMSK0 |= (1 << OCIE0A); //Set the ISR COMPA vect
	TCCR0B |= (1 << CS02);	// set the prescaler to 256 and start the timer
	
	OCR1A = 0x4E1F; //represents 1.25ms timer
	TCCR1B |= (1 << CS10) | (1 << WGM12); // CTC mode and pre-scale 1
	TIMSK1 |= (1 << OCIE1A) | (1 << OCIE1B);
	//	A & B Interrupts are called when OCR1A and OCR1B overflow
	
	sei();
	
	while (1)
	{
		
	}
}