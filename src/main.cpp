#include <Arduino.h>
#include <Adafruit_NeoPixel.h>

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

// Global variables
volatile int current_number = 0;
volatile uint32_t last_button_a_time = 0;
volatile uint32_t last_button_b_time = 0;
volatile bool button_a_pressed = false;
volatile bool button_b_pressed = false;

// Initialize NeoPixel strip object
Adafruit_NeoPixel matrix(NUM_PIXELS, LED_MATRIX_PIN, NEO_GRB + NEO_KHZ800);

// LED patterns for numbers 0-9 (5x5 matrix)
const bool number_patterns[10][MATRIX_HEIGHT][MATRIX_WIDTH] = {
    // 0
    {
        {1, 1, 1, 1, 1},
        {1, 0, 0, 0, 1},
        {1, 0, 0, 0, 1},
        {1, 0, 0, 0, 1},
        {1, 1, 1, 1, 1}
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
        {0, 0, 0, 0, 1},
        {1, 1, 1, 1, 1},
        {1, 0, 0, 0, 0},
        {1, 1, 1, 1, 1}
    },
    // 3
    {
        {1, 1, 1, 1, 1},
        {0, 0, 0, 0, 1},
        {0, 1, 1, 1, 1},
        {0, 0, 0, 0, 1},
        {1, 1, 1, 1, 1}
    },
    // 4
    {
        {1, 0, 0, 0, 1},
        {1, 0, 0, 0, 1},
        {1, 1, 1, 1, 1},
        {0, 0, 0, 0, 1},
        {0, 0, 0, 0, 1}
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
        {0, 0, 0, 0, 1},
        {0, 0, 0, 1, 0},
        {0, 0, 1, 0, 0},
        {0, 1, 0, 0, 0}
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

// Button A interrupt handler
void IRAM_ATTR buttonA_ISR() {
    uint32_t current_time = millis();
    if (current_time - last_button_a_time >= DEBOUNCE_DELAY_MS) {
        if (current_number < 9) {
            current_number++;
            button_a_pressed = true;
        }
        last_button_a_time = current_time;
    }
}

// Button B interrupt handler
void IRAM_ATTR buttonB_ISR() {
    uint32_t current_time = millis();
    if (current_time - last_button_b_time >= DEBOUNCE_DELAY_MS) {
        if (current_number > 0) {
            current_number--;
            button_b_pressed = true;
        }
        last_button_b_time = current_time;
    }
}

// Update LED matrix display
void update_led_matrix(int number) {
    if (number < 0 || number > 9) return;
    
    matrix.clear();
    
    // Update the LED matrix based on the number pattern
    for (int y = 0; y < MATRIX_HEIGHT; y++) {
        for (int x = 0; x < MATRIX_WIDTH; x++) {
            if (number_patterns[number][y][x]) {
                // Calculate pixel index
                int pixel = y * MATRIX_WIDTH + x;
                matrix.setPixelColor(pixel, matrix.Color(0, 255, 0)); // Green color
            }
        }
    }
    
    matrix.show();
}

void setup() {
    // Initialize serial communication
    Serial.begin(115200);
    
    // Initialize LED pins
    pinMode(LED_RED_PIN, OUTPUT);
    pinMode(LED_GREEN_PIN, OUTPUT);
    pinMode(LED_BLUE_PIN, OUTPUT);
    
    // Initialize buttons with pull-up and interrupts
    pinMode(BUTTON_A_PIN, INPUT_PULLUP);
    pinMode(BUTTON_B_PIN, INPUT_PULLUP);
    attachInterrupt(digitalPinToInterrupt(BUTTON_A_PIN), buttonA_ISR, FALLING);
    attachInterrupt(digitalPinToInterrupt(BUTTON_B_PIN), buttonB_ISR, FALLING);
    
    // Initialize NeoPixel matrix
    matrix.begin();
    matrix.setBrightness(50); // Set brightness to 50%
    matrix.show(); // Initialize all pixels to 'off'
    
    // Initial display
    update_led_matrix(current_number);
}

void loop() {
    // Blink red LED (5 times per second = 100ms on, 100ms off)
    digitalWrite(LED_RED_PIN, HIGH);
    delay(100);
    digitalWrite(LED_RED_PIN, LOW);
    delay(100);
    
    // Update LED matrix if number changed
    if (button_a_pressed || button_b_pressed) {
        update_led_matrix(current_number);
        button_a_pressed = false;
        button_b_pressed = false;
    }
}
