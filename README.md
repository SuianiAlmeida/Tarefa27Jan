# Projeto LED Matrix

Este projeto implementa um contador digital usando a placa BitDogLab com o microcontrolador RP2040, demonstrando o uso de interrupções, debouncing de botões e controle de LEDs.

## Funcionalidades

1. LED RGB piscando em vermelho 5 vezes por segundo
2. Botão A (GPIO 5) incrementa o número exibido
3. Botão B (GPIO 6) decrementa o número exibido
4. Matriz de LEDs WS2812 5x5 exibindo números de 0 a 9

## Componentes Utilizados

- Matriz 5x5 de LEDs WS2812 (GPIO 7)
- LED RGB (GPIOs 11, 12, 13)
- Botão A (GPIO 5)
- Botão B (GPIO 6)

## Requisitos de Software

- Raspberry Pi Pico SDK
- CMake
- Compilador GCC para ARM

## Como Compilar

1. Configure a variável de ambiente PICO_SDK_PATH apontando para o diretório do SDK:
```bash
export PICO_SDK_PATH=/caminho/para/pico-sdk
```

2. Crie e entre no diretório de build:
```bash
mkdir build
cd build
```

3. Execute o CMake e compile:
```bash
cmake ..
make
```

4. O arquivo .uf2 será gerado em build/led_matrix_interrupt.uf2

## Características Técnicas

- Implementação de debouncing via software
- Uso de interrupções para os botões
- Controle de LEDs WS2812
- LED RGB piscando em 5Hz