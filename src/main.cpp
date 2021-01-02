#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <string>

#include "graphics.h"

void
error_dialog(std::string error_message);

#ifdef _WIN32
#include <Windows.h>
#include <Winuser.h>

void
error_dialog(std::string error_message)
{
  MessageBox(nullptr, error_message.c_str(), "Error", MB_OK);
}
#endif

#ifdef __APPLE__
void
error_dialog(std::string error_message)
{

}
#endif

int
main(int argc, const char **argv)
{
  GLFWwindow *window;

  if (glfwInit() != GLFW_TRUE)
  {
    error_dialog("Couldn't initialize GLFW.");
    return 0;
  }

#ifdef __APPLE__
  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
  glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
  glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif

  window = glfwCreateWindow(1280, 720, "fp2d", nullptr, nullptr);
  if (window == nullptr)
  {
    error_dialog("Couldn't create window.");
    glfwTerminate();
    return 0;
  }

  glfwMakeContextCurrent(window);

  if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
  {
    error_dialog("Couldn't initialize OpenGL.");
    glfwTerminate();
    return 0;
  }

  GraphicsServer *renderer = new GraphicsServer();

  while (!glfwWindowShouldClose(window))
  {
    glfwPollEvents();

    int display_w, display_h;
    glfwGetFramebufferSize(window, &display_w, &display_h);
    glViewport(0, 0, display_w, display_h);
    glClearColor(0, 0, 0, 1);
    glClear(GL_COLOR_BUFFER_BIT);

    renderer->draw();

    glfwSwapBuffers(window);
  }

  delete renderer;

  glfwTerminate();

  return 0;
}
