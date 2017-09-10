#pragma once
#ifndef O_TEXTURE_REGION_HPP_
#define O_TEXTURE_REGION_HPP_

#include "omicron.hpp"
#include <be/core/extents.hpp>
#include <glm/vec2.hpp>

namespace o {

//////////////////////////////////////////////////////////////////////////////
enum class Axis {
   neg_y,
   pos_y,
   neg_x,
   pos_x
};

//////////////////////////////////////////////////////////////////////////////
struct TextureRegion final {
   rect tex_bounds;
   rect display_bounds;
   vec2 advance;
   Axis right = Axis::pos_x;
   Axis down = Axis::pos_y;
};

//////////////////////////////////////////////////////////////////////////////
inline vec2 unit_axis(Axis axis) {
   switch (axis) {
      case Axis::neg_y: return vec2(0, -1);
      case Axis::pos_y: return vec2(0, 1);
      case Axis::neg_x: return vec2(-1, 0);
      case Axis::pos_x: return vec2(1, 0);
      default: return vec2();
   }
}

} // o

#endif
