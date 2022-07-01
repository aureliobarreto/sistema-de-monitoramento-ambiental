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
### Sobre o projeto
Sistema proposto pela disciplina TEC499 MI Sistemas Digitais 

### Equipe de Desenvolvimento
<div id="equipe">    
    <ul>
	<li><a href="https://github.com/argalvao"> Abel Ramalho Galvão</li>
	<li><a href="https://github.com/aureliobarreto"> Aurélio Rocha Barreto </a></li>
        <li><a href="https://github.com/amandassa"> Amanda Silva Santos </a> </li>
	</ul>
    <h1>Tutor</h1>
    <ul>
        <li><a href="https://github.com/thiagocj">Thiago Cerqueira de Jesus</a></li>
    </ul>
</div>

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

<ul>
<b>mosquitto</b>
<li><a href="https://mosquitto.org/download/">Download</a></li>
<li><a href="https://mosquitto.org/api/files/mosquitto-h.html">Documentação</a></li>
</ul>
<p>Feito isso, basta efetuar a transferência desse arquivo para a raspberry:</p>
<li><a href="https://github.com/aureliobarreto/sistema-de-monitoramento-ambiental/blob/main/pbl3.c">pbl3.c</a></li>
<p>Caso sua raspberry não possua acesso à Internet você pode acessá-la via SSH e transferir o arquivo via SCP de sua máquina origem para a raspberry.</p>

<p>Com o código já na raspberry para compilar e executar siga os seguintes passos
Compilar: sudo gcc -o pbl3 pbl3.c pbl3-dev.c -lwiringPi -lwiringPiDev -lpthread -lmosquitto
Executar: sudo ./pbl3.c </p>
