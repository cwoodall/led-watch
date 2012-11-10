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

// Controls the RGB LED Minute output
// 00-09 : R (R)
// 10-19 : Y (RG)
// 20-29 : G (G)
// 30-39 : Cyan (GB)
// 40-49 : B (B)
// 50-59 : Magenta (BR)
inline void displayMinutes(uint8_t min) {
    PORTA &= 0x8F; // Clear Minute Bits
    if (min >= 50) {
        PORTA |= 0x40;
    } else if (min >= 40) {
        PORTA |= 0x40 | 0x10;
    } else if (min >= 30) {
        PORTA |= 0x10;
    } else if (min >= 20) {
        PORTA |= 0x20 | 0x10;
    } else if (min >= 10) {
        PORTA |= 0x20;
    } else if (min >= 0) {
        PORTA |= 0x40 | 0x20;
    }
}

inline void displayHours(uint8_t hours)
{
    // Clear hour bits
    PORTA &= 0xF0;

    // Load in hour
    PORTA |= (0x0F & hours);
}

void setupDisplayTimer()
{
    TCCR0A = 0x00;
    TCCR0A |= (0<<WGM01); // Turn on Clear on Compare

    TCCR0B = 0x00;
    TCCR0B |= (1 << CS00);

    TIMSK0 = 0x01;
    TIFR0 = 0x01;
}

void setupClockTimer()
{
    // No special outputs from the 16-bit timer
    TCCR1A = 0x00;

    TCCR1B = 0x00; 
    // TODO REPLACE 1MHz code with 32.768kHz code.
    TCCR1B |= (1<<WGM12) | // Setup Clear Timer on Compare OCR1A 
              (0 << CS12) | 
              (1 << CS11) | 
              (1 << CS10); // Use clk/64 prescaler for now
       
    TCCR1C = 0x00;

    // TODO CHANGE
    // Set Compare A for 1MHz/64
    // OCCRA1AH = 15625 = 0x3D09
    OCR1AH = 0x3D;
    OCR1AL = 0x09;

    TCNT1H = 0;
    TCNT1L = 0;
    
// Set-up Interrupts
    TIMSK1 = 0x00;
    TIMSK1 |= (1 << OCIE1A); // Only enable the Compare A interrupt
    
}

uint8_t seconds = 0;
uint8_t hours = 6;
uint8_t minutes = 11;
ISR(TIM1_COMPA_vect)
{
    seconds = (seconds + 1) % 60;

    if (seconds == 0)
    {
        minutes = (minutes + 1) % 60;
        if (minutes == 0) 
        {
            hours = (hours + 1) % 12;
        }
    }
}

ISR(TIM0_OVF_vect)
{
    static uint8_t display_ctr = 0;

    if (display_ctr < 4) { 
        PORTA &= 0xF0;
        PORTA |= (hours & (0x01 << display_ctr));
    } else {
        displayMinutes(minutes); 
    }
    
    if (display_ctr < 4) {
        display_ctr += 1;
    } else {
        display_ctr = 0;
    }
}

// Write an ISR which wakes up on PC interrupt on switch
// If switch pressed then turn on display and another counter
// and counter compare interrupt to control the display!

int main() 
{    
    cli();
    // Set HOUR[0-3] to outputs
    // Set MIN[0-3] to outputs
    // Set SW1 to input
    DDRA = 0x7F;
    PORTA = 0xF0;


    // HOUR LEDs are high active
    // MIN LEDs are low active
    // Set pullup resistor on SW1
    setupClockTimer();
    setupDisplayTimer();
    sei();

    while(1) {}
/*
    while(1)
    {
        if (!(PINA & 0x80))
        {

            minutes = (minutes + 10) % 60;
            hours = (hours + 1) % 12;

            for (uint8_t i = 0; i < 100; i++) 
            {
                displayMinutes(minutes);
                _delay_ms(10);
                PORTA = 0xF0;
                displayHours(hours);
                _delay_ms(10);
                PORTA = 0xF0;

            }
        } else {
            PORTA = 0xF0;
        }       
        
    }
*/
}

