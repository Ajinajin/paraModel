#version 330 core
out vec4 FragColor;

in vec3 CalcPos;

uniform float centerX;
uniform float centerY;
uniform float centerZ;



void main(){

	float d=(CalcPos[0]-centerX)*(CalcPos[0]-centerX)+(CalcPos[1]-centerY)*(CalcPos[1]-centerY)+(CalcPos[2]-centerZ)*(CalcPos[2]-centerZ);
	
	float distance1=1.0f;
	float distance2=64.0f;
	float distance3=255.0f;
	float distance4=900.0f;
	float distance5=2500.0f;

	
	float tranS=0.5f;
	
	
	if(d<distance1)
		FragColor = vec4(1.0, 0.0, d/distance1, tranS);
	else if(d<distance2)
		FragColor = vec4((d-distance2)/(distance1-distance2), 0.0, 1.0, tranS);
	else if(d<distance3)
		FragColor = vec4(0.0, (d-distance2)/(distance3-distance2), 1.0, tranS);
	else if(d<distance4)
		FragColor = vec4(0.0, 1.0, (d-distance4)/(distance3-distance4), tranS);
	else if(d<distance5)
		FragColor = vec4((d-distance4)/(distance5-distance4), 1.0, 0.0, tranS);
	else
		FragColor = vec4(1.0, 1.0, 1.0, tranS);
}
