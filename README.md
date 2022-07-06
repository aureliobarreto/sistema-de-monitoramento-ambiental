# ✅ Sistema de Monitoramento Ambiental
## Sistema para obter medições de sensores em tempo real por estação de medição física ou cliente remoto.



Índice
=================
<!--ts-->
   * [Sobre](#sobre)
   * [Equipe de Desenvolvimento](#equipe-de-desenvolvimento)
   * [Tecnologias](#tecnologias)
   * [Configuração do Ambiente](#configuracao)
   * [Como usar](#como-usar)
   * [Anexos](#anexos)
<!--te-->
<div id="sobre">
    <h1>:pencil: Sobre o projeto</h1>
    <p align="justify">
    O projeto proposto pela disciplina <b><a href="http://sites.ecomp.uefs.br/tec499/2016-1">TEC499 - MI Sistemas Digitais</a></b> trata-se de um protótipo de um sistema para
    monitoramento ambiental que controla sensores analógicos e digitais e uma IHM (Interface Homem-Máquina) para apresentação das informações.
    </p>
    <h3>SBC (Single Board Bomputer)</h3>
    <p align="justify">
    O protótipo foi desenvolvido na SBC Raspberry Pi Zero e mede temperatura, umidade, pressão atmosférica e luminosidade. O protótipo apresenta em tempo real as informações de leitura em um display de LCD com base na seleção de chaves, sendo 4 chaves, uma para cada tipo de leitura. É realizado também o armazenamento do histórico das últimas 10 medições realizadas. O protótipo conta ainda com 3 botões, um para exibir o histórico de medições e outros dois para incrementar e decrementar o tempo de leitura dos sensores.
    </p>
    <h4>MQTT (Message Queuing Telemetry Transport)</h4>
    <p align="justify">
    O protocolo MQTT foi implementado para realizar a comunicação da <a href="#estacao_local">estação local</a> (SBC) com uma estação remota (Web). Ele é utilizado para enviar em tempo real as medições que estão sendo exibidas no display da estação local. Uma vez que o botão de exibição do histórico é pressionado, o protocolo MQTT também é utilizado para realizar o envio do histórico para a <a href="#estacao_remota">estação remota</a>.
    </p>
    <h3>Cliente Web</h3>
</div>


<div id="equipe-de-desenvolvimento">    
    <h1>:man_technologist: Equipe de Desenvolvimento</h1>
    <ul>
	<li><a href="https://github.com/argalvao"> Abel Ramalho Galvão</li>
	<li><a href="https://github.com/aureliobarreto"> Aurélio Rocha Barreto </a></li>
        <li><a href="https://github.com/amandassa"> Amanda Silva Santos </a> </li>
	</ul>
    <h3>Tutor</h3>
    <ul>
        <li><a href="https://github.com/thiagocj">Thiago Cerqueira de Jesus</a></li>
    </ul>
</div>

<div id="tecnologias">
    <h1>🛠 Tecnologias</h1>
    <p>As seguintes tecnologias e ferramentas foram usadas na construção do projeto:</p>
    <ul>
    <li><a href="https://www.ibm.com/docs/pt/i/7.2?topic=languages-c-c">C</li>
    <li><a href="https://docs.python.org/3/">Python</li>
    <li><a href="https://flask.palletsprojects.com/en/2.1.x/">Flask</li>
    <li><a href="https://mqtt.org/">MQTT</li>
    <li><a href="https://www.raspberrypi.com/products/raspberry-pi-zero/">RaspberryPi Zero</li>
    </ul>
</div>

## ⚙️ Configuração do Ambiente
### Raspbery PI (Estação Física)
Para conseguir rodar o programa em C, devemos certificar que esteja instalado na Raspberry Pi Zero a biblioteca:

<div id="configuracao">
    <h1>⚙️ Configuração do Ambiente</h1>
    <h3>Raspbery PI (Estação Física)</h3>
    <p  align="justify">Para conseguir executar o programa em C, devemos certificar que esteja instalado na Raspberry Pi Zero as seguintes bibliotecas:</p>
    <ul>
        <li><a href="http://wiringpi.com/download-and-install/">WiringPi</a></li>  
        <li><a href="https://github.com/ControlEverythingCommunity/ADS1115">ADS1115</a></li>
        <li><a href="https://mosquitto.org/download/">Mosquitto</a></li>
    </ul>
    <p  align="justify">Caso sua raspberry não possua acesso à Internet você pode acessá-la via SSH e transferir a biblioteca e a pasta do repositório via SCP de sua máquina origem para o Raspberry.</p> 
</div>
	
### 📜 Com acesso a sua Raspberry siga os seguintes passos

```bash
# Clone este repositório
$ git clone <https://github.com/aureliobarreto/sistema-de-monitoramento-ambiental>
# Acesse a pasta do projeto no terminal/cmd
$ cd sistema-de-monitoramento-ambiental
# Compile o arquivo .C utilizando o arquivo Makefile que está no diretório
$ sudo make
# Execute a aplicação
$ sudo ./pbl3
```

### Interface Web (Cliente)

<div id="anexos">
	<h1> Anexos </h1>
    <div id="estacao_local" style="display: inline_block" align="center">
			<img src="https://github.com/aureliobarreto/sistema-de-monitoramento-ambiental/blob/main/imagens/estacao_local.jpeg"/><br>
		<p>
		<b>Imagem 01</b> - Estação local. 
		</p>
	</div>
	<div id="estacao_remota" style="display: inline_block" align="center">
			<img src="https://github.com/aureliobarreto/sistema-de-monitoramento-ambiental/blob/main/imagens/estacao_remota.png"/><br>
		<p>
		<b>Imagem 02</b> - Estação remota. <b>
		</p>
	</div>	
</div>