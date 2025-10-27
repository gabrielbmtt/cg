#include <iostream>
#include <cmath>
#include <vector>
#include "../include/utils.impl.h"
#include "../include/CG.h"
#include "../include/pixels.h"

// produto vetorial 
Vector vectorProduct (Vector a, Vector b) {
    return Vector (
        a[1] * b[2] - a[2] * b[1],
        a[2] * b[0] - a[0] * b[2],
        a[0] * b[1] - a[1] * b[0]
    );
}

// minimo entre 2 numeros
double min (double a, double b) {
    if (a < b) {
        return a;
    }

    return b;
}

//maximo entre 2 numeros
double max (double a, double b) {
    if (a > b) {
        return a;
    }

    return b;
}

void Scene::setEyeCenter (Vector* eyeCenter) {
    this->eyeCenter = eyeCenter;
}

void Scene::setWindowHeight (double windowHeight) {
    this->windowHeight = windowHeight;
}

void Scene::setWindowWidth (double windowWidth) {
    this->windowWidth = windowWidth;
}

void Scene::setCanvasWidth (double canvasWidth) {
    this->canvasWidth = canvasWidth;
}

void Scene::setCanvasHeight (double canvasHeight) {
    this->canvasHeight = canvasHeight;
}

void Scene::setWindowDistance (double windowDistance) {
    this->windowDistance = windowDistance;
}

void Scene::setBackgroundColor (Color* color) {
    this->backgroundColor = color;
}

void Scene::setEnvironmentLight (Vector* environmentLight) {
    this->environmentLight = environmentLight;
}

void Scene::addLightSource (Light* lightSource) {
    this->lights.push_back (lightSource);
}

void Scene::addObject (Object* object) {
    this->objects.push_back (object);
}

Vector* Scene::getEyeCenter () {
    return this->eyeCenter;
}

double Scene::getWindowHeight () {
    return this->windowHeight;
}

double Scene::getWindowWidth () {
    return this->windowWidth;
}

double Scene::getCanvasWidth () {
    return this->canvasWidth;
}

double Scene::getCanvasHeight () {
    return this->canvasHeight;
}

double Scene::getWindowDistance () {
    return this->windowDistance;
}

Vector* Scene::getEnvironmentLight () {
    return this->environmentLight;
}

void Scene::raycast (SDL_Renderer* renderer) {
    const double nLin = this->getCanvasHeight ();
    const double nCol = this->getCanvasWidth ();

    const double hJanela = this->getWindowHeight ();
    const double wJanela = this->getWindowWidth ();

    const double dx = wJanela / nCol;
    const double dy = hJanela / nLin;

    int numberOfObjects = this->objects.size();

    const double z = -this->getWindowDistance();

    for (int l = 0; l < nLin; l++) {
        const double y = hJanela / 2.0 - dy / 2.0 - l * dy;

        for (int c = 0; c < nCol; c++) {
            const double x = -wJanela / 2.0 + dx / 2.0 + c * dx;

            Vector* P0 = new Vector (*this->getEyeCenter());
            Vector* direction = new Vector (x, y, z);
            Sp<Ray> ray = new Ray (P0, direction);

            int nearestObjectIndex = 0;
            Sp<IntersectionResult> nearestResult = new IntersectionResult (false, nullptr, 0, ObjectRegion::UNKNOWN);

            for (int i = 0; i < numberOfObjects; i++) {
                Sp<IntersectionResult> result = this->objects[i]->getIntersectionResult (ray.pointer);
                if (result->getHasIntersection() &&
                (!nearestResult->getHasIntersection() || result->getDistanceFromP0() < nearestResult->getDistanceFromP0())) {
                    delete nearestResult.pointer;
                    nearestResult.pointer = result.pointer;
                    result.pointer = nullptr;
                    nearestObjectIndex = i;
                }
            }
            if (nearestResult->getHasIntersection()) {
                Sp<Color> colorToPaint = this->objects[nearestObjectIndex]->getColorToBePainted (
                    nearestResult.pointer,
                    this->lights,
                    this->objects,
                    ray.pointer,
                    this->environmentLight
                );
                setPaintColor (renderer, colorToPaint->r, colorToPaint->g, colorToPaint->b, colorToPaint->a);
                paintPixel (renderer, c, l);
            }
        }
    }
}

