#include "mob.hpp"
#include "texture_manager.hpp"
#include "mesh_helpers.hpp"
#include <be/core/service_helpers.hpp>
#include <sstream>
#include <glm/gtc/matrix_transform.hpp>
#include <be/core/logging.hpp>
#include <be/util/service_xoroshiro_128_plus.hpp>
#include <random>

namespace o {

//////////////////////////////////////////////////////////////////////////////
Mob::Mob(MeshManager& mm) {
   auto& tm = service<TextureManager>();
   auto& tiles = tm.get(Id("tiles"));
   tiles_ = &tiles;

   avatar_mesh_ = mm.obtain(1);
   avatar_mesh_.texture_glid(tiles.glid());
   avatar_mesh_.size(4);
   avatar_mesh_.enabled(true);
   avatar_mesh_.layer(1);
   avatar_mesh_.transform(&avatar_transform_);

   direction_ = 'u';
   attack_time_remaining_ = 0;

   auto& rnd = service<util::xo128p>();
   std::uniform_int_distribution<int> dist('1', '4');

   variant_ = ( char ) dist(rnd);

   switch (variant_) {
      case '1':
         move_speed_ = 1.5f;
         hp_ = 2;
         break;
      case '2':
         move_speed_ = 3.f;
         hp_ = 1;
         break;
      case '3':
         move_speed_ = 0.25f;
         hp_ = 10;
         break;
      case '4':
         move_speed_ = 0.25f;
         hp_ = 5;
         break;
   }

   update_avatar_mesh_();
}

//////////////////////////////////////////////////////////////////////////////
void Mob::mobs_left(U32 count, U32 total) {
   if (variant_ == '4') {
      move_speed_ = 0.25f + (total - count) * 4.f / total;
   }
}

//////////////////////////////////////////////////////////////////////////////
void Mob::player_distance(F32 dst) {
   if (variant_ == '3') {
      move_speed_ = 0.25f + sqrt(dst / 2);
   }
}

//////////////////////////////////////////////////////////////////////////////
void Mob::pos(vec2 new_pos) {
   position_ = new_pos;
   avatar_transform_ = glm::translate(mat4(), vec3(position_, 0));
}

//////////////////////////////////////////////////////////////////////////////
void Mob::update(F64 dt) {
   bool dirty = false;

   if (hp_ <= 0) {
      if (avatar_mesh_) {
         avatar_mesh_ = Mesh();
      }
      return;
   }

   if (attack_time_remaining_ > 0) {
      attack_time_remaining_ -= ( F32 ) dt;
   }

   if (movement_intention_ != vec2()) {
      if (abs(movement_intention_.x) > abs(movement_intention_.y)) {
         if (movement_intention_.x < 0) {
            if (direction_ != 'l') {
               direction_ = 'l';
               dirty = true;
            }
         } else {
            if (direction_ != 'r') {
               direction_ = 'r';
               dirty = true;
            }
         }
      } else {
         if (movement_intention_.y < 0) {
            if (direction_ != 'u') {
               direction_ = 'u';
               dirty = true;
            }
         } else {
            if (direction_ != 'd') {
               direction_ = 'd';
               dirty = true;
            }
         }
      }

      position_ += move_speed_ * glm::normalize(movement_intention_) * ( F32 ) dt;
      avatar_transform_ = glm::translate(mat4(), vec3(position_, 0));
   }

   if (dirty) {
      be_verbose() << "Mob Mesh Change"
         & attr("movement_intention") << movement_intention_.x << ", " << movement_intention_.y
         & attr("position") << position_.x << ", " << position_.y
         & attr("attack_time") << attack_time_remaining_
         | default_log();

      update_avatar_mesh_();
   }
}

//////////////////////////////////////////////////////////////////////////////
void Mob::update_avatar_mesh_() {
   S name = "enemy";
   name.append(1, variant_);
   name.append(1, '-');
   name.append(1, direction_);
   TextureRegion region = tiles_->region(Id(name));

   update_verts(avatar_mesh_.verts(), region, vec2(), 1 / 16.f);
}

} // o
