#include <allegro5/allegro.h>
#include <allegro5/allegro_image.h>
#include <allegro5/allegro_font.h>
#include <allegro5/allegro_ttf.h>
#include <allegro5/allegro_primitives.h>
#include <stdlib.h>
#include <stdio.h>
#include <time.h>

#define LARGURA_TELA 720
#define ALTURA_TELA 480
#define QTD_OBJETOS_POR_SEGUNDO 1

// Estrutura para representar os objetos
typedef struct {
    float x, y; // Coordenadas do objeto
    int capturado; // Indica se o objeto foi capturado
    int tipo; // Tipo do objeto (1 para incremento, -1 para decremento)
} Objeto;

// Variáveis globais
ALLEGRO_DISPLAY *tela;
ALLEGRO_EVENT_QUEUE *fila_eventos;
ALLEGRO_TIMER *temporizador;
ALLEGRO_BITMAP *imagem_jogador, *imagem_objeto, *imagem_objeto_penalidade, *imagem_fundo;
ALLEGRO_FONT *fonte;
Objeto *objetos;
int jogador_x, jogador_y;
int objetos_capturados = 0;
int jogo_finalizado = 0;
int pontos = 0;
int reiniciar_jogo = 0;
int num_objetos = 0;
float tempo_para_criar_objetos = 0;


// Variáveis para animação do sprite
int largura_quadro = 50; // Largura de cada quadro
int altura_quadro = 80; // Altura de cada quadro
int quadro_atual = 0; // Quadro atual da animação
int num_quadros = 4; // Número total de quadros no sprite
float duracao_quadro = 0.1; // Duração de cada quadro (em segundos)
float tempo_quadro = 0; // Tempo acumulado para alternar os quadros

// Função para inicializar o jogo
void inicializar_jogo() {
    jogador_x = LARGURA_TELA / 2;
    jogador_y = ALTURA_TELA - 100; // Posição inicial do jogador
    objetos_capturados = 0;
    pontos = 0;
    num_objetos = 0;
    jogo_finalizado = 0;

    // Aloca memória inicial para objetos se ainda não foi alocada
    if (objetos == NULL) {
        objetos = (Objeto *)malloc(sizeof(Objeto) * QTD_OBJETOS_POR_SEGUNDO); // Cast explícito para Objeto*
        if (objetos == NULL) {
            fprintf(stderr, "Erro ao alocar memória para objetos!\n");
            exit(-1); // Finaliza o jogo caso haja erro de alocação
        }
    }

    // Cria novos objetos, ajustando sua posição inicial fora da tela
    for (int i = num_objetos; i < num_objetos + QTD_OBJETOS_POR_SEGUNDO; i++) {
        objetos[i].x = rand() % (LARGURA_TELA - 50);
        objetos[i].y = -50; // Inicia fora da tela, acima
        objetos[i].capturado = 0;
        objetos[i].tipo = (rand() % 2) == 0 ? 1 : -1; // Alterna entre objetos que somam e que subtraem pontos
    }
    num_objetos += QTD_OBJETOS_POR_SEGUNDO;
}



// Função para desenhar os objetos na tela
void desenhar_objetos() {
    for (int i = 0; i < num_objetos; i++) {
        if (!objetos[i].capturado) {
            ALLEGRO_BITMAP *imagem = (objetos[i].tipo == 1) ? imagem_objeto : imagem_objeto_penalidade;
            al_draw_bitmap(imagem, objetos[i].x, objetos[i].y, 0);
        }
    }
}

// Função para desenhar o jogador
void desenhar_jogador() {
    al_draw_bitmap_region(
        imagem_jogador, 
        quadro_atual * largura_quadro, // Posição X do quadro no sprite
        0, // Posição Y do quadro no sprite
        largura_quadro, // Largura do quadro
        altura_quadro, // Altura do quadro
        jogador_x, // Posição X na tela
        jogador_y, // Posição Y na tela
        0 // Sem espelhamento
    );
}

// Função para desenhar o fundo
void desenhar_fundo() {
    al_draw_bitmap(imagem_fundo, 0, 0, 0);
}

// Função para desenhar a pontuação na tela
void desenhar_pontuacao() {
    al_draw_textf(fonte, al_map_rgb(255, 255, 255), LARGURA_TELA - 100, 20, ALLEGRO_ALIGN_RIGHT, "Pontos: %d", pontos);
}

