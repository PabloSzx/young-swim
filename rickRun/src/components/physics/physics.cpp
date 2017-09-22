#include "physics.h"

using namespace std;
Bullet::Bullet(int nmax) {
    this->broadphase = new btDbvtBroadphase();
    
    this->collisionConfiguration = new btDefaultCollisionConfiguration();
    this->dispatcher = new btCollisionDispatcher(this->collisionConfiguration);
    
    this->solver = new btSequentialImpulseConstraintSolver;
    
    this->dynamicsWorld = new btDiscreteDynamicsWorld(this->dispatcher, this->broadphase, this->solver, this->collisionConfiguration);
    
    this->dynamicsWorld->setGravity(btVector3(0, 0, 0));
    
    this->n = 0;
    this->nmax = nmax;
    this->lastPlatform = 4;
    
    this->shapes = static_cast<btCollisionShape **>(malloc(sizeof(btCollisionShape *) * nmax));
    this->motionStates = static_cast<btDefaultMotionState **>(malloc(sizeof(btDefaultMotionState *) * nmax));
    this->rigidBodys = static_cast<btRigidBody **>(malloc(sizeof(btRigidBody *) * nmax));
}

void Bullet::setGravity(int i, btVector3 vect) {
    this->wakeUp(i);
    this->dynamicsWorld->setGravity(vect);
}

void Bullet::applyGravity(int i)
{
    this->wakeUp(i);
    this->rigidBodys[i]->applyGravity();
}

void Bullet::getGravity(int i) {
    btVector3 vect = this->rigidBodys[i]->getGravity();
    cout << vect.getX() << "-" << vect.getY() << "-" << vect.getZ() << endl;
}

void Bullet::newPlane(btVector3 plane, float constant, int index)
{
    this->shapes[this->n] = new btStaticPlaneShape(plane, constant);
    
    this->motionStates[this->n] = new btDefaultMotionState(btTransform(btQuaternion(0, 0, 0, 1), btVector3(0, -1, 0)));
    btRigidBody::btRigidBodyConstructionInfo
    groundRigidBodyCI(0, this->motionStates[this->n], this->shapes[this->n], btVector3(0, 0, 0));
    
    this->rigidBodys[this->n] = new btRigidBody(groundRigidBodyCI);
    
    this->rigidBodys[this->n]->setCcdMotionThreshold(1e-7);
    this->rigidBodys[this->n]->setCcdSweptSphereRadius(0.50);
    this->rigidBodys[this->n]->setFriction(0.1);
    this->rigidBodys[this->n]->setUserIndex(index);
    this->dynamicsWorld->addRigidBody(this->rigidBodys[this->n]);
    this->n += 1;
}

void Bullet::newFallBody(btVector3 extents, btVector3 pos, btScalar mass, btVector3 velocity, int index) {
    // this->shapes[this->n] = new btSphereShape(radius);
    this->shapes[this->n] = new btBoxShape(extents);
    
    this->motionStates[this->n] = new btDefaultMotionState(btTransform(btQuaternion(0, 0, 0, 1), pos));
    
    btVector3 fallInertia(0, 0, 0);
    
    this->shapes[this->n]->calculateLocalInertia(mass, fallInertia);
    
    btRigidBody::btRigidBodyConstructionInfo fallRigidBodyCI(mass, this->motionStates[this->n], this->shapes[this->n], fallInertia);
    
    this->rigidBodys[this->n] = new btRigidBody(fallRigidBodyCI);
    // this->rigidBodys[this->n]->setRestitution(-100.0);
    
    this->rigidBodys[this->n]->setCcdMotionThreshold(1e-7);
    this->rigidBodys[this->n]->setCcdSweptSphereRadius(0.0);
    
    this->rigidBodys[this->n]->setFriction(0.5);
    this->rigidBodys[this->n]->setLinearVelocity(velocity);

    this->rigidBodys[this->n]->setUserIndex(index);

    this->dynamicsWorld->addRigidBody(this->rigidBodys[this->n]);
    
    if ((this->n + 1) == this->nmax)
    {
        cout << "No more bodys slots available" << endl;
    } else {
        this->n += 1;
    }
}

