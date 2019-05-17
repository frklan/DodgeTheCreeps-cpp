#include <random>
#include <cmath>

#include <Godot.hpp>
#include <Node.hpp>
#include <ResourceLoader.hpp>
#include <PackedScene.hpp>
#include <OS.hpp>
#include <SceneTree.hpp>
#include <Input.hpp>
#include <Timer.hpp>
#include <Position2D.hpp>
#include <PathFollow2D.hpp>
#include <AudioStreamPlayer.hpp>
#include <Viewport.hpp>
#include <Curve2D.hpp>
#include <Path2D.hpp>

#include "main.h"
#include "mob.h"
#include "hud.h"
#include "player.h"
#include "touchpad.h"

namespace godot {


  void Main::_register_methods() {
      register_method("_init", &Main::_init);
      register_method("_ready", &Main::_ready);
      register_method("_process", &Main::_process);
      register_method("onGameOver", &Main::onGameOver);
      register_method("onStartTimerTimeOut", &Main::onStartTimerTimeOut);
      register_method("onMobTimerTimeout", &Main::onMobTimerTimeout);
      register_method("onScoreTimerTimeout", &Main::onScoreTimerTimeout);
      register_method("startNewGame", &Main::startNewGame);
  }

  Main::Main() {
    rng.seed(std::random_device()());
    mobScene = ResourceLoader::get_singleton()->load("res://scenes/mob/mob.tscn");
    touchPadScene = ResourceLoader::get_singleton()->load("res://scenes/touchpad/touchpad.tscn");
  }

  void Main::_init() {
  }

  void Main::_ready() {
    get_node("Player")->connect("gameOver", this, "onGameOver");
    get_node("StartTimer")->connect("timeout", this, "onStartTimerTimeOut");
    get_node("MobTimer")->connect("timeout", this, "onMobTimerTimeout");
    get_node("ScoreTimer")->connect("timeout", this, "onScoreTimerTimeout");

    hud = cast_to<HUD>(get_node("HUD"));
    hud->connect("startGame", this, "startNewGame");

    auto viewport = get_viewport();
    auto screenSize= viewport->get_size();

    // set path elemts to screen edge
    auto curve = cast_to<Path2D>(get_node("MobPath"))->get_curve();
    curve->clear_points();
    curve->add_point({0, 0});
    curve->add_point({screenSize.width, 0});
    curve->add_point({screenSize.width, screenSize.height});
    curve->add_point({0, screenSize.height});
    curve->add_point({0, 0});
  }

  void Main::_process(float delta) {

  }

  void Main::onGameOver() {
    cast_to<Timer>(get_node("ScoreTimer"))->stop();
    cast_to<Timer>(get_node("MobTimer"))->stop();

    cast_to<AudioStreamPlayer>(get_node("BackgroundMusic"))->stop();
    cast_to<AudioStreamPlayer>(get_node("SoundEffects"))->play();

    auto touchPad = cast_to<Touchpad>(get_node("Touchpad"));
    touchPad->set_deferred("disabled", true);
    touchPad->set_visible(false);
  }

  void Main::onStartTimerTimeOut() {
    cast_to<Timer>(get_node("MobTimer"))->start();
    cast_to<Timer>(get_node("ScoreTimer"))->start();

    cast_to<AudioStreamPlayer>(get_node("BackgroundMusic"))->play();
  }

  void Main::onScoreTimerTimeout() {
    score++;
    hud->showScore(score);
  }
  
  void Main::onMobTimerTimeout() {
    // # Choose a random location on Path2D.
    // $MobPath/MobSpawnLocation.set_offset(randi())
    auto spawnLocation = cast_to<PathFollow2D>(get_node("MobPath/MobSpawnLocation"));
    spawnLocation->set_offset(getRandom());

    // # Create a Mob instance and add it to the scene.
    // var mob = Mob.instance()
    // add_child(mob)
    auto newMob = cast_to<KinematicBody2D>(mobScene->instance());
    newMob->set_name("Mob");
    get_node("MobHolder")->add_child(newMob);

    // # Set the mob's position to a random location.
    // mob.position = $MobPath/MobSpawnLocation.position
    newMob->set_position(spawnLocation->get_position());
    //newMob->set_position({getRandom(500, 1000), getRandom(500, 1000)});
    
    // # Set the mob's direction perpendicular to the path direction.
    // var direction = $MobPath/MobSpawnLocation.rotation + PI / 2
    //auto direction = spawnLocation->get_rotation() + M_PI_2;
    //auto direction = M_PI_2;
    auto direction = getRandom(0, M_PI * 2);

    // # Add some randomness to the direction.
    // direction += rand_range(-PI / 4, PI / 4)
    // mob.rotation = direction
    //direction += getRandom(-M_PI_4, M_PI_4);
    newMob->set_rotation(-direction);
    float l = getRandom(10000, 30000);

    auto y = newMob->get_position().y - sin(direction) * l;
    auto x = newMob->get_position().x + cos(direction) * l;
    
    //auto x = newMob->get_position().x - getRandom(-10510, -10010);
    //auto y = newMob->get_position().y - getRandom(-10100, -10050); 
    cast_to<Mob>(newMob)->setTarget({static_cast<float_t>(x), static_cast<float_t>(y)});
    // # Choose the velocity.
    // mob.set_linear_velocity(Vector2(rand_range(mob.min_speed, mob.max_speed), 0).rotated(direction))
    //newMob->set_linear_velocity(Vector2(getRandom(150, 250), 0).rotated(direction));
  }

  void Main::startNewGame() {
    purgeMobs();
    
    auto startPos = cast_to<Position2D>(get_node("StartPosition"))->get_position();
    cast_to<Player>(get_node("Player"))->start(startPos);
    
    auto touchPad = cast_to<Touchpad>(get_node("Touchpad"));
    touchPad->set_deferred("disabled", false);
    touchPad->set_visible(true);

    score = 0;
    hud->showScore(score);
  
    hud->showMessage("Get Ready!");
    cast_to<Timer>(get_node("StartTimer"))->start();
  }

  float Main::getRandom(float min, float max) {
    std::uniform_real_distribution<float> dist(min, max);
    return dist(rng);
  }

  void Main::purgeMobs() {
    auto mobs = get_node("MobHolder");
    for(auto i = 0; i < mobs->get_child_count(); i++) {
      mobs->get_child(i)->queue_free();
    }
  }
}
