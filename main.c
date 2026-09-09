#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define MAX_FUNC 500
#define MAX_PROJ 2000
#define TAMANHO_HASH 701

typedef struct {
    int functional;
    char nome[100];
    float salario;
} Funcionario;

typedef struct {
    char nome[100];
    char data_inicio[11];
    char data_termino[11];
    int tempo_estimado;
    float valor_estimado;
    int resp_funcional;
} Projeto;

typedef struct {
    int id_funcionario;    
    char email[100];        
    int ocupado;            
} ElementoHash;

void relatorio_maiores_salarios(Funcionario f_vet[], int f_total);
void relatorio_grandes_projetos(Projeto p_vet[], int p_total);
void relatorio_projetos_atrasados(Projeto p_vet[], int p_total);
void cadastrar_gerente_teclado(ElementoHash tabela_hash[]);

int inserir_funcionario(Funcionario vet[], int *total, int func_novo, char nome_novo[], float salario_novo) {
    if (*total >= MAX_FUNC) {
        printf("Erro: Limite maximo de funcionarios atingido!\n");
        return 0;
    }

    for (int idx = 0; idx < *total; idx++) {
        if (vet[idx].functional == func_novo) {
            printf("Erro: Numero funcional %d ja esta cadastrado! Ignorando linha.\n", func_novo);
            return 0;
        }
    }

    int i = *total - 1;
    while (i >= 0 && vet[i].functional > func_novo) {
        vet[i + 1] = vet[i];
        i--;
    }

    vet[i + 1].functional = func_novo;

    int k = 0;
    while(nome_novo[k] != '\0' && k < 99) {
        vet[i + 1].nome[k] = nome_novo[k];
        k++;
    }
    vet[i + 1].nome[k] = '\0';

    vet[i + 1].salario = salario_novo;

    (*total)++;
    return 1;
}

void carregar_funcionarios_arquivo(Funcionario vet[], int *total) {
    FILE *arquivo = fopen("funcionarios.txt", "r");
    if (arquivo == NULL) {
        printf("Aviso: Arquivo 'funcionarios.txt' nao encontrado. Iniciando vetor vazio.\n");
        return;
    }

    int id;
    char nome[100];
    float salario;

    while (fscanf(arquivo, "%d;%[^;];%f\n", &id, nome, &salario) != EOF) {
        inserir_funcionario(vet, total, id, nome, salario);
    }

    fclose(arquivo);
    printf("Dados carregados do arquivo com sucesso!\n");
}

void listar_funcionarios(Funcionario vet[], int total) {
    if (total == 0) {
        printf("Nenhum funcionario cadastrado.\n");
        return;
    }
    printf("\n=== LISTA DE FUNCIONARIOS (ORDENADA POR ID) ===\n");
    for (int i = 0; i < total; i++) {
        printf("Funcional: %d | Nome: %s | Salario: R$ %.2f\n", vet[i].functional, vet[i].nome, vet[i].salario);
    }
    printf("===============================================\n");
}

int comparar_strings(char nome1[], char nome2[]) {
    int i = 0;
    while (nome1[i] != '\0' && nome2[i] != '\0') {
        if (nome1[i] != nome2[i]) {
            return nome1[i] - nome2[i];
        }
        i++;
    }
    return nome1[i] - nome2[i];
}

