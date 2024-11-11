/**************************************************
*
* Samuel Vieira da Cunha Nahas
* Trabalho 2
* Professor(a): Diego Padilha Rupert
*
*/ 

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_OP 3
#define MAX_LINE 128

typedef struct instr {
    char op[MAX_OP+1];    /* Operação */

    char reg1[MAX_OP+1];  /* Registrador de operando 1, se registrador */
    int val1;             /* Valor do operando 1, se inteiro*/

    char reg2[MAX_OP+1];  /* Registrador de operando 2, se houver*/

    struct instr *next;   /* Anterior */
    struct instr *prev;   /* Próximo */
} instruction;

typedef enum { // usando enum para definir as posições de acesso no vetor de registradores
    ACC = 0,
    DAT = 1,
    EXT = 2,
    PC = 3
} reg;

// função para criar uma nova instrução
instruction *createInstruction(char *op, char *reg1, char *reg2, int val1) {

    instruction *instr = (instruction*)malloc(sizeof(instruction));
    strcpy(instr->op, op);
    strcpy(instr->reg1, reg1);
    strcpy(instr->reg2, reg2);
    instr->val1 = val1;
    instr->next = NULL;
    instr->prev = NULL;

    return instr;

}

// função que insere uma nova instrução no final da lista
void insertInstruction(instruction **head, char *op, char *reg1, char *reg2, int val1) {

    instruction *novoInstr = createInstruction(op, reg1, reg2, val1);

    if (*head == NULL) { // caso a lista esteja vazia

        *head = novoInstr;
        return;

    }

    instruction *p = *head;

    while (p->next != NULL) // while para pecorrer a lista até encontrar o último elemento
        p = p->next;

    // inseriondo no ultimo elemento da lista
    p->next = novoInstr;
    novoInstr->prev = p;

}

// função que libera a memória alocada a lista de instruções
void limpaInstructions(instruction **head) {

    instruction *p = *head;

    while (p != NULL) {  // while para pecorrer a lista e liberar a memória

        instruction *aux = p; // salvando na auxiliar o elemento que vai ser liberado
        p = p->next; // trocando a cabeça para próximo elemento da lista
        free(aux); 

    }

}

// função para pegar as informações da linha e inserir como instrução na lista
void decodificaLinha(char *line, instruction **head) {

    // variaveis que serão usadas para armazenar as informações da instrução
    char op[MAX_OP + 1] = "";
    char reg1[MAX_OP + 1] = "";
    char reg2[MAX_OP + 1] = "";
    int val1 = 0;

    char temp[MAX_OP + 1] = ""; // variavel temporaria para armazenar até decidir se o operando 1 eh um registrador ou um inteiro
    
    int i = 0;
    int caractereValidado = 0; // variavel para contar os caracteres validos
    int operacaoSelecionada = 0; // variavel para selecionar qual operação vai receber o conteudo da linha

    while (line[i] != '#' && line[i] != '\0') { // while para pecorrer a linha até encontrar o comentário ou acabar a linha

        if (line[i] != ' ' && line[i] != '\n') { // se o caractere não for espaço ou quebra de linha

            if (operacaoSelecionada == 0) // a primeira informação da linha é a operação
                op[caractereValidado] = line[i];

            else if (operacaoSelecionada == 1) // a segunda informação da linha pode ser o primeiro registrador ou um inteiro
                temp[caractereValidado] = line[i];

            else if (operacaoSelecionada == 2) // a terceira informação da linha é o segundo registrador
                reg2[caractereValidado] = line[i];

            caractereValidado++;

        } else { // ocorre quando o caractere eh um espaço ou uma quebra de linha

            if (caractereValidado > 0) { // se anteiormente houve algum caractere valido

                if (operacaoSelecionada == 1) { // se a operação for o segundo

                    if (temp[0] >= 'a' && temp[0] <= 'z') // se o primeiro caractere eh uma letra, então é um registrador
                        strcpy(reg1, temp);

                    else // se não, então é um inteiro
                        val1 = atoi(temp); // atoi função do stdlib para converter string para inteiro

                }

                operacaoSelecionada++; // seleciona a proxima operação

            }

            caractereValidado = 0; // como ocorreu um espaço ou uma quebra de linha, define o caractereValidado para 0 novamente

        } 
        
        if (line[i + 1] == '\0' && operacaoSelecionada == 1) { // se o proximo caractere for o fim da linha e a operação for o segundo

            if (temp[0] >= 'a' && temp[0] <= 'z') // se o primeiro caractere eh uma letra, então é um registrador
                strcpy(reg1, temp);

            else // se não, então é um inteiro
                val1 = atoi(temp); // atoi função do stdlib para converter string para inteiro

        }

        i++;

    
    }

    if (strcmp(op, "") != 0)  // se a operação não for vazia
        insertInstruction(head, op, reg1, reg2, val1); // insere a instrução quando os valores obtidos na linha forem validos

}

