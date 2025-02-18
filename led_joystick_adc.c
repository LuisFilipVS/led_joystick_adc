#include <stdio.h>
#include "pico/stdlib.h"
#include "hardware/adc.h"
#include "hardware/i2c.h"
#include "hardware/pwm.h"
#include "lib/ssd1306.h"
#include "lib/font.h"

//Configuração incial das portas I2C para funcionamento do display LED
#define I2C_PORT i2c1
#define I2C_SDA 14
#define I2C_SCL 15
#define endereco 0x3C

//Configuração dos LEDs
#define LED_VERDE    11
#define LED_AZUL     12
#define LED_VERMELHO 13

//Configuração do Joystick
#define JOYSTICK_X_PIN 26  // GPIO para eixo X
#define JOYSTICK_Y_PIN 27  // GPIO para eixo Y
#define JOYSTICK_PB 22 // GPIO para botão do Joystick

//Definição do Botão para ligar ou desligar o RGB
#define Botao_A 5 // GPIO para botão A

//Instanciação dos métodos
void ligarLedAzul(float dutyCicle);
void ligarLedVerde(float dutyCicle);

static volatile uint32_t last_time = 0;

//Configuração para inciação da Placa em modo Bootsel ao selecionar o botão B
#include "pico/bootrom.h"
#define botaoB 6
void gpio_irq_handler(uint gpio, uint32_t events){
    // Obtém o tempo atual em microssegundos
    uint32_t current_time = to_us_since_boot(get_absolute_time());
    // Verifica se passou tempo suficiente desde o último evento

    if(gpio == botaoB){
        reset_usb_boot(0, 0);
    } else if (gpio == JOYSTICK_PB){
        if (current_time - last_time > 200000) // 200 ms de debouncing
        {
            last_time = current_time; // Atualiza o tempo do último evento
            printf("Botao clicado \n");
            gpio_put(LED_VERMELHO, !gpio_get(LED_VERMELHO));
        }
    }
}

uint pwm_init_gpio(uint gpio, uint wrap) {
    gpio_set_function(gpio, GPIO_FUNC_PWM);

    uint slice_num = pwm_gpio_to_slice_num(gpio);
    pwm_set_wrap(slice_num, wrap);
    
    pwm_set_enabled(slice_num, true);  
    return slice_num;  
}


int get_posicaoX(int xValue){
    int value, intensidade;
    float dutyCicle = 0.0;
    //Max Value = 64
    //Limiar = 4095
    //x 24 1891 4088
    //Y 24 1992 4089

    if (xValue > 2000){
        intensidade = (2088 - (4088 - xValue));
        dutyCicle = (float) intensidade/2088;
        value = (dutyCicle * 60) + 60;
        //printf("Para Direita: intensidade: '%d' | dutyCicle: '%f' | value: '%d' \n", intensidade, dutyCicle, value);
    
    } else if (xValue < 1900){
        intensidade = 1900 - xValue;
        dutyCicle = (float) intensidade/1900; 
        value = 60 - (dutyCicle * 60);
        //printf("Para Esquerda: intensidade: '%d' | dutyCicle: '%f' | value: '%d' \n", intensidade, dutyCicle, value);
    }

    //dutyCicle = (float) intensidade/2188; 
    ligarLedAzul(dutyCicle);
    //printf("Valor: '%d' | DutyCicle: '%f' \n", value, dutyCicle);
    return value;

}

int get_posicaoY(int yValue){
    int value, intensidade;
    float dutyCicle = 0.0;
    //Max Value = 64
    //Limiar = 4095
    //x 24 1891 4088
    //Y 24 1992 4089

    if (yValue > 2100){
        intensidade = (1988 - (4088 - yValue));
        dutyCicle = (float) intensidade/1988;
        value = 30 - (dutyCicle * 30);
        printf("Para Cima: intensidade: '%d' | dutyCicle: '%f' | value: '%d' \n", intensidade, dutyCicle, value);
    
    } else if (yValue < 2100){
        intensidade = 2100 - yValue;
        dutyCicle = (float) intensidade/2100; 
        value = 30 + (dutyCicle * 30);
        printf("Para baixo: intensidade: '%d' | dutyCicle: '%f' | value: '%d' \n", intensidade, dutyCicle, value);
    }
    
    //dutyCicle = (float) intensidade/2000;
    ligarLedVerde(dutyCicle);
    printf("Valor: '%d' | DutyCicle: '%f' | Medida: '%d' \n", value, dutyCicle, yValue);

    return value;
}