int inserir_projeto(Projeto vet[], int *total, char nome_novo[], char d_ini[], char d_fim[], int tempo, float valor, int resp) {
    if (*total >= MAX_PROJ) {
        printf("Erro: Limite maximo de projetos atingido!\n");
        return 0;
    }

    for (int idx = 0; idx < *total; idx++) {
        if (comparar_strings(vet[idx].nome, nome_novo) == 0) {
            printf("Erro: O projeto '%s' ja esta cadastrado! Ignorando linha.\n", nome_novo);
            return 0;
        }
    }

    int i = *total - 1;
    while (i >= 0 && comparar_strings(vet[i].nome, nome_novo) > 0) {
        vet[i + 1] = vet[i];
        i--;
    }

    int k;

    for(k = 0; nome_novo[k] != '\0'; k++) vet[i + 1].nome[k] = nome_novo[k];
    vet[i + 1].nome[k] = '\0';

    for(k = 0; d_ini[k] != '\0'; k++) vet[i + 1].data_inicio[k] = d_ini[k];
    vet[i + 1].data_inicio[k] = '\0';

    for(k = 0; d_fim[k] != '\0'; k++) vet[i + 1].data_termino[k] = d_fim[k];
    vet[i + 1].data_termino[k] = '\0';

    vet[i + 1].tempo_estimado = tempo;
    vet[i + 1].valor_estimado = valor;
    vet[i + 1].resp_funcional = resp;

    (*total)++;
    return 1;
}

void carregar_projetos_arquivo(Projeto vet[], int *total) {
    FILE *arquivo = fopen("projetos.txt", "r");
    if (arquivo == NULL) {
        printf("Aviso: Arquivo 'projetos.txt' nao encontrado. Iniciando vetor vazio.\n");
        return;
    }

    char nome[100], d_ini[11], d_fim[11];
    int tempo, resp;
    float valor;

    while (fscanf(arquivo, "%[^;];%[^;];%[^;];%d;%f;%d\n", nome, d_ini, d_fim, &tempo, &valor, &resp) != EOF) {
        inserir_projeto(vet, total, nome, d_ini, d_fim, tempo, valor, resp);
    }

    fclose(arquivo);
    printf("Dados dos projetos carregados com sucesso!\n");
}

void listar_projetos(Projeto vet[], int total) {
    if (total == 0) {
        printf("Nenhum projeto cadastrado.\n");
        return;
    }
    printf("\n=== LISTA DE PROJETOS (ORDENADA POR NOME) ===\n");
    for (int i = 0; i < total; i++) {
        printf("Projeto: %s | Inicio: %s | Termino: %s | Tempo: %d meses | Valor: R$ %.2f | Resp. ID: %d\n",
               vet[i].nome, vet[i].data_inicio, vet[i].data_termino, vet[i].tempo_estimado, vet[i].valor_estimado, vet[i].resp_funcional);
    }
    printf("==============================================\n");
}

