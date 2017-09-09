#include "splash_scene.hpp"
#include "game.hpp"
#include <be/core/service_helpers.hpp>
#include <GL/glew.h>
#include <be/platform/glfw.hpp>
#include <be/core/logging.hpp>

namespace o {

//////////////////////////////////////////////////////////////////////////////
SplashScene::SplashScene()
   : curtain_opacity_(1.f),
     curtain_speed_(initial_curtain_speed_) {
   change_state_(state::fade_in);
}

//////////////////////////////////////////////////////////////////////////////
void SplashScene::key_up(I16 key) {
   if (state_ != state::fade_out) {
      change_state_(state::fade_out);
      curtain_speed_ = fast_curtain_speed_;
   }
}

//////////////////////////////////////////////////////////////////////////////
void SplashScene::mouse_up(I8 btn) {
   if (state_ != state::fade_out) {
      change_state_(state::fade_out);
      curtain_speed_ = fast_curtain_speed_;
   }
}

//////////////////////////////////////////////////////////////////////////////
void SplashScene::update(F64 dt) {
   state_time_ += dt;

   switch (state_) {

      case state::fade_in:
         last_curtain_opacity_ = curtain_opacity_;
         if (curtain_opacity_ > 0.f) {
            curtain_opacity_ -= ( F32 ) (curtain_speed_ * dt);
         }
         if (curtain_opacity_ <= 0.f) {
            change_state_(state::wait);
         }
         break;

      case state::wait:
         if (state_time_ >= wait_duration_) {
            change_state_(state::fade_out);
         }
         break;

      case state::fade_out:
         last_curtain_opacity_ = curtain_opacity_;
         if (curtain_opacity_ < 1.f) {
            curtain_opacity_ += ( F32 ) (curtain_speed_ * dt);
         }
         if (curtain_opacity_ >= 1.f) {
            service<Game>().new_game();
         }
         break;

      default:
         break;
   }
}

//////////////////////////////////////////////////////////////////////////////
void SplashScene::render(F64 dt, F64 f) {
   F32 o = 1.f - ( F32 ) (last_curtain_opacity_ + f * (curtain_opacity_ - last_curtain_opacity_));

   glBegin(GL_LINES);
   glColor4f(1, 0, 0, o);      glVertex2f(1, 0);
   glColor4f(0, 1, 0, o);      glVertex2f(0, 1);
   glColor4f(0, 0, 1, o);      glVertex2f(0, 0);
   glColor4f(1, 1, 1, o);      glVertex2f(1, 1);
   glVertex2f(0, 0.5f);
   glVertex2f(( F32 ) f, 0.5f);
   glEnd();
}

//////////////////////////////////////////////////////////////////////////////
void SplashScene::change_state_(state s) {
   state_ = s;
   state_time_ = 0;
}

} // o
