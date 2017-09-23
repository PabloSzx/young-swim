#include "gaming.h"
#include <iostream>
#include <time.h>

using namespace std;
Gaming::Gaming(
    int minXVelocity, int maxXVelocity, int maxYVelocity, int maxZVelocity,
    int minX, int maxX, int minZ, int maxZ,
    double forceHorizontalJump, double forceVerticalUpJump, double forceVerticalDownJump,
    double forceForwardJump, double forceBackwardJump
    )
{
    this->maxYVelocity = 8;
    this->maxXVelocity = 1;
    this->maxZVelocity = 8;
    this->minXVelocity = -1;
    this->minX = minX;
    this->maxX = maxX;
    this->minZ = minZ;
    this->maxZ = maxZ;

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
        world->applyImpulse(1, btVector3(0, 0, (this->minZ - rickPos.getZ()) * 4));
    }
    else if (rickPos.getZ() > this->maxZ)
    {
        world->applyImpulse(1, btVector3(0, 0, (this->maxZ - rickPos.getZ()) * 4));
    }
    if (rickPos.getX() > this->maxX)
    {
        world->applyImpulse(1, btVector3(-1, 0, 0));
    }
    else if (rickPos.getX() < this->minX)
    {
        world->applyImpulse(1, btVector3(1, 0, 0));
    }
}

void Gaming::checkRickVel(Bullet *world)
{
    btVector3 rickVelocity = world->getVelocity(1);
    if (rickVelocity.getY() > this->maxYVelocity)
    {
        world->setVelocity(1, btVector3(rickVelocity.getX(), rickVelocity.getY() - 1, rickVelocity.getZ()));
    }
    else if (rickVelocity.getZ() > this->maxZVelocity)
    {
        world->setVelocity(1, btVector3(rickVelocity.getX(), rickVelocity.getY(), rickVelocity.getZ() - 1));
    }
    else if (rickVelocity.getZ() < - this->maxZVelocity) 
    {
        world->setVelocity(1, btVector3(rickVelocity.getX(), rickVelocity.getY(), rickVelocity.getZ() + 1));
    }
    else if (rickVelocity.getX() > this->maxXVelocity)
    {
        world->setVelocity(1, btVector3(rickVelocity.getX() - 1, rickVelocity.getY(), rickVelocity.getZ()));
    }
    else if (rickVelocity.getX() < this->minXVelocity)
    {
        world->setVelocity(1, btVector3(this->minXVelocity, rickVelocity.getY(), rickVelocity.getZ()));
    }
}

int Gaming::getN(int lastPos) {
    int second;
    int third;
    if (lastPos == 0) {
        second = 3;
        third = 6;
    } else if (lastPos == 3) {
        second = 6;
        third = 0;
    } else {
        second = 0;
        third = 3;
    }
    int nrandom = rand() % 10;
    if (nrandom < 2)
    {
        return lastPos;
    } else if (nrandom < 6) {
        return second;
    } 
    return third;
}

btVector3 Gaming::getPlatformPos(int lastZPlatform, int lastYPlatform, int x) {
    btVector3 ret = btVector3(x, getN(lastYPlatform), getN(lastZPlatform));
    return ret;
}

