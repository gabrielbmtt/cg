#include <iostream>
#include <string>
#include "./include/CG.h"
#include "./include/utils.impl.h"
#include "./include/pixels.h"
#include <cmath>



int main () {
    //4-5) Esfera
    Sphere* esfera = new Sphere (
        40.0, //4) Esfera com raio R = 40 cm
        new Vector (0.7, 0.2, 0.2), //5) Reflectividade da esfera K_d = K_e = K_a = (0.7, 0.2, 0.2)
        new Vector (0, 0, -100), //4) centro C = (0, 0, - 100cm). 
        10.0 //5) Shininess = m = 10
    );


    //6-7 Plano do chão  
    Plan* plano = new Plan (
        new Vector (0, -40, 0), //6) Ponto conhecido do plano P_pi = (0, - R, 0)
        new Vector (0, 1, 0), //6) vetor unitário normal ao plano  n_bar = (0, 1, 0)
        new Vector (0.2, 0.7, 0.2), //7) Reflectividade do plano do chão K_d = K_a = (0.2, 0.7, 0.2),  K_e = (0.0, 0.0, 0.0)
        1.0 //7) Shininess = m = 1
    );
    //8-9 Plano de fundo
    Plan* plano_2 = new Plan (
        new Vector (0, 0, -200), // 8) Ponto conhecido do plano P_pi = (0, 0, -200cm)
        new Vector (0, 0, 1), // 8) vetor unitário normal ao plano, n_bar = (0, 0, 1)
        new Vector (0.3, 0.3, 0.7), // 9) Reflectividade do plano do chão K_d = K_a = (0.3, 0.3, 0.7),  K_e = (0.0, 0.0, 0.0)
        1.0 // 9) Shininess = m = 1
    );

    //10 Fonte pontual
    Light* luz = new Light (
        new Vector (0.7, 0.7, 0.7), // Intensidade da fonte  I_F = (0.7, 0.7, 0.7)
        new Vector (0, 60, -30) // posição da fonte P_F = (0, 60cm, -30cm)
    );

    Sp<Scene> cena = new Scene (
        new Vector (0, 0, 0),
        60.0, 60.0,
        500, 500,
        30,
        new Color (100, 100, 100, 255)
    );
    // Tarefa 4 (cone e cilindro adicionados)
    Cylinder* cylinder = new Cylinder (
        new Vector (0, 0, -100), // Centro da base localizado no centro da esfera
        new Vector (-1 / sqrt(3), 1 / sqrt(3), -1 / sqrt(3)), // Vetor-direção do cilindro d_cil = (-1/sqrt(3), 1/sqrt(3), -1/sqrt(3))
        120.0, // Altura do cilindro igual três vezes o Raio da esfera
        40.0 / 3.0, // Raio da base igual a im terço do Raio da esfera 
        new Vector (0.2, 0.3, 0.8), // Kd = Ke = Ka = ( 0.2, 0.3, 0.8)
        5.0
    );

    Cone* cone = new Cone (
        new Vector (*cylinder->getTopCenter ()), // Centro da base localizado no centro do topo do cilindro
        new Vector (-1 / sqrt(3), 1 / sqrt(3), -1 / sqrt(3)), // Vetor-direção do cilindro d_cil = (-1/sqrt(3), 1/sqrt(3), -1/sqrt(3))
        20.0, // Altura do cone igual a (1/3)Raio da base
        60.0, // Raio da base igual a 1.5*Raio da esfera
        new Vector (0.8, 0.3, 0.2),// Kd = Ke = Ka = ( 0.2, 0.3, 0.8)
        5.0 // Kd = Ke = Ka = ( 0.8, 0.3, 0.2)
    );



    //11) luz ambiente
    cena->setEnvironmentLight (
        new Vector (0.3, 0.3, 0.3) // 11) Luz ambiente: Intensidade I_A = (0.3, 0.3, 0.3)
    );

    cena->addLightSource(luz);
    cena->addObject(esfera);
    cena->addObject(plano);
    cena->addObject(plano_2);
    cena->addObject(cylinder);
    cena->addObject(cone);
    cena->render();

    return 0;
}