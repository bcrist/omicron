#pragma once
#ifndef O_SCENE_HPP_
#define O_SCENE_HPP_

#include "omicron.hpp"
#include <be/core/glm.hpp>
#include <glm/vec2.hpp>

namespace o {

class Scene {
public:
   virtual ~Scene() = default;

   virtual void key_down(I16 key) = 0;
   virtual void key_up(I16 key) = 0;

   virtual void mouse_down(I8 btn) = 0;
   virtual void mouse_up(I8 btn) = 0;
   virtual void mouse_move(vec2 norm_pos) = 0;
   virtual void mouse_wheel(I32 delta) = 0;

   virtual void update(F64 dt) = 0;
   virtual void render(F64 dt, F64 f) = 0;

};

} // o

#endif
