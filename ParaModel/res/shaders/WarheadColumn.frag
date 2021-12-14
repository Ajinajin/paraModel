#version 330 core
out vec4 FragColor;

uniform float tranS;

uniform float R;
uniform float G;
uniform float B;


void main(){

	//FragColor = vec4(250.0/255.0, 128.0/255.0, 114.0/255.0, tranS);
	FragColor = vec4( R/255.0, G/255.0, B/255.0, tranS);
}
