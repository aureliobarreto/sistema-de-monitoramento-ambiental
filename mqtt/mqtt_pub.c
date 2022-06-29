#include <stdio.h>
#include <mosquitto.h>
#include <string.h>

char *envio_de_msg = "Amanda da Silva e Aurélio Barreto";

int main(){
    int rc;
    struct mosquitto * mosq;
    mosquitto_lib_init();
    mosq = mosquitto_new("TP02-G01", true, NULL);
    mosquitto_username_pw_set(mosq, "aluno", "aluno*123");
    rc = mosquitto_connect(mosq, "10.0.0.101", 1883, 60);
    if (rc != 0)
    {
        printf("Cliente não conectado ao broker! Error: %d\n", rc);
        mosquitto_destroy(mosq);
        return -1;
    }
    printf("## Conectado com sucesso ao broker ##\n");
    mosquitto_publish(mosq, NULL, "TP02-G01", strlen(envio_de_msg), envio_de_msg, 0, false);
    mosquitto_disconnect(mosq);
    mosquitto_destroy(mosq);
    mosquitto_lib_cleanup();    
    return 0;
}