void Scene::render () {
    SDL_Renderer* renderer = nullptr;
    SDL_Window* window = nullptr;

    initializeSDLAndWindow (&window, &renderer, this->getCanvasHeight(), this->getCanvasWidth());

    if (this->backgroundColor == nullptr) {
        std::cout << "backgroundColor is null" << std::endl;
    }

    setWindowBackground (
        renderer,
        this->backgroundColor->r,
        this->backgroundColor->g,
        this->backgroundColor->b,
        this->backgroundColor->a
    );

    this->raycast(renderer);

    update (renderer);
    listenEventQuit (window);
}

LightsArray Scene::getLights () {
    return this->lights;
}

ObjectsArray Scene::getObjects () {
    return this->objects;
}

Scene::Scene () {}

Scene::Scene (
    Vector* eyeCenter,
    double windowHeight,
    double windowWidth,
    int canvasHeight,
    int canvasWidth,
    double windowDistance,
    Color* color
) {
    this->setEyeCenter (eyeCenter);
    this->setWindowHeight (windowHeight);
    this->setWindowWidth (windowWidth);
    this->setCanvasHeight (canvasHeight);
    this->setCanvasWidth (canvasWidth);
    this->setWindowDistance (windowDistance);
    
    if (color == nullptr) {
        this->setBackgroundColor (new Color (0, 0, 0, 255));
    } else {
        this->setBackgroundColor (color);
    }
}

Scene::~Scene () {
    delete this->getEyeCenter();
    
    for (auto i = this->objects.begin(); i != this->objects.end(); i++) {
        delete (*i);
    }

    for (auto i = this->lights.begin(); i != this->lights.end(); i++) {
        delete (*i);
    }
}

Color::Color (int r, int g, int b, int a) {
    this->r = r > 255 ? 255 : r; 
    this->g = g > 255 ? 255 : g; 
    this->b = b > 255 ? 255 : b; 
    this->a = a > 255 ? 255 : a; 
}

Vector::Vector () {}

Vector::Vector (double x, double y, double z) {
    this->positions[0] = x;
    this->positions[1] = y;
    this->positions[2] = z;
}

double& Vector::operator [] (int idx) {
    return this->positions[idx % 3];
}

Vector Vector::operator + (const Vector& operand) {
    Vector result (
        this->positions[0] + operand.positions[0],
        this->positions[1] + operand.positions[1],
        this->positions[2] + operand.positions[2]
    );

    return result;
}

Vector Vector::operator + (const double& operand) {
    Vector result (
        this->positions[0] + operand,
        this->positions[1] + operand,
        this->positions[2] + operand
    );

    return result;
}

Vector Vector::operator - (const Vector& operand) {
    Vector result (
        this->positions[0] - operand.positions[0],
        this->positions[1] - operand.positions[1],
        this->positions[2] - operand.positions[2]
    );

    return result;
}

Vector Vector::operator - (const double& operand) {
    Vector result (
        this->positions[0] - operand,
        this->positions[1] - operand,
        this->positions[2] - operand
    );

    return result;
}

Vector Vector::operator * (const Vector& operand) {
    Vector result (
        this->positions[0] * operand.positions[0],
        this->positions[1] * operand.positions[1],
        this->positions[2] * operand.positions[2]
    );

    return result;
}

Vector Vector::operator * (const double& operand) {
    Vector result (
        this->positions[0] * operand,
        this->positions[1] * operand,
        this->positions[2] * operand
    );

    return result;
}

Vector Vector::operator / (const Vector& operand) {
    Vector result (
        this->positions[0] / operand.positions[0],
        this->positions[1] / operand.positions[1],
        this->positions[2] / operand.positions[2]
    );

    return result;
}

Vector Vector::operator / (const double& operand) {
    Vector result (
        this->positions[0] / operand,
        this->positions[1] / operand,
        this->positions[2] / operand
    );

    return result;
}

double Vector::getMagnitude () {
    double result = 0;

    for (int i = 0; i < 3; i++) {
        result += pow (this->positions[i], 2.0);
    }

    return sqrt (result);
}

double scalarProduct (Vector a, Vector b)  {
    double result = 0;

    for (int i = 0; i < 3; i++) {
        result += a[i] * b[i];
    }

    return result;
}

double scalarProduct (Vector *a, Vector *b)  {
    double result = 0;

    for (int i = 0; i < 3; i++) {
        result += (*a)[i] * (*b)[i];
    }

    return result;
}

void Light::setIntensity (Vector* intensity) {
    this->intensity = intensity;
}

void Light::setPosition (Vector* position) {
    this->position = position;
}

Vector* Light::getIntensity () {
    return this->intensity;
}

Vector* Light::getPosition () {
    return this->position;
}

Light::Light () {}

