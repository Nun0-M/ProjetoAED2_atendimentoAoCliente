#include "cabecalho.h"

void limparBuffer() {
    int c;
    while ((c = getchar()) != '\n' && c != EOF);
}

Clientes *criar_cliente() {
  Clientes *novo_cliente = (Clientes*)malloc(sizeof(Clientes));
  if (novo_cliente == NULL) {
    printf("Erro: Falha ao alocar memória para novo cliente.\n");
    exit(1);
  }
    return novo_cliente;
}

Clientes* inserir_cliente(Clientes* cabecalista, Clientes* novo_cliente) {
    if (cabecalista == NULL || novo_cliente->Tempo_atendimento_atribuido < cabecalista->Tempo_atendimento_atribuido) {
        novo_cliente->proximo = cabecalista;
        return novo_cliente;
    }
    Clientes* atual = cabecalista;
    while (atual->proximo != NULL && atual->proximo->Tempo_atendimento_atribuido <= novo_cliente->Tempo_atendimento_atribuido) {
        atual = atual->proximo;
    }
    novo_cliente->proximo = atual->proximo;
    atual->proximo = novo_cliente;
    return cabecalista;
}

Clientes* Ler_Clientes() {
   char paragem;
    FILE *lista_clientes;
    char ficheiro[20];
    Clientes *cabecalista = NULL;
    printf("Introduza o nome do documento da lista dos clientes: ");
    scanf("%19s", ficheiro);

    lista_clientes = fopen(ficheiro, "r");

    if (lista_clientes != NULL) {
        char linha[1024];
        while (fgets(linha, sizeof(linha), lista_clientes) != NULL) {
            Clientes *novo_cliente = criar_cliente();
            sscanf(linha, "%[^;];%12[^;];%d;%d\n", novo_cliente->Nome, novo_cliente->Numero, &novo_cliente->Especialidade,&novo_cliente->Prioridade);
            novo_cliente->Estado_atendimento = 0;
            novo_cliente->Tempo_espera = 0;
            novo_cliente->Tempo_atendimento_atribuido = 0;
            cabecalista = inserir_cliente(cabecalista, novo_cliente);
        }
        fclose(lista_clientes);
        Clientes *atual = cabecalista;
    } else {
        printf("Erro ao abrir o arquivo da lista dos clientes!\n");
    }
  limparBuffer();
  printf("Introduzir carater para prosseguir: ");
  scanf(" %c",&paragem);
  limparBuffer();
  system("cls");
  return cabecalista;
}

Clientes* organizarClientes(Clientes* cabecalista) {
    if (cabecalista == NULL || cabecalista->proximo == NULL) {
        return cabecalista;
    }

    Clientes *prioridade0_head = NULL;
    Clientes *prioridade1_head = NULL;
    Clientes *temp = cabecalista;
    Clientes *proximo;

    while (temp != NULL) {
        proximo = temp->proximo;

        // Criar uma cópia do nó atual
        Clientes *novoCliente = (Clientes *)malloc(sizeof(Clientes));
        if (novoCliente == NULL) {
            // Tratamento de erro de alocação de memória
            // Aqui você pode inserir um código para lidar com o erro
            return cabecalista;
        }
        *novoCliente = *temp;  // Copia os dados do cliente atual

        if (temp->Prioridade == 0) {
            // Insere na lista de prioridade 0
            novoCliente->proximo = prioridade0_head;
            prioridade0_head = novoCliente;
        } else if (temp->Prioridade == 1) {
            // Insere na lista de prioridade 1
            novoCliente->proximo = prioridade1_head;
            prioridade1_head = novoCliente;
        }

        temp = proximo;
    }
    temp = prioridade1_head;
    printf("imprimir lista\n");
    while(temp!=NULL){
      printf("%s\n",temp->Nome);
      temp=temp->proximo;
    }
    system("cls");
    // Concatenando a lista de prioridade 1 seguida pela lista de prioridade 0
    if (prioridade1_head == NULL) {
        cabecalista = prioridade0_head;
    } else {
        Clientes *ultimoPrioridade1 = prioridade1_head;
        while (ultimoPrioridade1->proximo != NULL) {
            ultimoPrioridade1 = ultimoPrioridade1->proximo;
        }
        ultimoPrioridade1->proximo = prioridade0_head;
        cabecalista = prioridade1_head;
    }

    return cabecalista;
}

void mostrarListaClientes(Clientes* cabecalista){
  Clientes *temp = cabecalista;
  char prosseguir;
  while (temp != NULL) {
    printf("Nome:%s, Telefone:%s, Especialidade:%d, Prioridade:%d\n", temp->Nome, temp->Numero, temp->Especialidade, temp->Prioridade);
    temp = temp->proximo;
  }
  free(temp);
  printf("Introduza um carater para prosseguir: ");
  limparBuffer();
  system("cls");
}

