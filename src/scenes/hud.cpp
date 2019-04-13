#include <Godot.hpp>
#include <Label.hpp>
#include <Timer.hpp>

#include "hud.h"
namespace godot {

  void HUD::_register_methods() {
    register_method("_ready", &HUD::_ready);
    register_method("onStartNewGamePressed", &HUD::onStartNewGamePressed);
    register_method("onGameOver", &HUD::onGameOver);
    register_method("onMessageTimerTimeout", &HUD::onMessageTimerTimeout);
    
    register_signal<HUD>("startGame");    
  }

   void HUD::_init() {
    
  }
  void HUD::_ready() {
    find_node("StartButton")->connect("pressed", this, "onStartNewGamePressed");
    find_node("MessageTimer")->connect("timeout", this, "onMessageTimerTimeout");
    
    // The "Player"  node is not a child of this node, 
    // so let's search for it globally
    get_node("/root")->find_node("Player", true, false)->connect("gameOver", this, "onGameOver");
  }

  void HUD::onStartNewGamePressed() {
    cast_to<Button>(find_node("StartButton"))->hide();
    showMessage("Get ready!");
    emit_signal("startGame");
  }

  void HUD::onGameOver() {
    showMessage("Game over", true);
    cast_to<Button>(find_node("StartButton"))->show();
  }

  void HUD::onMessageTimerTimeout() {
    cast_to<Label>(find_node("MessageLabel"))->hide();
  }

  void HUD::showMessage(godot::String message, bool timeout) {
    auto msgLabel = cast_to<Label>(find_node("MessageLabel"));
    msgLabel->set_text(message);
    msgLabel->show();

    if(timeout == true) {
      auto timer = cast_to<Timer>(find_node("MessageTimer"));
      timer->start();
    }
  }

  void HUD::showScore(const int score) {
    auto scoreLabel =  cast_to<Label>(find_node("ScoreLabel"));
    auto s = godot::String("{0}");
    scoreLabel->set_text(s.format(godot::Array::make(score)));
  }
}
