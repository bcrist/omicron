#include <be/core/lifecycle.hpp>
#include <be/platform/lifecycle.hpp>
#include <GL/glew.h>
#include <be/platform/glfw.hpp>

using namespace be;

int main(int argc, char** argv) {
   CoreInitLifecycle cilc;
   CoreLifecycle clc;
   PlatformLifecycle plc;

   GLFWwindow* wnd = glfwCreateWindow(800, 600, "Omicron", nullptr, nullptr);
   glewInit();
   glfwShowWindow(wnd);

   while (!glfwWindowShouldClose(wnd)) {

      glfwPollEvents();


      glfwSwapBuffers(wnd);
   }

   return 0;
}
