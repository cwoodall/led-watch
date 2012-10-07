#define F_CPU 1000000
#include <avr/io.h>
#include <util/delay.h>

#define HOUR0 PA0
#define HOUR1 PA1
#define HOUR2 PA2
#define HOUR3 PA3
#define MIN0  PA4
#define MIN1  PA5
#define MIN2  PA6
#define SW1   PA7
#define SW0   PB2
int main() 
{    
    // Set HOUR[0-3] to outputs
    // Set MIN[0-3] to outputs
    // Set SW1 to input
    DDRA = 0x80;
    // HOUR LEDs are high active
    // MIN LEDs are low active
    // Set pullup resistor on SW1
    PORTA = 0xF0;

    while (1) 
    {
	for (int i = 0; i < 4; i++)
	{
	    PORTA &= (0xF0 | (0x1 << i));
	    _delay_us(10000);
	}
	for (int i = 4; i < 7; i++)
	{
	    PORTA &= ~(0x01<<i);
	    _delay_us(10000);
	}
    }
}
