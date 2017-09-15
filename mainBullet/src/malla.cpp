#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <assert.h>
#include <iostream>
#include <string>
#include <GL/glew.h> // include GLEW and new version of GL on Windows
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <assimp/cimport.h> // C importer
#include <assimp/scene.h> // collects data
#include <assimp/postprocess.h> // various extra operations
#include "tools.h"
#include "malla.h"

using namespace std;
malla::malla(char* filename) {
  this->pos = glm::vec3(0,0,0);
  this->model = glm::mat4();
  this->filename = filename;
  this->matloc = 0;
  assert(this->load_mesh(filename));
}

GLuint malla::getvao(){
  return this->vao;
}

int malla::getnumvertices(){
  return this->numvertices;
}

glm::vec3 malla::getpos(){
  return this->pos;
}

glm::mat4 malla::getmodel(){
  return this->model;
}

void malla::scale(glm::vec3 vec) {
  this->model = glm::scale(this->model, vec);

  for (int i = 0; i < this->numvertices; i++) {
    this->points[i * 3] = this->points[i * 3] * vec[0];
    this->points[i * 3 + 1] = this->points[i * 3 + 1] * vec[1];
    this->points[i * 3 + 2] = this->points[i * 3 + 2] * vec[2];
  }

  this->maxX = this->maxX * vec[0];
  this->maxY = this->maxY * vec[1];
  this->maxZ = this->maxZ * vec[2];
  this->minX = this->minX * vec[0];
  this->minY = this->minY * vec[1];
  this->minZ = this->minZ * vec[2];


  this->LX = this->maxX - this->minX;
  this->LY = this->maxY - this->minY;
  this->LZ = this->maxZ - this->minZ;
}

void malla::setpos(glm::vec3 pos){
  this->pos = pos;
  this->model = glm::translate(glm::mat4(), this->pos);
}

void malla::setmatloc(GLuint shaderprog, const char* matrix) {
  this->matloc = glGetUniformLocation (shaderprog, matrix);
}

void malla::model2shader(GLuint shaderprog) {
  glUseProgram (shaderprog);
  glUniformMatrix4fv (this->matloc, 1, GL_FALSE, &this->model[0][0]);

}

bool malla::colisiona (malla* comparacion) {
  GLfloat dx = abs(this->pos[0] - comparacion->pos[0]);
  GLfloat dy = abs(this->pos[1] - comparacion->pos[1]);
  GLfloat dz = abs(this->pos[2] - comparacion->pos[2]);

  if ((dx <= (this->LX/2 + comparacion->LX/2)) && (dy <= (this->LY/2 + comparacion->LY/2)) && (dz <= (this->LZ/2 + comparacion->LZ/2))) {
    return true;
  }

  return false;
}

void malla::printMax() {
  cout << "maxX: " << this->maxX << endl;
  cout << "maxY: " << this->maxY << endl;
  cout << "maxZ: " << this->maxZ << endl;

  cout << "minX: " << this->minX << endl;
  cout << "minY: " << this->minY << endl;
  cout << "minZ: " << this->minZ << endl;
}

char* malla::getfilename(){
  return this->filename;
}