Light::Light (Vector* intensity, Vector* position) {
    this->setIntensity(intensity);
    this->setPosition(position);
}

Light::~Light () {
    delete this->getPosition();
    delete this->getIntensity();
}

Ray::Ray (Vector* P0, Vector* dir) {
    this->P0 = P0;
    this->dir = dir;
}

Ray::~Ray () {
    delete this->P0;
    delete this->dir;
}

void IntersectionResult::setHasIntersection (bool hasIntersection) {
    this->hasIntersection = hasIntersection;
}

bool IntersectionResult::getHasIntersection () {
    return this->hasIntersection;
}

void IntersectionResult::setIntersectionPoint (Vector* intersectionPoint) {
    this->intersectionPoint = intersectionPoint;
}

Vector* IntersectionResult::getIntersectionPoint () {
    return this->intersectionPoint;
}

void IntersectionResult::setDistanceFromP0 (double distanceFromP0) {
    this->distanceFromP0 = distanceFromP0;
}

double IntersectionResult::getDistanceFromP0 () {
    return this->distanceFromP0;
}

void IntersectionResult::setObjectRegion (ObjectRegion region) {
    this->region = region;
}

ObjectRegion IntersectionResult::getObjectRegion () {
    return this->region;
}

IntersectionResult IntersectionResult::operator = (const IntersectionResult& result) {
    this->setDistanceFromP0(result.distanceFromP0);
    this->setHasIntersection(result.hasIntersection);
    
    if (this->getIntersectionPoint() != nullptr && result.intersectionPoint != nullptr) {
        *this->getIntersectionPoint() = *result.intersectionPoint;
    }

    return *this;
}

IntersectionResult::IntersectionResult () {}

IntersectionResult::IntersectionResult (bool hasIntersection, Vector* intersectionPoint, double distanceFromP0, ObjectRegion region) {
    this->setHasIntersection (hasIntersection);
    this->setIntersectionPoint (intersectionPoint);
    this->setDistanceFromP0 (distanceFromP0);
    this->setObjectRegion (region);
}

IntersectionResult::IntersectionResult (const IntersectionResult& result) {
    this->distanceFromP0 = result.distanceFromP0;
    this->hasIntersection = result.hasIntersection;
    
    if (result.intersectionPoint != nullptr) {
        this->intersectionPoint = new Vector (*result.intersectionPoint);
    }
}

IntersectionResult::~IntersectionResult () {
    delete this->getIntersectionPoint();
}

ObjectType Sphere::getObjectType () {
    return this->type;
}

void Sphere::setRadius (double radius) {
    this->radius = radius;
}

double Sphere::getRadius () {
    return this->radius;
}

void Sphere::setReflectivity (Vector* reflectivity) {
    this->reflectivity = reflectivity;
}

Vector* Sphere::getReflectivity () {
    return this->reflectivity;
}

void Sphere::setCenter (Vector* center) {
    this->center = center;
}

Vector* Sphere::getCenter () {
    return this->center;
}

void Sphere::setShininess (double shininess) {
    this->shininess = shininess;
}

double Sphere::getShininess () {
    return this->shininess;
}

Sphere::Sphere () {}

Sphere::Sphere (double radius, Vector* reflectivity, Vector* center, double shininess) {
    this->setRadius (radius);
    this->setReflectivity (reflectivity);
    this->setCenter (center);
    this->setShininess (shininess);
}

Sphere::~Sphere () {
    delete this->getReflectivity();
    delete this->getCenter();
}

