#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aEnd;

uniform mat4 projection;
uniform mat4 view;
uniform mat4 model;
out vec3 CalcPos;


uniform float fStep;
uniform float centerX;
uniform float centerY;
uniform float centerZ;

out float centerX2;
out float centerY2;
out float centerZ2;

void main(){
	vec3 center = vec3(centerX,centerY,centerZ);
	vec3 aPos1 = center + fStep * (aPos - center);
	
	gl_Position = projection * view * model * vec4(aPos1, 1.0f);
	CalcPos = aPos1;
	
	centerX2 = centerX;
	centerY2 = centerY;
	centerZ2 = centerZ;
}
