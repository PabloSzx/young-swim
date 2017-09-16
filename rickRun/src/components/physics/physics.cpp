#include "physics.h"

Bullet::Bullet(int nmax) {
    this->broadphase = new btDbvtBroadphase();
    
    this->collisionConfiguration = new btDefaultCollisionConfiguration();
    this->dispatcher = new btCollisionDispatcher(this->collisionConfiguration);
    
    this->solver = new btSequentialImpulseConstraintSolver;
    
    this->dynamicsWorld = new btDiscreteDynamicsWorld(this->dispatcher, this->broadphase, this->solver, this->collisionConfiguration);
    
    this->setGravity(btVector3(0, -9.8, 0));
    
    this->n = 0;
    this->nmax = nmax;
    
    this->shapes = static_cast<btCollisionShape **>(malloc(sizeof(btCollisionShape *) * nmax));
    this->motionStates = static_cast<btDefaultMotionState **>(malloc(sizeof(btDefaultMotionState *) * nmax));
    this->rigidBodys = static_cast<btRigidBody **>(malloc(sizeof(btRigidBody *) * nmax));
}

void Bullet::setGravity(btVector3 vect) {
    this->dynamicsWorld->setGravity(vect);
}

void Bullet::newPlane(btVector3 plane, btScalar constant)
{
    this->shapes[this->n] = new btStaticPlaneShape(plane, constant);
    
    this->motionStates[this->n] = new btDefaultMotionState(btTransform(btQuaternion(0, 0, 0, 1), btVector3(0, -1, 0)));
    btRigidBody::btRigidBodyConstructionInfo
    groundRigidBodyCI(0, this->motionStates[this->n], this->shapes[this->n], btVector3(0, 0, 0));
    
    this->rigidBodys[this->n] = new btRigidBody(groundRigidBodyCI);
    
    
    this->dynamicsWorld->addRigidBody(this->rigidBodys[this->n]);
    
    this->n += 1;
}

void Bullet::newFallBody(btScalar radius, btVector3 pos) {
    this->shapes[this->n] = new btSphereShape(radius);
    
    this->motionStates[this->n] = new btDefaultMotionState(btTransform(btQuaternion(0, 0, 0, 1), pos));
    
    btScalar mass = 1;
    
    btVector3 fallInertia(0, 0, 0);
    
    this->shapes[this->n]->calculateLocalInertia(mass, fallInertia);
    
    btRigidBody::btRigidBodyConstructionInfo fallRigidBodyCI(mass, this->motionStates[this->n], this->shapes[this->n], fallInertia);
    
    this->rigidBodys[this->n] = new btRigidBody(fallRigidBodyCI);
    
    this->setVelocity(this->n, btVector3(0.0, 0.0, 0.0));
    
    this->dynamicsWorld->addRigidBody(this->rigidBodys[this->n]);
    
    this->n += 1;
}

void Bullet::setVelocity(int i, btVector3 vel) {
    this->rigidBodys[i]->setLinearVelocity(vel);
}

void Bullet::stepSimulation() {
    this->dynamicsWorld->stepSimulation(1 / 60.f, 10);
}

btVector3 Bullet::getTransformOrigin(int i) {
    btTransform trans;
    this->rigidBodys[i]->getMotionState()->getWorldTransform(trans);
    
    return trans.getOrigin();
}

