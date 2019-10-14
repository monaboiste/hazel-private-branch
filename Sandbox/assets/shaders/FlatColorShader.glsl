#type vertex
#version 330 core

layout(location = 0) in vec3 a_position;

uniform mat4 u_viewProjection;
uniform mat4 u_transform;

out vec3 v_position;

void main()
{
	gl_Position = u_viewProjection * u_transform * vec4(a_position, 1.0);	
}



#type fragment
#version 330 core

layout(location = 0) out vec4 color;

uniform vec4 u_color;

void main()
{
	color = u_color;
}