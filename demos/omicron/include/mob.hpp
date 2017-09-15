#pragma once
#ifndef O_MOB_HPP_
#define O_MOB_HPP_

#include "mesh_manager.hpp"
#include "texture.hpp"

namespace o {

//////////////////////////////////////////////////////////////////////////////
class Mob final : Immovable { 
public:
   Mob(MeshManager& mm);

   I32 hp() const {
      return hp_;
   }

   void hp(I32 hp) {
      if (hp_ != hp) {
         hp_ = hp;
      }
   }

   bool attacking() const {
      return attack_time_remaining_ <= 0;
   }

   void attack_cooldown() {
      attack_time_remaining_ = attack_time_;
   }

   void mobs_left(U32 count, U32 total);

   void player_distance(F32 dst);

   vec2 pos() const {
      return position_;
   }

   void pos(vec2 new_pos);
   
   void signal_movement(vec2 direction) {
      movement_intention_ = direction;
   }

   void update(F64 dt);

private:
   void update_avatar_mesh_();

   static constexpr F32 attack_time_ = 0.25f;

   I32 hp_;
   vec2 position_;
   vec2 movement_intention_;
   F32 attack_time_remaining_;
   F32 move_speed_;
   
   char variant_;
   char direction_;

   Texture* tiles_;

   Mesh avatar_mesh_;
   mat4 avatar_transform_;
};

} // o

#endif
