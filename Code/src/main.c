/*
 * main.c
 * Author: Tomás Vidal
 * Last revision: 30/07/2026
 *
 * TLDR: ATM just a square wave generator
 * through the pin 9 (PB9)
 */

#ifndef F_CPU
#define F_CPU 16000000UL
#endif

#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>

#include "definitions.h"

#define TOGGLING_PIN (PB2)

/**
 * @brief Initializes the TIM1 (16 bits)
 * So it triggers an interruption with a 
 * frequency of 120KHz
 */
void timer1_init(void);

int main(void)
{

  // timer1_init();

  // Sets the PB9 (pin 9) as output
  DDRD |= (1<<TOGGLING_PIN);

  while (1)
  {
    PORTB ^= (1<<TOGGLING_PIN);
    _delay_us(8);
  }
}

// Toggles the Pin 9 (PB1)
ISR(TIMER1_COMPA_vect) {
  PORTB ^= (1 << TOGGLING_PIN);
}

void timer1_init(void) {
    // Disable global interrupts during setup
    cli();

    // Reset Timer1 Control Registers
    TCCR1A = 0;
    TCCR1B = 0;
    TCNT1  = 0;

    // Set Compare Match value for ~120 kHz (16MHz / (1 * 133) = 120.3kHz)
    // OCR1A = 132;
    OCR1A = 100;

    // Configure Timer1:
    // 1. CTC mode (WGM12 = 1)
    // 2. Prescaler = 1 (CS10 = 1)
    TCCR1B |= (1 << WGM12) | (1 << CS10);

    // Enable Timer1 Output Compare A Match Interrupt
    TIMSK1 |= (1 << OCIE1A);

    // Enable global interrupts
    sei();
}