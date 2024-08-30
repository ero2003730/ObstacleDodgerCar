// Definição dos pinos do sensor ultrassônico
const int trigPin = 12;    // Pino TRIG do sensor
const int echoPin = 13;    // Pino ECHO do sensor

// Definição dos pinos do driver de motor
const int IN1 = 7;  // Pino IN1
const int IN2 = 6;  // Pino IN2
const int IN3 = 5;  // Pino IN3
const int IN4 = 4;  // Pino IN4
const int ENA = 9;  // Pino ENA para PWM do motor A
const int ENB = 10; // Pino ENB para PWM do motor B

// Definição das constantes auxiliares para controlar os motores
const int DISTANCIA_SEGURA = 50; // [cm]
const int PAUSA = 100; // [ms]

// Funções auxiliares para controle
int ler_distancia(void);
void mover_frente(void);
void mover_tras(void);
void parar(void);
void virar_esquerda(void);

void setup() {
  // Configura os pinos do sensor ultrassônico
  pinMode(trigPin, OUTPUT);
  pinMode(echoPin, INPUT);
  digitalWrite(trigPin, LOW); // Por padrão, TRIG em LOW

  // Configura os pinos do driver de motor
  pinMode(IN1, OUTPUT);
  pinMode(IN2, OUTPUT);
  pinMode(IN3, OUTPUT);
  pinMode(IN4, OUTPUT);

  // Configura os pinos de PWM como saída
  pinMode(ENA, OUTPUT);
  pinMode(ENB, OUTPUT);

  parar(); // Robô parado por padrão
}

void loop() {
  // Lê a distância
  int distancia = ler_distancia();

  // Verifica se há um obstáculo na frente
  if(distancia < DISTANCIA_SEGURA){
    // Para o robô
    parar();
    delay(500); // Pausa para o próximo movimento

    // Move o robô para trás
    mover_tras();

    // Para o robô após um curto intervalo
    delay(1000);
    parar();

    // Sempre vira para a esquerda
    virar_esquerda();

    // Para o robô após um curto intervalo
    delay(500);
    parar();
  } else {
    // Move o robô para a frente
    mover_frente();
  }

  // Pausa para a próxima leitura
  delay(PAUSA);
}

// Ler a distância com o sensor ultrassônico
int ler_distancia(void){
  // Realiza o pulso de 10 microsegundos no trigger do sensor
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);

  // Mede o pulso em microsegundos retornado para o echo do sensor
  // e converte o tempo para distância ao dividir por 58
  return pulseIn(echoPin, HIGH) / 58; // [cm]
}

// Mover o robô para a frente
void mover_frente(void){
  analogWrite(ENA, 220);
  analogWrite(ENB, 220);

  digitalWrite(IN1, LOW);
  digitalWrite(IN2, HIGH);
  digitalWrite(IN3, LOW);
  digitalWrite(IN4, HIGH);
}

// Mover o robô para trás
void mover_tras(void){
  analogWrite(ENA, 150);
  analogWrite(ENB, 150);

  digitalWrite(IN1, HIGH);
  digitalWrite(IN2, LOW);
  digitalWrite(IN3, HIGH);
  digitalWrite(IN4, LOW);
}

// Virar o robô para a esquerda
void virar_esquerda(void){
  analogWrite(ENA, 255);
  analogWrite(ENB, 255);

  digitalWrite(IN1, LOW);
  digitalWrite(IN2, HIGH);
  digitalWrite(IN3, HIGH);
  digitalWrite(IN4, HIGH);
}

// Parar o robô
void parar(void){
  analogWrite(ENA, 0);
  analogWrite(ENB, 0);

  digitalWrite(IN1, LOW);
  digitalWrite(IN2, LOW);
  digitalWrite(IN3, LOW);
  digitalWrite(IN4, LOW);
}