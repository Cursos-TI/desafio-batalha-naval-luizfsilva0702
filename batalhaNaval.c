#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>

// Definições de constantes
#define TAMANHO_TABULEIRO 10
#define TAMANHO_NAVIO 3
#define AGUA 0 // Representa água
#define NAVIO 3 // Representa parte de um navio
// Novos valores para representar as áreas de efeito das habilidades:
#define HABILIDADE_NAO_AFETADA 0 // Usado nas matrizes de habilidade
#define HABILIDADE_AFETADA 1 // Usado nas matrizes de habilidade
#define EFEITO_HABILIDADE 5 // Valor usado no tabuleiro principal para a área afetada

// Dimensão fixa das matrizes de habilidade (ex: 5x5)
#define TAMANHO_HABILIDADE 5 

// --- Protótipos das Funções ---

// Funções do Nível Aventureiro
bool validar_posicao(int linha, int coluna, int tamanho, char orientacao, int tabuleiro[TAMANHO_TABULEIRO][TAMANHO_TABULEIRO]);
void posicionar_navio(int linha_inicio, int coluna_inicio, int tamanho, char orientacao, int tabuleiro[TAMANHO_TABULEIRO][TAMANHO_TABULEIRO]);
void exibir_tabuleiro(int tabuleiro[TAMANHO_TABULEIRO][TAMANHO_TABULEIRO]);

// Novas Funções para o Nível Avançado (Habilidades)
void criar_matriz_cone(int matriz_habilidade[TAMANHO_HABILIDADE][TAMANHO_HABILIDADE]);
void criar_matriz_cruz(int matriz_habilidade[TAMANHO_HABILIDADE][TAMANHO_HABILIDADE]);
void criar_matriz_octaedro(int matriz_habilidade[TAMANHO_HABILIDADE][TAMANHO_HABILIDADE]);
void sobrepor_habilidade(int tabuleiro[TAMANHO_TABULEIRO][TAMANHO_TABULEIRO], int matriz_habilidade[TAMANHO_HABILIDADE][TAMANHO_HABILIDADE], int linha_origem, int coluna_origem);

int main() {
    // 1. Representação do Tabuleiro: Matriz 10x10 inicializada com AGUA (0)
    int tabuleiro[TAMANHO_TABULEIRO][TAMANHO_TABULEIRO];
    
    // Inicializa todas as posições do tabuleiro com 0 (água)
    for (int i = 0; i < TAMANHO_TABULEIRO; i++) {
        for (int j = 0; j < TAMANHO_TABULEIRO; j++) {
            tabuleiro[i][j] = AGUA;
        }
    }

    // --- Configurações dos Quatro Navios (Nível Aventureiro) ---
    
    // Navio 1: Horizontal (L: 2, C: 1)
    int l1 = 2, c1 = 1; char o1 = 'H';
    // Navio 2: Vertical (L: 5, C: 6)
    int l2 = 5, c2 = 6; char o2 = 'V';
    // Navio 3: Diagonal D1 (L+: 0, C+: 0)
    int l3 = 0, c3 = 0; char o3 = '1'; 
    // Navio 4: Diagonal D2 (L+: 7, C-: 9)
    int l4 = 7, c4 = 9; char o4 = '2'; 

    printf("--- Posicionamento dos Navios ---\n");
    // Posiciona os navios (assumindo que as coordenadas não se sobrepõem e são válidas)
    if (validar_posicao(l1, c1, TAMANHO_NAVIO, o1, tabuleiro)) posicionar_navio(l1, c1, TAMANHO_NAVIO, o1, tabuleiro);
    if (validar_posicao(l2, c2, TAMANHO_NAVIO, o2, tabuleiro)) posicionar_navio(l2, c2, TAMANHO_NAVIO, o2, tabuleiro);
    if (validar_posicao(l3, c3, TAMANHO_NAVIO, o3, tabuleiro)) posicionar_navio(l3, c3, TAMANHO_NAVIO, o3, tabuleiro);
    if (validar_posicao(l4, c4, TAMANHO_NAVIO, o4, tabuleiro)) posicionar_navio(l4, c4, TAMANHO_NAVIO, o4, tabuleiro);

    // --- Criação e Sobreposição das Habilidades (Nível Avançado) ---

    int matriz_habilidade[TAMANHO_HABILIDADE][TAMANHO_HABILIDADE];
    
    // Habilidade 1: Cone
    criar_matriz_cone(matriz_habilidade);
    int origem_cone_l = 0; // Ponto de origem no tabuleiro 10x10
    int origem_cone_c = 4;
    sobrepor_habilidade(tabuleiro, matriz_habilidade, origem_cone_l, origem_cone_c);
    printf("✅ Habilidade Cone aplicada a partir de (%d, %d).\n", origem_cone_l, origem_cone_c);

    // Habilidade 2: Cruz
    criar_matriz_cruz(matriz_habilidade);
    int origem_cruz_l = 5; // Ponto de origem no tabuleiro 10x10
    int origem_cruz_c = 2;
    sobrepor_habilidade(tabuleiro, matriz_habilidade, origem_cruz_l, origem_cruz_c);
    printf("✅ Habilidade Cruz aplicada a partir de (%d, %d).\n", origem_cruz_l, origem_cruz_c);
    
    // Habilidade 3: Octaedro
    criar_matriz_octaedro(matriz_habilidade);
    int origem_octa_l = 6; // Ponto de origem no tabuleiro 10x10
    int origem_octa_c = 7;
    sobrepor_habilidade(tabuleiro, matriz_habilidade, origem_octa_l, origem_octa_c);
    printf("✅ Habilidade Octaedro aplicada a partir de (%d, %d).\n", origem_octa_l, origem_octa_c);

    printf("\n--- Tabuleiro de Batalha Naval (%dx%d) com Habilidades ---\n", TAMANHO_TABULEIRO, TAMANHO_TABULEIRO);
    
    // 3. Exibir o Tabuleiro
    exibir_tabuleiro(tabuleiro);
    printf("\nLegenda: 0=Água, *3=Navio, **5=Área de Habilidade* (pode sobrepor navio/água).\n");

    return 0;
}

