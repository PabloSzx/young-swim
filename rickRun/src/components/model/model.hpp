#ifndef MALLA_H
#define MALLA_H

#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <btBulletDynamicsCommon.h>

using namespace std;

extern GLuint shader_programme;
extern GLuint color;
class Model{
private:
  GLuint vao, vbo, tanbo, ebo;
  int numvertices;
  glm::vec3 pos;
  glm::mat4 model;
  int matloc;
  GLfloat *points;
  GLfloat maxX; //maxX ES EL PUNTO MAS ALTO DEL OBJETO ORIENTADO EN (0, 0)
  GLfloat maxY; //maxY ES EL PUNTO MAS ALTO DEL OBJETO ORIENTADO EN (0, 0)
  GLfloat maxZ; //maxZ ES EL PUNTO MAS ALTO DEL OBJETO ORIENTADO EN (0, 0)
  GLfloat minX; //minX ES EL PUNTO MAS BAJO DEL OBJETO ORIENTADO EN (0, 0)
  GLfloat minY; //minY ES EL PUNTO MAS BAJO DEL OBJETO ORIENTADO EN (0, 0)
  GLfloat minZ; //minZ ES EL PUNTO MAS BAJO DEL OBJETO ORIENTADO EN (0, 0)
  char *filename;
  glm::vec3 lastScale;
  GLfloat red;
  GLfloat green;
  GLfloat blue;
  GLfloat *texcoords;
  // GLuint tex;

  GLuint FTYPE;
  GLuint FFACTOR;
  GLuint nfaces;

  GLuint tex_rgb, tex_normal;
  GLint texloc_rgb, texloc_normal;

public:
  btConvexHullShape *convexShape;
  Model(char *filename);
  Model(char *filename, char *texname, char *normalname);
  Model(char *filename, char *texname, char *normalname, float nscale);
  GLfloat LX; //LX ES EL LARGO EN EL EJE X DEL OBJETO
  GLfloat LY; //LY ES EL LARGO EN EL EJE Y DEL OBJETO
  GLfloat LZ; //LZ ES EL LARGO EN EL EJE Z DEL OBJETO

  // gets
  GLuint getvao();
  int getnumvertices();
  glm::vec3 getpos();
  glm::mat4 getmodel();

  char *getfilename();

  // sets
  void setvao(GLuint vao);
  void setnumvertices(int num);
  void setpos(glm::vec3 p);
  void scale(glm::vec3 vec);
  void setfilename(char *f);
  void setmatloc(GLuint shader, const char *mat);
  void model2shader(GLuint shaderprog);
  void draw();
  void draw(GLuint shaderprog);
  bool load_mesh(const char *file_name);
  bool load_texture(const char *file_name, GLuint *tex);
  bool load_texture_rgb(const char *filename, const char *sampler_name);
  bool load_texture_normal(const char *filename, const char *sampler_name);
  void setAmbientalCoef(GLfloat a, GLfloat b, GLfloat c);
  void setDiffuseCoef(GLfloat a, GLfloat b, GLfloat c);
  void setSpecularCoef(GLfloat a, GLfloat b, GLfloat c);

  void printMax();
  bool colisiona(Model *compar);
  void setColor(GLfloat red, GLfloat green, GLfloat blue);
};

#endif
