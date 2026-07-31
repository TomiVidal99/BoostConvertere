/*
 * main.c
 * Author: Tomás Vidal
 * Last revision: 30/07/2026
 *
 * TLDR: Fast PWM generator at ~120kHz with button polling on PD2
 * Output: Pin 9 (PB1 / OC1A)
 * Button: Pin 2 (PD2) to GND
 */

#ifndef F_CPU
#define F_CPU 16000000UL
#endif

#include <avr/io.h>
#include <util/delay.h>

#include "definitions.h"

// Array of duty cycle percentages to cycle through
const uint8_t DUTY_STEPS[] = {10, 25, 50, 75, 90};
#define NUM_DUTY_STEPS (sizeof(DUTY_STEPS) / sizeof(DUTY_STEPS[0]))

int main(void)
{
    // Configure PB1 (Pin 9) as output
    DDRB |= (1 << PWM_PIN);

    // Initialize Button & Timer1 PWM
    button_init();
    timer1_pwm_init();

    uint8_t current_step = 2; // Start at index 2 (50%)
    pwm_set_duty_percent(DUTY_STEPS[current_step]);

    uint8_t button_last_state = 1; // High due to internal pull-up

    while (1)
    {
        // 100ms polling rate
        _delay_ms(100);

        // Read current button pin state (0 = Pressed, 1 = Released)
        uint8_t button_current_state = (PIND & (1 << BUTTON_PIN)) ? 1 : 0;

        // Detect falling edge (transition from Released -> Pressed)
        if (button_last_state == 1 && button_current_state == 0)
        {
            // Cycle to the next step
            current_step = (current_step + 1) % NUM_DUTY_STEPS;
            pwm_set_duty_percent(DUTY_STEPS[current_step]);
        }

        // Save state for edge detection
        button_last_state = button_current_state;
    }
}

void button_init(void) {
    // Set PD2 as input
    DDRD &= ~(1 << BUTTON_PIN);

    // Enable internal pull-up resistor on PD2
    PORTD |= (1 << BUTTON_PIN);
}

void timer1_pwm_init(void) {
    // Reset Timer1 registers
    TCCR1A = 0;
    TCCR1B = 0;
    TCNT1  = 0;

    ICR1 = 96;

    // Set initial pulse width (0% duty cycle)
    OCR1A = 0;

    // Configure Timer1:
    // 1. Mode 14: Fast PWM with TOP = ICR1 (WGM13 = 1, WGM12 = 1, WGM11 = 1)
    // 2. Non-inverting PWM on OC1A/PB1 (COM1A1 = 1)
    // 3. Prescaler = 1 (CS10 = 1)
    TCCR1A |= (1 << COM1A1) | (1 << WGM11);
    TCCR1B |= (1 << WGM13) | (1 << WGM12) | (1 << CS10);
}

void pwm_set_duty_percent(uint8_t duty_percent) {
    // Clamp percentage to 100 max
    if (duty_percent > 100) {
        duty_percent = 100;
    }

    // Integer math: Multiply first, then divide to retain precision
    OCR1A = ((uint32_t)duty_percent * ICR1) / 100;
}

void pwm_set_duty_permille(uint16_t duty_permille) {
    // Allows 0.1% resolution step adjustments (0 = 0.0%, 1000 = 100.0%)
    if (duty_permille > 1000) {
        duty_permille = 1000;
    }

    // OCR1A = (duty * ICR1) / 1000
    OCR1A = ((uint32_t)duty_permille * ICR1) / 1000;
}