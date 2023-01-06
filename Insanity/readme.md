# Insanity
Dumb game haha

# Conventions
For include guards, I'm using the following convention:

```
<project>_<file>_<extension>_INCLUDED
example:
include/aimages.hpp
INSANITY_AIMAGES_HPP_INCLUDED
```

This is the Boost naming convention. 

# Building
To build the project, run the following commands:

```
mkdir build
cd build
cmake ..
cmake --build .
```

There should now be a `Insanity` executable file in `build/bin`. Run this file from `build` as `./bin/Insanity` or else it will fail to find the resource folder. 

If that's way too much work for you, write `ln -s ./bin/Insanity i` and you can run it using `./i` from then on you lazy bastard.

## Ubuntu Issues

If cmake complains about missing dependencies when you try an build, attempt to run the following command:

```
sudo apt install libasound2-dev mesa-common-dev libx11-dev libxrandr-dev libxi-dev xorg-dev libgl1-mesa-dev libglu1-mesa-dev
```

This will download and install all of the raylib dependencies if they haven't been already. Barebones ubuntu does not include some of these packages.

## Windows Information
If you are using Windows the executable was likely built to `build/bin/Debug/Insanity.exe`. There is a cmake command to generate a hard link to `build/Insanity.exe`. Do not try to run any other `Insanity.exe` as it will fail to find the resource directory and you will cry. You must move it to `build` or run the hard-linked file that exists there. Anything else will result in unhappiness. Because windows results in unhappiness.

### Note:
I have no idea what happens if you try generating a release candidate on windows. It probably won't work as the vc compiler will place the executable somewhere else. We will burn this bridge when it is needed.

## Extra build information
By default, Insanity will create a `Debug` build. If you wish to create a `Release` build, run `cmake -DCMAKE_BUILD_TYPE=Release ..` instead of the normal `cmake ..` command. 
Note that you will then have to reset `-DCMAKE_BUILD_TYPE` to `Debug` or an empty string ("`-DCMAKE_BUILD_TYPE=`") to make a debug build again.

During `Debug` builds, so long as you are not on windows (fuck windows), cmake will create a symlink between the build directory and the repository `rsc` folder. This means that any changes to the repository resources files, such as the `.lua` files, will be realized immediately on program restart. 
Note that for Windows builds, `cmake ..` needs to be re-run every time a resource file changes. This means that developing on windows sucks and you should use a unix-based os.

# Save state:
Insanity has the concept of a save state. See the file `include/savestate.hpp` for the technical contract, and `src/savestate.cpp` for the implementation details. I will give a high level here.

A save state contains the following information:
- The current level (as an unsigned integer beginning at 1)
- The creepy flags (as a boolean flag)
- The 'fear factor' (as a floating point value between 0 and 1)

## Lua API
To interface with the save states, the following bindings have been created:

todo:: finish this section

# Lua API Details
oh yeah baby. here we go.

Save file will store creepy flags that are set to true, what stage the player is on, and the `fear_factor`, and Lua will be able to retrieve this information as needed. 

## Questions:

Aidan's questions about the stuff written above:

- How many creepy flags are there? I have it set to 7 for now but let me know if that should change. This cannot change often as it will invalidate any prior save states.
-- I really dont know, it depends how we wind up fully fleshing out the game, and how many weird things we decide to do
- 

# Extra

- Maybe look at SWIG in the future? http://www.swig.org/Doc1.3/SWIGDocumentation.html
- Here are some resources on git:
```
general:
good common command reference: https://medium.com/@taylorwan/git-for-dummies-63d0c85a239

branches:
short youtube video: https://www.youtube.com/watch?v=JTE2Fn_sCZs
good practical tutorial: https://linuxconfig.org/git-branching-tutorial-for-beginners
technical explanation: https://git-scm.com/book/en/v2/Git-Branching-Branches-in-a-Nutshell
```
