Tarefa 1:

Escreva um código, baseado na discussão das duas primeiras aulas, que pinte uma esfera no canvas. Os requisitos são os seguintes:
1) Defina uma janela através da qual o pintor verá a esfera 
>> A largura da janela, em metros, será armazenada na variável wJanela
>> A altura. da janela, em metros, será armazenada na variável  hJanela
>> O centro da janela está sobre o eixo z do sistema de coordenadas na posição (0, 0, - dJanela) em metros. Assim, todos os pontos no plano da Janela terão coordenada z = - dJanela.

2) O olho do pintor está na origem do sistema de coordenadas (0,0,0)


3) O raio da esfera deve ser armazenado na variável rEsfera

4) O centro da esfera deve estar sobre o eixo z com coordenada z < - (dJanela + rEsfera)

5) A cor da esfera deve ser esfColor = 255, 0, 0


6)  A cor de background deve ser cinza bgColor = 100, 100, 100

7) Defina o número de colunas nCol e o número de linhas nLin da matriz de cores da imagem.
>> nCol representa tanto o número de colunas na tela de mosquito que está presa na Janela, quanto o número de colunas do quadriculado que o pintor marcou a lápis no Canvas de pintura.
>> nLin, de maneira análoga, representa tanto o número de linhas na tela de mosquito que está presa na Janela, quanto o número de linhas do quadriculado que o pintor marcou a lápis no Canvas de pintura

Note que
O loop aninhado mais externo é o loop de linhas que vai de 0 até nLin - 1.  ( for int l = 0; l < nLin; l++)
O loop aninhado mais interno é o loop de colunas que vai de 0 até nCol -1  (for int c =0; c < nCol; c++)

As dimensões dos retângulos da tela de mosquito são:
Dx = wJanela/nCol
Dy = hJanela/nLin

As coordenadas do ponto do centro de um retângulo da tela de mosquito correspondente ao retângulo (riscado a lápis) na posição (l, c) é dado por

x = - wJanela/2. + Dx/2  + c*Dx
y =.  hJanela/2.  -  Dy/2  -  l*Dy

Cada raio (semi reta) parte do olho do pintor E = (0,0, 0) e  passa pelo ponto (x, y, -dJanela).

Caso o raio enviado não tenha interseção com a esfera, armazene a cor de background na posição (l, c) da matriz de cores do Canvas.
Caso o raio tangencie ou tenha interseção plena com a esfera, armazene a cor da esfera (esfColor) na posição (l, c) da matriz de cores do Canvas.
////////////////////////////////////////////////
Tarefa 2:
Modificar o método da Tarefa 01 para que, caso haja interseção de um raio com a esfera, a cor retornada seja dada pela energia luminosa que vem do ponto de interseção PI em direção ao olho do observador.  Essa energia luminosa é o resultado da interação entre a energia luminosa emitida pela fonte pontual e o material da esfera no ponto de interseção. 
Ela é composta de duas parcelas: a reflexão DIFUSA (I_d) e a reflexão Especular. (I_e), onde

I_d =( I_F@K)* (l . n)

I_e = (I_F@K)*(v . r)^m.

Use os seguintes atributos da fonte de luz pontual:

I_F = (0.7, 0.7, 0.7)  // Intensidade da fonte pontual

P_F = (0, 5, 0)   // Posição da fonte pontual situada a 5 metros acima do olho do observador.
////////////////////////////////////////////////
Tarefa 3:
Refaça a Tarefa 02 com a inclusão de dois planos. Siga a seguinte especificação:
1) Janela de 60cm x 60 cm (H_J = 60, W_J = 60) 
2) Window = Canvas de  500 x 500 pixels (H_C = nLin = 500, W_C = nCol = 500)
3) Coordenada z da Janela, z = -d = -30cm
4) Esfera com raio R = 40 cm e centro C = (0, 0, - 100cm). 
5) Reflectividade da esfera K_d = K_e = K_a = (0.7, 0.2, 0.2),  Shininess = m = 10
6) Plano do chão:  Ponto conhecido do plano P_pi = (0, - R, 0),  vetor unitário normal ao plano, n_bar = (0, 1, 0)
7) Reflectividade do plano do chão K_d = K_a = (0.2, 0.7, 0.2),  K_e = (0.0, 0.0, 0.0),  Shininess = m = 1
8 Plano de Fundo:  Ponto conhecido do plano P_pi = (0, 0, -200cm),  vetor unitário normal ao plano, n_bar = (0, 0, 1)
9) Reflectividade do plano do chão K_d = K_a = (0.3, 0.3, 0.7),  K_e = (0.0, 0.0, 0.0),  Shininess = m = 1
10) Fonte pontual: Intensidade da fonte  I_F = (0.7, 0.7, 0.7),  posição da fonte P_F = (0, 60cm, -30cm)
11) Luz ambiente: Intensidade I_A = (0.3, 0.3, 0.3)

Obs: 
1) Lembre-se que uma interseção só é válida se t_i > 0.  
2) A cena é composta de uma esfera, dois planos, uma fonte de luz pontual e uma fonte de luz ambiente. 
3) Quando um raio for lançado através da janela, ele pode intersectar os três objetos. Assim o ponto que interseção visto pelo olho do observador é aquele que tiver o menor t_i positivo.
4) Quando o ponto de interseção visto estiver no plano do chão ou no plano de fundo, não calcule as contribuições difusa e especular sem antes verificar se o raio P(s) = P_i + s*vetor_l  está sendo obstruído plea esfera.
////////////////////////////////////////////////
Tarefa 4:
Na cena da Tarefa 03, inclua os seguintes objetos
Cilindro:
>> Centro da base localizado no centro da esfera
>> Raio da base igual a im terço do Raio da esfera 
>> Altura do cilindro igual três vezes o Raio da esfera
>> Vetor-direção do cilindro d_cil = (-1/sqrt(3), 1/sqrt(3), -1/sqrt(3))
>> Kd = Ke = Ka = ( 0.2, 0.3, 0.8)
Cone:
>> Centro da base localizado no centro do topo do cilindro
>> Raio da base igual a 1.5*Raio da esfera
>> Altura do cone igual a (1/3)Raio da base
>> Vetor-direção do cilindro d_cil = (-1/sqrt(3), 1/sqrt(3), -1/sqrt(3))
>>  Kd = Ke = Ka = ( 0.8, 0.3, 0.2)
