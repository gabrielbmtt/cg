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
    //////////////////// Esfera


    //1) Defina uma janela através da qual o pintor verá a esfera
    double wJanela = 100.0; //A largura da janela, em metros, será armazenada na variável wJanela
    double hJanela = 100.0; //A altura. da janela, em metros, será armazenada na variável  hJanela
    double dJanela = 50.0;

    double olho_obsv[3] = { 0.0, 0.0, 0.0 }; //2) O olho do pintor está na origem do sistema de coordenadas (0,0,0)
    double rEsfera = 10.0; //3) O raio da esfera deve ser armazenado na variável rEsfera
    double centroEsfera[3] = { 0.0, 0.0, -80.0 }; //4) O centro da esfera deve estar sobre o eixo z com coordenada z < - (dJanela + rEsfera)

    double centro_Janela[3] = { 0.0, 0.0, -dJanela }; //Tarefa 2

    // reflexividade da esfera
    double reflexividade[3] = { 255, 0, 0 }; //Tarefa 2

    //Dimensoes dos retangulos na tela de mosquito
    double dx = wJanela / nCol;
    double dy = hJanela / nLin;

    double w[3] = {
        olho_obsv[0] - centroEsfera[0],
        olho_obsv[1] - centroEsfera[1],
        olho_obsv[2] - centroEsfera[2]
    };

    double fAmbiente[3] = {//Tarefa 2
        0.05, 0.05, 0.05 
    };

    double iAmbiente [3] = {//Tarefa 2
        reflexividade[0] * fAmbiente[0],                
        reflexividade[1] * fAmbiente[1],                
        reflexividade[2] * fAmbiente[2]                
    };

    //////////////////// fonte de luz

    double intensidade_FontedeLuz[3] = { 0.7, 0.7, 0.7 };
    double posicao_FontedeLuz[3] = { 0.0, 5.0, 0.0 };
    


    // todos os pontos no plano da Janela terão coordenada z = - dJanela
    double z = -dJanela;
    for (int l = 0; l < nLin; l++) { //Loop Linhas
        //Calcular a coord 'y' para a linha 'l'
        double y = hJanela/2.0 - dy/2.0 - l*dy; 

        for (int c = 0; c < nCol; c++) { //Loop Colunas
            //Calcular a coord 'x' para a coluna 'c'
            double x = -wJanela/2.0 + dx/2.0  + c*dx;

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
                //TAREFA 2

                //Ponto de Intersecao
                //resolver equacao de 2 grau para achar a distancia do olho ate o ponto de colisao/intersecao
                
                double t = (-bx - sqrt(delta)) / (2 * ax);//ponto de intersecao mais prox do observador

                //Tendo t podemos calcular as coord x,y,z de PI na superficie
                //PI = P0 + t*d
                double ponto_Intersecao[3] = {
                    olho_obsv[0] + dir[0] * t,
                    olho_obsv[1] + dir[1] * t,
                    olho_obsv[2] + dir[2] * t
                };

                //Calcular vetores de iluminacao
                //vetor l - aponta de PI para a fonte de luz

                //criado subtraindo a posicao inicial (ponto_Intersecao) da posicao final (posicao_FontedeLuz)
                double sourceMinusIntersection[3] = {
                    posicao_FontedeLuz[0] - ponto_Intersecao[0],
                    posicao_FontedeLuz[1] - ponto_Intersecao[1],
                    posicao_FontedeLuz[2] - ponto_Intersecao[2]
                };
                //comprimento = magnitude
                double sourceMinusIntersectionMagnitude = magnitude(sourceMinusIntersection, 3);
                //Normalizar = dividir vetor pela magnitude -> gera comprimento 1 -> vetor l
                double lx[3] = {
                    sourceMinusIntersection[0] / sourceMinusIntersectionMagnitude,
                    sourceMinusIntersection[1] / sourceMinusIntersectionMagnitude,
                    sourceMinusIntersection[2] / sourceMinusIntersectionMagnitude
                };

                //vetor n - Vetor NORMAL à superfície da esfera em PI
                //é aquele que aponta para fora e e faz 90 graus no ponto
                // Para uma esfera, ele é simplesmente o vetor do centro da esfera até o ponto de interseção, normalizado.
                double n[3] = {
                    (ponto_Intersecao[0] - centroEsfera[0]) / rEsfera,
                    (ponto_Intersecao[1] - centroEsfera[1]) / rEsfera,
                    (ponto_Intersecao[2] - centroEsfera[2]) / rEsfera
                };

                //vetor v - vetor visao que aponta de PI ate o olho do obsv
                //tem direcao oposta ao original e tambem precisa ser normalizado
                double directionMagnitude = magnitude(dir, 3);
                double v[3] = {
                    -dir[0] / directionMagnitude,
                    -dir[1] / directionMagnitude,
                    -dir[2] / directionMagnitude,
                };

                //vetor r - reflexao que aponta para onde o raio de luz l deveria ser refletido se a superficie (n) fosse um espelho
                //r = 2*(l.n) * n - l
                double lscalarProductN = scalarProduct(lx, n, 3); 
                double r[3] = {
                    2 * lscalarProductN * n[0] - lx[0],
                    2 * lscalarProductN * n[1] - lx[1],
                    2 * lscalarProductN * n[2] - lx[2]
                };

                //calculo dos fatores de iluminacao

                //Difuso - medir o quanto diretamente a superficie esta virada para a luz
                //(l . n) é maximo quando l e n estao alinhados
                //max(..., 0.0) para garantir que o fator nao seja negativo (se a luz estiver atras)
                double fDifusa = max(
                    scalarProduct(lx, n, 3),
                    0.0
                );

                //Especular - medir o quanto alinhado o reflexo da luz (r)) esta com a visao do obsv (v)
                //"Se estiverem perfeitamente alinhados, vemos um ponto de brilho intenso.""
                double fEspeculada = max(
                    scalarProduct(r, v, 3),
                    0.0
                );

                //cores

                //Difusa (I_d) é a cor principal do objeto na luz
                //"É a cor da luz (intensidade) interagindo com a cor do objeto (reflexividade), ponderada pelo Fator Difuso."
                double iDifusa[3] = {
                    intensidade_FontedeLuz[0] * reflexividade[0] * fDifusa,
                    intensidade_FontedeLuz[1] * reflexividade[1] * fDifusa,
                    intensidade_FontedeLuz[2] * reflexividade[2] * fDifusa
                };

                //Especular I_e é a cor do brilho
                //"Interação da cor da luz/objeto, ponderada pelo Fator Especular."
                double iEspeculada[3] = {
                    intensidade_FontedeLuz[0] * reflexividade[0] * fEspeculada,
                    intensidade_FontedeLuz[1] * reflexividade[1] * fEspeculada,
                    intensidade_FontedeLuz[2] * reflexividade[2] * fEspeculada
                };

                //Cor Final/total é a soma de todas as componentes de luz
                double colorRGBToPaint[3] = {
                    iDifusa[0] + iEspeculada[0] + iAmbiente[0],
                    iDifusa[1] + iEspeculada[1] + iAmbiente[1],
                    iDifusa[2] + iEspeculada[2] + iAmbiente[2]
                };

                //pintar com o setPaintColor
                //Usar min(..., 255) para garantir que o valor de cor nao ultrapasse o maximo permitido de 255
                setPaintColor(
                    renderer,
                    min((int)colorRGBToPaint[0], 255),
                    min((int)colorRGBToPaint[1], 255),
                    min((int)colorRGBToPaint[2], 255),
                    255
                );

                //Pinta o pixel na tela com a cor final calculada.
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

    setWindowBackground (renderer, 100, 100, 100, 255);
    
    //Desenhar a cena
    render_esfera (renderer);

    //Exibir o resultado na janela
    update (renderer);

    //Manter a janela aberta ate eu fechar
    listenEventQuit (window);
    return 0;
}