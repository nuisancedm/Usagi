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
out vec2 v_TexCoords;
					
void main()
{
	v_Position = a_Position;
	v_TexCoords = a_TexCoords;
	gl_Position = u_ViewProjection * u_Transform * vec4(v_Position, 1.0);
}

#type fragment
#version 330 core
out vec4 color;

in vec2 v_TexCoords;	
uniform sampler2D u_Texture;

void main()
{
	// color = texture(u_Texture, v_TexCoords);
	color = vec4(v_TexCoords, 0.0, 1.0);
}