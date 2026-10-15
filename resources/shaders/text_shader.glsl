:type vertex
#version 450 core

layout(location = 0) in vec3 a_Position;
layout(location = 1) in vec4 a_Color;
layout(location = 2) in vec2 a_TexCoord;
layout(location = 3) in float a_TexIndex;

uniform mat4 u_ViewProjection;

struct VertexOutput
{
	vec4 Color;
	vec2 TexCoord;
};

layout(location = 0) out VertexOutput Output;
layout(location = 2) out float v_TexIndex;

void main()
{
	Output.Color = a_Color;
	Output.TexCoord = a_TexCoord;
	v_TexIndex = a_TexIndex;

	gl_Position = u_ViewProjection * vec4(a_Position, 1.0);
}

:type fragment
#version 450 core

layout(location = 0) out vec4 o_Color;

struct VertexOutput
{
    vec4 Color;
    vec2 TexCoord;
};

layout(location = 0) in VertexOutput Input;
layout(location = 2) in float v_TexIndex;

uniform sampler2D u_Textures[32];

float median(float r, float g, float b)
{
    return max(min(r, g), min(max(r, g), b));
}

float screenPxRange()
{
    vec2 unitRange = vec2(2.0) / vec2(textureSize(u_Textures[int(v_TexIndex)], 0));

    vec2 screenTexSize = 1.0 / fwidth(Input.TexCoord);

    return max(0.5 * dot(unitRange, screenTexSize), 1.0);
}

void main()
{
    vec3 msd = texture( u_Textures[int(v_TexIndex)], Input.TexCoord).rgb;

    float sd = median(msd.r, msd.g, msd.b);

    float screenPxDistance = screenPxRange() * (sd - 0.5);
    float coverage = clamp(screenPxDistance + 0.5, 0.0, 1.0);

    o_Color = vec4(Input.Color.rgb, Input.Color.a * coverage);
}

