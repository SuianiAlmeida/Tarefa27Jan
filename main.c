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

// Function prototypes
void gpio_callback(uint gpio, uint32_t events);
void update_led_matrix(int number);
void init_led_matrix(void);
void set_led_pixel(int x, int y, uint32_t color);

// Button interrupt callback
void gpio_callback(uint gpio, uint32_t events) {
    uint32_t current_time = to_ms_since_boot(get_absolute_time());
    
    if (gpio == BUTTON_A_PIN) {
        if (current_time - last_button_a_time >= DEBOUNCE_DELAY_MS) {
            if (current_number < 9) {
                current_number++;
                button_a_pressed = true;
            }
            last_button_a_time = current_time;
        }
    } else if (gpio == BUTTON_B_PIN) {
        if (current_time - last_button_b_time >= DEBOUNCE_DELAY_MS) {
            if (current_number > 0) {
                current_number--;
                button_b_pressed = true;
            }
            last_button_b_time = current_time;
        }
    }
}

// Initialize LED matrix
void init_led_matrix(void) {
    PIO pio = PIO_INSTANCE;
    int sm = SM;
    uint offset = pio_add_program(pio, &ws2812_program);
    ws2812_program_init(pio, sm, offset, LED_MATRIX_PIN, 800000, false);
}

// Set individual LED in the matrix
void set_led_pixel(int x, int y, uint32_t color) {
    int index = y * MATRIX_WIDTH + x;  // Mapeia a posição para um índice
    uint32_t rgb = ((color & 0xFF0000) >> 8) | ((color & 0x00FF00) << 8) | ((color & 0x0000FF) << 16);
    pio_sm_put_blocking(PIO_INSTANCE, SM, rgb << 8u);  // WS2812 usa GRB, não RGB
}

// Update LED matrix display
void update_led_matrix(int number) {
    if (number < 0 || number > 9) return;
    
    for (int y = 0; y < MATRIX_HEIGHT; y++) {
        for (int x = 0; x < MATRIX_WIDTH; x++) {
            if (number_patterns[number][y][x]) {
                set_led_pixel(x, y, 0x00FF00); // Verde
            } else {
                set_led_pixel(x, y, 0x000000); // Desligado
            }
        }
    }
}

// Apaga todos os LEDs da matriz ao iniciar
void clear_led_matrix() {
    for (int y = 0; y < MATRIX_HEIGHT; y++) {
        for (int x = 0; x < MATRIX_WIDTH; x++) {
            set_led_pixel(x, y, 0x000000); // Apaga todos os LEDs
        }
    }
}

int main() {
    stdio_init_all();

    gpio_init(LED_RED_PIN);
    gpio_set_dir(LED_RED_PIN, GPIO_OUT);
    
    gpio_init(BUTTON_A_PIN);
    gpio_init(BUTTON_B_PIN);
    gpio_set_dir(BUTTON_A_PIN, GPIO_IN);
    gpio_set_dir(BUTTON_B_PIN, GPIO_IN);
    gpio_pull_up(BUTTON_A_PIN);
    gpio_pull_up(BUTTON_B_PIN);

    gpio_set_irq_enabled_with_callback(BUTTON_A_PIN, GPIO_IRQ_EDGE_FALL, true, &gpio_callback);
    gpio_set_irq_enabled_with_callback(BUTTON_B_PIN, GPIO_IRQ_EDGE_FALL, true, &gpio_callback);

    init_led_matrix();
    clear_led_matrix();  // Chama a função para apagar a matriz antes de iniciar

    while (1) {
        gpio_put(LED_RED_PIN, 1);
        sleep_ms(100);
        gpio_put(LED_RED_PIN, 0);
        sleep_ms(100);

        if (button_a_pressed || button_b_pressed) {
            update_led_matrix(current_number);
            button_a_pressed = false;
            button_b_pressed = false;
        }
    }

    return 0;
}