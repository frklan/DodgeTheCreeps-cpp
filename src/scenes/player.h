#pragma once

#include <Godot.hpp>
#include <Sprite.hpp>

namespace godot {
  class Player : public Sprite {
    GODOT_CLASS(Player, Sprite)

    public:
      static void _register_methods();

      Player();
      virtual ~Player() {}

      void _init();
      void _process(float delta);
    private:
  };
}
