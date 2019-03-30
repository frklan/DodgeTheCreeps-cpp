# Dodge the Creeps

My second attempt at following "Your first game" tutorial over at [Godot Docs](https://docs.godotengine.org/en/3.1/getting_started/step_by_step/your_first_game.html). All source code converted to C++.

In my [first attemt](https://github.com/frklan/DodgeTheCreeps) following the tutorial, I used the GDScript; Since I prefer C++, I've converted the game to C++. Overall the C++ interface works very good, althou it's a bit annoying the the engine does not support try..catch, smart pointers and other "modern" additions to the language. The Godot documentation could also be improved wtih regards to C++.

While, I'll continue to use C++ with Godot, it's most probably quite stupid to write a full game in C++; GDScript takes care of lot of stuff for you that you'll have to do manually in C++ and It's much quicker to code in. Still I prefer C++..

## Compiling for OSX

(this will most probably work for Linux and possibly even for Windows)

Make sure ```CXX``` and ```CC``` environment variables points to valid compilers.

Clone and init the repo like so:

```bash
git clone https://github.com/frklan/DodgeTheCreeps-cpp.git
cd DodgeTheCreeps-cpp
git submodule init
```

### Godot-cpp

Compile the Godot C++ bindings for our platform,

```bash
cd src/godot-cpp
scons platform=osx generate_bindings=yes
```

This only needs to be done once.

### Project C++ source

The C++ code compiles like so:

```bash
scons platform=osx target=debug
```

This step needs to be repeated every time the code is updated.

### Running the game

When the C++ bidnings and project C++ code hase been compiled, we can run the scene from within Godot by hitting ⌘+R (or ⌘+B to run the full game).

## Compiling for iOS

Compiling for iOS is slightly more involved since we need to export the project to Xcode and run in from there, anyhow the following steps works for me.

### Godot C++ bidnings

Before we compile the [Godot C++ bindings](https://github.com/GodotNativeTools/godot-cpp.git), the scons make file repo needs to be updated. Add the following function after the import statement on row 3:

```python
def sys_exec(args):
    proc = subprocess.Popen(args, stdout=subprocess.PIPE)
    (out, err) = proc.communicate()
    return out.rstrip("\r\n").lstrip()
```

(source available [here](https://friendpaste.com/CrlWiNALwdX6lWJe72eI2))

iOS and the simulator needs to be added to the platform list, it's also a good idea to set ````bits```` to 64 as default, like so (row 25 - 27):

```python
opts.Add(EnumVariable('platform', 'Target platform', host_platform,
                    allowed_values=('linux', 'osx', 'windows', 'ios', 'ios.simulator'),
                    ignorecase=2))
opts.Add(EnumVariable('bits', 'Target platform bits', '64', ('default', '32', '64')))
```

Next we need to define how to compile for iOS and the simulator (row 113

```python
elif env['platform'] == "ios":
    env["CXX"] = sys_exec(["xcrun", "-sdk", "iphoneos", "-find", "clang++"])
    env["CC"] = sys_exec(["xcrun", "-sdk", "iphoneos", "-find", "clang"])
    IOS_PLATFORM_SDK = sys_exec(["xcode-select", "-p"]) + "/Platforms"
    SDK_VERSION = sys_exec(["xcodebuild", "-sdk", "iphoneos", "-version", "|", "grep", "SDKVersion", "|", "awk", "'{print$2}'])"])
    SDK_MIN_VERSION = "10.3"   

    env.Append(CCFLAGS = ['-arch', 'arm64', '-arch', 'armv7', '-arch', 'armv7s', '-std=c++14', 
     '-isysroot', ('%s/iPhoneOS.platform/Developer/SDKs/iPhoneOS%s.sdk' % (IOS_PLATFORM_SDK, SDK_VERSION)),  
     ('-miphoneos-version-min=%s' % SDK_MIN_VERSION)])

    env.Append(LINKFLAGS = ['-arch', 'arm64', '-arch', 'armv7', '-arch', 'armv7s',
    '-isysroot', '%s/iPhoneOS.platform/Developer/SDKs/iPhoneOS%s.sdk' % (IOS_PLATFORM_SDK, SDK_VERSION) , 
    '-miphoneos-version-min=%s' % SDK_MIN_VERSION])

    if env['target'] in ('debug','d'):
      env.Append(CCFLAGS = ['-g']) 
    else:
      env.Append(CCFLAGS = ['-O3'])

elif env['platform'] == "ios.simulator":
    env["CXX"] = sys_exec(["xcrun", "-sdk", "iphoneos", "-find", "clang++"])
    env["CC"] = sys_exec(["xcrun", "-sdk", "iphoneos", "-find", "clang"])
    IOS_PLATFORM_SDK = sys_exec(["xcode-select", "-p"]) + "/Platforms"
    SDK_VERSION = sys_exec(["xcodebuild", "-sdk", "iphonesimulator", "-version", "|", "grep", "SDKVersion", "|", "awk", "'{print$2}'])"])
    SDK_MIN_VERSION = "10.3"   

    env.Append(CCFLAGS = ['-arch', 'x86_64', '-std=c++14', 
     '-isysroot', ('%s/iPhoneSimulator.platform/Developer/SDKs/iPhoneSimulator%s.sdk' % (IOS_PLATFORM_SDK, SDK_VERSION)),  
     ('-miphoneos-version-min=%s' % SDK_MIN_VERSION)])

    env.Append(LINKFLAGS = ['-arch', 'x86_64',
    '-isysroot', '%s/iPhoneSimulator.platform/Developer/SDKs/iPhoneSimulator%s.sdk' % (IOS_PLATFORM_SDK, SDK_VERSION) , 
    '-miphoneos-version-min=%s' % SDK_MIN_VERSION])

    if env['target'] in ('debug','d'):
      env.Append(CCFLAGS = ['-g']) 
    else:
      env.Append(CCFLAGS = ['-O3'])
```

**Note** The iOS SDK is hard coded to 10.3 in this example.

I also like to have my compilers defined in ````CC````and ````CXX```` hence I add these lines to SConstruct as well:

```python
# honour CC and CXX
env['CC'] = os.environ['CC']
env['CXX'] = os.environ['CXX']
```

The full SConstruct file is available [here](https://gist.github.com/frklan/3c554ef7a787dad432b1c7f7f3d72953)

Now we should be able to compile the Godot C++ bindings for iOS and the simulator

```bash
cd lib/godot-cpp

scons platform=ios generate_bindings=yes -j8
scons platform=ios.simulator generate_bindings=yes -j8
```

It's possible to combine the libraries for all architectures into one file by executing:

```bash
lipo -create bin/libgodot-cpp.ios.debug.64.a bin/libgodot-cpp.ios.simulator.debug.64.a -output bin/libgodot-cpp.fat.ios.debug.64.a
```

(this require that ```godot_cpp_library``` in SConstruct in he next step points to the fat library)

### Project C++ source

Next we need to build the project source by executing the following in the root of the repo:

```bash
scons platform=ios target=debug static=yes -j8
scons platform=ios.simulator target=debug static=yes -j8
```

### Exporting to Xcode

Follow the official Godot iOS export guide [here](https://docs.godotengine.org/en/latest/getting_started/workflow/export/exporting_for_ios.html).

**Note** that we need to add "Feature" "x86_64" in the export dialog  to get the simulator version exported.

In Xcode the following settings needs to be updated

1. Update the valid architectures to reflect if we're runing in the simualtor (x86_64) or on a real device (arm64, armv7 etc.)
2. add ```(PROJECT_DIR)/DodetheCreeps/lib/godot-cpp/bin``` and ```$(PROJECT_DIR)/DodetheCreeps/build/lib/ios``` to the Library Search Paths
3. Set development target to 10.3

If you follow the Active development considerations in the Godot documentation (which you probably should) the Library Search Paths shoule be set to ```$(PROJECT_DIR)/DodetheCreeps/lib/godot-cpp/bin``` and ```[path to repo]/build/lib/ios```

**Note that the full soruce code will be included in the app package doing this**

Currently the repo does not contain image assets as required by Xcode, hence the Images.xcassets resource must be deleted in Xcode.

## Debuging

### osx

As long as we have compiled the C++ code with debug symbols (```` scons ... target=debug ````) we can attach to the process directly from e.g. Visual Code:

1. Compile and run the game as described above
2. Hit 'F5' or click 'Debug' in Visual Code
3. Search for 'Godot' and select the entry that corresponds to the scene you're running 

### ios / ios simulator

Debugging is done using Xcode, just navigate to the source file you're intereseted in, set a breakpoint and launch the app in the simulator (or on a connected iOS device).

## TODO

- [x] compile to iOS

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
