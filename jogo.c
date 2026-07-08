#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "util.h" 
#include <stdbool.h>

// definicoes janela
#define LARGURA_JANELA 800
#define ALTURA_JANELA 800
#define TITULO_JANELA "Nonogram"
    
// definicoes tabuleiro
#define LINHAS 8
#define COLUNAS 8
#define ALTURA_CELULA 50
#define LARGURA_CELULA 50
#define LARGURA_TABULEIRO LARGURA_CELULA * COLUNAS
#define ALTURA_TABULEIRO ALTURA_CELULA * LINHAS
#define MARGIN_HORIZONTAL (int)((LARGURA_JANELA / 2) - (LARGURA_TABULEIRO / 2))
#define MARGIN_VERTICAL (int)((ALTURA_JANELA / 2) - (ALTURA_TABULEIRO / 2))
#define maxjogadas 64

typedef struct{
    Botao classico;
    Botao normal;
    Botao tutorial;
}Menu;
typedef enum {
    MENU,
    Jogo_Classico,
    Jogo_Normal,
    Tutorial,
    vitoria_classico,
    vitoria_normal,
    derrota_normal,
}EstadoTela;
typedef struct 
{
    int linha;
    int coluna;
    int estado_anterior;
}jogada;


void inicia_menu(Menu *m);
void desenha_menu(Tela *t, Menu *m);
EstadoTela verifica_menu(Tela *t, Menu *m);
void desenha_tutorial(Tela *t);
void inicia_tabuleiro(Celula tabuleiro[][COLUNAS]);
void desenha_tabuleiro(Tela *t, Celula tabuleiro[][COLUNAS], int resultado[][COLUNAS]);
void desenha_restantes(Tela *t, Celula tabuleiro[][COLUNAS], int resultado[][COLUNAS]);
int celulas_restantes(Celula tabuleiro[][COLUNAS], int resultado[][COLUNAS]);
void verifica_cliqueclassico(Tela *t, Celula tabuleiro[][COLUNAS], jogada historico[], int *topo, int resultado[][COLUNAS], EstadoTela *estado);
void verifica_clique_normal(Tela *t, Celula tabuleiro[][COLUNAS], int resultado[][COLUNAS], EstadoTela *estado, int *vidas);
void escreve_numeros(Tela *t, int *numeros, int qtd, Ponto inicio, bool horizontal);
void cicla_estado_celula(Celula *c);
void desenha_botao(Tela *t, Botao *b);
void inicia_botoes_jogo(Botao *dica);
void inicia_botoes_jogo1(Botao *reset);
void inicia_botoes_jogo2(Botao *voltar);
void inicia_botoes_jogo3(Botao *desfazer);
void inicia_botoes_jogo4(Botao *reinicia);
void inicia_botoes_jogo5(Botao *resolver);
void resolve_nano(Celula tabuleiro[][COLUNAS], int resultado[][COLUNAS]);
void dica_nono(Celula tabuleiro[][COLUNAS], int resultado[][COLUNAS]);
void gera_resultado(int resultado[][COLUNAS]);
void reseta_tabuleiro(Celula tabuleiro[][COLUNAS], int *topo);
void calcula_linha(int resultado[][COLUNAS], int linha, int numeros[], int *qtd);
void calcula_coluna(int resultado[][COLUNAS], int coluna, int numeros[], int *qtd);
EstadoTela verifica_botoes_jogo(Tela *t, Botao *voltar, Botao *reset, Botao *desfazer, Botao *reinicia, Botao *resolver, Botao *dica, EstadoTela estado_atual, int resultado[][COLUNAS], Celula tabuleiro_nonogram[][COLUNAS], jogada historico[], int *topo, int *vidas, int *dicas);
void desenha_vitoria(Tela *t);
void desenha_derrota(Tela *t);
void desenha_vidas(Tela *t, int vidas);
bool verifica_vitoria(Celula tabuleiro[][COLUNAS], int resultado[][COLUNAS]);

