#!python
import os, subprocess

def sys_exec(args):
    proc = subprocess.Popen(args, stdout=subprocess.PIPE)
    (out, err) = proc.communicate()
    return out.rstrip("\r\n").lstrip()

opts = Variables([], ARGUMENTS)

# Gets the standard flags CC, CCX, etc.
env = DefaultEnvironment()

# Generates help for the -h scons option.
Help(opts.GenerateHelpText(env))

# Define our options
opts.Add(EnumVariable('target', "Compilation target", 'debug', ['debug', 'release']))
opts.Add(EnumVariable('platform', "Compilation platform", '', ['', 'windows', 'x11', 'linux', 'osx', 'ios', 'ios.simulator']))
opts.Add(PathVariable('target_path', 'The path where the lib is installed.', 'build/lib/'))
opts.Add(PathVariable('target_name', 'The library name.', 'libdodge', PathVariable.PathAccept))
opts.Add(EnumVariable('static', "static lib", 'no', ['no', 'yes']))
opts.Add(PathVariable('godot_cpp_bindings_path', 'Path to Godot CPP', 'lib/godot-cpp/'))
opts.Add(PathVariable('godot_cpp_library', 'Godot CPP library name', 'libgodot-cpp', PathVariable.PathAccept))

# Updates the environment with the option variables.
opts.Update(env)

# Local dependency paths, adapt them to your setup
env['godot_headers_path'] = env['godot_cpp_bindings_path'] + 'godot_headers/'

# honour CC and CXX
env['CC'] = os.environ['CC']
env['CXX'] = os.environ['CXX']

# only support 64 at this time..
bits = 64

if env['platform'] == '':
    print("No valid target platform selected.")
    quit();

# Check our platform specifics
if env['platform'] == "osx":
    env['target_path'] += 'osx/'
    env['godot_cpp_library'] += '.osx'
    if env['target'] in ('debug', 'd'): 
        env.Append(CCFLAGS = ['-g', '-arch', 'x86_64', '-std=c++17'])
        env.Append(LINKFLAGS = ['-arch', 'x86_64'])
    else:
        env.Append(CCFLAGS = ['-g','-O3', '-arch', 'x86_64', '-std=c++17'])
        env.Append(LINKFLAGS = ['-arch', 'x86_64'])

elif env['platform'] == "ios":
    env['target_path'] += 'ios/'
    env['godot_cpp_library'] += '.ios'
    IOS_PLATFORM_SDK = sys_exec(["xcode-select", "-p"]) + "/Platforms"
    SDK_VERSION = sys_exec(["xcodebuild", "-sdk", "iphoneos", "-version", "|", "grep", "SDKVersion", "|", "awk", "'{print$2}'])"])
    SDK_MIN_VERSION = "10.3"   


    env.Append(CCFLAGS = ['-arch', 'arm64', '-arch', 'armv7', '-arch', 'armv7s', '-std=c++17', 
     '-isysroot', ('%s/iPhoneOS.platform/Developer/SDKs/iPhoneOS%s.sdk' % (IOS_PLATFORM_SDK, SDK_VERSION)),  
     ('-miphoneos-version-min=%s' % SDK_MIN_VERSION)])
    
    env.Append(LINKFLAGS = ['-arch', 'arm64', '-arch', 'armv7', '-arch', 'armv7s',
    '-isysroot', '%s/iPhoneOS.platform/Developer/SDKs/iPhoneOS%s.sdk' % (IOS_PLATFORM_SDK, SDK_VERSION) , 
    '-miphoneos-version-min=%s' % SDK_MIN_VERSION])


    if env['target'] in ('debug'):
      env.Append(CCFLAGS = ['-g']) 
    else:
      env.Append(CCFLAGS = ['-O3'])

elif env['platform'] == "ios.simulator":
    env['static'] = 'yes'
    env['target_path'] += 'ios/'
    env['godot_cpp_library'] += '.ios.simulator'
    
    IOS_PLATFORM_SDK = sys_exec(["xcode-select", "-p"]) + "/Platforms"
    SDK_VERSION = sys_exec(["xcodebuild", "-sdk", "iphonesimulator", "-version", "|", "grep", "SDKVersion", "|", "awk", "'{print$2}'])"])
    SDK_MIN_VERSION = "10.3"   


    env.Append(CCFLAGS = ['-arch', 'x86_64', '-std=c++17', 
     '-isysroot', ('%s/iPhoneSimulator.platform/Developer/SDKs/iPhoneSimulator%s.sdk' % (IOS_PLATFORM_SDK, SDK_VERSION)),  
     ('-miphoneos-version-min=%s' % SDK_MIN_VERSION)])
    
    env.Append(LINKFLAGS = ['-arch', 'x86_64',
    '-isysroot', '%s/iPhoneSimulator.platform/Developer/SDKs/iPhoneSimulator%s.sdk' % (IOS_PLATFORM_SDK, SDK_VERSION) , 
    '-miphoneos-version-min=%s' % SDK_MIN_VERSION])
    

    if env['target'] in ('debug'):
      env.Append(CCFLAGS = ['-g']) 
    else:
      env.Append(CCFLAGS = ['-O3'])

elif env['platform'] in ('x11', 'linux'):
    env['target_path'] += 'x11/'
    env['godot_cpp_library'] += '.linux'
    if env['target'] in ('debug'):
        env.Append(CCFLAGS = ['-fPIC', '-g3','-Og', '-std=c++17'])
    else:
        env.Append(CCFLAGS = ['-fPIC', '-g','-O3', '-std=c++17'])

elif env['platform'] == "windows":
    env['target_path'] += 'win64/'
    env['godot_cpp_library'] += '.windows'
    # This makes sure to keep the session environment variables on windows,
    # that way you can run scons in a vs 2017 prompt and it will find all the required tools
    env.Append(ENV = os.environ)

    env.Append(CCFLAGS = ['-DWIN32', '-D_WIN32', '-D_WINDOWS', '-W3', '-GR', '-D_CRT_SECURE_NO_WARNINGS'])
    if env['target'] in ('debug'):
        env.Append(CCFLAGS = ['-EHsc', '-D_DEBUG', '-MDd'])
    else:
        env.Append(CCFLAGS = ['-O2', '-EHsc', '-DNDEBUG', '-MD'])



env['godot_cpp_library'] += '.' + env['target']
env['godot_cpp_library'] += '.' + str(bits)
env['target_name'] += '.' + env['platform'] + '.' + str(bits)

# make sure our binding library is properly includes
env.Append(CPPPATH=['.', env['godot_headers_path'], env['godot_cpp_bindings_path'] + 'include/', env['godot_cpp_bindings_path'] + 'include/core/', env['godot_cpp_bindings_path'] + 'include/gen/'])
env.Append(LIBPATH=[env['godot_cpp_bindings_path'] + 'bin/'])
env.Append(LIBS=[env['godot_cpp_library']])

# tweak this if you want to use different folders, or more folders, to store your source code in.
env.Append(CPPPATH=['godot/src/'])
sources = Glob('src/*.cpp')
sources += Glob('src/scenes/*.cpp')

if env['static'] == "yes":
    library = env.StaticLibrary(target=env['target_path'] + env['target_name'] , source=sources)
else:
    library = env.SharedLibrary(target=env['target_path'] + env['target_name'] , source=sources)

Default(library)