IntersectionResult* Sphere::getIntersectionResult (Ray* ray) {

    Vector w = *(ray->P0) - *(this->getCenter());

    double a = scalarProduct (ray->dir, ray->dir);
    double b = 2 * scalarProduct (w, *(ray->dir));
    double c = scalarProduct (w, w) - pow (this->getRadius(), 2.0);

    double discriminant = (pow (b, 2.0) - 4 * a * c);

    double t;
    IntersectionResult* result = new IntersectionResult ();
    result->setObjectRegion (ObjectRegion::SPHERE_SURFACE);

    if (discriminant == 0) {
        result->setHasIntersection (true);
        t = (-b + sqrt(discriminant)) / (2 * a);

        // ignora intersecao se  ocorrer atras do observador/camera -> t < 0
        // 1) Lembre-se que uma interseção só é válida se t_i > 0.
        if (t < 0) {
            result->setHasIntersection (false);
            return result;
        }

        Vector* intersectionPoint = new Vector();
        *intersectionPoint = (*ray->P0) + (*ray->dir) * t;
        result->setIntersectionPoint (intersectionPoint);

        Vector distanceFromP0Vector = (*ray->P0) - (*intersectionPoint);
        double distanceBetweenP0AndIntersection = distanceFromP0Vector.getMagnitude();
        result->setDistanceFromP0 (distanceBetweenP0AndIntersection);

    } else if (discriminant > 0) {
        result->setHasIntersection (true);

        double t1 = (-b + sqrt(discriminant)) / (2 * a);

        // ignora intersecao se  ocorrer atras do observador/camera -> t < 0
        //1) Lembre-se que uma interseção só é válida se t_i > 0.
        if (t1 < 0) {
            result->setHasIntersection (false);
            return result;
        }

        double t2 = (-b - sqrt(discriminant)) / (2 * a);

        Vector intersectionPoint1 = (*ray->P0) + (*ray->dir) * t1;
        Vector intersectionPoint2 = (*ray->P0) + (*ray->dir) * t2;

        double distanceP0toT1 = intersectionPoint1.getMagnitude ();
        double distanceP0toT2 = intersectionPoint2.getMagnitude ();

        Vector* intersectionPoint = new Vector ();

        if (distanceP0toT1 < distanceP0toT2) {

            *intersectionPoint = intersectionPoint1;
            result->setDistanceFromP0 (distanceP0toT1);

        } else {

            *intersectionPoint = intersectionPoint2;
            result->setDistanceFromP0 (distanceP0toT2);

        }

        result->setIntersectionPoint (intersectionPoint);

    } else {
        result->setHasIntersection (false);
    }

    return result;

}

Color* Sphere::getColorToBePainted (
    IntersectionResult* intersectionResult,
    LightsArray lightsArray,
    ObjectsArray objectsArray,
    Ray* ray,
    Vector* environmentLight
) {
    
    Vector resultColorRate (0, 0, 0);

    Vector* intersectionPoint = intersectionResult->getIntersectionPoint();

    Vector n = (*intersectionPoint - *this->getCenter()) / this->getRadius();

    Vector v = ((*ray->dir) * (-1)) / ray->dir->getMagnitude();

    for (auto i = lightsArray.begin(); i != lightsArray.end(); i++) {

        Vector sourceMinusIntersection = *((*i)->getPosition()) - *intersectionPoint;

        double sourceMinusIntersectionMagnitude = sourceMinusIntersection.getMagnitude();

        Vector l = sourceMinusIntersection / sourceMinusIntersectionMagnitude;
        
        // verificando se o Ray acverta outros objetos
        Sp<Ray> verifyShadowRay = new Ray (
            new Vector (
                (*intersectionPoint)
            ),
            new Vector (l)
        );

        bool hasIntersectionWithOtherObjects = false;

        for (auto j = objectsArray.begin(); (j != objectsArray.end() && !hasIntersectionWithOtherObjects); j++) {
            
            if ((*j) != this) {
                Sp<IntersectionResult> intersectionShadow = (*j)->getIntersectionResult (verifyShadowRay.pointer);

                hasIntersectionWithOtherObjects =
                    intersectionShadow->getHasIntersection() &&
                    (intersectionShadow->getDistanceFromP0() < intersectionResult->getDistanceFromP0());
            }

        }

        // calculando cor a ser pintada
        if (!hasIntersectionWithOtherObjects) {
            double lScalarProductN = scalarProduct (l, n);

            Vector r = n * (2 * lScalarProductN) - l;

            double fDifusa = max (
                0.0,
                scalarProduct (l, n)
            );

            double fEspeculada = pow (
                max (
                    0.0,
                    scalarProduct (r, v)
                ),
                this->getShininess()
            );

            Vector iDifusa = (*(*i)->getIntensity()) * (*this->getReflectivity()) * fDifusa;

            Vector iEspeculada = (*(*i)->getIntensity()) * (*this->getReflectivity()) * fEspeculada;

            resultColorRate = resultColorRate + iDifusa + iEspeculada;
        }
    }

    if (environmentLight != nullptr) {
        resultColorRate = resultColorRate + ((*environmentLight) * (*this->getReflectivity()));
    }

    return new Color (
        resultColorRate[0] * 255,
        resultColorRate[1] * 255,
        resultColorRate[2] * 255,
        255
    );


}

ObjectType Plan::getObjectType () {
    return this->type;
}

void Plan::setInitialPoint (Vector* initialPoint) {
    this->initialPoint = initialPoint;
}

Vector* Plan::getInitialPoint () {
    return this->initialPoint;
} 

void Plan::setNormal (Vector* normal) {
    this->normal = normal;
}

