#pragma once
#ifndef O_GAME_SCENE_HPP_
#define O_GAME_SCENE_HPP_

#include "scene.hpp"
#include "mesh_manager.hpp"
#include "environment.hpp"

namespace o {

//////////////////////////////////////////////////////////////////////////////
class GameScene final : public Scene, Immovable {
   
   enum class state {
      fade_in,
      play,
      fade_out
   };

public:
   GameScene();

   virtual const char* name() const override {
      return "GameScene";
   }

   virtual void init() override;

   virtual void key_down(I16 key) override;
   virtual void key_up(I16 key) override;

   virtual void mouse_down(I8 btn) override;
   virtual void mouse_up(I8 btn) override;
   virtual void mouse_move(vec2 pos) override { }
   virtual void mouse_wheel(I32 delta) override { }

   virtual void update(F64 dt) override;

   virtual void render(F64 dt, F64 f) override;

private:
   void change_state_(state s);

   vec2 window_dim_;

   state state_;
   F64 state_time_;

   MeshManager mm_;

   static constexpr F64 curtain_speed_ = 0.75;
   F32 last_curtain_opacity_;
   F32 curtain_opacity_;

   Environment env_;
   
};

} // o

#endif