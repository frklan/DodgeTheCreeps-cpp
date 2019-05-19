#include <iostream>
#include <cassert>
#include <memory>

#include "Godot.hpp"
#include "OS.hpp"
#include "touchpad.h"
#include <Texture.hpp>
#include <Input.hpp>
#include "InputEventScreenTouch.hpp"
#include "InputEventMouseButton.hpp"
#include "InputEventScreenDrag.hpp"
#include "InputEventMouseMotion.hpp"

namespace godot {
   void printPos(godot::String msg, godot::Vector2 pos) { 
    auto m = std::make_unique<char*>(msg.alloc_c_string());
    std::cout << m.get() << ": " << pos.x << ";" << pos.y << '\n';
  }

  void Touchpad::_register_methods() {
    register_method("_ready", &Touchpad::_ready);
    register_method("_init", &Touchpad::_init);
    register_method("_input", &Touchpad::_input);
    register_method("onTouchTimerTimeout", &Touchpad::onTouchTimerTimeout);

    register_signal<Touchpad>("touchpad_update");
  }

  void Touchpad::onTouchTimerTimeout() {
    setTouchpadPos(currentTouchPos);
  }

  void Touchpad::_ready() {
    input = Input::get_singleton();
    assert(input != nullptr);

    touchpad_ring = cast_to<TextureRect>(find_node("touchpad_ring"));
    assert(touchpad_ring != nullptr);

    touchTimer = cast_to<Timer>(find_node("Timer", true , false));
    assert(touchTimer != nullptr);
    touchTimer->connect("timeout", this, "onTouchTimerTimeout");
    //this->get_tree()->create_timer(2)->connect("timeout", this, "onTouchTimerTimeout");
    

    radius = get_texture()->get_size().width / 2 - touchpad_ring->get_texture()->get_size().width / 2;
    setTouchpadRing2Center();
  }

  void Touchpad::_input(InputEvent* event) {
        if(event->is_class("InputEventMouseMotion")) {
          auto e = cast_to<InputEventMouseMotion>(event);
          currentTouchPos = e->get_global_position();
          touchTimer->stop();
          touchTimer->start();
        } else if(event->is_class("InputEventMouseButton")) {
          auto e = cast_to<InputEventMouseButton>(event);
          currentTouchPos = e->get_global_position();
          touched = e->is_pressed() && this->get_rect().has_point(currentTouchPos);
          
        } else if(event->is_class("InputEventScreenDrag")) {
          auto e = cast_to<InputEventScreenDrag>(event);
          currentTouchPos = e->get_position();
          touchTimer->stop();
          touchTimer->start();
        } else if(event->is_class("InputEventScreenTouch")) {
          auto e = cast_to<InputEventScreenTouch>(event);
          currentTouchPos = e->get_position();
          touched = e->is_pressed() && this->get_rect().has_point(currentTouchPos);
        }

      if(touched) {
        setTouchpadPos(currentTouchPos);
      } else {
        touchTimer->stop();
        setTouchpadRing2Center();
      }
  }

  void Touchpad::setTouchpadRing2Center() {
    touchpad_ring->set_margin(0, -touchpad_ring->get_texture()->get_size().x / 2);
    touchpad_ring->set_margin(1, -touchpad_ring->get_texture()->get_size().y / 2);
    touchpad_ring->set_margin(2, -touchpad_ring->get_texture()->get_size().x / 2);
    touchpad_ring->set_margin(3, -touchpad_ring->get_texture()->get_size().y / 2);
  }

  godot::Vector2 Touchpad::getTouchpadCenter() {
    return (get_global_position() + get_size() / 2) ;
  }

  void Touchpad::setTouchpadPos(godot::Vector2 pos) {
    pos -= getTouchpadCenter();
    pos = pos.clamped(radius);
    auto strenght = pos.length() / radius * 100;
    auto angle = pos.angle();

    if(strenght != 0) {
      pos = pos + get_texture()->get_size() / 2 - touchpad_ring->get_texture()->get_size() / 2;
      touchpad_ring->set_position(pos);
      emit_signal("touchpad_update", strenght, angle);
    }
  }
}
