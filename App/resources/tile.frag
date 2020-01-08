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
	float level = int(texture(Texture0, UV).r * 10.0) / 10.0;

	FragColor = vec4(vec3(0.593, 0.980, 0.593) * level, 1.0);
}
