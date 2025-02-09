// ---------------------------------------------------------------------------
// Inclusão de Bibliotecas
// ---------------------------------------------------------------------------
#include <stdio.h>
#include <stdlib.h>
#include <string.h>   // Para usar strcpy, strcat, etc.
#include <ctype.h>    // Para usar isdigit()
#include "pico/stdlib.h"
#include "hardware/gpio.h"
#include "hardware/i2c.h"
#include "hardware/pio.h" // Para uso da PIO
#include "inc/ssd1306.h"
#include "inc/font.h"
#include "ws2812.pio.h"

// ---------------------------------------------------------------------------
// Definições de pinos para o Display e Botões/LEDs
// ---------------------------------------------------------------------------
#define BUTTON_A    5
#define BUTTON_B    6
#define LED_GREEN   11
#define LED_BLUE    12

// ---------------------------------------------------------------------------
// Definições de pinos e parâmetros para o WS2812 (Matriz 5x5)
// ---------------------------------------------------------------------------
#define WS2812_PIN  7       // Pino onde está conectada a matriz WS2812
#define NUM_LEDS    25      // Tamanho da matriz (5x5)

// ---------------------------------------------------------------------------
// I2C (Display) - Ajuste conforme seu display SSD1306
// ---------------------------------------------------------------------------
#define I2C_PORT i2c1
#define I2C_SDA 14
#define I2C_SCL 15
#define OLED_ADDR 0x3C

// ---------------------------------------------------------------------------
// Globais para Display
// ---------------------------------------------------------------------------
ssd1306_t ssd;
volatile bool led_green_state = false;
volatile bool led_blue_state  = false;
absolute_time_t last_debounce_a;
absolute_time_t last_debounce_b;

// Duas strings: estado dos LEDs e último caractere digitado
static char ledStatus[32]  = "Leds desligados";
static char lastChar[32]   = "Caractere: (nada)";

// ---------------------------------------------------------------------------
// Globais para a Matriz 5x5
// ---------------------------------------------------------------------------
PIO pio = pio0;    // Usaremos a PIO0
int sm  = 0;       // State machine

// Mapeamento físico (linha, coluna) -> índice do LED
static const uint8_t LEDmap[5][5] = {
    {24, 23, 22, 21, 20},
    {15, 16, 17, 18, 19},
    {14, 13, 12, 11, 10},
    { 5,  6,  7,  8,  9},
    { 4,  3,  2,  1,  0}
};

// Mapeamento dos números 0–9 (1 = LED aceso, 0 = apagado)
static const uint8_t numbers[10][NUM_LEDS] = {
    { // 0
      1,1,1,1,1,
      1,0,0,0,1,
      1,0,0,0,1,
      1,0,0,0,1,
      1,1,1,1,1
    },
    { // 1
      0,0,1,0,0,
      0,1,1,0,0,
      0,0,1,0,0,
      0,0,1,0,0,
      0,1,1,1,0
    },
    { // 2
      1,1,1,1,1,
      0,0,0,0,1,
      1,1,1,1,1,
      1,0,0,0,0,
      1,1,1,1,1
    },
    { // 3
      1,1,1,1,1,
      0,0,0,0,1,
      0,1,1,1,1,
      0,0,0,0,1,
      1,1,1,1,1
    },
    { // 4
      1,0,0,1,0,
      1,0,0,1,0,
      1,1,1,1,1,
      0,0,0,1,0,
      0,0,0,1,0
    },
    { // 5
      1,1,1,1,1,
      1,0,0,0,0,
      1,1,1,1,1,
      0,0,0,0,1,
      1,1,1,1,1
    },
    { // 6
      1,1,1,1,1,
      1,0,0,0,0,
      1,1,1,1,1,
      1,0,0,0,1,
      1,1,1,1,1
    },
    { // 7
      1,1,1,1,1,
      0,0,0,0,1,
      0,0,0,1,0,
      0,0,1,0,0,
      0,1,0,0,0
    },
    { // 8
      1,1,1,1,1,
      1,0,0,0,1,
      1,1,1,1,1,
      1,0,0,0,1,
      1,1,1,1,1
    },
    { // 9
      1,1,1,1,1,
      1,0,0,0,1,
      1,1,1,1,1,
      0,0,0,0,1,
      1,1,1,1,1
    }
};

