#include <random>
#include <Godot.hpp>
#include <AnimatedSprite.hpp>
#include <VisibilityNotifier2D.hpp>
#include <KinematicCollision2D.hpp>

#include "player.h"
#include "mob.h"

namespace godot {
  Mob::Mob()
  { }
  
  void Mob::_register_methods() {
    register_property<Mob, float>("Minimum speed", &Mob::setMinSpeed, &Mob::getMinSpeed, 150);
    register_property<Mob, float>("Maximum speed", &Mob::setMaxSpeed, &Mob::getMaxSpeed, 250);
    
    register_method("_draw", &Mob::_draw);
    register_method("_init", &Mob::_init);
    register_method("_ready", &Mob::_ready);
    register_method("onScreenExited", &Mob::onScreenExited);
    register_method("getMaxSpeed", &Mob::getMaxSpeed);
    register_method("_physics_process", &Mob::_physics_process);
  }

  void Mob::_init() {
    
  }

  void Mob::_ready() {
    std::mt19937 rng;
    rng.seed(std::random_device()());
    std::uniform_int_distribution<std::mt19937::result_type> dist(0, mobType.size() - 1);

    auto sprite = cast_to<AnimatedSprite>(get_node("AnimatedSprite"));
    sprite->set_animation(mobType[dist(rng)]);
    
    auto visibilityNode = cast_to<VisibilityNotifier2D>(get_node("VisibilityNotifier2D"));
    visibilityNode->connect("screen_exited", this, "onScreenExited");
  }

  void Mob::_physics_process(float delta) {
    auto m = target - get_position();
    auto collision = move_and_collide(m * delta * 0.1f);
    
    if(collision.is_valid()) {
      cast_to<Player>(get_node("/root")->find_node("Player", true, false))->onBodyentered(this);
    }
  }

  void Mob::_draw() {
    // auto direction = get_rotation();
    
    // auto x = cos(direction * 180 / M_PI) * 100;
    // auto y = tan(direction * 180 / M_PI) * x;
    // draw_circle({static_cast<real_t>(x), static_cast<real_t>(y)}, 3, {100, 100, 100, 100});
    
  }

  void Mob::onScreenExited() {
    queue_free();
  }

}
