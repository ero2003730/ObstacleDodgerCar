// Definições para o sensor ultrassônico
#define TRIGGER_PIN 51  // Pino do Trigger
#define ECHO_PIN 50     // Pino do Echo

// Definições para os pinos do driver de motor com PWM
const int PIN_MOTOR_IN1 = 2;  // Controle de direção das rodas direitas
const int PIN_MOTOR_IN2 = 3;  // Controle de direção das rodas esquerdas

// Definições para os pinos de PWM dos motores
const int PIN_PWM_MOTOR_DIREITA = 6;  // PWM para as rodas direitas
const int PIN_PWM_MOTOR_ESQUERDA = 7; // PWM para as rodas esquerdas

// Definição do pino do potenciômetro
const int PIN_POT = A0;

// Definição do pino do buzzer
const int PIN_BUZZER = 8;

// Constantes auxiliares
const int DISTANCIA_SEGURA = 100; // [cm] - Distância mínima segura
const int PAUSA = 100; // [ms] - Pausa entre leituras

void setup() {
  Serial.begin(9600);

  // Configura os pinos do sensor ultrassônico
  pinMode(TRIGGER_PIN, OUTPUT);
  pinMode(ECHO_PIN, INPUT);

  // Configura os pinos do driver de motor
  pinMode(PIN_MOTOR_IN1, OUTPUT);
  pinMode(PIN_MOTOR_IN2, OUTPUT);

  // Configura os pinos PWM
  pinMode(PIN_PWM_MOTOR_DIREITA, OUTPUT);
  pinMode(PIN_PWM_MOTOR_ESQUERDA, OUTPUT);

  // Configura o pino do buzzer
  pinMode(PIN_BUZZER, OUTPUT);

  // Configura o pino do potenciômetro
  pinMode(PIN_POT, INPUT);

  parar(); // Robo parado por padrão
  desligar_buzzer(); // Buzzer desligado por padrão
}

void loop() {
  // Lê o valor do potenciômetro e converte para uma velocidade PWM (0 a 255)
  int velocidadePWM = map(analogRead(PIN_POT), 0, 1023, 0, 255);

  // Medir a distância usando o sensor ultrassônico
  long distancia = medirDistancia();

  // Exibir a distância no monitor serial
  Serial.print("Distancia: ");
  Serial.print(distancia);
  Serial.println(" cm");

  // Se a distância for menor que a segura, o buzzer liga e o robô faz a manobra de desvio
  if (distancia <= DISTANCIA_SEGURA) {
    ligar_buzzer(); // Liga o buzzer
    parar(); // Para o robô
    delay(500); // Pausa para o próximo movimento

    // Move o robô para trás por um curto intervalo para ganhar espaço
    mover_tras(velocidadePWM);
    delay(1000);
    parar();

    // Calcula o ângulo de desvio necessário
    float anguloDesvio = calcularAnguloDesvio(distancia, DISTANCIA_SEGURA);

    // Gira o robô no ângulo calculado
    girar(anguloDesvio, velocidadePWM);

    // Retoma o movimento para frente
    mover_frente(velocidadePWM);

    // Desliga o buzzer após a manobra
    desligar_buzzer();
  } else {
    // Se não há obstáculo, continua em frente
    mover_frente(velocidadePWM);
  }

  // Pausa para a próxima leitura
  delay(PAUSA);
}

// Função para medir a distância com o sensor ultrassônico
long medirDistancia() {
  digitalWrite(TRIGGER_PIN, LOW);
  delayMicroseconds(2);
  digitalWrite(TRIGGER_PIN, HIGH);
  delayMicroseconds(10);
  digitalWrite(TRIGGER_PIN, LOW);

  long duracao = pulseIn(ECHO_PIN, HIGH);
  long distancia = duracao * 0.034 / 2;

  return distancia;
}

// Função para calcular o ângulo de desvio necessário
float calcularAnguloDesvio(long distancia, long distanciaMinima) {
  if (distancia > 0 && distancia <= distanciaMinima) {
    float angulo = (distanciaMinima - distancia) / distanciaMinima * 90;  // Máximo de 90 graus de desvio
    return angulo > 0 ? angulo : 90;  // Limite mínimo de 90 graus
  }
  return 0;  // Sem desvio se a distância é maior que a mínima
}

// Função para girar o robô no ângulo especificado usando PWM
void girar(float angulo, int velocidadePWM) {
  // Gira o robô para a direita pelo tempo proporcional ao ângulo desejado
  float tempoRotacao = angulo * 10;  // Ajuste o fator de multiplicação conforme necessário

  if (angulo > 0) {
    // Gira para a direita
    analogWrite(PIN_PWM_MOTOR_DIREITA, velocidadePWM);
    analogWrite(PIN_PWM_MOTOR_ESQUERDA, 0);
  } else {
    // Gira para a esquerda
    analogWrite(PIN_PWM_MOTOR_DIREITA, 0);
    analogWrite(PIN_PWM_MOTOR_ESQUERDA, velocidadePWM);
  }

  delay(tempoRotacao);
  parar();
}

// Função para mover o robô para a frente usando PWM
void mover_frente(int velocidadePWM) {
  analogWrite(PIN_PWM_MOTOR_DIREITA, velocidadePWM);
  analogWrite(PIN_PWM_MOTOR_ESQUERDA, velocidadePWM);

  digitalWrite(PIN_MOTOR_IN1, HIGH);
  digitalWrite(PIN_MOTOR_IN2, LOW);
}

// Função para mover o robô para trás usando PWM
void mover_tras(int velocidadePWM) {
  analogWrite(PIN_PWM_MOTOR_DIREITA, velocidadePWM);
  analogWrite(PIN_PWM_MOTOR_ESQUERDA, velocidadePWM);

  digitalWrite(PIN_MOTOR_IN1, LOW);
  digitalWrite(PIN_MOTOR_IN2, HIGH);
}

// Função para parar o robô
void parar(void) {
  analogWrite(PIN_PWM_MOTOR_DIREITA, 0);
  analogWrite(PIN_PWM_MOTOR_ESQUERDA, 0);

  digitalWrite(PIN_MOTOR_IN1, LOW);
  digitalWrite(PIN_MOTOR_IN2, LOW);
}

// Função para ligar o buzzer
void ligar_buzzer(void) {
  digitalWrite(PIN_BUZZER, HIGH);
}

// Função para desligar o buzzer
void desligar_buzzer(void) {
  digitalWrite(PIN_BUZZER, LOW);
}