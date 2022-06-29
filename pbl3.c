/* CODE TO COMPILE
    sudo gcc -o pbl3 pbl3.c pbl3-dev.c -lwiringPi -lwiringPiDev -lpthread -lmosquitto
    sudo ./pbl3
*/

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <wiringPi.h>
#include <lcd.h>
#include <unistd.h>
#include <linux/i2c-dev.h>
#include <linux/ioctl.h>
#include <fcntl.h>
#include <string.h>
#include <time.h>
#include <ads1115_rpi.h>
#include <stdint.h>
#include <errno.h>
#include <pthread.h>
#include <mosquitto.h>
#include <locale.h>
//#include <curses.h>

// Pinagem LCD
#define LCD_RS 6
#define LCD_E 31
#define LCD_D4 26
#define LCD_D5 27
#define LCD_D6 28
#define LCD_D7 29

// Pinagem DHT11
#define MAXTIMINGS 85
#define DHTPIN 4

// Pinagem Botões
#define botao1 21
#define botao2 24
#define botao3 25

// Pinagem Chaves
#define chave1 7
#define chave2 0
#define chave3 2
#define chave4 3

#define MAX_LENGTH 100
#define NUM_STRINGS 10

#define CONCAT 16

#define MAX 150
#define DADOS 20

char historico[DADOS][MAX] = {""};

int count = 0;

int zerar_historico = 0;

char luminosidade[16];
char pressao[16];
float temp;
float temp2;
int lcd;
int dht11_dat[5] = {0, 0, 0, 0, 0};

int tempo_cliente = 2000;

void tempo(int contador)
{
    if (contador == 0)
    {
        contador = 2000;
        delay(contador);
    }
    else
    {
        delay(contador);
    }
}

// Funcionamento do SUB

void on_connect(struct mosquitto *mosq, void *obj, int rc)
{
    printf("Cliente: %d\n", *(int *)obj);
    if (rc)
    {
        printf("Erro: %d\n", rc);
        exit(-1);
    }
    mosquitto_subscribe(mosq, NULL, "TP02-G01/tempo", 0);
}

void on_message(struct mosquitto *mosq, void *obj, const struct mosquitto_message *msg)
{
    time_t t = time(NULL);
    struct tm tm = *localtime(&t);
    char *recebido = msg->payload;
    int convertido = atoi(recebido);
    tempo_cliente = convertido * 1000;
    if (tempo_cliente != 0 && tempo_cliente > 2)
    {
        tempo(tempo_cliente);
        printf("Tempo alterado em %s para %d segundos - %d-%02d-%02d %02d:%02d:%02d\n", msg->topic, convertido, tm.tm_mday, tm.tm_mon + 1, tm.tm_year + 1900, tm.tm_hour, tm.tm_min, tm.tm_sec);
    }
}

void *sub(void *args)
{
    int rc, id = 12;

    mosquitto_lib_init();

    struct mosquitto *mosq;

    mosq = mosquitto_new("TP02-G01/tempo", true, &id);
    mosquitto_username_pw_set(mosq, "aluno", "aluno*123");
    mosquitto_connect_callback_set(mosq, on_connect);
    mosquitto_message_callback_set(mosq, on_message);

    rc = mosquitto_connect(mosq, "10.0.0.101", 1883, 60);
    if (rc)
    {
        printf("Cliente não conectado ao broker! Error: %d\n", rc);
    }

    mosquitto_loop_start(mosq);
    printf("Pressione Enter para sair...\n");
    getchar();
    mosquitto_loop_stop(mosq, true);

    mosquitto_disconnect(mosq);
    mosquitto_destroy(mosq);
    mosquitto_lib_cleanup();

    return NULL;
}

//////////////////////////////////////////////////

