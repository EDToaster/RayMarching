using namespace std;

#include "init.h"
#include "config.h"

#define ERR_S_COMPILE 1
#define ERR_P_LINKING 2

void init_window(int *frameBufferWidth, int *frameBufferHeight, GLFWwindow **window) {
  *frameBufferWidth = 0;
  *frameBufferHeight = 0;

  glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, VERSION_MAJOR);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, VERSION_MINOR);
  glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);

  // macOS x
  glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);

  *window = glfwCreateWindow(WIDTH, HEIGHT, TITLE, NULL, NULL);

  glfwGetFramebufferSize(*window, frameBufferWidth, frameBufferHeight);
  glViewport(0, 0, *frameBufferWidth, *frameBufferHeight);

  glfwMakeContextCurrent(*window);
}

void init_glew() {
  glewExperimental = GL_TRUE;

  // error check
  if (glewInit() != GLEW_OK) {
    cerr << "GLEW init failed" << endl;
    glfwTerminate();
  }
}

GLuint create_shader(GLuint type, string filename) {

  // aggregation
  string temp = "";
  string src = "";

  ifstream in_file;

  in_file.open(filename); 

  if (in_file.is_open()) {
    while (getline(in_file, temp))
      src += temp + "\n";
  } else cerr << "SHADER init failed" << endl;

  in_file.close();

  GLuint shader = glCreateShader(type);
  const GLchar *ssrc = src.c_str();
  glShaderSource(shader, 1, &ssrc, NULL);
  glCompileShader(shader);

  // error checking
  char infoLog[512];
  GLint success;

  glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
  if (!success) {
    glGetShaderInfoLog(shader, 512, NULL, infoLog);
    cerr << "SHADER compile failed" << endl << infoLog << endl;
    throw ERR_S_COMPILE;
  }

  return shader;
}

GLuint create_program(GLuint vshader, GLuint fshader) {


  GLuint program = glCreateProgram();
  glAttachShader(program, vshader);
  glAttachShader(program, fshader);

  glLinkProgram(program);

  // error checking
  char infoLog[512];
  GLint success;

  glGetProgramiv(program, GL_LINK_STATUS, &success);

  if (!success) {
    glGetProgramInfoLog(program, 512, NULL, infoLog);
    cerr << "PROGRAM linking failed" << endl << infoLog << endl;
    throw ERR_P_LINKING;
  }

  return program;
}

void init_opengl_options() {
  glEnable(GL_DEPTH_TEST);
  glEnable(GL_CULL_FACE);

  glCullFace(GL_BACK);
  glFrontFace(GL_CCW);

  glEnable(GL_BLEND);
  glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

  glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
}

void init_shader(GLuint &program) {
  try {
    // vertex
    GLuint vshader = create_shader(GL_VERTEX_SHADER, "core/v.glsl");

    // fragment
    GLuint fshader = create_shader(GL_FRAGMENT_SHADER, "core/f.glsl");

    // program
    program = create_program(vshader, fshader);

    // cleanup
    glUseProgram(0);
    glDeleteShader(vshader);
    glDeleteShader(fshader);
  } catch (int status) { glfwTerminate(); }
}