/* CODE TO COMPILE
    sudo gcc -o pbl3 pbl3.c pbl3-dev.c -lwiringPi -lwiringPiDev
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

//Pinagem LCD
#define LCD_RS  6              
#define LCD_E   31               
#define LCD_D4  26               
#define LCD_D5  27               
#define LCD_D6  28               
#define LCD_D7  29 
#define MAXTIMINGS 85
#define DHTPIN 4

char historico[10];


char luminosidade[16];
char pressao[16];
float temp;
float temp2;
int lcd;
int dht11_dat[5] = {0, 0, 0, 0, 0};

void ldr_bpm(){ // Função para leitura de luminosidade do sensor LDR
	time_t t = time(NULL);
  	struct tm tm = *localtime(&t);
	lcd = lcdInit (2, 16, 4, LCD_RS, LCD_E, LCD_D4, LCD_D5, LCD_D6, LCD_D7, 0, 0, 0, 0);

	uint8_t laststate = HIGH;
    uint8_t counter = 0;
    uint8_t j = 0, i;
    float f; 

    dht11_dat[0] = dht11_dat[1] = dht11_dat[2] = dht11_dat[3] = dht11_dat[4] = 0;

    pinMode(DHTPIN, OUTPUT);
    digitalWrite(DHTPIN, LOW);
    delay(18);
        
    digitalWrite(DHTPIN, HIGH);
    delayMicroseconds(40);
        
    pinMode(DHTPIN, INPUT);

	for (i = 0; i < MAXTIMINGS; i++){
        counter = 0;
        while (digitalRead(DHTPIN) == laststate){
            counter++;
            delayMicroseconds(1);
            if (counter == 255){
                break;
            }
        }
        laststate = digitalRead(DHTPIN);

        if (counter == 255)
            break;

        if ((i >= 4) && (i % 2 == 0)){
            dht11_dat[j / 8] <<= 1;
            if (counter > 16)
                dht11_dat[j / 8] |= 1;
            j++;
        }
    }
	
    if(openI2CBus("/dev/i2c-1") == -1){
		printf("Sensor não está sendo lido!\n");
	}
	setI2CSlave(0x48);
	printf("Luminosidade: %.2f | %d-%02d-%02d %02d:%02d:%02d\n", readVoltage(0), tm.tm_mday, tm.tm_mon + 1, tm.tm_year + 1900, tm.tm_hour, tm.tm_min, tm.tm_sec);
	
    temp = readVoltage(0);
	printf("Pressão: %.2f | %d-%02d-%02d %02d:%02d:%02d\n", readVoltage(1), tm.tm_mday, tm.tm_mon + 1, tm.tm_year + 1900, tm.tm_hour, tm.tm_min, tm.tm_sec);
    temp2 = readVoltage(1);
    char array1[10];
    sprintf(array1, "%.2f", temp);
    strcat(strcpy(luminosidade, "Lum: "), array1);
	char array2[10];
    sprintf(array2, "%.2f", temp2);
    strcat(strcpy(pressao, "Pre: "), array2);    

	lcdPosition(lcd, 0, 0);
    lcdPuts(lcd, luminosidade); // Escrevendo no display
	lcdPosition(lcd, 0, 1);
	lcdPuts(lcd, pressao); // Escrevendo no display
	delay(2000);

	if ((j >= 40) && (dht11_dat[4] == ((dht11_dat[0] + dht11_dat[1] + dht11_dat[2] + dht11_dat[3]) & 0xFF))){
        f = dht11_dat[2] * 9. / 5. + 32;
		lcdPosition(lcd, 0, 0);
        lcdPrintf(lcd, "Umidade: %d.%d %%\n", dht11_dat[0], dht11_dat[1]);
		printf("Umidade: %d.%d %% | %d-%02d-%02d %02d:%02d:%02d\n", dht11_dat[0], dht11_dat[1], tm.tm_mday, tm.tm_mon + 1, tm.tm_year + 1900, tm.tm_hour, tm.tm_min, tm.tm_sec);

        lcdPosition(lcd, 0, 1);
        lcdPrintf(lcd, "Temp: %d.0 C", dht11_dat[2]);
		printf("Temperatura: %d.0 C | %d-%02d-%02d %02d:%02d:%02d\n", dht11_dat[2], tm.tm_mday, tm.tm_mon + 1, tm.tm_year + 1900, tm.tm_hour, tm.tm_min, tm.tm_sec);
    }
	delay(2000);
}

int main(){ // Função principal do sistema
	wiringPiSetup();
	int i;
	for (i = 2; i <= 3; i++)
	{
		ldr_bpm();  
        system("clear");
		i--;
	}
    return 0;
}