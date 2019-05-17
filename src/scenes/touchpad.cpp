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

    register_signal<Touchpad>("touchpad_update");
  }

  void Touchpad::_ready() {
    input = Input::get_singleton();
    assert(input != nullptr);

    touchpad_ring = cast_to<TextureRect>(find_node("touchpad_ring"));
    assert(touchpad_ring != nullptr);

    radius = get_texture()->get_size().width / 2 - touchpad_ring->get_texture()->get_size().width / 2;
    setTouchpadRing2Center();
  }

  void Touchpad::_input(InputEvent* event) {
    if(!OS::get_singleton()->has_touchscreen_ui_hint ()) {
      if(input->is_mouse_button_pressed(1)) {
        godot::Vector2 mousePos{0, 0};
      
        if(event->is_class("InputEventMouseMotion")) {
          auto e = cast_to<InputEventMouseMotion>(event);
          mousePos = e->get_global_position() - getTouchpadCenter();
        } else if(event->is_class("InputEventMouseButton")) {
          auto e = cast_to<InputEventMouseButton>(event);
          mousePos = e->get_global_position() - getTouchpadCenter();
          
        }
        mousePos = mousePos.clamped(radius);
        setTouchpadPos(mousePos);
      } else {
        setTouchpadRing2Center();
      }
    } else {
      godot::Vector2 touchPos{0, 0};
      if(event->is_class("InputEventScreenTouch")) {
        auto e = cast_to<InputEventScreenTouch>(event);
        touchPos = e->get_position() - getTouchpadCenter();
        if(!e->is_pressed()) { // bail if touch ended!
          setTouchpadRing2Center();
          return;
        }
      } else if(event->is_class("InputEventScreenDrag")) {
        auto e = cast_to<InputEventScreenDrag>(event);
        touchPos = e->get_position() - getTouchpadCenter();
      } else { 
          setTouchpadRing2Center();
      }
      touchPos = touchPos.clamped(radius);
      setTouchpadPos(touchPos);
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
    auto strenght = pos.length() / radius * 100;
    auto angle = pos.angle();

    if(strenght != 0) {
      pos = pos + get_texture()->get_size() / 2 - touchpad_ring->get_texture()->get_size() / 2;
      touchpad_ring->set_position(pos);
      emit_signal("touchpad_update", strenght, angle);
    }
  }
}