bool malla::load_mesh (const char* file_name) {
  const aiScene* scene = aiImportFile(file_name, aiProcess_Triangulate);
  if (!scene) {
    fprintf (stderr, "ERROR: reading mesh %s\n", file_name);
    return false;
  }
  printf ("  %i animations\n", scene->mNumAnimations);
  printf ("  %i cameras\n", scene->mNumCameras);
  printf ("  %i lights\n", scene->mNumLights);
  printf ("  %i materials\n", scene->mNumMaterials);
  printf ("  %i meshes\n", scene->mNumMeshes);
  printf ("  %i textures\n", scene->mNumTextures);

  /* get first mesh in file only */
  const aiMesh* mesh = scene->mMeshes[0];
  printf ("    %i vertices in mesh[0]\n", mesh->mNumVertices);

  /* pass back number of vertex points in mesh */
  this->numvertices = mesh->mNumVertices;

  /* generate a VAO, using the pass-by-reference parameter that we give to the
  function */
  glGenVertexArrays (1, &this->vao);
  glBindVertexArray (this->vao);

  /* we really need to copy out all the data from AssImp's funny little data
  structures into pure contiguous arrays before we copy it into data buffers
  because assimp's texture coordinates are not really contiguous in memory.
  i allocate some dynamic memory to do this. */
  GLfloat* points = NULL; // array of vertex points
  GLfloat* normals = NULL; // array of vertex normals
  GLfloat* texcoords = NULL; // array of texture coordinates
  if (mesh->HasPositions ()) {
    points = (GLfloat*)malloc (this->numvertices * 3 * sizeof (GLfloat));
    GLfloat maxX;
    GLfloat maxY;
    GLfloat maxZ;
    GLfloat minX;
    GLfloat minY;
    GLfloat minZ;

    for (int i = 0; i < this->numvertices; i++) {
      const aiVector3D* vp = &(mesh->mVertices[i]);
      if (i == 0) {
        maxX = (GLfloat)vp->x;
        minX = (GLfloat)vp->x;
        minY = (GLfloat)vp->y;
        maxY = (GLfloat)vp->y;
        minZ = (GLfloat)vp->z;
        maxZ = (GLfloat)vp->z;
      } else {
        if ((GLfloat)vp->x > maxX) {
          maxX = (GLfloat)vp->x;
        }
        if ((GLfloat)vp->y > maxY) {
          maxY = (GLfloat)vp->y;
        }
        if ((GLfloat)vp->z > maxZ) {
          maxZ = (GLfloat)vp->z;
        }
        if ((GLfloat)vp->x < minX) {
          minX = (GLfloat)vp->x;
        }
        if ((GLfloat)vp->y < minY) {
          minY = (GLfloat)vp->y;
        }
        if ((GLfloat)vp->z < minZ) {
          minZ = (GLfloat)vp->z;
        }
      }

      points[i * 3] = (GLfloat)vp->x;
      points[i * 3 + 1] = (GLfloat)vp->y;
      points[i * 3 + 2] = (GLfloat)vp->z;
    }

    this->maxX = maxX;
    this->maxY = maxY;
    this->maxZ = maxZ;
    this->minX = minX;
    this->minY = minY;
    this->minZ = minZ;

    this->LX = maxX - minX;
    this->LY = maxY - minY;
    this->LZ = maxZ - minZ;
  }
  if (mesh->HasNormals ()) {
    normals = (GLfloat*)malloc (this->numvertices * 3 * sizeof (GLfloat));
    for (int i = 0; i < this->numvertices; i++) {
      const aiVector3D* vn = &(mesh->mNormals[i]);
      normals[i * 3] = (GLfloat)vn->x;
      normals[i * 3 + 1] = (GLfloat)vn->y;
      normals[i * 3 + 2] = (GLfloat)vn->z;
    }
  }
  if (mesh->HasTextureCoords (0)) {
    texcoords = (GLfloat*)malloc (this->numvertices * 2 * sizeof (GLfloat));
    for (int i = 0; i < this->numvertices; i++) {
      const aiVector3D* vt = &(mesh->mTextureCoords[0][i]);
      texcoords[i * 2] = (GLfloat)vt->x;
      texcoords[i * 2 + 1] = (GLfloat)vt->y;
    }
  }

  /* copy mesh data into VBOs */
  if (mesh->HasPositions ()) {
    GLuint vbo;
    glGenBuffers (1, &vbo);
    glBindBuffer (GL_ARRAY_BUFFER, vbo);
    glBufferData (
      GL_ARRAY_BUFFER,
      3 * this->numvertices * sizeof (GLfloat),
      points,
      GL_STATIC_DRAW
    );
    glVertexAttribPointer (0, 3, GL_FLOAT, GL_FALSE, 0, NULL);
    glEnableVertexAttribArray (0);
    this->points = points;
    // free (points);
  }
  if (mesh->HasNormals ()) {
    GLuint vbo;
    glGenBuffers (1, &vbo);
    glBindBuffer (GL_ARRAY_BUFFER, vbo);
    glBufferData (
      GL_ARRAY_BUFFER,
      3 * this->numvertices * sizeof (GLfloat),
      normals,
      GL_STATIC_DRAW
    );
    glVertexAttribPointer (1, 3, GL_FLOAT, GL_FALSE, 0, NULL);
    glEnableVertexAttribArray (1);
    free (normals);
  }
  if (mesh->HasTextureCoords (0)) {
    GLuint vbo;
    glGenBuffers (1, &vbo);
    glBindBuffer (GL_ARRAY_BUFFER, vbo);
    glBufferData (
      GL_ARRAY_BUFFER,
      2 * this->numvertices * sizeof (GLfloat),
      texcoords,
      GL_STATIC_DRAW
    );
    glVertexAttribPointer (2, 2, GL_FLOAT, GL_FALSE, 0, NULL);
    glEnableVertexAttribArray (2);
    free (texcoords);
  }
  if (mesh->HasTangentsAndBitangents ()) {
    // NB: could store/print tangents here
  }

  aiReleaseImport (scene);
  printf ("mesh loaded\n");

  return true;
}
