#pragma once
#ifndef O_WINDOW_HPP_
#define O_WINDOW_HPP_

#include "scene.hpp"
#include <GL/glew.h>
#include <be/platform/glfw.hpp>
#include <be/core/glm.hpp>
#include <glm/vec2.hpp>
#include <be/core/service.hpp>

namespace o {

class Window;

} // o
namespace be {

///////////////////////////////////////////////////////////////////////////////
template <>
struct SuppressUndefinedService<o::Window> : True { };

} // be
namespace o {

class Window final : Movable {
public:
   Window();
   Window(Window&& other);
   Window& operator=(Window&& other);
   ~Window();

   explicit operator bool() const noexcept {
      return nullptr != glfw_;
   }

   GLFWwindow* glfw() const noexcept {
      return glfw_;
   }

   void create();
   void show();
   void hide();

   Scene* scene() const;
   void scene(Scene* scene);

private:
   void destroy_();

   GLFWwindow* glfw_;
};

} // o

#endif