Vector* Plan::getNormal () {
    return this->normal;
}

void Plan::setReflectivity (Vector* reflectivity) {
    this->reflectivity = reflectivity;
}

Vector* Plan::getReflectivity () {
    return this->reflectivity;
}

void Plan::setShininess (double shininess) {
    this->shininess = shininess;
}

double Plan::getShininess () {
    return this->shininess;
}

Plan::Plan () {}

Plan::Plan (Vector* initialPoint, Vector* normal, Vector* reflectivity, double shininess) {
    this->setInitialPoint (initialPoint);
    this->setNormal (normal);
    this->setReflectivity (reflectivity);
    this->setShininess (shininess);
}

Plan::~Plan () {
    delete this->getInitialPoint();
    delete this->getNormal();
    delete this->getReflectivity();
}

IntersectionResult* Plan::getIntersectionResult (Ray* ray) {

    IntersectionResult* result = new IntersectionResult ();
    result->setObjectRegion (ObjectRegion::PLAN);
    result->setHasIntersection (true);

    Vector w = *(ray->P0) - *this->getInitialPoint();

    double dirScalarN = scalarProduct (ray->dir, this->getNormal());

    if (dirScalarN == 0) {
        result->setHasIntersection (false);
        return result;
    }

    double t = (-scalarProduct (w, *this->normal)) / dirScalarN;

    if (t < 0) {
        result->setHasIntersection (false);
        return result;
    }

    Vector* intersectionPoint = new Vector ();
    *intersectionPoint = *ray->P0 + *ray->dir * t;

    Vector distanceVector = (*intersectionPoint) - (*ray->P0);
    double distanceFromP0 = distanceVector.getMagnitude ();

    result->setIntersectionPoint (intersectionPoint);
    result->setDistanceFromP0 (distanceFromP0);

    return result;

}

Color* Plan::getColorToBePainted (
    IntersectionResult* intersectionResult,
    LightsArray lightsArray,
    ObjectsArray objectsArray,
    Ray* ray,
    Vector* environmentLight
) {

    Vector resultColorRate (0, 0, 0);

    Vector v = ((*ray->dir) * -1) / ray->dir->getMagnitude();

    for (auto i = lightsArray.begin(); i != lightsArray.end(); i++) {

        Vector* intersectionPoint = intersectionResult->getIntersectionPoint();
        
        Vector l = (*((*i)->getPosition()) - *intersectionPoint) / (*((*i)->getPosition()) - *intersectionResult->getIntersectionPoint()).getMagnitude();
        // verifica se a luz intercepta outros objetos
        Sp<Ray> verifyShadowRay = new Ray (
            new Vector (
                (*intersectionPoint)
            ),
            new Vector (l)
        );

        bool hasIntersectionWithOtherObjects = false;

        for (auto j = objectsArray.begin(); (j != objectsArray.end() && !hasIntersectionWithOtherObjects); j++) {
            
            if ((*j) != this) {
                Sp<IntersectionResult> intersectionShadow = (*j)->getIntersectionResult (verifyShadowRay.pointer);

                hasIntersectionWithOtherObjects =
                    intersectionShadow->getHasIntersection() &&
                    (intersectionShadow->getDistanceFromP0() < intersectionResult->getDistanceFromP0());
            }

        }

        // calculando cor a ser pintada
        if (!hasIntersectionWithOtherObjects) {    
            Vector r = (*this->getNormal()) * (2 * scalarProduct (l, *this->getNormal())) -  l;

            double fDifusa = max (
                scalarProduct (l, *this->getNormal()),
                0.0
            );

            double fEspeculada = pow (
                max (
                    scalarProduct (r, v),
                    0.0
                ),
                this->getShininess()
            );

            Vector iDifusa = (*(*i)->getIntensity()) * (*this->getReflectivity()) * fDifusa;

            Vector iEspeculada = (*(*i)->getIntensity()) * (*this->getReflectivity()) * fEspeculada;

            resultColorRate = resultColorRate + iDifusa + iEspeculada;
        }


    }

    if (environmentLight != nullptr) {
        resultColorRate = resultColorRate + ((*environmentLight) * (*this->getReflectivity()));
    }

    return new Color (
        resultColorRate[0] * 255,
        resultColorRate[1] * 255,
        resultColorRate[2] * 255,
        255
    );
}


ObjectType Cylinder::getObjectType () {
    return this->type;
}
void Cylinder::setBaseCenter (Vector* baseCenter) { 
    this->baseCenter = baseCenter; 
}

