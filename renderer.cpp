#include "libs.h"
#include "config.h"
#include "init.h"

#define NUM_VERTICES 4
#define NUM_INDICES 6

Vertex vertices[NUM_VERTICES] = {
  glm::vec2(-1.0f, -1.0f),    
  glm::vec2(1.0f, -1.0f),   
  glm::vec2(1.0f, 1.0f),
  glm::vec2(-1.0f, 1.0f),
};

GLuint indices[NUM_INDICES] = { 0, 1, 2, 0, 2, 3 };


int main(int argc, char **argv) {

  // GLFW
  glfwInit();

  // WINDOW
  int frameBufferWidth;
  int frameBufferHeight;
  GLFWwindow *window;

  init_window(&frameBufferWidth, &frameBufferHeight, &window);

  // GLEW
  init_glew();

  // OpenGL OPTIONS
  init_opengl_options();

  // SHADER
  GLuint core_program;
  init_shader(core_program);

  // VAO
  GLuint VAO;
  glCreateVertexArrays(1, &VAO);
  glBindVertexArray(VAO);

  // VBO
  GLuint VBO;
  glGenBuffers(1, &VBO);
  glBindBuffer(GL_ARRAY_BUFFER, VBO);
  glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

  // EBO
  GLuint EBO;
  glGenBuffers(1, &EBO);
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
  glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

  // SET VERTEX ATRRIB POINTERS AND ENABLE, INPUT ASSEMBLY
  glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (GLvoid *) offsetof(Vertex, position));
  glEnableVertexAttribArray(0);

  // UNBIND VAO
  glBindVertexArray(0);


  // UNIFORM
  GLint pos = glGetUniformLocation(core_program, "cam_pos");
  GLint dir = glGetUniformLocation(core_program, "cam_dir");

  float tick = 0;

  // MAIN
  while (!glfwWindowShouldClose(window)) {
    // inputs
    glfwPollEvents();

    // tick
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) glfwSetWindowShouldClose(window, GLFW_TRUE);

    glUniform3f(pos, 0, 0, tick);
    // glUniform3f(pos, std::cos(tick), 0, std::sin(tick));
    // glUniform3f(dir, std::sin(tick / 10), 0, 1);
    glUniform3f(dir, 0, 0, 1);
    // glUniform3f(dir, -std::cos(tick), 0, -std::sin(tick));

    tick += 0.2;

    // render
    glClearColor(CLEAR_COLOUR);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);

    // draw
    glUseProgram(core_program);
    glBindVertexArray(VAO);
    glDrawElements(GL_TRIANGLES, NUM_INDICES, GL_UNSIGNED_INT, 0);

    glfwSwapBuffers(window);
    glFlush();

  }



  // END
  glfwDestroyWindow(window);
  glfwTerminate();
  glDeleteProgram(core_program);

  return 0;
}