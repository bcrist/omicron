#include "mesh_helpers.hpp"
#include "mesh_manager.hpp"
#include <cassert>

namespace o {

//////////////////////////////////////////////////////////////////////////////
void update_verts(Buf<Vertex> verts, const TextureRegion& region, RGBA color, F32 depth) {
   Vertex* v = verts.get();
   assert(verts.size() >= 4);

   vec2 down = unit_axis(region.down);
   vec2 right = unit_axis(region.right);
   vec2 center = region.tex_bounds.center();
   vec2 dim = region.tex_bounds.dim * 0.5f;

   v[0].color = color;
   v[0].tc = center + down * dim.y + right * -dim.x;
   v[0].pos = vec3(region.display_bounds.top_left(), depth);

   v[1].color = color;
   v[1].tc = center + down * -dim.y + right * -dim.x;
   v[1].pos = vec3(region.display_bounds.bottom_left(), depth);

   v[2].color = color;
   v[2].tc = center + down * -dim.y + right * dim.x;
   v[2].pos = vec3(region.display_bounds.bottom_right(), depth);

   v[3].color = color;
   v[3].tc = center + down * dim.y + right * dim.x;
   v[3].pos = vec3(region.display_bounds.top_right(), depth);
}

//////////////////////////////////////////////////////////////////////////////
void update_verts(Buf<Vertex> verts, const TextureRegion& region, vec2 baked_offset, F32 baked_scale, RGBA color, F32 depth) {
   update_verts(std::move(verts), region, vec3(baked_offset, depth), baked_scale, color);
}

//////////////////////////////////////////////////////////////////////////////
void update_verts(Buf<Vertex> verts, const TextureRegion& region, vec2 baked_offset, vec2 baked_scale, RGBA color, F32 depth) {
   update_verts(std::move(verts), region, vec3(baked_offset, depth), baked_scale, color);
}

//////////////////////////////////////////////////////////////////////////////
void update_verts(Buf<Vertex> verts, const TextureRegion& region, vec3 baked_offset, F32 baked_scale, RGBA color) {
   Vertex* v = verts.get();
   assert(verts.size() >= 4);

   vec2 down = unit_axis(region.down);
   vec2 right = unit_axis(region.right);
   vec2 center = region.tex_bounds.center();
   vec2 dim = region.tex_bounds.dim * 0.5f;

   v[0].color = color;
   v[0].tc = center + down * dim.y + right * -dim.x;
   v[0].pos = baked_offset + vec3(baked_scale * region.display_bounds.top_left(), 0);

   v[1].color = color;
   v[1].tc = center + down * -dim.y + right * -dim.x;
   v[1].pos = baked_offset + vec3(baked_scale * region.display_bounds.bottom_left(), 0);

   v[2].color = color;
   v[2].tc = center + down * -dim.y + right * dim.x;
   v[2].pos = baked_offset + vec3(baked_scale * region.display_bounds.bottom_right(), 0);

   v[3].color = color;
   v[3].tc = center + down * dim.y + right * dim.x;
   v[3].pos = baked_offset + vec3(baked_scale * region.display_bounds.top_right(), 0);
}

//////////////////////////////////////////////////////////////////////////////
void update_verts(Buf<Vertex> verts, const TextureRegion& region, vec3 baked_offset, vec2 baked_scale, RGBA color) {
   Vertex* v = verts.get();
   assert(verts.size() >= 4);

   vec2 down = unit_axis(region.down);
   vec2 right = unit_axis(region.right);
   vec2 center = region.tex_bounds.center();
   vec2 dim = region.tex_bounds.dim * 0.5f;

   v[0].color = color;
   v[0].tc = center + down * dim.y + right * -dim.x;
   v[0].pos = baked_offset + vec3(baked_scale * region.display_bounds.top_left(), 0);

   v[1].color = color;
   v[1].tc = center + down * -dim.y + right * -dim.x;
   v[1].pos = baked_offset + vec3(baked_scale * region.display_bounds.bottom_left(), 0);

   v[2].color = color;
   v[2].tc = center + down * -dim.y + right * dim.x;
   v[2].pos = baked_offset + vec3(baked_scale * region.display_bounds.bottom_right(), 0);

   v[3].color = color;
   v[3].tc = center + down * dim.y + right * dim.x;
   v[3].pos = baked_offset + vec3(baked_scale * region.display_bounds.top_right(), 0);
}

//////////////////////////////////////////////////////////////////////////////
Mesh text_mesh(MeshManager& mm, const Texture& tex, const S& text, RGBA color, vec2 offset, F32 scale, F32 depth) {
   Mesh mesh = mm.obtain(( U32 ) text.size());
   mesh.texture_glid(tex.glid());
   text_mesh_append(mesh, tex, text, color, offset, scale, depth);
   return mesh;
}

//////////////////////////////////////////////////////////////////////////////
Mesh text_mesh(MeshManager& mm, const Texture& tex, const S& text, RGBA color, vec2 offset, vec2 scale, F32 depth) {
   Mesh mesh = mm.obtain(( U32 ) text.size());

   text_mesh_append(mesh, tex, text, color, offset, scale, depth);
   return mesh;
}

//////////////////////////////////////////////////////////////////////////////
std::pair<rect, rect> text_mesh_append(Mesh& mesh, const Texture& tex, const S& text, RGBA color, vec2 offset, F32 scale, F32 depth) {
   Buf<Vertex> verts = mesh.verts();
   assert(mesh.texture_glid() == 0 || mesh.texture_glid() == tex.glid());
   assert(verts.size() >= mesh.size() + 4 * text.size());
   mesh.texture_glid(tex.glid());

   rect em_box_bounds;
   rect glyph_bounds;

   em_box_bounds.offset = offset;

   if (!text.empty()) {
      std::size_t vi = 4 * mesh.size();
      vec3 baked_offset = vec3(offset, depth);
      for (auto c : text) {
         TextureRegion region = tex.region(Id(c));
         update_verts(sub_buf(verts, vi), region, baked_offset, scale, color, depth);
         baked_offset += vec3(scale * region.advance, 0);

         if (c == '\n') {
            baked_offset.x = offset.x;
         }
         vi += 4;

         em_box_bounds = em_box_bounds.union_bounds(baked_offset);
      }

      Vertex* v = verts.get() + mesh.size();
      glyph_bounds = glyph_bounds.union_bounds(v->pos);
      ++v;
      for (std::size_t i = 1, end = 4 * text.size(); i < end; ++i) {
         glyph_bounds = glyph_bounds.union_bounds(v->pos);
         ++v;
      }
   }

   mesh.size(mesh.size() + 4 * ( U32 ) text.size());
   return std::make_pair(em_box_bounds, glyph_bounds);
}

//////////////////////////////////////////////////////////////////////////////
std::pair<rect, rect> text_mesh_append(Mesh& mesh, const Texture& tex, const S& text, RGBA color, vec2 offset, vec2 scale, F32 depth) {
   Buf<Vertex> verts = mesh.verts();
   assert(mesh.texture_glid() == 0 || mesh.texture_glid() == tex.glid());
   assert(verts.size() >= mesh.size() + 4 * text.size());
   mesh.texture_glid(tex.glid());

   rect em_box_bounds;
   rect glyph_bounds;

   em_box_bounds.offset = offset;

   if (!text.empty()) {
      std::size_t vi = 4 * mesh.size();
      vec3 baked_offset = vec3(offset, depth);
      for (auto c : text) {
         TextureRegion region = tex.region(Id(c));
         update_verts(sub_buf(verts, vi), region, baked_offset, scale, color, depth);
         baked_offset += vec3(scale * region.advance, 0);

         if (c == '\n') {
            baked_offset.x = offset.x;
         }
         vi += 4;

         em_box_bounds = em_box_bounds.union_bounds(baked_offset);
      }

      Vertex* v = verts.get() + mesh.size();
      glyph_bounds = glyph_bounds.union_bounds(v->pos);
      ++v;
      for (std::size_t i = 1, end = 4 * text.size(); i < end; ++i) {
         glyph_bounds = glyph_bounds.union_bounds(v->pos);
         ++v;
      }
   }

   mesh.size(mesh.size() + 4 * ( U32 ) text.size());
   return std::make_pair(em_box_bounds, glyph_bounds);
}

} // o
