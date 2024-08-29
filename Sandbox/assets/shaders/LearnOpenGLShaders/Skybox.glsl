#type vertex
#version 330 core
layout(location = 0) in vec3 a_Position;			

uniform mat4 u_Transform;
uniform mat4 u_ViewProjection;

uniform mat4 u_View;
uniform mat4 u_Projection;


out vec3 v_TexCoords;

void main()
{
    v_TexCoords = a_Position;  // Pass the position as texture coordinates
    gl_Position = u_Projection * u_View * vec4(a_Position, 1.0);
}

#type fragment
#version 330 core
out vec4 FragColor;

in vec3 v_TexCoords;

uniform samplerCube skybox;

void main()
{    
    FragColor = texture(skybox, v_TexCoords);
}
