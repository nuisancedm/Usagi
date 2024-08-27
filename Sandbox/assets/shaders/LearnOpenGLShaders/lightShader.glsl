#type vertex
#version 330 core
layout(location = 0) in vec3 a_Position;		
layout(location = 1) in vec3 a_Normal;		
layout(location = 2) in vec3 a_Tangent;		
layout(location = 3) in vec3 a_Binormal;		
layout(location = 4) in vec2 a_TexCoords;		
			
uniform mat4 u_Transform;
uniform mat4 u_ViewProjection;

out vec3 v_Position;
					
void main()
{
	v_Position = a_Position;
	gl_Position = u_ViewProjection * u_Transform * vec4(v_Position, 1.0);
}

#type fragment
#version 330 core
out vec4 color;

uniform vec3 u_lightColor;

void main()
{
	color = vec4(u_lightColor,1.0f);
}