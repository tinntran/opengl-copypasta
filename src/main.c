#include "defer.h"
#include "GL/glew.h"
#include "GLFW/glfw3.h"
#include "shader.h"

void error_callback(int error_code, const char* description)
{
  fprintf(stderr, "{GLFW} An error has occured! (CODE %d): %s\n", error_code, description);
}

void frame_buffer_size_callback(GLFWwindow* _window, int width, int height)
{
  (void)_window;
  glViewport(0, 0, width, height);
}

int main(void)
{
  int result = 0;

  glfwSetErrorCallback(error_callback);

  if (!glfwInit())
    return_defer(1);

  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
  glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
  glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE);

  GLFWwindow* window = glfwCreateWindow(1280, 720, "COPYPASTA", NULL, NULL);
  glfwMakeContextCurrent(window);

  if (glewInit()) {
    MLOG("GL", "GLEW could not initialized! {CODE %d}: %s\n", glGetError(), glewGetErrorString(glGetError()));
    return_defer(1);
  }

  MLOG("GL", "OpenGL v%s\n", glGetString(GL_VERSION));

  unsigned int vertex_shader, fragment_shader;

  if (!create_shader_from_glsl(&vertex_shader, GL_VERTEX_SHADER, "src/glsl/vertex.glsl"))
    return_defer(1);
  if (!create_shader_from_glsl(&fragment_shader, GL_FRAGMENT_SHADER, "src/glsl/fragment.glsl"))
    return_defer(1);

  unsigned int program;

  create_shader_program(&program, 2, vertex_shader, fragment_shader);
  glLinkProgram(program);

  glDeleteShader(vertex_shader);
  glDeleteShader(fragment_shader);

  float vertices[] = {
    -0.5f, -0.5f, 1.0, 0.0, 0.0, // bottom left
     0.5f, -0.5f, 0.0, 1.0, 0.0, // bottom right
    -0.5f,  0.5f, 0.0, 0.0, 1.0, // top left
     0.5f,  0.5f, 1.0, 1.0, 1.0  // top right
  };

  unsigned int indices[] = {
    0, 1, 2,
    1, 2, 3
  };

  unsigned int VBO, VAO;
  glGenVertexArrays(1, &VAO);
  glGenBuffers(1, &VBO);

  unsigned int vao, vbo, ebo;
  glGenVertexArrays(1, &vao);
  glGenBuffers(1, &vbo);
  glGenBuffers(1, &ebo);

  glBindVertexArray(vao);
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
      glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
      glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

      glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
      glEnableVertexAttribArray(0);
      glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(2 * sizeof(float)));
      glEnableVertexAttribArray(1);

    glBindBuffer(GL_ARRAY_BUFFER, 0);
  glBindVertexArray(0);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

  while (!glfwWindowShouldClose(window)) {
    glClearColor(0.1, 0.0, 0.0, 1);
    glClear(GL_COLOR_BUFFER_BIT);

    glUseProgram(program);
    glBindVertexArray(vao);
    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

    glfwSwapBuffers(window);
    glfwPollEvents();
  }

defer:
  if (window)
    glfwDestroyWindow(window);

  glfwTerminate();
  return result;
}
