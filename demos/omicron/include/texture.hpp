#pragma once
#ifndef O_TEXTURE_HPP_
#define O_TEXTURE_HPP_

#include <GL/glew.h>
#include <be/core/be.hpp>
#include <be/core/buf.hpp>
#include <be/core/glm.hpp>
#include <glm/glm.hpp>

namespace o {

class GlTexture final : be::Movable {
public:
   GlTexture() noexcept;
   GlTexture(GlTexture&& other) noexcept;
   GlTexture& operator=(GlTexture&& other) noexcept;
   ~GlTexture() noexcept;

   void init() noexcept;
   void deinit() noexcept;

   void bind() noexcept;
   static void unbind() noexcept;

   void enable() noexcept;
   static void disable() noexcept;

   void upload(be::Buf<be::UC> data, be::ivec2 dim, int comps);

private:
   GLuint id_;
   static GLuint bound_id_;
   static bool tex_enabled_;
};



class Texture final {
public:

   Texture(be::Buf<be::UC> data, be::ivec2 dim, int comps)

   //TextureRegion region() const;

   //TextureRegion region(be::Id id) const;



private:
   GlTexture gl_;
   be::ivec2 dim_;
   // std::unordered_map<be::Id, TextureRegion> regions_;

};

} // o

#endif