void inicializar_fila_atendentes(Fila_atendentes *fila) {
  fila->inicio_fila = NULL; // Inicializa a fila como NULL
  fila->fim_fila = NULL;    // Atribui ao final da fila NULL
}

void organizar_atendente(Fila_atendentes *fila, Atendente *atendente) {
  atendente->proximo_atendente = NULL; // Garantir que o próximo do novo atendente seja NULL
  if (fila->inicio_fila == NULL) { // Se a fila estiver vazia
    fila->inicio_fila = atendente;
    fila->fim_fila = atendente;
  } else {
    fila->fim_fila->proximo_atendente = atendente; // Definir o próximo do último atendente como o novo atendente
    fila->fim_fila = atendente; // Atualizar o fim da fila para o novo atendente
  }
}

void libertar_ligar(Clientes *cliente_atual, Clientes *cabecaLista, Fila_atendentes fila) {
    Clientes *membro_aseguir_temp;
    Clientes *temp;
    temp = cabecaLista;
    membro_aseguir_temp = cabecaLista;
    while (membro_aseguir_temp != NULL) {
        membro_aseguir_temp = membro_aseguir_temp->proximo;
        if (membro_aseguir_temp == cliente_atual) {
            break;
        }
        temp = temp->proximo;
    }
    membro_aseguir_temp = membro_aseguir_temp->proximo;
    temp->proximo = membro_aseguir_temp;
    free(cliente_atual);
}

  //esta funcao vai dar return á cabeça da lista dos atendentes ocupados
void atribuirAtendentes(Fila_atendentes *fila, Clientes *cabecaLista, Atendente *listaAtendenteOcupado) {
    Atendente* atendenteTemp = fila->inicio_fila;
    Atendente* nextAtendenteTemp; // Temporary pointer to store the next Atendente

    while (atendenteTemp!= NULL) {
        nextAtendenteTemp = atendenteTemp->proximo_atendente; // Store the next Atendente
        Clientes* clienteTemp = cabecaLista;
        while (clienteTemp!= NULL) {
            if (clienteTemp->Especialidade == atendenteTemp->Especialidade && clienteTemp->Estado_atendimento == 0) {
                clienteTemp->atendente_atribuido = atendenteTemp->numero_Funcionario;
                clienteTemp->Estado_atendimento = 1;
                clienteTemp->Tempo_atendimento_atribuido = rand() % 10 + 1; // Consider using a more robust timing mechanism
                atendenteTemp->proximo_atendente = listaAtendenteOcupado;
                listaAtendenteOcupado = atendenteTemp;
                fila->inicio_fila = nextAtendenteTemp; // Update fila->inicio_fila only after processing the current Atendente
                break;
            }
            clienteTemp = clienteTemp->proximo;
        }
        atendenteTemp = nextAtendenteTemp; // Move to the next Atendente
    }
}

void mostrarListaClientesAtendidos(Clientes* cabecalista){
  Clientes *temp = cabecalista;
  printf("__________________________________________\n");
  while (temp != NULL) {
    if(temp->Estado_atendimento==1){
    printf("Nome:%s, Telefone:%s, Especialidade:%d, Prioridade:%d\n", temp->Nome, temp->Numero, temp->Especialidade, temp->Prioridade);
    printf("Atendido pelo atendedor nr : %d \n",temp->atendente_atribuido);
    printf("Tempo de atendimento restante:%d\n",temp->Tempo_atendimento_atribuido);
    printf("_______________________________________\n");
     }
     temp = temp->proximo;
   }
  free(temp);
}

void mostrarListaClientesPorAtender(Clientes* cabecalista){
  Clientes *temp = cabecalista;
    printf("__________________________________________\n");
  while (temp != NULL) {
    if(temp->Estado_atendimento==0){
    printf("Nome:%s, Telefone:%s, Especialidade:%d, Prioridade:%d\n", temp->Nome, temp->Numero, temp->Especialidade, temp->Prioridade);
    printf("Tempo de espera: %d\n",temp->Tempo_espera);
    printf("_______________________________________\n");
     }
     temp = temp->proximo;
   }
  free(temp);
}

void atualizarTempo(Clientes* cabecalista,Atendente* listaAtendentensOcupados){
  Clientes *temp = cabecalista;
  Atendente *atendenteOcupadoTemp;
  atendenteOcupadoTemp = listaAtendentensOcupados;
  while(temp != NULL){
    if(temp->Estado_atendimento==1){
      temp->Tempo_atendimento_atribuido--;
    }else{
      temp->Tempo_espera++;
    }
    temp=temp->proximo;
  }
  while (atendenteOcupadoTemp!=NULL){
    atendenteOcupadoTemp->Tempo_atendimento_atribuido--;
    atendenteOcupadoTemp = atendenteOcupadoTemp->proximo_atendente;
  }
  free(atendenteOcupadoTemp);
  free(temp);
}

int isEmpty(Fila_atendentes fila){
return(fila.inicio_fila==NULL);
}

