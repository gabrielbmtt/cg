#include <iostream>
#include <cmath>
#include <SDL3/SDL.h>
#include "pixels.h"
#include "matrix.h"

#define nCol 700.0 //>> nCol representa tanto o número de colunas na tela de mosquito que está presa na Janela, 
//quanto o número de colunas do quadriculado que o pintor marcou a lápis no Canvas de pintura.
#define nLin 700.0 //>> nLin, de maneira análoga, representa tanto o número de linhas na tela de mosquito que está presa na Janela, 
//quanto o número de linhas do quadriculado que o pintor marcou a lápis no Canvas de pintura


void render_esfera (SDL_Renderer* renderer) {
    //1) Defina uma janela através da qual o pintor verá a esfera
    double wJanela = 100.0; //A largura da janela, em metros, será armazenada na variável wJanela
    double hJanela = 100.0; //A altura. da janela, em metros, será armazenada na variável  hJanela
    double dJanela = 50.0;

    double olho_obsv[3] = { 0.0, 0.0, 0.0 }; //2) O olho do pintor está na origem do sistema de coordenadas (0,0,0)
    double rEsfera = 10.0; //3) O raio da esfera deve ser armazenado na variável rEsfera
    double centroEsfera[3] = { 0.0, 0.0, -80.0 }; //4) O centro da esfera deve estar sobre o eixo z com coordenada z < - (dJanela + rEsfera)

    //Dimensoes dos retangulos na tela de mosquito
    double dx = wJanela / nCol;
    double dy = hJanela / nLin;

    double w[3] = {
        olho_obsv[0] - centroEsfera[0],
        olho_obsv[1] - centroEsfera[1],
        olho_obsv[2] - centroEsfera[2]
    };
    // todos os pontos no plano da Janela terão coordenada z = - dJanela
    double z = -dJanela;
    for (int l = 0; l < nLin; l++) { //Loop Linhas
        //Calcular a coord 'y' para a linha 'l'
        double y = hJanela / 2.0 -  dy / 2.0  - l * dy; 

        for (int c = 0; c < nCol; c++) { //Loop Colunas
            //Calcular a coord 'x' para a coluna 'c'
            double x = -wJanela / 2.0 + dx / 2.0  + c*dx;

            //Definir o vetor direção do raio: do olho (0,0,0) passando pelo ponto (x,y,z)
            double dir[3] = { x, y, z };

            //Calcular os coeficientes a, b, c da equacao de 2 grau para a intersecao raio - esfera
            double ax = scalarProduct (dir, dir , 3);
            double bx = 2 * scalarProduct (w, dir, 3);
            double cx = scalarProduct(w, w, 3) - pow (rEsfera, 2.0);

            //Delta/Discriminante da funcao de 2 grau
            double delta = pow (bx, 2.0) - 4*ax*cx;

            //Intersecao
            if (delta >= 0) {
                //5) A cor da esfera deve ser esfColor = 255, 0, 0
                //Caso o raio tangencie ou tenha interseção plena com a esfera, armazene a cor da esfera (esfColor) na posição (l, c) da matriz de cores do Canvas.
                setPaintColor(renderer, 255, 0, 0, 255);
                paintPixel(renderer, c, l);
            } else {
                //6)  A cor de background deve ser cinza bgColor = 100, 100, 100
                //Caso o raio enviado não tenha interseção com a esfera, armazene a cor de background na posição (l, c) da matriz de cores do Canvas.
                setPaintColor(renderer, 100, 100, 100, 255);
                paintPixel(renderer, c, l);
            }
        }
    }
} 


int main(int argc, char *argv[]) {
    SDL_Window* window = nullptr;
    SDL_Renderer* renderer = nullptr;

    //Preparar ambiente SDL -> cria a janela e o renderizador
    initializeSDLAndWindow (&window, &renderer, nCol, nLin);

    //Desenhar a cena
    render_esfera (renderer);

    //Exibir o resultado na janela
    update (renderer);

    //Manter a janela aberta ate eu fechar
    listenEventQuit (window);
    return 0;
}