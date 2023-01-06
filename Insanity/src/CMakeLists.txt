
set (INSANITY_SRCS
    "main.cpp"
    "aluautil.cpp"
    "aimages.cpp"
    "asounds.cpp"
    "savestate.cpp"
    "testlib.cpp"
    )

# Add the binary path to the include path so that files
# find our generated insanity_config.h
include_directories(
    "${PROJECT_BINARY_DIR}"
    "${PROJECT_SOURCE_DIR}/include"
    )

# -----------------
# Get our libraries

include(FetchContent)
# Get LUA
set(LUA_BUILD_AS_CXX ON CACHE BOOL "" FORCE)
FetchContent_Declare(lua GIT_REPOSITORY https://github.com/walterschell/Lua.git GIT_TAG master)
FetchContent_MakeAvailable(lua)
# Get RAYLIB
set(BUILD_EXAMPLES OFF CACHE BOOL "" FORCE) # don't build the supplied examples
set(BUILD_GAMES    OFF CACHE BOOL "" FORCE) # don't build the supplied example games
FetchContent_Declare(raylib GIT_REPOSITORY https://github.com/raysan5/raylib.git GIT_TAG master)
FetchContent_MakeAvailable(raylib)

# -----------------

# Should be done...
add_executable(Insanity ${INSANITY_SRCS})
target_link_libraries(Insanity
    # Need to link our downloaded libraries too!
    lua_static  # not called lua for some reason. See the lua-repo's 
                #   root cmake to see where this comes from
    raylib      # this one is reasonably named lol
    )


IF (WIN32) 
    # Create a shortcut (at build time) to the executable so it runs in the correct
    # directory.
    add_custom_command(TARGET Insanity POST_BUILD
        COMMAND mklink /h Insanity.exe bin\\Debug\\Insanity.exe || (exit 0)
        WORKING_DIRECTORY ${CMAKE_BINARY_DIR}
        COMMENT "Creating Insanity.exe shortcut"
    )
    # IF (CMAKE_BUILD_TYPE MATCHES Debug)
    #     add_custom_command(TARGET Insanity POST_BUILD
    #         COMMAND mklink /d .\\rsc ${CMAKE_SOURCE_DIR}/rsc || (exit 0)
    #         WORKING_DIRECTORY ${CMAKE_BINARY_DIR}
    #         COMMENT "Creating symbolic link to rsc"
    #     )
    # ELSE()
    # We also need to copy resources. 
    file(COPY ${CMAKE_SOURCE_DIR}/rsc DESTINATION ${CMAKE_BINARY_DIR})
    # ENDIF()
ELSE()
    # If we are in debug mode...
    IF (CMAKE_BUILD_TYPE MATCHES Debug)
        # We want to create a symbolic link to the rsc directory instead of copying it
        add_custom_command(TARGET Insanity POST_BUILD
            # The `|| exit 0` allows the ln command to fail without aborting the make :)
            COMMAND ln -s ${CMAKE_SOURCE_DIR}/rsc ${CMAKE_BINARY_DIR} 2> /dev/null || exit 0
            WORKING_DIRECTORY ${CMAKE_BINARY_DIR}
            COMMENT "Creating symbolic link to repository rsc directory"
        )
    ELSE()
        # Otherwise, we copy resources.
        file(COPY ${CMAKE_SOURCE_DIR}/rsc DESTINATION ${CMAKE_BINARY_DIR})
    ENDIF()
ENDIF()