int main(int argc, char **argv) {
    srand(time(NULL));
    int tecla_pressionada;  
    Tela t;
    Menu menu;
    Celula tabuleiro_nonogram[LINHAS][COLUNAS];
    int resultado[LINHAS][COLUNAS];
    jogada historico[maxjogadas]; 
    int topo=0;
    int vidas=3;
    int dicas=3;
    EstadoTela estado=MENU;
    Botao botao_dica;
    Botao botao_reset;
    Botao botao_voltar;
    Botao botao_desfazer;
    Botao botao_reinicia;
    Botao botao_resolver;

    inicia_tela(&t, LARGURA_JANELA, ALTURA_JANELA, TITULO_JANELA);
    inicia_menu(&menu);
    gera_resultado(resultado);
    inicia_tabuleiro(tabuleiro_nonogram);
    inicia_botoes_jogo(&botao_dica);
    inicia_botoes_jogo1(&botao_reset);
    inicia_botoes_jogo2(&botao_voltar);
    inicia_botoes_jogo3(&botao_desfazer);
    inicia_botoes_jogo4(&botao_reinicia);
    inicia_botoes_jogo5(&botao_resolver);
    while((tecla_pressionada=codigo_tecla(&t)) != ALLEGRO_EVENT_DISPLAY_CLOSE)
     {
        limpa_tela(&t);
        switch(estado){
        case MENU:
            desenha_menu(&t,&menu);
            estado=verifica_menu(&t, &menu);
            break;


        case Jogo_Classico:
            desenha_tabuleiro(&t, tabuleiro_nonogram, resultado);
            desenha_restantes(&t, tabuleiro_nonogram, resultado);
            desenha_botao(&t, &botao_dica);
            desenha_botao(&t, &botao_reset);
            desenha_botao(&t, &botao_voltar);
            desenha_botao(&t, &botao_desfazer);
            desenha_botao(&t, &botao_reinicia);
            desenha_botao(&t, &botao_resolver);
            estado=verifica_botoes_jogo(&t, &botao_voltar, &botao_reset, &botao_desfazer, &botao_reinicia, &botao_resolver, &botao_dica, estado, resultado, tabuleiro_nonogram, historico, &topo, &vidas, &dicas); 
            verifica_cliqueclassico(&t, tabuleiro_nonogram, historico, &topo, resultado, &estado);
        break;

        case Jogo_Normal:
            desenha_tabuleiro(&t, tabuleiro_nonogram, resultado);
            desenha_vidas(&t, vidas);
            desenha_botao(&t, &botao_dica);
            desenha_botao(&t, &botao_reset);
            desenha_botao(&t, &botao_voltar); 
            desenha_botao(&t, &botao_reinicia);
            desenha_botao(&t, &botao_resolver);
            estado=verifica_botoes_jogo(&t, &botao_voltar, &botao_reset, NULL, &botao_reinicia, &botao_resolver, &botao_dica, estado, resultado, tabuleiro_nonogram, NULL, NULL, &vidas, &dicas);
            verifica_clique_normal(&t, tabuleiro_nonogram, resultado, &estado, &vidas);
           
        break;

        case Tutorial:
            desenha_tutorial(&t);   
            desenha_botao(&t, &botao_voltar);
            estado=verifica_botoes_jogo(&t, &botao_voltar, &botao_reset, NULL, &botao_reinicia, &botao_resolver, &botao_dica, estado, resultado, tabuleiro_nonogram, NULL, NULL, &vidas, &dicas);
        break;

        case vitoria_classico:
            desenha_vitoria(&t);
            desenha_botao(&t, &botao_voltar);
            estado= verifica_botoes_jogo(&t, &botao_voltar, NULL, NULL, &botao_reinicia, &botao_resolver, &botao_dica, estado, resultado, tabuleiro_nonogram, NULL, NULL, &vidas, &dicas);
            if(estado == MENU){
                vidas=3;
                topo=0;
            }
        break;
        case vitoria_normal:
            desenha_vitoria(&t);
            desenha_botao(&t, &botao_voltar);
            estado = verifica_botoes_jogo(&t, &botao_voltar, NULL, NULL, &botao_reinicia, &botao_resolver, &botao_dica, estado, resultado, tabuleiro_nonogram, NULL, NULL, &vidas, &dicas);
            if(estado==MENU){
                vidas=3;
                topo=0;
            }

        break;
        case derrota_normal:
            desenha_derrota(&t);
            desenha_botao(&t, &botao_voltar);
            estado = verifica_botoes_jogo(&t, &botao_voltar, NULL, NULL, &botao_reinicia, &botao_resolver, &botao_dica, estado, resultado, tabuleiro_nonogram, NULL, NULL, &vidas, &dicas);
            if(estado == MENU){
                vidas = 3;
                topo = 0;
            }
        break;
        }
        mostra_tela();
        espera(30);
    
}
    finaliza_tela(&t);

    return 0;
}

