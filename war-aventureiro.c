#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

// Definição da Struct conforme solicitado
typedef struct {
    char nome[30];
    char cor[10];
    int tropas;
} Territorio;

// --- Protótipos das Funções ---
void cadastrarTerritorios(Territorio* mapa, int quantidade);
void exibirMapa(Territorio* mapa, int quantidade);
void atacar(Territorio* atacante, Territorio* defensor);
void liberarMemoria(Territorio* mapa);

int main() {
    int n, op_atq, op_def;
    srand(time(NULL)); // Semente para números aleatórios

    printf("Bem-vindo ao Simulador de Guerra!\n");
    printf("Quantos territórios deseja cadastrar? ");
    if (scanf("%d", &n) != 1 || n <= 0) {
        printf("Entrada inválida. Encerrando.\n");
        return 1;
    }

    // Alocação dinâmica de memória usando calloc (limpa a memória com zeros)
    Territorio* mapa = (Territorio*) calloc(n, sizeof(Territorio));

    if (mapa == NULL) {
        printf("Erro ao alocar memória!\n");
        return 1;
    }

    cadastrarTerritorios(mapa, n);

    int jogando = 1;
    while (jogando) {
        exibirMapa(mapa, n);

        printf("\n--- FASE DE ATAQUE ---\n");
        printf("Escolha o índice do território ATACANTE (0 a %d) ou -1 para sair: ", n - 1);
        if (scanf("%d", &op_atq) != 1) {
            printf("Entrada inválida!\n");
            int c; while ((c = getchar()) != '\n' && c != EOF) {}
            continue;
        }

        if (op_atq == -1) break;

        printf("Escolha o índice do território DEFENSOR (0 a %d): ", n - 1);
        if (scanf("%d", &op_def) != 1) {
            printf("Entrada inválida!\n");
            int c; while ((c = getchar()) != '\n' && c != EOF) {}
            continue;
        }

        // Validações básicas
        if (op_atq < 0 || op_atq >= n || op_def < 0 || op_def >= n) {
            printf("Índices inválidos!\n");
            continue;
        }

        // Regra: Não pode atacar a si mesmo ou território da mesma cor
        if (op_atq == op_def) {
            printf("Ataque cancelado: Atacante e defensor são o mesmo território!\n");
            continue;
        }

        if (strcmp(mapa[op_atq].cor, mapa[op_def].cor) == 0) {
            printf("Ataque cancelado: Você não pode atacar seu próprio exército (%s)!\n", mapa[op_atq].cor);
        } else {
            // Passando endereços (ponteiros) para a função
            atacar(&mapa[op_atq], &mapa[op_def]);
        }
    }

    liberarMemoria(mapa);
    printf("Simulação encerrada. Memória liberada.\n");

    return 0;
}

// --- Implementação das Funções ---

void cadastrarTerritorios(Territorio* mapa, int quantidade) {
    for (int i = 0; i < quantidade; i++) {
        printf("\nTerritório [%d]:\n", i);
        printf("Nome: ");
        scanf("%29s", (mapa + i)->nome); // Uso de aritmética de ponteiros
        printf("Cor do Exército: ");
        scanf("%9s", (mapa + i)->cor);
        printf("Quantidade de Tropas: ");
        scanf("%d", &(mapa + i)->tropas);
    }
}

void exibirMapa(Territorio* mapa, int quantidade) {
    printf("\n========= MAPA ATUAL =========\n");
    for (int i = 0; i < quantidade; i++) {
        printf("[%d] %-15s | Cor: %-10s | Tropas: %d\n",
                i, mapa[i].nome, mapa[i].cor, mapa[i].tropas);
    }
    printf("==============================\n");
}

void atacar(Territorio* atacante, Territorio* defensor) {
    if (atacante->tropas < 2) {
        printf("O atacante %s não tem tropas suficientes para atacar (mínimo 2).\n", atacante->nome);
        return;
    }

    // Simulação de dados (1 a 6)
    int dadoAtacante = (rand() % 6) + 1;
    int dadoDefensor = (rand() % 6) + 1;

    printf("\nBatalha: %s (%d) vs %s (%d)\n", atacante->nome, dadoAtacante, defensor->nome, dadoDefensor);

    if (dadoAtacante > dadoDefensor) {
        printf("VITÓRIA DO ATACANTE! %s conquistou %s.\n", atacante->nome, defensor->nome);

        // Atualização dos campos via ponteiros
        strcpy(defensor->cor, atacante->cor);
        defensor->tropas = atacante->tropas / 2;
        atacante->tropas -= defensor->tropas;
    } else {
        printf("DERROTA! O defensor %s resistiu.\n", defensor->nome);
        atacante->tropas -= 1; // Atacante perde uma tropa se perder ou empatar
    }
}

void liberarMemoria(Territorio* mapa) {
    free(mapa);
}
