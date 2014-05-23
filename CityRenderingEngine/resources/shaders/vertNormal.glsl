# version 330 core

uniform mat4 modelMatrix;
uniform mat4 viewMatrix;
uniform mat4 projMatrix;
uniform float time;

in vec3 position;
in vec4 colour;
in vec2 texCoord;
in vec3 normal;

out Vertex {
	vec3 worldPos;
	vec2 texCoord;
	vec4 colour;
	vec3 normal;
} OUT;

void main(void) {
	vec4 worldPos = modelMatrix * vec4(position, 1.0);

	gl_Position = (projMatrix * viewMatrix) * worldPos;

	OUT.texCoord = texCoord;
	OUT.worldPos = worldPos.xyz;
	OUT.colour = colour;

	mat3 normalMatrix = transpose(inverse(mat3(modelMatrix)));

	OUT.normal = normalize(normalMatrix * normalize(normal));
}