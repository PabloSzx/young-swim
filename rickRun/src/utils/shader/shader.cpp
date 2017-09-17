#include "shader.h"
#include "../log/log.h"
#include <stdio.h>
#include <time.h>
#include <string.h>
#include <assert.h>
#include "../../data/constants.h"

bool shader_parse_file_into_str(
	const char *file_name, char *shader_str, int max_len)
{
	shader_str[0] = '\0'; // reset string
	FILE* file = fopen (file_name , "r");
	if (!file) {
		log_gl_log_err ("ERROR: opening file for reading: %s\n", file_name);
		return false;
	}
	int current_len = 0;
	char line[2048];
	strcpy (line, ""); // remember to clean up before using for first time!
	while (!feof (file)) {
		if (NULL != fgets (line, 2048, file)) {
			current_len += strlen (line); // +1 for \n at end
			if (current_len >= max_len) {
				log_gl_log_err (
					"ERROR: shader length is longer than string buffer length %i\n",
					max_len
				);
			}
			strcat (shader_str, line);
		}
	}
	if (EOF == fclose (file)) { // probably unnecesssary validation
		log_gl_log_err ("ERROR: closing file from reading %s\n", file_name);
		return false;
	}
	return true;
}

bool shader_create_shader(const char *file_name, GLuint *shader, GLenum type)
{
	log_gl_log ("creating shader from %s...\n", file_name);
	char shader_string[MAX_SHADER_LENGTH];
	shader_parse_file_into_str (file_name, shader_string, MAX_SHADER_LENGTH);
	*shader = glCreateShader (type);
	const GLchar* p = (const GLchar*)shader_string;
	glShaderSource (*shader, 1, &p, NULL);
	glCompileShader (*shader);
	// check for compile errors
	int params = -1;
	glGetShaderiv (*shader, GL_COMPILE_STATUS, &params);
	if (GL_TRUE != params) {
		log_gl_log_err ("ERROR: GL shader index %i did not compile\n", *shader);
		log_print_shader_info_log (*shader);
		return false; // or exit or something
	}
	log_gl_log ("shader compiled. index %i\n", *shader);
	return true;
}

void shader_print_programme_info_log(GLuint sp)
{
	int max_length = 2048;
	int actual_length = 0;
	char log[2048];
	glGetProgramInfoLog (sp, max_length, &actual_length, log);
	printf ("program info log for GL index %u:\n%s", sp, log);
	log_gl_log ("program info log for GL index %u:\n%s", sp, log);
}

bool shader_is_programme_valid(GLuint sp)
{
	glValidateProgram (sp);
	GLint params = -1;
	glGetProgramiv (sp, GL_VALIDATE_STATUS, &params);
	if (GL_TRUE != params) {
		log_gl_log_err ("program %i GL_VALIDATE_STATUS = GL_FALSE\n", sp);
		shader_print_programme_info_log (sp);
		return false;
	}
	log_gl_log ("program %i GL_VALIDATE_STATUS = GL_TRUE\n", sp);
	return true;
}

bool shader_create_programme(GLuint vert, GLuint frag, GLuint *programme)
{
	*programme = glCreateProgram ();
	log_gl_log(
		"created programme %u. attaching shaders %u and %u...\n",
		*programme,
		vert,
		frag);
	glAttachShader (*programme, vert);
	glAttachShader (*programme, frag);
	// link the shader programme. if binding input attributes do that before link
	glLinkProgram (*programme);
	GLint params = -1;
	glGetProgramiv (*programme, GL_LINK_STATUS, &params);
	if (GL_TRUE != params) {
		log_gl_log_err (
			"ERROR: could not link shader programme GL index %u\n",
			*programme
		);
		shader_print_programme_info_log (*programme);
		return false;
	}
	// assert (is_programme_valid (*programme));
	// delete shaders here to free memory
	glDeleteShader (vert);
	glDeleteShader (frag);
	return true;
}

GLuint shader_create_programme_from_files()
{
	GLuint vert, frag, programme;
	assert(shader_create_shader(VERTEX_SHADER_FILE, &vert, GL_VERTEX_SHADER));
	assert(shader_create_shader(FRAGMENT_SHADER_FILE, &frag, GL_FRAGMENT_SHADER));
	assert (shader_create_programme (vert, frag, &programme));
	return programme;
}
