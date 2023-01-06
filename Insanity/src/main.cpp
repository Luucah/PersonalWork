
// My imports
#include <aimages.hpp>
#include <aluautil.hpp>
#include <asounds.hpp>
// Including our generated file with version information!
#include <insanity_config.h>

#include <testlib.hpp>

// Our libraries. These are probably already
// included, but better safe than sorry
#include <lauxlib.h>
#include <lua.h>
#include <lualib.h>
#include <raylib.h>

// Other includes
#include <cassert>
#include <cmath>
#include <filesystem>
#include <iostream>

#define DEBUG

#define FPS 60
#define GLSL_VERSION 330
#define GENERAL_SHADER_LOCATION "rsc/shaders/" PLATFORM_STRING "/game_shader.glsl"
#define IMAGE_SHADER_LOCATION "rsc/shaders/" PLATFORM_STRING "/image_shader.glsl"

#define CANVAS_W 1200
#define CANVAS_H 900

#define FONT_SIZE 120
#define PARAGRAPH_SIZE 60
#define LINE_HEIGHT 46
#define IMAGE_HEIGHT 400
#define IMAGE_WIDTH 400
#define MARGIN 120
#define BACKGROUND_COLOR CLITERAL(Color){20,20,20,255}

#define CHECK_QUIT()                                                           \
    if (WindowShouldClose()) {                                                 \
        teardown();                                                            \
        exit(-1);                                                              \
    }

#define CHECK_RESIZE()                                                         \
    if (IsWindowResized()) {                                                   \
        screen_width = GetScreenWidth();                                       \
        screen_height = GetScreenHeight();                                     \
        target_width = ((double)screen_height / CANVAS_H) * CANVAS_W;          \
    }

#ifdef DEBUG
#define HOTRELOAD()                                                            \
    if (GetKeyPressed() == KEY_R) {                                            \
        UnloadShader(effect_shader);                                           \
        UnloadShader(image_shader);                                           \
        loadShader();                                                          \
    }
#else
#define HOTRELOAD() ;
#endif

int screen_width = 1200;
int screen_height = 900;
int target_width = ((double)screen_height / CANVAS_H) * CANVAS_W;

// We do have some globals. I think this is OK.

// We have our two shaders, one for the general
// frame-by-frame effects and one for the images
// when they are drawn to the internal canvas.
Shader effect_shader;
Shader image_shader;

// We have said canvas. We use this to optimize our rendering
// process.
RenderTexture2D target;

// We have our font. This should probably go.
// todo:: what to do with global font.
Font custom_font;

// Sound manager!
ASoundManager sound_manager;

// Buffer to keep track of images to unload
// after the frame is done drawing.
UNLOAD_BUFFER unload_buffer;

// Location for the effect shader's "uTime" uniform.
int timeLoc;

void loadShader() {
    effect_shader =
        LoadShader(0, TextFormat(GENERAL_SHADER_LOCATION, GLSL_VERSION));
    timeLoc = GetShaderLocation(effect_shader, "uTime");

    image_shader =
        LoadShader(0, TextFormat(IMAGE_SHADER_LOCATION, GLSL_VERSION));

    // Need to set the render width and height
    int widthLoc = GetShaderLocation(effect_shader, "renderWidth");
    int heightLoc = GetShaderLocation(effect_shader, "renderHeight");

    SetShaderValue(effect_shader, widthLoc, &(screen_width),
                   SHADER_UNIFORM_INT);
    SetShaderValue(effect_shader, heightLoc, &(screen_height),
                   SHADER_UNIFORM_INT);
}

void init() {
    SetTraceLogLevel(LOG_ERROR); // todo:: remove?
    InitWindow(screen_width, screen_height, "Insanity");
    SetWindowState(FLAG_WINDOW_RESIZABLE);
    InitAudioDevice();
    SetMasterVolume(2.0);
    SetWindowPosition(0, 0);

    SetTargetFPS(FPS);
    // Canvas is made the same size regardless of the
    // size of the actual display.
    target = LoadRenderTexture(CANVAS_W, CANVAS_H);
    loadShader();

    // We're going to automatically play this sound on loop
    sound_manager.play_repeating("rsc/sounds/background-static.wav");

    custom_font = LoadFont("rsc/fonts/BakbakOne-Regular.ttf");
}

