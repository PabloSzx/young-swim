#define GL_LOG_FILE "log/gl.log"

#ifdef APPLE
#define VERTEX_SHADER_FILE "shaders/mac/test_vs.glsl"
#define FRAGMENT_SHADER_FILE "shaders/mac/test_fs.glsl"
#else
#define VERTEX_SHADER_FILE "shaders/linux/test_vs.glsl"
#define FRAGMENT_SHADER_FILE "shaders/linux/test_fs.glsl"
#endif

#define RICK "mallas/rick.obj"
#define SUELO "mallas/suelo.obj"
#define PLATAFORMA "mallas/plataforma.obj"
