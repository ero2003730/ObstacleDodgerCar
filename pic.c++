// Definição das máscaras para os dígitos no display de 7 segmentos
unsigned char ucMask[] = {0x3F, 0x06, 0x5B, 0x4F, 0x66, 0x6D, 0x7D, 0x07, 0x7F, 0x6F};

// Variáveis globais
unsigned int velocidade = 0; // Variável para armazenar a velocidade recebida
char comando;                // Variável para armazenar o comando recebido

void exibir_velocidade(unsigned int velocidade);
void inicializar_uart(void);
void configurar_pinos(void);
void ligar_buzzer_e_led(void);
void desligar_buzzer_e_led(void);

void main()
{
    configurar_pinos(); // Configura os pinos
    inicializar_uart(); // Inicializa a UART

    while (1)
    {
        if (UART1_Data_Ready())
        {                           // Verifica se há dados disponíveis na UART
            comando = UART1_Read(); // Lê o dado recebido

            if (comando >= '0' && comando <= '9')
            {
                // Se for um número, converte para um inteiro e armazena como velocidade
                velocidade = comando - '0';
                velocidade = velocidade * 10 + (UART1_Read() - '0'); // Lê o próximo dígito
                exibir_velocidade(velocidade);                       // Exibe a velocidade no display
            }
            else if (comando == 'L')
            {
                ligar_buzzer_e_led(); // Liga o buzzer e o LED
            }
            else if (comando == 'D')
            {
                desligar_buzzer_e_led(); // Desliga o buzzer e o LED
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

// Função para exibir a velocidade no display de 7 segmentos
void exibir_velocidade(unsigned int velocidade)
{
    unsigned int unidade, dezena;
    unidade = velocidade % 10;
    dezena = (velocidade / 10) % 10;

    // Mostra a dezena
    PORTD = ucMask[dezena];
    PORTA.RA3 = 1;
    Delay_ms(2);
    PORTA.RA3 = 0;

    // Mostra a unidade
    PORTD = ucMask[unidade];
    PORTA.RA4 = 1;
    Delay_ms(2);
    PORTA.RA4 = 0;
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