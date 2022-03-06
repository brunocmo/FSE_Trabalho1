# Trabalho 1 - 2021/2

Trabalho 1 da disciplina de Fundamentos de Sistemas Embarcados (2021/2)

[Link repositório](https://gitlab.com/fse_fga/trabalhos-2021_2/trabalho-1-2021-2)

***

## Aluno
|Matrícula | Aluno |
| -- | -- |
| 18/0117548  |  [Bruno Carmo Nunes](https://github.com/brunocmo) |

## Requisitos

É necessário o *git*, *make*, e *gpp* instalados.

Tem o uso das bibliotecas instalados na Raspberry Pi: *wiringPi.h*, *wiringPiI2C.h* e *softPwm.h*.

***
## Instalação 

Para compilar o programa:

`make`

Para executar o programa`:

`make run`

Para limpar os arquivos .o e prog(executável):

`make clean`

***
## Uso

Ao iniciar o programa é possível ver três opções:

* A primeira você vai no modo terminal onde coloca o valor da temperatura desejada e depois escolhe se deseja colocar manualmente os valores para o PID, caso não queira o sistema já identifica se é a placa rasp42 ou rasp43, colocando então automaticamente os valores padrões dessas placas. Em seguida a aplicação começa, podendo então ligar ou desligar o forno pela dashboard. Caso queira parar o programa basta apertar CTRL+C para desligar o sistema e terminar o programa.

* A segunda você pode escolher ou não colocar os valores do PID como no passo anterior ou pegar de forma automática. Depois disso o controle é totalmente na dashboard disponibilizada. Onde pode ter a função de ligar, desligar, mudar o modo para potenciômetro ou modo curva reFlow. Caso queira parar o programa basta apertar CTRL+C para desligar o sistema e terminar o programa.

* A terceira opção termina o programa.

Toda vez que é iniciado o programa e o forno é ligado, são registrados os valores de data/hora, temperaturas e sinais, dentro do arquivo em *doc/log.csv* .

***
## Experimentos

1. Forno ligado por 10 minutos no modo potenciômetro

    * Figura mostrando as temperaturas e o tempo:

        ![Potenciometro1](/doc/potenciometro1.png)

    * Figura mostrando o sinal e o tempo:

        ![Potenciometro12](/doc/potenciometro2.png)


2. Forno ligado por 10 minutos no modo curva reFlow

    * Figura mostrando as temperaturas e o tempo:

        ![curvaFlow1](/doc/curvaFlow1.png)

    * Figura mostrando o sinal e o tempo:

        ![curvaFlow2](/doc/curvaFlow2.png)

***


## Screenshots

![terminal1](/doc/terminal1.png)

![terminal2](/doc/terminal2.png)