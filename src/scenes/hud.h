#pragma once

#include <Godot.hpp>
#include <Button.hpp>
#include <Node2D.hpp>

namespace godot {
  class HUD : public Node2D {
    GODOT_CLASS(HUD, Node2D)
    public:
      static void _register_methods();
      
      HUD() {};
      virtual ~HUD() {}
      
      void _init();
      void _ready();
      void onStartNewGamePressed();
      void onMessageTimerTimeout();
      void onGameOver();

      void showMessage(godot::String message, bool timeout = true);
      void showScore(const int score);
  };
}
