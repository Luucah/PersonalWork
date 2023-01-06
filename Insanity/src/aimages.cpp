
#include <aimages.hpp>

// Loads, draws, then adds this image to the list
// of images to unload.
void draw_whole_image(const std::string &filename, const Rectangle &dest,
                      const Vector2 &origin, float rotation,
                      const Color &tint, UNLOAD_BUFFER& unload_buffer) {
    // Load image data into CPU memory
    Image image = LoadImage(filename.c_str());
    // Image converted to texture, GPU memory (RAM -> VRAM)
    Texture2D texture = LoadTextureFromImage(image);
    // Unload image data from CPU memory
    UnloadImage(image);
    // Draw the image to the target (setup before function call)
    DrawTexturePro(
        texture, Rectangle{0., 0., (float)texture.width, (float)texture.height},
        dest, origin, rotation, tint);
    // Queue the unloading of the image data from GPU memory
    unload_buffer.push_back(texture);
    return;
}

// Needs to be called periodically to clear all the loaded
// images.
void purge_buffer(UNLOAD_BUFFER& unload_buffer) {
    if (unload_buffer.empty())
        return;
    for (const Texture2D &tex : unload_buffer) {
        UnloadTexture(tex);
    }
    unload_buffer.clear();
}