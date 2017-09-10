#include "texture_manager.hpp"
#include <be/core/buf.hpp>

namespace o {

//////////////////////////////////////////////////////////////////////////////
void TextureManager::clear() {
   init_default_();
   textures_.clear();
}

//////////////////////////////////////////////////////////////////////////////
void TextureManager::add(Id id, Texture texture) {
   textures_[id] = std::move(texture);
}

//////////////////////////////////////////////////////////////////////////////
void TextureManager::init_default_() {
   if (!default_texture_) {
      default_texture_ = Texture(tmp_buf("\x33\x99\x99\x33"), ivec2(2), 1);
   }
}

//////////////////////////////////////////////////////////////////////////////
Texture& TextureManager::get(Id id) {
   auto it = textures_.find(id);
   if (it != textures_.end()) {
      return it->second;
   } else {
      init_default_();
      return default_texture_;
   }
}

} // o