// --- Implementação das Funções do Nível Avançado (Habilidades) ---

/**
 * Cria a matriz de efeito de habilidade tipo Cone (base na última linha, ponta na primeira).
 * Matriz 5x5: Base (3ª linha) com 5, 2ª linha com 3, 1ª linha com 1.
 */
void criar_matriz_cone(int matriz_habilidade[TAMANHO_HABILIDADE][TAMANHO_HABILIDADE]) {
    // Inicializa a matriz com 0
    for (int i = 0; i < TAMANHO_HABILIDADE; i++) {
        for (int j = 0; j < TAMANHO_HABILIDADE; j++) {
            matriz_habilidade[i][j] = HABILIDADE_NAO_AFETADA;
        }
    }
    
    // Define a área de efeito em formato de cone (exemplo 5x5)
    int centro = TAMANHO_HABILIDADE / 2; // 2
    
    // Utiliza loops e condicionais para definir a área de efeito
    for (int i = 0; i < TAMANHO_HABILIDADE; i++) {
        for (int j = 0; j < TAMANHO_HABILIDADE; j++) {
            // Distância Manhattan do centro da base (i=4, j=2)
            // Simplificado para um padrão triangular (base no i=4)
            if (i >= 2) { // Afeta as 3 últimas linhas
                if (j >= centro - (TAMANHO_HABILIDADE - 1 - i) && j <= centro + (TAMANHO_HABILIDADE - 1 - i)) {
                    matriz_habilidade[i][j] = HABILIDADE_AFETADA;
                }
            } else if (i == 1) { // Linha 1
                if (j >= 1 && j <= 3) matriz_habilidade[i][j] = HABILIDADE_AFETADA;
            } else if (i == 0) { // Linha 0 (ponta)
                if (j == centro) matriz_habilidade[i][j] = HABILIDADE_AFETADA;
            }
        }
    }

    // Padrão do exemplo (mais simples e fixo)
    matriz_habilidade[0][2] = HABILIDADE_AFETADA; // 0 0 1 0 0
    matriz_habilidade[1][1] = HABILIDADE_AFETADA; // 0 1 1 1 0
    matriz_habilidade[1][2] = HABILIDADE_AFETADA;
    matriz_habilidade[1][3] = HABILIDADE_AFETADA;
    matriz_habilidade[2][0] = HABILIDADE_AFETADA; // 1 1 1 1 1
    matriz_habilidade[2][1] = HABILIDADE_AFETADA;
    matriz_habilidade[2][2] = HABILIDADE_AFETADA;
    matriz_habilidade[2][3] = HABILIDADE_AFETADA;
    matriz_habilidade[2][4] = HABILIDADE_AFETADA;
}

