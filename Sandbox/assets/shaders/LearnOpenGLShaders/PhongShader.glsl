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

struct Material {
    vec3 ambient;
    sampler2D diffuse;
    sampler2D specular;
    float shininess;
}; 

struct PointLight {
    vec3 position;

    vec3 ambient;
    vec3 diffuse;
    vec3 specular;

	vec3 attenuationParams;
};

in vec3 v_FragPos;
in vec3 v_Normal;
in vec2 v_TexCoords;	

uniform vec3 u_viewPosition;
uniform PointLight pointLight;
uniform Material material;

void main()
{
	vec3 ambient =  vec3(texture(material.diffuse, v_TexCoords)) * pointLight.ambient; 

	vec3 f_Normal = normalize(v_Normal);
	vec3 lightDir = normalize(pointLight.position - v_FragPos);
	float diff = max(dot(f_Normal, lightDir), 0.0);
	vec3 diffuse = vec3(texture(material.diffuse, v_TexCoords)) * diff *  pointLight.diffuse;

	vec3 viewDir = normalize(u_viewPosition - v_FragPos);
	//  ============= PHONG LIGHTING ============
	// vec3 reflectDir = reflect(-lightDir, f_Normal);
	// float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);
	// ============== BLING PHONG LIGHTING ============
	vec3 halfwayDir = normalize(lightDir + viewDir);
	float spec = pow(max(dot(f_Normal, halfwayDir), 0.0), material.shininess);
	vec3 specular = vec3(texture(material.specular, v_TexCoords)).rrr * spec * pointLight.specular;

	float distance = length(pointLight.position - v_FragPos);
	float attenuation = 1.0 / (pointLight.attenuationParams.x + pointLight.attenuationParams.y * distance + 
					pointLight.attenuationParams.z * (distance * distance));

	color = vec4(attenuation*(ambient + diffuse + specular), 1.0f);
}