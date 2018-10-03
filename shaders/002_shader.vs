#version 330 core
layout (location = 0) in vec3 pos;
layout (location = 1) in vec3 col; //from vertex attribute

out vec3 ourColor; //for fragment

uniform float xPos;		//uniform simulate to global, can set by CPU for GPU
uniform float yPos;		//uniform simulate to global, can set by CPU for GPU

out vec3 outColor;

void main()
{
	gl_Position = vec4(pos.x + xPos, pos.y + yPos, pos.z, 1.0f);
	//gl_Position = vec4(pos.x + xPos, -pos.y + yPos, pos.z, 1.0f);

	ourColor = col;
	outColor = pos; //make outColor equals pos
}