void inicia_menu(Menu *m){
    m->classico.ret.pos.x=250;
    m->classico.ret.pos.y=250;
    m->classico.ret.tam.larg=300;
    m->classico.ret.tam.alt=60;
    m->classico.texto="Modo Classico";

    m->normal.ret.pos.x=250;
    m->normal.ret.pos.y=350;
    m->normal.ret.tam.larg=300;
    m->normal.ret.tam.alt=60;
    m->normal.texto="Modo Normal";

    m->tutorial.ret.pos.x=250;
    m->tutorial.ret.pos.y=450;
    m->tutorial.ret.tam.larg=300;
    m->tutorial.ret.tam.alt=60;
    m->tutorial.texto="Tutorial";
}
void resolve_nano(Celula tabuleiro[][COLUNAS], int resultado[][COLUNAS]){
    for(int i = 0; i < LINHAS; i++)
    {
        for(int j = 0; j < COLUNAS; j++)
        {
            if(resultado[i][j] ==1){
                tabuleiro[i][j].estado = COLORIDO;
            }
            else{
                tabuleiro[i][j].estado = VAZIO;
                }
        }
    }
}
void dica_nono(Celula tabuleiro[][COLUNAS], int resultado[][COLUNAS])
{
    for(int i = 0; i < LINHAS; i++)
    {
        for(int j = 0; j < COLUNAS; j++)
        {
            if(resultado[i][j] == 1 && tabuleiro[i][j].estado != COLORIDO)
            {
                tabuleiro[i][j].estado = COLORIDO;
                return;
            }
        }
    }
}
void gera_resultado(int resultado[][COLUNAS])
{
    for(int i = 0;i<LINHAS; i++)
    {
        for(int j = 0;j<COLUNAS; j++)
        {
            resultado[i][j]=rand()%2;
        }
    }
    for(int i = 0;i<LINHAS; i++)
    {
    for(int j = 0;j<COLUNAS; j++)
    {
        printf("%d ",resultado[i][j]);
    }
    printf("\n");
}
printf("\n");
    
}
EstadoTela verifica_menu(Tela *t, Menu *m) {
    Ponto cursor = posicao_mouse(t);

    if (botao_clicado(t)) {
        if (ret_contains_pt(m->classico.ret, cursor)){
            t->_botao=false;
            return Jogo_Classico;
        }
        if (ret_contains_pt(m->normal.ret, cursor)){
            t->_botao=false;
            return Jogo_Normal;
        }
        if (ret_contains_pt(m->tutorial.ret, cursor)){
            t->_botao = false;
            return Tutorial;
        }
    }
    return MENU;
}


