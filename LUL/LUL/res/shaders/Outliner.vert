#shader vertex 
#version 330 core

layout(location = 0) in vec3 vertex;
layout(location = 1) in vec2 texCoord;

uniform mat4 u_Model;
uniform mat4 u_View;
uniform mat4 u_Projection;

void main() 
{
    
	gl_Position = u_Projection * u_View * u_Model * vec4(vertex, 1);
};

#shader fragment
#version 330 core

out vec4 fragColor;

void main()
{	
	fragColor = vec4(0.14, 0.28, 0.46, 1.0);
}
