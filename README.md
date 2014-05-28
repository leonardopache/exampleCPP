exampleCPP
==========

C++ Projeto exemplo de um Demultiplexer, através de 1 entrada separar em linhas agrupadas.

Problema:
Você tem quatro servidores. Cada um deles tem vários terabytes de arquivos de log de um servidor HTTP, incluindo o cookie "userid", ordenado pelo tempo. Escreva um programa que separe todos os dados de um determinado userid em um arquivo próprio, oroidenado pelo tempo. Você pode colocar os arquivos de saída em qualquer um dos servidores ou em qualquer combinação de servidores, mas todas as entradas de um determinado userid devem estar em um único arquivo no cluster.


Exemplo de entrada:
177.126.180.83 - - [15/Aug/2013:13:54:38 -0300] "GET /meme.jpg HTTP/1.1" 200 2148 "-" "userid=5352b590-05ac-11e3-9923-c3e7d8408f3a"
177.126.180.83 - - [15/Aug/2013:13:54:38 -0300] "GET /lolcats.jpg HTTP/1.1" 200 5143 "-" "userid=f85f124a-05cd-11e3-8a11-a8206608c529"
177.126.180.83 - - [15/Aug/2013:13:57:48 -0300] "GET /lolcats.jpg HTTP/1.1" 200 5143 "-" "userid=5352b590-05ac-11e3-9923-c3e7d8408f3a"

Exemplo de saida:
No servidor 3 (por exemplo) há um arquivo chamado /tmp/5352b590-05ac-11e3-9923-
c3e7d8408f3a que contém as linhas:
177.126.180.83 - - [15/Aug/2013:13:54:38 -0300] "GET /meme.jpg HTTP/1.1" 200 2148 "-" "userid=5352b590-05ac-11e3-9923-c3e7d8408f3a"
177.126.180.83 - - [15/Aug/2013:13:57:48 -0300] "GET /lolcats.jpg HTTP/1.1" 200 5143 "-" "userid=5352b590-05ac-11e3-9923-c3e7d8408f3a"
No servidor 2 (por exemplo) há um arquivo chamado /tmp/f85f124a-05cd-11e3-8a11-a8206608c529 que contém a linha:
177.126.180.83 - - [15/Aug/2013:13:54:38 -0300] "GET /lolcats.jpg HTTP/1.1" 200 5143 "-" "userid=f85f124a-05cd-11e3-8a11-a8206608c529"


Premissas:
----------
-Faça rodar o mais rápido que puder, mas pense arquiteturalmente e seja pragmático (um algoritmo melhor é ok, reescrever o parser em assembler não é ok);
-Parsear o arquivo não é o ponto do exercício, assuma todas as simplificações
-Você não precisa implementar algo realmente distribuído. Sinta-se à vontade para simular os logs em vários servidores com arquivos em vários diretórios e ao invés de usar conexões via rede, usar alguma forma para simular localmente os quatro servidores (exemplo: threads, processos).

Solução
------------

Funcionalidades cobertas pelo programa
--------------------------------------

Possíveis melhorias identificadas
---------------------------------


Como Executar
-------------  
