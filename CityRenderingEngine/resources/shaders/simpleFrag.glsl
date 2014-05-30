# version 150 core

uniform sampler2D texture0;

in Vertex {
	smooth vec4 colour;
	smooth vec2 texCoord;
} IN;

out vec4 fragColour;

void main(void) {
    fragColour = vec4(1.0, 1.0, 1.0, 1.0);
}