#version 330 core
in vec2 texCoords;
out vec4 fragment_color;
uniform sampler2D textureSampler;
uniform float transparency;
void main()
{
	vec4 texColor = texture(textureSampler, texCoords);
	fragment_color = vec4(texColor.rgb, texColor.a * transparency);
}