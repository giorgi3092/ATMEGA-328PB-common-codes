/*
 * Created: 9/28/2018 12:13:02 PM
 * Author : Giorgi Aptsiauri
 */
#define F_CPU 16000000UL
#include <avr/io.h>
#include <util/delay.h>
#include <ctype.h>

#define TIME_UNIT 200 // defined in ms
#define DOT 1   // 1 corresponds to a dot. Dot is one unit long
#define DASH 0  // 0 corresponds to a dash. Dash is three units long

const unsigned char LETTERS[36][6] = {
	{'1', '0','\0'},				// A
	{'0','1','1','1','\0'},		// B
	{'0','1','0','1','\0'},		// C
	{'0','1','1','\0'},			// D
	{'1','\0'},					// E
	{'1','1','0','1','\0'},		// F		
	{'0','0','1','\0'},			// G
	{'1','1','1','1','\0'},		// H
	{'1','1','\0'},				// I
	{'1','0','0','0','\0'},		// J
	{'0','1','0','\0'},			// K
	{'1','0','1','1','\0'},		// L
	{'0','0','\0'},				// M
	{'0','1','\0'},				// N
	{'0','0','0','\0'},			// O	
	{'1','0','0','1','\0'},		// P	
	{'0','0','1','0','\0'},		// Q	
	{'1','0','1','\0'},			// R	
	{'1','1','1','\0'},			// S
	{'0','\0'},					// T	
	{'1','1','0','\0'},			// U	
	{'1','1','1','0','\0'},		// V	
	{'1','0','0','\0'},			// W	
	{'0','1','1','0','\0'},		// X	
	{'0','1','0','0','\0'},		// Y
	{'0','0','1','1','\0'},		// Z	
	{'0','0','0','0','0','\0'},	// 0	
	{'1','0','0','0','0','\0'},	// 1	
	{'1','1','0','0','0','\0'},	// 2	
	{'1','1','1','0','0','\0'},	// 3	
	{'1','1','1','0','\0'},		// 4	
	{'1','1','1','1','1','\0'},	// 5
	{'0','1','1','1','1','\0'},	// 6
	{'0','0','1','1','1','\0'},	// 7
	{'0','0','0','1','1','\0'},	// 8
	{'0','0','0','0','1','\0'},	// 9	
};


void dot(void)
{
	PORTB |= (1 << PORTB5);
	_delay_ms(1*TIME_UNIT);
}

void dash(void)
{
	PORTB |= (1 << PORTB5);
	_delay_ms(3*TIME_UNIT);
}

void space_between_parts_of_the_same_letters(void)
{
	PORTB &= ~(1 << PORTB5);
	_delay_ms(1*TIME_UNIT);
}

void space_between_letters(void)
{
	PORTB &= ~(1 << PORTB5);
	_delay_ms(3*TIME_UNIT);
}

void space_between_words(void)
{
	PORTB &= ~(1 << PORTB5);
	_delay_ms(7*TIME_UNIT);
}


/* handles calling appropriate functions to either output DOT, DASH or SPACE for a single character
* in the input string */
void MorseLEDLight(unsigned char c)
{
	unsigned char i = 0;	// while iterator
	LOOP: while(1)
	{
		if(LETTERS[c][i] == '1')
		{
			dot();
		} else if(LETTERS[c][i] == '0')
		{
			dash();
		} else if (LETTERS[c][i] == '\0')
		{
			goto BREAK_IF_INVALID_CHAR;    // break the loop if neither '1' or '0'
		}
		i++;
		space_between_parts_of_the_same_letters();
		goto LOOP;
		BREAK_IF_INVALID_CHAR: break;
	}
}

// parse the input string to map each character in it to an actual "Morse Character"
void MorseLED(unsigned char* strPtr)
{
	unsigned char c;		// temp character storage
	unsigned char* COPYstrPtr = strPtr;
	LOOP: while( (c = *(COPYstrPtr++)) != '\n')
	{
		c = toupper(c);
		if(c >= 'A' && c <= 'Z')			// for alphabetic characters
		{
			c -= 65;
			MorseLEDLight(c);
		} else if(c >= '0' && c <= '9')		// for numbers
		{
			c -= 22;
			MorseLEDLight(c);
		} else if(c == ' ')					// for space characters
		{
			space_between_words();
		} else
		{
			goto BREAK_IF_INVALID_CHAR;		// break the loop if invalid characters are found in the input string
		}
		if(c != ' ' && *(COPYstrPtr) != '\n')
			space_between_letters();
		goto LOOP;
		BREAK_IF_INVALID_CHAR: break;
	}
}

int main(void)
{
	unsigned char INITIALS[] = "GITHUB\n";
	DDRB |= (1 << DDRB5);	// LED pin set to OUTPUT
	
	while(1)
	{
		MorseLED(&INITIALS[0]);
		space_between_words();   // space between words.
								 // This is for infinite loop though.
	}
}