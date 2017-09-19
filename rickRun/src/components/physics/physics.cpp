#include "physics.h"

using namespace std;
Bullet::Bullet(int nmax) {
    this->broadphase = new btDbvtBroadphase();
    
    this->collisionConfiguration = new btDefaultCollisionConfiguration();
    this->dispatcher = new btCollisionDispatcher(this->collisionConfiguration);
    
    this->solver = new btSequentialImpulseConstraintSolver;
    
    this->dynamicsWorld = new btDiscreteDynamicsWorld(this->dispatcher, this->broadphase, this->solver, this->collisionConfiguration);
    
    this->dynamicsWorld->setGravity(btVector3(0, -9.8, 0));
    
    this->n = 0;
    this->nmax = nmax;
    
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

void Bullet::newPlane(btVector3 plane, float constant)
{
    this->shapes[this->n] = new btStaticPlaneShape(plane, constant);
    
    this->motionStates[this->n] = new btDefaultMotionState(btTransform(btQuaternion(0, 0, 0, 1), btVector3(0, -1, 0)));
    btRigidBody::btRigidBodyConstructionInfo
    groundRigidBodyCI(0, this->motionStates[this->n], this->shapes[this->n], btVector3(0, 0, 0));
    
    this->rigidBodys[this->n] = new btRigidBody(groundRigidBodyCI);
    
    this->rigidBodys[this->n]->setCcdMotionThreshold(1e-7);
    this->rigidBodys[this->n]->setCcdSweptSphereRadius(0.50);
    this->rigidBodys[this->n]->setFriction(0.1);

    this->dynamicsWorld->addRigidBody(this->rigidBodys[this->n]);
    this->n += 1;
}

void Bullet::newFallBody(btVector3 extents, btVector3 pos, btScalar mass) {
    // this->shapes[this->n] = new btSphereShape(radius);
    this->shapes[this->n] = new btBoxShape(extents);
    
    this->motionStates[this->n] = new btDefaultMotionState(btTransform(btQuaternion(0, 0, 0, 1), pos));
    
    // btScalar mass = m;
    
    btVector3 fallInertia(0, 10, 0);
    
    this->shapes[this->n]->calculateLocalInertia(mass, fallInertia);
    
    btRigidBody::btRigidBodyConstructionInfo fallRigidBodyCI(mass, this->motionStates[this->n], this->shapes[this->n], fallInertia);
    
    this->rigidBodys[this->n] = new btRigidBody(fallRigidBodyCI);
    // this->rigidBodys[this->n]->setRestitution(-100.0);
    
    this->rigidBodys[this->n]->setCcdMotionThreshold(1e-7);
    this->rigidBodys[this->n]->setCcdSweptSphereRadius(0.0);

    // cout << "friccion de " << this->n << ": " << this->rigidBodys[this->n]->getFriction() << endl;
    this->rigidBodys[this->n]->setFriction(0.1);
    
    this->dynamicsWorld->addRigidBody(this->rigidBodys[this->n]);

    this->n += 1;
    if (this->n == this->nmax) {
        cout << "nmax logrado " << this->n << " vs " << this->nmax << endl; 
        this->n = 2;
    }
}

void Bullet::setVelocity(int i, btVector3 vel) {
    this->rigidBodys[i]->setLinearVelocity(vel);
}

void Bullet::applyTranslate(int i, btVector3 vect) {
    this->rigidBodys[i]->translate(vect);
}

void Bullet::applyImpulse(int i, btVector3 impulse) {
    this->wakeUp(i);
    this->rigidBodys[i]->applyCentralImpulse(impulse);
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

void Bullet::translate(int i, btVector3 pos) {
    this->rigidBodys[i]->clearForces();
}

btVector3 Bullet::getTransformOrigin(int i) {
    btTransform trans;
    this->rigidBodys[i]->getMotionState()->getWorldTransform(trans);
    
    return trans.getOrigin();
}

