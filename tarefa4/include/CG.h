#pragma once

#include <vector>
#include <iostream>
#include <SDL3/SDL.h>

using std::ostream;

double min (double a, double b);
double max (double a, double b);



enum class ObjectType { SPHERE, PLAN, CYLINDER, CONE };
enum class ObjectRegion { SPHERE_SURFACE, CYLINDER_SURFACE, CYLINDER_BASE, CYLINDER_TOP, PLAN, CONE_SURFACE, CONE_BASE, UNKNOWN };

class Object;
class Light;
using ObjectsArray = std::vector<Object*>;
using LightsArray = std::vector<Light*>;

// vetor contendo 3 posicoes: x,y e z
class Vector {
    private:
        double positions[3];

    public:
        // normalizar vetor
        double getMagnitude ();

        double& operator [] (int index);
        Vector operator + (const Vector& operand);
        Vector operator + (const double& operand);
        Vector operator - (const Vector& operand);
        Vector operator - (const double& operand);
        Vector operator * (const Vector& operand);
        Vector operator * (const double& operand);
        Vector operator / (const Vector& operand);
        Vector operator / (const double& operand);
        
        Vector ();
        Vector (double x, double y, double z);
};

Vector vectorProduct (Vector a, Vector b);

// prod escalar de 2 vetores
double scalarProduct (Vector a, Vector b);
double scalarProduct (Vector *a, Vector *b);

class Color {
    public:
        // valor de R em RGB (maximo 255)
        int r;
        // valor de G em RGB (maximo 255)
        int g;
        // valor de B em RGB (maximo 255)
        int b;
        // valor de opacidade em RGB (maximo 255)
        int a;

        Color (int r, int g, int b, int a);
};

class Light {
    private:
        // intensidade da fonte de luz
        Vector* intensity = nullptr;

        // posicao da fonte de luz
        Vector* position = nullptr;

    public:
        // definir intensidade da fonte de luz
        void setIntensity (Vector* intensity);

        // definir posicao da fonte de luz
        void setPosition (Vector* position);

        // retorna intensidade da fonte deluz
        Vector* getIntensity ();

        //retorna posicao da fonte deluz
        Vector* getPosition();

        Light ();
        Light (Vector* intensity, Vector* position);
        ~Light();
};

class IntersectionResult {
    private:
        // booleano dizendo se acertou/interceptou objeto
        bool hasIntersection;

        // vetor contendo o ponto de intersecao
        Vector* intersectionPoint = nullptr;

        // distancia de P0 até intersectionPoint
        double distanceFromP0;

        // regiao do ojb acertada pelo ray
        ObjectRegion region;

    public:
        // set de booleano de intersecao com objeto
        void setHasIntersection (bool hasIntersection);

        // setar ponto de intersecao com objeto
        void setIntersectionPoint (Vector* intersectionPoint);

        // get booleano de intersecao com objeto
        bool getHasIntersection ();

        // get do ponto de intersecao com objeto
        Vector* getIntersectionPoint ();

        // setar distancia de P0 até intersectionPoint
        void setDistanceFromP0 (double distanceFromP0);

        // get da distancia de P0 até intersectionPoint
        double getDistanceFromP0 ();

        // setar regiao do obj
        void setObjectRegion (ObjectRegion region);

        // get da object region
        ObjectRegion getObjectRegion ();

        IntersectionResult operator = (const IntersectionResult& result);

        IntersectionResult ();
        IntersectionResult (bool hasIntersection, Vector* intersectionPoint, double distanceFromP0, ObjectRegion region);
        ~IntersectionResult ();
        IntersectionResult (const IntersectionResult& result);

};

class Ray {
    public:
        // ponto inicial
        Vector* P0;

        // direction of the Ray
        Vector* dir;

        Ray (Vector* P0, Vector* dir);
        ~Ray();
        
};

class Object {
    protected:
        ObjectType type;
        Vector* reflectivity;
        double shininess;
        
    public:
        virtual ObjectType getObjectType () = 0;
        virtual IntersectionResult* getIntersectionResult (Ray* ray) = 0;
        virtual Color* getColorToBePainted (
            IntersectionResult* intersectionResult,
            LightsArray lightsArray,
            ObjectsArray objectsArray,
            Ray* ray,
            Vector* environmentLight
        ) = 0;
};

class Sphere : public Object {
    private:
        // tipagem do objeto
        ObjectType type = ObjectType::SPHERE;

        // raio da esfera
        double radius;

        // reflexividade do material da esfera
        Vector* reflectivity;

        // vetor centro da esfera
        Vector* center;

        // brilho da esfera
        double shininess = 1.0;

    public:
        // get da tipagem do Objeto
        ObjectType getObjectType ();

        // setar raio da esfera
        void setRadius (double radius);

        // get raio da esfera
        double getRadius ();

        // setar reflexividade do material da esfera
        void setReflectivity (Vector* reflectivity);

        // get reflexividade do material da esfera
        Vector* getReflectivity ();

