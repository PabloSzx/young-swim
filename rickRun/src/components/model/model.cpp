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
#include "../../util/window/window.hpp"
#include "../../util/stb/stb_image.hpp"
#include "model.hpp"

using namespace std;

Model::Model(char *filename)
{
  this->pos = glm::vec3(0, 0, 0);
  this->model = glm::mat4();
  this->filename = filename;
  this->matloc = 0;
  this->lastScale = glm::vec3(1.0f, 1.0f, 1.0f);
  assert(this->load_mesh(filename));
  this->setmatloc(shader_programme, "matrix");
  this->red = 1.0f;
  this->green = 1.0f;
  this->blue = 1.0f;

  this->setpos(glm::vec3(0.0f, 0.0f, 0.0f));
  this->scale(glm::vec3(1.0f));
  // this->load_texture(const_cast<char *>("mesh/white.jpg"));
  this->load_texture(const_cast<char *>("mesh/black.png"));
}

Model::Model(char *filename, char *texname)
{
  this->pos = glm::vec3(0, 0, 0);
  this->model = glm::mat4();
  this->filename = filename;
  this->matloc = 0;
  this->lastScale = glm::vec3(1.0f, 1.0f, 1.0f);
  assert(this->load_mesh(filename));
  this->setmatloc(shader_programme, "matrix");
  this->red = 1.0f;
  this->green = 1.0f;
  this->blue = 1.0f;

  this->setpos(glm::vec3(0.0f, 0.0f, 0.0f));
  this->scale(glm::vec3(1.0f));
  this->load_texture(texname);
}

GLuint Model::getvao(){
  return this->vao;
}

int Model::getnumvertices(){
  return this->numvertices;
}

glm::vec3 Model::getpos(){
  return this->pos;
}

glm::mat4 Model::getmodel(){
  return this->model;
}

void Model::scale(glm::vec3 vec) {
  this->lastScale = vec;
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

void Model::setpos(glm::vec3 pos){
  this->pos = pos;
  this->model = glm::translate(glm::mat4(), this->pos);
  this->model = glm::scale(this->model, this->lastScale);
}

void Model::setmatloc(GLuint shaderprog, const char* matrix) {
  this->matloc = glGetUniformLocation (shaderprog, matrix);
}

void Model::model2shader(GLuint shaderprog) {
  this->setmatloc(shader_programme, "matrix");
  // glUniform3f(color, this->red, this->green, this->blue);
  GLuint tex_location = glGetUniformLocation(shaderprog, "basic_texture");
  glActiveTexture(GL_TEXTURE0);
  glBindTexture(GL_TEXTURE_2D, tex);
  glUniform1i(tex_location, 0);

  glUseProgram (shaderprog);
  glUniformMatrix4fv (this->matloc, 1, GL_FALSE, &this->model[0][0]);
}

void Model::draw() {
  this->model2shader(shader_programme);
  glBindVertexArray(this->getvao());
  glDrawArrays(GL_TRIANGLES, 0, this->getnumvertices());
}

bool Model::colisiona (Model* comparacion) {
  GLfloat dx = abs(this->pos[0] - comparacion->pos[0]);
  GLfloat dy = abs(this->pos[1] - comparacion->pos[1]);
  GLfloat dz = abs(this->pos[2] - comparacion->pos[2]);

  if ((dx <= (this->LX/2 + comparacion->LX/2)) && (dy <= (this->LY/2 + comparacion->LY/2)) && (dz <= (this->LZ/2 + comparacion->LZ/2))) {
    return true;
  }

  return false;
}

void Model::setColor(GLfloat r, GLfloat g, GLfloat b) {
  this->red = r;
  this->green = g;
  this->blue = b;
}

void Model::printMax() {
  cout << "maxX: " << this->maxX << endl;
  cout << "maxY: " << this->maxY << endl;
  cout << "maxZ: " << this->maxZ << endl;

  cout << "minX: " << this->minX << endl;
  cout << "minY: " << this->minY << endl;
  cout << "minZ: " << this->minZ << endl;
}

char* Model::getfilename(){
  return this->filename;
}

bool Model::load_mesh (const char* file_name) {
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

    this->texcoords = texcoords;
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

bool Model::load_texture(const char *file_name)
{
  int x, y, n;
  int force_channels = 4;
  unsigned char *image_data = stbi_load(file_name, &x, &y, &n, force_channels);
  //printf("x = %i    y = %i\n", x, y);
  if (!image_data)
  {
    fprintf(stderr, "ERROR: could not load %s\n", file_name);
    return false;
  }
  // NPOT check
  if ((x & (x - 1)) != 0 || (y & (y - 1)) != 0)
  {
    fprintf(
        stderr, "WARNING: texture %s is not power-of-2 dimensions\n", file_name);
  }
  int width_in_bytes = x * 4;
  unsigned char *top = NULL;
  unsigned char *bottom = NULL;
  unsigned char temp = 0;
  int half_height = y / 2;

  for (int row = 0; row < half_height; row++)
  {
    top = image_data + row * width_in_bytes;
    bottom = image_data + (y - row - 1) * width_in_bytes;
    for (int col = 0; col < width_in_bytes; col++)
    {
      temp = *top;
      *top = *bottom;
      *bottom = temp;
      top++;
      bottom++;
    }
  }
  tex = 0;
  glGenTextures(1, &tex);
  glActiveTexture(GL_TEXTURE0);
  glBindTexture(GL_TEXTURE_2D, tex);
  glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, x, y, 0, GL_RGBA, GL_UNSIGNED_BYTE, image_data);
  glGenerateMipmap(GL_TEXTURE_2D);
  // probar cambiar GL_CLAMP_TO_EDGE por GL_REPEAT
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
  GLfloat max_aniso = 16.0f;
  glGetFloatv(GL_MAX_TEXTURE_MAX_ANISOTROPY_EXT, &max_aniso);
  // set the maximum!
  glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAX_ANISOTROPY_EXT, max_aniso);

  // GLuint tex_location = glGetUniformLocation(shader_programme, "basic_texture");
  // glUniform1i(tex_location, 0); 
  return true;
}
