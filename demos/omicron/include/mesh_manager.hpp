#pragma once
#ifndef O_MESH_MANAGER_HPP_
#define O_MESH_MANAGER_HPP_

#include "omicron.hpp"
#include "mesh.hpp"

namespace o {

//////////////////////////////////////////////////////////////////////////////
class MeshManager final : Immovable {
   friend class Mesh;
public:
   MeshManager();
   ~MeshManager();

   Mesh obtain(I32 n_quads);
   
   void render();

private:
   void reclaim_(MeshData* data);
   void make_dirty_();
   void make_clean_();

   bool dirty_ = true;
   std::vector<std::unique_ptr<MeshData>> meshes_;
};

} // o

#endif
