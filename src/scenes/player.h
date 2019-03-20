#pragma once

#include <Godot.hpp>
#include <Area2D.hpp>

namespace godot {
  class Player : public Area2D {
    GODOT_CLASS(Player, Area2D)

    public:
      static void _register_methods();

      Player();
      virtual ~Player() {}

      void _init();
      void _ready();
      void _process(float delta);


      float getSpeed() const noexcept { return speed; }
      void setSpeed(float speed) noexcept { this->speed = speed; }

    private:
      float speed;
      Size2 screenSize;
      
      float clamp(const float value, const float min, const float max);
  };
}