void desenha_menu(Tela *t, Menu *m){
    Cor preto={0, 0, 0};

    define_cor(t, preto);

    Ponto titulo = {300, 100};
    escreve_texto(t, titulo, "BEM VINDO A NONOGRAM");

    desenha_botao(t, &m->classico);
    desenha_botao(t, &m->normal);
    desenha_botao(t, &m->tutorial);
}
void desenha_tutorial(Tela *t) {//utilizado chatgpt
    char *texto[]={
        "No Nonogram, os numeros indicam grupos",
        "consecutivos de quadrados pintados.",
        "Quando ha mais de um numero, cada",
        "grupo deve ser separado por pelo",
        "menos um quadrado vazio.",
        "Comece pelas linhas e colunas",
        "com numeros maiores.",
        "Use as informacoes das linhas",
        "para confirmar as colunas",
        "e vice-versa.",
        "Repita ate revelar o desenho."
    };

    int x = 150;
    int y = 180;

    for(int i = 0; i < 11; i++){
        escreve_texto(t, (Ponto){x,y+i*30}, texto[i]);
    }
}
void desenha_botao(Tela *t, Botao *b){
    desenha_retangulo(b->ret, t, false);

    Ponto p={
        b->ret.pos.x+20,
        b->ret.pos.y+20
    };
    escreve_texto(t, p, (char *) b->texto);
}
void inicia_botoes_jogo(Botao *dica)
{
    dica->ret.pos.x=200;
    dica->ret.pos.y=650;
    dica->ret.tam.larg=150;
    dica->ret.tam.alt=50;
    dica->texto="Dica(3)";
}
void inicia_botoes_jogo1(Botao *reset){
    reset->ret.pos.x=400;
    reset->ret.pos.y=650;
    reset->ret.tam.larg=150;
    reset->ret.tam.alt=50;
    reset->texto="Limpar";
}
void inicia_botoes_jogo2(Botao *voltar){
    voltar->ret.pos.x=600;
    voltar->ret.pos.y=650;
    voltar->ret.tam.larg=150;
    voltar->ret.tam.alt=50;
    voltar->texto="Voltar";
}
void inicia_botoes_jogo3(Botao *desfazer){
    desfazer->ret.pos.x=650;
    desfazer->ret.pos.y=200;
    desfazer->ret.tam.larg=100;
    desfazer->ret.tam.alt=50;
    desfazer->texto="Desfazer";
}
void inicia_botoes_jogo4(Botao *reinicia){
    reinicia->ret.pos.x=300;
    reinicia->ret.pos.y=50;
    reinicia->ret.tam.larg=200;
    reinicia->ret.tam.alt=50;
    reinicia->texto="Novo nanogram";
}
void inicia_botoes_jogo5(Botao *resolver){
    resolver->ret.pos.x=30;
    resolver->ret.pos.y=50;
    resolver->ret.tam.larg=200;
    resolver->ret.tam.alt=50;
    resolver->texto="Resolver Nono";
}
void reseta_tabuleiro(Celula tabuleiro[][COLUNAS],int *topo){

    for(int i = 0; i < LINHAS; i++)
    {
        for(int j = 0; j < COLUNAS; j++)
        {
            tabuleiro[i][j].estado=VAZIO;
        }
    }
    if (topo != NULL)
    {
        *topo=0;
    }
}
EstadoTela verifica_botoes_jogo(Tela *t, Botao *voltar, Botao *reset, Botao *desfazer, Botao *reinicia, Botao *resolver,Botao *dica, EstadoTela estado_atual, int resultado[][COLUNAS], Celula tabuleiro_nonogram[][COLUNAS], jogada historico[], int *topo, int *vidas, int *dicas)
{
    Ponto cursor = posicao_mouse(t);
 
    if(botao_clicado(t)){
        if(ret_contains_pt(voltar->ret, cursor)){
            gera_resultado(resultado);
            reseta_tabuleiro(tabuleiro_nonogram, topo);
            if(vidas != NULL){
                *vidas=3;
            }
            if(dicas != NULL){
                *dicas=3;
            }
            t->_botao = false;
            return MENU;
        }
        if (reset != NULL && ret_contains_pt(reset->ret, cursor))
        {
            reseta_tabuleiro(tabuleiro_nonogram, topo);
            t->_botao = false;
        }
        if (desfazer != NULL && historico != NULL && topo != NULL && ret_contains_pt(desfazer->ret, cursor)){
            if (*topo > 0) {
                (*topo)--;
                int i = historico[*topo].linha;
                int j = historico[*topo].coluna;
                tabuleiro_nonogram[i][j].estado = historico[*topo].estado_anterior;
            }
            t->_botao = false;
        }
        if (reinicia != NULL && ret_contains_pt(reinicia->ret, cursor))
        {
            gera_resultado(resultado);
            reseta_tabuleiro(tabuleiro_nonogram, topo);
            t->_botao = false;
        }
        if (resolver != NULL && ret_contains_pt(resolver->ret, cursor))
        {
            resolve_nano(tabuleiro_nonogram, resultado);
            t->_botao = false;
        }
        if (dica != NULL && ret_contains_pt(dica->ret, cursor)){
        if (*dicas > 0)
        {
            dica_nono(tabuleiro_nonogram, resultado);
            (*dicas)--;
        }

        t->_botao = false;
        }
    }
    return estado_atual;
}
void calcula_linha(int resultado[][COLUNAS],int linha,int numeros[],int *qtd)
{
    int contador = 0;
    *qtd = 0;

    for(int j = 0; j < COLUNAS; j++)
    {
        if(resultado[linha][j]==1)
        {
            contador++;
        }
        else if(contador>0)
        {
            numeros[*qtd]=contador;
            (*qtd)++; 
            contador=0;
        }
    }
    if(contador > 0)
    {
        numeros[*qtd] = contador;
        (*qtd)++;
    }
}
void calcula_coluna(int resultado[][COLUNAS],int coluna,int numeros[],int *qtd)
{
    int contador=0;
    *qtd=0;

    for(int i=0;i<LINHAS;i++)
    {
        if(resultado[i][coluna]==1)
        {
            contador++;
        }
        else if(contador>0)
        {
            numeros[*qtd]=contador;
            (*qtd)++;
            contador=0;
        }
    }

    if(contador>0)
    {
        numeros[*qtd]=contador;
        (*qtd)++;
    }
}
void inicia_tabuleiro(Celula tabuleiro[][COLUNAS]){    
    for (int i = 0;i < LINHAS; i++){
        for (int j = 0; j < COLUNAS; j++){
            int x= MARGIN_HORIZONTAL+(LARGURA_CELULA*j); 
            int y=MARGIN_VERTICAL+(ALTURA_CELULA*i);
            Celula c={{x,y},{LARGURA_CELULA, ALTURA_CELULA},VAZIO};
            tabuleiro[i][j] = c;
        }
    }
}
void desenha_restantes(Tela *t,Celula tabuleiro[][COLUNAS],int resultado[][COLUNAS])
{
    char texto[50];

    sprintf(texto,"Faltam: %d",celulas_restantes(tabuleiro, resultado));
    escreve_texto(t,(Ponto){20, 20}, texto);
}
int celulas_restantes(Celula tabuleiro[][COLUNAS], int resultado[][COLUNAS])
{
    int total = 0;
    int corretas = 0;

    for(int i = 0; i < LINHAS; i++)
    {
        for(int j = 0; j < COLUNAS; j++)
        {
            if(resultado[i][j] == 1)
            {
                total++;
                if(tabuleiro[i][j].estado == COLORIDO)
                {
                    corretas++;
                }
            }
        }
    }
    return total - corretas;
}
 void desenha_tabuleiro(Tela *t, Celula tabuleiro[][COLUNAS], int resultado[][COLUNAS]) 
    {
        Cor preto = {0, 0, 0};
        Cor vermelho = {1, 0, 0};
        define_cor(t, preto); 

        for (int i = 0; i < LINHAS; i++){
            for (int j = 0; j < COLUNAS; j++) {
                desenha_retangulo(tabuleiro[i][j], t, tabuleiro[i][j].estado==COLORIDO);
                if (tabuleiro[i][j].estado == ALERTA){
                    define_cor(t, vermelho);
                    desenha_x_dentro_ret(tabuleiro[i][j], t);
                    define_cor(t, preto);
                }
            }
        }
        for (int i = 0; i < LINHAS; i++){
            int x = MARGIN_HORIZONTAL - 10;
            int y = MARGIN_VERTICAL + (i * ALTURA_CELULA) + 15;
            Ponto p = { x , y };

            int numeros[COLUNAS];
            int qtd;

            calcula_linha(resultado, i, numeros, &qtd);
            for(int k = 0; k < qtd; k++);{
            escreve_numeros(t, numeros, qtd, p, true);
        }
        }
        for (int j = 0; j < COLUNAS; j++) {
            int x = MARGIN_HORIZONTAL + (j * LARGURA_CELULA) + 15;
            int y = MARGIN_VERTICAL - 10;
            Ponto p = { x , y };

            int numeros[LINHAS];
            int qtd;
            calcula_coluna(resultado, j, numeros, &qtd);
            escreve_numeros(t, numeros, qtd, p, false);
        }
    }
