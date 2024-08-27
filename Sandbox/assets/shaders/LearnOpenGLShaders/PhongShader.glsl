#type vertex
#version 330 core
layout(location = 0) in vec3 a_Position;		
layout(location = 1) in vec3 a_Normal;		
layout(location = 2) in vec3 a_Tangent;		
layout(location = 3) in vec3 a_Binormal;		
layout(location = 4) in vec2 a_TexCoords;		
			
uniform mat4 u_Transform;
uniform mat4 u_ViewProjection;
uniform mat3 u_NormalMatrix;

out vec3 v_FragPos;
out vec3 v_Normal;
out vec2 v_TexCoords;
					
void main()
{
	vec3 v_Position = a_Position;
	v_Normal = u_NormalMatrix * a_Normal;
	v_TexCoords = a_TexCoords;
	v_FragPos = vec3(u_Transform * vec4(v_Position,1.0f));

	gl_Position = u_ViewProjection * u_Transform * vec4(v_Position,1.0f);
}

#type fragment
#version 330 core
out vec4 color;

in vec3 v_FragPos;
in vec3 v_Normal;
in vec2 v_TexCoords;	

uniform vec3 u_lightColor;
uniform vec3 u_lightPosition;
uniform vec3 u_viewPosition;

uniform sampler2D u_Texture;

void main()
{
	float ambientStrength = 0.1;
	vec3 ambient =  ambientStrength * u_lightColor; 

	vec3 f_Normal = normalize(v_Normal);
	vec3 lightDir = normalize(u_lightPosition - v_FragPos);
	float diff = max(dot(f_Normal, lightDir), 0.0);
	vec3 diffuse = diff * u_lightColor;

	float specularStrength = 0.5;
	vec3 viewDir = normalize(u_viewPosition - v_FragPos);
	vec3 reflectDir = reflect(-lightDir, f_Normal);
	float spec = pow(max(dot(viewDir, reflectDir), 0.0), 128);
	vec3 specular = specularStrength * spec * u_lightColor;

	color = texture(u_Texture, v_TexCoords) * vec4(ambient + diffuse + specular, 1.0f);

	// DEBUG
	// color = texture(u_Texture, v_TexCoords);
}