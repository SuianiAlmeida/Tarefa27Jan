#include <stdio.h>
#include "pico/stdlib.h"
#include "hardware/gpio.h"
#include "hardware/pwm.h"
#include "hardware/irq.h"
#include "hardware/pio.h"
#include "ws2812.pio.h"

// Pin definitions
#define LED_MATRIX_PIN 7
#define LED_RED_PIN 11
#define LED_GREEN_PIN 12
#define LED_BLUE_PIN 13
#define BUTTON_A_PIN 5
#define BUTTON_B_PIN 6

// WS2812 LED Matrix configuration
#define MATRIX_WIDTH 5
#define MATRIX_HEIGHT 5
#define NUM_PIXELS (MATRIX_WIDTH * MATRIX_HEIGHT)

// Debounce configuration
#define DEBOUNCE_DELAY_MS 50

// PIO instance and state machine
#define PIO_INSTANCE pio0
#define SM 0  // State Machine