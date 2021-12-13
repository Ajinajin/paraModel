#version 330 core
out vec4 FragColor;

uniform float R;
uniform float G;
uniform float B;


void main(){

	FragColor = vec4( R/255.0, G/255.0, B/255.0, 1.0f);
}
