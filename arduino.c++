// Definição dos pinos do sensor ultrassônico
const int trigPin = 12; // Pino TRIG do sensor
const int echoPin = 13; // Pino ECHO do sensor

// Definição dos pinos do driver de motor
const int IN1 = 7;  // Pino IN1
const int IN2 = 6;  // Pino IN2
const int IN3 = 5;  // Pino IN3
const int IN4 = 4;  // Pino IN4
const int ENA = 9;  // Pino ENA para PWM do motor A
const int ENB = 10; // Pino ENB para PWM do motor B

// Definição do pino do potenciômetro
const int potPin = A0; // Pino do potenciômetro

// Definição das constantes auxiliares para controlar os motores
const int DISTANCIA_SEGURA = 50; // [cm]
const int PAUSA = 100;           // [ms]

// Variáveis para controlar os tempos
unsigned long tempo_anterior = 0;
unsigned long tempo_parada = 0;
unsigned long tempo_reverso = 0;
unsigned long tempo_virada = 0;
bool em_reverso = false;
bool virando = false;
volatile int velocidade = 150; // Velocidade inicial

int ler_distancia(void);
void mover_frente(void);
void mover_tras(void);
void parar(void);
void virar_esquerda(void);

void setup()
{
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

  // Configuração do Timer1 para gerar interrupção a cada 1 segundo
  noInterrupts();
  TCCR1A = 0;
  TCCR1B = 0;
  TCNT1 = 0;
  OCR1A = 31249;                       
  TCCR1B |= (1 << WGM12);              // Modo CTC (Clear Timer on Compare Match)
  TCCR1B |= (1 << CS12) | (1 << CS10); // Prescaler 1024
  TIMSK1 |= (1 << OCIE1A);             // Habilita interrupção por comparação A
  interrupts();
}

void loop()
{
  unsigned long tempo_atual = millis();

  // Lê a distância
  int distancia = ler_distancia();

  if (distancia < DISTANCIA_SEGURA && !em_reverso && !virando)
  {
    parar();
    tempo_parada = tempo_atual;
    em_reverso = true;
  }

  if (em_reverso)
  {
    if (tempo_atual - tempo_parada >= 500 && tempo_atual - tempo_parada < 1500)
    {
      mover_tras();
    }
    else if (tempo_atual - tempo_parada >= 1500)
    {
      parar();
      tempo_reverso = tempo_atual;
      em_reverso = false;
      virando = true;
    }
  }

  if (virando)
  {
    if (tempo_atual - tempo_reverso >= 500 && tempo_atual - tempo_reverso < 1000)
    {
      virar_esquerda();
    }
    else if (tempo_atual - tempo_reverso >= 1000)
    {
      parar();
      virando = false;
    }
  }

  if (!em_reverso && !virando && tempo_atual - tempo_anterior >= PAUSA)
  {
    mover_frente();
    tempo_anterior = tempo_atual;
  }
}

// Manipulador da interrupção
ISR(TIMER1_COMPA_vect)
{
  int potValue = analogRead(potPin);             // Lê o valor do potenciômetro (0 a 1023)
  velocidade = map(potValue, 0, 1023, 150, 220); // Mapeia para o range de 150 a 220
}

// Ler a distância com o sensor ultrassônico
int ler_distancia(void)
{
  digitalWrite(trigPin, LOW);
  delayMicroseconds(2);
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);

  long duracao = pulseIn(echoPin, HIGH);
  int distancia = duracao * 0.034 / 2;

  return distancia;
}

// Mover o robô para a frente
void mover_frente(void)
{
  analogWrite(ENA, velocidade);
  analogWrite(ENB, velocidade);

  digitalWrite(IN1, LOW);
  digitalWrite(IN2, HIGH);
  digitalWrite(IN3, LOW);
  digitalWrite(IN4, HIGH);
}

// Mover o robô para trás
void mover_tras(void)
{
  analogWrite(ENA, 150);
  analogWrite(ENB, 150);

  digitalWrite(IN1, HIGH);
  digitalWrite(IN2, LOW);
  digitalWrite(IN3, HIGH);
  digitalWrite(IN4, LOW);
}

// Virar o robô para a esquerda
void virar_esquerda(void)
{
  analogWrite(ENA, 255);
  analogWrite(ENB, 255);

  digitalWrite(IN1, LOW);
  digitalWrite(IN2, HIGH);
  digitalWrite(IN3, HIGH);
  digitalWrite(IN4, HIGH);
}

// Parar o robô
void parar(void)
{
  analogWrite(ENA, 0);
  analogWrite(ENB, 0);

  digitalWrite(IN1, LOW);
  digitalWrite(IN2, LOW);
  digitalWrite(IN3, LOW);
  digitalWrite(IN4, LOW);
}