void menu_funcionarios(Funcionario vet[], int *total) {
    int sub_opcao;
    int id_busca, indice;

    do {
        system("cls");
        printf("--- MENU DE FUNCIONARIOS ---");
        printf("\n1 - Listar Todos");
        printf("\n2 - Buscar por ID (Funcional)");
        printf("\n3 - Inserir Novo (Teclado)");
        printf("\n4 - Alterar Salario");
        printf("\n5 - Remover Funcionario");
        printf("\n0 - Voltar ao Menu Principal");
        printf("\nEscolha: ");
        scanf("%d", &sub_opcao);

        switch(sub_opcao) {
            case 1:
                system("cls");
                listar_funcionarios(vet, *total);
                printf("\nPressione Enter para voltar...");
                getchar(); getchar();
                break;

            case 2:
                system("cls");
                printf("--- BUSCAR FUNCIONARIO ---");
                printf("\nDigite o Numero Funcional: ");
                scanf("%d", &id_busca);

                indice = busca_binaria_funcionario(vet, *total, id_busca);

                if (indice != -1) {
                    printf("\nFuncionario Encontrado!");
                    printf("\nID: %d | Nome: %s | Salario: R$ %.2f\n", vet[indice].functional, vet[indice].nome, vet[indice].salario);
                } else {
                    printf("\nErro: Funcionario nao encontrado.\n");
                }
                printf("\nPressione Enter para voltar...");
                getchar(); getchar();
                break;

            case 3:
                system("cls");
                printf("--- CADASTRAR NOVO FUNCIONARIO ---\n");
                int novo_id;
                char novo_nome[100];
                float novo_salario;

                printf("Digite o Numero Funcional: ");
                scanf("%d", &novo_id);
                printf("Digite o Nome: ");
                scanf(" %[^\n]", novo_nome);
                printf("Digite o Salario: R$ ");
                scanf("%f", &novo_salario);

                if (inserir_funcionario(vet, total, novo_id, novo_nome, novo_salario)) {
                    printf("\nFuncionario cadastrado com sucesso!\n");
                }
                printf("\nPressione Enter para voltar...");
                getchar(); getchar();
                break;

            case 4:
                system("cls");
                printf("--- ALTERAR FUNCIONARIO ---");
                printf("\nDigite o ID que deseja alterar: ");
                scanf("%d", &id_busca);

                indice = busca_binaria_funcionario(vet, *total, id_busca);

                if (indice != -1) {
                    printf("\nFuncionario: %s | Salario Atual: R$ %.2f", vet[indice].nome, vet[indice].salario);
                    printf("\nDigite o novo salario: ");
                    scanf("%f", &vet[indice].salario);
                    printf("\nSalario alterado com sucesso!\n");
                } else {
                    printf("\nErro: ID nao cadastrado.\n");
                }
                printf("\nPressione Enter para voltar...");
                getchar(); getchar();
                break;

            case 5:
                system("cls");
                printf("--- REMOVER FUNCIONARIO ---");
                printf("\nDigite o ID do funcionario que deseja demitir/remover: ");
                scanf("%d", &id_busca);

                if (remover_funcionario(vet, total, id_busca)) {
                    printf("\nFuncionario removido com sucesso do sistema!\n");
                } else {
                    printf("\nErro: Nao foi possivel remover. ID nao encontrado.\n");
                }
                printf("\nPressione Enter para voltar...");
                getchar(); getchar();
                break;

            case 0:
                break;
            default:
                printf("\nOpcao invalida! Pressione Enter...");
                getchar(); getchar();
        }
    } while(sub_opcao != 0);
}