void Bullet::editLastPlatform(btVector3 pos, btScalar mass, btVector3 velocity, int index) {
    this->dynamicsWorld->removeRigidBody(this->rigidBodys[this->lastPlatform]);

    this->motionStates[this->lastPlatform] = new btDefaultMotionState(btTransform(btQuaternion(0, 0, 0, 1), pos));

    btVector3 fallInertia(0, 0, 0);

    this->shapes[this->lastPlatform]->calculateLocalInertia(mass, fallInertia);

    btRigidBody::btRigidBodyConstructionInfo fallRigidBodyCI(mass, this->motionStates[this->lastPlatform], this->shapes[this->lastPlatform], fallInertia);

    this->rigidBodys[this->lastPlatform] = new btRigidBody(fallRigidBodyCI);
    // this->rigidBodys[this->n]->setRestitution(-100.0);

    this->rigidBodys[this->lastPlatform]->setCcdMotionThreshold(1e-7);
    this->rigidBodys[this->lastPlatform]->setCcdSweptSphereRadius(0.0);

    this->rigidBodys[this->lastPlatform]->setFriction(0.5);
    this->rigidBodys[this->lastPlatform]->setLinearVelocity(velocity);
    this->rigidBodys[this->lastPlatform]->setUserIndex(index);

    this->dynamicsWorld->addRigidBody(this->rigidBodys[this->lastPlatform]);

    this->lastPlatform += 1;

    if (this->lastPlatform == this->nmax) {
        this->lastPlatform = 4;
    }
}

void Bullet::editBody(int i, btVector3 extents, btVector3 pos, btScalar mass, btVector3 velocity, int index) {
    this->dynamicsWorld->removeRigidBody(this->rigidBodys[i]);
    
    this->shapes[i] = new btBoxShape(extents);
    
    this->motionStates[i] = new btDefaultMotionState(btTransform(btQuaternion(0, 0, 0, 1), pos));
    
    btVector3 fallInertia(0, 0, 0);
    
    this->shapes[i]->calculateLocalInertia(mass, fallInertia);
    
    btRigidBody::btRigidBodyConstructionInfo fallRigidBodyCI(mass, this->motionStates[i], this->shapes[i], fallInertia);
    
    this->rigidBodys[i] = new btRigidBody(fallRigidBodyCI);
    // this->rigidBodys[this->n]->setRestitution(-100.0);
    
    this->rigidBodys[i]->setCcdMotionThreshold(1e-7);
    this->rigidBodys[i]->setCcdSweptSphereRadius(0.0);
    
    this->rigidBodys[i]->setFriction(0.5);
    this->rigidBodys[i]->setLinearVelocity(velocity);
    this->rigidBodys[i]->setUserIndex(index);

    this->dynamicsWorld->addRigidBody(this->rigidBodys[i]);
}

void Bullet::setVelocity(int i, btVector3 vel) {
    this->wakeUp(i);
    this->rigidBodys[i]->setLinearVelocity(vel);
}

btVector3 Bullet::getVelocity(int i) {
    return this->rigidBodys[i]->getLinearVelocity();
}

void Bullet::applyTranslate(int i, btVector3 vect) {
    this->rigidBodys[i]->translate(vect);
}

void Bullet::applyImpulse(int i, btVector3 impulse) {
    this->wakeUp(i);
    this->rigidBodys[i]->applyCentralImpulse(impulse);
    // this->applyGravity(i);
}

void Bullet::applyForce(int i, btVector3 force)
{
    this->wakeUp(i);
    this->rigidBodys[i]->applyCentralForce(force);
    // this->applyGravity(i);
}

void Bullet::wakeUp(int i) {
    if (!this->rigidBodys[i]->isActive())
    {
        this->rigidBodys[i]->activate();
    }
}

void Bullet::stepSimulation() {
    this->dynamicsWorld->stepSimulation(1 / 60.f, 10);
}

void Bullet::checkCollision(bool* allowJump) {
    int numManifolds = this->dynamicsWorld->getDispatcher()->getNumManifolds();
    // cout << "numManifolds: " << numManifolds << endl;
    for (int i = 0; i < numManifolds; i++)
    {
        btPersistentManifold *contactManifold = this->dynamicsWorld->getDispatcher()->getManifoldByIndexInternal(i);
        const btCollisionObject *obA = contactManifold->getBody0();
        const btCollisionObject *obB = contactManifold->getBody1();
        
        int a = obA->getUserIndex();
        int b = obB->getUserIndex();
        
        if (a == -1) {
            int numContacts = contactManifold->getNumContacts();
            for (int j = 0; j < numContacts; j++)
            {
                btManifoldPoint &pt = contactManifold->getContactPoint(j);
                if (pt.getDistance() < 0.f)
                {
                    // cout << "choque!     " << a << " / " << b << endl;
                    if (b >= 0 && *allowJump == false) {
                        *allowJump = true;
                    }
                    // const btVector3 &ptA = pt.getPositionWorldOnA();
                    // const btVector3 &ptB = pt.getPositionWorldOnB();
                    // const btVector3 &normalOnB = pt.m_normalWorldOnB;
                }
            }
        }
    }
}

void Bullet::translate(int i, btVector3 pos) {
    this->rigidBodys[i]->clearForces();
}

btVector3 Bullet::getTransformOrigin(int i) {
    btTransform trans;
    this->rigidBodys[i]->getMotionState()->getWorldTransform(trans);
    
    return trans.getOrigin();
}

int Bullet::getLastPlatform() {
    return this->lastPlatform;
}

int Bullet::getNMax() {
    return this->nmax;
}

