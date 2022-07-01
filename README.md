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
    <h3>Tutor</h3>
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

## ⚙️ Configuração do Ambiente
### Raspbery PI (Estação Física)
Para conseguir rodar o programa em C, devemos certificar que esteja instalado na Raspberry Pi Zero a biblioteca:

<ul>
	<li><b>mosquitto</b></li>
	<li><a href="https://mosquitto.org/download/">Download</a></li>
	<li><a href="https://mosquitto.org/api/files/mosquitto-h.html">Documentação</a></li>
</ul>
	
<ul>
	<p>Caso sua raspberry não possua acesso à Internet você pode acessá-la via SSH e transferir a biblioteca e a pasta do repositório via SCP de sua máquina origem para a raspberry.</p> 
</ul>

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
