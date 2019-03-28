# Dode the Creeps

My second attempt at following "Your first game" tutorial over at [Godot Docs](https://docs.godotengine.org/en/3.1/getting_started/step_by_step/your_first_game.html). All source code converted to C++.

In my [first attemt](https://github.com/frklan/DodgeTheCreeps) following the tutorial, I used the GDScript; Since I prefer C++, I've converted the game to C++. Overall the C++ interface works very good, althou it's a bit annoying the the engine does not support try..catch, smart pointers and other "modern" additions to the language. The Godot documentation could also be improved wtih regards to C++.

While, I'll continue to use C++ with Godot, it's most probably quite stupid to write a full game in C++; GDScript takes care of lot of stuff for you that you'll have to do manually in C++ and It's much quicker to code in. Still I prefer C++..

## Compiling

Make sure ````CXX```` and ````CC```` environment variables points to valid compilers.

### Godot-cpp

First we'll need to compile the Godot C++ bindings for our platform,

````
git submodule init
cd src/godot-cpp
scons platform=<os> bits=32|64 generate_bindings=yes
````

This only needs to be done once.

### Project C++ source

The C++ code compiles like so:

````
scons platform=<os> target=debug
````

### Godot game

When the C++ bidnings and project C++ code hase been compiled, we can run the scene from within Godot.

## Debuging

As long as we have compiled the C++ code with debug symbols (```` scons ... target=debug ````) we can attach to the process directly from e.g. Visual Code:

  1. Compile and run the game as described above
  2. Hit 'F5' or click 'Debug' in Visual Code
  3. Search for 'Godot' and select the entry that corresponds to the scene you're running 

## TODO
- [ ] compile to iOS

## Contributing

I don't expect to updated this project, still contributions are always welcome!

When contributing to this repository, please first discuss the change you wish to make via the issue tracker, email, or any other method with the owner of this repository before making a change.

Please note that we have a code of conduct, you are required to follow it in all your interactions with the project.

## Versioning

We use [SemVer](http://semver.org/) for versioning. For the versions available, see the [tags on this repository](https://github.com/frklan/GameEngine/tags).

## Authors

* **Fredrik Andersson** - *Initial work* - [frklan](https://github.com/frklan)

## License

This project is licensed under the MIT License - see the [LICENSE](LICENSE) file for details
