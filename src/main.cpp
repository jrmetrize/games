#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <string>

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

int
main(int argc, const char **argv)
{
  GLFWwindow *window;

  glfwInit();
  window = glfwCreateWindow(1280, 720, "fp2d", NULL, NULL);
  glfwMakeContextCurrent(window);
  gladLoadGL();

  while (!glfwWindowShouldClose(window))
  {
    glClearColor(1, 0, 1, 1);
    glClear(GL_COLOR_BUFFER_BIT);
    glfwSwapBuffers(window);
    glfwPollEvents();
  }

  glfwTerminate();
  return 0;
}
