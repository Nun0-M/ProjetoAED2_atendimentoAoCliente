#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <limits.h>
#include <string.h>
#include <unistd.h>

typedef struct Clientela{
    char Nome[50];
    char Numero[13];
    int Especialidade;
    int Prioridade; //bool
    int Tempo_atendimento_atribuido;
    int Estado_atendimento; //bool
    int Tempo_espera;
    int atendente_atribuido;
    struct Clientela *proximo;
}Clientes;

typedef struct Atendentes{
    int numero_Funcionario;
    int Especialidade; //1 , 2 , ou 0 para geral
    int Tempo_atendimento_atribuido;
    struct Atendentes *proximo_atendente;
}Atendente;

typedef struct{
    Atendente* inicio_fila;
    Atendente* fim_fila;
}Fila_atendentes;

