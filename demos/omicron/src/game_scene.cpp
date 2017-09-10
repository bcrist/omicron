#include "game_scene.hpp"
#include "game.hpp"
#include "window.hpp"
#include "texture.hpp"
#include <be/core/service_helpers.hpp>
#include <GL/glew.h>
#include <be/platform/glfw.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

namespace o {
namespace {

constexpr const char* env_data =
"                                                  "
"                                                  "
"                                                  "
"                                                  "
"                                                  "
"                                                  "
"                                                  "
"                                                  "
"                                                  "
"                                                  "
"                                                  "
"                                                  "
"                                                  "
"                                                  "
"                                                  "
"                                                  "
"                                                  "
"                                                  "
"                                                  "
"                                                  "
"                                                  "
"                                                  "
"                                                  "
"                                                  "
"                                                  "
"                                                  "
"                                                  "
"                                                  "
"                                                  "
"                                                  "
"                                                  "
"                                                  "
"                                                  "
"                                                  "
"                                                  "
"                                                  "
"                                                  "
"                                                  "
"                                                  "
"                                                  "
;

} // o::()

//////////////////////////////////////////////////////////////////////////////
GameScene::GameScene()
   : curtain_opacity_(1.f),
     env_(mm_, ivec2(50, 40), env_data),
     player_(mm_, vec2(service<Window>().dim()) / 48.f) {
   change_state_(state::fade_in);
}

//////////////////////////////////////////////////////////////////////////////
void GameScene::key_down(I16 key) {
   switch (key) {
      case GLFW_KEY_W:
         player_.signal_movement(vec2(0, -1));
         break;
      case GLFW_KEY_A:
         player_.signal_movement(vec2(-1, 0));
         break;
      case GLFW_KEY_S:
         player_.signal_movement(vec2(0, 1));
         break;
      case GLFW_KEY_D:
         player_.signal_movement(vec2(1, 0));
         break;

      default:
         break;
   }
}

//////////////////////////////////////////////////////////////////////////////
void GameScene::key_up(I16 key) {
   switch (key) {
      case GLFW_KEY_W:
         player_.signal_movement(vec2(0, 1));
         break;
      case GLFW_KEY_A:
         player_.signal_movement(vec2(1, 0));
         break;
      case GLFW_KEY_S:
         player_.signal_movement(vec2(0, -1));
         break;
      case GLFW_KEY_D:
         player_.signal_movement(vec2(-1, 0));
         break;

      default:
         break;
   }
}

//////////////////////////////////////////////////////////////////////////////
void GameScene::mouse_down(I8 btn) {
   if (btn == 0) {
      player_.block(false);
      player_.attack();
   } else if (btn == 1) {
      player_.block(true);
   }
}

//////////////////////////////////////////////////////////////////////////////
void GameScene::mouse_up(I8 btn) {
   player_.block(false);
}

//////////////////////////////////////////////////////////////////////////////
void GameScene::init() {
   window_dim_ = vec2(service<Window>().dim());
   vec2 world_dim = window_dim_ / 48.f;
   vec2 hdim = world_dim * 0.5f;

   glMatrixMode(GL_PROJECTION);
   glm::mat4 proj = glm::ortho(-hdim.x, hdim.x, hdim.y, -hdim.y);
   glLoadMatrixf(glm::value_ptr(proj));
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
            change_state_(state::play);
         }
         break;

      case state::play:
         // TODO if(player.dead()) { change_state_(state::fade_out); }

         player_.update(dt);

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
void GameScene::render(F64 dt, F64 f) {
   mm_.render();

   if (curtain_opacity_ > 0.f) {
      glMatrixMode(GL_MODELVIEW);
      glLoadIdentity();

      glDisable(GL_DEPTH_TEST);
      glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
      GlTexture::disable();

      vec2 hdim = window_dim_ * 0.5f;

      glBegin(GL_QUADS);
      vec4 color(0, 0, 0, last_curtain_opacity_ + ( F32 ) (f * (curtain_opacity_ - last_curtain_opacity_)));
      glColor4fv(glm::value_ptr(color));
      glVertex2f(-hdim.x, hdim.y);
      glVertex2f(hdim.x, hdim.y);
      glVertex2f(hdim.x, -hdim.y);
      glVertex2f(-hdim.x, -hdim.y);
      glEnd();
   }
}

//////////////////////////////////////////////////////////////////////////////
void GameScene::change_state_(state s) {
   state_ = s;
   state_time_ = 0;
}

} // o
