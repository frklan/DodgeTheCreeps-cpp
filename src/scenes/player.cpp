
#include <Godot.hpp>
#include <AnimatedSprite.hpp>
#include <Input.hpp>
#include <OS.hpp>
#include <Object.hpp>
#include <CollisionShape2D.hpp>

#include "player.h"
#include "mob.h"

namespace godot {
  Player::Player() :
  speed(400)
  { }
  
  void Player::_register_methods() {
    //register_property<Player, float>("Move speed", &Player::setSpeed, &Player::getSpeed, 400);

    register_method("_init", &Player::_init);
    register_method("_ready", &Player::_ready);
    register_method("_process", &Player::_process);
    register_method("onBodyentered", &Player::onBodyentered);

    register_signal<Player>("hit");    
  }

  void Player::_init() {
    
  }

  void Player::_ready() {
    screenSize = get_viewport_rect().size;
    this->connect("body_entered", this, "onBodyentered");

    //this->hide();
  }

  void Player::_process(float delta) {
    Vector2 velocity{0, 0};

    auto input = Input::get_singleton();
    
    if(input->is_action_pressed("ui_right")) {
      velocity.x += 1;
    }
    if(input->is_action_pressed("ui_left")) {
      velocity.x -= 1;
    }
    if(input->is_action_pressed("ui_down")) {
      velocity.y += 1;
    }
    if(input->is_action_pressed("ui_up")) {
      velocity.y -= 1;
    }

    auto sprite = cast_to<AnimatedSprite>(get_node("AnimatedSprite"));
    if(velocity.length() > 0) {
      velocity = velocity.normalized() * speed;

      Godot::print(velocity);

      sprite->play();
    } else {
      sprite->stop();
    }

    auto position = this->get_position();
    position += velocity * delta;
    position.x = clamp(position.x, 0 , screenSize.x);
    position.y = clamp(position.y, 0 , screenSize.y);
    this->set_position(position);
    
    

    if(velocity.x != 0) {
      sprite->set_animation("right");
      sprite->set_flip_v(false);
      sprite->set_flip_h(velocity.x < 0);
    } else if(velocity.y != 0) {
      sprite->set_animation("up");
      sprite->set_flip_v(velocity.y > 0);
    }
  }

  void Player::onBodyentered(Object* object) {
    Godot::print("onBodyEnterd!");
    hide();
    emit_signal("hit");
    auto collision = cast_to<CollisionShape2D>(get_node("CollisionShape2D"));
    collision->set_deferred("disabled", true);

    //auto mob = cast_to<Mob>(object);
  }

  void Player::start(Vector2 pos) {
    this->set_position(pos);
    show();
    auto collision = cast_to<CollisionShape2D>(get_node("CollisionShape2D"));
    collision->set_deferred("disabled", false);
  }

  float Player::clamp(const float value, const float min, const float max) {    
    if(value < min) return min;
    if(value > max) return max;
    
    return value;
  }
}
