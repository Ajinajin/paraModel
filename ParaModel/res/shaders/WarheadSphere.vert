#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 a_offset;

uniform mat4 projection;
uniform mat4 view;
uniform mat4 model;


void main(){
	
	gl_Position = projection * view * model * vec4(aPos + a_offset, 1.0f);
}
