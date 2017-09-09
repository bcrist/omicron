#include "texture.hpp"
#include <cassert>

namespace o {

GLuint GlTexture::bound_id_ = 0;
bool GlTexture::tex_enabled_ = false;

//////////////////////////////////////////////////////////////////////////////
GlTexture::GlTexture() noexcept
   : id_(0) { }

//////////////////////////////////////////////////////////////////////////////
GlTexture::GlTexture(GlTexture&& other) noexcept
   : id_(other.id_) {
   other.id_ = 0;
}

//////////////////////////////////////////////////////////////////////////////
GlTexture& GlTexture::operator=(GlTexture&& other) noexcept {
   deinit();
   id_ = other.id_;
   other.id_ = 0;
   return *this;
}

//////////////////////////////////////////////////////////////////////////////
GlTexture::~GlTexture() noexcept {
   deinit();
}

//////////////////////////////////////////////////////////////////////////////
void GlTexture::init() noexcept {
   if (id_ == 0) {
      glGenTextures(1, &id_);
   }
}

//////////////////////////////////////////////////////////////////////////////
void GlTexture::deinit() noexcept {
   if (id_ != 0) {
      if (bound_id_ == id_) {
         unbind();
      }
      glDeleteTextures(1, &id_);
      id_ = 0;
   }
}

//////////////////////////////////////////////////////////////////////////////
void GlTexture::bind() const noexcept {
   if (bound_id_ != id_) {
      glBindTexture(GL_TEXTURE_2D, id_);
      bound_id_ = id_;
   }
}

//////////////////////////////////////////////////////////////////////////////
void GlTexture::unbind() noexcept {
   if (bound_id_ != 0) {
      glBindTexture(GL_TEXTURE_2D, 0);
      disable();
   }
}

//////////////////////////////////////////////////////////////////////////////
void GlTexture::enable() const noexcept {
   bind();
   if (!tex_enabled_) {
      glEnable(GL_TEXTURE);
      tex_enabled_ = true;
   }
}

//////////////////////////////////////////////////////////////////////////////
void GlTexture::disable() noexcept {
   if (tex_enabled_) {
      glDisable(GL_TEXTURE);
   }
}

//////////////////////////////////////////////////////////////////////////////
void GlTexture::upload(const Buf<const UC>& data, ivec2 dim, int comps) const noexcept {
   assert(id_ != 0);
   assert(data.size() >= dim.x * dim.y * comps);

   glBindTexture(GL_TEXTURE_2D, id_);
   glPixelStorei(GL_UNPACK_ALIGNMENT, 1);

   GLenum internal_format = 0;
   GLenum format = 0;
   switch (comps) {
      case 1:
         internal_format = GL_LUMINANCE8;
         format = GL_LUMINANCE;
         break;
      case 2:
         internal_format = GL_LUMINANCE8_ALPHA8;
         format = GL_LUMINANCE_ALPHA;
         break;
      case 3:
         internal_format = GL_RGB8;
         format = GL_RGB;
         break;
      case 4:
         internal_format = GL_RGBA8;
         format = GL_RGBA;
         break;
      default:
         assert(false);
   }

   glTexImage2D(GL_TEXTURE_2D, 0, internal_format, dim.x, dim.y, 0, format, GL_UNSIGNED_BYTE, data.get());
   glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
   glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
   glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
   glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
   glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
   glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
}

//////////////////////////////////////////////////////////////////////////////
Texture::Texture(const Buf<const UC>& data, ivec2 dim, int comps) noexcept
   : dim_(dim) {
   gl_.init();
   gl_.upload(data, dim_, comps);
}

} // o
