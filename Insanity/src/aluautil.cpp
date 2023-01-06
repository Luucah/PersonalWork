
#include <aluautil.hpp>
#include <string>
#include <iostream>

bool check_lua(lua_State *L, int r) {
    if (r != LUA_OK) {
        std::string error = lua_tostring(L, -1);
        std::cout << "LUA ERROR: " << error << std::endl;
        return false;
    }
    return true;
}

// Register all the lua binds in one place.
void register_binds(lua_State *L) {
    lua_register(L, "_delay", bind_delay);
    lua_register(L, "_title", bind_title);
    lua_register(L, "_subtitle", bind_subtitle);
    lua_register(L, "_clear", bind_clear);
    lua_register(L, "_play_sound", bind_play_sound);
    lua_register(L, "_play_and_wait_sound", bind_play_and_wait_sound);
    lua_register(L, "_play_and_wait_video", bind_play_and_wait_video);
    lua_register(L, "_show_image_left", bind_show_image_left);
    lua_register(L, "_show_image_right", bind_show_image_right);
    lua_register(L, "_wait_for_selection", bind_wait_for_selection);
    lua_register(L, "_set_bw", bind_set_bw);
    lua_register(L, "_set_invert", bind_set_invert);
    lua_register(L, "_add_line", bind_paragraph_line);

    // Save state manipulation
    lua_register(L, "_load_save", bind_load_save);
    lua_register(L, "_store_save", bind_store_save);
    lua_register(L, "_close_save", bind_close_save);

    // Save state accessors
    lua_register(L, "_get_level", bind_save_get_level);
    lua_register(L, "_set_level", bind_save_set_level);
    lua_register(L, "_get_creepy_flag", bind_save_get_flag);
    lua_register(L, "_set_creepy_flag", bind_save_set_flag);
    lua_register(L, "_get_fearfactor", bind_save_get_fearfactor);
    lua_register(L, "_set_fearfactor", bind_save_set_fearfactor);
}
