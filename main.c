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

// Global variables
volatile int current_number = 0;
volatile uint32_t last_button_a_time = 0;
volatile uint32_t last_button_b_time = 0;
volatile bool button_a_pressed = false;
volatile bool button_b_pressed = false;

// LED patterns for numbers 0-9 (5x5 matrix)
const bool number_patterns[10][MATRIX_HEIGHT][MATRIX_WIDTH] = {
    // 0
    {
        {0, 1, 1, 1, 0},
        {1, 0, 0, 0, 1},
        {1, 0, 0, 0, 1},
        {1, 0, 0, 0, 1},
        {0, 1, 1, 1, 0}
    },
    // 1
    {
        {0, 0, 1, 0, 0},
        {0, 1, 1, 0, 0},
        {0, 0, 1, 0, 0},
        {0, 0, 1, 0, 0},
        {0, 1, 1, 1, 0}
    },
    // 2
    {
        {1, 1, 1, 1, 1},
        {0, 0, 0, 1, 0},
        {0, 0, 1, 0, 0},
        {0, 1, 0, 0, 0},
        {1, 1, 1, 1, 1}
    },
    // 3
    {
        {1, 1, 1, 1, 1},
        {0, 0, 0, 0, 1},
        {0, 1, 1, 1, 0},
        {0, 0, 0, 0, 1},
        {1, 1, 1, 1, 1}
    },
    // 4
    {
        {1, 0, 0, 1, 0},
        {1, 0, 0, 1, 0},
        {1, 1, 1, 1, 1},
        {0, 0, 0, 1, 0},
        {0, 0, 0, 1, 0}
    },
    // 5
    {
        {1, 1, 1, 1, 1},
        {1, 0, 0, 0, 0},
        {1, 1, 1, 1, 1},
        {0, 0, 0, 0, 1},
        {1, 1, 1, 1, 1}
    },
    // 6
    {
        {1, 1, 1, 1, 1},
        {1, 0, 0, 0, 0},
        {1, 1, 1, 1, 1},
        {1, 0, 0, 0, 1},
        {1, 1, 1, 1, 1}
    },
    // 7
    {
        {1, 1, 1, 1, 1},
        {0, 0, 0, 1, 0},
        {0, 0, 1, 0, 0},
        {0, 1, 0, 0, 0},
        {1, 0, 0, 0, 0}
    },
    // 8
    {
        {1, 1, 1, 1, 1},
        {1, 0, 0, 0, 1},
        {1, 1, 1, 1, 1},
        {1, 0, 0, 0, 1},
        {1, 1, 1, 1, 1}
    },
    // 9
    {
        {1, 1, 1, 1, 1},
        {1, 0, 0, 0, 1},
        {1, 1, 1, 1, 1},
        {0, 0, 0, 0, 1},
        {1, 1, 1, 1, 1}
    }
};