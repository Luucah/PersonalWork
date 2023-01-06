#pragma once
#ifndef INSANITY_ALUA_UTIL_HPP_INCLUDED
#define INSANITY_ALUA_UTIL_HPP_INCLUDED

#include <lauxlib.h>
#include <lua.h>
#include <lualib.h>

bool check_lua(lua_State *L, int r);

int bind_delay(lua_State *);
int bind_title(lua_State *);
int bind_subtitle(lua_State *);
int bind_clear(lua_State *);
int bind_play_sound(lua_State *);
int bind_play_and_wait_sound(lua_State *);
int bind_play_and_wait_video(lua_State *);
int bind_show_image_left(lua_State *);
int bind_show_image_right(lua_State *);
int bind_wait_for_selection(lua_State *);
int bind_set_bw(lua_State *);
int bind_set_invert(lua_State *);
int bind_paragraph_line(lua_State *);

// Expects string containing the filename for the save data.
// Returns the save_state object.
int bind_load_save(lua_State *);
// Expects string containing the filename for the save data and the save_state
// object. Returns nothing.
int bind_store_save(lua_State *);
// Expects a save_state object to close. Returns nothing.
int bind_close_save(lua_State *);

int bind_save_get_level(lua_State *);
int bind_save_set_level(lua_State *);

int bind_save_get_fearfactor(lua_State *);
int bind_save_set_fearfactor(lua_State *);

int bind_save_get_flag(lua_State *);
int bind_save_set_flag(lua_State *);

void register_binds(lua_State *L);

#endif