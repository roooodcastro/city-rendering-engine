# version 330 core

uniform mat4 modelMatrix;
uniform mat4 viewMatrix;
uniform mat4 projMatrix;
uniform float time;

in vec3 position;
in vec4 colour;
in vec2 texCoord;

out Vertex {
	smooth vec4 colour;
	smooth vec2 texCoord;
	smooth vec3 worldPos;
} OUT;

void main(void) {
	mat4 mvp = projMatrix * viewMatrix * modelMatrix;

	gl_Position = mvp * vec4(position, 1.0);

	OUT.colour = colour;
	OUT.texCoord = texCoord;
}