Vector* Cylinder::getBaseCenter () { 
    return this->baseCenter; 
}

void Cylinder::setTopCenter (Vector* topCenter) { 
    this->topCenter = topCenter; 
}

Vector* Cylinder::getTopCenter () { 
    return this->topCenter; 
}

void Cylinder::setRadius (double radius) { 
    this->radius = radius; 
}

double Cylinder::getRadius () { 
    return this->radius; 
}
void Cylinder::setReflectivity (Vector* reflectivity) { 
    this->reflectivity = reflectivity; 
}
Vector* Cylinder::getReflectivity () { 
    return this->reflectivity; 
}
void Cylinder::setShininess (double shininess) { 
    this->shininess = shininess; 
}
double Cylinder::getShininess () { 
    return this->shininess; 
}
void Cylinder::setHeight (double height) { 
    this->height = height; 
}
double Cylinder::getHeight () { 
    return this->height; 
}
void Cylinder::setDirection (Vector* direction) { 
    this->direction = direction; 
}
Vector* Cylinder::getDirection () { 
    return this->direction; 
}

IntersectionResult* Cylinder::getIntersectionResult (Ray* ray) {
    IntersectionResult* intersectionResult = new IntersectionResult ();
    intersectionResult->setHasIntersection (false);

    Vector cylinderDirection = *this->getDirection();
    Vector w = *ray->P0 - *this->getBaseCenter();

    double a = scalarProduct(*ray->dir, *ray->dir) - pow(scalarProduct(*ray->dir, cylinderDirection), 2.0);
    double b = 2 * (scalarProduct(*ray->dir, w) - scalarProduct(*ray->dir, cylinderDirection) * scalarProduct(w, cylinderDirection));
    double c = scalarProduct(w, w) - pow(scalarProduct(w, cylinderDirection), 2.0) - pow(this->getRadius(), 2.0);

    double discriminant = pow(b, 2.0) - 4 * a * c;

    if (discriminant >= 0) {
        double t = (-b - sqrt(discriminant)) / (2.0 * a); 
        if (t > 0) {
            Vector intersectionPoint = *ray->P0 + (*ray->dir * t);
            
            
            double projection = scalarProduct(intersectionPoint - *this->getBaseCenter(), cylinderDirection);
            if (projection >= 0 && projection <= this->getHeight()) {
                intersectionResult->setHasIntersection(true);
                intersectionResult->setDistanceFromP0((intersectionPoint - *ray->P0).getMagnitude());
                intersectionResult->setIntersectionPoint(new Vector(intersectionPoint));
                intersectionResult->setObjectRegion(ObjectRegion::CYLINDER_SURFACE);
            }
        }
    }
    return intersectionResult;
}

Color* Cylinder::getColorToBePainted (
    IntersectionResult* intersectionResult,
    LightsArray lightsArray,
    ObjectsArray objectsArray,
    Ray* ray,
    Vector* environmentLight
) {
    Vector pIMinusCb = *intersectionResult->getIntersectionPoint() - *this->getBaseCenter();
    Vector cylinderDirection = *this->getDirection();
    double projectionSize = scalarProduct(pIMinusCb, cylinderDirection);
    Vector normalNotUnitary = pIMinusCb - (cylinderDirection * projectionSize);
    Vector n = normalNotUnitary / normalNotUnitary.getMagnitude();

    Vector resultColorRate (0, 0, 0);
    Vector* intersectionPoint = intersectionResult->getIntersectionPoint();
    Vector v = ((*ray->dir) * (-1)) / ray->dir->getMagnitude();

    for (auto i = lightsArray.begin(); i != lightsArray.end(); i++) {
        Vector sourceMinusIntersection = *((*i)->getPosition()) - *intersectionPoint;
        double sourceMinusIntersectionMagnitude = sourceMinusIntersection.getMagnitude();
        Vector l = sourceMinusIntersection / sourceMinusIntersectionMagnitude;

        Sp<Ray> verifyShadowRay = new Ray(new Vector(*intersectionPoint), new Vector(l));
        bool hasIntersectionWithOtherObjects = false;
        for (auto j = objectsArray.begin(); j != objectsArray.end() && !hasIntersectionWithOtherObjects; j++) {
            if ((*j) != this) {
                Sp<IntersectionResult> intersectionShadow = (*j)->getIntersectionResult(verifyShadowRay.pointer);
                if (intersectionShadow->getHasIntersection()) hasIntersectionWithOtherObjects = true;
            }
        }

        if (!hasIntersectionWithOtherObjects) {
            Vector r = n * (2 * scalarProduct(l, n)) - l;
            double fDifusa = max(0.0, scalarProduct(l, n));
            double fEspeculada = pow(max(0.0, scalarProduct(r, v)), this->getShininess());
            Vector iDifusa = (*(*i)->getIntensity()) * (*this->getReflectivity()) * fDifusa;
            Vector iEspeculada = (*(*i)->getIntensity()) * (*this->getReflectivity()) * fEspeculada;
            resultColorRate = resultColorRate + iDifusa + iEspeculada;
        }
    }
    if (environmentLight != nullptr) {
        resultColorRate = resultColorRate + ((*environmentLight) * (*this->getReflectivity()));
    }
    return new Color(resultColorRate[0] * 255, resultColorRate[1] * 255, resultColorRate[2] * 255, 255);
}

