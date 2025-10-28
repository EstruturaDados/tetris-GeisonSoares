#include <stdio.h>
#include <stdlib.h> 
#include <string.h> 
#include <time.h>   
#include <stdbool.h>

// Desafio Tetris Stack
// Nível Mestre: Integração Estratégica (Trocas Diretas)

// --- Constantes Globais ---
#define CAPACIDADE_FILA 5    
#define CAPACIDADE_PILHA 3   
#define NUM_TIPOS_PECA 4

// --- Estrutura de Dados ---
typedef struct {
    char nome; 
    int id;    
} Peca;

// --- Variáveis Globais de Controle ---
// Fila (Circular)
Peca fila[CAPACIDADE_FILA];
int frente = 0;
int traseira = 0;
int tamanho_fila = 0;

// Pilha (Linear)
Peca pilha[CAPACIDADE_PILHA];
int topo = -1; 
int tamanho_pilha = 0;

// Contador Global de ID
int proximo_id_global = 0; 
const char tipos_peca[NUM_TIPOS_PECA] = {'I', 'T', 'O', 'L'};

// ----------------------------------------------------------------------------
// --- Protótipos das Funções ---
// ----------------------------------------------------------------------------
void limparBufferEntrada();
Peca gerarPeca();
bool filaCheia();
bool filaVazia();
void enqueue(Peca nova_peca);
Peca dequeue();
bool pilhaCheia();
bool pilhaVazia();
void push(Peca nova_peca);
Peca pop();
void inicializarFilaPilha();
void mostrarFilaEPilha();

// Ações do Nível Mestre
void trocarPecaAtualAcao();         // Ação 4: Troca Simples (Frente vs Topo)
void trocaMultiplaAcao();           // Ação 5: Troca Múltipla (3 Fila vs 3 Pilha)

// ----------------------------------------------------------------------------
// --- Implementação das Funções Auxiliares e Fila/Pilha (Mantidas) ---
// ----------------------------------------------------------------------------

void limparBufferEntrada() { int c; while ((c = getchar()) != '\n' && c != EOF) {} }
Peca gerarPeca() {
    Peca nova_peca;
    int indice_aleatorio = rand() % NUM_TIPOS_PECA;
    nova_peca.nome = tipos_peca[indice_aleatorio];
    nova_peca.id = proximo_id_global++; 
    return nova_peca;
}
bool filaCheia() { return tamanho_fila == CAPACIDADE_FILA; }
bool filaVazia() { return tamanho_fila == 0; }
void enqueue(Peca nova_peca) {
    if (filaCheia()) { return; } 
    fila[traseira] = nova_peca; traseira = (traseira + 1) % CAPACIDADE_FILA; tamanho_fila++;
}
Peca dequeue() {
    if (filaVazia()) { return (Peca){'X', -1}; } 
    Peca peca_removida = fila[frente]; frente = (frente + 1) % CAPACIDADE_FILA; tamanho_fila--;
    return peca_removida;
}
bool pilhaCheia() { return topo == CAPACIDADE_PILHA - 1; }
bool pilhaVazia() { return topo == -1; }
void push(Peca nova_peca) {
    if (pilhaCheia()) { printf("[ERRO] Pilha de reserva cheia!\n"); return; }
    topo++; pilha[topo] = nova_peca; tamanho_pilha++;
}
Peca pop() {
    if (pilhaVazia()) { return (Peca){'X', -1}; } 
    Peca peca_removida = pilha[topo]; topo--; tamanho_pilha--;
    return peca_removida;
}
void inicializarFilaPilha() {
    for (int i = 0; i < CAPACIDADE_FILA; i++) { enqueue(gerarPeca()); }
    printf("[SETUP] Fila e Pilha inicializadas. Fila cheia (%d), Pilha vazia.\n", CAPACIDADE_FILA);
}
void mostrarFilaEPilha() {
    printf("\n--- Estado Atual ---\n");
    
    // FILA (Mantida: Frente -> Traseira)
    printf("Fila de peças (%d/%d): [ ", tamanho_fila, CAPACIDADE_FILA);
    if (filaVazia()) {
        printf("VAZIA ]\n");
    } else {
        for (int i = 0; i < tamanho_fila; i++) {
            int indice = (frente + i) % CAPACIDADE_FILA; Peca p = fila[indice];
            printf("%c %d ", p.nome, p.id);
            if (i < tamanho_fila - 1) { printf("] [ "); }
        }
        printf("]\n");
    }
    
    // PILHA (Alterada para exibir: Topo -> Base)
    printf("Pilha de reserva (%d/%d): [Topo -> Base]: [ ", tamanho_pilha, CAPACIDADE_PILHA);
    if (pilhaVazia()) {
        printf("VAZIA ]\n");
    } else {
        // O loop agora começa no 'topo' e desce até o índice 0 (Base)
        for (int i = topo; i >= 0; i--) { 
            Peca p = pilha[i];
            printf("%c %d ", p.nome, p.id);
            if (i > 0) { printf("] [ "); } // O separador deve vir depois, se não for a última peça (Base)
        }
        printf("]\n");
    }
    printf("--------------------\n");
}

