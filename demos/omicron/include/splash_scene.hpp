#pragma once
#ifndef O_SPLASH_SCENE_HPP_
#define O_SPLASH_SCENE_HPP_

#include "scene.hpp"
#include "mesh_manager.hpp"

namespace o {

//////////////////////////////////////////////////////////////////////////////
class SplashScene final : public Scene, Immovable {
   
   enum class state {
      fade_in_logo,
      wait_logo,
      fade_out_logo,
      fade_in_title,
      wait_title,
      fade_out_title
   };

public:
   SplashScene();

   virtual const char* name() const override {
      return "SplashScene";
   }

   virtual void init() override;

   virtual void key_down(I16 key) override { }
   virtual void key_up(I16 key) override;

   virtual void mouse_down(I8 btn) override { }
   virtual void mouse_up(I8 btn) override;
   virtual void mouse_move(vec2 pos) override { }
   virtual void mouse_wheel(I32 delta) override { }

   virtual void update(F64 dt) override;

   virtual void render(F64 dt, F64 f) override;

private:
   //state get_next_state_(state s) const;
   void change_state_(state s);

   vec2 window_dim_;

   state state_;
   F64 state_time_;

   MeshManager mm_;

   static constexpr F32 initial_curtain_speed_ = 1.0f;
   static constexpr F32 fast_curtain_speed_ = 2.0f;
   static constexpr F64 wait_duration_logo_ = 1.5;
   static constexpr F64 wait_duration_title_ = 2.5;
   F32 last_curtain_opacity_;
   F32 curtain_opacity_;
   F32 curtain_speed_;

   Mesh logos_;

   mat4 title_transform_;
   Mesh title_;
   
};

} // o

#endif