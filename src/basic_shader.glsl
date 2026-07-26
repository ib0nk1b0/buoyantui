:type vertex
#version 450 core

layout(location = 0) in vec3 a_Position;
layout(location = 1) in vec4 a_Color;
layout(location = 2) in vec2 a_TexCoord;
layout(location = 3) in float a_TexIndex;

uniform mat4 u_ViewProjection;

layout(location = 0) out vec4 v_Color;
layout(location = 1) out vec2 v_TexCoord;
layout(location = 2) out float v_TexIndex;

void main()
{
    v_Color = a_Color;
    v_TexCoord = a_TexCoord;
    v_TexIndex = a_TexIndex;

    gl_Position = u_ViewProjection * vec4(a_Position, 1.0);
}

:type fragment 
#version 450 core

layout(location = 0) out vec4 o_Color;

uniform float u_Width;
uniform float u_Height;
uniform float u_Time;

layout(location = 0) in vec4 v_Color;
layout(location = 1) in vec2 v_TexCoord;
layout(location = 2) in float v_TexIndex;

uniform sampler2D u_Textures[32];

void main()
{
	vec4 texColor = v_Color;
    float scale = 1.0f;
    texColor *= texture(u_Textures[int(v_TexIndex)], v_TexCoord * scale);
    
	o_Color = texColor;
}
