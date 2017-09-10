#include "game.hpp"
#include "window.hpp"
#include "texture_manager.hpp"
#include "splash_scene.hpp"
#include "game_scene.hpp"
#include <be/core/service_helpers.hpp>
#include <be/core/log_exception.hpp>
#include <be/cli/cli.hpp>
#include <be/core/logging.hpp>
#include <be/util/path_glob.hpp>
#include <be/util/paths.hpp>
#include <be/util/get_file_contents.hpp>
#include <pugixml/pugixml.hpp>

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
   try {
      using namespace cli;
      Processor proc;

      proc
      (prologue(ct::Table() << ct::header << "OMICRON").query())
         (verbosity_param({ "v" }, { "verbosity" }, "LEVEL", default_log().verbosity_mask()))
         ;

      proc.process(argc, argv);
   } catch (const cli::OptionError& e) {
      log_exception(e);
   } catch (const cli::ArgumentError& e) {
      log_exception(e);
   } catch (const FatalTrace& e) {
      log_exception(e);
   } catch (const RecoverableTrace& e) {
      log_exception(e);
   } catch (const fs::filesystem_error& e) {
      log_exception(e);
   } catch (const std::system_error& e) {
      log_exception(e);
   } catch (const std::exception& e) {
      log_exception(e);
   }

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
   std::vector<Path> atlases = util::glob("*.atlas", asset_root, util::PathMatchType::files);
   for (Path& p : atlases) {
      using namespace pugi;
      Buf<UC> data = util::get_file_contents_buf(p);
      xml_document doc;
      doc.load_buffer_inplace(data.get(), data.size());

      auto root_node = doc.root().first_child();
      auto common_node = root_node.child("common");
      vec2 scale(common_node.attribute("scaleW").as_uint(), common_node.attribute("scaleH").as_uint());

      auto page_node = root_node.child("pages").first_child();
      Id tex_id = Id(Path(page_node.attribute("file").value()).stem().generic_string());
      Texture& tex = tm.get(tex_id);

      for (xml_node char_node : root_node.child("chars").children()) {
         Id region_id;
         if (xml_attribute attr = char_node.attribute("id"); !attr.empty()) {
            region_id = Id(attr.as_uint());
         } else {
            region_id = Id(char_node.attribute("name").value());
         }

         TextureRegion region;

         region.display_bounds.dim.x = ( F32 ) char_node.attribute("width").as_uint();
         region.display_bounds.dim.y = ( F32 ) char_node.attribute("height").as_uint();

         if (xml_attribute attr = char_node.attribute("xoffset"); !attr.empty()) {
            region.display_bounds.offset.x = ( F32 ) attr.as_int();
         } else {
            region.display_bounds.offset.x = -region.display_bounds.dim.x / 2;
         }

         if (xml_attribute attr = char_node.attribute("yoffset"); !attr.empty()) {
            region.display_bounds.offset.y = ( F32 ) attr.as_int();
         } else {
            region.display_bounds.offset.y = -region.display_bounds.dim.y / 2;
         }

         region.tex_bounds.offset.x = char_node.attribute("x").as_uint() / scale.x;
         region.tex_bounds.offset.y = char_node.attribute("y").as_uint() / scale.y;
         region.tex_bounds.dim.x = region.display_bounds.dim.x / scale.x;
         region.tex_bounds.dim.y = region.display_bounds.dim.y / scale.y;

         if (xml_attribute attr = char_node.attribute("xadvance"); !attr.empty()) {
            region.advance.x = ( F32 ) attr.as_int();
         }

         if (xml_attribute attr = char_node.attribute("yadvance"); !attr.empty()) {
            region.advance.y = ( F32 ) attr.as_int();
         }

         if (xml_attribute attr = char_node.attribute("down"); !attr.empty()) {
            if (attr.value() == S("-y")) {
               region.down = Axis::neg_y;
            } else if (attr.value() == S("+y")) {
               region.down = Axis::pos_y;
            } else if (attr.value() == S("-x")) {
               region.down = Axis::neg_x;
            } else if (attr.value() == S("+x")) {
               region.down = Axis::pos_x;
            }
         }

         if (xml_attribute attr = char_node.attribute("right"); !attr.empty()) {
            if (attr.value() == S("-y")) {
               region.right = Axis::neg_y;
            } else if (attr.value() == S("+y")) {
               region.right = Axis::pos_y;
            } else if (attr.value() == S("-x")) {
               region.right = Axis::neg_x;
            } else if (attr.value() == S("+x")) {
               region.right = Axis::pos_x;
            }
         }

         tex.add_region(region_id, region);

         be_short_debug() << "Texture Atlas Region: " << region_id | default_log();
      }

      be_verbose() << "Loaded Texture Atlas"
            & attr(ids::log_attr_path) << p.string()
            & attr(ids::log_attr_id) << tex_id
            | default_log();

   }

   return *this;
}

//////////////////////////////////////////////////////////////////////////////
int Game::run() {
   Window& window = service<Window>();
   window.show();

   glClearColor(0, 0, 0, 0);

   glEnable(GL_BLEND);
   glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
   glDisable(GL_CULL_FACE);

   scene(std::make_unique<SplashScene>());

   prev_update_time_ = -fixed_update_rate_;
   update_time_ = 0;
   new_game_scene_ = false;

   glfwSetTime(0);

   while (!glfwWindowShouldClose(window.glfw())) {
      if (new_game_scene_) {
         scene(std::make_unique<GameScene>());
         new_game_scene_ = false;
      }

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

   scene(std::unique_ptr<Scene>());

   return 0;
}

//////////////////////////////////////////////////////////////////////////////
void Game::new_game() {
   new_game_scene_ = true;
}

//////////////////////////////////////////////////////////////////////////////
Scene* Game::scene() {
   return scene_.get();
}

//////////////////////////////////////////////////////////////////////////////
void Game::scene(std::unique_ptr<Scene> s) {
   if (s) {
      be_verbose() << "Scene Change"
         & attr(ids::log_attr_name) << s->name()
         | default_log();

      s->init();
   }
   service<Window>().scene(s.get());
   scene_ = std::move(s);
}

} // o
