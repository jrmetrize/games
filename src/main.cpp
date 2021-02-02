#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <string>
#include <chrono>

#include "audio.h"
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

  // TODO: we need to be much more careful about dependencies here. Ideally,
  // none of the singletons depend on each other in constructors, and
  // proper initialization with dependencies will happend after all singletons
  // have been created.
  InputMonitor *input = new InputMonitor(window);
  InputMonitor::set_instance(input);

  GameState *state = new GameState();
  GameState::set_instance(state);

  GraphicsServer *renderer = new GraphicsServer(window);
  GraphicsServer::set_instance(renderer);

  AudioServer *audio = new AudioServer();
  AudioServer::set_instance(audio);

  std::chrono::time_point<std::chrono::steady_clock> last_frame =
    std::chrono::steady_clock::now();

  while (state->game_open())
  {
    std::chrono::time_point<std::chrono::steady_clock> current_frame =
      std::chrono::steady_clock::now();
    float duration =
      float(std::chrono::duration_cast<std::chrono::microseconds>(current_frame
      - last_frame).count()) / (1000 * 1000);
    last_frame = current_frame;
    state->update(duration);
    renderer->draw();
  }

  delete audio;
  delete renderer;
  delete state;
  delete input;

  glfwTerminate();

  return 0;
}
