# Controle Avançado de LEDs, Display SSD1306 e Matriz WS2812 com RP2040 

## 📝 Visão Geral
Este projeto explora a interação entre um microcontrolador RP2040 (BitDogLab) e periféricos eletrônicos, utilizando comunicação serial (UART e I2C). O código implementa:

- 🎛 Manipulação de LEDs comuns e endereçáveis WS2812 (Matriz 5x5);
- 📟 Controle de um display OLED SSD1306 via I2C;
- 🎮 Interação com botões por meio de interrupções (IRQ) e debounce por software;
- 🔆 Alternância do LED central da placa.

---

## 🚀 Funcionalidades
### 🔡 Entrada de caracteres via UART
✅ O caractere digitado no **Serial Monitor** é mostrado no **display SSD1306**.
✅ Se for um número (0-9), a **matriz WS2812** exibirá o padrão correspondente.
✅ Caracteres não numéricos **apagam a matriz de LEDs**.

### 🔘 Controle de LEDs e Botões
🟢 **Botão A (GPIO 5):** Alterna o estado do LED verde (GPIO 11).
🔵 **Botão B (GPIO 6):** Alterna o estado do LED azul (GPIO 12).
📢 As alterações são registradas no **display SSD1306** e no **Serial Monitor**.

### 🔳 Controle da Matriz WS2812 (5x5)
💡 Exibição de padrões numéricos (0 a 9) na matriz WS2812.
🛠️ Implementação de **debounce** para evitar leituras incorretas.

---

## 🛠️ Componentes Utilizados
- 🖥️ **Placa BitDogLab (RP2040)**
- 🔲 **Matriz 5x5 WS2812** (GPIO 7)
- 🔴🟢🔵 **LED RGB** (GPIOs 11, 12, 13)
- 🔘 **Botão A** (GPIO 5)
- 🔘 **Botão B** (GPIO 6)
- 📟 **Display SSD1306** via I2C (GPIOs 14 e 15)

---

## 📂 Estrutura do Repositório
```
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
```

---

## ⚙️ Configuração e Execução
### 📌 Dependências
🔹 **Pico SDK**
🔹 **CMake**
🔹 **Toolchain para RP2040**

### 📥 Passos para Compilar
```sh
git clone <URL_DO_REPOSITORIO>
cd <NOME_DO_REPOSITORIO>
source ~/pico-sdk/pico_sdk_init.sh
mkdir build && cd build
cmake ..
make -j4
```

### 🚀 Upload do Firmware
1. Pressione o **BOOTSEL** e o **RESET** na BitDogLab e conecte via USB.
2. Arraste o arquivo `.uf2` gerado para a unidade montada.

---

## 🎮 Como Utilizar
1. Conecte a placa BitDogLab ao PC via **USB**.
2. Abra o **Serial Monitor** (Baud Rate: 115200).
3. Digite um caractere ou um número e observe sua exibição no **display SSD1306** e na **matriz WS2812** (caso número).
4. Pressione os botões para alternar os LEDs.
5. Ações registradas no **Serial Monitor**.

---

## 🎥 Demonstração em Vídeo
📹 *Adicione aqui o link para o vídeo de demonstração do projeto.*

---

## 📝 Licença
Este projeto está sob a **licença MIT**. Fique à vontade para modificar e aprimorar conforme necessário.

---

## 👨‍💻 Desenvolvedor
Projeto criado por **Ighor Dias** no curso **Formação Básica em Software Embarcado - Embarcatech**.

---

## 📚 Referências
🔗 [Documentação do Pico SDK](https://datasheets.raspberrypi.com/pico/raspberry-pi-pico-c-sdk.pdf)
🔗 [Guia do display SSD1306](https://learn.adafruit.com/monochrome-oled-breakouts/arduino-library-and-examples)
🔗 [Uso de LEDs WS2812 com RP2040](https://github.com/raspberrypi/pico-examples/tree/master/pio/ws2812)