/**
 * Cria a matriz de efeito de habilidade tipo Cruz (centro).
 */
void criar_matriz_cruz(int matriz_habilidade[TAMANHO_HABILIDADE][TAMANHO_HABILIDADE]) {
    // Inicializa a matriz com 0
    for (int i = 0; i < TAMANHO_HABILIDADE; i++) {
        for (int j = 0; j < TAMANHO_HABILIDADE; j++) {
            matriz_habilidade[i][j] = HABILIDADE_NAO_AFETADA;
        }
    }
    
    // Define a área de efeito em formato de cruz (exemplo 5x5)
    int centro = TAMANHO_HABILIDADE / 2; // 2
    
    // Utiliza loops e condicionais: Linha central (i=2) e Coluna central (j=2)
    for (int i = 0; i < TAMANHO_HABILIDADE; i++) {
        for (int j = 0; j < TAMANHO_HABILIDADE; j++) {
            // Se estiver na linha central (i == centro) OU na coluna central (j == centro)
            if (i == centro || j == centro) {
                matriz_habilidade[i][j] = HABILIDADE_AFETADA;
            }
        }
    }
}

/**
 * Cria a matriz de efeito de habilidade tipo Octaedro (diamante).
 */
void criar_matriz_octaedro(int matriz_habilidade[TAMANHO_HABILIDADE][TAMANHO_HABILIDADE]) {
    // Inicializa a matriz com 0
    for (int i = 0; i < TAMANHO_HABILIDADE; i++) {
        for (int j = 0; j < TAMANHO_HABILIDADE; j++) {
            matriz_habilidade[i][j] = HABILIDADE_NAO_AFETADA;
        }
    }

    // Define a área de efeito em formato de octaedro/diamante (exemplo 5x5)
    int centro = TAMANHO_HABILIDADE / 2; // 2
    
    // Utiliza loops e condicionais (Distância de Manhattan do centro da matriz <= 2)
    for (int i = 0; i < TAMANHO_HABILIDADE; i++) {
        for (int j = 0; j < TAMANHO_HABILIDADE; j++) {
            // Calcula a distância de Manhattan (abs(i - centro) + abs(j - centro))
            int distancia_manhattan = abs(i - centro) + abs(j - centro);
            
            // Se a distância for menor ou igual à metade do tamanho (neste caso, 2)
            if (distancia_manhattan <= centro) {
                matriz_habilidade[i][j] = HABILIDADE_AFETADA;
            }
        }
    }
}

/**
 * Sobrepõe a matriz de efeito de habilidade ao tabuleiro principal.
 * Garante que a área de efeito permaneça dentro dos limites do tabuleiro.
 * @param tabuleiro O tabuleiro 10x10.
 * @param matriz_habilidade A matriz 5x5 de efeito (com 0s e 1s).
 * @param linha_origem A linha central no tabuleiro onde o efeito começa.
 * @param coluna_origem A coluna central no tabuleiro onde o efeito começa.
 */
void sobrepor_habilidade(int tabuleiro[TAMANHO_TABULEIRO][TAMANHO_TABULEIRO], int matriz_habilidade[TAMANHO_HABILIDADE][TAMANHO_HABILIDADE], int linha_origem, int coluna_origem) {
    int offset = TAMANHO_HABILIDADE / 2; // 2 (Para centralizar o efeito)

    // Percorre a matriz de habilidade (i_h, j_h)
    for (int i_h = 0; i_h < TAMANHO_HABILIDADE; i_h++) {
        for (int j_h = 0; j_h < TAMANHO_HABILIDADE; j_h++) {
            
            // Se a posição na matriz de habilidade for afetada (valor 1)
            if (matriz_habilidade[i_h][j_h] == HABILIDADE_AFETADA) {
                
                // Calcula a coordenada correspondente no tabuleiro principal (i_t, j_t)
                // A origem é o centro da habilidade no tabuleiro.
                int i_t = linha_origem + (i_h - offset);
                int j_t = coluna_origem + (j_h - offset);
                
                // Validação de limites (condicionais)
                if (i_t >= 0 && i_t < TAMANHO_TABULEIRO && j_t >= 0 && j_t < TAMANHO_TABULEIRO) {
                    
                    // Sobrepõe o valor da habilidade (5) no tabuleiro principal
                    // Nota: O navio (3) pode ser substituído/sobreposto pela área de efeito (5)
                    tabuleiro[i_t][j_t] = EFEITO_HABILIDADE;
                }
            }
        }
    }
}