void pub(char *envio)
{
    int rc;
    struct mosquitto *mosq;
    mosquitto_lib_init();
    mosq = mosquitto_new("publisher-test", true, NULL);
    mosquitto_username_pw_set(mosq, "aluno", "aluno*123");
    rc = mosquitto_connect(mosq, "10.0.0.101", 1883, 60);
    if (rc != 0)
    {
        printf("Cliente não conectado ao broker! Error: %d\n", rc);
        mosquitto_destroy(mosq);
    }
    mosquitto_publish(mosq, NULL, "TP02-G01", strlen(envio), envio, 0, false);
    // printf("## Envio realizado com sucesso ao broker ##\n");
    mosquitto_disconnect(mosq);
    mosquitto_destroy(mosq);
    mosquitto_lib_cleanup();
}

void gravar_historico(char *v1, char *v2)
{
    FILE *arq;
    int result;

    arq = fopen("historico.txt", "at"); // Cria um arquivo texto para grava��o
    if (arq == NULL)                    // Se n�o conseguiu criar
    {
        printf("Problemas na CRIACAO do arquivo\n");
        return;
    }

    result = fprintf(arq, v1);
    result = fprintf(arq, " ");
    result = fprintf(arq, v2);
    result = fprintf(arq, "\n");

    if (result == EOF)
        printf("Erro na Gravacao\n");

    fclose(arq);
}

void concatenar_umidade(int umidade_v1, int umidade_v2)
{
    // Vari�veis para umidade
    char *text_umidade = "Umi: ";
    char umi[MAX];
    char umi_1[MAX];
    char umi_2[MAX];
    char umi_valor[MAX];
    char umi_final[MAX];

    sprintf(umi_1, "%d", umidade_v1); // int to string
    sprintf(umi_2, "%d", umidade_v2);

    strcat(strcpy(umi, umi_1), ".");
    strcat(strcpy(umi_valor, umi), umi_2);
    strcat(strcpy(umi_final, text_umidade), umi_valor);

    gravar_historico(text_umidade, umi_valor);
}

void concatenar_temperatura(int temperatura)
{

    // Variaveis para temperatura
    char *text_temp = "Temp: ";
    char temp_valor[MAX];
    char temp_final[MAX];

    sprintf(temp_valor, "%d", temperatura); // int to string

    strcat(strcpy(temp_final, text_temp), temp_valor);

    gravar_historico(text_temp, temp_final);
}

void concatenar_luminosidade(float luminosidade)
{

    // Vari�veis para luminosidade
    char *text_luminosidade = "Lumi: ";
    char lum_valor[MAX];
    char lum_final[MAX];

    sprintf(lum_valor, "%.2f", luminosidade);

    strcat(strcpy(lum_final, text_luminosidade), lum_valor);

    gravar_historico(text_luminosidade, lum_valor);
}

void concatenar_pressao(float pressao)
{

    // Vari�veis para press�o
    char *text_pressao = "Press: ";
    char press_valor[MAX];
    char press_final[MAX];

    sprintf(press_valor, "%.2f", pressao);

    strcat(strcpy(press_final, text_pressao), press_valor);

    gravar_historico(text_pressao, press_valor);
}

void ler_historico()
{
    FILE *textfile;
    char *text;
    char *result;
    long numbytes;

    if (digitalRead(botao1) == LOW)
    {
        delay(500);

        textfile = fopen("historico.txt", "rt");
        if (textfile == NULL)
            printf("Erro ao abrir histórico");

        fseek(textfile, 0L, SEEK_END);
        numbytes = ftell(textfile);
        fseek(textfile, 0L, SEEK_SET);

        text = (char *)calloc(numbytes, sizeof(char));
        if (text == NULL)
            printf("Histórico vazio");

        fread(text, sizeof(char), numbytes, textfile);

        pub(text);
        fclose(textfile);
    }
    textfile = fopen("historico.txt", "w");
    fclose(textfile);
}

