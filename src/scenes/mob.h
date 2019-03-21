#pragma once

#include <Godot.hpp>
#include <RigidBody2D.hpp>

namespace godot {
  class Mob : public RigidBody2D {
    GODOT_CLASS(Mob, RigidBody2D)

    public:
      static void _register_methods();
    
      Mob();
      virtual ~Mob() {}

      void _init();
      void _ready();
      void onScreenExited();

      void setMinSpeed(float speed) noexcept { this->minSpeed = speed; }
      float getMinSpeed() const noexcept { return minSpeed; }

      void setMaxSpeed(float speed) noexcept { this->maxSpeed = speed; }
      float getMaxSpeed() const noexcept { return maxSpeed; }


    private:
      float minSpeed = 150;
      float maxSpeed = 250;

      const Array mobType = Array::make<godot::String>("walk", "swim", "fly");
  };
}