// função para executar a instrução mov
void mov(char *reg1, char *reg2, int val, int *regs) {

    // trecho para pegar o valor do primeiro registrador se não for um inteiro
    if (strcmp(reg1, "acc") == 0)
        val = regs[ACC];
    
    else if (strcmp(reg1, "dat") == 0)
        val = regs[DAT];
    
    else if (strcmp(reg1, "ext") == 0)
        val = regs[EXT];

    else if (strcmp(reg1, "pc") == 0)
        val = regs[PC];

    // trecho para atribuir o valor ao segundo registrador
    if (strcmp(reg2, "acc") == 0)
        regs[ACC] = val;
    
    else if (strcmp(reg2, "dat") == 0)
        regs[DAT] = val;
    
    else if (strcmp(reg2, "ext") == 0)
        regs[EXT] = val;

}

// funções para executar a instrução add
void add(char *reg1, int val, int *regs) {

     if (strcmp(reg1, "acc") == 0)
        regs[ACC] += regs[ACC];
    
    else if (strcmp(reg1, "dat") == 0)
        regs[ACC] += regs[DAT];
    
    else if (strcmp(reg1, "ext") == 0)
        regs[ACC] += regs[EXT];
    
    else
        regs[ACC] += val;

}

// funções para executar a instrução sub
void sub(char *reg1, int val, int *regs) {

    if (strcmp(reg1, "acc") == 0)   
        regs[ACC] -= regs[ACC]; 
    
    else if (strcmp(reg1, "dat") == 0)
        regs[ACC] -= regs[DAT]; 

    else if (strcmp(reg1, "ext") == 0)
        regs[ACC] -= regs[EXT];

    else
        regs[ACC] -= val;

}

// função para executar a instrução mul
void mul(char *reg1, int val, int *regs) {

    if (strcmp(reg1, "acc") == 0)
        regs[ACC] *= regs[ACC];
    
    else if (strcmp(reg1, "dat") == 0)
        regs[ACC] *= regs[DAT];
    
    else if (strcmp(reg1, "ext") == 0)
        regs[ACC] *= regs[EXT];
    
    else
        regs[ACC] *= val;

}

// função para executar a instrução div
void divi (char *reg1, int val, int *regs) { // tive que trocar o nome da função para divi já que div é uma função do stdlib
    
    if (strcmp(reg1, "acc") == 0)
        regs[ACC] /= regs[ACC];
    
    else if (strcmp(reg1, "dat") == 0)
        regs[ACC] /= regs[DAT];
    
    else if (strcmp(reg1, "ext") == 0)
        regs[ACC] /= regs[EXT];
    
    else
        regs[ACC] /= val;

}

// função para executar a instrução mod
void mod(char *reg1, int val, int *regs) {

    if (strcmp(reg1, "acc") == 0)
        regs[ACC] %= regs[ACC];
    
    else if (strcmp(reg1, "dat") == 0)
        regs[ACC] %= regs[DAT];
    
    else if (strcmp(reg1, "ext") == 0)
        regs[ACC] %= regs[EXT];
    
    else
        regs[ACC] %= val;

}

// função para executar a instrução jmp
void jmp(int val, int *regs) {

    regs[PC] += val;

}

// função para executar a instrução jeq
void jeq(int val, int *regs) {

    if (regs[ACC] == 0)
        regs[PC] += val;

}

// função para executar a instrução jlt
void jlt(int val, int *regs) {
    
    if (regs[ACC] < 0)
        regs[PC] += val;

}

// função para executar a instrução jgt
void jgt(int val, int *regs) {
    
    if (regs[ACC] > 0)
        regs[PC] += val;

}

