#pragma once
#ifndef O_PLAYER_HPP_
#define O_PLAYER_HPP_

#include "mesh_manager.hpp"
#include "texture.hpp"

namespace o {

//////////////////////////////////////////////////////////////////////////////
class Player final : Immovable { 
public:
   Player(MeshManager& mm, vec2 screen_size);

   I32 hp() const {
      return hp_;
   }

   void hp(I32 hp) {
      if (hp_ != hp) {
         hp_ = hp;
         build_hp_mesh_();
      }
   }

   bool blocking() const {
      return blocking_;
   }

   void block(bool b) {
      blocking_ = b;
   }

   bool attacking() const {
      return attack_time_remaining_ > 0;
   }

   void attack() {
      if (attack_time_remaining_ <= 0) {
         attack_time_remaining_ = attack_time_;
      }
   }

   vec2 pos() const {
      return position_;
   }

   void pos(vec2 new_pos);

   bool facing_left() const {
      return facing_left_;
   }

   vec2 movement_intention() const {
      return movement_intention_;
   }

   void signal_movement(vec2 direction) {
      movement_intention_ += direction;
   }

   void update(F64 dt);

private:
   void build_hp_mesh_();
   void update_avatar_mesh_();

   static constexpr F32 attack_time_ = 0.1f;
   static constexpr F32 walk_time_ = 0.15f;
   static constexpr F32 move_speed_ = 2.5f;

   I32 hp_;
   vec2 position_;
   vec2 movement_intention_;
   F32 attack_time_remaining_;
   F32 walk_cycle_time_;
   bool blocking_;
   bool facing_left_;
   int sprite_index_;

   Texture* font_;
   Texture* tiles_;
   vec2 screen_size_;

   Mesh avatar_mesh_;
   Mesh hp_mesh_;

   mat4 avatar_transform_;
};

} // o

#endif
