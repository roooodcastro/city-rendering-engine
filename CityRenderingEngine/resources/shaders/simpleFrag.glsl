# version 150 core

uniform sampler2D texture0;
uniform float time;

in Vertex {
	smooth vec4 colour;
	smooth vec2 uv_map;
} IN;

out vec4 gl_FragColor;

void main(void) {
    gl_FragColor = vec4(time,1,0,1);
}