void menu_projetos(Projeto vet[], int *total) {
    int sub_opcao;
    char nome_busca[100];
    int indice;
    
    do {
        system("cls");
        printf("--- MENU DE PROJETOS ---");
        printf("\n1 - Listar Todos");
        printf("\n2 - Buscar por Nome");
        printf("\n3 - Inserir Novo (Teclado)");
        printf("\n4 - Alterar Dados (Tempo/Valor)");
        printf("\n5 - Remover Projeto");
        printf("\n0 - Voltar ao Menu Principal");
        printf("\nEscolha: ");
        scanf("%d", &sub_opcao);

        switch(sub_opcao) {
            case 1:
                system("cls");
                listar_projetos(vet, *total);
                printf("\nPressione Enter para voltar...");
                getchar(); getchar();
                break;
                
            case 2:
                system("cls");
                printf("--- BUSCAR PROJETO ---");
                printf("\nDigite o Nome do Projeto: ");
                scanf(" %[^\n]", nome_busca);
                
                indice = busca_binaria_projeto(vet, *total, nome_busca);
                
                if (indice != -1) {
                    printf("\nProjeto Encontrado!");
                    printf("\nNome: %s", vet[indice].nome);
                    printf("\nInicio: %s | Termino: %s", vet[indice].data_inicio, vet[indice].data_termino);
                    printf("\nTempo Estimado: %d meses | Valor: R$ %.2f", vet[indice].tempo_estimado, vet[indice].valor_estimado);
                    printf("\nID do Responsavel: %d\n", vet[indice].resp_funcional);
                } else {
                    printf("\nErro: Projeto nao encontrado.\n");
                }
                printf("\nPressione Enter para voltar...");
                getchar(); getchar();
                break;

            case 3:
                system("cls");
                printf("--- CADASTRAR NOVO PROJETO ---\n");
                char n_nome[100], n_ini[11], n_fim[11];
                int n_tempo, n_resp;
                float n_valor;

                printf("Nome do Projeto: ");
                scanf(" %[^\n]", n_nome);
                printf("Data de Inicio (DD/MM/AAAA): ");
                scanf(" %[^\n]", n_ini);
                printf("Data de Termino (DD/MM/AAAA): ");
                scanf(" %[^\n]", n_fim);
                printf("Tempo Estimado (meses): ");
                scanf("%d", &n_tempo);
                printf("Valor Estimado: R$ ");
                scanf("%f", &n_valor);
                printf("Numero Funcional do Responsavel: ");
                scanf("%d", &n_resp);

                if (inserir_projeto(vet, total, n_nome, n_ini, n_fim, n_tempo, n_valor, n_resp)) {
                    printf("\nProjeto cadastrado com sucesso e ordenado alfabeticamente!\n");
                }
                printf("\nPressione Enter para voltar...");
                getchar(); getchar();
                break;

            case 4:
                system("cls");
                printf("--- ALTERAR PROJETO ---");
                printf("\nDigite o Nome do projeto que deseja alterar: ");
                scanf(" %[^\n]", nome_busca);
                
                indice = busca_binaria_projeto(vet, *total, nome_busca);
                
                if (indice != -1) {
                    printf("\nProjeto: %s", vet[indice].nome);
                    printf("\nDigite o novo Tempo Estimado (meses): ");
                    scanf("%d", &vet[indice].tempo_estimado);
                    printf("Digite o novo Valor Estimado: R$ ");
                    scanf("%f", &vet[indice].valor_estimado);
                    printf("\nDados do projeto alterados com sucesso!\n");
                } else {
                    printf("\nErro: Projeto nao cadastrado.\n");
                }
                printf("\nPressione Enter para voltar...");
                getchar(); getchar();
                break;

            case 5:
                system("cls");
                printf("--- REMOVER PROJETO ---");
                printf("\nDigite o Nome do projeto que deseja excluir: ");
                scanf(" %[^\n]", nome_busca);
                
                if (remover_projeto(vet, total, nome_busca)) {
                    printf("\nProjeto removido com sucesso do sistema!\n");
                } else {
                    printf("\nErro: Nao foi possivel remover. Projeto nao encontrado.\n");
                }
                printf("\nPressione Enter para voltar...");
                getchar(); getchar();
                break;

            case 0:
                break;
            default:
                printf("\nOpcao invalida! Pressione Enter...");
                getchar(); getchar();
        }
    } while(sub_opcao != 0);
}

void menu_relatorios(Funcionario f_vet[], int f_total, Projeto p_vet[], int p_total, ElementoHash tabela_hash[]) {
    int opcao_rel;
    do {
        system("cls");
        printf("--- CONSULTAS E RELATORIOS ---");
        printf("\n1 - Listar Maiores Salarios (> R$ 10.000,00)");
        printf("\n2 - Grandes Projetos por Valor (> R$ 500.000,00)");
        printf("\n3 - Projetos Atrasados por Tempo de Atraso");
        printf("\n4 - Consultar Projetos e E-mails dos Gerentes (Hash)");
        printf("\n5 - Cadastrar/Mapear E-mail de um Gerente (Teclado)");
        printf("\n0 - Voltar ao Menu Principal");
        printf("\nEscolha: ");
        scanf("%d", &opcao_rel);

        switch(opcao_rel) {
            case 1:
                system("cls");
                relatorio_maiores_salarios(f_vet, f_total);
                printf("\nPressione Enter para voltar...");
                getchar(); getchar();
                break;
                
            case 2:
                system("cls");
                relatorio_grandes_projetos(p_vet, p_total);
                printf("\nPressione Enter para voltar...");
                getchar(); getchar();
                break;
                
            case 3:
                system("cls");
                relatorio_projetos_atrasados(p_vet, p_total);
                printf("\nPressione Enter para voltar...");
                getchar(); getchar();
                break;
                
            case 4:
                system("cls");
                relatorio_gerentes_emails(p_vet, p_total, tabela_hash);
                printf("\nPressione Enter para voltar...");
                getchar(); getchar();
                break;

            case 5: 
                system("cls");
                cadastrar_gerente_teclado(tabela_hash);
                printf("\nPressione Enter para voltar...");
                getchar(); getchar();
                break;
                
            case 0:
                break;
            default:
                printf("\nOpcao invalida! Pressione Enter...");
                getchar(); getchar();
        }
    } while(opcao_rel != 0);
}