void sensor()
{
    pinMode(chave1, INPUT);
    pinMode(chave2, INPUT);
    pinMode(chave3, INPUT);
    pinMode(chave4, INPUT);
    time_t t = time(NULL);
    struct tm tm = *localtime(&t);
    uint8_t laststate = HIGH;
    uint8_t counter = 0;
    uint8_t j = 0, i;
    float f;
    char array1[10];
    char array2[10];

    char buffer[CONCAT];
    char buffer_hora[CONCAT];
    char buffer_umi[CONCAT];
    char str[5];

    char *text_umidade = "Umidade: ";
    char umi[MAX];
    char umi_1[MAX];
    char umi_2[MAX];
    char umi_valor[MAX];
    char umi_final[MAX];

    char *text_temperatura = "Temperatura: ";
    char temp_valor[MAX];
    char temp_final[MAX];

    if (openI2CBus("/dev/i2c-1") == -1)
    {
        printf("Sensor não está sendo lido!\n");
    }

    setI2CSlave(0x48);

    temp = readVoltage(0);
    temp2 = readVoltage(1);

    dht11_dat[0] = dht11_dat[1] = dht11_dat[2] = dht11_dat[3] = dht11_dat[4] = 0;

    pinMode(DHTPIN, OUTPUT);
    digitalWrite(DHTPIN, LOW);
    delay(18);

    digitalWrite(DHTPIN, HIGH);
    delayMicroseconds(40);

    pinMode(DHTPIN, INPUT);

    for (i = 0; i < MAXTIMINGS; i++)
    {
        counter = 0;
        while (digitalRead(DHTPIN) == laststate)
        {
            counter++;
            delayMicroseconds(1);
            if (counter == 255)
            {
                break;
            }
        }
        laststate = digitalRead(DHTPIN);

        if (counter == 255)
            break;

        if ((i >= 4) && (i % 2 == 0))
        {
            dht11_dat[j / 8] <<= 1;
            if (counter > 16)
                dht11_dat[j / 8] |= 1;
            j++;
        }
    }

    if ((j >= 40) && (dht11_dat[4] == ((dht11_dat[0] + dht11_dat[1] + dht11_dat[2] + dht11_dat[3]) & 0xFF)))
    {
        f = dht11_dat[2] * 9. / 5. + 32;

        FILE *arq;
        int result;

        time_t current_time;
        struct tm *time_info;
        char hora[8];

        time(&current_time);
        time_info = localtime(&current_time);

        strftime(hora, 8, "%H:%M", time_info);

        arq = fopen("historico.txt", "at"); // Cria um arquivo texto para grava��o
        if (arq == NULL)                    // Se n�o conseguiu criar
        {
            printf("Problemas na CRIACAO do arquivo\n");
            return;
        }

        result = fprintf(arq, "\n");
        result = fprintf(arq, hora);
        result = fprintf(arq, "\n\n");
        fclose(arq);

        concatenar_umidade(dht11_dat[0], dht11_dat[1]);
        concatenar_temperatura(dht11_dat[2]);
        concatenar_luminosidade(temp);
        concatenar_pressao(temp2);

        if (digitalRead(chave1) == LOW && digitalRead(chave2) == HIGH && digitalRead(chave3) == HIGH && digitalRead(chave4) == HIGH)
        {
            char int_1[5];
            char int_2[5];
            lcdPosition(lcd, 0, 0);
            lcdPrintf(lcd, "Umi: %d.%d %%", dht11_dat[0], dht11_dat[1]);
            printf("Umidade: %d.%d %% | %d-%02d-%02d %02d:%02d:%02d\n", dht11_dat[0], dht11_dat[1], tm.tm_mday, tm.tm_mon + 1, tm.tm_year + 1900, tm.tm_hour, tm.tm_min, tm.tm_sec);

            sprintf(umi_1, "%d", dht11_dat[0]); // int to string
            sprintf(umi_2, "%d", dht11_dat[1]);

            strcat(strcpy(umi, umi_1), ".");
            strcat(strcpy(umi_valor, umi), umi_2);
            strcat(strcpy(umi_final, text_umidade), umi_valor);
            pub(umi_final); // ENVIANDO PARA O CLIENTE
        }
        else if (digitalRead(chave1) == HIGH && digitalRead(chave2) == LOW && digitalRead(chave3) == HIGH && digitalRead(chave4) == HIGH)
        {
            lcdPosition(lcd, 0, 1);
            lcdPrintf(lcd, "Temp: %d.0 C", dht11_dat[2]);
            printf("Temperatura: %d.0 C | %d-%02d-%02d %02d:%02d:%02d\n", dht11_dat[2], tm.tm_mday, tm.tm_mon + 1, tm.tm_year + 1900, tm.tm_hour, tm.tm_min, tm.tm_sec);

            sprintf(temp_valor, "%d", dht11_dat[2]); // int to string

            strcat(strcpy(temp_final, text_temperatura), temp_valor);
            pub(temp_final); // ENVIANDO PARA O CLIENTE
        }
        else if (digitalRead(chave1) == LOW && digitalRead(chave2) == LOW && digitalRead(chave3) == HIGH && digitalRead(chave4) == HIGH)
        {
            lcdPosition(lcd, 0, 0);
            lcdPrintf(lcd, "Umi: %d.%d %%", dht11_dat[0], dht11_dat[1]);
            printf("Umidade: %d.%d %% | %d-%02d-%02d %02d:%02d:%02d\n", dht11_dat[0], dht11_dat[1], tm.tm_mday, tm.tm_mon + 1, tm.tm_year + 1900, tm.tm_hour, tm.tm_min, tm.tm_sec);

            sprintf(umi_1, "%d", dht11_dat[0]); // int to string
            sprintf(umi_2, "%d", dht11_dat[1]);

            strcat(strcpy(umi, umi_1), ".");
            strcat(strcpy(umi_valor, umi), umi_2);
            strcat(strcpy(umi_final, text_umidade), umi_valor);
            pub(umi_final); // ENVIANDO PARA O CLIENTE

            lcdPosition(lcd, 0, 1);
            lcdPrintf(lcd, "Temp: %d.0 C", dht11_dat[2]);
            printf("Temperatura: %d.0 C | %d-%02d-%02d %02d:%02d:%02d\n", dht11_dat[2], tm.tm_mday, tm.tm_mon + 1, tm.tm_year + 1900, tm.tm_hour, tm.tm_min, tm.tm_sec);

            sprintf(temp_valor, "%d", dht11_dat[2]); // int to string

            strcat(strcpy(temp_final, text_temperatura), temp_valor);
            pub(temp_final); // ENVIANDO PARA O CLIENTE
        }
        else if (digitalRead(chave1) == HIGH && digitalRead(chave2) == HIGH && digitalRead(chave3) == LOW && digitalRead(chave4) == HIGH)
        {
            sprintf(array1, "%.2f", temp);
            strcat(strcpy(luminosidade, "Lumi: "), array1);
            printf("Luminosidade: %.2f | %d-%02d-%02d %02d:%02d:%02d\n", readVoltage(0), tm.tm_mday, tm.tm_mon + 1, tm.tm_year + 1900, tm.tm_hour, tm.tm_min, tm.tm_sec);
            lcdPosition(lcd, 0, 0);
            lcdPuts(lcd, luminosidade); // Escrevendo no display
            pub(luminosidade);          // ENVIANDO PARA O CLIENTE
        }
        else if (digitalRead(chave1) == HIGH && digitalRead(chave2) == HIGH && digitalRead(chave3) == HIGH && digitalRead(chave4) == LOW)
        {
            sprintf(array2, "%.2f", temp2);
            strcat(strcpy(pressao, "Press: "), array2);
            printf("Pressão: %.2f | %d-%02d-%02d %02d:%02d:%02d\n", readVoltage(1), tm.tm_mday, tm.tm_mon + 1, tm.tm_year + 1900, tm.tm_hour, tm.tm_min, tm.tm_sec);
            lcdPosition(lcd, 0, 1);
            lcdPuts(lcd, pressao); // Escrevendo no display

            pub(pressao); // ENVIANDO PARA O CLIENTE
        }
        else if (digitalRead(chave1) == HIGH && digitalRead(chave2) == HIGH && digitalRead(chave3) == LOW && digitalRead(chave4) == LOW)
        {
            sprintf(array1, "%.2f", temp);
            strcat(strcpy(luminosidade, "Lum: "), array1);
            printf("Luminosidade: %.2f | %d-%02d-%02d %02d:%02d:%02d\n", readVoltage(0), tm.tm_mday, tm.tm_mon + 1, tm.tm_year + 1900, tm.tm_hour, tm.tm_min, tm.tm_sec);
            lcdPosition(lcd, 0, 0);
            lcdPuts(lcd, luminosidade); // Escrevendo no display
            pub(luminosidade);          // ENVIANDO PARA O CLIENTE

            sprintf(array2, "%.2f", temp2);
            strcat(strcpy(pressao, "Pre: "), array2);
            printf("Pressão: %.2f | %d-%02d-%02d %02d:%02d:%02d\n", readVoltage(1), tm.tm_mday, tm.tm_mon + 1, tm.tm_year + 1900, tm.tm_hour, tm.tm_min, tm.tm_sec);
            lcdPosition(lcd, 0, 1);
            lcdPuts(lcd, pressao); // Escrevendo no display
            pub(pressao);          // ENVIANDO PARA O CLIENTE
        }
        else
        {
            lcdClear(lcd);
            lcdPosition(lcd, 4, 0);
            lcdPuts(lcd, "PBL 3");
            lcdPosition(lcd, 1, 1);
            lcdPuts(lcd, "Sist. Digitais");
        }
        tempo(tempo_cliente);
        lcdClear(lcd);
    }
    zerar_historico++;
    if (zerar_historico == 19)
    {
        FILE *arq;
        arq = fopen("historico.txt", "w");
        fclose(arq);
    }
}

