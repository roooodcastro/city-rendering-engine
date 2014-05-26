# version 150 core

uniform mat4 modelMatrix;
uniform mat4 viewMatrix;
uniform mat4 projMatrix;
uniform float time;

in vec3 position;
in vec4 colour;
in vec2 uv_map;

out Vertex {
	smooth vec4 colour;
	smooth vec2 uv_map;
} OUT;

void main(void) {
	mat4 mvp = projMatrix * viewMatrix * modelMatrix;

	gl_Position = mvp * vec4(position, 1.0);

	OUT.colour = vec4(1,1,1,1);
	OUT.uv_map = uv_map;
}