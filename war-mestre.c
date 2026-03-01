#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#define MAX_NOME 30
#define MAX_COR 15
#define TAM_MISSAO 150

// --- Definição das Structs ---
typedef struct {
    char nome[MAX_NOME];
    char cor[MAX_COR];
    int tropas;
} Territorio;

typedef struct {
    char nome[MAX_NOME];
    char cor[MAX_COR];
    char* missao; // Ponteiro para alocação dinâmica da missão
} Jogador;

// --- Protótipos das Funções ---
void cadastrarTerritorios(Territorio* mapa, int quantidade);
void exibirMapa(Territorio* mapa, int quantidade);
void atacar(Territorio* atacante, Territorio* defensor);
void atribuirMissao(char* destino, char* missoes[], int totalMissoes);
void exibirMissao(char* missao);
int verificarMissao(char* missao, char* corJogador, Territorio* mapa, int tamanho);
void liberarMemoria(Territorio* mapa, Jogador* jogadores, int numJogadores);

int main() {
    int n, numJogadores, op_atq, op_def;
    srand(time(NULL)); // Semente para geração de números aleatórios

    // Vetor de strings com as missões pré-definidas
    char* bancoMissoes[] = {
        "Dominar 3 territorios",
        "Eliminar todas as tropas da cor Vermelho",
        "Eliminar todas as tropas da cor Azul",
        "Acumular um total de 15 tropas",
        "Conquistar todo o mapa" // Todos os territórios
    };
    int totalMissoes = 5;

    printf("=== BEM-VINDO AO SIMULADOR DE GUERRA: DESAFIO FINAL ===\n\n");

    // 1. Configuração do Mapa
    printf("Quantos territorios deseja cadastrar? ");
    if (scanf("%d", &n) != 1 || n <= 0) {
        printf("Entrada invalida. Encerrando.\n");
        return 1;
    }

    // Alocação dinâmica dos territórios
    Territorio* mapa = (Territorio*) calloc(n, sizeof(Territorio));
    if (mapa == NULL) {
        printf("Erro ao alocar memoria para o mapa!\n");
        return 1;
    }
    cadastrarTerritorios(mapa, n);

    // 2. Configuração dos Jogadores
    printf("\nQuantos jogadores vao participar? ");
    scanf("%d", &numJogadores);

    // Alocação dinâmica do vetor de jogadores
    Jogador* jogadores = (Jogador*) calloc(numJogadores, sizeof(Jogador));
    if (jogadores == NULL) {
        printf("Erro ao alocar memoria para jogadores!\n");
        free(mapa);
        return 1;
    }

    printf("\n--- CADASTRO DE JOGADORES ---\n");
    for(int i = 0; i < numJogadores; i++) {
        printf("Jogador [%d] - Nome: ", i + 1);
        scanf("%29s", jogadores[i].nome);
        printf("Jogador [%d] - Cor do Exercito: ", i + 1);
        scanf("%14s", jogadores[i].cor);

        // Alocação dinâmica da string de missão do jogador
        jogadores[i].missao = (char*) malloc(TAM_MISSAO * sizeof(char));
        if (jogadores[i].missao == NULL) {
            printf("Erro ao alocar memoria para missao!\n");
            return 1;
        }

        // Passagem por referência (ponteiro) para atribuição
        atribuirMissao(jogadores[i].missao, bancoMissoes, totalMissoes);

        printf("Ola %s! ", jogadores[i].nome);
        exibirMissao(jogadores[i].missao); // Passagem para exibição
    }

    // 3. Loop Principal do Jogo
    int jogando = 1;
    while (jogando) {
        exibirMapa(mapa, n);

        printf("\n--- FASE DE ATAQUE ---\n");
        printf("Escolha o indice do territorio ATACANTE (0 a %d) ou -1 para sair: ", n - 1);
        if (scanf("%d", &op_atq) != 1) {
            int c; while ((c = getchar()) != '\n' && c != EOF) {}
            continue;
        }

        if (op_atq == -1) break;

        printf("Escolha o indice do territorio DEFENSOR (0 a %d): ", n - 1);
        if (scanf("%d", &op_def) != 1) {
            int c; while ((c = getchar()) != '\n' && c != EOF) {}
            continue;
        }

        // Validações de regras de ataque
        if (op_atq < 0 || op_atq >= n || op_def < 0 || op_def >= n) {
            printf("Indices invalidos!\n");
            continue;
        }
        if (op_atq == op_def) {
            printf("Ataque cancelado: Atacante e defensor sao o mesmo territorio!\n");
            continue;
        }
        // strcasecmp é usado para ignorar diferenças de maiúsculas/minúsculas
        if (strcasecmp(mapa[op_atq].cor, mapa[op_def].cor) == 0) {
            printf("Ataque cancelado: Voce nao pode atacar seu proprio exercito (%s)!\n", mapa[op_atq].cor);
            continue;
        }

        // Executa o ataque via ponteiros
        atacar(&mapa[op_atq], &mapa[op_def]);

        // 4. Verificação Silenciosa de Missões no final do turno
        for(int i = 0; i < numJogadores; i++) {
            if (verificarMissao(jogadores[i].missao, jogadores[i].cor, mapa, n)) {
                printf("\n====================================================\n");
                printf("🏆 VITORIA ESTRATEGICA! 🏆\n");
                printf("O jogador %s (%s) cumpriu sua missao:\n", jogadores[i].nome, jogadores[i].cor);
                printf("Objetivo concluido: %s\n", jogadores[i].missao);
                printf("====================================================\n");
                jogando = 0; // Encerra o jogo
                break;
            }
        }
    }

    // 5. Limpeza de Memória
    liberarMemoria(mapa, jogadores, numJogadores);
    printf("\nSimulacao encerrada. Memoria liberada com sucesso.\n");

    return 0;
}

