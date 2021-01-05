#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <string>

#include "graphics.h"
#include "input.h"
#include "state.h"
#include "screens/title_screen.h"

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

  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
  glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
  glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);

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

  GraphicsServer *renderer = new GraphicsServer(window);
  GameState *state = new GameState(renderer);
  InputMonitor *input = new InputMonitor(window, state);

  while (state->game_open())
    renderer->draw();

  delete renderer;
  delete state;
  delete input;

  glfwTerminate();

  return 0;
}
