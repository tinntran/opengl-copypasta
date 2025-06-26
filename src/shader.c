#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdarg.h>

#include "defer.h"
#include "shader.h"

bool read_file(FileContent* content, const char *path)
{
  bool result = true;
  (void)content;

  FILE* f = fopen(path, "rb");
  if (f == NULL)
    return_defer(false);

  if (fseek(f, 0, SEEK_END) == -1)
    return_defer(false);

  long len = ftell(f);

  if (fseek(f, 0, SEEK_SET) == -1)
    return_defer(false);

  size_t new_count = content->count + len;
  if (content->capacity < new_count) {
    content->items = realloc(content->items, new_count * sizeof(char));
    if (content->items == NULL)
      return_defer(false);

    content->capacity = new_count;
  }

  fread(content->items + content->count, len, 1, f);
  content->count = new_count;

defer:
  if (!result)
    MLOG("SHADER", "`%s` could not be read! (CODE %d): %s\n", path, errno, strerror(errno));
  if (f)
    fclose(f);

  return result;
}

int create_shader_from_glsl(GLuint* shader, GLenum shader_type, const char* path)
{
  int result = true;
  unsigned int id;

  FileContent content = {0};
  if (!read_file(&content, path))
    return_defer(false);

  id = glCreateShader(shader_type);

  glShaderSource(id, 1, (const char* const *)&content.items, NULL);
  glCompileShader(id);

  glGetShaderiv(id, GL_COMPILE_STATUS, &result);

  if (!result) {
    char log[512];

    glGetShaderInfoLog(id, 512, NULL, log);
    MLOG("SHADER", "Compilation error at `%s`:\n%s", path, log);
  } else {
    MLOG("SHADER", "Compiled `%s` with id `%d`\n", path, id);
  }

defer:
  *shader = id;
  free(content.items);
  return result;
}

bool create_shader_program(GLuint* program, size_t size, ...)
{
  *program = glCreateProgram();

  MLOG("SHADER", "Shader program created with id `%d`", *program);

  if (size > 0)
    printf(" with attachments [");

  va_list att;
  va_start(att, size);
  for (size_t i = 0; i < size; i++) {
    unsigned int id = va_arg(att, GLuint);
    glAttachShader(*program, id);

    if (i < size - 1)
      printf("%d, ", id);
    else
      printf("%d", id);
  }
  va_end(att);

  if (size > 0)
    printf("]");

  printf("\n");

  return true;
}
