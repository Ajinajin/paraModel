#version 330 core
out vec4 FragColor;

uniform float tranS;

uniform float R;
uniform float G;
uniform float B;

uniform int if_sectionalMode;

in vec3 pos;

void main(){

		
		
		
	if(if_sectionalMode==0)
	{
		FragColor = vec4( R/255.0, G/255.0, B/255.0, tranS);
	}
	else
	{
		
		if(pos[2]<0)
		{
			FragColor = vec4( R/255.0, G/255.0, B/255.0, tranS);
		}
		else
		{
			discard;
		}
		
	}
	
}
