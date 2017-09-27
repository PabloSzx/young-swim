#ifndef WORLD_H
#define WORLD_H

#include "../../components/model/model.hpp"
#include "../../components/physics/physics.hpp"
#include "../../components/gaming/gaming.hpp"

#include "../../data/constants.hpp"

extern Bullet *world;
extern Bullet *parallaxHouses;
extern Bullet *parallaxProps;

class World {
private:
    int nPlataformas;
    int nHouses;
    int nProps;
    double platformVelocity;
    Model* rick;
    Model **plataformas;
    Model *plano;
    Model **casas;
    Model **props;
    
    btVector3 rickPos;
    btVector3 platPos;
    btVector3 casaPos;
    btVector3 propPos;
    
    const char* getRandomProp();
public:
    World(int nPlataformas, int nHouses, int nProps, double platformInitialVelocity);//listo
    void genRick();//listo
    void genPlatforms();//listo
    void genPhysics();//listo
    void genParallaxHouses(Gaming* rules); //listo
    void genParallaxProps(Gaming* rules);//listo
    void startPlatformVelocity();//listo
    void morePlatformVelocity();//listo
    void startHousesVelocity();//listo
    void moreHousesVelocity();//listo
    void startPropsVelocity();//listo
    void morePropsVelocity();//listo
    void getPhysicsPos();//listo
    void gravityRick();//listo
    void dynamicPlatforms();//listo
    void dynamicHouses(Gaming* rules);//listo
    void dynamicProps(Gaming* rules);//listo
    
    void drawRick();//listo
    void drawPlatforms();//listo
    void drawPlane();//listo
    void drawHouses(Gaming* rules);
    void drawProps();


    btVector3 getRickPos();
};

#endif
