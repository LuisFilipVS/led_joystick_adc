## 🎮 Controle de LEDs e Display OLED com Raspberry Pi Pico 💡

### Autor
Luis Filipe Vieira Silva

Este projeto implementa um sistema de controle de LEDs RGB e um display OLED utilizando um Raspberry Pi Pico. O código permite interação através de um joystick analógico 🎚️ e botões, controlando a intensidade dos LEDs e exibindo informações gráficas no display.

## 🛠️ Componentes Utilizados

- 🖥️ Raspberry Pi Pico
- 🖼️ Display OLED SSD1306 (I2C)
- 🎮 Joystick analógico
- 🔘 Botões (para reset e controle)
- 🌈 LEDs RGB (Verde, Azul, Vermelho)

## 📚 Bibliotecas Necessárias

O código utiliza as seguintes bibliotecas:

- `pico/stdlib.h` (Funções padrão do Raspberry Pi Pico)
- `hardware/adc.h` (Conversão Analógica-Digital para o joystick)
- `hardware/i2c.h` (Comunicação com o display OLED via I2C)
- `hardware/pwm.h` (Controle dos LEDs via PWM)
- `ssd1306.h` e `font.h` (Bibliotecas para manipulação do display OLED SSD1306)

## ⚡ Configuração dos Pinos

- **📟 I2C (Display OLED SSD1306):**
  - SDA: GPIO 14
  - SCL: GPIO 15
- **💡 LEDs:**
  - 🟢 Verde: GPIO 11
  - 🔵 Azul: GPIO 12
  - 🔴 Vermelho: GPIO 13
- **🎮 Joystick:**
  - Eixo X: GPIO 26
  - Eixo Y: GPIO 27
  - Botão: GPIO 22
- **🔘 Botões adicionais:**
  - Botão A (Liga/Desliga RGB): GPIO 5
  - Botão B (Modo Bootsel/Reset): GPIO 6

## ⚙️ Funcionamento do Código

1. **🔌 Inicialização:**

   - Configura os pinos de LEDs, joystick e botões.
   - Inicializa a comunicação I2C e configura o display OLED.
   - Configura interrupções para os botões.

2. **📏 Leitura do Joystick:**

   - O código lê os valores analógicos dos eixos X e Y.
   - Ajusta a intensidade dos LEDs conforme o movimento do joystick.

3. **💡 Controle dos LEDs RGB:**

   - Utiliza PWM para controlar a intensidade dos LEDs azul e verde.
   - Alterna o estado do LED vermelho quando o botão do joystick é pressionado.

4. **📺 Exibição no Display OLED:**

   - Um retângulo é desenhado na tela representando os movimentos do joystick.
   - As coordenadas X e Y são atualizadas em tempo real.

## 🛠️ Como Compilar e Executar

1. Certifique-se de que o SDK do Raspberry Pi Pico está instalado.
2. Compile o código com o CMake:
   ```sh
   mkdir build
   cd build
   cmake ..
   make
   ```
3. 🚀 Envie o arquivo `.uf2` gerado para o Raspberry Pi Pico.

## 🔎 Observações

- O botão B pode ser usado para reiniciar o Raspberry Pi Pico em modo BOOTSEL.
- Há um sistema de debounce implementado para evitar leituras indesejadas dos botões.

---

🚀 Projeto desenvolvido para controle interativo de LEDs e display OLED utilizando o Raspberry Pi Pico.


## 🎥 Vídeo Demonstrativo

🔗 [Clique aqui para assistir](https://drive.google.com/file/d/1dvplmqpv3u8oHRhB_Ceebqie-QApQYYW/view?usp=sharing)


