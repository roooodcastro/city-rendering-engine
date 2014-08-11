# version 330 core

uniform samplerCube cubeTex;
uniform vec3 cameraPos;

in Vertex {
    vec3 normal;
} IN;

out vec4 gl_FragColor;

void main(void) {
    // Calculates both sun and sky colour
    vec3 sunColour = vec3(0.95, 0.9, 0.70);
    vec3 sunPosition = vec3(0.2, 0.5, 0.1);
    vec3 normalized = (normalize(IN.normal / 2.0)) + 0.5;
    float yFactor = smoothstep(-0.4, 0.2, IN.normal.y) / 2;
    vec3 skyColour = vec3(0.95 - yFactor, 1.1 - yFactor, 0.8);
    float length = length(IN.normal - sunPosition);
    length /= 2.0;
    length = sqrt(length);
    length = smoothstep(0.14, 0.4, length);
    if (length <= 0.03) {
        length *= length;
        sunColour.z -= 0.05;
    }
    // Mix sun and sky colour accordingly
    gl_FragColor = vec4(mix(sunColour, skyColour, length), 1.0);
    
    //gl_FragColor = texture(cubeTex, normalize(IN.normal));
    //gl_FragColor = vec4(yFactor, yFactor, yFactor, 1.0);
    //gl_FragColor = vec4(((normalize(IN.normal / 2.0)) + 0.5), 1);
}