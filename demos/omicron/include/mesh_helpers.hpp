#pragma once
#ifndef O_MESH_HELPERS_HPP_
#define O_MESH_HELPERS_HPP_

#include "mesh.hpp"
#include "texture.hpp"
#include "texture_region.hpp"

namespace o {

//////////////////////////////////////////////////////////////////////////////
void update_verts(Buf<Vertex> verts, const TextureRegion& region, RGBA color = RGBA(255), F32 depth = 0);
void update_verts(Buf<Vertex> verts, const TextureRegion& region, vec2 baked_offset, F32 baked_scale = 1.f, RGBA color = RGBA(255), F32 depth = 0);
void update_verts(Buf<Vertex> verts, const TextureRegion& region, vec2 baked_offset, vec2 baked_scale, RGBA color = RGBA(255), F32 depth = 0);
void update_verts(Buf<Vertex> verts, const TextureRegion& region, vec3 baked_offset, F32 baked_scale = 1.f, RGBA color = RGBA(255));
void update_verts(Buf<Vertex> verts, const TextureRegion& region, vec3 baked_offset, vec2 baked_scale, RGBA color = RGBA(255));


//////////////////////////////////////////////////////////////////////////////
Mesh text_mesh(MeshManager& mm, const Texture& tex, const S& text, RGBA color = RGBA(255), vec2 offset = vec2(), F32 scale = 1.f, F32 depth = 0);
Mesh text_mesh(MeshManager& mm, const Texture& tex, const S& text, RGBA color, vec2 offset, vec2 scale, F32 depth = 0);
std::pair<rect, rect> text_mesh_append(Mesh& mesh, const Texture& tex, const S& text, RGBA color = RGBA(255), vec2 offset = vec2(), F32 scale = 1.f, F32 depth = 0);
std::pair<rect, rect> text_mesh_append(Mesh& mesh, const Texture& tex, const S& text, RGBA color, vec2 offset, vec2 scale, F32 depth = 0);

} // o

#endif
