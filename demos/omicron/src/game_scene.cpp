#include "game_scene.hpp"
#include "game.hpp"
#include <be/core/service_helpers.hpp>
#include <GL/glew.h>
#include <be/platform/glfw.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "texture_manager.hpp"
#include <be/core/service_helpers.hpp>

namespace o {

//////////////////////////////////////////////////////////////////////////////
GameScene::GameScene()
   : curtain_opacity_(1.f) {
   change_state_(state::fade_in);
}

//////////////////////////////////////////////////////////////////////////////
void GameScene::key_up(I16 key) {
   if (state_ != state::fade_out_dead) {
      change_state_(state::fade_out_dead);
   }
}

//////////////////////////////////////////////////////////////////////////////
void GameScene::mouse_up(I8 btn) {
   if (state_ != state::fade_out_dead) {
      change_state_(state::fade_out_dead);
   }
}

//////////////////////////////////////////////////////////////////////////////
void GameScene::init() {
}

//////////////////////////////////////////////////////////////////////////////
void GameScene::update(F64 dt) {
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
            change_state_(state::fade_out_dead);
         }
         break;

      case state::fade_out_dead:
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
void GameScene::render(F64 dt, F64 f) {

   glMatrixMode(GL_PROJECTION);
   glm::mat4 proj = glm::ortho(0, 1, 0, 1);
   glLoadMatrixf(glm::value_ptr(proj));

   glMatrixMode(GL_MODELVIEW);
   glLoadIdentity();

   const Texture& tex = service<TextureManager>().get(Id("splash"));
   tex.gl().enable();

   glBegin(GL_QUADS);

   glColor4f(1, 1, 1, 1);
   glTexCoord2f(0, 0); glVertex2f(0, 1);
   glTexCoord2f(1, 0); glVertex2f(1, 1);
   glTexCoord2f(1, 1); glVertex2f(1, 0);
   glTexCoord2f(0, 1); glVertex2f(0, 0);

   glEnd();


   GlTexture::disable();

   glMatrixMode(GL_MODELVIEW);
   glLoadIdentity();

   if (curtain_opacity_ > 0.f) {
      glBegin(GL_QUADS);
      glColor4f(0, 0, 0, last_curtain_opacity_ + ( F32 ) (f * (curtain_opacity_ - last_curtain_opacity_)));
      glVertex2f(-1, -1);
      glVertex2f(1, -1);
      glVertex2f(1, 1);
      glVertex2f(-1, 1);
      glEnd();
   }
}

//////////////////////////////////////////////////////////////////////////////
void GameScene::change_state_(state s) {
   state_ = s;
   state_time_ = 0;
}

} // o
