#type vertex
#version 450 core

layout(location = 0) in vec3 a_position;
layout(location = 1) in vec4 a_color;
layout(location = 2) in vec2 a_texCoord;
layout(location = 3) in float a_texIndex;
layout(location = 4) in float a_tilingFactor;

uniform mat4 u_viewProjection;

out vec4 v_color;
out vec2 v_texCoord;
out float v_texIndex;
out float v_tilingFactor;

void main()
{
	v_color = a_color;
	v_texCoord = a_texCoord;
	v_texIndex = a_texIndex;
	v_tilingFactor = a_tilingFactor;
	gl_Position = u_viewProjection * vec4(a_position, 1.0);	
}


#type fragment
#version 450 core

layout(location = 0) out vec4 color;

uniform sampler2D u_textures[32];

in vec4 v_color;
in vec2 v_texCoord;
in float v_texIndex;
in float v_tilingFactor;

void main()
{
	// @NOTE: Might not work on every gpu cards, thus changed it to switch statement
	// color = texture(u_textures[int(v_texIndex)], v_texCoord * v_tilingFactor) * v_color;
	vec4 texColor = v_color;
	switch(int(v_texIndex))
	{
	case 0: texColor *=  texture(u_textures[0], v_texCoord * v_tilingFactor); break;
	case 1: texColor *=  texture(u_textures[1], v_texCoord * v_tilingFactor); break;
	case 2: texColor *=  texture(u_textures[2], v_texCoord * v_tilingFactor); break;
	case 3: texColor *=  texture(u_textures[3], v_texCoord * v_tilingFactor); break;
	case 4: texColor *=  texture(u_textures[4], v_texCoord * v_tilingFactor); break;
	case 5: texColor *=  texture(u_textures[5], v_texCoord * v_tilingFactor); break;
	case 6: texColor *=  texture(u_textures[6], v_texCoord * v_tilingFactor); break;
	case 7: texColor *=  texture(u_textures[7], v_texCoord * v_tilingFactor); break;
	case 8: texColor *=  texture(u_textures[8], v_texCoord * v_tilingFactor); break;
	case 9: texColor *=  texture(u_textures[9], v_texCoord * v_tilingFactor); break;
	case 10: texColor *=  texture(u_textures[10], v_texCoord * v_tilingFactor); break;
	case 11: texColor *=  texture(u_textures[11], v_texCoord * v_tilingFactor); break;
	case 12: texColor *=  texture(u_textures[12], v_texCoord * v_tilingFactor); break;
	case 13: texColor *=  texture(u_textures[13], v_texCoord * v_tilingFactor); break;
	case 14: texColor *=  texture(u_textures[14], v_texCoord * v_tilingFactor); break;
	case 15: texColor *=  texture(u_textures[15], v_texCoord * v_tilingFactor); break;
	case 16: texColor *=  texture(u_textures[16], v_texCoord * v_tilingFactor); break;
	case 17: texColor *=  texture(u_textures[17], v_texCoord * v_tilingFactor); break;
	case 18: texColor *=  texture(u_textures[18], v_texCoord * v_tilingFactor); break;
	case 19: texColor *=  texture(u_textures[19], v_texCoord * v_tilingFactor); break;
	case 20: texColor *=  texture(u_textures[20], v_texCoord * v_tilingFactor); break;
	case 21: texColor *=  texture(u_textures[21], v_texCoord * v_tilingFactor); break;
	case 22: texColor *=  texture(u_textures[22], v_texCoord * v_tilingFactor); break;
	case 23: texColor *=  texture(u_textures[23], v_texCoord * v_tilingFactor); break;
	case 24: texColor *=  texture(u_textures[24], v_texCoord * v_tilingFactor); break;
	case 25: texColor *=  texture(u_textures[25], v_texCoord * v_tilingFactor); break;
	case 26: texColor *=  texture(u_textures[26], v_texCoord * v_tilingFactor); break;
	case 27: texColor *=  texture(u_textures[27], v_texCoord * v_tilingFactor); break;
	case 28: texColor *=  texture(u_textures[28], v_texCoord * v_tilingFactor); break;
	case 29: texColor *=  texture(u_textures[29], v_texCoord * v_tilingFactor); break;
	case 30: texColor *=  texture(u_textures[30], v_texCoord * v_tilingFactor); break;
	case 31: texColor *=  texture(u_textures[31], v_texCoord * v_tilingFactor); break;
	}
	color = texColor;
}