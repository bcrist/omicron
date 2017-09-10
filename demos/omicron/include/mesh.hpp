#pragma once
#ifndef O_MESH_HPP_
#define O_MESH_HPP_

#include "omicron.hpp"
#include <be/core/glm.hpp>
#include <be/core/buf.hpp>
#include <glm/glm.hpp>
#include <GL/glew.h>

namespace o {

class MeshManager;

struct Vertex {
   RGBA color;
   glm::vec2 tc;
   glm::vec3 pos;
};

struct MeshData {
   I16 layer = 0;
   bool depth_test = false;
   bool depth_write = false;
   GLenum blend_src = GL_SRC_ALPHA;
   GLenum blend_dest = GL_ONE_MINUS_SRC_ALPHA;
   GLuint tex_id = 0;
   mat4* transform = nullptr;
   U32 size = 0;
   Buf<Vertex> verts;
   bool enabled = false;
   bool same_as_last_;
};

class Mesh final : Movable {
   friend class MeshManager;
public:
   Mesh() noexcept;
   Mesh(Mesh&& other) noexcept;
   Mesh& operator=(Mesh&& other) noexcept;
   ~Mesh();

   explicit operator bool() const {
      return nullptr != data_;
   }

   U32 size() const;
   void size(U32 size);

   bool enabled() const;
   void enabled(bool enable);

   I16 layer() const;
   void layer(I16 l);

   bool depth_test() const;
   void depth_test(bool test);

   bool depth_write() const;
   void depth_write(bool write);

   GLenum blend_func_src() const;
   GLenum blend_func_dest() const;
   void blend_func_src(GLenum f);
   void blend_func_dest(GLenum f);
   void blend_func(GLenum src, GLenum dest);

   GLuint texture_glid() const;
   void texture_glid(GLuint id);

   mat4* transform() const;
   void transform(mat4* ptr);

   Buf<Vertex> verts();
   Buf<const Vertex> verts() const;

private:
   void assign_(MeshManager* mgr, MeshData* data);
   void reset_();

   MeshManager* mgr_;
   MeshData* data_;
};

} // o

#endif
