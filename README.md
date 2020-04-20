# cpp_profiler
C++ profiler utility to profile code with the chrome tracing tools. Based on TheCherno's tutorial.

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

## Usage
The profiling library makes use of macros to only be defined in case the `PROFILING` variable is defined.
To profile a function just use the following macros:

```lang=cpp
void function() {
  PROFILE_FUNCTION();

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

