#include "player.hpp"
#include "texture_manager.hpp"
#include "mesh_helpers.hpp"
#include <be/core/service_helpers.hpp>
#include <sstream>
#include <glm/gtc/matrix_transform.hpp>
#include <be/core/logging.hpp>

namespace o {

//////////////////////////////////////////////////////////////////////////////
Player::Player(MeshManager& mm, vec2 screen_size)
   : hp_(24),
     screen_size_(screen_size) {
   auto& tm = service<TextureManager>();
   auto& tiles = tm.get(Id("tiles"));
   auto& font = tm.get(Id("petme128_0"));
   tiles_ = &tiles;
   font_ = &font;

   avatar_mesh_ = mm.obtain(1);
   avatar_mesh_.texture_glid(tiles.glid());
   avatar_mesh_.size(4);
   avatar_mesh_.enabled(true);
   avatar_mesh_.layer(1);
   avatar_mesh_.transform(&avatar_transform_);

   hp_mesh_ = mm.obtain(20);
   hp_mesh_.texture_glid(font.glid());
   hp_mesh_.enabled(true);
   hp_mesh_.layer(99);
   
   facing_left_ = false;
   sprite_index_ = 1;
   attack_time_remaining_ = 0;
   walk_cycle_time_ = 0;
   blocking_ = false;

   build_hp_mesh_();
   update_avatar_mesh_();
}

//////////////////////////////////////////////////////////////////////////////
void Player::update(F64 dt) {
   bool dirty = false;

   if (attack_time_remaining_ > 0 || blocking_) {
      if (sprite_index_ != 2) {
         sprite_index_ = 2;
         dirty = true;
      }
      attack_time_remaining_ -= (F32)dt;
   } else if (blocking_) {
      if (sprite_index_ != 2) {
         sprite_index_ = 2;
         dirty = true;
      }
   } else if (movement_intention_ != vec2()) {
      if (movement_intention_.x < 0) {
         if (!facing_left_) {
            facing_left_ = true;
            dirty = true;
         }
      } else {
         if (facing_left_) {
            facing_left_ = false;
            dirty = true;
         }
      }

      if (sprite_index_ == 2) {
         sprite_index_ = 0;
         dirty = true;
      }

      walk_cycle_time_ -= (F32) dt;

      if (walk_cycle_time_ <= 0) {
         walk_cycle_time_ = walk_time_;
         if (sprite_index_ == 1) {
            sprite_index_ = 0;
            dirty = true;
         } else {
            sprite_index_ = 1;
            dirty = true;
         }
      }

      position_ += move_speed_ * glm::normalize(movement_intention_) * (F32)dt;
      avatar_transform_ = glm::translate(mat4(), vec3(position_, 0));
   } else if (sprite_index_ != 0) {
      sprite_index_ = 0;
      dirty = true;
   }

   if (dirty) {
      be_info() << "Player Mesh Change"
         & attr("movement_intention") << movement_intention_.x << ", " << movement_intention_.y
         & attr("position") << position_.x << ", " << position_.y
         & attr("blocking") << (blocking_ ? "yes" : "no")
         & attr("attack_time") << attack_time_remaining_
         & attr("walk_cycle_time_") << walk_cycle_time_
         & attr("sprite_index_") << sprite_index_
         | default_log();

      update_avatar_mesh_();
   }
}

//////////////////////////////////////////////////////////////////////////////
void Player::build_hp_mesh_() {
   hp_mesh_.size(0);

   std::ostringstream oss;
   oss << hp_;
   S hp_str = oss.str();

   text_mesh_append(hp_mesh_, *font_, "\xff", RGBA(255, 0, 0, 255), screen_size_ * -0.0f, 1/16.f);
   text_mesh_append(hp_mesh_, *font_, hp_str, RGBA(255), screen_size_ * -0.0f + vec2(1, 0));
}

//////////////////////////////////////////////////////////////////////////////
void Player::update_avatar_mesh_() {
   TextureRegion region;
   if (facing_left_) {
      switch (sprite_index_) {
         case 0: region = tiles_->region(Id("player1-l")); break;
         case 1: region = tiles_->region(Id("player2-l")); break;
         case 2: region = tiles_->region(Id("player3-l")); break;
      }
   } else {
      switch (sprite_index_) {
         case 0: region = tiles_->region(Id("player1-r")); break;
         case 1: region = tiles_->region(Id("player2-r")); break;
         case 2: region = tiles_->region(Id("player3-r")); break;
      }
   }
   update_verts(avatar_mesh_.verts(), region, vec2(), 1/16.f);


}

} // o
