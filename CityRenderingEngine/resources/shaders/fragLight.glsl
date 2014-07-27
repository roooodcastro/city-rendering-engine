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
    vec3 worldNormal;
	vec4 colour;
	vec2 uv_map;
	vec3 normal;
    vec3 cameraPos;
    vec4 viewSpace;
} IN;

out vec4 gl_FragColor;

const vec3 fogColour = vec3(0.0, 0.0, 0.0);
const float fogDensity = 0.05;

void main(void) {
	vec3 finalColour = vec3(0, 0, 0);
    vec3 finalColourGamma = vec3(0, 0, 0);
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

            // Calculate ambient, diffuse and specular lighting
            vec3 texLightCol = texCol.rgb * lightSource.colour;
            vec3 ambient = texLightCol * 0.1;
            vec3 diffuse = texLightCol * lambert * lightSource.intensity;
            vec3 specular = texLightCol * sFactor * lightSource.intensity;
            
            // Calculate rim lighting
            float gamma = 1.0/0.7;
            vec3 rimColour = vec3(0.15, 0.15, 0.15) + (vec3(0.05, 0.05, 0.05) * texLightCol);
            float rim = 1.0 - max(dot(viewDir, IN.worldNormal), 0.0);
            rim = smoothstep(0.6, 1.0, rim);
            vec3 finalRim = rimColour * vec3(rim, rim, rim);
            
            // Calculate fog
            //float dist = abs(IN.viewSpace.z);
            float dist = length(IN.viewSpace);
            float heightFactor = 0.5 + clamp(IN.cameraPos.y / 3000.0, 0.0, 1.0);
            float fogMinDist = 3000 * heightFactor;
            float fogMaxDist = 4000 * heightFactor;
            float fogFactor = (fogMaxDist - dist) / (fogMaxDist - fogMinDist);
            fogFactor = clamp(fogFactor, 0.0, 1.0);
            
            finalColour = vec3(ambient + diffuse + finalRim);
            finalColourGamma = vec3(pow(finalColour.r, gamma), pow(finalColour.g, gamma), pow(finalColour.b, gamma));
            
            finalColourGamma = mix(fogColour, finalColourGamma, fogFactor);
        }
	}
	gl_FragColor = vec4(finalColourGamma, texCol.w);
}