/**
 * Função para exibir o tabuleiro no console de forma organizada.
 * Imprime os índices de colunas e linhas.
 * Diferentes caracteres para Água, Navio e Habilidade.
 */
void exibir_tabuleiro(int tabuleiro[TAMANHO_TABULEIRO][TAMANHO_TABULEIRO]) {
    // Imprime cabeçalho de colunas (índices)
    printf("   ");
    for (int j = 0; j < TAMANHO_TABULEIRO; j++) {
        printf(" %d", j); // Imprime o índice da coluna
    }
    printf("\n");
    // Separador visual
    printf("  +---------------------\n"); 

    // Imprime as linhas do tabuleiro
    for (int i = 0; i < TAMANHO_TABULEIRO; i++) {
        printf("%d |", i); // Imprime o índice da linha
        for (int j = 0; j < TAMANHO_TABULEIRO; j++) {
            // Usa diferentes caracteres/valores para melhor visualização
            if (tabuleiro[i][j] == AGUA) {
                printf(" 0"); // Água
            } else if (tabuleiro[i][j] == NAVIO) {
                printf(" 3"); // Navio
            } else if (tabuleiro[i][j] == EFEITO_HABILIDADE) {
                printf(" 5"); // Habilidade
            } else {
                 printf(" ?"); // Outro (erro)
            }
        }
        printf("\n");
    }
}

// --- Implementação das Funções do Nível Aventureiro (Mantidas) ---

/**
 * Função para validar se o navio pode ser posicionado.
 * Verifica os limites do tabuleiro e se a posição já está ocupada.
 * Suporta orientações 'H', 'V', '1' (Diagonal L+, C+) e '2' (Diagonal L+, C-).
 */
bool validar_posicao(int linha_inicio, int coluna_inicio, int tamanho, char orientacao, int tabuleiro[TAMANHO_TABULEIRO][TAMANHO_TABULEIRO]) {
    if (linha_inicio < 0 || coluna_inicio < 0 || linha_inicio >= TAMANHO_TABULEIRO || coluna_inicio >= TAMANHO_TABULEIRO) return false;
    
    for (int i = 0; i < tamanho; i++) {
        int linha_atual = linha_inicio;
        int coluna_atual = coluna_inicio;

        if (orientacao == 'H') coluna_atual += i;
        else if (orientacao == 'V') linha_atual += i;
        else if (orientacao == '1') { // Diagonal D1: L+, C+
            linha_atual += i;
            coluna_atual += i;
        } else if (orientacao == '2') { // Diagonal D2: L+, C-
            linha_atual += i;
            coluna_atual -= i; 
        } else return false; // Orientação inválida

        if (linha_atual < 0 || linha_atual >= TAMANHO_TABULEIRO || 
            coluna_atual < 0 || coluna_atual >= TAMANHO_TABULEIRO) return false; // Sai dos limites

        if (tabuleiro[linha_atual][coluna_atual] != AGUA) return false; // Sobreposição (apenas com NAVIO, ignora HABILIDADE)
    }
    return true;
}

/**
 * Função para posicionar o navio no tabuleiro, marcando as células com NAVIO (3).
 */
void posicionar_navio(int linha_inicio, int coluna_inicio, int tamanho, char orientacao, int tabuleiro[TAMANHO_TABULEIRO][TAMANHO_TABULEIRO]) {
    for (int i = 0; i < tamanho; i++) {
        if (orientacao == 'H') {
            tabuleiro[linha_inicio][coluna_inicio + i] = NAVIO;
        } else if (orientacao == 'V') {
            tabuleiro[linha_inicio + i][coluna_inicio] = NAVIO;
        } else if (orientacao == '1') {
            tabuleiro[linha_inicio + i][coluna_inicio + i] = NAVIO;
        } else if (orientacao == '2') {
            tabuleiro[linha_inicio + i][coluna_inicio - i] = NAVIO;
        }
    }
}