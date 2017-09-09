#pragma once
#ifndef O_TEXTURE_HPP_
#define O_TEXTURE_HPP_

#include "omicron.hpp"
#include <GL/glew.h>
#include <be/core/be.hpp>
#include <be/core/buf.hpp>
#include <be/core/glm.hpp>
#include <glm/glm.hpp>

namespace o {

//////////////////////////////////////////////////////////////////////////////
class GlTexture final : Movable {
public:
   GlTexture() noexcept;
   GlTexture(GlTexture&& other) noexcept;
   GlTexture& operator=(GlTexture&& other) noexcept;
   ~GlTexture() noexcept;

   void init() noexcept;
   void deinit() noexcept;

   explicit operator bool() const noexcept {
      return id_ != 0;
   }

   GLuint id() const noexcept {
      return id_;
   }

   void bind() const noexcept;
   static void unbind() noexcept;

   void enable() const noexcept;
   static void disable() noexcept;

   void upload(const Buf<const UC>& data, ivec2 dim, int comps) const noexcept;

private:
   GLuint id_;
   static GLuint bound_id_;
   static bool tex_enabled_;
};

//////////////////////////////////////////////////////////////////////////////
class Texture final {
public:
   Texture() noexcept { };
   Texture(const Buf<const UC>& data, ivec2 dim, int comps) noexcept;

   explicit operator bool() const noexcept {
      return !!gl_;
   }

   GLuint glid() const noexcept {
      return gl_.id();
   }

   ivec2 dim() const noexcept {
      return dim_;
   }

private:
   GlTexture gl_;
   ivec2 dim_;
};

} // o

#endif
