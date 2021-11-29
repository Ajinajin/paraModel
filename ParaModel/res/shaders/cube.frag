#version 330 core
out vec4 FragColor;
in vec2 TexCoords;

uniform sampler2D ambientMap;

void main(){
	FragColor = texture2D(ambientMap, TexCoords);
	//FragColor = vec4(1, 1, 0.0,0.5);
}
