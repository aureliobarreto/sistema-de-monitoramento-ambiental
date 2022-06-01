/* CODE TO COMPILE
    gcc -o pbl3 pbl3.c -lwiringPi -lwiringPiDev
    sudo ./pbl3
*/


#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <wiringPi.h>
#include <lcd.h>
#include <unistd.h>
#include <linux/i2c-dev.h>
#include <fcntl.h>
#include <linux/ioctl.h>
#include <string.h>


#include <ads1115_rpi.h>

//Pinagem das chaves
#define chave_0 15
#define chave_1 17
#define chave_2 27
#define chave_3 22

//Pinagem dos bottões
//#define botao_0 3
//#define botao_1 35
//#define botao_2 37

const int botao_1 = 35;
const int botao_2 = 37;

//Pino do LED
#define LED 13

//Pinagem LCD
#define LCD_RS  6              
#define LCD_E   31               
#define LCD_D4  26               
#define LCD_D5  27               
#define LCD_D6  28               
#define LCD_D7  29 

//Pinagem DHT11
#define TEMPO 85
#define DHT	16

char pbl3[16] = "## PBL 3 ##";


void dht11(){ // Função para leitura de temperatura e humidade do sensor DHT11
    int data[5] = {0, 0, 0, 0, 0};

    uint8_t laststate	= HIGH;
	uint8_t counter		= 0;
	uint8_t j			= 0, i;
	data[0] = data[1] = data[2] = data[3] = data[4] = 0;

    pinMode(DHT, OUTPUT);
	digitalWrite( DHT, LOW );
	delay( 18 );

    pinMode(DHT, INPUT);

    for (i = 0; i < TEMPO; i++){
        counter = 0;
        while (digitalRead(DHT) == laststate){
			counter++;
			delayMicroseconds(1);
			if (counter == 255){
				break;
			}
		}
		laststate = digitalRead(DHT);
        if ( counter == 255 ){
		    break;
        }
        if ( (i >= 4) && (i % 2 == 0) ){
			data[j / 8] <<= 1;
			if ( counter > 16 )
				data[j/8] |= 1;
			j++;
		}
    }
    if ( (j >= 40) && (data[4] == ((data[0] + data[1] + data[2] + data[3]) & 0xFF))){
		float h = (float)((data[0] << 8) + data[1]) / 10;
		if ( h > 100 ){
			h = data[0];
		}
		float c = (float)(((data[2] & 0x7F) << 8) + data[3]) / 10;
		if ( c > 125 ){
			c = data[2];
		}
		if ( data[2] & 0x80 ){
			c = -c;
		}
		float f = c * 1.8f + 32;
		printf( "Umidade = %.1f %% Temperatura = %.1f *C (%.1f *F)\n", h, c, f );
	}else{
		printf( "Sensor com problemas!\n" );
	}
}

// ############################################################################################################################

int i2cFile;
unsigned char writeBuf[3] = {0};

int openI2CBus(char *bus)
{
	if ((i2cFile = open(bus, O_RDWR)) < 0)
	{
		printf("Sensor com problemas. \n");
		return -1;
	} else {
		printf("\n");
		return 1;
	}
}

int setI2CSlave(unsigned char deviceAddr)
{
	if(ioctl(i2cFile, I2C_SLAVE, deviceAddr) < 0)
	{
		printf("Erro ao receber dados do endereço: 0x%x. \n", deviceAddr);
		return -1;
	} else {
		return 1;
	}

}

float readVoltage(int channel)
{
	unsigned char readBuf[2] = {0};
	unsigned int analogVal;
	float voltage;
	unsigned int config = 0;

	config = 	CONFIG_REG_OS_SINGLE		|
				CONFIG_REG_PGA_4_096V 		|
				CONFIG_REG_MODE_SINGLE 		|
				CONFIG_REG_DR_128SPS 			|
				CONFIG_REG_CMODE_TRAD 		|
				CONFIG_REG_CPOL_ACTIV_LOW 	|
				CONFIG_REG_CLATCH_NONLATCH 	|
				CONFIG_REG_CQUE_NONE;

	void configDevice(unsigned int config)
	{
		writeBuf[0] = 0x01;
		writeBuf[1] = config >> 8;
		writeBuf[2] = config && 0xFF;
		write(i2cFile, writeBuf, 3);
		usleep(25);
	}

	switch (channel) {
		case 0:
			config |= CONFIG_REG_MUX_CHAN_0;
			break;
		case 1:
			config |= CONFIG_REG_MUX_CHAN_1;
			break;
		case 2:
			config |= CONFIG_REG_MUX_CHAN_2;
			break;
		case 3:
			config |= CONFIG_REG_MUX_CHAN_3;
			break;
		default:
			printf("canais de leitura 0-3\n");
	}
	configDevice(config);
	usleep(135000);

	writeBuf[0] = 0x00;
	write(i2cFile, writeBuf, 1);

	if(read(i2cFile, readBuf, 2) != 2) // read data and check error
	{
		printf("Error : Input/Output Error \n");
	}
	else
	{
		analogVal = readBuf[0] << 8 | readBuf[1];
		voltage = (float)analogVal*4.096/32767.0;
	}

	return voltage;
}

void lcd(char data[16]){ // Função para impressão de dados no LCD
    /* 
    Pinagem utilizada nas placas no LEDs
    */                           

    int lcd;
    lcd = lcdInit (2, 16, 4, LCD_RS, LCD_E, LCD_D4, LCD_D5, LCD_D6, LCD_D7, 0, 0, 0, 0);

    lcdPuts(lcd, data); // Escrevendo no display
}

// ############################################################################################################################

char luminosidade[16];
float temp;

void ldr(){ // Função para leitura de luminosidade do sensor LDR
    if(openI2CBus("/dev/i2c-1") == -1){
		printf("LDR não está sendo lido!\n");
	}
	setI2CSlave(0x48);
	printf("Luminosidade: %.2f\n", readVoltage(0));
    printf("Pressão: %.2f\n", readVoltage(1));
    temp = readVoltage(0);
    char array[10];
    sprintf(array, "%.2f", temp);
    strcat(strcpy(luminosidade, "Lum: "), array);
    lcd(luminosidade);
}

char pressao[16];
float temp2;

void bmp(){ // Função para leitura de pressão atmosférica do sensor BMP180 ou BMP085
    if(openI2CBus("/dev/i2c-1") == -1){
		printf("BPM não está sendo lido!\n");
	}
    setI2CSlave(0x48);
	printf("Pressão: %.2f\n", readVoltage(2));
    temp2 = readVoltage(2);
    char array[10];
    sprintf(array, "%.2f", temp2);
    strcat(strcpy(pressao, "Press: "), array);
    lcd(pressao);
}



void mqtt(){ // Função para envio de informações via protocolo MQTT

}

int main(){ // Função principal do sistema
    wiringPiSetup();

    pinMode(botao_1, INPUT);
    pinMode(botao_2, INPUT);

    while (1)
    {
        //dht11();
        /*
        if (digitalRead(botao_1) == HIGH)
        {
            ldr();
            delay(2000);
        }else if (digitalRead(botao_2) == HIGH)
        {
            bmp();
            delay(2000);
        }else{
            lcd(pbl3);
        }     
        */
        ldr();
        delay(2000);
        bmp();
        delay(2000);  
        system("clear");
    }
    return 0;
}
