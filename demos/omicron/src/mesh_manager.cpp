#include "mesh_manager.hpp"
#include "texture.hpp"
#include <glm/gtc/type_ptr.hpp>
#include <algorithm>
#include <be/core/logging.hpp>

namespace o {

using namespace be;

//////////////////////////////////////////////////////////////////////////////
MeshManager::MeshManager()
   : dirty_(true) { }

//////////////////////////////////////////////////////////////////////////////
MeshManager::~MeshManager() {
   assert(meshes_.empty());
}

//////////////////////////////////////////////////////////////////////////////
Mesh MeshManager::obtain(I32 n_quads) {
   Mesh mesh;
   auto ptr = std::make_unique<MeshData>();
   MeshData* data = ptr.get();
   data->verts = make_buf<Vertex>(n_quads * 4);
   mesh.assign_(this, data);
   meshes_.push_back(std::move(ptr));
   make_dirty_();
   return mesh;
}

//////////////////////////////////////////////////////////////////////////////
void MeshManager::render() {
   make_clean_();

   GLenum blend_src = GL_SRC_ALPHA;
   GLenum blend_dest = GL_ONE_MINUS_SRC_ALPHA;
   GLenum depth_func = GL_LESS;
   bool depth_mask = true;
   bool depth_enabled = false;
   bool depth_test = false;
   bool depth_write = false;
   GLuint tex_id = 0;
   mat4* transform_ptr = nullptr;
   mat4 transform;

   glEnable(GL_BLEND);
   glBlendFunc(blend_src, blend_dest);

   glDisable(GL_DEPTH_TEST);
   glDepthFunc(depth_func);
   glDepthMask(depth_mask);

   GlTexture::disable();

   glMatrixMode(GL_MODELVIEW);
   glLoadIdentity();

   for (auto& ptr : meshes_) {
      MeshData& data = *ptr;
      if (!data.enabled) {
         continue;
      }

      if (!data.same_as_last_) {

         if (data.blend_src != blend_src || data.blend_dest != blend_dest) {
            blend_src = data.blend_src;
            blend_dest = data.blend_dest;
            glBlendFunc(blend_src, blend_dest);
         }

         if (data.depth_test != depth_test || data.depth_write != depth_write) {
            depth_test = data.depth_test;
            depth_write = data.depth_write;

            if (depth_test) {
               if (!depth_enabled) {
                  depth_enabled = true;
                  glEnable(GL_DEPTH_TEST);
               }
               if (depth_func != GL_LESS) {
                  depth_func = GL_LESS;
                  glDepthFunc(depth_func);
               }
               if (depth_write) {
                  if (!depth_mask) {
                     depth_mask = true;
                     glDepthMask(GL_TRUE);
                  }
               } else {
                  if (depth_mask) {
                     depth_mask = false;
                     glDepthMask(GL_FALSE);
                  }
               }
            } else {
               if (depth_write) {
                  if (!depth_enabled) {
                     depth_enabled = true;
                     glEnable(GL_DEPTH_TEST);
                  }
                  if (depth_func != GL_ALWAYS) {
                     depth_func = GL_ALWAYS;
                     glDepthFunc(depth_func);
                  }
                  if (!depth_mask) {
                     depth_mask = true;
                     glDepthMask(GL_TRUE);
                  }
               } else {
                  if (depth_enabled) {
                     depth_enabled = false;
                     glDisable(GL_DEPTH_TEST);
                  }
               }
            }
         }

         if (data.tex_id != tex_id) {
            tex_id = data.tex_id;
            if (tex_id == 0) {
               GlTexture::disable();
            } else {
               GlTexture::enable(tex_id);
            }
         }

         if (data.transform != transform_ptr) {
            transform_ptr = data.transform;
            if (!transform_ptr) {
               glLoadIdentity();
            } else if (transform != *transform_ptr) {
               transform = *transform_ptr;
               glLoadMatrixf(glm::value_ptr(transform));
            }
         }
      }

      Vertex* it = data.verts.get();
      assert(data.size <= data.verts.size());
      glBegin(GL_QUADS);
      if (tex_id != 0) for (U32 i = 0, n = data.size; i < n; ++i) {
         glColor4ubv(glm::value_ptr(it->color));
         glTexCoord2fv(glm::value_ptr(it->tc));
         glVertex3fv(glm::value_ptr(it->pos));

         ++it;
      } else for (U32 i = 0, n = data.size; i < n; ++i) {
         glColor4ubv(glm::value_ptr(it->color));
         glVertex3fv(glm::value_ptr(it->pos));
         ++it;
      }
      glEnd();
      if (data.debug) {
         Vertex* it2 = data.verts.get();
         for (U32 i = 0, n = data.size; i < n; ++i) {
            be_short_debug()
               << "C: " << (U32)it2->color.r << "," << (U32)it2->color.g << "," << (U32)it2->color.b << "," << (U32)it2->color.a
               << "\tT: " << it2->tc.x << "," << it2->tc.y
               << "\tP: " << it2->pos.x << "," << it2->pos.y << "," << it2->pos.z
               | default_log();
            
            ++it2;
         }
      }
   }
}

//////////////////////////////////////////////////////////////////////////////
void MeshManager::reclaim_(MeshData* data) {
   for (auto it = meshes_.begin(), end = meshes_.end(); it != end; ++it) {
      MeshData* m = it->get();
      if (m == data) {
         meshes_.erase(it);
         break;
      }
   }
}

//////////////////////////////////////////////////////////////////////////////
void MeshManager::make_dirty_() {
   dirty_ = true;
}

//////////////////////////////////////////////////////////////////////////////
void MeshManager::make_clean_() {
   if (dirty_) {
      std::sort(meshes_.begin(), meshes_.end(), [](std::unique_ptr<MeshData>& aptr, std::unique_ptr<MeshData>& bptr) {
         MeshData& a = *aptr;
         MeshData& b = *bptr;

         if (a.layer != b.layer) {
            return a.layer < b.layer;
         }

         if (a.tex_id != b.tex_id) {
            return a.tex_id < b.tex_id;
         }

         if (a.transform != b.transform) {
            return a.transform < b.transform;
         }

         if (a.depth_test != b.depth_test) {
            return b.depth_test;
         }

         if (a.depth_write != b.depth_write) {
            return b.depth_write;
         }

         if (a.blend_src != b.blend_src) {
            return a.blend_src < b.blend_src;
         }

         if (a.blend_dest != b.blend_dest) {
            return a.blend_dest < b.blend_dest;
         }

         return false;
      });

      MeshData* prev = nullptr;
      for (auto& ptr : meshes_) {
         MeshData* data = ptr.get();
         if (!data->enabled) {
            continue;
         }

         data->same_as_last_ = prev != nullptr &&
            data->transform == prev->transform &&
            data->tex_id == prev->tex_id &&
            data->layer == prev->layer &&
            data->depth_test == prev->depth_test &&
            data->depth_write == prev->depth_write &&
            data->blend_src == prev->blend_src &&
            data->blend_dest == prev->blend_dest;

         prev = data;
      }

      dirty_ = false;
   }
}

} // o
