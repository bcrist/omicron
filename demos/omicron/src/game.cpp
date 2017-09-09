#include "game.hpp"
#include "window.hpp"
#include "texture_manager.hpp"
#include "splash_scene.hpp"
#include "game_scene.hpp"
#include <be/core/service_helpers.hpp>
#include <be/core/logging.hpp>
#include <be/util/path_glob.hpp>
#include <be/util/paths.hpp>
#include <be/util/get_file_contents.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/matrix_transform.hpp>

#ifndef STBI_NO_STDIO
#define STBI_NO_STDIO
#endif
#include <stb/stb_image.h>

namespace o {
namespace {

//////////////////////////////////////////////////////////////////////////////
void stbi_delete(void* ptr, std::size_t) {
   stbi_image_free(ptr);
}

} // o::()

//////////////////////////////////////////////////////////////////////////////
Game& Game::operator()(int argc, char** argv) {
   service<Window>().create();

   Path asset_root = util::default_special_path(util::SpecialPath::install) / "assets";

   // load textures
   TextureManager& tm = service<TextureManager>();
   tm.clear();
   std::vector<Path> images = util::greb(".*\\.(png|tga)", asset_root, util::PathMatchType::files);
   for (Path& p : images) {
      Buf<UC> data = util::get_file_contents_buf(p);
      int w, h, comp;
      auto ptr = stbi_load_from_memory(data.get(), ( int ) data.size(), &w, &h, &comp, 0);
      if (ptr) {
         Buf<const UC> buf(ptr, (std::size_t)(w * h * comp), stbi_delete);
         Id id(p.stem().generic_string());
         tm.add(id, Texture(buf, ivec2(w, h), comp));

         be_verbose() << "Loaded Texture"
            & attr(ids::log_attr_path) << p.string()
            & attr(ids::log_attr_id) << id
            & attr("Dimensions") << w << "x" << h
            | default_log();

      } else {
         be_warn() << "Failed to load texture!"
            & attr(ids::log_attr_path) << p.string()
            & attr(ids::log_attr_error) << S(stbi_failure_reason())
            | default_log();
      }
   }

   // load texture atlases
   // TODO

   return *this;
}

//////////////////////////////////////////////////////////////////////////////
int Game::run() {
   Window& window = service<Window>();
   window.show();

   glMatrixMode(GL_PROJECTION);
   glm::mat4 proj = glm::ortho(0, 1, 0, 1);
   glLoadMatrixf(glm::value_ptr(proj));

   glMatrixMode(GL_MODELVIEW);
   glLoadIdentity();

   glEnable(GL_BLEND);
   glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

   scene(std::make_unique<SplashScene>());

   prev_update_time_ = -fixed_update_rate_;
   update_time_ = 0;

   while (!glfwWindowShouldClose(window.glfw())) {
      glfwPollEvents();
      glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

      F64 time = glfwGetTime();
      while (time > update_time_) {
         prev_update_time_ = update_time_;
         update_time_ += fixed_update_rate_;
         Scene* s = scene_.get();
         if (s) {
            s->update(fixed_update_rate_);
         }
      }

      F64 interpolation_coeff = (time - prev_update_time_) / fixed_update_rate_;
      Scene* s = scene_.get();
      if (s) {
         s->render(fixed_update_rate_, interpolation_coeff);
      }

      if (time > timer_reset_interval_) {
         glfwSetTime(time - prev_update_time_);
         prev_update_time_ = 0;
         update_time_ = fixed_update_rate_;
      }
      glfwSwapBuffers(window.glfw());
   }

   return 0;
}

//////////////////////////////////////////////////////////////////////////////
void Game::new_game() {
   scene(std::make_unique<GameScene>());
}

//////////////////////////////////////////////////////////////////////////////
Scene* Game::scene() {
   return scene_.get();
}

//////////////////////////////////////////////////////////////////////////////
void Game::scene(std::unique_ptr<Scene> s) {
   be_verbose() << "Scene Change"
      & attr(ids::log_attr_name) << s->name()
      | default_log();

   s->update(fixed_update_rate_);
   service<Window>().scene(s.get());
   scene_ = std::move(s);
}

} // o
