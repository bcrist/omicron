#include "game.hpp"
#include "texture_manager.hpp"

#include <be/core/lifecycle.hpp>
#include <be/platform/lifecycle.hpp>
#include <GL/glew.h>
#include <be/platform/glfw.hpp>
#include <be/core/glm.hpp>
#include <be/util/path_glob.hpp>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#ifdef BE_DEBUG
#pragma comment(lib, "glew-debug")
#pragma comment(lib, "core-debug")
#pragma comment(lib, "core-id-with-names-debug")
#else
#pragma comment(lib, "glew")
#pragma comment(lib, "core")
#pragma comment(lib, "core-id-with-names")
#endif

using namespace be;

void keyboard_handler(GLFWwindow* wnd, int key, int scancode, int action, int modifiers);
void mouse_handler(GLFWwindow* wnd, double x, double y);
void mouse_wheel_handler(GLFWwindow* wnd, double x, double y);
void mouse_btn_handler(GLFWwindow* wnd, int button, int action, int modifiers);

vec2 mouse_pos;

//void hover_handler(GLFWwindow* wnd, int state);
//void focus_handler(GLFWwindow* wnd, int state);
//void minimize_handler(GLFWwindow* wnd, int state);
//void maximize_handler(GLFWwindow* wnd, int state);

int main(int argc, char** argv) {
   CoreInitLifecycle cilc;
   CoreLifecycle clc;
   PlatformLifecycle plc;

   GLFWmonitor* monitor = glfwGetPrimaryMonitor();
   const GLFWvidmode* monitor_mode = glfwGetVideoMode(monitor);

   ivec2 monitor_pos;
   glfwGetMonitorPos(monitor, &monitor_pos.x, &monitor_pos.y);

   glfwDefaultWindowHints();
   glfwWindowHint(GLFW_VISIBLE, GLFW_FALSE);
   glfwWindowHint(GLFW_DECORATED, GLFW_FALSE);
   glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE);
   glfwWindowHint(GLFW_SRGB_CAPABLE, GLFW_TRUE);
   glfwWindowHint(GLFW_RED_BITS, monitor_mode->redBits);
   glfwWindowHint(GLFW_GREEN_BITS, monitor_mode->greenBits);
   glfwWindowHint(GLFW_BLUE_BITS, monitor_mode->blueBits);
   glfwWindowHint(GLFW_ALPHA_BITS, 0);
   glfwWindowHint(GLFW_SAMPLES, 4);

   glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 1);
   glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 0);
   glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_ANY_PROFILE);

#ifdef BE_DEBUG
   glfwWindowHint(GLFW_OPENGL_DEBUG_CONTEXT, GLFW_TRUE);
#else
   glfwWindowHint(GLFW_OPENGL_DEBUG_CONTEXT, GLFW_FALSE);
#endif
   
   GLFWwindow* wnd = glfwCreateWindow(monitor_mode->width, monitor_mode->height, "Omicron", nullptr, nullptr);
   if (!wnd) {
      throw std::runtime_error("Failed to create window!");
   }

   glfwSetWindowPos(wnd, monitor_pos.x, monitor_pos.y);
   glfwMakeContextCurrent(wnd);
   glfwSwapInterval(0);
   glewInit();
   
   TextureManager tm;

   be::Path asset_root = be::util::default_special_path(be::util::SpecialPath::install) / "assets";

   std::vector<be::Path> atlases = be::util::glob("*.atlas", asset_root, be::util::PathMatchType::files);







   glfwSetKeyCallback(wnd, keyboard_handler);
   glfwSetMouseButtonCallback(wnd, mouse_btn_handler);
   glfwSetScrollCallback(wnd, mouse_wheel_handler);
   glfwSetCursorPosCallback(wnd, mouse_handler);

   glfwShowWindow(wnd);

   vec2 wnd_size(monitor_mode->width, monitor_mode->height);

   glMatrixMode(GL_PROJECTION);
   glm::mat4 proj = glm::ortho(0, 1, 0, 1);
   glLoadMatrixf(glm::value_ptr(proj));

   glMatrixMode(GL_MODELVIEW);
   glLoadIdentity();

   while (!glfwWindowShouldClose(wnd)) {
      glfwPollEvents();
      glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);


      glBegin(GL_LINES);
      glColor3f(1, 0, 0);      glVertex2f(1, 0);
      glColor3f(0, 1, 0);      glVertex2f(0, 1);
      glColor3f(0, 0, 1);      glVertex2f(0, 0);
      glColor3f(1, 1, 1);      vec2 p = mouse_pos / wnd_size;      glVertex2f(p.x, p.y);
      glEnd();


      glfwSwapBuffers(wnd);
   }

   glfwDestroyWindow(wnd);

   return 0;
}

void keyboard_handler(GLFWwindow* wnd, int key, int scancode, int action, int modifiers) {
   if (key == GLFW_KEY_ESCAPE) {
      glfwSetWindowShouldClose(wnd, GLFW_TRUE);
   }
}

void mouse_handler(GLFWwindow* wnd, double x, double y) {
   mouse_pos.x = (F32)x;
   mouse_pos.y = (F32)y;
}

void mouse_wheel_handler(GLFWwindow* wnd, double x, double y) {

}

void mouse_btn_handler(GLFWwindow* wnd, int button, int action, int modifiers) {

}