void escreve_numeros(Tela *t, int *numeros, int qtd, Ponto inicio, bool horizontal){
    int x =inicio.x, y= inicio.y, offset = 20;
    if(horizontal)
    {
        for (int i=qtd-1; i>=0;i--){
            char buffer[10];
            sprintf(buffer,"%d",numeros[i]);

            x-=offset;

            Ponto p={x,y};
            escreve_texto(t, p, buffer);
        }
    }
    else
    {
        for (int i=qtd-1; i>=0;i--){
            char buffer[10];
            sprintf(buffer,"%d",numeros[i]);

            y-=offset;

            Ponto p={x,y};
            escreve_texto(t, p, buffer);
        }
    }
}
void verifica_cliqueclassico(Tela *t, Celula tabuleiro[][COLUNAS], jogada historico[], int *topo, int resultado[][COLUNAS], EstadoTela *estado){
    Ponto mouse = posicao_mouse(t);

    if (botao_clicado(t)){
        int x_clique=(mouse.x-MARGIN_HORIZONTAL);
        int y_clique=(mouse.y-MARGIN_VERTICAL);

        if (x_clique >= 0 && y_clique >= 0) {
            int j=(int)(x_clique/LARGURA_CELULA);
            int i=(int)(y_clique/ALTURA_CELULA); 

            if (i>=0 && i<LINHAS && j>=0 && j<COLUNAS){
                if (*topo < maxjogadas) {
                    historico[*topo].linha = i;
                    historico[*topo].coluna = j;
                    historico[*topo].estado_anterior = tabuleiro[i][j].estado;
                    (*topo)++;
                }
                cicla_estado_celula(&tabuleiro[i][j]);

                if (verifica_vitoria(tabuleiro, resultado))
                    *estado = vitoria_classico;
            }
        }
        t->_botao = false;
    }
}
void verifica_clique_normal(Tela *t, Celula tabuleiro[][COLUNAS], int resultado[][COLUNAS], EstadoTela *estado, int *vidas){
    Ponto mouse = posicao_mouse(t);
    if (botao_clicado(t)) {
    int j = (int)((mouse.x-MARGIN_HORIZONTAL)/LARGURA_CELULA);
    int i = (int)((mouse.y-MARGIN_VERTICAL)/ALTURA_CELULA);

        if (i >= 0 && i < LINHAS && j >= 0 && j < COLUNAS) {
            if (tabuleiro[i][j].estado == VAZIO){
                if (resultado[i][j] == 1){
                    tabuleiro[i][j].estado = COLORIDO;
                }
                else{
                    tabuleiro[i][j].estado = ALERTA;
                    (*vidas)--;
                    if (*vidas <= 0){
                        *estado = derrota_normal;
                        t->_botao = false;
                        return;
                    }
                }
            } else {
                cicla_estado_celula(&tabuleiro[i][j]);
            }
            if (verifica_vitoria(tabuleiro, resultado))
                *estado = vitoria_normal;
        }
        t->_botao = false;
    }
}
void cicla_estado_celula(Celula *c){
    switch (c->estado){
        case VAZIO: c->estado = COLORIDO;
            break;
        case COLORIDO: c->estado = ALERTA;
            break;
        case ALERTA: c->estado = VAZIO;
            break;
    }
}
bool verifica_vitoria(Celula tabuleiro[][COLUNAS], int resultado[][COLUNAS]){
    
    for(int i = 0; i < LINHAS; i++){
        int dicas_esperadas[COLUNAS];
        int qtd_esperada = 0;
        calcula_linha(resultado, i, dicas_esperadas, &qtd_esperada);
        int blocos_jogador[COLUNAS];
        int qtd_jogador = 0;
        int contador = 0;

        for (int j = 0; j < COLUNAS; j++){
            if (tabuleiro[i][j].estado == COLORIDO) {
                contador++;
            } 
            else if (contador > 0){
                blocos_jogador[qtd_jogador] = contador;
                qtd_jogador++;
                contador = 0;
            }
        }
        if(contador > 0){
            blocos_jogador[qtd_jogador] = contador;
            qtd_jogador++;
        }
        if(qtd_esperada != qtd_jogador) {
            return false;
        }
        for(int k = 0; k < qtd_esperada; k++){
            if (dicas_esperadas[k] != blocos_jogador[k]) {
                return false;
            }
        }
    }
    for (int j = 0; j < COLUNAS; j++){
        int dicas_esperadas[LINHAS];
        int qtd_esperada = 0;
        calcula_coluna(resultado, j, dicas_esperadas, &qtd_esperada);

        int blocos_jogador[LINHAS];
        int qtd_jogador = 0;
        int contador = 0;

        for(int i = 0; i < LINHAS; i++){
            if(tabuleiro[i][j].estado == COLORIDO) {
                contador++;
            }
            else if(contador > 0){
                blocos_jogador[qtd_jogador] = contador;
                qtd_jogador++;
                contador = 0;
            }
        }
        if (contador > 0) {
            blocos_jogador[qtd_jogador] = contador;
            qtd_jogador++;
        }

        if (qtd_esperada != qtd_jogador) {
            return false;
        }

        for (int k = 0; k < qtd_esperada; k++) {
            if (dicas_esperadas[k] != blocos_jogador[k]) {
                return false;
            }
        }
    }
    return true;
}
void desenha_vitoria(Tela *t){
    Cor verde ={0, 0.7, 0};
    define_cor(t, verde);
    Ponto p ={220, 300};
    Ponto p2 ={220, 370};
    escreve_texto(t, p, "Parabens!Voce venceu!");
    escreve_texto(t, p2, "Clique em voltar para jogar novamente");
}
void desenha_derrota(Tela *t){
    Cor vermelho={0.8, 0, 0};
    define_cor(t, vermelho);
    Ponto p={230, 300};
    Ponto p2={230, 370};
    escreve_texto(t, p, "Suas vidas acabaram, tente novamente");
    escreve_texto(t, p2, "Clique em voltar para jogar novamente");
}
void desenha_vidas(Tela *t, int vidas){
    Cor vermelho = {0.8, 0, 0};
    Cor cinza= {0.6, 0.6, 0.6};
    char buf[20];
    sprintf(buf, "Vidas: %d", vidas);
    define_cor(t, vidas > 0 ? vermelho : cinza);
    Ponto p = {20, 20};
    escreve_texto(t, p, buf);
}
