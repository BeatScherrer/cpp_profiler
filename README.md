# cpp_profiler
C++ profiler utility to profile code with the chrome tracing tools. Based on [TheCherno](https://www.youtube.com/channel/UCQ-W1KE9EYfdxhL6S4twUNw)'s concept and code snippets. Many kudos to [TheCherno](https://www.youtube.com/channel/UCQ-W1KE9EYfdxhL6S4twUNw) and his quality youtube tutorials on C++.

## installation

### CMake
To build and install with cmake first clone the repo:

```lang=sh
git@github.com:BeatScherrer/cpp_profiler.git
```

cd into the repo
```lang=sh
cd /path/to/cpp_profiler
```

create a build directory and cd into it
```lang=sh
mkdir build && cd build
```

run cmake in the build directory

```lang=sh
cmake ..
```

compile the sources

```lang=sh
make
```

finally install the files

```lang=sh
sudo make install
```

### TODO add to AUR
I plan on adding the package to AUR to be installed with the AUR helper:

```lang=sh
yay cpp_profiler
```

### simple_catkin (ROS)
For use with [simple_catkin](https://github.com/catkin/catkin_simple) specify the following catkin support rep as dependency:

```lang=xml
<build_dependency>cpp_profiling_catkin</build_dependency>
```

Currently the catkin support repo needs the library to be installed on the system but I plan on adding a switch to get it from github in case it is not found.

## Usage
The profiling library makes use of macros to only be defined in case the `PROFILING` variable is defined.
To profile a function just use the following macros:

```lang=cpp
void function() {
  PROFILE_FUNCTION;

  // DO STUFF
}
```

The `PROFILE_FUNCTION()` macro calls the `PROFILE_SCOPE(name)` macro which in turns creates the following timer:

```lang=cpp
InstrumentationTimer timer##__LINE__(name)
```

The output consists of a `.json` file which can be loaded directly into the chrome tracer. Therefore go to the following url:

```
chrome://tracing
```

### Using timers


## Contribution
To contribute just use the provided `.clang-format` formatting.

## TODO

- Create a timer for multiple function calls to get statistical values
- Add support for overwrite (currently `__func__` is used to name the profile)