// --- Implementação das Funções ---

void cadastrarTerritorios(Territorio* mapa, int quantidade) {
    for (int i = 0; i < quantidade; i++) {
        printf("\nTerritorio [%d]:\n", i);
        printf("Nome: ");
        scanf("%29s", (mapa + i)->nome);
        printf("Cor do Exercito: ");
        scanf("%14s", (mapa + i)->cor);
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
        printf("O atacante %s nao tem tropas suficientes para atacar (minimo 2).\n", atacante->nome);
        return;
    }

    int dadoAtacante = (rand() % 6) + 1;
    int dadoDefensor = (rand() % 6) + 1;

    printf("\n⚔️ Batalha: %s (%d) vs %s (%d)\n", atacante->nome, dadoAtacante, defensor->nome, dadoDefensor);

    if (dadoAtacante > dadoDefensor) {
        printf("VITORIA DO ATACANTE! %s conquistou %s.\n", atacante->nome, defensor->nome);
        strcpy(defensor->cor, atacante->cor);
        defensor->tropas = atacante->tropas / 2;
        atacante->tropas -= defensor->tropas;
    } else {
        printf("DERROTA! O defensor %s resistiu.\n", defensor->nome);
        atacante->tropas -= 1;
    }
}

// Sorteia e copia uma missão para a memória alocada do jogador
void atribuirMissao(char* destino, char* missoes[], int totalMissoes) {
    int indiceSorteado = rand() % totalMissoes;
    strcpy(destino, missoes[indiceSorteado]);
}

// Passagem por valor (embora seja ponteiro, usamos apenas para leitura e exibição)
void exibirMissao(char* missao) {
    printf("Sua missao secreta eh: %s\n", missao);
}

// Verifica se a condição de vitória foi atingida
int verificarMissao(char* missao, char* corJogador, Territorio* mapa, int tamanho) {
    int contTerritoriosJogador = 0;
    int contTropasJogador = 0;
    int contVermelho = 0;
    int contAzul = 0;

    // Varredura do mapa atual para colher os dados
    for(int i = 0; i < tamanho; i++) {
        if(strcasecmp(mapa[i].cor, corJogador) == 0) {
            contTerritoriosJogador++;
            contTropasJogador += mapa[i].tropas;
        }
        if(strcasecmp(mapa[i].cor, "Vermelho") == 0) contVermelho++;
        if(strcasecmp(mapa[i].cor, "Azul") == 0) contAzul++;
    }

    // Avaliação da missão específica usando strcmp
    if(strcmp(missao, "Dominar 3 territorios") == 0) {
        return (contTerritoriosJogador >= 3);
    }
    else if(strcmp(missao, "Eliminar todas as tropas da cor Vermelho") == 0) {
        // Vence se a cor do jogador não for vermelho E não sobrar territórios vermelhos
        return (strcasecmp(corJogador, "Vermelho") != 0 && contVermelho == 0);
    }
    else if(strcmp(missao, "Eliminar todas as tropas da cor Azul") == 0) {
        return (strcasecmp(corJogador, "Azul") != 0 && contAzul == 0);
    }
    else if(strcmp(missao, "Acumular um total de 15 tropas") == 0) {
        return (contTropasJogador >= 15);
    }
    else if(strcmp(missao, "Conquistar todo o mapa") == 0) {
        return (contTerritoriosJogador == tamanho);
    }

    return 0; // Missão ainda não cumprida
}

// Função robusta de liberação de memória para evitar Memory Leaks
void liberarMemoria(Territorio* mapa, Jogador* jogadores, int numJogadores) {
    free(mapa); // Libera o vetor de territórios
    for(int i = 0; i < numJogadores; i++) {
        free(jogadores[i].missao); // Libera a string da missão de cada jogador
    }
    free(jogadores); // Libera o vetor de jogadores
}
