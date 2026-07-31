#ifndef DEFINITIONS_H
#define DEFINITIONS_H

#include <stdint.h>

// - - - - - - - - - - -  PINS DEFINITIONS - - - - - - - - - -
#define PWM_PIN    (PB1) // Arduino Pin 9 (OC1A)
#define BUTTON_PIN (PD2) // Arduino Pin 2

// - - - - - - - - - - -  USEFUL MACROS - - - - - - - - - -
#define PIN_B_ENABLE(PIN) (DDRB |= (1 << PIN))
#define PIN_B_ON(PIN) (PORTB |= (1 << PIN))
#define PIN_B_OFF(PIN) (PORTB &= ~(1 << PIN))

// - - - - - - - - - - -  FUNCTIONS - - - - - - - - - -
/**
 * @brief Configures Timer1 for Fast PWM mode (~120kHz) on PB1
 */
void timer1_pwm_init(void);

/**
 * @brief Configures the button pin as input with internal pull-up
 */
void button_init(void);

/**
 * @brief Sets the duty cycle as a percentage (0 to 100)
 * @param duty_percent Integer percentage between 0 and 100
 */
void pwm_set_duty_percent(uint8_t duty_percent);

/**
 * @brief Sets the duty cycle with higher precision (0 to 1000 representing 0.0% to 100.0%)
 * @param duty_permille Permille value between 0 and 1000
 */
void pwm_set_duty_permille(uint16_t duty_permille);

#endif // DEFINITIONS_H