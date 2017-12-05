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
  this->load_texture_rgb("assets/black.png", "texasmp_rgb");
  //this->load_texture(const_cast<char *>("assets/black.png"));
}

Model::Model(char *filename, char *texname, char *normalname)
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
  this->load_texture_rgb(texname, "texsamp_rgb");
  this->load_texture_normal(normalname, "texsamp_normal");

  GLuint ka = glGetUniformLocation(shader_programme, "ka");
  GLuint kd = glGetUniformLocation(shader_programme, "kd");
  GLuint ks = glGetUniformLocation(shader_programme, "ks");


  glUniform4f(ka, 0.05, 0.05, 0.05, 1.0);
  glUniform4f(kd, 0.8, 0.8, 0.8, 1.0);
  glUniform4f(ks, 0.5, 0.5, 0.5, 1.0);
}

Model::Model(char *filename, char *texname, char *normalname, float nscale)
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
  this->scale(glm::vec3(nscale));
  this->load_texture_rgb(texname, "texsamp_rgb");
  this->load_texture_normal(normalname, "texsamp_normal");

  GLuint ka = glGetUniformLocation(shader_programme, "ka");
  GLuint kd = glGetUniformLocation(shader_programme, "kd");
  GLuint ks = glGetUniformLocation(shader_programme, "ks");

  glUniform4f(ka, 0.05, 0.05, 0.05, 1.0);
  glUniform4f(kd, 0.8, 0.8, 0.8, 1.0);
  glUniform4f(ks, 0.5, 0.5, 0.5, 1.0);
}

void Model::setAmbientalCoef(GLfloat a, GLfloat b, GLfloat c) {
  GLuint ka = glGetUniformLocation(shader_programme, "ka");

  glUniform4f(ka, a, b, c, 1.0);
}

void Model::setDiffuseCoef(GLfloat a, GLfloat b, GLfloat c)
{
  GLuint kd = glGetUniformLocation(shader_programme, "kd");

  glUniform4f(kd, a, b, c, 1.0);
}

void Model::setSpecularCoef(GLfloat a, GLfloat b, GLfloat c)
{
  GLuint ks = glGetUniformLocation(shader_programme, "ks");

  glUniform4f(ks, a, b, c, 1.0);
}

GLuint Model::getvao()
{
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
  this->setmatloc(shaderprog, "matrix");
  glActiveTexture(GL_TEXTURE0);
  glBindTexture(GL_TEXTURE_2D, tex_rgb);
  glUniform1i(texloc_rgb, 0);

  glUseProgram (shaderprog);
  glUniformMatrix4fv (this->matloc, 1, GL_FALSE, &this->model[0][0]);
}

void Model::draw() {
  glUseProgram(shader_programme);
  glBindVertexArray(this->vao);
  glActiveTexture(GL_TEXTURE0);
  glBindTexture(GL_TEXTURE_2D, tex_rgb);
  glActiveTexture(GL_TEXTURE1);
  glBindTexture(GL_TEXTURE_2D, tex_normal);

  glUniformMatrix4fv(this->matloc, 1, GL_FALSE, &this->model[0][0]);
  glDrawElements(this->FTYPE, this->FFACTOR * this->nfaces, GL_UNSIGNED_INT, 0);
  glBindVertexArray(0);

}

void Model::draw(GLuint shaderprog)
{
  glUseProgram(shaderprog);
  glActiveTexture(GL_TEXTURE0);
  glBindVertexArray(this->vao);
  glBindTexture(GL_TEXTURE_2D, tex_rgb);
  glUniformMatrix4fv(this->matloc, 1, GL_FALSE, &this->model[0][0]);
  glDrawElements(this->FTYPE, this->FFACTOR * this->nfaces, GL_UNSIGNED_INT, 0);
  glBindVertexArray(0);
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
}

char* Model::getfilename(){
  return this->filename;
}

