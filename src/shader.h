#ifndef SHADER_H_
#define SHADER_H_

#include <stdio.h>
#include <stdbool.h>

#include "GL/glew.h"

typedef struct {
  char* items;
  size_t count, capacity;
} FileContent;

bool read_file(FileContent* name, const char* path);
int create_shader_from_glsl(GLuint* shader, GLenum shader_type, const char* path);
bool create_shader_program(GLuint* program, size_t size, ...);

#endif //SHADER_H_
