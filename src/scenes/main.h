#pragma once

#include <random>

#include <Godot.hpp>
#include <Node.hpp>
#include <PackedScene.hpp>
#include <RigidBody2D.hpp>
#include "mob.h"
#include "hud.h"

namespace godot {
  constexpr float MIN_RANDOM = std::numeric_limits<float>::min();
  constexpr float MAX_RANDOM = std::numeric_limits<float>::max() / 2;

  class Main : public Node {
    GODOT_CLASS(Main, Node)

    public:
      static void _register_methods();

      Main();
      virtual ~Main() {}

      void _init();
      void _ready();
      void _process(float delta);

      void onGameOver();
      void onStartTimerTimeOut();
      void onMobTimerTimeout();
      void onScoreTimerTimeout();
    
      void startNewGame();

    private:
    uint16_t score = 0;
    std::mt19937 rng;
    HUD* hud = nullptr;
    //std::uniform_int_distribution<std::mt19937::result_type> dist;
    
    godot::Ref<PackedScene> mobScene;
    godot::Ref<PackedScene> touchPadScene;

    float getRandom(float min = 0.0f, float max = 32565.0f);
    void purgeMobs();
  };
}
