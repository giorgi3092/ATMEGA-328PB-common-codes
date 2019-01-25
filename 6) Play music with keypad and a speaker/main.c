/*
 * This program needs a keypad and a speaker to work. See comments to check where they need to be connected. You can play music with this program
 * Created: 10/15/2018 8:51:21 PM 
 * Author : aptsi
 */ 
#define F_CPU 16000000l
#include <avr/io.h>
#include <util/delay.h>
#include <math.h>

/*Step 1: calculate baud rate*/

// keypad PINS initialization. D4-D7 as pull-up inputs. B0-B3 as outputs.
void keypad_Init(void)
{
	// output waveform on PB1
	DDRC |= (1 << DDRC1));
	
	// pull-up inputs
	DDRD &= ~(1 << DDRD4) & ~(1 << DDRD5) & ~(1 << DDRD6) & ~(1 << DDRD7);
	PORTD |= (1 << PORTD4) | (1 << PORTD5) | (1 << PORTD6) | (1 << PORTD7);
	
	// outputs
	DDRB |= (1 << DDRB0) | (1 << DDRB1) | (1 << DDRB2) | (1 << DDRB3);
	
	// set outputs to high
	PORTB |= (1 << PORTB0) | (1 << PORTB1) | (1 << PORTB2) | (1 << PORTB3);
}

double period_Gen(unsigned char key)
{
	double root = (pow(2, 1.0 / 12));
	switch(key)
	{
		case '1':
			return 1 / (440 * pow(root, 0));
		break;
		
		case '2':
			return	1 / (440 * pow(root, 1));
		break;
		
		case '3':
			return	1 / (440 * pow(root, 2));
		break;
		
		case 'A':
			return	1 / (440 * pow(root, 3));
		break;
		
		case '4':
			return	1 / (440 * pow(root, 4));
		break;
		
		case '5':
			return	1 / (440 * pow(root, 5));
		break;
		
		case '6':
			return	1 / (440 * pow(root, 6));
		break;
		
		case 'B':
			return	1 / (440 * pow(root, 7));
		break;
		
		case '7':
			return	1 / (440 * pow(root, 8));
		break;
		
		case '8':
			return	1 / (440 * pow(root, 9));
		break;
		
		case '9':
			return	1 / (440 * pow(root, 10));
		break;
		
		case 'C':
			return	1 / (440 * pow(root, 11));
		break;
		
		case '*':
			return	1 / (440 * pow(root, 12));
		break;
		
		case '0':
			return	1 / (440 * pow(root, 13));
		break;
		
		case '#':
			return	1 / (440 * pow(root, 14));
		break;
		
		case 'D':
			return	1 / (440 * pow(root, 15));
		break;
	}
}

void freq_Output(double period, unsigned char key_pressed)
{
	TCNT1 = 0x0000; // clear the timer from its present value to 0
	int OCR1A_Value = period*1000/(0.004);
	OCR1A = OCR1A_Value; //represents timer conforming to the identified period
	OCR1B = OCR1A_Value/2; //represents 50% duty cycle
	TCCR1B |= (1 << CS11) |  (1 << CS10) | (1 << WGM12); // set pre-scaler to 8 and start the timer
	
	//CREATE A LOGIC 1 SIGNAL (TURN ON LED)
	PORTC |= (1 << PORTC1);
	
	while ( (TIFR1 & (1 << OCF1B) ) == 0){} // wait for OCR1B overflow event
	TIFR1 |= (1 << OCF1B); // reset OCR1B overflow flag
	
	//CREATE A LOGIC 0 SIGNAL (TURN OFF LED)
	PORTC &= ~(1 << PORTC1);
	
	while ( (TIFR1 & (1 << OCF1A) ) == 0){} // wait for OCR1A overflow event
	TIFR1 |= (1 << OCF1A); // reset OCR0A overflow flag
}

// keypad scan. 
void keypad_Scan()
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
					freq_Output(period_Gen(keys[j][i]), j+4);
				}
			}
			PORTB |= (1 << i);
		}
	}
} 

int main(void)
{
    while (1) 
    {
		// keypad_Scan();
		keypad_Scan();
    }
}