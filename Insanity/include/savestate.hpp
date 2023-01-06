#pragma once
#ifndef INSANITY_SAVESTATE_HPP_INCLUDED
#define INSANITY_SAVESTATE_HPP_INCLUDED

#include <string>

#define CREEPY_FLAG_COUNT 7

typedef unsigned int LEVEL_NUMBER;

struct save_state {
    // Values
    LEVEL_NUMBER current_level;
    double       fear_factor;
    bool         creepy_flags[CREEPY_FLAG_COUNT];

    // Default constructor, sets everything to
    // initial state, perfect for creating the initial
    // state if a save file is not found.
    save_state() : current_level(1), fear_factor(0.) {
        for (unsigned int i = 0; i < CREEPY_FLAG_COUNT; i++)
            creepy_flags[i] = false;
    }

    bool         get_flag(unsigned int flag) { return creepy_flags[flag]; }
    LEVEL_NUMBER get_level() { return current_level; }
    double       get_fear_factor() { return fear_factor; }

    void set_level(LEVEL_NUMBER level) { current_level = level; }
    void set_fear_factor(double ff) { fear_factor = ff; }
    void set_flag(unsigned int flag, bool state) { creepy_flags[flag] = state; }
};

#endif