// ----------------------------------------------------------------------------
// --- Funções de Ações do Nível Mestre (Troca Direta) ---
// ----------------------------------------------------------------------------

/**
 * AÇÃO 4: Trocar peça atual. Substitui a peça da frente da fila com o topo da pilha.
 * (Troca direta de valores nos arrays)
 */
void trocarPecaAtualAcao() {
    // Operadores lógicos e condicionais: Validação
    if (filaVazia() || tamanho_fila < 1) {
        printf("[ERRO] A fila deve ter pelo menos 1 peça para a troca.\n");
        return;
    }
    if (pilhaVazia() || tamanho_pilha < 1) {
        printf("[ERRO] A pilha deve ter pelo menos 1 peça para a troca.\n");
        return;
    }

    // Índices relevantes:
    int indice_frente_fila = frente; // Índice onde a frente da fila está no array
    int indice_topo_pilha = topo;    // Índice onde o topo da pilha está no array

    // Troca dos elementos (Integração de estruturas: troca de valores em estruturas compostas)
    Peca temp = fila[indice_frente_fila];
    fila[indice_frente_fila] = pilha[indice_topo_pilha];
    pilha[indice_topo_pilha] = temp;

    printf("\n[AÇÃO 4] Troca realizada: Peça da frente da fila trocada com o topo da pilha.\n");
}

/**
 * AÇÃO 5: Troca Múltipla. Alterna as três primeiras peças da fila com as três peças da pilha.
 */
void trocaMultiplaAcao() {
    const int N = 3; // Número de peças a serem trocadas

    // Operadores lógicos e condicionais: Validação
    if (tamanho_fila < N) {
        printf("[ERRO] A fila deve ter pelo menos %d peças para a troca múltipla.\n", N);
        return;
    }
    if (tamanho_pilha != CAPACIDADE_PILHA) {
        printf("[ERRO] A pilha deve estar CHEIA (%d peças) para a troca múltipla.\n", CAPACIDADE_PILHA);
        return;
    }
    
    // As peças da fila a serem trocadas são: [frente], [frente+1], [frente+2] (circularmente).
    // As peças da pilha a serem trocadas são: [Base/0], [1], [Topo/2].

    printf("\n[AÇÃO 5] Troca múltipla em bloco de %d peças iniciada...\n", N);

    for (int i = 0; i < N; i++) {
        // 1. Determina o índice circular na fila:
        int indice_fila = (frente + i) % CAPACIDADE_FILA;
        
        // 2. Determina o índice na pilha (Base para Topo):
        int indice_pilha = i; 

        // 3. Troca dos elementos (Integração de estruturas: troca de valores)
        Peca temp = fila[indice_fila];
        fila[indice_fila] = pilha[indice_pilha];
        pilha[indice_pilha] = temp;

        printf("  Troca %d: %s <=> %s\n", i + 1, fila[indice_fila].nome == temp.nome ? "ERRO" : "SUCESSO", pilha[indice_pilha].nome == temp.nome ? "ERRO" : "SUCESSO"); // Visualização simples
    }

    printf("[SUCESSO] Troca múltipla concluída entre os 3 primeiros da fila e os 3 da pilha.\n");
}


