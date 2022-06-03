#include <stdio.h>
#include <mosquitto.h>

int main(){
    int rc;
    struct mosquitto * mosq;
    mosquitto_lib_init();
    mosq = mosquitto_new("publisher-test", true, NULL);
    rc = mosquitto_connect(mosq, "localhost", 1884, 60);
    if (rc != 0)
    {
        printf("Cliente não conectado ao broker! Error: %d\n", rc);
        mosquitto_destroy(mosq);
        return -1;
    }
    printf("## Conectado com sucesso ao broker ##\n");
    mosquitto_publish(mosq, NULL, "test/t1", 16, "Testando PBL 3", 0, false);
    mosquitto_disconnect(mosq);
    mosquitto_destroy(mosq);
    mosquitto_lib_cleanup();    
    return 0;
}