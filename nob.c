#define NOB_IMPLEMENTATION

#include <stdio.h>
#include <stdbool.h>

#include "nob.h"

#define CC "cc"
#define CFLAGS "-Wall", "-Wextra", "-ggdb"
#define LIBS "-Llib/", "-lglfw3", "-lGLEW", "-lGL", "-lm"

#define SRCDIR "src/"
#define BUILDDIR ".build/"
#define EXE "copypasta"

const char* executables[] = {
  "main.c",
  "shader.c"
};

bool buildo()
{
  bool result = true;
  Nob_Cmd cmd = {0};
  Nob_String_Builder input = {0}, output = {0};

  for (size_t i = 0; i < NOB_ARRAY_LEN(executables); i++) {
    input.count = 0;
    nob_sb_append_cstr(&input, SRCDIR);
    nob_sb_append_cstr(&input, executables[i]);
    nob_sb_append_null(&input);

    output.count = 0;
    nob_sb_append_cstr(&output, BUILDDIR);
    nob_sb_append_cstr(&output, executables[i]);
    nob_sb_append_cstr(&output, ".o");
    nob_sb_append_null(&output);

    nob_cmd_append(&cmd, CC, CFLAGS);
    nob_cmd_append(&cmd, "-o", output.items);
    nob_cmd_append(&cmd, "-c", input.items);

    if (!nob_cmd_run_sync_and_reset(&cmd))
      nob_return_defer(false);
  }

defer:
  nob_cmd_free(cmd);
  nob_sb_free(input);
  return result;
}

bool buildexe()
{
  bool result = true;
  Nob_Cmd cmd = {0};
  Nob_String_Builder input = {0};

  nob_cmd_append(&cmd, CC, CFLAGS);
  nob_cmd_append(&cmd, "-o", EXE);

  for (size_t i = 0; i < NOB_ARRAY_LEN(executables); i++) {
    input.count = 0;
    nob_sb_append_cstr(&input, BUILDDIR);
    nob_sb_append_cstr(&input, executables[i]);
    nob_sb_append_cstr(&input, ".o");
    nob_sb_append_null(&input);

    nob_cmd_append(&cmd, nob_temp_strdup(input.items));
  }

  nob_cmd_append(&cmd, LIBS);

  if (!nob_cmd_run_sync_and_reset(&cmd))
    nob_return_defer(false);

defer:
  nob_cmd_free(cmd);
  nob_sb_free(input);
  return result;
}

int main(int argc, char** argv)
{
  NOB_GO_REBUILD_URSELF(argc, argv);

  if (!nob_mkdir_if_not_exists(BUILDDIR))
    return 1;

  if (!buildo() || !buildexe())
    return 1;

  return 0;
}
