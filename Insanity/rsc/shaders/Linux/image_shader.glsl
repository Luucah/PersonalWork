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