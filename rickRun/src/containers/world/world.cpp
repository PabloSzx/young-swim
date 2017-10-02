#include "world.hpp"

const char* World::getRandomProp() {
    int n = rand() % 4;
    switch (n) {
        case 0:
        return "mesh/arbol.obj";
        case 1:
        return "mesh/pasto.obj";
        case 2 : 
        return "mesh/planta1.obj";
        case 3:
        return "mesh/planta2.obj";
    }
    return "";
}

World::World(int nPlataformas, int nHouses, int nProps, double platformInitialVelocity) {
    this->nPlataformas = nPlataformas;
    this->nHouses = nHouses;
    this->nProps = nProps;
    this->platformVelocity = platformInitialVelocity;
}
void World::genRick() {
    this->rick = new Model(const_cast<char *>("mesh/rick.obj"));
    this->rick->scale(glm::vec3(0.6f));
    this->rick->setColor(1.0f, 0.894f, 0.882f);
    this->rick->model2shader(shader_programme);
    platformWorld->newFallBody(btVector3(rick->LX / 2, rick->LY / 4, rick->LZ / 2), btVector3(0.0, 10.0, 0.0), 1.0, btVector3(0, 0, 0), -1); //1
};
void World::genPlatforms(Parameters* rules) {
    this->plataformas = static_cast<Model **>(malloc(sizeof(Model *) * this->nPlataformas));
    this->plataformas[0] = new Model(const_cast<char *>("mesh/platform.obj"));
    this->plataformas[0]->setColor(0.753f, 0.753f, 0.753f);
    this->plataformas[0]->model2shader(shader_programme);
    this->platPos = btVector3(1.0, 0.0, 0.0);
    
    platformWorld->newFallBody(btVector3(this->plataformas[0]->LX / 2, this->plataformas[0]->LY * 3, this->plataformas[0]->LZ / 2), this->platPos, 10000, btVector3(0, 0, 0), PLATFORMS_START_INDEX);
    
    for (int i = 1; i < this->nPlataformas; i+=1) {
        this->platPos = rules->getNextPlatformPos(this->platPos.getZ(), this->platPos.getY(), i * this->plataformas[0]->LX);
        this->plataformas[i] = new Model(const_cast<char *>("mesh/platform.obj"));
        this->plataformas[i]->setColor(0.753f, 0.753f, 0.753f);
        this->plataformas[i]->model2shader(shader_programme);
        platformWorld->newFallBody(btVector3(this->plataformas[0]->LX / 2, this->plataformas[0]->LY * 3, this->plataformas[0]->LZ / 2), this->platPos, 10000, btVector3(0, 0, 0), i + PLATFORMS_START_INDEX);
    }
    
};
void World::genPhysics() {
    platformWorld = new Bullet(this->nPlataformas + 2, btVector3(0, 0, 0), PLATFORMS_START_INDEX);
    platformWorld->newPlane(btVector3(0, 1, 0), -3.7, 0); //0
    this->plano = new Model(const_cast<char *>("mesh/plano.obj"));
    this->plano->setColor(0.8f, 0.0f, 0.0f);
    this->plano->model2shader(shader_programme);
};
void World::genParallaxHouses(Parameters* rules) {
    parallaxHouses = new Bullet(nHouses, btVector3(0, 0, 0), 0);
    
    this->casas = static_cast<Model **>(malloc(sizeof(Model *) * nHouses));
    
    this->casas[0] = new Model(const_cast<char *>("mesh/casa.obj"));
    this->casas[0]->setColor(0.545f, 0.271f, 0.075f);
    this->casas[0]->scale(glm::vec3(5.0f,5.0f,1.0f));
    this->casas[0]->model2shader(shader_programme);
    
    this->casaPos = btVector3(0, 0, 12);
    parallaxHouses->newFallBody(btVector3(this->casas[0]->LX / 2, this->casas[0]->LY / 2 + 0.1, this->casas[0]->LZ / 2), this->casaPos, 1, btVector3(this->platformVelocity * 0.5, 0, 0), PARALLAX_START_INDEX);
    
    for (int i = 1; i < this->nHouses; i += 1)
    {
        this->casaPos = rules->getNextHousePos(this->casaPos.getX(), this->casaPos.getY(), this->casaPos.getZ());
        this->casas[i] = new Model(const_cast<char *>("mesh/casa.obj"), glm::vec3(this->casaPos.getX(), this->casaPos.getY(), this->casaPos.getZ()));
        this->casas[i]->setColor(0.545f, 0.271f, 0.075f);
        this->casas[i]->scale(glm::vec3(5.0f,5.0f,1.0f));
        this->casas[i]->model2shader(shader_programme);
        parallaxHouses->newFallBody(btVector3(this->casas[i]->LX / 2, this->casas[i]->LY / 2 + 0.1, this->casas[i]->LZ / 2), this->casaPos, 1, btVector3(this->platformVelocity * 0.5, 0, 0), i);
    }
    
};
void World::genParallaxProps(Parameters* rules) {
    parallaxProps = new Bullet(nProps, btVector3(0, 0, 0), 0);
    this->props = static_cast<Model **>(malloc(sizeof(Model *) * nProps));
    
    this->props[0] = new Model(const_cast<char *>(this->getRandomProp()));
    this->props[0]->setColor(0.196f, 0.804f, 0.196f);
    this->props[0]->model2shader(shader_programme);
    this->propPos = btVector3(0, 0, 7);
    parallaxProps->newFallBody(btVector3(this->props[0]->LX / 2, this->props[0]->LY / 2 + 0.1, this->props[0]->LZ / 2), propPos, 1, btVector3(this->platformVelocity, 0, 0), PARALLAX_START_INDEX);
    
    for (int i = 1; i < this->nProps; i += 1)
    {
        this->propPos = rules->getNextPropPos(this->propPos.getX(), this->propPos.getY(), this->propPos.getZ());
        this->props[i] = new Model(const_cast<char *>(this->getRandomProp()), glm::vec3(this->propPos.getX(), this->propPos.getY(), this->propPos.getZ()));
        this->props[i]->setColor(0.196f, 0.804f, 0.196f);
        this->props[i]->model2shader(shader_programme);
        parallaxProps->newFallBody(btVector3(this->props[i]->LX / 2 + 0.1, this->props[i]->LY / 2 + 0.1, this->props[i]->LZ / 2), this->propPos, 1, btVector3(this->platformVelocity, 0, 0), i + PARALLAX_START_INDEX);
    }
    
};
void World::startPlatformVelocity() {
    this->platformVelocity = -5.0;
    for (int i = PLATFORMS_START_INDEX; i < nPlataformas + PLATFORMS_START_INDEX; i += 1)
    {
        platformWorld->setVelocity(i, btVector3(this->platformVelocity, 0, 0));
    }
    this->startHousesVelocity();
    this->startPropsVelocity();
};
void World::startHousesVelocity() {
    for (int i = 0; i < this->nHouses; i += 1) {
        parallaxHouses->setVelocity(i, btVector3(this->platformVelocity * 0.5, 0, 0));
    }
};
void World::startPropsVelocity() {
    for (int i = 0; i < this->nProps; i += 1) {
        parallaxProps->setVelocity(i, btVector3(this->platformVelocity, 0, 0));
    }
};
void World::morePlatformVelocity() {
    platformVelocity -= 5.0;
    for (int i = 0; i < nPlataformas; i += 1) {
        platformWorld->setVelocity(i + PLATFORMS_START_INDEX, btVector3(platformVelocity, 0, 0));
    }
    
    this->moreHousesVelocity();
    this->morePropsVelocity();
};
void World::moreHousesVelocity() {
    for (int i = 0; i < nHouses; i += 1)
    {
        parallaxHouses->setVelocity(i, btVector3(platformVelocity * 0.5, 0, 0));
    }
    
};
void World::morePropsVelocity() {
    for (int i = 0; i < nProps; i += 1)
    {
        parallaxProps->setVelocity(i, btVector3(platformVelocity, 0, 0));
    }
    
};
void World::gravityRick() {
    platformWorld->applyForce(1, btVector3(0, -9.8, 0));
};
void World::dynamicPlatforms(Parameters* rules) {
    int previousPlatform;
    
    if (abs(platformWorld->getTransformOrigin(platformWorld->getLastPlatform()).getX() - platformWorld->getTransformOrigin(1).getX()) > (20 * this->plataformas[0]->LX))
    {
        // cout << "last platform modified" << platformWorld->getLastPlatform() << endl;
        if (platformWorld->getLastPlatform() == PLATFORMS_START_INDEX) {
            previousPlatform = platformWorld->getNMax() - 1;
        } else {
            previousPlatform = platformWorld->getLastPlatform() - 1;
        }
        this->platPos = rules->getNextPlatformPos(this->platPos.getZ(), this->platPos.getY(), platformWorld->getTransformOrigin(previousPlatform).getX() + this->plataformas[0]->LX);
        platformWorld->editLastPlatform(this->platPos, 10000, btVector3(this->platformVelocity, 0, 0), platformWorld->getLastPlatform());
    }
    
};
void World::dynamicHouses(Parameters* rules) {
    int previousParallaxObj;
    btVector3 previousObj;
    
    if (abs(parallaxHouses->getTransformOrigin(parallaxHouses->getLastPlatform()).getX() - platformWorld->getTransformOrigin(1).getX()) > (10 * this->casas[0]->LX))
    {
        // cout << "last house modified " << parallaxHouses->getLastPlatform() << endl;
        if (parallaxHouses->getLastPlatform() == PARALLAX_START_INDEX) {
            previousParallaxObj = parallaxHouses->getNMax() - 1;
        } else {
            previousParallaxObj = parallaxHouses->getLastPlatform() - 1;
        }
        previousObj = parallaxHouses->getTransformOrigin(previousParallaxObj);
        
        this->casaPos = rules->getNextHousePos(previousObj.getX(), previousObj.getY(), previousObj.getZ());
        parallaxHouses->editLastPlatform(this->casaPos, 1, btVector3(this->platformVelocity * 0.5, 0, 0), parallaxHouses->getUserIndex(parallaxHouses->getLastPlatform()));
    }
    
};
void World::dynamicProps(Parameters *rules) {
    int previousParallaxObj;
    btVector3 previousObj;
    
    if (abs(parallaxProps->getTransformOrigin(parallaxProps->getLastPlatform()).getX() - platformWorld->getTransformOrigin(1).getX()) > (10 * this->props[0]->LX))
    {
        // cout << "last props modified " << parallaxProps->getLastPlatform() << endl;
        
        if (parallaxProps->getLastPlatform() == PARALLAX_START_INDEX)
        {
            previousParallaxObj = parallaxProps->getNMax() - 1;
        }
        else
        {
            previousParallaxObj = parallaxProps->getLastPlatform() - 1;
        }
        previousObj = parallaxProps->getTransformOrigin(previousParallaxObj);
        
        this->propPos = rules->getNextHousePos(previousObj.getX(), previousObj.getY(), previousObj.getZ());
        parallaxProps->editLastPlatform(this->propPos, 1, btVector3(this->platformVelocity, 0, 0), parallaxProps->getUserIndex(parallaxProps->getLastPlatform()));
    }
    
};
void World::getPhysicsPos() {
    this->rickPos = platformWorld->getTransformOrigin(1);
    
}
void World::drawRick() {
    this->rick->setpos(glm::vec3(this->rickPos.getX(), this->rickPos.getY(), this->rickPos.getZ()));
    this->rick->draw();
}
void World::drawPlatforms() {
    btVector3 plataformaPos;
    
    for (int i = 0; i < nPlataformas; i += 1)
    {
        plataformaPos = platformWorld->getTransformOrigin(i + PLATFORMS_START_INDEX);
        this->plataformas[i]->setpos(glm::vec3(plataformaPos.getX(), plataformaPos.getY(), plataformaPos.getZ()));
        this->plataformas[i]->draw();
    }
    
};
void World::drawPlane() {
    for (float i = -20; i <= 20; i += 1.1) {
        for (float j = -20; j <= 20; j += 1.1)
        {
            this->plano->setpos(glm::vec3(this->rickPos.getX() + i, -5, this->rickPos.getZ() + j));
            this->plano->draw();
        }
    }
    
};
void World::drawHouses(Parameters* rules) {
    for (int i = 0; i < this->nHouses; i += 1)
    {
        this->casaPos = parallaxHouses->getTransformOrigin(i);
        this->casas[i]->setpos(glm::vec3(this->casaPos.getX(), this->casaPos.getY(), 8 + rules->getDistanciaEntreCapas()));
        this->casas[i]->draw();
    }
    
    for (int i = 0; i < this->nHouses; i += 1)
    {
        this->casaPos = parallaxHouses->getTransformOrigin(i);
        this->casas[i]->setpos(glm::vec3(this->casaPos.getX(), this->casaPos.getY(), -4 - rules->getDistanciaEntreCapas()));
        this->casas[i]->draw();
    }
    
    
};
void World::drawProps() {
    for (int i = 0; i < this->nProps; i += 1)
    {
        this->propPos = parallaxProps->getTransformOrigin(i);
        this->props[i]->setpos(glm::vec3(this->propPos.getX(), this->propPos.getY(), 8));
        this->props[i]->draw();
    }
    
    for (int i = 0; i < this->nProps; i += 1)
    {
        this->propPos = parallaxProps->getTransformOrigin(i);
        this->props[i]->setpos(glm::vec3(this->propPos.getX(), this->propPos.getY(), -4));
        this->props[i]->draw();
    }
    
};

btVector3 World::getRickPos() {
    return this->rickPos;
}
