#include "gaming.h"
#include <iostream>
#include <time.h>

using namespace std;
Gaming::Gaming(
    int minXVelocity, int maxXVelocity, int maxYVelocity, int maxZVelocity,
    int minX, int maxX, int minZ, int maxZ,
    double forceHorizontalJump, double forceVerticalUpJump, double forceVerticalDownJump,
    double forceForwardJump, double forceBackwardJump,
    double distanciaEntreHouse, double distanciaEntreProp
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

    this->setForceHorizontalJump(forceHorizontalJump);
    this->setForceVerticalUpJump(forceVerticalUpJump);
    this->setForceVerticalDownJump(forceVerticalDownJump);
    this->setForceForwardJump(forceForwardJump);
    this->setForceBackwardJump(forceBackwardJump);
}
void Gaming::setForceHorizontalJump(double force) {
    jumpHorizontalForce = force;
    this->forceHorizontalJump = force;
}
void Gaming::setForceVerticalUpJump(double force)
{
    jumpVerticalUpForce = force;
    this->forceVerticalUpJump = force;
}
void Gaming::setForceVerticalDownJump(double force)
{
    jumpVerticalDownForce = force;
    this->forceVerticalDownJump = force;
}
void Gaming::setForceForwardJump(double force)
{
    jumpForwardForce = force;
    this->forceForwardJump = force;
}
void Gaming::setForceBackwardJump(double force)
{
    jumpBackwardForce = force;
    this->forceBackwardJump = force;
}
void Gaming::checkRickPos(Bullet* world) {
    btVector3 rickPos = world->getTransformOrigin(1);
    if (rickPos.getZ() < this->minZ)
    {
        world->applyImpulse(1, btVector3(0, 0, (this->minZ - rickPos.getZ())));
    }
    else if (rickPos.getZ() > this->maxZ)
    {
        world->applyImpulse(1, btVector3(0, 0, (this->maxZ - rickPos.getZ())));
    }
    if (rickPos.getX() > this->maxX)
    {
        world->applyImpulse(1, btVector3(-0.01, 0, 0));
    }
    else if (rickPos.getX() < this->minX)
    {
        world->applyImpulse(1, btVector3(0.01, 0, 0));
    }
}

void Gaming::checkRickVel(Bullet *world)
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

int Gaming::getN(int previousPos) {
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
    int nrandom = rand() % 10;
    if (nrandom < 2)
    {
        return previousPos;
    } else if (nrandom < 6) {
        return second;
    } 
    return third;
}

btVector3 Gaming::getPlatformPos(int previousZPlatform, int previousYPlatform, int x) {
    btVector3 ret = btVector3(x, getN(previousYPlatform), getN(previousZPlatform));
    return ret;
}

btVector3 Gaming::getHousePos(double previousXHouse, double y, double z)
{
    btVector3 ret = btVector3(previousXHouse + this->distanciaEntreHouse, y, z);
    return ret;
}

btVector3 Gaming::getPropPos(double previousXProp, double y, double z)
{
    btVector3 ret = btVector3(previousXProp + this->distanciaEntreProp, y, z);
    return ret;
}
