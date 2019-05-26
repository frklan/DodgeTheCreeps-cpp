
#include <Godot.hpp>
#include <AnimatedSprite.hpp>
#include <Input.hpp>
#include <InputEvent.hpp>
#include <InputEventScreenDrag.hpp>
#include <OS.hpp>
#include <Object.hpp>
#include <Timer.hpp>
#include <String.hpp>
#include <CollisionShape2D.hpp>
#include <KinematicCollision2D.hpp>
#include "InputEventScreenTouch.hpp"
#include "InputEventScreenDrag.hpp"
#include "player.h"
#include "mob.h"

namespace godot {
  Player::Player() :
  speed(400)
  { }
  
  void Player::_register_methods() {
    register_property<Player, float>("Speed", &Player::setSpeed, &Player::getSpeed, 400);

    register_method("_init", &Player::_init);
    register_method("_ready", &Player::_ready);
    register_method("_process", &Player::_process);
    register_method("_physics_process", &Player::_physics_process);
    register_method("onPlayerHit", &Player::onPlayerHit);
    register_method("onTouchpadEvent", &Player::onTouchpadEvent);

    register_signal<Player>("gameOver");
    register_signal<Player>("hit");   
  }

  void Player::_init() {
    
  }

  void Player::_ready() {
    screenSize = get_viewport_rect().size;
    this->connect("hit", this, "onPlayerHit");
    this->hide();

    auto touchpad = get_node("/root")->find_node("Touchpad", true, false);
    if(touchpad != nullptr) {
      touchpad->connect("touchpad_update", this, "onTouchpadEvent");
    } else {
      std::cerr << "Touchpad not found!\n";
    }

    sprite = cast_to<AnimatedSprite>(get_node("AnimatedSprite"));
  }

  void Player::_process(float delta) {
    auto position = this->get_position();
    if(position.x < 0) position.x = 0;
    if(position.x > screenSize.x) position.x = screenSize.x;
    if(position.y > screenSize.y) position.y = screenSize.y;
    if(position.y < 0) position.y = 0;
    set_position(position);
  }

  void Player::_physics_process(float delta) {
    // Key input @ desktop
    if(!OS::get_singleton()->has_touchscreen_ui_hint ()) {
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
      if(velocity.length() == 0 ) {
        sprite->stop();
      }
    }

    
    // this will also handle move avtions from touchpad (set in onTouchpadEvent)
   
    if(velocity.length() > 0) {
      if(!sprite->is_playing()){
        sprite->play();
      }

      if(abs(velocity.x) > abs(velocity.y)) {
        sprite->set_animation("right");
      } else {
        sprite->set_animation("up");
      }
      sprite->set_flip_h(velocity.x < 0);
      sprite->set_flip_v(velocity.y > 0);
      
      auto collision = this->move_and_collide(velocity * delta * getSpeed());
      if(collision.is_valid()) {
        onCollision(collision->get_collider());
      }
      velocity = {0, 0};
    }
    
  }

  void Player::onTouchpadEvent(float strenght, float angle) {
    if(this->is_visible()){
      velocity.y += sin(angle) * strenght * 0.02;
      velocity.x += cos(angle) * strenght * 0.02;

      // std::cout << "onTouchpadEvent(): velocity = " << velocity.x << ';' << velocity.y 
      //   << ", strenght = " << strenght << ", angle = " << angle << '\n';
    }
  }

  void Player::onCollision(Object* object) {
    hide();
    emit_signal("hit");
    auto collision = cast_to<CollisionShape2D>(get_node("CollisionShape2D"));
    collision->set_deferred("disabled", true);
  }

  void Player::onPlayerHit() {
    // todo: deduct life etc. 
    emit_signal("gameOver");
  }

  void Player::start(Vector2 pos) {   
    sprite->stop();
    sprite->set_animation("up");
    
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