int busca_binaria_funcionario(Funcionario vet[], int total, int func_procurado) {
    int inicio = 0;
    int fim = total - 1;
    int meio;

    while (inicio <= fim) {
        meio = (inicio + fim) / 2;
        if (vet[meio].functional == func_procurado) {
            return meio;
        }
        else if (vet[meio].functional > func_procurado) {
            fim = meio - 1;
        }
        else {
            inicio = meio + 1;
        }
    }
    return -1;
}

int busca_binaria_projeto(Projeto vet[], int total, char nome_procurado[]) {
    int inicio = 0;
    int fim = total - 1;
    int meio;

    while (inicio <= fim) {
        meio = (inicio + fim) / 2;

        int comp = comparar_strings(vet[meio].nome, nome_procurado);

        if (comp == 0) {
            return meio; 
        }
        else if (comp > 0) {
            fim = meio - 1; 
        }
        else {
            inicio = meio + 1; 
        }
    }
    return -1; 
}

int remover_funcionario(Funcionario vet[], int *total, int id_remover) {
    int indice = busca_binaria_funcionario(vet, *total, id_remover);

    if (indice == -1) {
        return 0;
    }

    for (int i = indice; i < (*total) - 1; i++) {
        vet[i] = vet[i + 1];
    }

    (*total)--;
    return 1;
}

int remover_projeto(Projeto vet[], int *total, char nome_remover[]) {
    int indice = busca_binaria_projeto(vet, *total, nome_remover);
    
    if (indice == -1) {
        return 0; 
    }
    
    for (int i = indice; i < (*total) - 1; i++) {
        vet[i] = vet[i + 1];
    }
    
    (*total)--;
    return 1;
}

void salvar_funcionarios_arquivo(Funcionario vet[], int total) {
    FILE *arquivo = fopen("funcionarios.txt", "w");
    if (arquivo == NULL) {
        printf("Erro ao abrir o arquivo para salvar os dados!\n");
        return;
    }

    for (int i = 0; i < total; i++) {
        fprintf(arquivo, "%d;%s;%.2f\n", vet[i].functional, vet[i].nome, vet[i].salario);
    }

    fclose(arquivo);
    printf("Dados de funcionarios salvos em 'funcionarios.txt' com sucesso!\n");
}

void salvar_projetos_arquivo(Projeto vet[], int total) {
    FILE *arquivo = fopen("projetos.txt", "w");
    if (arquivo == NULL) {
        printf("Erro ao abrir o arquivo para salvar os projetos!\n");
        return;
    }

    for (int i = 0; i < total; i++) {
        fprintf(arquivo, "%s;%s;%s;%d;%.2f;%d\n", 
                vet[i].nome, vet[i].data_inicio, vet[i].data_termino, 
                vet[i].tempo_estimado, vet[i].valor_estimado, vet[i].resp_funcional);
    }

    fclose(arquivo);
    printf("Dados de projetos salvos em 'projetos.txt' com sucesso!\n");
}

void bubble_sort_salario_decrescente(Funcionario vet[], int total) {
    int i, j;
    Funcionario aux;
    
    for (i = 0; i < total - 1; i++) {
        for (j = 0; j < total - i - 1; j++) {
            if (vet[j].salario < vet[j + 1].salario) {
                aux = vet[j];
                vet[j] = vet[j + 1];
                vet[j + 1] = aux;
            }
        }
    }
}

