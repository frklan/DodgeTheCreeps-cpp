#pragma once

#include <Godot.hpp>
#include <KinematicBody2D.hpp>

namespace godot {
  class Mob : public KinematicBody2D {
    GODOT_CLASS(Mob, KinematicBody2D)

    public:
      static void _register_methods();
    
      Mob();
      virtual ~Mob() = default;
      
      void _init();
      void _draw();
      void _ready();
      void _physics_process(float delta);
      void onScreenExited();

      void setMinSpeed(float speed) noexcept { this->minSpeed = speed; }
      float getMinSpeed() const noexcept { return minSpeed; }

      void setMaxSpeed(float speed) noexcept { this->maxSpeed = speed; }
      float getMaxSpeed() const noexcept { return maxSpeed; }

      bool unreference(){ return true; };
      bool init_ref(){ return true; };
	    bool reference(){ return true; };

      void setTarget(Vector2 newTarget) { 
        target = newTarget;
        std::cout << "x = " << target.x << '\n';
        std::cout << "y = " << target.y << '\n';
      }

    private:
      float minSpeed;
      float maxSpeed;
      Vector2 target;

      const Array mobType = Array::make<godot::String>("walk", "swim", "fly");
  };
}