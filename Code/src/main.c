/*
 * main.c
 * Author: Tomás Vidal
 * Last revision: 30/07/2026
 *
 * TLDR: Fast PWM generator at ~120kHz with integer duty cycle
 * Output: Pin 9 (PB1 / OC1A)
 */

#ifndef F_CPU
#define F_CPU 16000000UL
#endif

#include <avr/io.h>
#include <util/delay.h>

#include "definitions.h"

int main(void)
{
    // Configure PB1 (Pin 9) as output
    DDRB |= (1 << PWM_PIN);

    // Initialize Timer1 in Fast PWM Mode
    timer1_pwm_init();

    // Set initial Duty Cycle to 50%
    pwm_set_duty_percent(75);

    while (1)
    {
        // // Example: Sweeping duty cycle from 10% to 90% in 5% steps
        // for (uint8_t d = 10; d <= 90; d += 5)
        // {
        //     pwm_set_duty_percent(d);
        //     _delay_ms(500);
        // }
    }
}

void timer1_pwm_init(void) {
    // Reset Timer1 registers
    TCCR1A = 0;
    TCCR1B = 0;
    TCNT1  = 0;

    ICR1 = 93; // for 120KHz
    // ICR1 = 200;

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
    // OCR1A = (duty * 132) / 100
    OCR1A = ((uint32_t)duty_percent * ICR1) / 100;
}

void pwm_set_duty_permille(uint16_t duty_permille) {
    // Allows 0.1% resolution step adjustments (0 = 0.0%, 1000 = 100.0%)
    if (duty_permille > 1000) {
        duty_permille = 1000;
    }

    // OCR1A = (duty * 132) / 1000
    OCR1A = ((uint32_t)duty_permille * ICR1) / 1000;
}