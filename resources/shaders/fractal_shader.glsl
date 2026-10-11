:type vertex
#version 450 core

layout(location = 0) in vec3 a_Position;
layout(location = 1) in vec4 a_Color;
layout(location = 2) in vec2 a_TexCoord;

uniform mat4 u_ViewProjection;

layout(location = 0) out vec4 v_Color;
layout(location = 1) out vec2 v_TexCoord;

void main()
{
    v_Color = a_Color;
    v_TexCoord = a_TexCoord;

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

vec3 palette(float t)
{
    vec3 a = vec3(0.5, 0.5, 0.5);
    vec3 b = vec3(0.5, 0.5, 0.5);
    vec3 c = vec3(1.0, 1.0, 1.0);
    vec3 d = vec3(0.263, 0.416, 0.557);
    return a + b * cos(6.28318 * (c * t + d));
}

void main()
{
    // o_Color = v_Color;
    vec2 res = vec2(u_Width, u_Height);
    vec2 uv = (gl_FragCoord.xy * 2.0 - res) / res.y;
    vec2 uv0 = uv;
    vec3 finalColor = vec3(0.0);

    for (float i = 0.0; i < 4.0; i++)
    {
        uv = fract(uv * 1.5) - 0.5;

        float d = length(uv) * exp(-length(uv0));

        vec3 col = palette(length(uv0) + i*.4 + u_Time*.4);

        d = sin(d*8. + u_Time)/8.;
        d = abs(d);

        d = pow(0.01 / d, 1.2);

        finalColor += col * d;
    }

    o_Color = vec4(finalColor, 1.0);
}
