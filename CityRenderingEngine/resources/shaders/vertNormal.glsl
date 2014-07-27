# version 330 core

uniform mat4 modelMatrix;
uniform mat4 viewMatrix;
uniform mat4 projMatrix;
uniform float time;


in vec3 position;
in vec4 colour;
in vec2 uv_map;
in vec3 normal;

out Vertex {
	vec3 worldPos;
    vec3 worldNormal;
	vec2 uv_map;
	vec4 colour;
	vec3 normal;
    vec3 cameraPos;
    vec4 viewSpace;
} OUT;

void main(void) {
    mat3 modelMat3 = mat3(modelMatrix);
	OUT.worldPos = modelMat3 * position;
    OUT.worldPos = normalize(modelMat3 * normal);
    OUT.viewSpace = viewMatrix * modelMatrix * vec4(position, 1);
    
	gl_Position = projMatrix * OUT.viewSpace;

	OUT.uv_map = uv_map;
	OUT.colour = colour;

	//mat3 normalMatrix = transpose(inverse(mat3(modelMatrix)));

    OUT.normal = normal;
    OUT.cameraPos = -(viewMatrix[3].xyz) * mat3(viewMatrix);
}