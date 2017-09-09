#pragma once
#ifndef O_TEXTURE_MANAGER_HPP_
#define O_TEXTURE_MANAGER_HPP_

#include "texture.hpp"
#include <be/core/id.hpp>

namespace o {

class TextureManager final {
public:

   const Texture& get(be::Id id)


private:
   std::unordered_map<be::Id, Texture> textures_;
   Texture default_texture_;
};


} // o

#endif
