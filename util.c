#include <stdio.h>
#include <stdlib.h>
#include <math.h> // sqrt, pow, ...

#include "util.h"

// inicializa a tela; deve ser chamada no inicio da execucao do programa
void inicia_tela(Tela *t, int larg, int alt, char *nome) {
    /* inicializa dados sobre o mouse */
    t->_mouse.x = t->_mouse.y = t->_tecla = 0;
    t->_botao = false;

    /* inicializa o allegro */
    if (!al_init()) {
        fprintf(stderr, "%s", "falha na inicializacao do allegro\n");
        exit(1);
    }

    /* conecta com tela */
    t->display = al_create_display(larg, alt);
    if (t->display == NULL) {
      fprintf(stderr, "%s", "falha ao criar display do allegro\n");
      exit(1);
    }

    t->janela.pos.x = t->janela.pos.y = 0;
    t->tam.larg = larg;
    t->tam.alt = alt;
    t->janela.tam.larg = t->tam.larg;
    t->janela.tam.alt = t->tam.alt;

    /* titulo da tela */
    al_set_window_title(t->display, nome);

    /* cria contexto grafico */
    t->ac_fundo = al_map_rgb(255, 255, 255);
    t->ac_cor = al_map_rgb(0, 0, 0);

    /* instala o driver de mouse e teclado */
    al_install_mouse();
    al_install_keyboard();
    al_init_primitives_addon();

    /* configura fonte */
    al_init_font_addon();
    al_init_ttf_addon();
    al_init_image_addon();
    t->fonte = al_load_font("data/Audiowide-Regular.ttf", 15, 0);
    if (t->fonte == NULL) {
      fprintf(stderr, "%s",  "falha ao carregar fonte do allegro\n");
      exit(1);
    }

    /* fila para eventos */
    t->queue = al_create_event_queue();
    if (t->queue == NULL) {
      fprintf(stderr, "%s",  "falha ao buscar eventos do allegro\n");
      exit(1);
    }

    /* registra para receber eventos de tela/teclado/mouse */
    al_register_event_source(t->queue, al_get_keyboard_event_source());
    al_register_event_source(t->queue, al_get_display_event_source(t->display));
    al_register_event_source(t->queue, al_get_mouse_event_source());
}

// limpa a tela toda
void limpa_tela(Tela *t) {
    al_clear_to_color(t->ac_fundo);
}

// faz aparecer na janela o que foi desenhado
void mostra_tela() {
    // troca os buffers de video, passando o que foi desenhado para tela
    al_flip_display();
}

// tempo de espera da tela, em microsegundos
void espera(double ms) {
    al_rest(ms / 1e3);
}

// finaliza a tela; deve ser chamado no final do programa
void finaliza_tela(Tela *t) {
    // programa vai fechar
    al_destroy_display(t->display);
    al_destroy_event_queue(t->queue);
    al_destroy_font(t->fonte);
}

// retorna o codigo da proxima tecla apertada (ou 0, se nenhuma tecla tiver sido apertada)
int codigo_tecla(Tela *t) {
    int tecla;
    processa_eventos(t);
    tecla = t->_tecla;
    t->_tecla = 0;
    // retorna a ultima _tecla pressionada
    return tecla;
}

// processa eventos da tela
void processa_eventos(Tela *t) {
    /* processa eventos do servidor X, atualizando a posicao do mouse
     * e ultima _tecla pressionada na variavel da tela. */
    ALLEGRO_EVENT event;

    // al_wait_for_event(t->queue, &event);
    while (al_get_next_event(t->queue, &event)) {
        switch (event.type) {
            /* _tecla foi pressionada */
            case ALLEGRO_EVENT_KEY_DOWN: {
                t->_tecla = event.keyboard.keycode;
                break;
            }
            case ALLEGRO_EVENT_MOUSE_AXES: {
                t->_mouse.x = event.mouse.x;
                t->_mouse.y = event.mouse.y;
                break;
            }
            case ALLEGRO_EVENT_MOUSE_BUTTON_DOWN: {
                if (event.mouse.button == 1)
                    t->_botao = true;
                break;
            }
            case ALLEGRO_EVENT_MOUSE_BUTTON_UP: {
                if (event.mouse.button == 1)
                    t->_botao = false;
                break;
            }
            case ALLEGRO_EVENT_DISPLAY_CLOSE: {
                t->_tecla = ALLEGRO_EVENT_DISPLAY_CLOSE;
                break;
            }
            case ALLEGRO_EVENT_TIMER: {
                break;
            }
            default:
                break;
        }
    }
}

