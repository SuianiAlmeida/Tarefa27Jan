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

 ## 📝 Licença

Este projeto está licenciado sob a Licença MIT - veja para mais detalhes.

MIT License

Copyright (c) 2024 Embarcatech Team

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.

---
Desenvolvido como parte do projeto Embarcatech 2024

## 🎯 Desenvolvedora

<table>
</td>
    <td align="center">
      <a href="https://github.com/suianialmeida">
        <img src="https://github.com/suianialmeida.png" width="100px;" alt="Foto de Suiani Almeida no GitHub"/><br>
        <b>Suiani Almeida</b>
        <p>Desenvolvedora em Sistemas Embarcados e IoT</p>
      </a>
      </td>
