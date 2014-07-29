# version 330 core

uniform samplerCube cubeTex;
uniform vec3 cameraPos;

in Vertex {
    vec3 normal;
} IN;

out vec4 gl_FragColor;

void main(void) {
    vec3 normalized = (normalize(IN.normal / 2.0)) + 0.5;
    float yFactor = smoothstep(-0.4, 0.2, IN.normal.y) / 2;
    gl_FragColor = vec4(0.95 - yFactor, 1.1 - yFactor, 0.8, 1.0);
    
    //gl_FragColor = texture(cubeTex, normalize(IN.normal));
    //gl_FragColor = vec4(yFactor, yFactor, yFactor, 1.0);
    //gl_FragColor = vec4(((normalize(IN.normal / 2.0)) + 0.5), 1);
}