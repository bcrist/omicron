#include "window.hpp"

namespace o {
namespace {

//////////////////////////////////////////////////////////////////////////////
void keyboard_handler(GLFWwindow* wnd, int key, int scancode, int action, int modifiers) {
   Scene* scene = static_cast< Scene* >(glfwGetWindowUserPointer(wnd));
   if (!scene) return;
   if (action == GLFW_PRESS) {
      scene->key_down(( U16 ) key);
   } else if (action == GLFW_RELEASE) {
      scene->key_up(( U16 ) key);
   }
}

//////////////////////////////////////////////////////////////////////////////
void mouse_handler(GLFWwindow* wnd, double x, double y) {
   Scene* scene = static_cast< Scene* >(glfwGetWindowUserPointer(wnd));
   if (!scene) return;
   vec2 pos(( F32 ) x, ( F32 ) y);
   // TODO normalize with window size
   scene->mouse_move(pos);
}

//////////////////////////////////////////////////////////////////////////////
void mouse_wheel_handler(GLFWwindow* wnd, double x, double y) {
   Scene* scene = static_cast< Scene* >(glfwGetWindowUserPointer(wnd));
   if (!scene) return;
   scene->mouse_wheel(( I32 ) y);
}

//////////////////////////////////////////////////////////////////////////////
void mouse_btn_handler(GLFWwindow* wnd, int button, int action, int modifiers) {
   Scene* scene = static_cast< Scene* >(glfwGetWindowUserPointer(wnd));
   if (!scene) return;
   if (action == GLFW_PRESS) {
      scene->mouse_down(( I8 ) button);
   } else if (action == GLFW_RELEASE) {
      scene->mouse_up(( I8 ) button);
   }
}

} // o::()

//////////////////////////////////////////////////////////////////////////////
Window::Window()
   : glfw_(nullptr) { }

//////////////////////////////////////////////////////////////////////////////
Window::Window(Window&& other)
   : glfw_(other.glfw_) {
   other.glfw_ = nullptr;
}

//////////////////////////////////////////////////////////////////////////////
Window& Window::operator=(Window&& other) {
   destroy_();
   glfw_ = other.glfw_;
   other.glfw_ = nullptr;
   return *this;
}

//////////////////////////////////////////////////////////////////////////////
Window::~Window() {
   destroy_();
}

//////////////////////////////////////////////////////////////////////////////
void Window::create() {
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
   glfwWindowHint(GLFW_SAMPLES, 4); // TODO allow MSAA to be turned off?

   glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 1);
   glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 0);
   glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_ANY_PROFILE);

#ifdef BE_DEBUG
   glfwWindowHint(GLFW_OPENGL_DEBUG_CONTEXT, GLFW_TRUE);
#else
   glfwWindowHint(GLFW_OPENGL_DEBUG_CONTEXT, GLFW_FALSE);
#endif

   glfw_ = glfwCreateWindow(monitor_mode->width, monitor_mode->height, "Omicron", nullptr, nullptr);
   if (!glfw_) {
      throw std::runtime_error("Failed to create window!");
   }

   glfwSetWindowPos(glfw_, monitor_pos.x, monitor_pos.y);
   glfwMakeContextCurrent(glfw_);
   glfwSwapInterval(0);
   glewInit();

   glfwSetWindowUserPointer(glfw_, nullptr);

   glfwSetKeyCallback(glfw_, keyboard_handler);
   glfwSetMouseButtonCallback(glfw_, mouse_btn_handler);
   glfwSetScrollCallback(glfw_, mouse_wheel_handler);
   glfwSetCursorPosCallback(glfw_, mouse_handler);
}

//////////////////////////////////////////////////////////////////////////////
void Window::show() {
   glfwShowWindow(glfw_);
}

//////////////////////////////////////////////////////////////////////////////
void Window::hide() {
   glfwHideWindow(glfw_);
}

//////////////////////////////////////////////////////////////////////////////
Scene* Window::scene() const {
   return static_cast< Scene* >(glfwGetWindowUserPointer(glfw_));
}

//////////////////////////////////////////////////////////////////////////////
void Window::scene(Scene* scene) {
   glfwSetWindowUserPointer(glfw_, scene);
}

//////////////////////////////////////////////////////////////////////////////
void Window::destroy_() {
   if (glfw_) {
      glfwDestroyWindow(glfw_);
      glfw_ = nullptr;
   }
}

} // o
