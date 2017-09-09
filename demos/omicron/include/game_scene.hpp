#pragma once
#ifndef O_GAME_SCENE_HPP_
#define O_GAME_SCENE_HPP_

#include "scene.hpp"

namespace o {

//////////////////////////////////////////////////////////////////////////////
class GameScene final : public Scene, Immovable {
   
   enum class state {
      fade_in,
      wait,
      fade_out_dead
   };

public:
   GameScene();

   virtual const char* name() const override {
      return "GameScene";
   }

   virtual void key_down(I16 key) override { }
   virtual void key_up(I16 key) override;

   virtual void mouse_down(I8 btn) override { }
   virtual void mouse_up(I8 btn) override;
   virtual void mouse_move(vec2 norm_pos) override { }
   virtual void mouse_wheel(I32 delta) override { }

   virtual void update(F64 dt) override;

   virtual void render(F64 dt, F64 f) override;

private:
   void change_state_(state s);

   // SpriteManager sm;

   state state_;
   F64 state_time_;

   F32 last_curtain_opacity_;
   F32 curtain_opacity_;

   static constexpr F64 curtain_speed_ = 0.5;
   static constexpr F64 wait_duration_ = 3.0;
};

} // o

#endif