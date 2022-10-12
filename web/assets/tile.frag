#version 410

uniform sampler2D Texture0;

// PS_0
layout(std140) uniform PSConstants2D
{
	vec4 g_colorAdd;
	vec4 g_sdfParam;
	vec4 g_internal;	
};

//
// PSInput
//
layout(location = 0) in vec4 Color;
layout(location = 1) in vec2 UV;
		
//
// PSOutput
//
layout(location = 0) out vec4 FragColor;
		
void main()
{
	float t = 8.0;
	float r = min(int(texture(Texture0, UV).r * t) / t, 1.0 - 1.0 / t);
	float g = min(int(texture(Texture0, UV).g * t) / t, 1.0 - 1.0 / t);
	float b = min(int(texture(Texture0, UV).b * t) / t, 1.0 - 1.0 / t);
	float a = min(int(texture(Texture0, UV).a * t) / t, 1.0 - 1.0 / t);

	FragColor = vec4(r, g, b, a);
}
