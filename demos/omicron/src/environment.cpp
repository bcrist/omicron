#include "environment.hpp"
#include "texture_manager.hpp"
#include "mesh_helpers.hpp"
#include <be/core/service_helpers.hpp>
#include <be/util/service_xoroshiro_128_plus.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <random>

namespace o {

//////////////////////////////////////////////////////////////////////////////
Environment::Environment(MeshManager& mm, ivec2 dim, const S& data) {
   assert(data.size() >= dim.x * dim.y);
   mesh_ = mm.obtain(dim.x * dim.y);
   Buf<Vertex> all_verts = mesh_.verts();
   
   auto& prng = service<util::xo128p>();
   auto& tm = service<TextureManager>();
   Texture& atlas = tm.get(Id("tiles"));

   mesh_.texture_glid(atlas.glid());
   mesh_.enabled(true);
   mesh_.layer(-1);
   mesh_.size(4 * dim.x * dim.y);
   mesh_.transform(&mesh_transform_);
   mesh_transform_ = glm::translate(mat4(), vec3(dim, 0) * -0.5f);

   TextureRegion grass[8];
   grass[0] = atlas.region(Id("grass0"));
   grass[1] = atlas.region(Id("grass1"));
   grass[2] = atlas.region(Id("grass2"));
   grass[3] = atlas.region(Id("grass3"));
   grass[4] = atlas.region(Id("grass4"));
   grass[5] = atlas.region(Id("grass5"));
   grass[6] = atlas.region(Id("grass6"));
   grass[7] = atlas.region(Id("grass7"));
   std::uniform_int_distribution<int> grass_dist(0, 7);

   for (std::size_t y = 0; y < dim.y; ++y) {
      for (std::size_t x = 0; x < dim.x; ++x) {
         std::size_t i = y * dim.x + x;
         Buf<Vertex> verts = sub_buf(all_verts, 4 * i);
         
         TextureRegion* region;
         switch (data[i]) {
            default:
               region = &grass[grass_dist(prng)];
         }

         update_verts(tmp_buf(verts), *region, vec2(x, y), 1 / 16.f, RGBA(255,255,255,255));
      }
   }

}

} // o