        // setarvetor centro da esfera (x,y,z)
        void setCenter (Vector* center);

        // get vetor centro da esfera (x,y,z)
        Vector* getCenter ();

        // setar brilho da esfera
        void setShininess (double shininess);

        // get brilho da esfera
        double getShininess ();

        // get intersecao do Ray com a Esfera
        IntersectionResult* getIntersectionResult (Ray* ray);

        // get cor
        Color* getColorToBePainted (
            IntersectionResult* intersectionResult,
            LightsArray lightsArray,
            ObjectsArray objectsArray,
            Ray* ray,
            Vector* environmentLight
        );

        Sphere ();
        Sphere (double radius, Vector* reflectivity, Vector* center, double shininess = 1.0);
        ~Sphere ();
        
};

class Plan : public Object {
    private:
        // tipagem do bojeto
        ObjectType type = ObjectType::PLAN;

        // ponto de referencia do plano
        Vector* initialPoint = nullptr;

        // reflexividade do material do plano
        Vector* reflectivity = nullptr;

        // vetor normal do plano
        Vector* normal = nullptr;

        // reflexividade do material do plano
        double shininess = 1.0;

    public:
        // get tipagem do Objeto
        ObjectType getObjectType ();

        // setar ponto de referencia do plano
        void setInitialPoint (Vector* initialPoint);

        // get ponto de referencia do plano
        Vector* getInitialPoint ();

        // setar vetor normal do plano
        void setNormal (Vector* normal);

        // get vetor normal do plano
        Vector* getNormal ();

        // setar reflexividade do material do plano
        void setReflectivity (Vector* reflectivity);

        // get reflexividade do material do plano
        Vector* getReflectivity ();

        // setar reflexividade do material do plano
        void setShininess (double shininess);

        // get reflexividade do material do plano
        double getShininess ();

        // get intersecao do plano com Ray
        IntersectionResult* getIntersectionResult (Ray* ray);

        // get cor
        Color* getColorToBePainted (
            IntersectionResult* intersectionResult,
            LightsArray lightsArray,
            ObjectsArray objectsArray,
            Ray* ray,
            Vector* environmentLight
        );

        Plan ();
        Plan (Vector* initialPoint, Vector* normal, Vector* reflectivity, double shininess = 1.0);
        ~Plan ();
        
};

// Tarefa 4 (cilindro)
class Cylinder : public Object {
    private:
        // tipagem do objeto
        ObjectType type = ObjectType::CYLINDER;

        // vetor centro da base do cilindro
        Vector* baseCenter = nullptr;

        // vetor centro do topo do cilindro
        Vector* topCenter = nullptr;

        // raio do cilindro
        double radius;

        // reflexividade do material do cilindro
        Vector* reflectivity = nullptr;

        // brilho do cilindro
        double shininess;

        // altura do cilindro
        double height;

        // vetor direção do eixo do cilindro
        Vector* direction = nullptr;

    public:
        // get tipagem do objeto
        ObjectType getObjectType ();

        // setar vetor centro da base
        void setBaseCenter (Vector* baseCenter);

        // get vetor centro da base
        Vector* getBaseCenter ();

        // setar vetor centro do topo
        void setTopCenter (Vector* topCenter);

        // get vetor centro do topo
        Vector* getTopCenter ();

        // setar raio do cilindro
        void setRadius (double radius);

        // get raio do cilindro
        double getRadius ();

        // setar reflexividade do material
        void setReflectivity (Vector* reflectivity);

        // get reflexividade do material
        Vector* getReflectivity ();

        // setar brilho do cilindro
        void setShininess (double shininess);

        // get brilho do cilindro
        double getShininess ();

        // setar altura do cilindro
        void setHeight (double height);

        // get altura do cilindro
        double getHeight ();

        // setar vetor direcao
        void setDirection (Vector* vector);

        // get vetor direcao
        Vector* getDirection ();

        // get intersecao do cilindro com Ray
        IntersectionResult* getIntersectionResult (Ray* ray);

        // get cor a ser pintada
        Color* getColorToBePainted (
            IntersectionResult* intersectionResult,
            LightsArray lightsArray,
            ObjectsArray objectsArray,
            Ray* ray,
            Vector* environmentLight
        );

        Cylinder ();
        Cylinder (Vector* baseCenter, Vector* topCenter, double radius, Vector* reflectivity, double shininess = 1.0);
        Cylinder (Vector* baseCenter, Vector* direction, double height, double radius, Vector* reflectivity, double shininess = 1.0);
        ~Cylinder ();
};

// Tarefa 4 (cone)
class Cone : public Object {
    private:
        // tipagem do objeto
        ObjectType type = ObjectType::CONE;

        // vetor centro da base do cone
        Vector* baseCenter = nullptr;

        // vetor do vertice do cone (tipo a pontinha dele)
        Vector* top = nullptr; 

        // raio da base do cone
        double radius;

        // altura do cone
        double height;

