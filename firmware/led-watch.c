/**
 * TODO
 * - Make firmware thats not so shitty...
 * - Get datasheet and write interrupt driven code
 * - Use 1MHz clock first as experiment
 * - Move to clock oscillator ASAP
 * - Low power consumption ASAP so I don't go through all of my batteries
 */
#define F_CPU 1000000
#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>
#define HOUR0 PA0
#define HOUR1 PA1
#define HOUR2 PA2
#define HOUR3 PA3
#define MIN0  PA4
#define MIN1  PA5
#define MIN2  PA6
#define SW1   PA7
#define SW0   PB2

int seconds = 0;
int minutes = 50;
int hours = 12;

int main() 
{    
    // Set HOUR[0-3] to outputs
    // Set MIN[0-3] to outputs
    // Set SW1 to input
    DDRA = 0xFF;
    // HOUR LEDs are high active
    // MIN LEDs are low active
    // Set pullup resistor on SW1
     
    PORTA = 0xF0;
    _delay_ms(500);
    PORTA = ~0x1F;
    _delay_ms(500);
    PORTA = ~0x2F;
    _delay_ms(500);
    PORTA = ~0x4F;
    _delay_ms(500);
    PORTA = 0xF0;
    while(1)
    {
        PORTA += 0x01;
        PORTA |= 0xF0;
        _delay_ms(500);
    }
}