void relatorio_maiores_salarios(Funcionario f_vet[], int f_total) {
    Funcionario filtrados[MAX_FUNC];
    int t_filtrados = 0;

    for (int i = 0; i < f_total; i++) {
        if (f_vet[i].salario > 10000.0) {
            filtrados[t_filtrados] = f_vet[i];
            t_filtrados++;
        }
    }

    if (t_filtrados == 0) {
        printf("\nNenhum funcionario recebe acima de R$ 10.000,00.\n");
        return;
    }

    bubble_sort_salario_decrescente(filtrados, t_filtrados);

    printf("\n=== MAIORES SALARIOS DA EMPRESA (> R$ 10.000,00) ===\n");
    for (int i = 0; i < t_filtrados; i++) {
        printf("%dº - Nome: %s | Salario: R$ %.2f | ID: %d\n", i + 1, filtrados[i].nome, filtrados[i].salario, filtrados[i].functional);
    }
    printf("====================================================\n");
}

int particionar_projetos_valor(Projeto vet[], int inicio, int fim) {
    // Escolhemos o valor do último elemento como pivô
    float pivo = vet[fim].valor_estimado;
    int i = (inicio - 1);
    Projeto aux;

    for (int j = inicio; j < fim; j++) {
        if (vet[j].valor_estimado > pivo) {
            i++;
            aux = vet[i];
            vet[i] = vet[j];
            vet[j] = aux;
        }
    }
    aux = vet[i + 1];
    vet[i + 1] = vet[fim];
    vet[fim] = aux;

    return (i + 1);
}

void quick_sort_projetos_decrescente(Projeto vet[], int inicio, int fim) {
    if (inicio < fim) {
        int pi = particionar_projetos_valor(vet, inicio, fim);

        quick_sort_projetos_decrescente(vet, inicio, pi - 1);
        quick_sort_projetos_decrescente(vet, pi + 1, fim);
    }
}

void relatorio_grandes_projetos(Projeto p_vet[], int p_total) {
    Projeto filtrados[MAX_PROJ];
    int t_filtrados = 0;

    for (int i = 0; i < p_total; i++) {
        if (p_vet[i].valor_estimado > 500000.0) {
            filtrados[t_filtrados] = p_vet[i];
            t_filtrados++;
        }
    }

    if (t_filtrados == 0) {
        printf("\nNenhum projeto cadastrado possui valor acima de R$ 500.000,00.\n");
        return;
    }

    quick_sort_projetos_decrescente(filtrados, 0, t_filtrados - 1);

    printf("\n=== GRANDES PROJETOS DA EMPRESA (> R$ 500.000,00) ===\n");
    for (int i = 0; i < t_filtrados; i++) {
        printf("%dº - Projeto: %s | Valor: R$ %.2f | Tempo: %d meses | Resp. ID: %d\n", 
               i + 1, filtrados[i].nome, filtrados[i].valor_estimado, filtrados[i].tempo_estimado, filtrados[i].resp_funcional);
    }
    printf("======================================================\n");
}

int converter_data_para_dias(char data_str[]) {
    int dia, mes, ano;
    sscanf(data_str, "%d/%d/%d", &dia, &mes, &ano);
    
    return (ano * 365) + (mes * 30) + dia;
}

void insertion_sort_projetos_atrasados(Projeto vet[], int atraso_dias[], int total) {
    int i, j, chave_atraso;
    Projeto chave_proj;

    for (i = 1; i < total; i++) {
        chave_atraso = atraso_dias[i];
        chave_proj = vet[i];
        j = i - 1;

        while (j >= 0 && atraso_dias[j] < chave_atraso) {
            atraso_dias[j + 1] = atraso_dias[j];
            vet[j + 1] = vet[j];
            j--;
        }
        atraso_dias[j + 1] = chave_atraso;
        vet[j + 1] = chave_proj;
    }
}