// Função para atualizar a posição dos objetos
void atualizar_objetos() {
    tempo_para_criar_objetos += 1.0 / 60.0; // Incrementa o tempo

    // Cria novos objetos a cada segundo
    if (tempo_para_criar_objetos >= 1.0) {
        // Aloca memória suficiente para os objetos existentes mais os novos
        Objeto *novos_objetos = (Objeto *)malloc(sizeof(Objeto) * (num_objetos + QTD_OBJETOS_POR_SEGUNDO));
        
        if (novos_objetos == NULL) {
            fprintf(stderr, "Erro ao alocar memória para novos objetos!\n");
            exit(-1); // Finaliza o jogo caso haja erro de alocação
        }

        // Copia os objetos existentes para a nova alocação
        for (int i = 0; i < num_objetos; i++) {
            novos_objetos[i] = objetos[i];
        }

        // Aloca novos objetos
        for (int i = num_objetos; i < num_objetos + QTD_OBJETOS_POR_SEGUNDO; i++) {
            novos_objetos[i].x = rand() % (LARGURA_TELA - 50);
            novos_objetos[i].y = -50; 
            novos_objetos[i].capturado = 0;
            novos_objetos[i].tipo = (rand() % 2) == 0 ? 1 : -1;
        }

        // Libera a memória da alocação anterior
        free(objetos);

        // Atualiza o ponteiro de objetos para a nova memória alocada
        objetos = novos_objetos;
        num_objetos += QTD_OBJETOS_POR_SEGUNDO;
        tempo_para_criar_objetos = 0; // Reseta o tempo
    }

    // Atualiza a posição dos objetos existentes
    for (int i = 0; i < num_objetos; i++) {
        if (!objetos[i].capturado) {
            objetos[i].y += 2; // Velocidade de queda dos objetos
            if (objetos[i].y > ALTURA_TELA) {
                objetos[i].y = -50; // Reinicia a queda do objeto, fora da tela
                objetos[i].x = rand() % (LARGURA_TELA - 50);
                objetos[i].tipo = (rand() % 2) == 0 ? 1 : -1;
            }
        }
    }
}



// Função para verificar colisões entre o jogador e os objetos
void verificar_colisoes() {
    for (int i = 0; i < num_objetos; i++) {
        if (!objetos[i].capturado && 
            jogador_x < objetos[i].x + 50 && jogador_x + 50 > objetos[i].x &&
            jogador_y < objetos[i].y + 50 && jogador_y + 50 > objetos[i].y) {
            objetos[i].capturado = 1;
            if (objetos[i].tipo == 1) {
                pontos++; // Aumenta os pontos para objetos positivos
                objetos_capturados++;
            } else if (objetos[i].tipo == -1) {
                jogo_finalizado = 1; // Fim do jogo caso o objeto de penalidade seja pego
            }
        }
    }
}

// Função para exibir os resultados ao final do jogo
void exibir_resultados() {
    al_draw_filled_rectangle(0, 0, LARGURA_TELA, ALTURA_TELA, al_map_rgba(0, 0, 0, 200));
    
    if (jogo_finalizado) {
        al_draw_text(fonte, al_map_rgb(255, 0, 0), LARGURA_TELA / 2, ALTURA_TELA / 2 - 50, ALLEGRO_ALIGN_CENTER, "GAME OVER!");

        al_draw_textf(fonte, al_map_rgb(255, 0, 0), LARGURA_TELA / 2, ALTURA_TELA / 2, ALLEGRO_ALIGN_CENTER, "Pontos coletados: %d", pontos);
    } else {
        al_draw_textf(fonte, al_map_rgb(255, 255, 255), LARGURA_TELA / 2, ALTURA_TELA / 2 - 50, ALLEGRO_ALIGN_CENTER, "Você coletou %d objetos!", pontos);
    }

    al_draw_text(fonte, al_map_rgb(255, 255, 255), LARGURA_TELA / 2, ALTURA_TELA / 2 + 50, ALLEGRO_ALIGN_CENTER, "Pressione ESC para sair");


    al_draw_text(fonte, al_map_rgb(255, 255, 255), LARGURA_TELA / 2, ALTURA_TELA / 2 + 50, ALLEGRO_ALIGN_CENTER, "Pressione ESC para sair");
}
