✨ Projeto: Controle de LEDs, Display SSD1306 e Matriz WS2812 com RP2040 ✨

📜 Descrição

Este projeto demonstra o uso de comunicação serial (UART e I2C) e interação com periféricos no microcontrolador RP2040 (placa BitDogLab). O código implementa:

Manipulação de LEDs comuns e endereçáveis WS2812;

Controle de um display OLED SSD1306 via I2C;

Interação com botões utilizando interrupções (IRQ) e debounce por software.

🚀 Funcionalidades

🔡 Entrada de caracteres via UART

✅ O caractere digitado no Serial Monitor é exibido no display SSD1306.
✅ Se for um número (0-9), a matriz WS2812 exibirá o padrão correspondente.
✅ Caracteres não numéricos apagam a matriz de LEDs.

🔘 Interação com botões

🟢 Botão A (GPIO 5): Alterna o estado do LED verde (GPIO 11).
🔵 Botão B (GPIO 6): Alterna o estado do LED azul (GPIO 12).
📢 O estado dos LEDs é atualizado no display SSD1306 e enviado ao Serial Monitor.

🔳 Controle da matriz 5x5 WS2812

💡 Exibe padrões correspondentes a números de 0 a 9.
🛠️ Implementação de debounce para evitar leituras incorretas.

🛠️ Hardware Utilizado

🖥️ Placa BitDogLab (RP2040)

🔲 Matriz 5x5 WS2812 (GPIO 7)

🔴🟢🔵 LED RGB (GPIOs 11, 12, 13)

🔘 Botão A (GPIO 5)

🔘 Botão B (GPIO 6)

🖥️ Display SSD1306 via I2C (GPIOs 14 e 15)

📂 Estrutura do Repositório

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

⚙️ Compilação e Execução

📌 Dependências

🔹 Pico SDK
🔹 CMake
🔹 Toolchain para RP2040

📥 Passos para Compilar

git clone <URL_DO_REPOSITORIO>
cd <NOME_DO_REPOSITORIO>
source ~/pico-sdk/pico_sdk_init.sh
mkdir build && cd build
cmake ..
make -j4

🚀 Envio do Firmware

Pressione o BOOTSEL na BitDogLab e conecte via USB.

Arraste o arquivo .uf2 gerado para a unidade montada.

🎮 Uso do Projeto

Conecte a placa RP2040 ao PC via USB.

Abra o Serial Monitor (Baud Rate: 115200).

Digite um caractere e observe as reações no display SSD1306 e na matriz WS2812.

Pressione os botões para alternar os LEDs.

As ações serão exibidas no Serial Monitor.

🎥 Demonstração em Vídeo

📹 Adicione aqui o link para o vídeo de demonstração do projeto.

📝 Licença

Este projeto está sob a licença MIT. Sinta-se livre para modificar e usar conforme necessário.

👤 Autor

Desenvolvido por Ighor Dias como parte do curso Formação Básica em Software Embarcado, oferecido pelo Embarcatech.

📚 Referências

🔗 Documentação do Pico SDK
🔗 Guia do display SSD1306
🔗 Uso de LEDs WS2812 com RP2040
