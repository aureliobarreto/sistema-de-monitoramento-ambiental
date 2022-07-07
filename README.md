# ✅ Sistema de Monitoramento Ambiental
## Sistema para obter medições de sensores em tempo real por estação de medição física ou cliente remoto.



Índice
=================
<!--ts-->
   * [Sobre](#sobre)
   * [Equipe de Desenvolvimento](#equipe-de-desenvolvimento)
   * [Tecnologias](#tecnologias)
   * [Solução](#solucao)
   * [Configuração do Ambiente](#configuracao)
     * [Estação Local](#configuracao-local)
     * [Estação Remota](#configuracao-remota)
   * [Executando Aplicação](#executar)
     * [Inicializando Estação Local](#executar-local)
     * [Inicializando Estação Remota](#executar-remota)
   * [Demonstração](#demonstracao)
   * [Possíveis Melhorias](#melhoria)
   * [Considerações Finais](#consideracoes-finais)
   * [Anexos](#anexos)
<!--te-->
<div id="sobre">
    <h1>:pencil: Sobre o projeto</h1>
    <p align="justify">
    O projeto proposto pela disciplina <b><a href="http://sites.ecomp.uefs.br/tec499">TEC499 - MI Sistemas Digitais</a></b> trata-se de um protótipo de um sistema para monitoramento ambiental que controla sensores analógicos e digitais e uma IHM (Interface Homem-Máquina) para apresentação das informações.
    </p>
</div>

<div id="solucao">
    <h1>💡 Solução</h1>
    <h3>SBC (Single Board Computer)</h3>
    <p align="justify">
    O protótipo foi desenvolvido na SBC Raspberry Pi Zero e mede temperatura, umidade, pressão atmosférica e luminosidade. O produto apresenta em tempo real as informações de leitura em um display de LCD 16x2 com base na seleção de chaves, sendo 4 chaves, uma para cada tipo de leitura. É realizado também o armazenamento do histórico das últimas 10 medições realizadas. O protótipo conta ainda com 3 botões, um para exibir o histórico de medições e outros dois para incrementar e decrementar o intervalo de leitura dos sensores. Para isso, foram configuradas interrupções que verificam qualquer alteração no estado lógico dos botões que, ao serem pressionados, interrompem a função principal de leitura dos sensores e realizam as alterações de tempo ou envio/exibição de histórico.
    </p>
    <h3>MQTT (Message Queuing Telemetry Transport)</h3>
    <p align="justify">
    O protocolo MQTT foi implementado para realizar a comunicação da <a href="#estacao_local">estação local</a> (SBC) com uma estação remota (Web). Ele é utilizado para enviar em tempo real as medições que estão sendo exibidas no display da estação local. Uma vez que o botão de exibição do histórico é pressionado, o protocolo MQTT também é utilizado para realizar o envio do histórico para o <a href="#estacao_remota">painel remoto</a>. Além disso, a estação local também utiliza o protocolo MQTT para receber instruções da estação remota, como alteração do tempo e solicitação de histórico. Para isso, foi configurado uma thread que fica sendo processada em paralelo à função principal, onde a estação local fica aguardando o recebimento de uma instrução, que por sua vez realiza uma interrupção no sistema a cada solicitação de mudança de tempo ou pedido de histórico.
    </p>
    <h3>Cliente Web</h3>
    As medições realizadas pelo módulo da SBC enviadas via MQTT são exibidas neste painel de monitoramento em tempo real. O cliente também pode solicitar a visualização do histórico de medições com os 10 últimos registros e alterar o intervalo entre as medições. As solicitações são enviadas via MQTT para a estação de medição na SBC.
    O servidor web foi implementado usando o framework Flask em Python e a biblioteca Flask MQTT para comunicação com o broker.
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
        <li><a href="https://www.raspberrypi.com/products/raspberry-pi-zero/">
        RaspberryPi Zero</li>
    </ul>
</div>

<div id="configuracao">
    <h1>⚙️ Configuração do Ambiente</h1>
    <h3>Raspbery PI Zero (Estação Local)</h3>
    <p  align="justify">Para conseguir executar o programa em C, devemos certificar que esteja instalado na Raspberry Pi Zero as seguintes bibliotecas:</p>
    <ul>
        <li><a href="http://wiringpi.com/download-and-install/">WiringPi</a></li>  
        <li><a href="https://github.com/ControlEverythingCommunity/ADS1115">ADS1115</a></li>
        <li><a href="https://mosquitto.org/download/">Mosquitto</a></li>
    </ul>
    <h3>Interface Web (Estação Remota)</h3>
    <p>Listar bibliotecas</p>
</div>

<div id="configuracao-local">
<h4>Para configurar a Raspberry siga os seguintes passos:<h4>
</div>

```bash
# Clone este repositório
$ git clone <https://github.com/aureliobarreto/sistema-de-monitoramento-ambiental>
# Acesse a pasta do projeto no terminal
$ cd sistema-de-monitoramento-ambiental
# Compile o projeto em C utilizando o arquivo Makefile que está no diretório
$ sudo make
```
<p  align="justify">Caso sua raspberry não possua acesso à Internet você pode acessá-la via SSH e transferir a biblioteca e a pasta do repositório via SCP de sua máquina origem para o Raspberry.</p> 

<div id="configuracao-remota">
<h4>Para configurar a interface Web siga os seguintes passos:</h4>
</div>

 ```bash
# Com o repositório clonado em sua máquina, 
# acesse a pasta do cliente no terminal
$ cd sistema-de-monitoramento-ambiental/client_src
# Caso não tenha Python 3 instalado globalmente, instale com:
$ sudo apt-get install python3
# E também o gerenciador de pacotes pip:
$ sudo apt-get install python3-pip
# Crie o ambiente virtual
$ python3 -m venv .venv
$ source .venv/bin/activate     # ative-o
# Instale as dependências
$ pip install -r requirements.txt
```

<div id="#executar">
    <h1>:arrow_right: Executar Aplicação</h1>
    <p>Para executar o projeto (estação local e estação remota), siga as instruções abaixo:</p>
</div>

<div id="#executar-local">
    <h3>Inicializar Estação Local</h3>

```bash
# Acesse a pasta do projeto no terminal
$ cd sistema-de-monitoramento-ambiental
# Execute o projeto utilizando o comando
$ sudo ./pbl3
```

</div>
<div id="#executar-local">
    <h3>Inicializar Estação Remota</h3>

```bash
# Com o ambiente virtual ativo e ainda no diretório /client_src, execute:
$ flask run
# Acesse a página localhost:5000 em seu navegador e clique em "Iniciar".
```
</div>

<div id="demonstracao">
<h1>:video_game: Demonstração</h1>
<h3>Estação Local</h3>
<p>
Para demosntrar o funcionamento da estação local, utiliza-se os botões e os switchs. A coleta dos dados é feita através do sensor DHT11 para umidade e temperatura e, dois potênciometros são utilizados para simular lumiosidade e pressão. Toda a exibição da leitura dos sensores é feita pelo Display de LCD.<br>
<img src="https://github.com/aureliobarreto/sistema-de-monitoramento-ambiental/blob/main/imagens/local_gif.gif"/>
</p>
</div>

<h4>Utilizando os switchs:</h4>

```
Os swichs são utilizados para selecionar o tipo de medição que deverá ser visualizado:

Switch 1 > Umidade na primeira linha do LCD
Switch 2 > Temperatura na segunda linha do LCD
Swichh 3 > Luminosidade na primeira linha do LCD
Switch 4 > Pressão na quarta linha do LCD
```
<h4>Utilizando os switchs:</h4>

```
Os botões são utilizados para modificar o tempo de leitura dos sensores e realizar uma chamada no histórico de medições:

Botão 1 > Exibe o histórico no LCD e envia para a estação rmeota
Botão 2 > Incrementa 1 segundo no tempo de leitura
Botão 3 > Decrementa 1 segundo no tempo de leitura
```

<h3>Estação Remota</h3>


<div id="melhoria">
<h1>:tickets: Melhorias Possíveis</h1>
<ul>
        <li>Substituição da função delay por um timer com o objetivo de otmizar a contagem de tempo</li>  
        <li>Melhora na chamada de interrupção</li>
        <li>Ler o arquivo de hisórico apenas uma vez para exibir no display e enviar para a estação remota</li>
    </ul>
</div>

<div id="consideracoes-finais">
    <h1>:label: Considerações finais</h1>
    <p>
   O projeto proposto cumpre todos os requisitos solicitados no problema, porém, é passivo de melhorias como a exibição do histório no display de LCD, onde linhas vazias do arquivo de histórico acabam reproduzindo caracteres sem sentido e no uso das interrupções dos botões. Faz-se necessário também, implemntar validações no envio das solicitações do cliente para a estação local.
    </p>
</div>

<div id="anexos">
	<h1>📎 Anexos </h1>
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