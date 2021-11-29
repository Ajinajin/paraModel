#version 330 core

in vec3 CalcPos;
in vec3 fragColor;
in vec2 TexCoords;

out vec4 FragColor;

uniform sampler2D ambientMap;
uniform float tran;

uniform int missileNum;
const int size = 10;

uniform float Radius[size];

uniform float inputsCenters[size * 3];


void main(){
	vec3 realColor;

	int index;
	int hitFlag=0;

	for(index=0;index<size;index++)
	{
		if((inputsCenters[index*3+0]!=-999)&&(Radius[0]!=-1))
		{
			float d=(CalcPos[0]-inputsCenters[index*3+0])*(CalcPos[0]-inputsCenters[index*3+0])+(CalcPos[1]-inputsCenters[index*3+1])*(CalcPos[1]-inputsCenters[index*3+1])+(CalcPos[2]-inputsCenters[index*3+2])*(CalcPos[2]-inputsCenters[index*3+2]);
		
			if(d < (Radius[0] * Radius[0]))
			{
				hitFlag=1;
			}
		}
	}

	if(hitFlag == 0)
	{
		realColor = vec3(1.0f,1.0f,1.0f);
	}
	else
	{
		realColor = fragColor;
	}



	vec4 texMask = texture2D(ambientMap, TexCoords);
	vec4 colorMask = vec4(realColor,tran);
	
	FragColor = mix(colorMask,texMask,0.5);
}
