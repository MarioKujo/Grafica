#version 330 core
in vec2 texCoords;
in vec3 fragNormal;
in vec3 fragPos;
out vec4 fragment_color;
uniform sampler2D textureSampler;

#define NUM_LIGHTS 3
uniform vec3 lightDirections[NUM_LIGHTS];
uniform vec3 lightColors[NUM_LIGHTS];
uniform float lightIntensities[NUM_LIGHTS];

void main()
{
	vec4 texColor = texture(textureSampler, texCoords);
	vec3 norm = normalize(fragNormal);
	vec3 totalDiffuse = vec3(0.0);
	for (int i = 0; i < NUM_LIGHTS; ++i)
	{
		vec3 lightDir = normalize(-lightDirections[i]);
		float diff = max(dot(norm, lightDir), 0.0);
		totalDiffuse += diff * lightColors[i] * lightIntensities[i];
	}
	vec3 result = texColor.rgb * totalDiffuse;
	fragment_color = vec4(result, texColor.a);
}