void relatorio_projetos_atrasados(Projeto p_vet[], int p_total) {
    Projeto filtrados[MAX_PROJ];
    int atraso_dias[MAX_PROJ];
    int t_filtrados = 0;

    time_t t = time(NULL);
    struct tm tm_atual = *localtime(&t);

    int dia_atual = tm_atual.tm_mday;
    int mes_atual = tm_atual.tm_mon + 1;     
    int ano_atual = tm_atual.tm_year + 1900; 

    int dias_atual = (ano_atual * 365) + (mes_atual * 30) + dia_atual;

    for (int i = 0; i < p_total; i++) {
        int dias_termino = converter_data_para_dias(p_vet[i].data_termino);
        
        if (dias_atual > dias_termino) {
            filtrados[t_filtrados] = p_vet[i];
            atraso_dias[t_filtrados] = dias_atual - dias_termino;
            t_filtrados++;
        }
    }

    if (t_filtrados == 0) {
        printf("\nParabens! Nenhum projeto esta atrasado em relacao a data de hoje (%02d/%02d/%04d).\n", 
               dia_atual, mes_atual, ano_atual);
        return;
    }

    insertion_sort_projetos_atrasados(filtrados, atraso_dias, t_filtrados);

    printf("\n=== RELATORIO DE PROJETOS ATRASADOS (Data Atual: %02d/%02d/%04d) ===\n", 
           dia_atual, mes_atual, ano_atual);
           
    for (int i = 0; i < t_filtrados; i++) {
        int meses_atraso = atraso_dias[i] / 30;
        int resto_dias = atraso_dias[i] % 30;

        printf("%dº - Projeto: %s | Termino: %s | Atraso: %d meses e %d dias (%d dias total)\n", 
               i + 1, filtrados[i].nome, filtrados[i].data_termino, meses_atraso, resto_dias, atraso_dias[i]);
    }
    printf("==========================================================================\n");
}

void inicializar_tabela_hash(ElementoHash tabela[]) {
    for (int i = 0; i < TAMANHO_HASH; i++) {
        tabela[i].ocupado = 0;
        tabela[i].id_funcionario = -1;
    }
}

int funcao_hash(int chave) {
    return chave % TAMANHO_HASH;
}

void inserir_hash(ElementoHash tabela[], int id, char email[]) {
    int posicao = funcao_hash(id);
    int tentativa = 0;

    while (tabela[posicao].ocupado == 1 && tentativa < TAMANHO_HASH) {
        if (tabela[posicao].id_funcionario == id) {
            break; 
        }
        posicao = (posicao + 1) % TAMANHO_HASH;
        tentativa++;
    }

    if (tentativa >= TAMANHO_HASH) {
        printf("Erro: Tabela Hash cheia! Nao foi possivel inserir o e-mail.\n");
        return;
    }

    tabela[posicao].id_funcionario = id;
    tabela[posicao].ocupado = 1;
    
    int k;
    for (k = 0; email[k] != '\0'; k++) tabela[posicao].email[k] = email[k];
    tabela[posicao].email[k] = '\0';
}

int buscar_hash_email(ElementoHash tabela[], int id_procurado, char email_retorno[]) {
    int posicao = funcao_hash(id_procurado);
    int tentativa = 0;

    while (tabela[posicao].ocupado != 0 && tentativa < TAMANHO_HASH) {
        if (tabela[posicao].id_funcionario == id_procurado) {
            int k;
            for (k = 0; tabela[posicao].email[k] != '\0'; k++) email_retorno[k] = tabela[posicao].email[k];
            email_retorno[k] = '\0';
            return posicao; 
        }
        posicao = (posicao + 1) % TAMANHO_HASH;
        tentativa++;
    }
    return -1;
}

void relatorio_gerentes_emails(Projeto p_vet[], int p_total, ElementoHash tabela_hash[]) {
    printf("\n=== CONSULTA RAPIDA: PROJETOS E E-MAILS DOS GERENTES ===\n");
    if (p_total == 0) {
        printf("Nenhum projeto cadastrado.\n");
        return;
    }

    char email_encontrado[100];

    for (int i = 0; i < p_total; i++) {
        int id_resp = p_vet[i].resp_funcional;
        
        int achou = buscar_hash_email(tabela_hash, id_resp, email_encontrado);

        printf("Projeto: %-20s | Gerente ID: %-3d | ", p_vet[i].nome, id_resp);
        if (achou != -1) {
            printf("E-mail: %s\n", email_encontrado);
        } else {
            printf("E-mail: [Nenhum e-mail mapeado para este ID]\n");
        }
    }
    printf("=========================================================\n");
}

