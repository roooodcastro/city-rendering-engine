# version 440 core

uniform sampler2D texture0;

// Light variables
struct LightSource {
    vec3 position;
    vec3 direction;
    vec3 colour;
    float radius;
    float intensity;
    int type; // 1 = POINT, 2 = SPOT, 3 = DIRECTIONAL
};

uniform LightSource lightSource;

uniform float time;

in Vertex {
	vec3 worldPos;
	vec4 colour;
	vec2 uv_map;
	vec3 normal;
    vec3 cameraPos;
} IN;

out vec4 gl_FragColor;

void main(void) {
	vec3 finalColour = vec3(0, 0, 0);
	vec4 texCol = texture(texture0, IN.uv_map);
    //vec4 texCol = vec4(1,1,1,1);
	
	for(int i = 0; i < 1; i++) {
        if (lightSource.type == 1) {
            // POINT LIGHT
            vec3 incident = normalize(lightSource.position - IN.worldPos);
            vec3 viewDir = normalize(IN.cameraPos - IN.worldPos);
            vec3 halfDir = normalize(incident + viewDir);

            float dist = length(lightSource.position - IN.worldPos);
            float atten = 0.8 - clamp(dist / (lightSource.radius * lightSource.radius), 0.0, 1.0);

            float lambert = max(0.0, dot(incident, IN.normal));

            float rFactor = max(0.0, dot(halfDir, IN.normal));
            float sFactor = pow(rFactor, 10.0);

            vec3 ambient = texCol.rgb * lightSource.colour * 0.05;
            vec3 diffuse = texCol.rgb * lightSource.colour * lambert * atten * lightSource.intensity;
            vec3 specular = lightSource.colour * sFactor * atten * lightSource.intensity;
            
            finalColour = vec3(ambient + diffuse + specular);
        } else if (lightSource.type == 2) {
            // SPOT LIGHT
            vec3 incident = normalize(lightSource.position - IN.worldPos);
            vec3 viewDir = normalize(IN.cameraPos - IN.worldPos);
            vec3 halfDir = normalize(incident + viewDir);

            float dist = length(lightSource.position - IN.worldPos);
            float atten = 0.8 - clamp(dist / (lightSource.radius * lightSource.radius), 0.0, 1.0);

            float lambert = max(0.0, dot(incident, IN.normal));

            float rFactor = max(0.0, dot(halfDir, IN.normal));
            float sFactor = pow(rFactor, 10.0);

            vec3 ambient = texCol.rgb * lightSource.colour * 0.05;
            vec3 diffuse = texCol.rgb * lightSource.colour * lambert * atten * lightSource.intensity;
            vec3 specular = lightSource.colour * sFactor * atten * lightSource.intensity;
            
            finalColour = vec3(ambient + diffuse + specular);
        } else {
            // DIRECTIONAL LIGHT
            vec3 incident = normalize(-lightSource.direction);
            vec3 viewDir = normalize(IN.cameraPos - IN.worldPos);
            vec3 halfDir = normalize(viewDir - incident);

            float lambert = max(0.0, dot(incident, IN.normal));

            float rFactor = max(0.0, dot(IN.normal, halfDir));
            float sFactor = pow(rFactor, 16.0);

            vec3 texLightCol = texCol.rgb * lightSource.colour;
            vec3 ambient = texLightCol * 0.05;
            vec3 diffuse = texLightCol * lambert * lightSource.intensity;
            vec3 specular = texLightCol * sFactor * lightSource.intensity;
            
            finalColour = vec3(ambient + diffuse + specular);
        }
	}
	gl_FragColor = vec4(finalColour, texCol.w);
}