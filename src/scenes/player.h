#pragma once

#include <mutex>
#include <Godot.hpp>
#include <KinematicBody2D.hpp>
#include <AnimatedSprite.hpp>

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
      void _physics_process(float delta);
      void onCollision(Object* object);
      void onPlayerHit();
      void onTouchpadEvent(float strenght, float angle);
      void start(Vector2 pos);

      float getSpeed() const noexcept { return speed; }
      void setSpeed(float newSpeed) noexcept { this->speed = newSpeed; }

    private:
      float speed;
      Size2 screenSize;
      Vector2 velocity{0, 0};
      AnimatedSprite* sprite;
      
      float clamp(const float value, const float min, const float max);
  };
}