        // vetor direcaoo do eixo do cone
        Vector* direction = nullptr;

        // brilho do cone
        double shininess;

        // cosseno ao quadrado do angulo do cone (para otimizar cálculos)
        double cos2angle;

        // reflexividade do material do cone
        Vector* reflectivity = nullptr;

    public:
        // get tipagem do objeto
        ObjectType getObjectType ();

        // get vetor centro da base
        Vector* getBaseCenter ();

        // setar vetor centro da base
        void setBaseCenter (Vector* baseCenter);

        // get vetor do vértice (ponta)
        Vector* getTop ();

        // setar vetor do vértice (ponta)
        void setTop (Vector* top);

        // get raio da base
        double getRadius ();

        // setar raio da base
        void setRadius (double radius);

        // get altura do cone
        double getHeight ();

        // setar altura do cone
        void setHeight (double height);

        // get vetor direção
        Vector* getDirection ();

        // setar vetor direção
        void setDirection (Vector* direction);

        // get cosseno ao quadrado do ângulo
        double getCos2Angle ();

        // setar cosseno ao quadrado do ângulo
        void setCos2Angle (double cos2angle);

        // get brilho do cone
        double getShininess ();

        // setar brilho do cone
        void setShininess (double shininess);

        // get reflexividade do material
        Vector* getReflectivity ();

        // setar reflexividade do material
        void setReflectivity (Vector* reflectivity);

        // get intersecao do cone com Ray
        IntersectionResult* getIntersectionResult (Ray* ray);

        // get cor a ser pintada
        Color* getColorToBePainted (
            IntersectionResult* intersectionResult,
            LightsArray lightsArray,
            ObjectsArray objectsArray,
            Ray* ray,
            Vector* environmentLight
        );

        Cone ();
        Cone (Vector* baseCenter, Vector* top, double radius, Vector* reflectivity, double shininess);
        Cone (Vector* baseCenter, Vector* direction, double height, double radius, Vector* reflectivity, double shininess = 1.0);
        ~Cone ();
};
class Scene {
    private:
        // posicao do olho do observador
        Vector* eyeCenter = nullptr;

        // altura da janela
        double windowHeight = 100.0;

        // largura da janela
        double windowWidth = 100.0;

        // altura canvas
        int canvasHeight = 400;

        // largura canvas
        int canvasWidth = 400;

        // distancia d da janela ate olho do observador z = -d
        double windowDistance = 50.0;

        // setar objetos na cena
        ObjectsArray objects;

        // setar luzes na cena
        LightsArray lights;

        // luz ambiente da cena
        Vector* environmentLight = nullptr;

        // cor de fundo da cena; se o raio nao interceptar nenhum objeto pinta o pixel com a cor padrao
        Color* backgroundColor = nullptr;

        // lança o raio em cada um dos pixels do canvas e verifica se houve intersecao, caso tenha, pinta o pixel 
        void raycast(SDL_Renderer* renderer);

    public:
        // setar posicao do olho do obsv
        void setEyeCenter (Vector* eyeCenter);

        // setar altura da janela
        void setWindowHeight (double windowHeight);

        // setar largura da janela
        void setWindowWidth (double windowWidth);

        // setar largura da canvas
        void setCanvasWidth (double canvasWidth);

        // setar altura da canvas
        void setCanvasHeight (double canvasHeight);

        // setar distancia da janela ao olho
        void setWindowDistance (double windowDistance);

        // setar cor de fundo
        void setBackgroundColor (Color* color);

        // setar luz ambiente
        void setEnvironmentLight (Vector* environmentLight);

        // adicionar fonte de luz
        void addLightSource (Light* lightSource);

        // adicionar objeto na cena
        void addObject (Object* object);

        //get ponteiro para o vetor que representa a posicao do olho do observador
        Vector* getEyeCenter ();

        // get altura da janela
        double getWindowHeight ();

        // get largura da janela
        double getWindowWidth ();

        // get largura do canvas
        double getCanvasWidth ();

        // get altura do canvas
        double getCanvasHeight ();

        // get distancia da janela ate o olho do obsv
        double getWindowDistance ();

        // get vetor da luz ambientye
        Vector* getEnvironmentLight ();

        // get copia do vetor com ponteiros das fontes de luzes
        LightsArray getLights ();

        // get copia do vetor com ponteiros dos objetos
        ObjectsArray getObjects ();

        // abrir janela e renderizar
        void render ();

        Scene ();

        // eyeCenter: vetor que representa posicao do olho na cena
        // windowHeight: altura da janela
        // window width: largura da janela
        // canvasHeight: altura do canvas
        // canvasWidth: largura do canvas
        // windowDistance: distancia da janela ate o olho
        // color: cor de fundo do canvas
        Scene (
            Vector* eyeCenter,
            double windowHeight,
            double windowWidth,
            int canvasHeight,
            int canvasWidth,
            double windowDistance,
            Color* backgroundColor = nullptr
        );

        ~Scene ();

};