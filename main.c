#include <stdio.h>
#include <stdlib.h>

// Definindo a estrutura de registro
typedef struct registro {
    int patrimonio;
    char local[20];
    char nome[20];
    int numSerial;
    char modelo[20];
    char observacao[200];
} registro;

/**
 * Função para criar um novo registro de inventário.
 * Solicita ao usuário que insira informações sobre um equipamento e escreve
 * o registro no arquivo de inventário.
 */
void create() {
    FILE *fp; // File Pointer
    int n, i;

    printf("\nQuantos registros quer fazer? ");
    if (scanf("%d", &n) != 1 || n <= 0) {
        printf("Entrada inválida.\n");

        // Limpa o buffer de entrada
        int c;
        while ((c = getchar()) != '\n' && c != EOF);

        return;
    }

    // Alocação dinâmica de memória para o registro
    registro *r = (registro *)calloc(n, sizeof(registro));

    if (r == NULL) {
        printf("Erro na alocação de memória.\n");
        return;
    }

    // Abre o arquivo de inventário para escrita
    fp = fopen("inventario.txt", "a");

    if (fp == NULL) {
        printf("Erro ao abrir o arquivo.\n");
        free(r); // Libera a memória antes de sair
        return;
    }

    for (i = 0; i < n; i++) {
        printf("\nDigite o nome da sede: ");
        scanf("%19s", r[i].local);

        printf("Digite seu nome: ");
        scanf(" %19[^\n]", r[i].nome);

        printf("Digite o numero do patrimonio: ");
        if (scanf("%d", &r[i].patrimonio) != 1) {
            printf("\nEntrada inválida.\n");
            free(r);
            fclose(fp);

            // Limpa o buffer de entrada
            int c;
            while ((c = getchar()) != '\n' && c != EOF);

            return;
        }


        printf("Digite o número de série: ");
        if (scanf("%d", &r[i].numSerial) != 1) {
            printf("\nEntrada inválida.\n");
            free(r);
            fclose(fp);

            // Limpa o buffer de entrada
            int c;
            while ((c = getchar()) != '\n' && c != EOF);

            return;
        }

        // Consuma o caractere de nova linha no buffer de entrada
        getchar();

        printf("Digite o modelo do equipamento: ");
        scanf(" %19[^\n]", r[i].modelo);

        printf("Observações: ");
        scanf(" %199[^\n]", r[i].observacao);
    }

    // Escreve os registros no arquivo inventario
    for (i = 0; i < n; i++) {
        fprintf(fp, "%d;%s;%s;%d;%d;%s;%s\n",
                r[i].patrimonio,
                r[i].local,
                r[i].nome,
                r[i].numSerial,
                r[i].patrimonio,
                r[i].modelo,
                r[i].observacao);
    }

    fclose(fp);
    free(r);
}

/**
 * Função para ler registros de inventário do arquivo e listá-los.
 */
void read() {
    registro r1; // Armazena os dados temporariamente
    FILE *fp;
    fp = fopen("inventario.txt", "r");

    if (fp == NULL) {
        printf("Erro ao abrir o arquivo.\n");
        return;
    }

    // Adicionado um cabeçalho para a tabela
    printf("\n---------------------------------------------------------------------------------------------------------------------\n");
    printf("\n%-10s %-20s %-20s %-15s %-15s %-20s %-200s\n",
           "Patrimônio", "Sede", "Nome", "Série",
           "Patrimônio", "Modelo", "Observação");
    printf("---------------------------------------------------------------------------------------------------------------------\n");

    // Reposiciona o ponteiro para o início do arquivo
    rewind(fp);

    while (fscanf(fp, "%d;%19[^;];%19[^;];%d;%d;%19[^;];%199[^\n]",
                  &r1.patrimonio,
                  r1.local,
                  r1.nome,
                  &r1.numSerial,
                  &r1.patrimonio,
                  r1.modelo,
                  r1.observacao) == 7) {

        // Imprimir os detalhes do registro em formato de tabela
        printf("%-10d %-20s %-20s %-15d %-15d %-20s %-200s\n",
               r1.patrimonio,
               r1.local,
               r1.nome,
               r1.numSerial,
               r1.patrimonio,
               r1.modelo,
               r1.observacao);
    }

    printf("---------------------------------------------------------------------------------------------------------------------\n");

    fclose(fp);
}

/**
 * Função para adicionar ou atualizar registros de inventário.
 */
