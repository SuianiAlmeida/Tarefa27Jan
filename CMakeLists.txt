# == DO NOT EDIT THE FOLLOWING LINES for the Raspberry Pi Pico VS Code Extension to work ==
if(WIN32)
    set(USERHOME $ENV{USERPROFILE})
else()
    set(USERHOME $ENV{HOME})
endif()
set(sdkVersion 2.1.0)
set(toolchainVersion 13_3_Rel1)
set(picotoolVersion 2.1.0)
set(picoVscode ${USERHOME}/.pico-sdk/cmake/pico-vscode.cmake)
if (EXISTS ${picoVscode})
    include(${picoVscode})
endif()
# ====================================================================================

set(PICO_BOARD pico CACHE STRING "Board type")

cmake_minimum_required(VERSION 3.13)

# Importando o SDK do Pico
include(pico_sdk_import.cmake)

# Nome do projeto
project(led_matrix_interrupt C CXX ASM)
set(CMAKE_C_STANDARD 11)
set(CMAKE_CXX_STANDARD 17)

# Inicializa o SDK
pico_sdk_init()

# Adiciona o executável principal
add_executable(led_matrix_interrupt
    main.c
)

# Garante que as bibliotecas sejam adicionadas depois do executável ser definido
target_link_libraries(led_matrix_interrupt 
    pico_stdlib
    hardware_pwm
    hardware_gpio
    hardware_pio
)

# Se houver um arquivo PIO para WS2812, gere o cabeçalho
pico_generate_pio_header(led_matrix_interrupt ${CMAKE_CURRENT_SOURCE_DIR}/ws2812.pio)

# Ativa a saída via USB e desativa UART
pico_enable_stdio_usb(led_matrix_interrupt 1)
pico_enable_stdio_uart(led_matrix_interrupt 0)

# Criação do UF2 e binários extras
pico_add_extra_outputs(led_matrix_interrupt)
