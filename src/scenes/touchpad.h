#pragma once

#include <Godot.hpp>
#include <Input.hpp>
#include <TextureRect.hpp>

namespace godot {
  class Touchpad : public TextureRect {
    GODOT_CLASS(Touchpad, TextureRect)

  public:
    static void _register_methods();
    
    void _init() {}
    void _ready();
 
    void _input(InputEvent* event);

  private:
    godot::Input* input;
    godot::TextureRect* touchpad_ring;
    float radius = 0;
    
    void setTouchpadRing2Center();
    godot::Vector2 getTouchpadCenter();
    void setTouchpadPos(godot::Vector2 pos);
  };
}