// ---------------------------------------------------------------------------
// Debounce (software): Retorna true se realmente pressionou
// ---------------------------------------------------------------------------
bool debounce(uint gpio, absolute_time_t *last_time) {
    absolute_time_t now = get_absolute_time();
    // Espera 200ms desde o último acionamento
    if (absolute_time_diff_us(*last_time, now) >= 200000) {
        *last_time = now;
        // Retorna true se o botão está pressionado (nível baixo)
        return (gpio_get(gpio) == 0);
    }
    return false;
}

// ---------------------------------------------------------------------------
// Monta a string de estado dos LEDs e atualiza o display
// ---------------------------------------------------------------------------
void updateLedString(void) {
    if (led_green_state && led_blue_state) {
        strcpy(ledStatus, "Verde/Azul ON");
    } else if (led_green_state) {
        strcpy(ledStatus, "Led Verde ON");
    } else if (led_blue_state) {
        strcpy(ledStatus, "Led Azul ON");
    } else {
        strcpy(ledStatus, "Verde/Azul OFF");
    }
}

// ---------------------------------------------------------------------------
// Desenha tudo no display: ledStatus + lastChar
// ---------------------------------------------------------------------------
void refresh_display(void) {
    // Limpa o buffer interno do display
    ssd1306_fill(&ssd, false);

    // Desenha o status dos LEDs na linha 0, col 0
    ssd1306_draw_string(&ssd, ledStatus, 0, 0);

    // Desenha o último caractere digitado na linha 20
    ssd1306_draw_string(&ssd, lastChar, 0, 20);

    // Envia ao display
    ssd1306_send_data(&ssd);
}

// ---------------------------------------------------------------------------
// Callback de interrupção dos botões
// ---------------------------------------------------------------------------
void gpio_callback(uint gpio, uint32_t events) {
    if (gpio == BUTTON_A && (events & GPIO_IRQ_EDGE_FALL)) {
        if (debounce(BUTTON_A, &last_debounce_a)) {
            led_green_state = !led_green_state;
            gpio_put(LED_GREEN, led_green_state);

            updateLedString();
            printf("LED Verde: %s\n", led_green_state ? "Ligado" : "Desligado");
            refresh_display();
        }
    }
    else if (gpio == BUTTON_B && (events & GPIO_IRQ_EDGE_FALL)) {
        if (debounce(BUTTON_B, &last_debounce_b)) {
            led_blue_state = !led_blue_state;
            gpio_put(LED_BLUE, led_blue_state);

            updateLedString();
            printf("LED Azul: %s\n", led_blue_state ? "Ligado" : "Desligado");
            refresh_display();
        }
    }
}

// ---------------------------------------------------------------------------
// Inicializa a PIO para a matriz 5x5 de WS2812
// ---------------------------------------------------------------------------
void setupMatrix(void) {
    // Seleciona uma state machine disponível
    sm = pio_claim_unused_sm(pio, true);
    // Carrega o programa ws2812
    uint offset = pio_add_program(pio, &ws2812_program);
    // Inicializa a state machine (pino, freq, etc.)
    ws2812_program_init(pio, sm, offset, WS2812_PIN, 800000, false);
}

// ---------------------------------------------------------------------------
// Desliga (limpa) completamente a matriz (tudo apagado)
// ---------------------------------------------------------------------------
void clearMatrix(void) {
    uint32_t color_off = 0x000000 << 8;
    uint32_t ledBuffer[NUM_LEDS];
    for (int i = 0; i < NUM_LEDS; i++) {
        ledBuffer[i] = color_off; // Apagado
    }
    // Enviar para os 25 LEDs
    for (int i = 0; i < NUM_LEDS; i++) {
        pio_sm_put_blocking(pio, sm, ledBuffer[i]);
    }
}

