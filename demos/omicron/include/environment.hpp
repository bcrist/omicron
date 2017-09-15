#pragma once
#ifndef O_ENVIRONMENT_HPP_
#define O_ENVIRONMENT_HPP_

#include "omicron.hpp"
#include "mesh_manager.hpp"

namespace o {

//////////////////////////////////////////////////////////////////////////////
class Environment final : Immovable {
public:
   Environment(MeshManager& mm, ivec2 dim, const S& data);

private:
   Mesh mesh_;
   mat4 mesh_transform_;
};

} // o

#endif
