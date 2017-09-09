#pragma once
#ifndef O_SPLASH_SCENE_HPP_
#define O_SPLASH_SCENE_HPP_

#include "scene.hpp"

namespace o {

//////////////////////////////////////////////////////////////////////////////
class SplashScene final : public Scene, Immovable {
   
   enum class state {
      fade_in,
      wait,
      fade_out
   };

public:
   SplashScene();

   virtual const char* name() const override {
      return "SplashScene";
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
   F32 curtain_speed_;

   static constexpr F32 initial_curtain_speed_ = 1.f;
   static constexpr F32 fast_curtain_speed_ = 2.f;
   static constexpr F64 wait_duration_ = 1.5;
};

} // o

#endif