int verificarChamadaTerminada(Atendente *listaAtendenteOcupado, Fila_atendentes *fila) {
    int numero_atendentes_removidos = 0;
    Atendente *temp = listaAtendenteOcupado;
    Atendente *anterior = NULL;

    while (temp != NULL) {
        if (temp->Tempo_atendimento_atribuido == 0) {
            // Atendente terminou o atendimento, remover da lista de atendentes ocupados
            if (anterior == NULL) {
                listaAtendenteOcupado = temp->proximo_atendente;
            } else {
                anterior->proximo_atendente = temp->proximo_atendente;
            }

            // Adicionar o atendente ao final da fila de atendentes livres
            if (fila->inicio_fila == NULL) {
                fila->inicio_fila = temp;
            } else {
                fila->fim_fila->proximo_atendente = temp;
            }
            fila->fim_fila = temp;
            temp->proximo_atendente = NULL;

            // Atualizar o ponteiro temp para o próximo atendente
            Atendente *prox = temp->proximo_atendente;
            free(temp);
            temp = prox;

            numero_atendentes_removidos++;
        } else {
            anterior = temp;
            temp = temp->proximo_atendente;
        }
    }

    return numero_atendentes_removidos;
}


void removerClientesTerminados(Clientes* cabecalista) {
    Clientes *temp = cabecalista;
    Clientes *anterior = NULL;

    while (temp != NULL) {
        if (temp->Estado_atendimento == 1 && temp->Tempo_atendimento_atribuido <= 0) {
            // Remoção do nó que atende aos requisitos
            if (anterior == NULL) {
                cabecalista = temp->proximo;
            } else {
                anterior->proximo = temp->proximo;
            }
            // Libera a memória do nó removido
            Clientes *prox = temp->proximo;
            free(temp);
            temp = prox;
        } else {
            anterior = temp;
            temp = temp->proximo;
        }
    }
}


int main() {
  int numero_atendentes_concluidos;
	int i;
  srand(time(NULL));
  Clientes *inicio_Lista = NULL;
  Fila_atendentes filaAtendentesLivres;//Fila dos atendentes livres
  Atendente *listaAtendentesOcupados;//Cabeça da lista com os atendentes ocupados
  listaAtendentesOcupados=NULL;
  int num_atendentes;
  int input_debug;
  char continuar;
  int simulacao_terminada = 0;
  listaAtendentesOcupados = NULL;
  inicio_Lista = Ler_Clientes();
  mostrarListaClientes(inicio_Lista);
  inicio_Lista = organizarClientes(inicio_Lista);
  mostrarListaClientes(inicio_Lista);
  printf("Digite o número de atendentes na fila: ");
  scanf("%d", &num_atendentes);
  inicializar_fila_atendentes(&filaAtendentesLivres);
  for (i = 0; i < num_atendentes; i++) {
    Atendente *novo_atendente = (Atendente *)malloc(sizeof(Atendente));
    novo_atendente->numero_Funcionario = i+1;
    novo_atendente->Especialidade = rand() % 3;
    novo_atendente->Tempo_atendimento_atribuido = 0;
    novo_atendente->proximo_atendente = NULL;
    organizar_atendente(&filaAtendentesLivres, novo_atendente);
  }
  
// Impressão dos detalhes de cada atendente na fila
  Atendente *temp = filaAtendentesLivres.inicio_fila;
  while (temp != NULL) {
    numero_atendentes_concluidos = 0;
    printf("Funcionario numero %d Especialidade %d\n", temp->numero_Funcionario, temp->Especialidade);
    temp = temp->proximo_atendente;
  }
  free(temp);
  // Paragem de debugging
  printf("Paragem de debugging: ");
  scanf("%d", &input_debug);
  limparBuffer();
  printf("Introduza um carater para comecar a simulacao: ");
  scanf("%c",&continuar);
  limparBuffer();
  system("cls");
    do{
      if(!isEmpty(filaAtendentesLivres)){  //Aqui determinamos que existem atendentesLivres, logo ativamos a funçao para atribuir atendentes
      atribuirAtendentes(&filaAtendentesLivres,inicio_Lista,listaAtendentesOcupados);
      }
      printf("Lista de clientes em atendimento\n");
      mostrarListaClientesAtendidos(inicio_Lista);
      printf("\nLista de clientes por atender\n");
      mostrarListaClientesPorAtender(inicio_Lista);
      numero_atendentes_concluidos = 0;
      sleep(10);
      atualizarTempo(inicio_Lista,listaAtendentesOcupados);
      numero_atendentes_concluidos = verificarChamadaTerminada(listaAtendentesOcupados,&filaAtendentesLivres);
      if(numero_atendentes_concluidos>0){
        removerClientesTerminados(inicio_Lista);
      }
      if (inicio_Lista==NULL && listaAtendentesOcupados == NULL ){  //Condição de paragem da simulação
        simulacao_terminada = 1;
      }
    }while(!simulacao_terminada);
    return 0;
}

