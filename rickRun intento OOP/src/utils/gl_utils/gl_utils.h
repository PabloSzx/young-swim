#ifndef GL_UTILS_H
#define GL_UTILS_H

#include <GLFW/glfw3.h>

class GLUtils{
private:
public:
  GLUtils();
  GLuint shader_programme;
};

bool create_shader(const char *file_name, GLuint *shader, GLenum type);
bool create_programme(GLuint vert, GLuint frag, GLuint *programme);
bool parse_file_into_str(const char *file_name, char *shader_str, int max_len);
bool is_programme_valid(GLuint sp);
GLuint create_programme_from_files();

extern GLUtils *engine;
extern GLuint shader_programme;
#endif