Cylinder::Cylinder () {}
Cylinder::Cylinder (Vector* baseCenter, Vector* topCenter, double radius, Vector* reflectivity, double shininess) {
    this->setBaseCenter (baseCenter);
    this->setTopCenter (topCenter);
    this->setRadius (radius);
    this->setReflectivity (reflectivity);
    this->setShininess (shininess);
    this->setHeight ((*this->getTopCenter() - *this->getBaseCenter()).getMagnitude());
    this->setDirection ( new Vector ((*this->getTopCenter() - *this->getBaseCenter()) / this->getHeight()) );
}
Cylinder::Cylinder (Vector* baseCenter, Vector* direction, double height, double radius, Vector* reflectivity, double shininess) {
    this->setBaseCenter (baseCenter);
    this->setRadius (radius);
    this->setReflectivity (reflectivity);
    this->setShininess (shininess);
    this->setHeight (height);
    *direction = *direction / (*direction).getMagnitude();
    this->setDirection (direction);
    this->setTopCenter ( new Vector (*this->getBaseCenter() + *this->getDirection () * this->getHeight ()) );
}
Cylinder::~Cylinder () {
    delete this->getBaseCenter ();
    delete this->getTopCenter ();
    delete this->getReflectivity ();
    delete this->getDirection();
}

ObjectType Cone::getObjectType () { 
    return this->type; 
}
void Cone::setBaseCenter (Vector* baseCenter) { 
    this->baseCenter = baseCenter; 
}
Vector* Cone::getBaseCenter () { 
    return this->baseCenter; 
}
void Cone::setTop (Vector* top) { 
    this->top = top; 
}
Vector* Cone::getTop () { 
    return this->top; 
}
void Cone::setRadius (double radius) { 
    this->radius = radius; 
}
double Cone::getRadius () { 
    return this->radius; 
}
void Cone::setReflectivity (Vector* reflectivity) { 
    this->reflectivity = reflectivity; 
}
Vector* Cone::getReflectivity () { 
    return this->reflectivity; 
}
void Cone::setShininess (double shininess) { 
    this->shininess = shininess; 
}
double Cone::getShininess () { 
    return this->shininess; 
}
void Cone::setHeight (double height) { 
    this->height = height; 
}
double Cone::getHeight () { 
    return this->height; 
}
void Cone::setDirection (Vector* direction) { 
    this->direction = direction; 
}
Vector* Cone::getDirection () { 
    return this->direction; 
}
void Cone::setCos2Angle(double cos2angle) { 
    this->cos2angle = cos2angle; 
}
double Cone::getCos2Angle() { 
    return this->cos2angle; 
}

IntersectionResult* Cone::getIntersectionResult (Ray* ray) {
    IntersectionResult* intersectionResult = new IntersectionResult();
    intersectionResult->setHasIntersection(false);

    Vector coneDirection = *this->getDirection();
    Vector w = *this->getTop() - *ray->P0;
    double cos2angle = this->getCos2Angle();

    double a = pow(scalarProduct(*ray->dir, coneDirection), 2.0) - scalarProduct(*ray->dir, *ray->dir) * cos2angle;
    double b = 2 * (scalarProduct(w, *ray->dir) * cos2angle - scalarProduct(w, coneDirection) * scalarProduct(*ray->dir, coneDirection));
    double c = pow(scalarProduct(w, coneDirection), 2.0) - scalarProduct(w, w) * cos2angle;

    double discriminant = pow(b, 2.0) - 4 * a * c;

    if (discriminant >= 0) {
        double t = (-b - sqrt(discriminant)) / (2.0 * a);
        if (t > 0) {
            Vector intersectionPoint = *ray->P0 + (*ray->dir * t);
            double projection = scalarProduct(intersectionPoint - *this->getBaseCenter(), coneDirection);
            if (projection >= 0 && projection <= this->getHeight()) {
                intersectionResult->setHasIntersection(true);
                intersectionResult->setDistanceFromP0((intersectionPoint - *ray->P0).getMagnitude());
                intersectionResult->setIntersectionPoint(new Vector(intersectionPoint));
                intersectionResult->setObjectRegion(ObjectRegion::CONE_SURFACE);
            }
        }
    }
    return intersectionResult;
}

