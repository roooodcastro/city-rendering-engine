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
uniform int numFloors;

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

const vec4 fogColour = vec4(0.0, 0.0, 0.0, 0.0);
const float fogDensity = 0.05;

void main(void) {
    // Calculate correct uv_map
    //numFloors = 7;
    vec2 correctUv = IN.uv_map;
    float uvY = IN.uv_map.y;
    if (numFloors >= 3) {
        if (uvY <= 1) {
            // Top floor
            correctUv.y = (4.0 / 7.0) + fract(correctUv.y) / 7.0;
        } else if (uvY <= (numFloors - 1)) {
            // Middle floors
            correctUv.y = (5.0 / 7.0) + fract(correctUv.y) / 7.0;
        } else {
            // Ground floor
            correctUv.y = (6.0 / 7.0) + fract(correctUv.y) / 7.0;
        }
    }
    if (numFloors == 2) {
        if (uvY <= 1) {
            // Top floor
            correctUv.y = (4.0 / 7.0) + fract(correctUv.y) / 7.0;
        } else {
            // Ground floor
            correctUv.y = (6.0 / 7.0) + fract(correctUv.y) / 7.0;
        }
    }
    if (numFloors == 1) {
        // Only have Ground floor...
        correctUv.y = (6.0 / 7.0) + fract(correctUv.y) / 7.0;
    }
    
    if (uvY < 0) {
        // Roof of the building
        correctUv.x = 1.0 - abs(correctUv.x);
        correctUv.y = abs(fract(IN.uv_map.y) / 7.0) * 4.0;
    }
    
    if (numFloors < 0) {
        // It's not a building at all! Leave the uv_map alone!
        correctUv = IN.uv_map;
    }
    
    
	vec3 finalColour = vec3(0, 0, 0);
    vec4 finalColourGamma = vec4(0, 0, 0, 0);
	vec4 texCol = texture(texture0, correctUv);
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
            vec3 ambient = texLightCol * 0.2;
            vec3 diffuse = texLightCol * lambert * lightSource.intensity;
            vec3 specular = texLightCol * sFactor * lightSource.intensity;
            
            // Calculate rim lighting
            float gamma = 1.0/0.7;
            vec3 rimColour = vec3(0.15, 0.10, 0.05) + (vec3(0.05, 0.05, 0.05) * texLightCol);
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
            finalColourGamma = vec4(pow(finalColour.r, gamma), pow(finalColour.g, gamma), pow(finalColour.b, gamma), texCol.w);
            
            finalColourGamma = mix(fogColour, finalColourGamma, fogFactor);
        }
	}
	gl_FragColor = finalColourGamma;
}