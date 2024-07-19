#ifndef SHADER_UTILS_H
#define SHADER_UTILS_H

GLuint load_shader(GLenum type, const char *shaderSrc);
GLuint create_program(const char *vertexSource, const char *fragmentSource);

#endif