// ----------------------------------------------------------------------------
// --- Funções de Ações Básicas (Reutilizadas do Aventureiro) ---
// ----------------------------------------------------------------------------

void jogarPecaAcao() {
    if (filaVazia()) { printf("[ERRO] Fila vazia! Não é possível jogar.\n"); return; }
    Peca jogada = dequeue(); 
    printf("[AÇÃO 1] Peça JOGADA (dequeue): [%c %d]\n", jogada.nome, jogada.id);
    Peca nova = gerarPeca();
    enqueue(nova); 
    printf("[REPOSIÇÃO] Nova peça adicionada ao final da fila: [%c %d]\n", nova.nome, nova.id);
}

void reservarPecaAcao() {
    if (pilhaCheia()) { printf("[ERRO] Pilha de reserva cheia!\n"); return; }
    if (filaVazia()) { printf("[ERRO] Fila vazia! Não há peças para reservar.\n"); return; }
    Peca reservada = dequeue(); 
    push(reservada);            
    printf("[AÇÃO 2] Peça RESERVADA (movida fila -> pilha): [%c %d]\n", reservada.nome, reservada.id);
    Peca nova = gerarPeca();
    enqueue(nova); 
    printf("[REPOSIÇÃO] Nova peça adicionada ao final da fila: [%c %d]\n", nova.nome, nova.id);
}

void usarPecaReservadaAcao() {
    if (pilhaVazia()) { printf("[ALERTA] Pilha de reserva vazia. Nenhuma peça para usar.\n"); return; }
    Peca usada = pop(); 
    printf("[AÇÃO 3] Peça RESERVADA USADA (removida do topo): [%c %d]\n", usada.nome, usada.id);
    printf("[REPOSIÇÃO OMITIDA] Fila de peças futuras mantida inalterada (5/5).\n");
}

// ----------------------------------------------------------------------------
// --- Função Principal (main) ---
// ----------------------------------------------------------------------------

int main() {
    int escolha;
    
    srand(time(NULL)); 

    printf("**********************************************\n");
    printf("* TETRIS STACK - NÍVEL MESTRE (TROCAS DIRETAS) *\n");
    printf("**********************************************\n");

    inicializarFilaPilha();
    mostrarFilaEPilha();
    
    do {
        printf("\n--- Menu de Ações ---\n");
        printf("1 - Jogar peça da frente da fila\n");
        printf("2 - Enviar peça da fila para a pilha de reserva\n");
        printf("3 - Usar peça da pilha de reserva\n");
        printf("4 - Trocar peça da frente da fila com o topo da pilha\n"); // NOVIDADE
        printf("5 - Trocar os 3 primeiros da fila com as 3 peças da pilha\n"); // NOVIDADE
        printf("0 - Sair\n");
        printf("Escolha: ");
        
        if (scanf("%d", &escolha) != 1) {
            printf("[ERRO] Entrada inválida. Tente novamente.\n");
            escolha = -1;
            limparBufferEntrada();
        }

        switch (escolha) {
            case 1: jogarPecaAcao(); break;
            case 2: reservarPecaAcao(); break;
            case 3: usarPecaReservadaAcao(); break;
            case 4: trocarPecaAtualAcao(); break; // Nível Mestre
            case 5: trocaMultiplaAcao(); break;   // Nível Mestre
            case 0: printf("\nSaindo da simulação.\n"); break;
            default: printf("\nOpção desconhecida. Tente novamente.\n");
        }
        
        // Exibir o estado atual após cada ação válida
        if (escolha >= 1 && escolha <= 5) {
             mostrarFilaEPilha();
        }
    } while (escolha != 0);

    return 0;
}