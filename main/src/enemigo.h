#ifndef ENEMIGO_H
#define ENEMIGO_H

using namespace std;

class enemigo{
    private:
        int vida;
        GLuint vao, vbo;
        int numvertices;
        char* filename;

    public:
        enemigo(char *filename);
        void atacar();
        void moverse();
        void morir();
        vec3 pos;
        

        // gets
        int getvida();
        GLuint getvao();
        int getnumvertices();
        vec3 getpos();
        char* getfilename();

        // sets
        void setvida(int vida);
        void setvao(GLuint vao);
        void setnumvertices(int num);
        void setpos(vec3 p);
        void setfilename(char *f);
};

#endif
