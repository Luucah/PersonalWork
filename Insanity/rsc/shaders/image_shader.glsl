#version 330

// Input vertex attributes (from vertex shader)
in vec2 fragTexCoord;
in vec4 fragColor;

// Input uniform values
uniform sampler2D texture0;
uniform vec4 colDiffuse;
uniform float BW_PERCENTAGE = 0.;
uniform float INVERT = 0.;

// Output fragment color
out vec4 finalColor;

#define PI 3.141592653589793238462643383279
#define TAU PI*2.

#define WAVE_FREQUENCY TAU*120.
#define WAVE_WARP_MAG 0.0002
#define WAVE_SPEED 30

#define FLICKER_SPEED 30 / 1200
#define FLICKER_SIZE .02

#define DISCONTINUITY_SPEED 0.9
#define DISCONTINUITY_SIZE 0.1

#define GREEN_SEP 0.001
#define BLUE_SEP 0.0015

#define RED 0.98
#define GREEN 0.92
#define BLUE 1.08

void main()
{
    vec4 color = texture(texture0, fragTexCoord);

    float bw = color.r + color.g + color.b;
    bw = bw / 3.;

    vec3 bwcolor = vec3(
        mix(color.r, bw, BW_PERCENTAGE),
        mix(color.g, bw, BW_PERCENTAGE),
        mix(color.b, bw, BW_PERCENTAGE)
    );

    bwcolor = (vec3(1.,1.,1.) - bwcolor) * (INVERT) + bwcolor * (1-INVERT);

    finalColor = vec4(bwcolor.rgb, color.a);
}