#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <assert.h>
#include <string>
#include <GL/glew.h> // include GLEW and new version of GL on Windows
#include "utils/maths_funcs.h"
#include "tools.h"
#include "model.h"

using namespace std;
model::model(char* filename){
	vida = 100;
	vec3 pos = vec3(0,0,0);
	this->filename = filename;
	assert(load_mesh(filename, &vao, &numvertices));
}

void model::atacar(){
}

void model::moverse(){
}

void model::morir(){
}



// gets
int model::getvida(){
    return this->vida;
}

GLuint model::getvao(){
    return this->vao;
}

int model::getnumvertices(){
    return this->numvertices;
}

vec3 model::getpos(){
    return this->getpos();
}

char* model::getfilename(){
    return this->filename;
}

/*
        // sets
        void setvida(int vida);
        void setvao(GLuint vao);
        void setnumvertices(int num);
        void setpos(vec3 p);
        void setfilename(char *f);
        */
