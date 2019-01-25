/*
 * This program demonstrates how to use the EEPROM of ATMEGA 328PB. Speficically, LED brightness increases
 * by 10% every time you power up the device. When it reaches 100%, brightness comes back to 0%.
 * Created: 11/30/2018 12:24:13 AM
 * Author : Giorgi Aptsiauri
 */ 
#define F_CPU 16000000LU // Clock Speed
#include <avr/io.h>
#include <util/delay.h>
#include <avr/eeprom.h>
#include <avr/interrupt.h>
#include <stdbool.h>

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
	DDRB |= (1<<DDRB5);
	
	//	write initial value 0 in EEPROM location 500
	if((eeprom_read_dword((uint32_t*)10000) == 0xFFFFFFFF)) // this code is entered only once across multiple executions
	{
		eeprom_write_dword((uint32_t*)960, 0); // initialize the dword to 0
		OCR1B = 0; // write that value into the timer OCR1B register
	} else if (eeprom_read_dword((uint32_t*)960) == 9990)
	{
		eeprom_write_dword((uint32_t*)960, 0); // restore to 0% DC
		OCR1B = 0; // write the updated value into the timer OCR1B register 
	} else
	{
		eeprom_write_dword((uint32_t*)960, eeprom_read_dword((uint32_t*)960) + 999); // 0 + 10*999 = 9990 which is just 9 bits less than OCR1A, so almost 100% DC at best
		OCR1B = eeprom_read_dword((uint32_t*)960); // write the updated value into the timer OCR1B register 
	}


	OCR1A = 9999; //represents 5ms timer
	TCCR1B |= (1 << CS11) | (1 << WGM12); // CTC mode and pre-scale 8
	TIMSK1 |= (1 << OCIE1A) | (1 << OCIE1B);
	//	A & B Interrupts are called when OCR1A and OCR1B overflow
	
	sei();
	
	while (1)
	{
		
	}
}