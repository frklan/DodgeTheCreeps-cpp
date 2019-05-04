#pragma once

#include <Godot.hpp>
#include <KinematicBody2D.hpp>

namespace godot {
  class Player : public KinematicBody2D {
    GODOT_CLASS(Player, KinematicBody2D)

    public:
      static void _register_methods();
      static void _bind_methods();

      Player();
      virtual ~Player() {}

      void _init();
      void _ready();
      void _process(float delta);
      void _input(InputEvent* event);
      void onBodyentered(Object* object);
      void onPlayerHit();
      void start(Vector2 pos);

      float getSpeed() const noexcept { return speed; }
      void setSpeed(float newSpeed) noexcept { this->speed = newSpeed; }

    private:
      float speed;
      Size2 screenSize;

      float clamp(const float value, const float min, const float max);
  };
}
