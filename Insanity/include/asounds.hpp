#pragma once
#ifndef INSANITY_ASOUNDS_HPP_INCLUDED
#define INSANITY_ASOUNDS_HPP_INCLUDED

#include <raylib.h>
#include <string>
#include <unordered_map>
#include <list>

// Manages the lifetime of all sounds using raylib
class ASoundManager {
public:
    typedef unsigned int SOUND_ID;
    // Needs to be called as fast as possible to
    // ensure seamless sound
    void update();
    // Call to play a song once given a filename
    void play_once(const std::string& filename);
    // Call to play a song on loop given a filename.
    // this method returns a sound id which can later be used
    // to end the playing of this sound.
    SOUND_ID play_repeating(const std::string& filename);
    // Eventually we can add a function to stop repeating
    // todo:: add stop repeating
    void teardown();
private:
    std::unordered_map<SOUND_ID, Sound> repeating;
    std::list<Sound> playing_once;
    SOUND_ID current_id = 0;
};

#endif