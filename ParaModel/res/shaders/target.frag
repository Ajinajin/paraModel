#version 330 core

in vec3 fragColor;
in vec2 TexCoords;

out vec4 FragColor;

uniform sampler2D ambientMap;
uniform float tranS;

void main(){
	int j=0;
	
	vec4 texMask = texture2D(ambientMap, TexCoords);
	vec4 colorMask = vec4(fragColor,tranS);
	
	FragColor = mix(colorMask,texMask,0.15);
}