bool Model::load_mesh (const char* file_name) {
  const aiScene* scene = aiImportFile(file_name, aiProcess_Triangulate);
  if (!scene) {
    return false;
  }

  const aiMesh* mesh = scene->mMeshes[0];

  this->numvertices = mesh->mNumVertices;
  this->nfaces = mesh->mNumFaces;
  this->FTYPE = GL_TRIANGLES;
  this->FFACTOR = 0;

  glGenVertexArrays (1, &this->vao);
  glBindVertexArray (this->vao);

  GLfloat* points = NULL; // array of vertex points
  GLfloat* normals = NULL; // array of vertex normals
  GLfloat* texcoords = NULL; // array of texture coordinates
  GLfloat *tangents = NULL;  // array of tangent vectors
  GLuint *indices = NULL;

  if (mesh->HasPositions ()) {
    points = (GLfloat*)malloc (this->numvertices * 3 * sizeof (GLfloat));
    GLfloat maxX;
    GLfloat maxY;
    GLfloat maxZ;
    GLfloat minX;
    GLfloat minY;
    GLfloat minZ;

    convexShape = new btConvexHullShape();

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

      if (numvertices < 400000) {
        convexShape->addPoint(btVector3(vp->x, vp->y, vp->z));
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
  if (mesh->HasFaces())
  {
    this->FFACTOR = mesh->mFaces[0].mNumIndices;
    indices = (GLuint *)malloc(this->nfaces * this->FFACTOR * sizeof(GLuint));
    for (int i = 0; i < this->nfaces; ++i)
    {
      for (int j = 0; j < this->FFACTOR; ++j)
      {
        indices[this->FFACTOR * i + j] = mesh->mFaces[i].mIndices[j];
      }
    }
  }

  if (mesh->HasTangentsAndBitangents())
  {
    tangents = (GLfloat *)malloc(this->numvertices * 4 * sizeof(GLfloat));
    for (int i = 0; i < this->numvertices; i++)
    {
      const aiVector3D *tangent = &(mesh->mTangents[i]);
      const aiVector3D *bitangent = &(mesh->mBitangents[i]);
      const aiVector3D *normal = &(mesh->mNormals[i]);

      // put the three vectors into my vec3 struct format for doing maths
      glm::vec3 t(tangent->x, tangent->y, tangent->z);
      glm::vec3 n(normal->x, normal->y, normal->z);
      glm::vec3 b(bitangent->x, bitangent->y, bitangent->z);
      glm::vec3 t_i = glm::normalize(t - n * glm::dot(n, t));

      // get determinant of T,B,N 3x3 matrix by dot*cross method
      float det = (glm::dot(glm::cross(n, t), b));
      if (det < 0.0f)
      {
        det = -1.0f;
      }
      else
      {
        det = 1.0f;
      }
      // push back 4d vector for inverse tangent with determinant
      tangents[i * 4 + 0] = t_i[0];
      tangents[i * 4 + 1] = t_i[1];
      tangents[i * 4 + 2] = t_i[2];
      tangents[i * 4 + 3] = det;
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
    if (indices)
    {
      glGenBuffers(1, &ebo);
      glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
      glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(GLuint) * nfaces * FFACTOR, indices, GL_STATIC_DRAW);
    }
    glVertexAttribPointer (0, 3, GL_FLOAT, GL_FALSE, 0, NULL);
    glEnableVertexAttribArray (0);
    this->points = points;
    free(indices);
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
  if (tangents)
  {
    glGenBuffers(1, &tanbo);
    glBindBuffer(GL_ARRAY_BUFFER, tanbo);
    glBufferData(GL_ARRAY_BUFFER, 4 * this->numvertices * sizeof(GLfloat), tangents, GL_STATIC_DRAW);
    glVertexAttribPointer(3, 4, GL_FLOAT, GL_FALSE, 0, NULL);
    glEnableVertexAttribArray(3);
    free(tangents);
  }

  glBindVertexArray(0);

  aiReleaseImport (scene);
  //printf ("mesh loaded\n");

  return true;
}

bool Model::load_texture(const char *file_name, GLuint *tex)
{
  int x, y, n;
  int force_channels = 4;
  unsigned char *image_data = stbi_load(file_name, &x, &y, &n, force_channels);
  if (!image_data)
  {
    fprintf(stderr, "ERROR: could not load %s\n", file_name);
    return false;
  }
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
  glGenTextures(1, tex);
  glBindTexture(GL_TEXTURE_2D, *tex);
  glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, x, y, 0, GL_RGBA, GL_UNSIGNED_BYTE, image_data);
  glGenerateMipmap(GL_TEXTURE_2D);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
  GLfloat max_aniso = 16.0f;
  glGetFloatv(GL_MAX_TEXTURE_MAX_ANISOTROPY_EXT, &max_aniso);

  glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAX_ANISOTROPY_EXT, max_aniso);
  return true;
}

bool Model::load_texture_normal(const char *filename, const char *sampler_name)
{
  glActiveTexture(GL_TEXTURE1);
  int code = load_texture(filename, &tex_normal);

  glUseProgram(shader_programme);

  texloc_normal = glGetUniformLocation(shader_programme, sampler_name);

  glUniform1i(texloc_normal, 1);
}

bool Model::load_texture_rgb(const char *filename, const char *sampler_name)
{
  glActiveTexture(GL_TEXTURE0);
  int code = load_texture(filename, &tex_rgb);

  glUseProgram(shader_programme);

  texloc_rgb = glGetUniformLocation(shader_programme, sampler_name);

  glUniform1i(texloc_rgb, 0);
  glUseProgram(0);
}
