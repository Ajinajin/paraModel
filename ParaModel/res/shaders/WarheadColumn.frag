#version 330 core
out vec4 FragColor;

uniform float tranS;

void main(){

	//FragColor = vec4(250.0/255.0, 128.0/255.0, 114.0/255.0, tranS);
	FragColor = vec4(0, 0, 1, tranS);
}
