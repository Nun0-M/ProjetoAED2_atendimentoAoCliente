#include "cabecalho.h"

void limparBuffer() {
    int c;
    while ((c = getchar()) != '\n' && c != EOF);
}
//---------------------------------------------------------------------------------------------------------------------------------------------------------------
//FunÃ§Ãµes para manipular a lista de clientes
Clientes *criar_cliente() {
  Clientes *novo_cliente = (Clientes*)malloc(sizeof(Clientes));
  if (novo_cliente == NULL) {
    printf("Erro: Falha ao alocar memÃ³ria para novo cliente.\n");
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

void mostrarListaClientes(Clientes* cabecalista){
  Clientes *temp = cabecalista;
  char prosseguir;
  while (temp != NULL) {
    printf("\n|-----Cliente %s-----|\n", temp->Nome);
    printf("Telefone: %s \nEspecialidade: %d \nPrioridade: %d\n", temp->Numero, temp->Especialidade, temp->Prioridade);
    temp = temp->proximo;
  }
  free(temp);
  printf("\n//---------------------//\n", temp->Nome);
  printf("Introduza um carater para prosseguir: ");
  limparBuffer();
  system("cls");
}

//--------------------------------------------------------------------------------------------------------------------------------------------------------------
//FunÃ§Ãµes para manipular a fila de atendentes

void inicializar_fila_atendentes(Fila_atendentes *fila) {
  fila->inicio_fila = NULL; // Inicializa a fila como NULL
  fila->fim_fila = NULL;    // Atribui ao final da fila NULL
}

int isEmpty(Fila_atendentes fila){
return(fila.inicio_fila==NULL);
}

void organizar_atendente(Fila_atendentes *fila, Atendente *atendente) {
  atendente->proximo_atendente = NULL; // Garantir que o prÃ³ximo do novo atendente seja NULL
  if (fila->inicio_fila == NULL) { // Se a fila estiver vazia
    fila->inicio_fila = atendente;
    fila->fim_fila = atendente;
  } else {
    fila->fim_fila->proximo_atendente = atendente; // Definir o prÃ³ximo do Ãºltimo atendente como o novo atendente
    fila->fim_fila = fila->fim_fila->proximo_atendente; // Atualizar o fim da fila para o novo atendente
  }
}

Atendente* criar_atendente() {
  Atendente *novo_atendente = (Atendente*)malloc(sizeof(Atendente));
  if (novo_atendente == NULL) {
    printf("Erro: Falha ao alocar memÃ³ria para novo atendente.\n");
    exit(1);
  }
    return novo_atendente;
}


void dequeue(Fila_atendentes *fila){
	Atendente *temp;
    temp = criar_atendente();
    temp = fila->inicio_fila;
    if(temp == fila->fim_fila){
    	fila->inicio_fila = NULL;
    	fila->fim_fila = NULL;
    	free(temp);
    	return;
	}
    fila->inicio_fila = fila->inicio_fila->proximo_atendente;
    free(temp);
}

void enqueue(Fila_atendentes *fila, Atendente *atendente){
    atendente->proximo_atendente = NULL;
    if(fila->inicio_fila == NULL){
        fila->inicio_fila = atendente;
        fila->fim_fila = atendente;
    }else{
        fila->fim_fila->proximo_atendente = atendente;
        fila->fim_fila = atendente;
    }
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
//FunÃ§Ãµes para manipular a lista de atendentes ocupados

Atendente* introduzir_no(Atendente* lista_atendentes_ocupados, Atendente* atendente_a_inserir) {
	Atendente *copiaAtendente;
	copiaAtendente = (Atendente*)malloc(sizeof(Atendente));
	copiaAtendente->Especialidade = atendente_a_inserir->Especialidade;
	copiaAtendente->numero_Funcionario = atendente_a_inserir->numero_Funcionario;
	copiaAtendente->Tempo_atendimento_atribuido = atendente_a_inserir->Tempo_atendimento_atribuido;
	copiaAtendente->proximo_atendente=NULL;
    if(lista_atendentes_ocupados==NULL){
        return copiaAtendente;
    }else{
        copiaAtendente->proximo_atendente = lista_atendentes_ocupados;
        return copiaAtendente;
    }
}
//-------------------------------------------------------------------------------------------------------------------------------------------------------

Clientes* Ler_Clientes() {
   char paragem;
    FILE *lista_clientes;
    char ficheiro[20];
    Clientes *cabecalista = NULL;
    printf("Introduza o nome do documento da lista dos clientes: ");
    scanf("%19s", ficheiro);
  strcat(ficheiro, ".txt"); //Adicionar o .txt ao nome do ficheiro
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

        // Criar uma cÃ³pia do nÃ³ atual
        Clientes *novoCliente = (Clientes *)malloc(sizeof(Clientes));
        if (novoCliente == NULL) {
            // Tratamento de erro de alocaÃ§Ã£o de memÃ³ria
            // Aqui vocÃª pode inserir um cÃ³digo para lidar com o erro
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


Atendente* atribuirAtendentes(Fila_atendentes *fila, Clientes *cabecaLista, Atendente *listaAtendenteOcupado) {
  Atendente* atendenteTemp = criar_atendente();
  atendenteTemp = fila->inicio_fila;
  while (atendenteTemp != NULL) {
    Clientes* clienteTemp = cabecaLista;
    while (clienteTemp != NULL) {
      if (clienteTemp->Especialidade == atendenteTemp->Especialidade && clienteTemp->Estado_atendimento == 0) {
        printf("cliente encontrado,a atribuir\n");
        clienteTemp->atendente_atribuido = atendenteTemp->numero_Funcionario;
        clienteTemp->Estado_atendimento = 1;
        clienteTemp->Tempo_atendimento_atribuido = rand() % 10 + 1;
        atendenteTemp->Tempo_atendimento_atribuido = clienteTemp->Tempo_atendimento_atribuido;
        listaAtendenteOcupado = introduzir_no(listaAtendenteOcupado, atendenteTemp);
        dequeue(fila);
        if (fila->inicio_fila != NULL) { // Add null check before accessing inicio_fila
          atendenteTemp = fila->inicio_fila;
        } else {
          atendenteTemp = NULL;
        }
        break;
      }
      clienteTemp = clienteTemp->proximo;
    }
    if(clienteTemp == NULL){
    	printf("Nï¿½o foi encontrado cliente para o funcionario disponivel.\n");
    	printf("A remover o atendedor nr %d da fila .\n",atendenteTemp->numero_Funcionario);
    	dequeue(fila);
    	if (fila->inicio_fila != NULL) { // Add null check before accessing inicio_fila
          atendenteTemp = fila->inicio_fila;
        } else {
          atendenteTemp = NULL;
        }
	}
  }
  free(atendenteTemp);
  printf("AtribuiÃ§Ã£o de atendentes terminada\n");
  return listaAtendenteOcupado;
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

void atualizarTempo(Clientes* cabecalista,Atendente* listaAtendentesOcupados){
  Clientes *temp = cabecalista;
  Atendente *atendenteOcupadoTemp;
  atendenteOcupadoTemp = criar_atendente();
  atendenteOcupadoTemp = listaAtendentesOcupados;
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

void enqueue2(Fila_atendentes *fila, Atendente *atendente){
	//Esta funï¿½ï¿½o serve para criar o nï¿½ da fila do atendente que deve apontar para o proximo membro da fila, e nï¿½o da lista
	//Serve maioritariamente para apioar a funï¿½ao abaixo verificarChamadaTerminada que remove os atendentes que jï¿½ terminaram a chamada e adiciona-os a fila de atendentes disponiveis
	Atendente* criarAtendenteDisponivel;
	criarAtendenteDisponivel = (Atendente*)malloc(sizeof(Atendente));
	criarAtendenteDisponivel->Especialidade = atendente->Especialidade;
	criarAtendenteDisponivel->numero_Funcionario=atendente->numero_Funcionario;
	criarAtendenteDisponivel->Tempo_atendimento_atribuido=0;
	criarAtendenteDisponivel->proximo_atendente=NULL;
	    if(fila->inicio_fila == NULL){
        fila->inicio_fila = criarAtendenteDisponivel ;
        fila->fim_fila = criarAtendenteDisponivel;
    }else{
        fila->fim_fila->proximo_atendente = criarAtendenteDisponivel;
        fila->fim_fila = criarAtendenteDisponivel;
    }
}

int verificarChamadaTerminada(Atendente **listaAtendenteOcupado, Fila_atendentes *fila) {
    int numero_atendentes_removidos = 0;
    Atendente *temp = *listaAtendenteOcupado;
    Atendente *anterior = NULL;
    while (temp != NULL) {
        if (temp->Tempo_atendimento_atribuido == 0) {
            // Atendente terminou o atendimento, remover da lista de atendentes ocupados
            if (anterior == NULL) {
                *listaAtendenteOcupado = temp->proximo_atendente;
            } else {
                anterior->proximo_atendente = temp->proximo_atendente;
            }
            // Adicionar o atendente ao final da fila de atendentes livres
            enqueue2(fila,temp);
            // Atualizar o ponteiro temp para o prï¿½ximo atendente
            Atendente *prox = temp->proximo_atendente;
            temp = prox;
            numero_atendentes_removidos++;
        } else {
            anterior = temp;
            temp = temp->proximo_atendente;
        }
    }
    return numero_atendentes_removidos;
}

Clientes* removerClientesTerminados(Clientes* cabecalista) {
    Clientes *temp = cabecalista;
    Clientes *anterior = NULL;
    while (temp != NULL) {
        if (temp->Tempo_atendimento_atribuido == 0 && temp->Estado_atendimento==1) {
            // Remoï¿½ï¿½o do nï¿½ que atende aos requisitos
            if (anterior == NULL) {
                cabecalista = temp->proximo;
            } else {
                anterior->proximo = temp->proximo;
            }
            printf("Cliente %s atendido\n",temp->Nome);
            printf("A remover da lista\n");
            // Liberta a memï¿½ria do nï¿½ removido
            Clientes *prox = temp->proximo;
            free(temp);
            temp = prox;
        } else {
            anterior = temp;
            temp = temp->proximo;
        }
    }
    return cabecalista;
}


int main() {
  int numero_atendentes_concluidos;
  int i;
  srand(time(NULL));
  Clientes *inicio_Lista = NULL;
  Fila_atendentes filaAtendentesLivres;//Fila dos atendentes livres
  Atendente *listaAtendentesOcupados;//CabeÃ§a da lista com os atendentes ocupados
  listaAtendentesOcupados = (Atendente*)malloc(sizeof(Atendente));
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
  printf("Digite o nÃºmero de atendentes na fila: ");
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
// ImpressÃ£o dos detalhes de cada atendente na fila
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
    if(!isEmpty(filaAtendentesLivres)){  //Aqui determinamos que existem atendentesLivres, logo ativamos a funÃ§ao para atribuir atendentes
      listaAtendentesOcupados = atribuirAtendentes(&filaAtendentesLivres,inicio_Lista,listaAtendentesOcupados);
    }
      printf("Lista de clientes em atendimento\n");
      mostrarListaClientesAtendidos(inicio_Lista);
      printf("\nLista de clientes por atender\n");
      mostrarListaClientesPorAtender(inicio_Lista);
      numero_atendentes_concluidos = 0;
      sleep(4);
      atualizarTempo(inicio_Lista,listaAtendentesOcupados);
      numero_atendentes_concluidos = verificarChamadaTerminada(&listaAtendentesOcupados,&filaAtendentesLivres);
      if(numero_atendentes_concluidos>0){
        inicio_Lista = removerClientesTerminados(inicio_Lista);
      }
      if (inicio_Lista==NULL && listaAtendentesOcupados == NULL ){  //CondiÃ§Ã£o de paragem da simulaÃ§Ã£o
        simulacao_terminada = 1;
      }
      //sleep(5); Para mostrar informaï¿½oes antes de atualizar a simulaï¿½ï¿½o
      system("cls");
    }while(!simulacao_terminada);
    return 0;
}