// desenha um retangulo (preenchido ou nao) com a cor padrao
void desenha_retangulo(Retangulo r, Tela *t, bool filled) {
    if (filled) {
        al_draw_filled_rectangle(
            /* canto superior esquerdo */
            r.pos.x, r.pos.y,
            /* canto inferior direito */
           r.pos.x + r.tam.larg, r.pos.y + r.tam.alt, 
           /* cor para preenchimento */
           t->ac_cor);
    } else {
        al_draw_rectangle(
            /* canto superior esquerdo */
            r.pos.x, r.pos.y,
            /* canto inferior direito */
            r.pos.x + r.tam.larg, r.pos.y + r.tam.alt, 
            /* cor e espessura da borda */
            t->ac_cor, 2);
    }
}

// desenha uma linha do ponto p1 ao ponto p2 com a cor padrao
void desenha_linha(Ponto p1, Ponto p2, Tela *t) {
    al_draw_line(p1.x, p1.y, p2.x, p2.y, t->ac_cor, 2);
}

// desenha um x dentro do retangulo na cor padrao
void desenha_x_dentro_ret(Retangulo r, Tela *t) {
    int offset = 5;
    Tamanho tam = r.tam;

    Ponto p1 = { r.pos.x + offset, r.pos.y + offset };
    Ponto p2 = { r.pos.x + tam.larg - offset, r.pos.y + tam.alt - offset };
    desenha_linha(p1, p2, t);

    Ponto p3 = { r.pos.x + tam.larg - offset, r.pos.y + offset };
    Ponto p4 = { r.pos.x + offset, r.pos.y + tam.alt - offset };
    desenha_linha(p3, p4, t);
}

// mantem rgb entre 0 e 1
int ajeita_rgb(int x) {
  return x < 0 ? 0 : (x > 1 ? 1 : x);
}

// altera a cor padrão (muda a cor dos proximos desenhos de linha/retangulo/caracteres/etc)
void define_cor(Tela *t, Cor c) {
    int R, G, B;
    R = ajeita_rgb(c.r) * ((1 << 8) - 1);
    G = ajeita_rgb(c.g) * ((1 << 8) - 1);
    B = ajeita_rgb(c.b) * ((1 << 8) - 1);
    t->ac_cor = al_map_rgb(R, G, B);
}

// escreve o texto s na posicao p da tela
void escreve_texto(Tela *t, Ponto p, char *s) {
    al_draw_text(t->fonte, t->ac_cor, p.x, p.y, ALLEGRO_ALIGN_LEFT, s);
}

// retorna a posicao do cursor do mouse
Ponto posicao_mouse(Tela *t) {
    processa_eventos(t);
    return t->_mouse;
}

// retorna se o botao esta pressionado
bool botao_clicado(Tela *t) {
    processa_eventos(t);
    return t->_botao;
}

// retorna tamanho necessario para se escrever o texto s
Tamanho tamanho_texto(Tela *t, char *s) {
    Tamanho tam;
    int bbx, bby, bbw, bbh;
    al_get_text_dimensions(t->fonte, s, &bbx, &bby, &bbw, &bbh);
    tam.larg = bbw;
    tam.alt = bbh;
    return tam;
}

// retorna se o ponto estiver dentro do retangulo
bool ret_contains_pt(Retangulo r, Ponto p) {
    return (p.x >= r.pos.x) && (p.x <= r.pos.x + r.tam.larg) &&
           (p.y >= r.pos.y) && (p.y <= r.pos.y + r.tam.alt);
}
