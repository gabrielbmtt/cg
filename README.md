Tarefa 1: Renderizando uma Esfera

Escrever um código, baseado na discussão das duas primeiras aulas, que pinte uma esfera no canvas.

Requisitos:

Janela de Visualização:

A largura da janela, em metros, será armazenada na variável wJanela.

A altura da janela, em metros, será armazenada na variável hJanela.

O centro da janela está sobre o eixo z do sistema de coordenadas na posição (0, 0, -dJanela) em metros. Assim, todos os pontos no plano da Janela terão coordenada z = -dJanela.

Observador (Olho):

Posicionado na origem do sistema de coordenadas: (0, 0, 0).

Esfera:

O raio da esfera deve ser armazenado na variável rEsfera.

O centro da esfera deve estar sobre o eixo z com coordenada z < -(dJanela + rEsfera).

A cor da esfera deve ser esfColor = (255, 0, 0).

Ambiente:

A cor de background deve ser cinza bgColor = (100, 100, 100).

Canvas (Matriz de Cores):

Defina o número de colunas nCol e o número de linhas nLin da matriz de cores da imagem.

nCol representa tanto o número de colunas na "tela de mosquito" que está presa na Janela, quanto o número de colunas do quadriculado que o pintor marcou a lápis no Canvas de pintura.

nLin, de maneira análoga, representa tanto o número de linhas na "tela de mosquito" quanto o número de linhas do quadriculado no Canvas de pintura.

Lógica de Renderização (Ray Casting):

Note que:

O loop aninhado mais externo é o loop de linhas: for (int l = 0; l < nLin; l++)

O loop aninhado mais interno é o loop de colunas: for (int c = 0; c < nCol; c++)

As dimensões dos retângulos da "tela de mosquito" são:

Dx = wJanela / nCol

Dy = hJanela / nLin

As coordenadas do ponto do centro de um retângulo da "tela de mosquito" correspondente ao retângulo (riscado a lápis) na posição (l, c) é dado por:

x = -wJanela / 2 + Dx / 2 + c * Dx

y = hJanela / 2 - Dy / 2 - l * Dy

Cada raio (semi-reta) parte do olho do pintor E = (0, 0, 0) e passa pelo ponto (x, y, -dJanela).

Caso 1: Se o raio enviado não tiver interseção com a esfera, armazene a cor de background (bgColor) na posição (l, c) da matriz de cores do Canvas.

Caso 2: Caso o raio tangencie ou tenha interseção plena com a esfera, armazene a cor da esfera (esfColor) na posição (l, c) da matriz de cores do Canvas.

------------------------------------

Tarefa 2: Adicionando Iluminação (Phong Shading)

Modificar o método da Tarefa 1 para que, caso haja interseção de um raio com a esfera, a cor retornada seja dada pela energia luminosa que vem do ponto de interseção $P_I$ em direção ao olho do observador. Essa energia luminosa é o resultado da interação entre a energia luminosa emitida pela fonte pontual e o material da esfera no ponto de interseção.

Ela é composta de duas parcelas: a reflexão DIFUSA ($I_d$) e a reflexão ESPECULAR ($I_e$), onde:

$I_d = (I_F \odot K) \times (l \cdot n)$

$I_e = (I_F \odot K) \times (v \cdot r)^m$

(Obs: $\odot$ denota multiplicação componente a componente (produto Hadamard), $\cdot$ denota produto escalar)

Atributos da Fonte de Luz Pontual:

Intensidade: I_F = (0.7, 0.7, 0.7)

Posição: P_F = (0, 5, 0) (Posição da fonte pontual situada a 5 metros acima do olho do observador)

------------------------------------

Tarefa 3: Expandindo a Cena (Múltiplos Objetos e Luz Ambiente)

Refaça a Tarefa 2 com a inclusão de dois planos. Siga a seguinte especificação:

Janela: 60cm x 60 cm (H_J = 60, W_J = 60)

Window (Canvas): 500 x 500 pixels (H_C = nLin = 500, W_C = nCol = 500)

Coordenada z da Janela: z = -d = -30cm

Esfera:

Raio: R = 40cm

Centro: C = (0, 0, -100cm)

Reflectividade: K_d = K_e = K_a = (0.7, 0.2, 0.2), Shininess m = 10

Plano do Chão:

Ponto conhecido: P_pi = (0, -R, 0)

Vetor unitário normal: n_bar = (0, 1, 0)

Reflectividade: K_d = K_a = (0.2, 0.7, 0.2), K_e = (0.0, 0.0, 0.0), Shininess m = 1

Plano de Fundo:

Ponto conhecido: P_pi = (0, 0, -200cm)

Vetor unitário normal: n_bar = (0, 0, 1)

Reflectividade: K_d = K_a = (0.3, 0.3, 0.7), K_e = (0.0, 0.0, 0.0), Shininess m = 1

Fonte Pontual:

Intensidade: I_F = (0.7, 0.7, 0.7)

Posição: P_F = (0, 60cm, -30cm)

Luz Ambiente:

Intensidade: I_A = (0.3, 0.3, 0.3)

Observações de Lógica:

Lembre-se que uma interseção só é válida se $t_i > 0$.

A cena é composta de uma esfera, dois planos, uma fonte de luz pontual e uma fonte de luz ambiente.

Quando um raio for lançado através da janela, ele pode intersectar os três objetos. O ponto de interseção visto pelo olho do observador é aquele que tiver o menor $t_i$ positivo.

Sombras: Quando o ponto de interseção visto estiver no plano do chão ou no plano de fundo, não calcule as contribuições difusa e especular sem antes verificar se o raio de sombra $P(s) = P_i + s \times \vec{l}$ está sendo obstruído pela esfera.

------------------------------------

Tarefa 4: Adicionando Objetos Complexos (Cilindro e Cone)

Na cena da Tarefa 3, inclua os seguintes objetos:

Cilindro:

Centro da base: Localizado no centro da esfera.

Raio da base: Igual a um terço do Raio da esfera.

Altura: Igual a três vezes o Raio da esfera.

Vetor-direção: d_cil = (-1/sqrt(3), 1/sqrt(3), -1/sqrt(3))

Material: Kd = Ke = Ka = (0.2, 0.3, 0.8)

Cone:

Centro da base: Localizado no centro do topo do cilindro.

Raio da base: 1.5 * Raio da esfera

Altura: (1/3) * Raio da base

Vetor-direção: d_cone = (-1/sqrt(3), 1/sqrt(3), -1/sqrt(3))

Material: Kd = Ke = Ka = (0.8, 0.3, 0.2)
