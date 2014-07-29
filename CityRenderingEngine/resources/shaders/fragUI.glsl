# version 330 core

uniform sampler2D texture0;

in Vertex {
	vec4 colour;
	vec2 uv_map;
} IN;

out vec4 gl_FragColor;

void main(void) {
    vec4 texColour = texture(texture0, IN.uv_map);
    texColour.w = max(texColour.w, 0.3);
    if (texColour.w == 0.3)
        texColour = vec4(0, 0, 0, 0.3);
    gl_FragColor = texColour;
}