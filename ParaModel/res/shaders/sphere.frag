#version 330 core
out vec4 FragColor;

in vec3 CalcPos;

uniform float tranS;

in float centerX2;
in float centerY2;
in float centerZ2;

void main(){

	//FragColor = vec4(1,0.41176, 0.70588, tranS);
	FragColor = vec4(0.34117,0.98039, 1, tranS);
}
