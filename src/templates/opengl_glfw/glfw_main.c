#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#define GL_GLEXT_PROTOTYPES

#include <GLFW/glfw3.h>
#include <GL/gl.h>
#include <GL/glext.h>

void window_resize_cb(GLFWwindow *win, int w, int h) {
  glViewport(0,0,w,h);
}

void draw_triangle() {
  static bool data_loaded = false;
  if (data_loaded) goto render;
  static const float vertices[] = {
    -0.75, -0.25, 0.0,
    -0.25, -0.25, 0.0,
    -0.5, 0.25, 0.0,
    0.25, -0.25, 0.0,
    0.75, -0.25, 0.0,
    0.5, 0.25, 0.0
  };
  static const unsigned int indices[] = {0, 1, 2, 3, 4, 5};
  static const char* triangle_vertex_shader_src =
    "#version 330 core\n"
    "layout (location = 0) in vec3 aPos;\n"
    "void main() {\n"
    "  gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.0);\n"
    "}\0";
  static const char* triangle_fragment_shader_src =
    "#version 330 core\n"
    "out vec4 FragColor;\n"
    "void main() {\n"
    "  FragColor = vec4(1.0, 0.5, 0.2, 1.0);\n"
    "}\0";
  static unsigned int VBO = 0;
  static unsigned int EBO = 0;
  static unsigned int VAO = 0;
  static unsigned int gl_vertex_shader = 0;
  static unsigned int gl_fragment_shader = 0;
  static unsigned int gl_shader_program = 0;
  int rc = 0;
  char *log_str = NULL;

  // load vertices (VBO & VAO)
  glGenVertexArrays(1, &VAO);
  glGenBuffers(1, &VBO);
  glGenBuffers(1, &EBO);
  glBindVertexArray(VAO);
  glBindBuffer(GL_ARRAY_BUFFER, VBO);
  glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
  glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

  // compile vertex shader
  gl_vertex_shader = glCreateShader(GL_VERTEX_SHADER);
  glShaderSource(gl_vertex_shader, 1, &triangle_vertex_shader_src, NULL);
  glCompileShader(gl_vertex_shader);
  glGetShaderiv(gl_vertex_shader, GL_COMPILE_STATUS, &rc);
  if (!rc) {
    log_str = (char*)calloc(100, sizeof(char));
    glGetShaderInfoLog(gl_vertex_shader, 100, NULL, log_str);
    fprintf(stderr, "Could not compile vertex shader:\n");
    fprintf(stderr, "%s\n", log_str);
    free(log_str);
    exit(1);
  }

  // compile fragment shader
  gl_fragment_shader = glCreateShader(GL_FRAGMENT_SHADER);
  glShaderSource(gl_fragment_shader, 1, &triangle_fragment_shader_src, NULL);
  glCompileShader(gl_fragment_shader);
  glGetShaderiv(gl_fragment_shader, GL_COMPILE_STATUS, &rc);
  if (!rc) {
    log_str = (char*)calloc(100, sizeof(char));
    glGetShaderInfoLog(gl_fragment_shader, 100, NULL, log_str);
    fprintf(stderr, "Could not compile fragment shader:\n");
    fprintf(stderr, "%s\n", log_str);
    free(log_str);
    exit(1);
  }

  // link shader
  gl_shader_program = glCreateProgram();
  glAttachShader(gl_shader_program, gl_vertex_shader);
  glAttachShader(gl_shader_program, gl_fragment_shader);
  glLinkProgram(gl_shader_program);
  glGetProgramiv(gl_shader_program, GL_LINK_STATUS, &rc);
  if (!rc) {
    log_str = (char*)calloc(100, sizeof(char));
    glGetProgramInfoLog(gl_shader_program, 100, NULL, log_str);
    fprintf(stderr, "Could not link shader program:\n");
    fprintf(stderr, "%s\n", log_str);
    free(log_str);
    exit(1);
  }
  glDeleteShader(gl_vertex_shader);
  glDeleteShader(gl_fragment_shader);
  gl_vertex_shader = 0;
  gl_fragment_shader = 0;

  // setup vertex attributes
  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3*sizeof(float), NULL);
  glEnableVertexAttribArray(0);

  // enable shader program
  glUseProgram(gl_shader_program);

  render:
  //glDrawArrays(GL_TRIANGLES, 0, 3);
  //glDrawElements(GL_TRIANGLES, 3, GL_UNSIGNED_INT, NULL);
  glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
  glDrawRangeElements(GL_TRIANGLES, 0, 2, 3, GL_UNSIGNED_INT, NULL);
  glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
  glDrawRangeElements(GL_TRIANGLES, 3, 5, 3, GL_UNSIGNED_INT, (void*)(3*sizeof(unsigned int)));
}

int main() {
  glfwInit();
  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
  glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
  glfwWindowHint(GLFW_MOUSE_PASSTHROUGH, GLFW_TRUE);

  GLFWwindow *w = NULL;
  if (!(w = glfwCreateWindow(800, 600, "test", NULL, NULL))) {
    fprintf(stderr, "Could not create window\n");
    exit(1);
  }

  glViewport(0,0,800,600);
  glfwMakeContextCurrent(w);
  glfwSetFramebufferSizeCallback(w, window_resize_cb);
  glfwSwapInterval(1);
  glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
  //glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

  while(!glfwWindowShouldClose(w)) {
    printf("a\n");
    glClearColor(0.2, 0.3, 0.3, 1.0);
    glClear(GL_COLOR_BUFFER_BIT);
    draw_triangle();

    glfwSwapBuffers(w);
    glfwWaitEvents();
  }

  glfwDestroyWindow(w);
  glfwTerminate();
  return 0;
}