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
	<li><b>mosquitto</b></li>
	<li><a href="https://mosquitto.org/download/">Download</a></li>
	<li><a href="https://mosquitto.org/api/files/mosquitto-h.html">Documentação</a></li>
</ul>
<p>Feito isso, basta efetuar a transferência desse arquivo para a raspberry:</p>
	<li><a href="https://github.com/aureliobarreto/sistema-de-monitoramento-ambiental/blob/main/pbl3.c">pbl3.c</a></li>
	
<ul>
	<p>Caso sua raspberry não possua acesso à Internet você pode acessá-la via SSH e transferir o arquivo via SCP de sua máquina origem para a raspberry.</p> 
</ul>

<ul><p>Com o código já na raspberry para compilar e executar siga os seguintes passos
Compilar: sudo gcc -o pbl3 pbl3.c pbl3-dev.c -lwiringPi -lwiringPiDev -lpthread -lmosquitto
Executar: sudo ./pbl3.c </p></ul>

### Pré-requisitos

Antes de começar, você vai precisar ter instalado em sua máquina as seguintes ferramentas:
[Git](https://git-scm.com), [Node.js](https://nodejs.org/en/). 
Além disto é bom ter um editor para trabalhar com o código como [VSCode](https://code.visualstudio.com/)

### 🎲 Rodando o Back End (servidor)

```bash
# Clone este repositório
$ git clone <https://github.com/tgmarinho/nlw1>

# Acesse a pasta do projeto no terminal/cmd
$ cd nlw1

# Vá para a pasta server
$ cd server

# Instale as dependências
$ npm install

# Execute a aplicação em modo de desenvolvimento
$ npm run dev:server

# O servidor inciará na porta:3333 - acesse <http://localhost:3333>
```