// função para executar a instrução prt
void prt(char *reg1, int val, int *regs) {

    if (strcmp(reg1, "acc") == 0)
        printf("%d\n", regs[ACC]);
    
    else if (strcmp(reg1, "dat") == 0)
        printf("%d\n", regs[DAT]);
    
    else if (strcmp(reg1, "ext") == 0)
        printf("%d\n", regs[EXT]);

    else if (strcmp(reg1, "pc") == 0)
        printf("%d\n", regs[PC]);
    
    else
        printf("%d\n", val);

}

// função para execitar as instruções da lista
void executar(instruction **head, int *regs) {

    instruction *p = *head; // pega a cabeça da lista

    while (p != NULL) { // while para pecorrer a lista até o fim

        int tempPC = regs[PC]; // salvando o PC atual numa variavel temporária
        
        // trecho para executar as instruções correspondentes
        if (strcmp(p->op, "mov") == 0)
            mov(p->reg1, p->reg2, p->val1, regs);
            
        else if (strcmp(p->op, "add") == 0) 
            add(p->reg1, p->val1, regs);

        else if (strcmp(p->op, "sub") == 0)
            sub(p->reg1, p->val1, regs);

        else if (strcmp(p->op, "mul") == 0)
            mul(p->reg1, p->val1, regs);

        else if (strcmp(p->op, "div") == 0)
            divi(p->reg1, p->val1, regs);

        else if (strcmp(p->op, "mod") == 0)
            mod(p->reg1, p->val1, regs);

        else if (strcmp(p->op, "jmp") == 0)
            jmp(p->val1, regs);

        else if (strcmp(p->op, "jeq") == 0)
            jeq(p->val1, regs);

        else if (strcmp(p->op, "jlt") == 0)
            jlt(p->val1, regs);

        else if (strcmp(p->op, "jgt") == 0)
            jgt(p->val1, regs);

        else if (strcmp(p->op, "prt") == 0)
            prt(p->reg1, p->val1, regs);


        if (tempPC == regs[PC]) // caso a instrução não seja jmp, jeq, jlt ou jgt soma mais um ao PC
            regs[PC]++;

        // while para pecorrer a lista de instruções de acordo com o PC indo pra frente da lista
        while (regs[PC] > tempPC) { 
           
            p = p->next;
            tempPC++;

        }

        // while para pecorrer a lista de instruções de acordo com o PC indo pra tras da lista
        while (regs[PC] < tempPC) { 

            p = p->prev;
            tempPC--;

        }

    }

}

int main(int argc, char *argv[]) {

    instruction *listaInstruction = NULL; // lista de instruções inicializada vazia

    int regs[4] = {0, 0, 0, 0}; // os registradores ACC, DAT, EXT e PC, respectivamente

    char *caminho = argv[argc - 1]; // pegar o caminho do arquivo pela linha de comando
    char *linha = (char*)malloc(MAX_LINE); // aloca uma linha para ser lida com tamanho maximo da linha

    char c; // variavel para ler caracter do arquivo
    int i = 0; // contador da linha

    FILE *fp = NULL; // ponteiro para o arquivo

    fp = fopen(caminho, "r"); // abrir o arquivo em modo leitura

    if (fp == NULL) { // quando não conseguir ler o arquivo

        printf("caminho errado\n");
        return 1;
    
    }

    while (fscanf(fp, "%c", &c) != EOF) { // lendo caracter por caracter do arquivo usando fscanf

        if (c == '\n' || i == MAX_LINE - 1) { // quando encontrar uma quebra de linha ou o fim da linha

            linha[i] = '\0'; // substituir o \n por \0

            decodificaLinha(linha, &listaInstruction); // inserindo as insformações lidas na lista

            free(linha); // libera da memória a linha lida

            linha = (char *)malloc(MAX_LINE); // aloca uma nova linha para ser lida
            i = 0; // zera o contador da linha

        } else { // quando ainda tem conteudo na linha

            linha[i] = c; // adiciona o caracter lido na linha
            i++; // incrementa o contador da linha

        }

    }

    free(linha); // libera da memória a última linha lida

    fclose(fp); // fecha o arquivo
    
    executar(&listaInstruction, regs); // executar as instruções presente na lista

    limpaInstructions(&listaInstruction); // libera a memória alocada a lista de instruções

    return 0;

}