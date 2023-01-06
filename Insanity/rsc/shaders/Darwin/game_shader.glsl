#version 330

// Input vertex attributes (from vertex shader)
in vec2 fragTexCoord;
in vec4 fragColor;

// Input uniform values
uniform sampler2D texture0;
uniform vec4 colDiffuse;
uniform float uTime = 0.0;

// Output fragment color
out vec4 finalColor;

// NOTE: Add here your custom variables

// NOTE: Render size values must be passed from code
uniform int renderWidth;
uniform int renderHeight;

float rand(vec2 co){
    return fract(sin(dot(co, vec2(12.9898, 78.233))) * 43758.5453);
}

#define PI 3.141592653589793238462643383279
#define TAU PI*2.

#define WAVE_FREQUENCY TAU*1200.
#define WAVE_WARP_MAG 0.001
#define WAVE_SPEED 30

#define DISCONTINUITY_SPEED 0.9
#define DISCONTINUITY_SIZE 0.1

#define GREEN_SEP 0.002
#define BLUE_SEP 0.0025

#define RED 0.98
#define GREEN 0.92
#define BLUE 1.08

float add_wave(float y, float yfreq, float speed, float edge, float size) {
    float wave = sin(y * TAU * yfreq + uTime * speed) * .5 + edge;
    return (1.-(floor(wave)+1.)) * size;
}

void main()
{
    // finalColor = vec4(fragTexCoord.x, fragTexCoord.y, 0., 1.);
    vec2 indexcoord = fragTexCoord;
    indexcoord.y += sin(fragTexCoord.x * WAVE_FREQUENCY + uTime * WAVE_SPEED) * WAVE_WARP_MAG;

    // Layer some scan lines
    indexcoord.x += add_wave(indexcoord.y, 1., DISCONTINUITY_SPEED, 0.499995, DISCONTINUITY_SIZE);
    indexcoord.x += add_wave(indexcoord.y, 2.3, DISCONTINUITY_SPEED * 0.9, 0.4999995, DISCONTINUITY_SIZE * -0.8);
    indexcoord.x += add_wave(indexcoord.y, 12.0, DISCONTINUITY_SPEED * 3.9, 0.49999999999995, DISCONTINUITY_SIZE * -0.8);

    // Separate out the colors slightly
    vec3 color = vec3(
        RED * texture(texture0, indexcoord).r,
        GREEN * texture(texture0, indexcoord+vec2(0, GREEN_SEP)).g,
        BLUE * texture(texture0, indexcoord+vec2(BLUE_SEP, 0.)).b
    );

    // vec3 color = texture(texture0, indexcoord).rgb;
    // float mag = dot(color, color);
    // mag = smoothstep(0.2, 0.7, mag);
    // float mag = color.x + color.y + color.z;
    // mag = mag / 3.;

    // Scan lines
    // float y = fragTexCoord.y * renderHeight + fragTexCoord.x * renderWidth * 0.01 + uTime * 2. + rand(fragTexCoord) * 0.1;
    // float isodd = floor(mod((y), 13.) / 11.);
    // mag = mag * (1 - 0.02 * isodd);

    finalColor = vec4(color.rgb, 1.);
    // finalColor = vec4(texture(texture0, fragTexCoord), 1.);
}