void draw() {
    float time = (float)GetTime();
    SetShaderValue(effect_shader, timeLoc, &time, SHADER_UNIFORM_FLOAT);
    BeginDrawing();
    ClearBackground(BLACK);
    BeginShaderMode(effect_shader);
    const int x = (screen_width - target_width) / 2;
    DrawTexturePro(
        target.texture, Rectangle{0, 0, CANVAS_W, -CANVAS_H},
        Rectangle{(float)x, 0, (float)target_width, (float)screen_height},
        Vector2{0, 0}, 0., RAYWHITE);
    EndShaderMode();
#ifdef DEBUG
    DrawFPS(0, 20);
#endif
    EndDrawing();

    // Update our sound manager so it can
    // unload and replay stuff
    sound_manager.update();
    // Clear the image buffer if needed
    purge_buffer(unload_buffer);
}

void teardown() {
    UnloadShader(effect_shader);
    sound_manager.teardown();
    UnloadFont(custom_font);
    CloseAudioDevice();
    CloseWindow();
}

void clear() {
    printf("Clearing\n");
    BeginTextureMode(target);
    ClearBackground(BACKGROUND_COLOR);
    EndTextureMode();
}
void delay(double seconds) {
    printf("Delaying for %f seconds\n", seconds);
    int frames = seconds * FPS;
    for (int i = 0; i < frames; i++) {
        draw();
        CHECK_QUIT();
        CHECK_RESIZE();
        HOTRELOAD();
    }
}
void title(const std::string &text, int size = FONT_SIZE) {
    printf("Drawing title: %s\n", text.c_str());
    BeginTextureMode(target);
    // Figure out the width so we can center align
    float width = MeasureTextEx(custom_font, text.c_str(), size, 0).x;
    DrawTextEx(custom_font, text.c_str(),
               Vector2{(CANVAS_W - width) / 2, MARGIN}, size, 0, WHITE);
    EndTextureMode();
}
int wait_for_selection() {
    printf("Waiting for selection\n");
    for (;;) {
        // LOOK FOR MOUSE BUTTON
        if (IsMouseButtonPressed(0)) {
            Vector2 mouse_loc = GetMousePosition();
            return (int)(mouse_loc.x >= (screen_width / 2)) + 1;
        }

        CHECK_QUIT();
        CHECK_RESIZE();
        HOTRELOAD();

        // DRAW
        draw();
    }
    return 0;
}
void play_sound(const std::string &sound_name) {
    sound_manager.play_once(sound_name);
}
void play_and_wait_sound(const std::string &sound_name) {
    // This sound doesn't use the sound manager
    // as we need to explicitely wait for it
    // to be done, so we'll manage the whole lifecycle.
    Sound s = LoadSound(sound_name.c_str());
    PlaySound(s);
    while (IsSoundPlaying(s)) {
        draw();
        CHECK_QUIT();
        CHECK_RESIZE();
        HOTRELOAD();
    }
    UnloadSound(s);
}
// todo:: play_and_wait_video
void play_and_wait_video(const std::string &video_name) {
    printf(
        "[C++] WARN: play_and_wait_video needs to be implemented. Skipping.\n");
    return;
}
void set_bw_percentage(float percentage) {
    int bwploc = GetShaderLocation(image_shader, "BW_PERCENTAGE");
    SetShaderValue(image_shader, bwploc, &(percentage), SHADER_UNIFORM_FLOAT);
}
void set_invert(float invert) {
    int bwploc = GetShaderLocation(image_shader, "INVERT");
    SetShaderValue(image_shader, bwploc, &(invert), SHADER_UNIFORM_FLOAT);
}
void paragraph_line(const std::string &text, int line_number) {
    BeginTextureMode(target);
    float width = MeasureTextEx(custom_font, text.c_str(), PARAGRAPH_SIZE, 0).x;
    DrawTextEx(custom_font, text.c_str(),
               Vector2{(CANVAS_W - width) / 2,
                       (float)(MARGIN + line_number * LINE_HEIGHT)},
               PARAGRAPH_SIZE, 0, WHITE);
    EndTextureMode();
}

