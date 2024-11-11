Simulador de Máquina Virtual - README
Descrição

Este programa simula a execução de uma sequência de instruções de uma máquina virtual simples, utilizando registradores e comandos como mov, add, sub, mul, div, entre outros. O código é implementado em linguagem C e é capaz de ler um arquivo contendo instruções, processá-las, e executar a simulação dos comandos de acordo com o estado dos registradores. As instruções são armazenadas em uma lista duplamente encadeada, e o programa simula o comportamento de uma CPU simples, controlando o fluxo de execução com o registrador de programa (PC).
Estrutura de Dados

A estrutura de dados principal usada no programa é a lista duplamente encadeada de instruções (instruction), onde cada instrução pode ter um operador (op), um ou dois registradores/valores de operandos (reg1, reg2, val1), e ponteiros para os elementos anterior e próximo da lista.
Registradores

Existem quatro registradores na simulação:

    ACC (Acumulador): Usado para armazenar resultados intermediários das operações.
    DAT: Registrador de dados.
    EXT: Registrador de extensão.
    PC (Program Counter): Registrador que armazena a posição da próxima instrução a ser executada.

Esses registradores são manipulados por várias instruções, como mov, add, jmp, etc.
Principais Funções

    Funções de Manipulação de Instruções:
        createInstruction(): Cria uma nova instrução e aloca memória para ela.
        insertInstruction(): Insere uma nova instrução no final da lista duplamente encadeada.
        limpaInstructions(): Libera a memória de todas as instruções da lista.

    Função de Decodificação de Linha:
        decodificaLinha(): Lê uma linha de texto e decodifica os campos da instrução (operador, registradores, valores) para inserir na lista de instruções.

    Funções de Execução de Instruções:
        mov(): Realiza a cópia de um valor de um registrador ou valor para outro registrador.
        add(), sub(), mul(), divi(), mod(): Realizam operações aritméticas no registrador ACC com base nos valores de outros registradores ou valores imediatos.
        jmp(), jeq(), jlt(), jgt(): Instruções de salto condicional ou incondicional.
        prt(): Imprime o valor de um registrador ou valor imediato.

    Função de Execução Geral:
        executar(): Percorre a lista de instruções e executa cada uma delas, modificando os registradores de acordo com os comandos. A execução pode alterar o PC, simulando o fluxo de controle do programa.

        Como Funciona

    Entrada de Dados: O programa recebe como argumento o caminho de um arquivo de texto que contém uma lista de instruções. Cada linha do arquivo pode conter uma instrução no formato:

    OPERAÇÃO REG1 REG2 VALOR

    Onde OPERAÇÃO é o nome do comando (como mov, add, etc.), REG1 e REG2 são registradores ou valores, e VALOR é um número inteiro (caso necessário).

    Leitura do Arquivo: O programa lê o arquivo linha por linha, decodifica cada linha em uma instrução e a insere em uma lista encadeada.

    Execução das Instruções: Após carregar as instruções, o programa executa-as na ordem definida pelo registrador de programa (PC), manipulando os registradores conforme a operação definida em cada instrução.

    Impressão de Resultados: Algumas instruções, como prt, imprimem o valor dos registradores ou valores diretamente na saída padrão.

    Finalização: Após a execução de todas as instruções, a memória alocada para as instruções é liberada.

Exemplo de Arquivo de Entrada

mov acc dat 0
add acc 5
sub dat 2
mul ext 3
jmp 2
prt acc 0

Este arquivo contém as instruções para mover valores entre os registradores, realizar operações aritméticas e imprimir o conteúdo do registrador ACC.
Descrição das Instruções no Exemplo:

    mov acc dat 0: Move o valor de DAT para o registrador ACC.
    add acc 5: Soma 5 ao valor no registrador ACC.
    sub dat 2: Subtrai 2 do valor no registrador DAT.
    mul ext 3: Multiplica o valor do registrador EXT por 3 e armazena no ACC.
    jmp 2: Realiza um salto para a instrução 2 (pular para o início).
    prt acc 0: Imprime o valor do registrador ACC.

Como Executar

    Compile o código usando um compilador C, por exemplo:

gcc -o simulador simulador.c

Execute o programa passando o caminho do arquivo de entrada como argumento:

    ./simulador caminho_do_arquivo.txt

Considerações

    O código manipula os registradores de maneira simplificada, sem lidar com detalhes mais avançados de uma máquina real, como interrupções ou gerenciamento de memória.
    As instruções de salto condicional (jmp, jeq, jlt, jgt) alteram o valor do registrador PC, controlando o fluxo de execução.
    O programa pode ser expandido para adicionar mais operações e funcionalidades, dependendo dos requisitos da simulação.

Licença

Este código é de uso acadêmico, criado como parte de um trabalho de ALGPROG2 em um curso de Ciência da Computação.
