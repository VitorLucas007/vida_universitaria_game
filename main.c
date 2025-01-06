#include "jogo.h"
// Função principal
int main() {
    srand(time(NULL));

    // Inicializa Allegro e seus componentes
    if (!al_init()) {
        fprintf(stderr, "Erro ao inicializar o Allegro!\n");
        return -1;
    }

    al_init_image_addon();
    al_init_font_addon();
    al_init_ttf_addon();
    al_init_primitives_addon();
    al_install_keyboard();

    tela = al_create_display(LARGURA_TELA, ALTURA_TELA);
    if (!tela) {
        fprintf(stderr, "Erro ao criar a tela!\n");
        return -1;
    }

    fonte = al_load_font("/home/lucas/Documentos/teste/assets/arial.ttf", 24, 0);
    imagem_jogador = al_load_bitmap("/home/lucas/Documentos/teste/assets/Player.png");
    imagem_objeto = al_load_bitmap("/home/lucas/Documentos/teste/assets/livro4.png");
    imagem_objeto_penalidade = al_load_bitmap("/home/lucas/Documentos/teste/assets/corote.png");
    imagem_fundo = al_load_bitmap("/home/lucas/Documentos/teste/assets/cenario3.jpg");

    if (!fonte || !imagem_jogador || !imagem_objeto || !imagem_objeto_penalidade || !imagem_fundo) {
        fprintf(stderr, "Erro ao carregar imagens ou fontes!\n");
        return -1;
    }

    temporizador = al_create_timer(1.0 / 60.0);
    fila_eventos = al_create_event_queue();

    al_register_event_source(fila_eventos, al_get_keyboard_event_source());
    al_register_event_source(fila_eventos, al_get_timer_event_source(temporizador));
    al_register_event_source(fila_eventos, al_get_display_event_source(tela));

    inicializar_jogo();
    al_start_timer(temporizador);

    ALLEGRO_KEYBOARD_STATE estado_teclado;

    while (1) {
        ALLEGRO_EVENT evento;
        al_wait_for_event(fila_eventos, &evento);

        if (evento.type == ALLEGRO_EVENT_DISPLAY_CLOSE) {
            break;
        } else if (evento.type == ALLEGRO_EVENT_TIMER) {
            al_get_keyboard_state(&estado_teclado);

            if (!jogo_finalizado) {
                // Movimenta o jogador
                if (al_key_down(&estado_teclado, ALLEGRO_KEY_LEFT) && jogador_x > 0) {
                    jogador_x -= 5;
                }
                if (al_key_down(&estado_teclado, ALLEGRO_KEY_RIGHT) && jogador_x < LARGURA_TELA - largura_quadro) {
                    jogador_x += 5;
                }

                // Sai do jogo ao pressionar ESC
                if (al_key_down(&estado_teclado, ALLEGRO_KEY_ESCAPE)) {
                    break;
                }

                // Atualiza o tempo da animação
                tempo_quadro += 1.0 / 60.0;
                if (tempo_quadro >= duracao_quadro) {
                    quadro_atual = (quadro_atual + 1) % num_quadros;
                    tempo_quadro = 0;
                }

                atualizar_objetos();
                verificar_colisoes();

                if (pontos >= 100) {
                    jogo_finalizado = 1;
                }

                al_clear_to_color(al_map_rgb(0, 0, 0));
                desenhar_fundo();
                desenhar_objetos();
                desenhar_jogador();
                desenhar_pontuacao();
            } else {
                // Verifica entrada na tela de resultados
                if (al_key_down(&estado_teclado, ALLEGRO_KEY_R)) {
                    reiniciar_jogo = 1;
                    break;
                }
                if (al_key_down(&estado_teclado, ALLEGRO_KEY_ESCAPE)) {
                    break;
                }
                al_clear_to_color(al_map_rgb(0, 0, 0));
                desenhar_fundo();
                exibir_resultados();
            }

            al_flip_display();
        }
    }

    if (reiniciar_jogo) {
        inicializar_jogo();
        main();
    }

    al_destroy_bitmap(imagem_jogador);
    al_destroy_bitmap(imagem_objeto);
    al_destroy_bitmap(imagem_objeto_penalidade);
    al_destroy_bitmap(imagem_fundo);
    al_destroy_font(fonte);
    al_destroy_timer(temporizador);
    al_destroy_event_queue(fila_eventos);
    free(objetos);

    return 0;
}
