#include "cabecalho.h"

void limparBuffer() {
    int c;
    while ((c = getchar()) != '\n' && c != EOF);
}
/*IMPLEMENTAÇAO DA lista dos clientes */
Clientes *Criar_Node_Cliente(){
    Clientes *novo_cliente;
    novo_cliente = (Clientes*)malloc(sizeof(Clientes));

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

void liberar_lista(Clientes *cabecalista) {
    Clientes *atual = cabecalista;
    while (atual != NULL) {
        Clientes *proximo = atual->proximo;
        free(atual);
        atual = proximo;
    }
}

Clientes* Ler_Clientes() {
   char paragem;
    FILE *lista_clientes;
    char ficheiro[20];
    Clientes *cabecalista = NULL;
    printf("Introduza o nome do documento da lista dos clientes: ");
    scanf("%s", ficheiro);

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
        // Se a lista estiver vazia ou tiver apenas um elemento, retorna a lista original
        return cabecalista;
    }
    Clientes *prioridade0_head = NULL;  // Cabeça da lista de prioridade 0
    Clientes *prioridade1_head = NULL;  // Cabeça da lista de prioridade 1
    Clientes *temp = cabecalista;
    Clientes *proximo;

    // Separando os clientes em duas listas baseado na prioridade
    while (temp != NULL) {
        proximo = temp->proximo;  // Salva o próximo nó antes de modificar temp
        
        // Aloca um novo nó para inserir na lista de prioridade
        Clientes *novo_cliente = malloc(sizeof(Clientes));
        if (novo_cliente == NULL) {
            // Tratamento de erro na alocação de memória
            perror("Erro na alocação de memória");
            exit(EXIT_FAILURE);
        }
        strcpy(novo_cliente->Nome, temp->Nome);
        strcpy(novo_cliente->Numero, temp->Numero);
        novo_cliente->Especialidade = temp->Especialidade;
        novo_cliente->Prioridade = temp->Prioridade;
        novo_cliente->proximo = NULL;
        
        // Insere o novo nó na lista apropriada (prioridade 0 ou 1)
        if (temp->Prioridade == 0) {
            // Insere na lista de prioridade 0
            if (prioridade0_head == NULL) {
                prioridade0_head = novo_cliente;
            } else {
                novo_cliente->proximo = prioridade0_head;
                prioridade0_head = novo_cliente;
            }
        } else if (temp->Prioridade == 1) {
            // Insere na lista de prioridade 1
            if (prioridade1_head == NULL) {
                prioridade1_head = novo_cliente;
            } else {
                novo_cliente->proximo = prioridade1_head;
                prioridade1_head = novo_cliente;
            }
        }

        temp = proximo;  // Avança para o próximo nó na lista original
    }

    // Concatenando a lista de prioridade 1 seguida pela lista de prioridade 0
    if (prioridade1_head == NULL) {
        // Se não há clientes com prioridade 1, retorna apenas a lista de prioridade 0
        cabecalista = prioridade0_head;
    } else {
        // Encontrando o último elemento da lista de prioridade 1
        Clientes *ultimoPrioridade1 = prioridade1_head;
        while (ultimoPrioridade1->proximo != NULL) {
            ultimoPrioridade1 = ultimoPrioridade1->proximo;
        }
        // Conectando a lista de prioridade 1 com a lista de prioridade 0
        ultimoPrioridade1->proximo = prioridade0_head;
        cabecalista = prioridade1_head;
    }
      free(temp);
      free(prioridade0_head);
      free(prioridade1_head);
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
Atendente* atribuirAtendentes(Fila_atendentes *fila, Clientes *cabecaLista, Atendente *listaAtendenteOcupado) {
  int clienteEncontrado = 0; // boolean variable
  Clientes *clienteTemp = cabecaLista;
  Atendente *atendenteTemp;
  Atendente *percorrerListaAtendentesOcupados;
  do {
    atendenteTemp = fila->inicio_fila;
    while (!clienteEncontrado || clienteTemp->proximo == NULL) {
      if (clienteTemp->Especialidade == atendenteTemp->Especialidade && clienteTemp->Estado_atendimento == 0) {
        clienteEncontrado = 1;
      }
      if (clienteEncontrado) {
        clienteTemp->atendente_atribuido = atendenteTemp->numero_Funcionario;
        clienteTemp->Estado_atendimento = 1;
        clienteTemp->Tempo_atendimento_atribuido = rand() % 10 + 1;
        if (listaAtendenteOcupado != NULL) {
          atendenteTemp->proximo_atendente = listaAtendenteOcupado;
        }
        listaAtendenteOcupado = atendenteTemp;
        break;
      }
      clienteTemp = clienteTemp->proximo;
    }
    fila->inicio_fila = fila->inicio_fila->proximo_atendente;
    free(atendenteTemp);
    clienteTemp = cabecaLista;
  } while (atendenteTemp != fila->fim_fila);
  printf("Did we actually get here?");
  return listaAtendenteOcupado;
}

void mostrarListaClientesAtendidos(Clientes* cabecalista){
  Clientes *temp = cabecalista;
  char prosseguir;
  while (temp != NULL) {
    if(temp->Estado_atendimento==1){
    printf("Nome:%s, Telefone:%s, Especialidade:%d, Prioridade:%d\n", temp->Nome, temp->Numero, temp->Especialidade, temp->Prioridade);
    printf("Atendido pelo atendedor nr : %d \n",temp->atendente_atribuido);
     }
     temp = temp->proximo;
   }
  free(temp);
  printf("Introduza um carater para prosseguir: ");
  limparBuffer();
  system("cls");
}

int isEmpty(Fila_atendentes fila){
return(fila.inicio_fila==NULL);
}

int main() {
  srand(time(NULL));
  Clientes *inicio_Lista = NULL;
  Fila_atendentes filaAtendentesLivres;//Fila dos atendentes livres
  Atendente *listaAtendentesOcupados;//Cabeça da lista com os atendentes ocupados
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
  for (int i = 0; i < num_atendentes; i++) {
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
    do{
      if(!isEmpty(filaAtendentesLivres)){  //Aqui determinamos que existem atendentesLivres, logo ativamos a funçao para atribuir atendentes
        listaAtendentesOcupados = atribuirAtendentes(&filaAtendentesLivres,inicio_Lista,&listaAtendentesOcupados);
      }
      mostrarListaClientesAtendidos(inicio_Lista);


      if (inicio_Lista==NULL && listaAtendentesOcupados == NULL ){  //Condição de paragem da simulação
        simulacao_terminada = 1;
      }
    }while(!simulacao_terminada);

    return 0;
}

