#include "parameters.hpp"
#include <iostream>
#include <time.h>

using namespace std;
Parameters::Parameters(
    int minXVelocity, int maxXVelocity, int maxYVelocity, int maxZVelocity,
    int minX, int maxX, int minZ, int maxZ,
    double forceHorizontalJump, double forceVerticalUpJump, double forceVerticalDownJump,
    double forceForwardJump, double forceBackwardJump,
    double distanciaEntreHouse, double distanciaEntreProp,
    double distanciaEntreCapas
    )
{
    this->maxYVelocity = maxYVelocity;
    this->maxXVelocity = maxXVelocity;
    this->maxZVelocity = maxZVelocity;
    this->minXVelocity = minXVelocity;
    this->minX = minX;
    this->maxX = maxX;
    this->minZ = minZ;
    this->maxZ = maxZ;

    this->distanciaEntreHouse = distanciaEntreHouse;
    this->distanciaEntreProp = distanciaEntreProp;
    this->distanciaEntreCapas = distanciaEntreCapas;

    this->setForceHorizontalJump(forceHorizontalJump);
    this->setForceVerticalUpJump(forceVerticalUpJump);
    this->setForceVerticalDownJump(forceVerticalDownJump);
    this->setForceForwardJump(forceForwardJump);
    this->setForceBackwardJump(forceBackwardJump);


    map<string, double> easy;
    map<string, double> normal;
    map<string, double> insane;

    easy.insert(make_pair("frequency", 10.0));
    easy.insert(make_pair("moreVelocity", 4.0));

    normal.insert(make_pair("frequency", 8.0));
    normal.insert(make_pair("moreVelocity", 5.0));

    insane.insert(make_pair("frequency", 6.0));
    insane.insert(make_pair("moreVelocity", 8.0));

    this->difficultyParameters.insert(make_pair("easy", easy));
    this->difficultyParameters.insert(make_pair("normal", normal));
    this->difficultyParameters.insert(make_pair("insane", insane));


}
void Parameters::setForceHorizontalJump(double force) {
    jumpHorizontalForce = force;
    this->forceHorizontalJump = force;
}

void Parameters::setForceVerticalUpJump(double force)
{
    jumpVerticalUpForce = force;
    this->forceVerticalUpJump = force;
}
void Parameters::setForceVerticalDownJump(double force)
{
    jumpVerticalDownForce = force;
    this->forceVerticalDownJump = force;
}
void Parameters::setForceForwardJump(double force)
{
    jumpForwardForce = force;
    this->forceForwardJump = force;
}
void Parameters::setForceBackwardJump(double force)
{
    jumpBackwardForce = force;
    this->forceBackwardJump = force;
}
void Parameters::checkRickPos(Bullet* world) {
    btVector3 rickPos = world->getTransformOrigin(1);
    if (rickPos.getZ() < this->minZ)
    {
        world->applyImpulse(1, btVector3(0, 0, deltaTime*100));
    }
    else if (rickPos.getZ() > this->maxZ)
    {
        world->applyImpulse(1, btVector3(0, 0, deltaTime*-10000));
    }
    if (rickPos.getX() > this->maxX)
    {
        world->applyImpulse(1, btVector3(deltaTime * -100, 0, 0));
    }
    else if (rickPos.getX() < this->minX)
    {
        world->applyImpulse(1, btVector3(deltaTime * 100, 0, 0));
    } 
    if (rickPos.getX() < -10.0) {
        background[8]->stop();
        background[8]->play();
        globalStatus = 1;
    }
}

void Parameters::checkRickVel(Bullet *world)
{
    btVector3 rickVelocity = world->getVelocity(1);
    if (rickVelocity.getY() > this->maxYVelocity)
    {
        world->setVelocity(1, btVector3(rickVelocity.getX(), this->maxYVelocity, rickVelocity.getZ()));
    }    
    if (rickVelocity.getZ() > this->maxZVelocity)
    {
        world->setVelocity(1, btVector3(rickVelocity.getX(), rickVelocity.getY(), this->maxZVelocity));
    }
    else if (rickVelocity.getZ() < - this->maxZVelocity) 
    {
        world->setVelocity(1, btVector3(rickVelocity.getX(), rickVelocity.getY(), - this->maxZVelocity));
    }    
    if (rickVelocity.getX() > this->maxXVelocity)
    {
        world->setVelocity(1, btVector3(this->maxXVelocity, rickVelocity.getY(), rickVelocity.getZ()));
    }
    else if (rickVelocity.getX() < this->minXVelocity)
    {
        world->setVelocity(1, btVector3(this->minXVelocity, rickVelocity.getY(), rickVelocity.getZ()));
    }
}

int Parameters::getN(int previousPos) {
    int second;
    int third;
    if (previousPos == 0) {
        second = 3;
        third = 6;
    } else if (previousPos == 3) {
        second = 6;
        third = 0;
    } else {
        second = 0;
        third = 3;
    }
    int nrandom = rand() % 20;
    if (nrandom < 2)
    {
        return previousPos;
    } else if (nrandom < 10) {
        return second;
    } 
    return third;
}

btVector3 Parameters::getNextPlatformPos(int previousZPlatform, int previousYPlatform, int x) {
    btVector3 ret = btVector3(x, getN(previousYPlatform), getN(previousZPlatform));
    return ret;
}

btVector3 Parameters::getNextHousePos(double previousXHouse, double y, double z)
{
    btVector3 ret = btVector3(previousXHouse + this->distanciaEntreHouse, y, z);
    return ret;
}

btVector3 Parameters::getNextPropPos(double previousXProp, double y, double z)
{
    btVector3 ret = btVector3(previousXProp + this->distanciaEntreProp, y, z);
    return ret;
}

double Parameters::getDistanciaEntreCapas() {
    return this->distanciaEntreCapas;
}

double Parameters::getDistance(Bullet* distanceScore) {
    return abs((int)distanceScore->getTransformOrigin(0).getX());
}

map< string, map<string, double> > Parameters::getDifficultyParameters() {
    return this->difficultyParameters;
}