void ligarLedAzul(float dutyCicle){
    pwm_set_gpio_level(LED_AZUL, dutyCicle * 4096);
}

void ligarLedVerde(float dutyCicle){
    pwm_set_gpio_level(LED_VERDE, dutyCicle * 4096);
}

int main()
{
    stdio_init_all();
    void configurarLeds();

    gpio_init(LED_VERMELHO);
    gpio_set_dir(LED_VERMELHO, GPIO_OUT);

    //Inicialização do ADC
    adc_init();
    uint pwm_wrap = 4096; //Definição de Cobertura

    // I2C Initialisation. Using it at 400Khz.
    i2c_init(I2C_PORT, 400 * 1000);

    gpio_set_function(I2C_SDA, GPIO_FUNC_I2C); // Set the GPIO pin function to I2C
    gpio_set_function(I2C_SCL, GPIO_FUNC_I2C); // Set the GPIO pin function to I2C
    gpio_pull_up(I2C_SDA); // Pull up the data line
    gpio_pull_up(I2C_SCL); // Pull up the clock line

    ssd1306_t ssd; // Inicializa a estrutura do display
    ssd1306_init(&ssd, WIDTH, HEIGHT, false, endereco, I2C_PORT); // Inicializa o display (128 Altura e 64 Largura)

    ssd1306_config(&ssd); // Configura o display
    ssd1306_send_data(&ssd); // Envia os dados para o display

    // Limpa o display. O display inicia com todos os pixels apagados.
    ssd1306_fill(&ssd, false);
    ssd1306_send_data(&ssd);

    gpio_init(botaoB);
    gpio_set_dir(botaoB, GPIO_IN);
    gpio_pull_up(botaoB);
    gpio_set_irq_enabled_with_callback(botaoB, GPIO_IRQ_EDGE_FALL, true, &gpio_irq_handler);


    //Configuração dos LEDS para trabalharem com ADC
    uint pwm_slice_VERDE = pwm_init_gpio(LED_VERDE, pwm_wrap);
    uint pwm_slice_AZUL = pwm_init_gpio(LED_AZUL, pwm_wrap);

    //Configuração do Joystick
    gpio_init(JOYSTICK_PB);
    gpio_set_dir(JOYSTICK_PB, GPIO_IN);
    gpio_pull_up(JOYSTICK_PB);

    adc_init(); //Iniciando processo para fazer leituras ADC
    adc_gpio_init(JOYSTICK_X_PIN);
    adc_gpio_init(JOYSTICK_Y_PIN);  

    uint16_t adc_value_x;
    uint16_t adc_value_y;  
    char str_x[5];  // Buffer para armazenar a string
    char str_y[5];  // Buffer para armazenar a string  

    uint32_t last_click_time = 0;
    bool cor = true;
    int posicaoY, posicaoX;

    gpio_set_irq_enabled_with_callback(JOYSTICK_PB, GPIO_IRQ_EDGE_FALL, true, &gpio_irq_handler);
    
    while (true) {
        adc_select_input(1); // Seleciona o ADC para eixo X. O pino 26 como entrada analógica
        adc_value_x = adc_read();
        adc_select_input(0); // Seleciona o ADC para eixo Y. O pino 27 como entrada analógica
        adc_value_y = adc_read();

        sprintf(str_x, "%d", adc_value_x);  // Converte o inteiro em string
        sprintf(str_y, "%d", adc_value_y);  // Converte o inteiro em string
        
        posicaoX = get_posicaoX(adc_value_x);
        posicaoY = get_posicaoY(adc_value_y);
        
        ssd1306_fill(&ssd, !cor); // Limpa o display
        ssd1306_rect(&ssd, 3, 3, 120, 60, cor, !cor); // Desenha um retângulo
        
        ssd1306_rect(&ssd,posicaoY,posicaoX,5,5, cor, cor);
        ssd1306_send_data(&ssd); // Atualiza o display
    }
}