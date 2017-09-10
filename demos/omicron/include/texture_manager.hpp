#pragma once
#ifndef O_TEXTURE_MANAGER_HPP_
#define O_TEXTURE_MANAGER_HPP_

#include "texture.hpp"
#include <be/core/id.hpp>
#include <unordered_map>
#include <be/core/service.hpp>

namespace o {

class TextureManager;

} // o
namespace be {

///////////////////////////////////////////////////////////////////////////////
template <>
struct SuppressUndefinedService<o::TextureManager> : True { };

} // be
namespace o {

//////////////////////////////////////////////////////////////////////////////
class TextureManager final {
public:
   void clear();
   void add(Id id, Texture texture);
   Texture& get(Id id);

private:
   void init_default_();

   std::unordered_map<Id, Texture> textures_;
   Texture default_texture_;
};

} // o

#endif
