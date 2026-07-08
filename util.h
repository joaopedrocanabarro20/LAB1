#include <allegro5/allegro.h>
#include <allegro5/allegro_font.h>
#include <allegro5/allegro_ttf.h>
#include <allegro5/allegro_image.h>
#include <allegro5/allegro_color.h>
#include <allegro5/allegro_primitives.h>

// estrutura que representa uma cor, com RGB podendo variar entre 0 e 1
typedef struct cor {
    float r;
    float g;
    float b;
} Cor;

// estrutura que representa um ponto, com as coordenadas x e y, do tipo float
typedef struct ponto {
    float x;
    float y;
} Ponto;

// estrutura que representa um tamanho, constituído por altura e largura, do tipo float
typedef struct tamanho {
    float larg;
    float alt;
} Tamanho;

/* Retangulo/Celula, constituído por:
    a) um ponto (que contém as coordenadas do canto inferior esquerdo do retangulo)
    b) um tamanho
    c) um estado, podendo ser vazio, colorido ou alerta (usado quando for Celula)
*/ 
typedef struct retangulo {
    Ponto pos;
    Tamanho tam;
    enum { VAZIO, COLORIDO, ALERTA } estado;
} Retangulo, Celula;

// estrutura que representa um botao, contendo o retangulo, o estado e o texto do botao
typedef struct botao {
    Retangulo ret;
    enum { NORMAL, HOVER, APERTADO } estado;
    char const *texto;
} Botao;

// estrutura que representa uma tela
typedef struct tela {
    ALLEGRO_DISPLAY *display;   // display X
    ALLEGRO_COLOR ac_fundo;     // cor de fundo
    ALLEGRO_COLOR ac_cor;       // cor padrao
    ALLEGRO_EVENT_QUEUE *queue; // fila de eventos
    ALLEGRO_FONT *fonte;        // fonte padrao
    Retangulo janela;           // retangulo que contem a janela
    Tamanho tam;                // tamanho da janela
    Ponto _mouse;               // onde esta o mouse
    bool _botao;                // estado do botao do mouse
    int _tecla;                 // ultima tecla apertada
} Tela;

// inicializa a tela; deve ser chamada no inicio da execucao do programa
void inicia_tela(Tela *t, int larg, int alt, char *nome);

// limpa a tela toda
void limpa_tela(Tela *t);

// faz aparecer na janela o que foi desenhado
void mostra_tela();

// tempo de espera da tela, em microsegundos
void espera(double ms);

// finaliza a tela; deve ser chamado no final do programa
void finaliza_tela(Tela *t);

// retorna o codigo da proxima tecla apertada (ou 0, se nenhuma tecla tiver sido apertada)
int codigo_tecla(Tela *t);

// processa eventos da tela
void processa_eventos(Tela *t);

// desenha um retangulo (preenchido ou nao) com a cor padrao
void desenha_retangulo(Retangulo r, Tela *t, bool filled);

// desenha uma linha do ponto p1 ao ponto p2 com a cor padrao
void desenha_linha(Ponto p1, Ponto p2, Tela *t);

// desenha um x dentro do retangulo na cor padrao
void desenha_x_dentro_ret(Retangulo r, Tela *t);

// mantem rgb entre 0 e 1
int ajeita_rgb(int x);

// altera a cor padrao (muda a cor dos proximos desenhos de linha/retangulo/caracteres/etc)
void define_cor(Tela *t, Cor c);

// escreve o texto s a partir da posicao p da tela
void escreve_texto(Tela *t, Ponto p, char *s);

// retorna a posicao do cursor do mouse
Ponto posicao_mouse(Tela *t);

// retorna se o botao do mouse estiver apertado
bool botao_clicado(Tela *t);

// retorna tamanho necessario para se escrever o texto s
Tamanho tamanho_texto(Tela *t, char *s);

// retorna se o ponto esta dentro do retangulo
bool ret_contains_pt(Retangulo r, Ponto p);
