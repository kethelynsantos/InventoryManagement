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
 * Fun��o para criar um novo registro de invent�rio.
 * Solicita ao usu�rio que insira informa��es sobre um equipamento e escreve
 * o registro no arquivo de invent�rio.
 */
void create() {
    FILE *fp; // File Pointer
    int n, i;

    printf("\nQuantos registros quer fazer? ");
    if (scanf("%d", &n) != 1 || n <= 0) {
        printf("Entrada inv�lida.\n");

        // Limpa o buffer de entrada
        int c;
        while ((c = getchar()) != '\n' && c != EOF);

        return;
    }

    // Aloca��o din�mica de mem�ria para o registro
    registro *r = (registro *)calloc(n, sizeof(registro));

    if (r == NULL) {
        printf("Erro na aloca��o de mem�ria.\n");
        return;
    }

    // Abre o arquivo de invent�rio para escrita
    fp = fopen("inventario.txt", "a");

    if (fp == NULL) {
        printf("Erro ao abrir o arquivo.\n");
        free(r); // Libera a mem�ria antes de sair
        return;
    }

    for (i = 0; i < n; i++) {
        printf("\nDigite o nome da sede: ");
        scanf("%19s", r[i].local);

        printf("Digite seu nome: ");
        scanf(" %19[^\n]", r[i].nome);

        printf("Digite o numero do patrimonio: ");
        if (scanf("%d", &r[i].patrimonio) != 1) {
            printf("\nEntrada inv�lida.\n");
            free(r);
            fclose(fp);

            // Limpa o buffer de entrada
            int c;
            while ((c = getchar()) != '\n' && c != EOF);

            return;
        }


        printf("Digite o n�mero de s�rie: ");
        if (scanf("%d", &r[i].numSerial) != 1) {
            printf("\nEntrada inv�lida.\n");
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

        printf("Observa��es: ");
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
 * Fun��o para ler registros de invent�rio do arquivo e list�-los.
 */
void read() {
    registro r1; // Armazena os dados temporariamente
    FILE *fp;
    fp = fopen("inventario.txt", "r");

    if (fp == NULL) {
        printf("Erro ao abrir o arquivo.\n");
        return;
    }

    // Adicionado um cabe�alho para a tabela
    printf("\n---------------------------------------------------------------------------------------------------------------------\n");
    printf("\n%-10s %-20s %-20s %-15s %-15s %-20s %-200s\n",
           "Patrim�nio", "Sede", "Nome", "S�rie",
           "Patrim�nio", "Modelo", "Observa��o");
    printf("---------------------------------------------------------------------------------------------------------------------\n");

    // Reposiciona o ponteiro para o in�cio do arquivo
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
 * Fun��o para adicionar ou atualizar registros de invent�rio.
 */
void update() {
    FILE *fp;
    FILE *tempFile;
    int patrimonio;
    int encontrado = 0;

    printf("\nDigite o n�mero de patrim�nio do registro que deseja atualizar: ");
    if (scanf("%d", &patrimonio) != 1) {
        printf("Entrada inv�lida.\n");

        // Limpa o buffer de entrada
        int c;
        while ((c = getchar()) != '\n' && c != EOF);

        return;
    }

    // Abre o arquivo de invent�rio original para leitura
    fp = fopen("inventario.txt", "r");

    if (fp == NULL) {
        printf("Erro ao abrir o arquivo.\n");
        return;
    }

    // Cria um arquivo tempor�rio para escrever os registros atualizados
    tempFile = fopen("temp_inventario.txt", "w");

    if (tempFile == NULL) {
        printf("Erro ao criar arquivo tempor�rio.\n");
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

            printf("Digite o novo n�mero de s�rie: ");
            if (scanf("%d", &r.numSerial) != 1) {
                printf("Entrada inv�lida.\n");
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

            printf("Digite a nova observa��o: ");
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

    // Renomeia o arquivo tempor�rio para o original
    rename("temp_inventario.txt", "inventario.txt");

    if (!encontrado) {
        printf("\nRegistro com n�mero de patrim�nio %d n�o encontrado.\n", patrimonio);
    } else {
        printf("\nRegistro atualizado com sucesso.\n");
    }
}

/**
 * Fun��o para excluir um registro de invent�rio com base no n�mero de patrim�nio.
 */
void deleteRecord() {
    FILE *fp;
    FILE *tempFile;
    int patrimonio;
    int encontrado = 0;

    printf("\nDigite o n�mero de patrim�nio do registro que deseja excluir: ");
    if (scanf("%d", &patrimonio) != 1) {
        printf("Entrada inv�lida.\n");
        // Limpa o buffer de entrada
        int c;
        while ((c = getchar()) != '\n' && c != EOF);

        return;
    }

    // Abre o arquivo de invent�rio original para leitura
    fp = fopen("inventario.txt", "r");

    if (fp == NULL) {
        printf("Erro ao abrir o arquivo.\n");
        return;
    }

    // Cria um arquivo tempor�rio para escrever os registros atualizados
    tempFile = fopen("temp_inventario.txt", "w");

    if (tempFile == NULL) {
        printf("Erro ao criar arquivo tempor�rio.\n");
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
            printf("Registro com n�mero de patrim�nio %d exclu�do com sucesso!\n", patrimonio);
            continue; // N�o escreve o registro exclu�do no arquivo tempor�rio
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

    // Renomeia o arquivo tempor�rio para o original
    rename("temp_inventario.txt", "inventario.txt");

    if (!encontrado) {
        printf("Registro com n�mero de patrim�nio %d n�o encontrado.\n", patrimonio);
    }
}

/**
 * Fun��o principal responsavel por gerenciar o programa.
 */
int main() {
    int ch;
    do {
        printf("\n0. Sair");
        printf("\n1. Criar");
        printf("\n2. Ler");
        printf("\n3. Editar");
        printf("\n4. Apagar\n");
        printf("\nEscolha uma op��o: ");
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
                printf("Op��o inv�lida. Tente novamente.\n");
                break;
        }

    } while (ch != 0);

    return 0;
}
