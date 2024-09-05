# Carrinho Autônomo com Desvio de Obstáculos e Controle de Velocidade

## Descrição do Projeto

Este projeto consiste em um carrinho autônomo que utiliza um **sensor ultrassônico** para detectar obstáculos e realizar manobras de desvio automaticamente. O carrinho é montado com uma estrutura de acrílico e possui **4 motores DC** que são controlados através de uma **ponte H** conectada ao **Arduino**, utilizando **PWM** para controlar a intensidade dos motores.

O sistema possui as seguintes funcionalidades:

- **Detecção de obstáculos**: O sensor ultrassônico calcula a distância até um objeto e, ao detectar que o carrinho está a uma distância menor do que o limite definido (**threshold**), ele para, faz uma manobra de ré e curva à esquerda, e então continua se movendo em linha reta até encontrar outro obstáculo.
- **Controle de velocidade**: Um **potenciômetro** conectado ao Arduino permite ajustar a intensidade e a velocidade dos motores DC, proporcionando um controle dinâmico de movimento.
- **Indicadores visuais**: O carrinho possui 3 LEDs que indicam o estado do movimento:
  - **LED verde**: Acende enquanto o carrinho está em movimento em linha reta.
  - **LED amarelo**: Acende quando o carrinho para após detectar um obstáculo.
  - **LED vermelho**: Acende durante as manobras de desvio (ré e curva).

Além disso, o Arduino se comunica com um **PIC** via **UART** para fornecer informações adicionais sobre o estado do carrinho.

## Comunicação com o PIC

A cada 1 segundo, o Arduino envia para o PIC o valor do potenciômetro, indicando a velocidade atual dos motores. O **PIC** exibe este valor em um **display LCD**.

Durante as manobras de desvio, quando o carrinho encontra um obstáculo, o Arduino também envia um comando para o PIC que:
- **Ativa o buzzer** e um **LED** no PIC enquanto o carrinho está manobrando ou parado.
- Assim que o obstáculo é desviado e o carrinho volta a se mover, o buzzer e o LED no PIC são desligados.

## Componentes Utilizados

- **Arduino**: Responsável pelo controle dos motores, leitura do sensor ultrassônico e potenciômetro, além da comunicação com o PIC via UART.
- **Sensor ultrassônico HC-SR04**: Para calcular a distância até os obstáculos.
- **Ponte H L298N**: Para controlar a direção e velocidade dos motores DC utilizando sinais PWM.
- **4 motores DC**: Para o movimento do carrinho.
- **3 LEDs (verde, amarelo, vermelho)**: Para indicar o estado do carrinho (movimento reto, parada ou manobra).
- **Potenciômetro**: Para ajustar a velocidade dos motores.
- **PIC**: Utilizado para exibir a velocidade no display LCD e controlar o buzzer e o LED durante as manobras.

## Funcionamento do Sistema

1. **Movimento em linha reta**: O carrinho se move em linha reta enquanto o sensor ultrassônico não detecta nenhum obstáculo à frente. O **LED verde** permanece aceso durante esse estado.

2. **Detecção de Obstáculo**: Quando o sensor detecta um obstáculo a uma distância menor do que o **threshold** configurado, o carrinho:
   - Para, desligando os motores (**LED amarelo** acende).
   - Faz uma manobra de ré por 1,5 segundos.
   - Gira para a esquerda por 1 segundo (**LED vermelho** acende durante a manobra).
   - Retorna a se mover em linha reta.

3. **Controle de Velocidade**: O potenciômetro ajusta dinamicamente a velocidade dos motores, variando entre valores de 150 a 220 (em termos de PWM).

4. **Comunicação com o PIC**:
   - A cada segundo, o Arduino envia o valor atual do potenciômetro para o PIC, que exibe a velocidade no **display LCD**.
   - Quando o carrinho está manobrando, o Arduino avisa o PIC, que então **ativa o buzzer** e **acende o LED** no PIC. Quando a manobra termina, o buzzer e o LED são desligados.

## Diagrama de Funcionamento

```plaintext
[Movimento Reto] --> [Detecção de Obstáculo] --> [Parada] --> [Ré] --> [Curva à Esquerda] --> [Movimento Reto]
    |                                               |                                   |
[LED Verde ON]                               [LED Amarelo ON]                      [LED Vermelho ON]
                                                                                        |
                                                                             [PIC Aciona Buzzer e LED]
````
## Vídeo

[Veja o vídeo do carrinho em funcionamento](./assets/Video.mp4)