void carregar_gerentes_arquivo(ElementoHash tabela[]) {
    FILE *arquivo = fopen("gerentes.txt", "r");
    if (arquivo == NULL) {
        printf("Aviso: Arquivo 'gerentes.txt' nao encontrado. Iniciando hash de e-mails vazia.\n");
        return;
    }

    int id;
    char email[100];

    while (fscanf(arquivo, "%d;%[^\n]\n", &id, email) != EOF) {
        inserir_hash(tabela, id, email);
    }

    fclose(arquivo);
    printf("Dados dos gerentes (e-mails) carregados com sucesso!\n");
}

void salvar_gerentes_arquivo(ElementoHash tabela[]) {
    FILE *arquivo = fopen("gerentes.txt", "w");
    if (arquivo == NULL) {
        printf("Erro ao abrir o arquivo para salvar os e-mails dos gerentes!\n");
        return;
    }

    for (int i = 0; i < TAMANHO_HASH; i++) {
        if (tabela[i].ocupado == 1) {
            fprintf(arquivo, "%d;%s\n", tabela[i].id_funcionario, tabela[i].email);
        }
    }

    fclose(arquivo);
    printf("Dados de e-mails salvos em 'gerentes.txt' com sucesso!\n");
}

void cadastrar_gerente_teclado(ElementoHash tabela_hash[]) {
    int id;
    char email[100];

    printf("--- CADASTRAR E-MAIL DE GERENTE ---\n");
    printf("Digite o Numero Funcional (ID) do Gerente: ");
    scanf("%d", &id);
    
    getchar(); 
    
    printf("Digite o E-mail do Gerente: ");
    scanf(" %[^\n]", email);

    inserir_hash(tabela_hash, id, email);

    printf("\nE-mail associado ao ID %d com sucesso na Tabela Hash!\n", id);
    printf("(Lembre-se: os dados serao salvos no arquivo 'gerentes.txt' ao fechar o sistema)\n");
}

int main() {
    Funcionario num[MAX_FUNC];
    Projeto nome[MAX_PROJ];
    ElementoHash tabela_emails[TAMANHO_HASH];

    int total_funcionarios = 0;
    int total_projetos = 0;

    inicializar_tabela_hash(tabela_emails);
    carregar_gerentes_arquivo(tabela_emails);
    carregar_funcionarios_arquivo(num, &total_funcionarios);
    carregar_projetos_arquivo(nome, &total_projetos);

    int opcao;

  do {
        system("cls");
        printf("=================================");
        printf("\n    SISTEMA DE GESTAO - IFSP     ");
        printf("\n=================================");
        printf("\n1 - Gerenciar Funcionarios");
        printf("\n2 - Gerenciar Projetos");
        printf("\n3 - Consultar Relatorios e Estatisticas");
        printf("\n0 - Sair do Sistema");
        printf("\nEscolha uma opcao: ");
        scanf("%d", &opcao);

       switch(opcao) {
            case 1:
                menu_funcionarios(num, &total_funcionarios);
                break;
            case 2:
                menu_projetos(nome, &total_projetos);
                break;
            case 3:
                menu_relatorios(num, total_funcionarios, nome, total_projetos, tabela_emails);
                break;
            case 0:
                system("cls");
                printf("\nSalvando dados nos arquivos antes de fechar...\n");

                salvar_funcionarios_arquivo(num, total_funcionarios);
                salvar_projetos_arquivo(nome, total_projetos);
                salvar_gerentes_arquivo(tabela_emails);

                printf("\nSaindo do sistema... Ate logo!\n");
                printf("\nPressione Enter para fechar a janela...");
                getchar(); getchar();
                break;
            default:
                printf("\nOpcao invalida! Pressione Enter...");
                getchar(); getchar();
        }
    } while(opcao != 0);

    return 0;
}
