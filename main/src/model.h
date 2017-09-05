#ifndef MODEL_H
#define MODEL_H

using namespace std;

class model{
    private:
        int vida;
        GLuint vao, vbo;
        int numvertices;
        vec3 pos;
        char* filename;

    public:
        model(char *filename);
        void atacar();
        void moverse();
        void morir();

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
