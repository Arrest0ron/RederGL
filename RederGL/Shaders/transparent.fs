#version 330 core

uniform sampler2D depthTexture; // Depth buffer texture
uniform vec3 cameraPosition;    // Camera position in world space
uniform float waterAlpha;       // Fixed transparency value for water

in vec4 Coord;                // World-space position of the fragment
flat in uint blockID;

out vec4 FragColor;

void main() {
    // Calculate the distance from the camera to the water surface
    float distanceToCamera = length(Coord.xyz - cameraPosition);

    // Sample the depth buffer to get the depth of the background
    float backgroundDepth = texture(depthTexture, gl_FragCoord.xy / textureSize(depthTexture, 0)).r;

    // Calculate the depth difference between the water and the background
    float depthDifference = abs(gl_FragCoord.z - backgroundDepth);

    // Enforce constant transparency (ignore overlapping water blocks)
    float adjustedAlpha = waterAlpha;

    // Optionally adjust the water color based on depth difference
    vec3 waterColor = vec3(0.1, 0.5, 1.0); // Blue water color
    waterColor *= smoothstep(0.0, 0.1, depthDifference); // Darken water closer to objects

    // Output the final color
    FragColor = vec4(waterColor, adjustedAlpha);
    // FragColor = vec4(0.5f,0.5f,0.5f, 0.5f);
}