#include <avr/io.h>
#include <avr/interrupt.h>
#include <stdint.h>
#include <stdio.h>


#define D4 eS_PORTD4
#define D5 eS_PORTD5
#define D6 eS_PORTD6
#define D7 eS_PORTD7
#define RS eS_PORTC6
#define EN eS_PORTC7
#define F_CPU 16000000UL // 10 MHz clock speed
#include <util/delay.h>

#include "lcd.h"

//volatile uint8_t start_send = 0;
//
void LCD_init(void);
void LCD_command(unsigned char cmd);
void LCD_data(unsigned char data);
void LCD_string(const char *str);

void FloatToStr(float num, char* str, int precision) {
	sprintf(str, "%.*f", precision, num);
}


int main(void)
{
	// Configure Port A and Port B as inputs
	DDRA = 0x00;
	DDRB = 0x00;
	
	// Configure Port C and Port D as outputs
	DDRC=0xFF;
	DDRD=0xFF;
	
	// Enable pull-up resistors on Port A and Port B
	PORTA = 0xFF;
	PORTB = 0xFF;

	uint32_t scounter;
	uint32_t ccounter;
	uint8_t portAValue;
	uint8_t portBValue ;
	char str[18];
	double freq;
	
	Lcd4_Init();     // Initialize the LCD


	//making output disable on counters
	PORTC |= (1 << PORTC2);
	PORTC |= (1 << PORTC1);
	PORTC |= (1 << PORTC0);
	PORTC &= ~(1 << PORTC3); // reset counters


	while (1)
	{
		scounter=0;
		ccounter=0;
		PORTC |= (1 << PORTC3); // un reset counters
		_delay_ms(1);
		PORTC |= (1 << PORTC4);		//make set 1
		PORTC &= ~(1 << PORTC5);	//make reset 0
		
		_delay_ms(100);

		// toggle reset pin
		PORTC |= (1 << PORTC5);		//make reset 1
		PORTC &= ~(1 << PORTC4);	//make set 0
		
		 PORTC &= ~(1 << PORTC2);	//output enable
		
		// Read the values of Port A and Port B
		 portAValue = PINA;
		 portBValue = PINB;
		
		 scounter=(scounter<<8)+portAValue;
		 ccounter=(ccounter<<8)+portBValue;
		
		 PORTC |= (1 << PORTC2);
		_delay_ms(1);
		 PORTC &= ~(1 << PORTC1);
		
		// Read the values of Port A and Port B
		 portAValue = PINA;
		 portBValue = PINB;
		
		 scounter=(scounter<<8)+portAValue;
		 ccounter=(ccounter<<8)+portBValue;
		
		 PORTC |= (1 << PORTC1);
		_delay_ms(1);
		 PORTC &= ~(1 << PORTC0);
		
		// Read the values of Port A and Port B
		 portAValue = PINA;
		 portBValue = PINB;
		
		 scounter=(scounter<<8)+portAValue;
		 ccounter=(ccounter<<8)+portBValue;
		
		 PORTC |= (1 << PORTC0);
		
		
		_delay_ms(1);
		PORTC &= ~(1 << PORTC0);

		// Read the values of Port A and Port B
		portAValue = PINA;
		portBValue = PINB;
		
		scounter=(scounter<<8)+portAValue;
		ccounter=(ccounter<<8)+portBValue;

		PORTC |= (1 << PORTC0);

		PORTC &= ~(1 << PORTC3);	//reseting counters
		
		freq=(double)scounter*500/ccounter;
		//sprintf(str, "%.5f", freq);
		sprintf(str, "%d", (int)freq);

		
		Lcd4_Set_Cursor(1,1);
		Lcd4_Write_String(str);
	}
	

}

