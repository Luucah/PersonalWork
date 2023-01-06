
#include <savestate.hpp>
#include <aluautil.hpp>

#include <cassert>
#include <fstream>

int bind_load_save(lua_State *L) {
    save_state* save = new save_state();

    assert(lua_gettop(L) == 1);
    const char* f = lua_tostring(L, 1);
    std::ifstream file(f);

    if (file.good()) {
        // Here's where the format comes in
        file >> save->current_level;
        file >> save->fear_factor;
        for (unsigned int i = 0; i < CREEPY_FLAG_COUNT; i++)
            file >> save->creepy_flags[i];
    }
    file.close();
    // If we fail to find the file, we'll just use the defaults.

    // Now we return this save state pointer to lua
    // so they can use it later
    lua_pushlightuserdata(L, save);
    return 1;
}

int bind_store_save(lua_State *L) {
    assert(lua_gettop(L) == 2);
    const char* f = lua_tostring(L, 2);
    std::ofstream file(f);
    assert(file.good());

    void* ptr = lua_touserdata(L, 1);
    save_state* save = (save_state*)ptr;

    file << save->current_level << ' ' << save->fear_factor;
    for (unsigned int i = 0; i < CREEPY_FLAG_COUNT; i++)
        file << ' ' << save->creepy_flags[i];

    file.close();

    return 0;
}

int bind_save_get_level(lua_State *L) {
    assert(lua_gettop(L) == 1);
    void* ptr = lua_touserdata(L, 1);
    save_state* save = (save_state*)ptr;
    lua_pushnumber(L, save->current_level);
    return 1;
}

int bind_save_set_level(lua_State *L) {
    assert(lua_gettop(L) == 2);
    void* ptr = lua_touserdata(L, 1);
    save_state* save = (save_state*)ptr;
    save->current_level = (LEVEL_NUMBER)lua_tonumber(L, 2);
    return 0;
}

int bind_close_save(lua_State *L) {
    assert(lua_gettop(L) == 1);
    // Delete the pointer given to us.
    delete lua_touserdata(L, 1);
    return 0;
}

int bind_save_get_fearfactor(lua_State *L) {
    assert(lua_gettop(L) == 1);
    void* ptr = lua_touserdata(L, 1);
    save_state* save = (save_state*)ptr;
    lua_pushnumber(L, save->fear_factor);
    return 1;
}
int bind_save_set_fearfactor(lua_State *L) {
    assert(lua_gettop(L) == 2);
    void* ptr = lua_touserdata(L, 1);
    save_state* save = (save_state*)ptr;
    save->fear_factor = (double)lua_tonumber(L, 2);
    return 0;
}

int bind_save_get_flag(lua_State *L) {
    assert(lua_gettop(L) == 2);
    void* ptr = lua_touserdata(L, 1);
    int index = (unsigned int)lua_tonumber(L, 2);
    assert(index < CREEPY_FLAG_COUNT);
    save_state* save = (save_state*)ptr;
    lua_pushboolean(L, save->creepy_flags[index]);
    return 1;
}
int bind_save_set_flag(lua_State *L) {
    assert(lua_gettop(L) == 3);
    void* ptr = lua_touserdata(L, 1);
    int index = (unsigned int)lua_tonumber(L, 2);
    assert(index < CREEPY_FLAG_COUNT);
    bool value = lua_toboolean(L, 3);
    save_state* save = (save_state*)ptr;
    save->creepy_flags[index] = value;
    return 0;
}