int bind_paragraph_line(lua_State *L) {
    assert(lua_gettop(L) == 2);
    std::string text = std::string(lua_tostring(L, 1));
    int         line_number = (int)lua_tonumber(L, 2);
    paragraph_line(text, line_number);
    return 0;
}
int bind_set_invert(lua_State *L) {
    assert(lua_gettop(L) == 1);
    float invert = (float)lua_tonumber(L, 1);
    set_invert(invert);
    return 0;
}
int bind_set_bw(lua_State *L) {
    assert(lua_gettop(L) == 1);
    float percentage = (float)lua_tonumber(L, 1);
    set_bw_percentage(percentage);
    return 0;
}
int bind_play_and_wait_video(lua_State *L) {
    assert(lua_gettop(L) == 1);
    std::string filename = std::string(lua_tostring(L, 1));
    play_and_wait_video(filename);
    return 0;
}
int bind_play_and_wait_sound(lua_State *L) {
    assert(lua_gettop(L) == 1);
    std::string filename = std::string(lua_tostring(L, 1));
    play_and_wait_sound(filename);
    return 0;
}
int bind_play_sound(lua_State *L) {
    assert(lua_gettop(L) == 1);
    std::string filename = std::string(lua_tostring(L, 1));
    play_sound(filename);
    return 0;
}

int bind_delay(lua_State *L) {
    assert(lua_gettop(L) == 1);
    double time = (double)lua_tonumber(L, 1);
    delay(time);
    return 0;
}
int bind_title(lua_State *L) {
    assert(lua_gettop(L) == 1);
    std::string str = std::string(lua_tostring(L, 1));
    title(str);
    return 0;
}
int bind_subtitle(lua_State *L) {
    assert(lua_gettop(L) == 1);
    std::string str = std::string(lua_tostring(L, 1));
    title(str, FONT_SIZE / 3);
    return 0;
}
int bind_clear(lua_State *L) {
    assert(lua_gettop(L) == 0);
    clear();
    return 0;
}

int bind_show_image_left(lua_State *L) {
    // Should have passed in the image name
    assert(lua_gettop(L) == 1);
    std::string filename = std::string(lua_tostring(L, 1));

    BeginTextureMode(target);
    BeginShaderMode(image_shader);
    draw_whole_image(filename,
                     Rectangle{MARGIN, CANVAS_H - MARGIN - IMAGE_HEIGHT,
                               IMAGE_WIDTH, IMAGE_HEIGHT},
                     Vector2{0., 0.}, 0, RAYWHITE, unload_buffer);
    EndShaderMode();
    EndTextureMode();

    return 0; // Doesn't return anything
}
int bind_show_image_right(lua_State *L) {
    // Should have passed in the image name
    assert(lua_gettop(L) == 1);
    std::string filename = std::string(lua_tostring(L, 1));

    BeginTextureMode(target);
    BeginShaderMode(image_shader);
    draw_whole_image(filename,
                     Rectangle{CANVAS_W - MARGIN - IMAGE_WIDTH,
                               CANVAS_H - MARGIN - IMAGE_HEIGHT, IMAGE_WIDTH,
                               IMAGE_HEIGHT},
                     Vector2{0., 0.}, 0, RAYWHITE, unload_buffer);
    EndShaderMode();
    EndTextureMode();

    return 0; // Doesn't return anything
}

int bind_wait_for_selection(lua_State *L) {
    assert(lua_gettop(L) == 0);
    lua_pushnumber(L, wait_for_selection());
    return 1;
}

// static const luaL_Reg mylibs[] = {
//     {"testlib", open_testlib},
//     {NULL, NULL}
// };

int main() {

    std::cout << "Insanity (" << PLATFORM_STRING << ") v" << VERSION_MAJOR << '.' << VERSION_MINOR
              << std::endl;

    lua_State *L = luaL_newstate();
    luaL_openlibs(L);

    register_binds(L);

    // register_testlib(L);
    luaL_requiref(L, "testlib", open_testlib, 0);
    lua_pop(L, 1);

    std::filesystem::path luafile("rsc/lua/game.lua");
    std::string           path = std::filesystem::absolute(luafile).string();
    printf("Going to open lua file at: %s\n", path.c_str());

    if (check_lua(L, luaL_dofile(L, path.c_str()))) {
        lua_getglobal(L, "main");
        if (lua_isfunction(L, -1)) {
            // Init the window now that we're all set up
            init();
            if (check_lua(L, lua_pcall(L, 0, 0, 0))) {
                std::cout << "Finished main" << std::endl;
            }
            teardown();
        }
    }
    return 0;
}
