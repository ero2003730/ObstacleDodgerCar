// --- Ligações entre PIC e LCD ---
sbit LCD_RS at RE2_bit; // PINO 2 DO PORTD INTERLIGADO AO RS DO DISPLAY
sbit LCD_EN at RE1_bit; // PINO 3 DO PORTD INTERLIGADO AO EN DO DISPLAY
sbit LCD_D7 at RD7_bit; // PINO 7 DO PORTD INTERLIGADO AO D7 DO DISPLAY
sbit LCD_D6 at RD6_bit; // PINO 6 DO PORTD INTERLIGADO AO D6 DO DISPLAY
sbit LCD_D5 at RD5_bit; // PINO 5 DO PORTD INTERLIGADO AO D5 DO DISPLAY
sbit LCD_D4 at RD4_bit; // PINO 4 DO PORTD INTERLIGADO AO D4 DO DISPLAY

// Selecionando direção de fluxo de dados dos pinos utilizados para a comunicação com display LCD
sbit LCD_RS_Direction at TRISE2_bit; // SETA DIREÇÃO DO FLUXO DE DADOS DO PINO 2 DO PORTD
sbit LCD_EN_Direction at TRISE1_bit; // SETA DIREÇÃO DO FLUXO DE DADOS DO PINO 3 DO PORTD
sbit LCD_D7_Direction at TRISD7_bit; // SETA DIREÇÃO DO FLUXO DE DADOS DO PINO 7 DO PORTD
sbit LCD_D6_Direction at TRISD6_bit; // SETA DIREÇÃO DO FLUXO DE DADOS DO PINO 6 DO PORTD
sbit LCD_D5_Direction at TRISD5_bit; // SETA DIREÇÃO DO FLUXO DE DADOS DO PINO 5 DO PORTD
sbit LCD_D4_Direction at TRISD4_bit; // SETA DIREÇÃO DO FLUXO DE DADOS DO PINO 4 DO PORTD

// Variáveis globais
unsigned int velocidade = 0; // Variável para armazenar a velocidade recebida
char comando;                // Variável para armazenar o comando recebido

void exibir_velocidade_lcd(unsigned int velocidade);
void inicializar_uart(void);
void configurar_pinos(void);
void ligar_buzzer_e_led(void);
void desligar_buzzer_e_led(void);

void main()
{
    configurar_pinos();       // Configura os pinos
    inicializar_uart();       // Inicializa a UART
    Lcd_Init();               // Inicializa o LCD
    Lcd_Cmd(_LCD_CLEAR);      // Limpa o display
    Lcd_Cmd(_LCD_CURSOR_OFF); // Desliga o cursor

    while (1)
    {
        if (UART1_Data_Ready())
        {
            comando = UART1_Read(); // Lê o dado recebido

            // Verifica se o comando recebido é 'L' ou 'D'
            if (comando == 'L')
            {
                ligar_buzzer_e_led(); // Liga o buzzer e o LED
            }
            else if (comando == 'D')
            {
                desligar_buzzer_e_led(); // Desliga o buzzer e o LED
            }
            else
            {
                // Se não for 'L' ou 'D', trata o comando como sendo a velocidade
                velocidade = comando;              // Recebe diretamente o valor da velocidade
                exibir_velocidade_lcd(velocidade); // Exibe a velocidade no LCD
            }
        }
    }
}

// Função para configurar os pinos do PIC
void configurar_pinos(void)
{
    ADCON1 = 0x0F; // Configura todos os pinos como digitais
    TRISA = 0;     // Define PORTA como saída
    PORTA = 0;     // Reseta todos os pinos do PORTA
    TRISD = 0;     // Define PORTD como saída
    PORTD = 0;     // Reseta todos os pinos do PORTD
    TRISC.RC1 = 0; // Define RC1 como saída para o buzzer
    TRISB.RB7 = 0; // Define RB7 como saída para o LED
}

// Função para inicializar a UART
void inicializar_uart(void)
{
    UART1_Init(9600); // Inicializa a UART com 9600 bps
    Delay_ms(100);    // Aguarda a estabilização
}

// Função para exibir a velocidade no display LCD
void exibir_velocidade_lcd(unsigned int velocidade)
{
    char texto[7];                 // Buffer para armazenar o texto da velocidade
    IntToStr(velocidade, texto);   // Converte o número de velocidade para string
    Lcd_Out(1, 1, "Velocidade: "); // Exibe a mensagem na linha 1 do LCD
    Lcd_Out(2, 1, texto);          // Exibe a velocidade na linha 2 do LCD
}

// Função para ligar o buzzer e o LED
void ligar_buzzer_e_led(void)
{
    PORTC.RC1 = 1; // Liga o buzzer
    PORTB.RB7 = 1; // Liga o LED
}

// Função para desligar o buzzer e o LED
void desligar_buzzer_e_led(void)
{
    PORTC.RC1 = 0; // Desliga o buzzer
    PORTB.RB7 = 0; // Desliga o LED
}