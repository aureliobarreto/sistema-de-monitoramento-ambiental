# ✅ Sistema de Monitoramento Ambiental
## Sistema para obter medições de sensores em tempo real por estação de medição física ou cliente remoto.



Índice
=================
<!--ts-->
   * [Sobre](#Sobre)
   * [Equipe de Desenvolvimento](#equipe-de-desenvolvimento)
   * [Tabela de Conteudo](#tabela-de-conteudo)
   * [Tecnologias](#tecnologias)
   * [Configuração do Ambiente](#instalacao)
   * [Como usar](#como-usar)
<!--te-->
### 🛠 Tecnologias

As seguintes tecnologias e ferramentas foram usadas na construção do projeto:

- C
- Python + Flask
- Mosquitto
- RaspberryPi Zero
- MQT

#### ⚙️ Configuração do Ambiente
Raspbery PI (Estação Física)
Para conseguir rodar o código em C, devemos certificar que esteja instalado na Raspberry Pi Zero as seguintes bibliotecas:
- <b>mosquitto</b>
- https://mosquitto.org/download/
- Documentação: https://mosquitto.org/api/files/mosquitto-h.html
- <b>pthread</b>
- Documentação: https://pubs.opengroup.org/onlinepubs/7908799/xsh/pthread.h.html
- Feito isso, basta efetuar a transferência desse arquivo para a raspberry:
- <li><a href="https://github.com/aureliobarreto/sistema-de-monitoramento-ambiental/blob/main/pbl3.c">pbl3.c</a></li>
- Caso sua raspberry não possua acesso à Internet você pode acessá-la via SSH e transferir o arquivo via SCP de sua máquina origem para a raspberry.

- Com o código já na raspberry para compilar e executar siga os seguintes passos
- Compilar: sudo gcc -o pbl3 pbl3.c pbl3-dev.c -lwiringPi -lwiringPiDev -lpthread -lmosquitto
- Executar: sudo ./pbl3.c
- 
- 
- 
