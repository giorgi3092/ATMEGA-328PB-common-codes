/*
 * LAB 7 - Interrupts.c
 *
 * Created: 11/7/2018 5:50:25 PM
 * Author : aptsi
 */ 

#include <avr/io.h>
#include <avr/interrupt.h>

unsigned short key_value = 7;

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


// keypad scan when interrupt happens
ISR (TIMER0_COMPA_vect)  // timer0 Compare register A interrupt
{
	// GPIO pins are already set up
	// setup keypad matrix. all keys except numeric ones, generate 0% DC PWM
	unsigned char keys[4][4] = { {1, 2, 3, 0},
	{4, 5, 6, 0},
	{7, 8, 9, 0},
	{0, 0, 0, 0}};

	for(unsigned char i = 0; i <= 3; ++i)
	{
		PORTB &= ~(1 << i);
		for(unsigned char j = 0; j <= 3; ++j)
		{
			if(!(PIND & (1 << (j+4))))
			{
				key_value = (short) keys[j][i];	// set the duty cycle
				OCR1B = 1999*key_value;
			}
		}
		PORTB |= (1 << i);
	}
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

int main(void)
{	
	cli();
    // set the on-board LED as output
    DDRB |= (1<<DDRB5);
	
	//	keypad init
	keypad_Init();
	
	
	// timer to call keypad scan every ~4ms
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