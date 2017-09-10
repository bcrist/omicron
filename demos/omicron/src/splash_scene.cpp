#include "splash_scene.hpp"
#include "game.hpp"
#include "window.hpp"
#include "texture_manager.hpp"
#include "mesh_helpers.hpp"
#include <be/core/service_helpers.hpp>
#include <GL/glew.h>
#include <be/platform/glfw.hpp>
#include <be/core/logging.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

namespace o {

//////////////////////////////////////////////////////////////////////////////
SplashScene::SplashScene()
   : curtain_opacity_(1.f),
   curtain_speed_(initial_curtain_speed_) {
   change_state_(state::fade_in_logo);
}

//////////////////////////////////////////////////////////////////////////////
void SplashScene::init() {
   window_dim_ = vec2(service<Window>().dim());
   vec2 hdim = window_dim_ * 0.5f;

   glMatrixMode(GL_PROJECTION);
   glm::mat4 proj = glm::ortho(-hdim.x, hdim.x, hdim.y, -hdim.y);
   glLoadMatrixf(glm::value_ptr(proj));

   //Texture& logos = service<TextureManager>().get(Id("logos"));
   Texture& font = service<TextureManager>().get(Id("petme128_0"));

   logos_ = mm_.obtain(3);
   // TODO logos

   title_ = mm_.obtain(8);

   F32 title_scale = window_dim_.x / 80;
   auto title_bounds = text_mesh_append(title_, font, "OMICRON", RGBA(255), vec2(), title_scale);
   title_transform_ = glm::translate(mat4(), vec3(title_bounds.second.dim * -0.5f, 0));
   title_.transform(&title_transform_);
}

//////////////////////////////////////////////////////////////////////////////
void SplashScene::key_up(I16 key) {
   if (state_ < state::fade_out_logo) {
      change_state_(state::fade_out_logo);
      curtain_speed_ = fast_curtain_speed_;
   } else if (state_ < state::fade_out_title) {
      change_state_(state::fade_out_logo);
      curtain_speed_ = fast_curtain_speed_;
   }
}

//////////////////////////////////////////////////////////////////////////////
void SplashScene::mouse_up(I8 btn) {
   key_up(0);
}

//////////////////////////////////////////////////////////////////////////////
void SplashScene::update(F64 dt) {
   state_time_ += dt;

   switch (state_) {

      case state::fade_in_logo:
         last_curtain_opacity_ = curtain_opacity_;
         if (curtain_opacity_ > 0.f) {
            curtain_opacity_ -= ( F32 ) (curtain_speed_ * dt);
         }
         if (curtain_opacity_ <= 0.f) {
            change_state_(state::wait_logo);
         }
         break;

      case state::wait_logo:
         if (state_time_ >= wait_duration_logo_) {
            change_state_(state::fade_out_logo);
         }
         break;

      case state::fade_out_logo:
         last_curtain_opacity_ = curtain_opacity_;
         if (curtain_opacity_ < 1.f) {
            curtain_opacity_ += ( F32 ) (curtain_speed_ * dt);
         }
         if (curtain_opacity_ >= 1.f) {
            change_state_(state::fade_in_title);
         }
         break;

      case state::fade_in_title:
         curtain_speed_ = initial_curtain_speed_;
         last_curtain_opacity_ = curtain_opacity_;
         if (curtain_opacity_ > 0.f) {
            curtain_opacity_ -= ( F32 ) (curtain_speed_ * dt);
         }
         if (curtain_opacity_ <= 0.f) {
            change_state_(state::wait_title);
         }
         break;

      case state::wait_title:
         if (state_time_ >= wait_duration_title_) {
            change_state_(state::fade_out_title);
         }
         break;

      case state::fade_out_title:
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

   
   logos_.enabled(state_ < state::fade_in_title);
   title_.enabled(state_ >= state::fade_in_title);
}

//////////////////////////////////////////////////////////////////////////////
void SplashScene::render(F64 dt, F64 f) {
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
void SplashScene::change_state_(state s) {
   be_verbose() << "state: " << ( U32 ) (s) | default_log();

   state_ = s;
   state_time_ = 0;
}

} // o
