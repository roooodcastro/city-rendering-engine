#version 330 core

uniform mat4 modelMatrix;
uniform mat4 viewMatrix;
uniform mat4 projMatrix;

in vec3 position;
in vec4 colour;
in vec2 uv_map;
in vec3 normal;

out Vertex {
	vec4 colour;
	vec2 uv_map;
} OUT;

void main(void) {
	mat4 mvp = projMatrix * viewMatrix * modelMatrix;

	gl_Position = mvp * vec4(position, 1.0);

	OUT.colour = colour;
	OUT.uv_map = uv_map;
}