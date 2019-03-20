# Dode the Creeps

Second attempt at following "Your first game" tutorial over at [Godot Docs](https://docs.godotengine.org/en/3.1/getting_started/step_by_step/your_first_game.html). All source code converted into C++.

As I stated in my [first attemt](https://github.com/frklan/DodgeTheCreeps) at following the tutorial, I'd like to code in C++. Not that C++ is better, just that I do like it and want to practise.

## Compiling

Make sure ````CXX```` and ````CC```` environment variables points to valid compilers.

### Godot-cpp

````
cd src/godot-cpp
scons platform=<os> bits=64 generate_bindings=yes
````

### Project C++ source

````
scons platform=<os> target=debug
````

### Godot game

Open the project in Godot and run the scene.

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