// ---------------------------------------------------------------------------
// Exibe um dígito [0-9] na matriz 5x5
// ---------------------------------------------------------------------------
void displayNumberOnMatrix(int digit) {
    // Cor ligada (por exemplo, azul)
    uint32_t color_on  = 0x0000FF << 8;
    // Cor desligada
    uint32_t color_off = 0x000000 << 8;

    // Buffer local de 25 LEDs
    uint32_t ledBuffer[NUM_LEDS];
    for (int i = 0; i < NUM_LEDS; i++) {
        ledBuffer[i] = color_off; // Desligado por padrão
    }

    // Preenche o ledBuffer[] de acordo com numbers[digit] e o mapeamento LEDmap
    for (int row = 0; row < 5; row++) {
        for (int col = 0; col < 5; col++) {
            int logicalIndex = row * 5 + col; // 0..24
            if (numbers[digit][logicalIndex] == 1) {
                int physicalIndex = LEDmap[row][col];
                ledBuffer[physicalIndex] = color_on;
            }
        }
    }

    // Enviar o buffer para os 25 LEDs da matriz
    for (int i = 0; i < NUM_LEDS; i++) {
        pio_sm_put_blocking(pio, sm, ledBuffer[i]);
    }
}

// ---------------------------------------------------------------------------
// Setup do sistema
// ---------------------------------------------------------------------------
void setup() {
    stdio_init_all();
    sleep_ms(2000); // atraso opcional para a USB CDC

    printf("Iniciando...\n");

    // I2C -> Display
    i2c_init(I2C_PORT, 400 * 1000);
    gpio_set_function(I2C_SDA, GPIO_FUNC_I2C);
    gpio_set_function(I2C_SCL, GPIO_FUNC_I2C);
    gpio_pull_up(I2C_SDA);
    gpio_pull_up(I2C_SCL);

    // Inicializa o display SSD1306
    ssd1306_init(&ssd, WIDTH, HEIGHT, false, OLED_ADDR, I2C_PORT);
    ssd1306_fill(&ssd, false);
    ssd1306_send_data(&ssd);

    // LEDs Verde e Azul
    gpio_init(LED_GREEN);
    gpio_set_dir(LED_GREEN, GPIO_OUT);
    gpio_init(LED_BLUE);
    gpio_set_dir(LED_BLUE, GPIO_OUT);

    // Botões
    gpio_init(BUTTON_A);
    gpio_set_dir(BUTTON_A, GPIO_IN);
    gpio_pull_up(BUTTON_A);

    gpio_init(BUTTON_B);
    gpio_set_dir(BUTTON_B, GPIO_IN);
    gpio_pull_up(BUTTON_B);

    // Interrupções (bordas de descida)
    gpio_set_irq_enabled_with_callback(BUTTON_A, GPIO_IRQ_EDGE_FALL, true, &gpio_callback);
    gpio_set_irq_enabled(BUTTON_B, GPIO_IRQ_EDGE_FALL, true);

    // Registra tempo inicial p/ debounce
    last_debounce_a = get_absolute_time();
    last_debounce_b = get_absolute_time();

    // Strings iniciais
    updateLedString();
    strcpy(lastChar, "NADA");
    refresh_display();
    printf("Setup concluído. Digite algo...\n");

    // Setup da Matriz 5x5 WS2812
    setupMatrix();

    // Inicia a matriz desligada (tudo apagado)
    clearMatrix();
}

// ---------------------------------------------------------------------------
// Função principal
// ---------------------------------------------------------------------------
int main() {
    setup();

    // Loop principal
    while (true) {
        // Bloqueia até o usuário digitar algo e apertar ENTER
        int c = getchar(); 
        // Se c == PICO_ERROR_TIMEOUT (ou < 0) é erro ou timeout

        if (c >= 32 && c < 127) {
            // Caractere ASCII imprimível
            printf("Recebido: %c\n", (char)c);

            // Preenche lastChar com esse caractere e atualiza display
            sprintf(lastChar, "CARACTERE: %c", (char)c);
            refresh_display();

            // Se for um dígito 0-9, atualiza a matriz
            if (isdigit(c)) {
                int digit = c - '0';
                displayNumberOnMatrix(digit);
            }
            else {
                // Qualquer caractere não-dígito -> desliga a matriz
                clearMatrix();
            }
        }
        else {
            // Se for ENTER, etc. (não imprimível)
            printf("Recebido ASCII %d (não imprimível)\n", c);
            // Desliga a matriz também, se quiser esse comportamento
            clearMatrix();
        }
    }
    return 0;
}
