#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#include <mosquitto.h>


void on_connect(struct mosquitto *mosq, void *obj, int rc) {
	printf("ID: %d\n", * (int *) obj);
	if(rc) {
		printf("Erro: %d\n", rc);
		exit(-1);
	}
	mosquitto_subscribe(mosq, NULL, "TP02-G01", 0);
}

void on_message(struct mosquitto *mosq, void *obj, const struct mosquitto_message *msg) {
	time_t t = time(NULL);
  	struct tm tm = *localtime(&t);
	char *recebido = msg->payload;
    int convertido = atoi(recebido);
	printf("Mensagem recebida em %s: %s - %d-%02d-%02d %02d:%02d:%02d\n", msg->topic, (char*) msg->payload,  tm.tm_mday, tm.tm_mon + 1, tm.tm_year + 1900, tm.tm_hour, tm.tm_min, tm.tm_sec);
}

int main() {
	int rc, id=12;

	mosquitto_lib_init();

	struct mosquitto *mosq;

	mosq = mosquitto_new("TP02-G01", true, &id);
	mosquitto_username_pw_set(mosq, "aluno", "aluno*123");
	mosquitto_connect_callback_set(mosq, on_connect);
	mosquitto_message_callback_set(mosq, on_message);
	
	rc = mosquitto_connect(mosq, "10.0.0.101", 1883, 60);
	if(rc) {
		printf("Cliente não conectado ao broker! Error: %d\n", rc);
		return -1;
	}

	mosquitto_loop_start(mosq);
	printf("Pressione Enter para sair...\n");
	getchar();
	mosquitto_loop_stop(mosq, true);

	mosquitto_disconnect(mosq);
	mosquitto_destroy(mosq);
	mosquitto_lib_cleanup();

	return 0;
}