void addTempo()
{
    if (digitalRead(botao2) == LOW)
    {
        delay(500);
        tempo_cliente = tempo_cliente + 1000;
        int temp = tempo_cliente / 1000;
        printf("#### Tempo de medição atualizado para: %d segundos\n", temp);
    }
}

void remTempo()
{
    if (digitalRead(botao3) == LOW)
    {
        delay(500);
        if (tempo_cliente < 2000)
        {
            printf("#### Tempo de medição não pode ser menor que dois segundos ####\n");
            tempo_cliente = 2000;
            printf("#### Tempo de medição atualizado para: %d segundos\n", temp);
        }
        else
        {
            tempo_cliente = tempo_cliente - 1000;
            int temp = tempo_cliente / 1000;
            printf("#### Tempo de medição atualizado para: %d segundos\n", temp);
        }
    }
}

void *interrupcao()
{
    wiringPiISR(botao1, INT_EDGE_BOTH, ler_historico);
    wiringPiISR(botao2, INT_EDGE_BOTH, addTempo);
    wiringPiISR(botao3, INT_EDGE_BOTH, remTempo);
}

void main(void)
{ // Função principal do sistema
    setlocale(LC_ALL, "Portuguese_Brasil");
    wiringPiSetup();
    lcd = lcdInit(2, 16, 4, LCD_RS, LCD_E, LCD_D4, LCD_D5, LCD_D6, LCD_D7, 0, 0, 0, 0);

    pinMode(botao1, INPUT);
    pinMode(botao2, INPUT);
    pinMode(botao3, INPUT);

    lcdPosition(lcd, 4, 0);
    lcdPuts(lcd, "PBL 3");
    lcdPosition(lcd, 1, 1);
    lcdPuts(lcd, "Sist. Digitais");
    delay(2000);
    lcdClear(lcd);

    pthread_t interrupt;
    pthread_create(&interrupt, NULL, interrupcao, NULL);

    pthread_t sub_t;
    pthread_create(&sub_t, NULL, sub, NULL);

    while (1)
    {
        sensor();
    }
}