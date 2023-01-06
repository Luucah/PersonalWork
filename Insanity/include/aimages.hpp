#pragma once
#ifndef INSANITY_AIMAGES_HPP_INCLUDED
#define INSANITY_AIMAGES_HPP_INCLUDED

#include <raylib.h>
#include <list>
#include <string>

typedef std::list<Texture2D> UNLOAD_BUFFER;

// todo:: reinvestigate this draw_whole_image function.
// todo:: is it really needed?!
void draw_whole_image(const std::string &filename, const Rectangle &dest,
                      const Vector2 &origin, float rotation,
                      const Color &tint, UNLOAD_BUFFER& unload_buffer);

// Needs to be called periodically to clear all the loaded
// images.
void purge_buffer(UNLOAD_BUFFER& unload_buffer);

#endif