void update() {
    FILE *fp;
    FILE *tempFile;
    int patrimonio;
    int encontrado = 0;

    printf("\nDigite o número de patrimônio do registro que deseja atualizar: ");
    if (scanf("%d", &patrimonio) != 1) {
        printf("Entrada inválida.\n");

        // Limpa o buffer de entrada
        int c;
        while ((c = getchar()) != '\n' && c != EOF);

        return;
    }

    // Abre o arquivo de inventário original para leitura
    fp = fopen("inventario.txt", "r");

    if (fp == NULL) {
        printf("Erro ao abrir o arquivo.\n");
        return;
    }

    // Cria um arquivo temporário para escrever os registros atualizados
    tempFile = fopen("temp_inventario.txt", "w");

    if (tempFile == NULL) {
        printf("Erro ao criar arquivo temporário.\n");
        fclose(fp);
        return;
    }

    registro r;

    while (fscanf(fp, "%d;%19[^;];%19[^;];%d;%d;%19[^;];%199[^\n]",
                  &r.patrimonio,
                  r.local,
                  r.nome,
                  &r.numSerial,
                  &r.patrimonio,
                  r.modelo,
                  r.observacao) == 7) {
        if (r.patrimonio == patrimonio) {
            encontrado = 1;
            printf("\nDigite o novo nome da sede: ");
            scanf("%19s", r.local);

            printf("Digite o novo nome: ");
            scanf(" %19[^\n]", r.nome);

            printf("Digite o novo número de série: ");
            if (scanf("%d", &r.numSerial) != 1) {
                printf("Entrada inválida.\n");
                fclose(fp);
                fclose(tempFile);
                remove("temp_inventario.txt");

                // Limpa o buffer de entrada
                int c;
                while ((c = getchar()) != '\n' && c != EOF);

                return;
            }

            printf("Digite o novo modelo do equipamento: ");
            scanf(" %19[^\n]", r.modelo);

            printf("Digite a nova observação: ");
            scanf(" %199[^\n]", r.observacao);
        }
        fprintf(tempFile, "%d;%s;%s;%d;%d;%s;%s\n",
                r.patrimonio,
                r.local,
                r.nome,
                r.numSerial,
                r.patrimonio,
                r.modelo,
                r.observacao);
    }

    fclose(fp);
    fclose(tempFile);

    // Remove o arquivo original
    remove("inventario.txt");

    // Renomeia o arquivo temporário para o original
    rename("temp_inventario.txt", "inventario.txt");

    if (!encontrado) {
        printf("\nRegistro com número de patrimônio %d não encontrado.\n", patrimonio);
    } else {
        printf("\nRegistro atualizado com sucesso.\n");
    }
}

/**
 * Função para excluir um registro de inventário com base no número de patrimônio.
 */
void deleteRecord() {
    FILE *fp;
    FILE *tempFile;
    int patrimonio;
    int encontrado = 0;

    printf("\nDigite o número de patrimônio do registro que deseja excluir: ");
    if (scanf("%d", &patrimonio) != 1) {
        printf("Entrada inválida.\n");
        // Limpa o buffer de entrada
        int c;
        while ((c = getchar()) != '\n' && c != EOF);

        return;
    }

    // Abre o arquivo de inventário original para leitura
    fp = fopen("inventario.txt", "r");

    if (fp == NULL) {
        printf("Erro ao abrir o arquivo.\n");
        return;
    }

    // Cria um arquivo temporário para escrever os registros atualizados
    tempFile = fopen("temp_inventario.txt", "w");

    if (tempFile == NULL) {
        printf("Erro ao criar arquivo temporário.\n");
        fclose(fp);
        return;
    }

    registro r;

    while (fscanf(fp, "%d;%19[^;];%19[^;];%d;%d;%19[^;];%199[^\n]",
                  &r.patrimonio,
                  r.local,
                  r.nome,
                  &r.numSerial,
                  &r.patrimonio,
                  r.modelo,
                  r.observacao) == 7) {
        if (r.patrimonio == patrimonio) {
            encontrado = 1;
            printf("Registro com número de patrimônio %d excluído com sucesso!\n", patrimonio);
            continue; // Não escreve o registro excluído no arquivo temporário
        }
        fprintf(tempFile, "%d;%s;%s;%d;%d;%s;%s\n",
                r.patrimonio,
                r.local,
                r.nome,
                r.numSerial,
                r.patrimonio,
                r.modelo,
                r.observacao);
    }

    fclose(fp);
    fclose(tempFile);

    // Remove o arquivo original
    remove("inventario.txt");

    // Renomeia o arquivo temporário para o original
    rename("temp_inventario.txt", "inventario.txt");

    if (!encontrado) {
        printf("Registro com número de patrimônio %d não encontrado.\n", patrimonio);
    }
}

/**
 * Função principal responsavel por gerenciar o programa.
 */
int main() {
    int ch;
    do {
        printf("\n0. Sair");
        printf("\n1. Criar");
        printf("\n2. Ler");
        printf("\n3. Editar");
        printf("\n4. Apagar\n");
        printf("\nEscolha uma opção: ");
        scanf("%d", &ch);

        switch (ch) {
            case 1:
                create();
                break;

            case 2:
                read();
                break;

            case 3:
                update();
                break;

            case 4:
                deleteRecord();
                break;

            case 0:
                printf("Saindo...\n");
                break;

            default:
                printf("Opção inválida. Tente novamente.\n");
                break;
        }

    } while (ch != 0);

    return 0;
}
