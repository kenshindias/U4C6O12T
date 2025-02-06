Projeto: Controle de LEDs, Display SSD1306 e Matriz WS2812 com RP2040

Descrição

Este projeto tem como objetivo demonstrar o uso de comunicação serial (UART e I2C) e a interação com periféricos em um microcontrolador RP2040 (placa BitDogLab). O código implementa a manipulação de LEDs, um display SSD1306 via I2C, e uma matriz de LEDs WS2812, além da interação com botões utilizando interrupções (IRQ) e debounce por software.

Funcionalidades

Entrada de caracteres via UART

O caractere digitado no Serial Monitor é exibido no display SSD1306.

Se o caractere for um número entre 0 e 9, a matriz WS2812 exibirá o padrão correspondente ao dígito.

Para caracteres não numéricos, a matriz de LEDs será apagada.

Interação com botões

Botão A (GPIO 5): Alterna o estado do LED verde (GPIO 11).

Botão B (GPIO 6): Alterna o estado do LED azul (GPIO 12).

O estado dos LEDs é atualizado no display SSD1306 e enviado ao Serial Monitor.

Controle de matriz 5x5 de LEDs WS2812

Exibe um padrão correspondente a números de 0 a 9.

Implementa debounce via software.

Hardware Utilizado

Placa BitDogLab (RP2040)

Matriz 5x5 WS2812 conectada ao GPIO 7

LED RGB conectado aos GPIOs 11, 12 e 13

Botão A conectado ao GPIO 5

Botão B conectado ao GPIO 6

Display SSD1306 via I2C (GPIOs 14 e 15)

Estrutura do Repositório

|-- inc/
|   |-- font.h
|   |-- ssd1306.h
|-- .gitignore
|-- CMakeLists.txt
|-- diagram.json
|-- pico_sdk_import.cmake
|-- U4C6O12T.c
|-- wokwi.toml
|-- ws2812.pio

Compilação e Execução

Dependências

Antes de compilar, certifique-se de ter instalado:

Pico SDK

CMake

Toolchain para RP2040

Passos para compilar

Clone o repositório e navegue até a pasta do projeto:

git clone <URL_DO_REPOSITORIO>
cd <NOME_DO_REPOSITORIO>

Configure o ambiente:

source ~/pico-sdk/pico_sdk_init.sh

Crie e entre na pasta build/:

mkdir build && cd build

Execute o CMake:

cmake ..

Compile o projeto:

make -j4

Envie o firmware para a placa RP2040:

Pressione o BOOTSEL na BitDogLab e conecte via USB.

Arraste o arquivo .uf2 gerado para a unidade montada.

Uso do Projeto

Conecte a placa RP2040 ao PC via USB.

Abra o Serial Monitor no VS Code ou outro terminal serial (Baud Rate: 115200).

Digite um caractere no Serial Monitor e observe o display SSD1306 e a matriz WS2812.

Pressione os botões para alternar os LEDs.

O estado dos LEDs e os caracteres digitados serão exibidos no Serial Monitor.

Licença

Este projeto está sob a licença MIT. Sinta-se livre para modificar e usar conforme necessário.

Autor

Desenvolvido por [Seu Nome] como parte do curso de Introdução às Interfaces de Comunicação Serial com RP2040.

Referências

Documentação do Pico SDK

Guia do display SSD1306

Uso de LEDs WS2812 com RP2040