Color* Cone::getColorToBePainted (
    IntersectionResult* intersectionResult,
    LightsArray lightsArray,
    ObjectsArray objectsArray,
    Ray* ray,
    Vector* environmentLight
) {
   
    Vector intersectionPoint = *intersectionResult->getIntersectionPoint();
    Vector vMinusPi = (*this->getTop() - intersectionPoint);
    Vector nBar = vectorProduct(vMinusPi, *this->getDirection());
    Vector normalNotUnitary = vectorProduct(nBar, vMinusPi);
    Vector n = normalNotUnitary / normalNotUnitary.getMagnitude();

    
    Vector resultColorRate (0, 0, 0);
    Vector v = ((*ray->dir) * (-1)) / ray->dir->getMagnitude();
    
    for (auto i = lightsArray.begin(); i != lightsArray.end(); i++) {
        Vector sourceMinusIntersection = *((*i)->getPosition()) - intersectionPoint;
        double sourceMinusIntersectionMagnitude = sourceMinusIntersection.getMagnitude();
        Vector l = sourceMinusIntersection / sourceMinusIntersectionMagnitude;

        Sp<Ray> verifyShadowRay = new Ray(new Vector(intersectionPoint), new Vector(l));
        bool hasIntersectionWithOtherObjects = false;
        for (auto j = objectsArray.begin(); j != objectsArray.end() && !hasIntersectionWithOtherObjects; j++) {
            if ((*j) != this) {
                Sp<IntersectionResult> intersectionShadow = (*j)->getIntersectionResult(verifyShadowRay.pointer);
                if (intersectionShadow->getHasIntersection()) hasIntersectionWithOtherObjects = true;
            }
        }

        if (!hasIntersectionWithOtherObjects) {
            Vector r = n * (2 * scalarProduct(l, n)) - l;
            double fDifusa = max(0.0, scalarProduct(l, n));
            double fEspeculada = pow(max(0.0, scalarProduct(r, v)), this->getShininess());
            Vector iDifusa = (*(*i)->getIntensity()) * (*this->getReflectivity()) * fDifusa;
            Vector iEspeculada = (*(*i)->getIntensity()) * (*this->getReflectivity()) * fEspeculada;
            resultColorRate = resultColorRate + iDifusa + iEspeculada;
        }
    }
    if (environmentLight != nullptr) {
        resultColorRate = resultColorRate + ((*environmentLight) * (*this->getReflectivity()));
    }
    return new Color(resultColorRate[0] * 255, resultColorRate[1] * 255, resultColorRate[2] * 255, 255);
}

Cone::Cone () {}
Cone::Cone (Vector* baseCenter, Vector* top, double radius, Vector* reflectivity, double shininess) {
    this->setBaseCenter (baseCenter);
    this->setTop (top);
    this->setRadius (radius);
    this->setReflectivity (reflectivity);
    this->setShininess (shininess);
    this->setHeight ( (*this->getTop() - *this->getBaseCenter()).getMagnitude() );
    this->setDirection ( new Vector( (*this->getTop() - *this->getBaseCenter()) / this->getHeight() ) );
    this->setCos2Angle(pow(this->getHeight(), 2.0) / (pow(this->getRadius(), 2.0) + pow(this->getHeight(), 2.0)));
}
Cone::Cone (Vector* baseCenter, Vector* direction, double height, double radius, Vector* reflectivity, double shininess) {
    this->setBaseCenter (baseCenter);
    this->setRadius (radius);
    this->setHeight (height);
    this->setReflectivity (reflectivity);
    this->setShininess (shininess);
    *direction = *direction / (*direction).getMagnitude();
    this->setDirection (direction);
    this->setTop ( new Vector(*this->getBaseCenter() + *this->getDirection() * this->getHeight()) );
    this->setCos2Angle(pow(this->getHeight(), 2.0) / (pow(this->getRadius(), 2.0) + pow(this->getHeight(), 2.0)));
}
Cone::~Cone () {
    delete this->getBaseCenter();
    delete this->getTop();
    delete this->getDirection();
    delete this->getReflectivity();
}