#pragma once
#ifndef O_GAME_HPP_
#define O_GAME_HPP_

#include "scene.hpp"
#include <be/core/lifecycle.hpp>
#include <be/platform/lifecycle.hpp>
#include <be/core/service.hpp>

namespace o {

class Game;

} // o
namespace be {

///////////////////////////////////////////////////////////////////////////////
template <>
struct SuppressUndefinedService<o::Game> : True { };

} // be
namespace o {

///////////////////////////////////////////////////////////////////////////////
class Game final {
public:
   Game& operator()(int argc, char** argv);
   int run();

   void new_game();

   Scene* scene();
   void scene(std::unique_ptr<Scene> s);

private:
   CoreInitLifecycle cilc_;
   CoreLifecycle clc_;
   PlatformLifecycle plc_;

   std::unique_ptr<Scene> scene_;
   F64 prev_update_time_;
   F64 update_time_;

   static constexpr F64 fixed_update_rate_ = 1 / 32.f;
   static constexpr F64 timer_reset_interval_ = 60.f;
};

} // o

#endif
