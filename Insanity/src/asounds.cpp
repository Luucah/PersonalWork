
#include <asounds.hpp>

void ASoundManager::update() {
    // See if any of the repeating sounds
    // need to be restarted.
    for (auto s : repeating) {
        if (!IsSoundPlaying(s.second)) {
            PlaySound(s.second);
        }
    }
    // See if any of the play once sounds are done.
    // If so, unload it, remove it, and move on.
    std::list<Sound>::iterator itr;
    for (itr = playing_once.begin(); itr != playing_once.end();) {
        if (!IsSoundPlaying(*itr)) {
            UnloadSound(*itr);
            itr = playing_once.erase(itr);
        } else
            ++itr;
    }
}

void ASoundManager::play_once(const std::string &filename) {
    Sound s = LoadSound(filename.c_str());
    PlaySound(s);
    playing_once.push_back(s);
}

typename ASoundManager::SOUND_ID
ASoundManager::play_repeating(const std::string &filename) {
    Sound s = LoadSound(filename.c_str());
    PlaySound(s);
    // Get the current id then make the next one so 
    // we can guarantee uniqueness
    SOUND_ID id = current_id++;
    // Track this pairing
    repeating.insert(std::make_pair(id, s));
    return id;
}

void ASoundManager::teardown() {
    // Stop everything!
    for (auto s : repeating) {
        UnloadSound(s.second);
    }
    for (auto s : playing_once) {
        UnloadSound(s);
    }
}
