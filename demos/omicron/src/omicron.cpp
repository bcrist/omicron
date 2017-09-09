#include "game.hpp"
#include <be/core/service_helpers.hpp>

#ifdef BE_DEBUG
#pragma comment(lib, "glew-debug")
#pragma comment(lib, "core-debug")
#pragma comment(lib, "core-id-with-names-debug")
#else
#pragma comment(lib, "glew")
#pragma comment(lib, "core")
#pragma comment(lib, "core-id-with-names")
#endif

int main(int argc, char** argv) {
   return be::service<o::Game>()(argc, argv).run();
}
